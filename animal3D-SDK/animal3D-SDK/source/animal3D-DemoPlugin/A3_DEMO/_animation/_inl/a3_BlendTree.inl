#include <stdlib.h>


#ifdef __ANIMAL3D_BLENDTREE_H
#ifndef __ANIMAL3D_BLENDTREE_INL
#define __ANIMAL3D_BLENDTREE_INL

// ---------------------------------------------------------------------------------

inline a3ui32 a3maskNode(a3_BlendTreeNode* node_inout)
{
	// loop through each a3_SpatialPose in maskBones
	for (a3ui32 i = 0; i < node_inout->numMaskBones; i++)
	{
		// set the pose to identity
		a3spatialPoseReset(node_inout->outPose->pose + node_inout->baskBoneIndices[i]);
	}

	return 0;
}

inline a3ret a3initBlendTree(a3_BlendTree* blend_out, a3ui32 nodeCount, a3_Hierarchy* hierarchy)
{
	blend_out->nodeCount = nodeCount;
	blend_out->poses = (a3_HierarchyPose*)malloc(sizeof(a3_HierarchyPose) * blend_out->nodeCount);

	blend_out->poses->pose = (a3_SpatialPose*)malloc(sizeof(a3_SpatialPose) * blend_out->nodeCount * hierarchy->numNodes); 		// set up nodes to be use with outpose's

	// Resets pose data with identity matrix
	for (a3ui32 i = 0; i < blend_out->nodeCount; ++i) {
		blend_out->poses[i].pose = blend_out->poses[0].pose + hierarchy->numNodes * i;
		a3hierarchyPoseReset(&blend_out->poses[i], blend_out->nodeCount, NULL, NULL);
		blend_out->nodes[i].outPose = &blend_out->poses[i];
	}

	// allocate temp operation data structures
	const a3ui32 NUM_TEMPS = 1;
	a3ui32 memreq = sizeof(a3_SpatialPoseBlendOp) * NUM_TEMP_STRUCTS +			// sposeOps
					sizeof(a3_SpatialPose) * NUM_TEMP_STRUCTS +					// outPoses for sposeOps
					sizeof(a3_HierarchyPoseBlendOp) * NUM_TEMPS +				// hposeOps
					sizeof(a3_HierarchyPose) * NUM_TEMPS +						// outPoses for hposeOps
					sizeof(a3_SpatialPose) * hierarchy->numNodes * NUM_TEMPS +	// spatial poses for hposeOps' outPoses
					sizeof(a3_HierarchyStateBlendOp) * NUM_TEMPS;				// ikOps

	// malloc all data
	blend_out->sposeOps = (a3_SpatialPoseBlendOp*)malloc(memreq);

	// assign pointers to sposeOps outPoses
	a3_SpatialPoseBlendOp* ptr = (blend_out->sposeOps + sizeof(a3_SpatialPoseBlendOp) * NUM_TEMP_STRUCTS); // cache where the hposeOps end
	for (a3ui32 i = 0; i < NUM_TEMP_STRUCTS; i++)
	{
		blend_out->sposeOps[i].pose_out = (a3_SpatialPose*)(ptr + sizeof(a3_SpatialPose) * i);
		blend_out->sposeOps[i].pose_out->transformMat = a3mat4_identity;
		blend_out->sposeOps[i].pose_out->rotate = a3vec4_zero;
		blend_out->sposeOps[i].pose_out->scale = a3vec4_one;
		blend_out->sposeOps[i].pose_out->translate = a3vec4_zero;
	}

	// assign pointers to hposeOps outPoses
	ptr += sizeof(a3_SpatialPose) * NUM_TEMP_STRUCTS; // cache where the hposeOps begin
	blend_out->hposeOps = (a3_HierarchyPoseBlendOp*)ptr;

	// assign outPoses of hposeOps
	ptr += sizeof(a3_HierarchyPoseBlendOp) * NUM_TEMPS;
	for (a3ui32 i = 0; i < NUM_TEMPS; i++)
	{
		blend_out->hposeOps[i].pose_out = (a3_HierarchyPose*)(ptr + sizeof(a3_HierarchyPose) * i);
	}

	// assign spatial poses for hposeOps' outPoses
	ptr += sizeof(a3_HierarchyPose) * NUM_TEMPS; // cache where the hposeOps' outPoses begin
	for (a3ui32 i = 0; i < NUM_TEMPS; i++)
	{
		// assign spatial poses for hposeOps' outPoses
		for (a3ui32 j = 0; j < hierarchy->numNodes; j++)
		{
			blend_out->hposeOps[i].pose_out->pose = (a3_SpatialPose*)(ptr + sizeof(a3_SpatialPose) * hierarchy->numNodes * i);
		}

		// zero out all data for these spatial poses
		a3hierarchyPoseReset(blend_out->hposeOps[i].pose_out, hierarchy->numNodes, NULL, NULL);
	}

	// assign ikOps
	ptr += sizeof(a3_SpatialPose) * hierarchy->numNodes * NUM_TEMPS; // cache where the ikOps begin
	blend_out->ikOps = (a3_HierarchyStateBlendOp*)ptr;
	return 0;
}

inline a3ret a3freeBlendTree(a3_BlendTree* blend_in)
{
	free(blend_in->poses->pose);
	free(blend_in->poses);

	// free temp operation data structures
	free(blend_in->sposeOps);
}

inline a3ret a3initBlendNode(a3_BlendTreeNode* node_out, a3_BlendTree* blendTree, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
}

inline a3ret a3initBlendFunction(a3_BlendTreeNode* node_out, a3_BlendTree* blendTree, a3_HierarchyPoseOp poseOp, a3_BlendTreeNode* inputNodes[])
{
}

inline a3ret a3freeBlendNode(a3_BlendTreeNode* node_in)
{
}

inline a3ret a3maskBlendNode(a3_BlendTreeNode* node_out, a3ui32 maskindecies1[128])
{
}

// TODO: wrap blend tree, hierarchyPoseGroup_skel, and hierarchy_skel in a struct and pass that instead
inline a3ret a3updateBlendTree(a3_BlendTree* blendTree, a3_HierarchyPoseGroup const* hierarchyPoseGroup_skel, a3_Hierarchy const* hierarchy_skel, const a3real dt)
{
	// Update clip controllers in the blend tree
	for (a3ui32 i = 0; i < blendTree->clipCount; i++)
	{
		a3clipControllerUpdate(&blendTree->clipControllers[i], dt);
	}

	/*
		first update the nodes which have no inputs and just sample from a clip
		for each node in the blend tree...
	*/
	for (a3ui32 i = 0; i < blendTree->nodeCount; i++)
	{
		// if the node is a clip node...
		if (blendTree->nodes[i].numInputs <= 0)
		{
			// get the clip controller
			// copy the pose from the clip controller to the node's out pose
			a3hierarchyPoseCopy(blendTree->nodes[i].outPose,
				hierarchyPoseGroup_skel->hpose + blendTree->nodes[i].myClipController->keyframeIndex, // get deltas of a pose in frame keyframeIndex
				hierarchy_skel->numNodes);
		}
	}
	/*
		finally execute the nodes of the blend tree in order
	*/
	const a3ui32 rootIndex = 0; // note: root index is assumed to be zero
	a3executeBlendTree(blendTree, &blendTree->nodes[rootIndex], blendTree->nodes[rootIndex].numInputs, blendTree->nodeCount, hierarchy_skel);

	return 0;
}

inline a3_HierarchyPose* a3executeBlendTree(a3_BlendTree* tree, a3_BlendTreeNode* node, const a3ui32 numOfInputs, const a3ui32 blendNodeCount, const a3_Hierarchy* heierarchy)
{
	// create an array of all input poses
	a3_HierarchyPose inPoses[8];
	a3_HierarchyPose* inPosePtr; // 8 max inputs
	inPoses[0].pose = (a3_SpatialPose*)malloc(sizeof(a3_SpatialPose) * heierarchy->numNodes * numOfInputs);
	inPosePtr = inPoses;

	// if we rely on any additional inputs...
	if (node->numInputs > 0)
	{
		// for each input pose that still needs to be solved...
		for (a3ui32 i = 0; i < numOfInputs; i++)
		{
			// allocate space for the input poses
			inPoses[i].pose = inPoses[0].pose + heierarchy->numNodes * i;
			a3hierarchyPoseReset(&inPoses[i], heierarchy->numNodes, NULL, NULL);


			// if there is an input node...
			if (node->inputNodes[i]) {
				// recurse solve the tree 
				a3executeBlendTree(tree, node->inputNodes[i], node->inputNodes[i]->numInputs, blendNodeCount, heierarchy); //calc children blend nodes
				a3hierarchyPoseCopy(&inPoses[i], node->inputNodes[i]->outPose, heierarchy->numNodes); // copy children blend into temp data to be operated on

			}

		}
	}
	else // if we don't rely on inputs and just need to sample a clip...
	{
		// nothing to do here - stop recursing
	}

	//operate on all inputs
	if (node->poseOp != 0)
	{
		// TODO: pick the right struct based on node type,
		//       and pass the right struct to the operation
		//	 a3_SpatialPoseBlendOp
			//a3_HierarchyStateBlendOp
			//a3_HierarchyPoseBlendOp
		switch (node->opType)
		{
		case 1:		// spose
		{
			tree->sposeOps[0].pose_out = node->outPose->pose;
			tree->sposeOps[0].pose_in[0] = inPosePtr->pose;
			tree->sposeOps[0].param[0] = node->opParams;

			node->poseOp(&tree->sposeOps[0], tree);
			break;
		}
		case 2:		//IK_SOLVER
			tree->ikOps[0].pose_out = node->outPose;
			tree->ikOps[0].pose_in[0] = inPosePtr;
			tree->ikOps[0].param_in = node->opParams;

			node->poseOp(&tree->ikOps[0], tree);
			break;
		case 5:		// hpose
			tree->hposeOps[0].pose_out = node->outPose;
			tree->hposeOps[0].pose_in[0] = inPosePtr;
			tree->hposeOps[0].param_in = node->opParams;

			node->poseOp(&tree->hposeOps[0], tree);
			break;
		default:
			break;
		}

		//node->poseOp(node->outPose, heierarchy->numNodes, inPosePtr, node->opParams);
	}
	else if(node->numInputs > 0) {
		a3hierarchyPoseCopy( node->outPose, &inPoses[0], heierarchy->numNodes); // copy children blend into temp data to be operated on
	}
	//else// no operations on this node, so just copy the in pose into the out pose.
	//a3hierarchyPoseCopy(node->outPose, &inPoses[0], heierarchy->numNodes); // take the in pose directly

	free(inPoses[0].pose);

	// mask the animation
	a3maskNode(node);

	return node->outPose;
}

// ---------------------------------------------------------------------------------

#endif	// !__ANIMAL3D_BLENDTREE_INL
#endif	// __ANIMAL3D_BLENDTREE_H