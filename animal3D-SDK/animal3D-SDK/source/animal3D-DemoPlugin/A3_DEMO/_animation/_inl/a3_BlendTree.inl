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

	// allocate temp operation data structures
	const a3ui32 NUM_TEMPS = 1;

	a3ui32 outPoseSize = sizeof(a3_HierarchyPose) * blend_out->nodeCount;									// for blend_out->poses
	a3ui32 outPosePoseSize = sizeof(a3_SpatialPose) * blend_out->nodeCount * hierarchy->numNodes; 			// for blend_out->poses' poses
	a3ui32 nodesSize = sizeof(a3_SpatialPose) * blend_out->nodeCount * hierarchy->numNodes * POSE_IN_MAX; 	// for blend node input poses
	a3ui32 sPoseOpsSize = sizeof(a3_SpatialPoseBlendOp) * NUM_TEMP_STRUCTS; 								// sposeOps
	a3ui32 sPoseOpsOutSize = sizeof(a3_SpatialPose) * NUM_TEMP_STRUCTS; 									// outPoses for sposeOps
	a3ui32 hPoseOpsSize = sizeof(a3_HierarchyPoseBlendOp) * NUM_TEMPS; 										// hposeOps
	a3ui32 hPoseOpsOutSize = sizeof(a3_HierarchyPose) * NUM_TEMPS; 											// outPoses for hposeOps
	a3ui32 hPoseOpsOutSPoseSize = sizeof(a3_SpatialPose) * hierarchy->numNodes * NUM_TEMPS; 				// spatial poses for hposeOps' outPoses
	a3ui32 ikOpsSize = sizeof(a3_HierarchyStateBlendOp) * NUM_TEMPS;										// ikOps

	// counts of each struct to be used in pointer arithmatic
	a3ui32 outPoseCount = blend_out->nodeCount;																// for blend_out->poses (these are hpose)
	a3ui32 outPosePoseCount = blend_out->nodeCount * hierarchy->numNodes; 									// for blend_out->poses' poses (these are spose)
	a3ui32 nodesCount = blend_out->nodeCount * hierarchy->numNodes * POSE_IN_MAX; 							// for blend node input poses
	a3ui32 sPoseOpsCount = NUM_TEMP_STRUCTS; 																// sposeOps
	a3ui32 sPoseOpsOutCount = NUM_TEMP_STRUCTS; 															// outPoses for sposeOps
	a3ui32 hPoseOpsCount = NUM_TEMPS; 																		// hposeOps
	a3ui32 hPoseOpsOutCount = NUM_TEMPS; 																	// outPoses for hposeOps
	a3ui32 hPoseOpsOutSPoseCount = hierarchy->numNodes * NUM_TEMPS; 										// spatial poses for hposeOps' outPoses
	a3ui32 ikOpsCount = NUM_TEMPS;																			// ikOps

	// ONE MALLOC
	a3ui32 memreq = outPoseSize +						// for blend_out->poses
					outPosePoseSize +					// for blend_out->poses' poses
					nodesSize +							// for blend node input poses
					sPoseOpsSize +						// sposeOps
					sPoseOpsOutSize +					// outPoses for sposeOps
					hPoseOpsSize +						// hposeOps
					hPoseOpsOutSize +					// outPoses for hposeOps
					hPoseOpsOutSPoseSize +				// spatial poses for hposeOps' outPoses
					ikOpsSize;							// ikOps

	/*
	T *p;
	struct BigStruct_t *item = new BigStruct_t[256];

	item[1];

	item + 0      item + 1	    item + 2
	item[0]       item[1]	    item[2]
	v	          v		        v
	+-------------+-------------+-------------+
	|sizeof(*item)|

	p[5] === *(p + 5)

	p[i] === *(p + i)
	*/

	// malloc all data
	blend_out->poses = (a3_HierarchyPose*)malloc(memreq);

	// assign poses' spatial poses
	blend_out->poses->pose = (a3_SpatialPose*)blend_out->poses + outPoseCount; //Want to be using count not size - EVERYWHERE

	// Resets pose data with identity matrix
	for (a3ui32 i = 0; i < outPoseCount; ++i) {
		blend_out->poses[i].pose = blend_out->poses[0].pose + hierarchy->numNodes * i;
		a3hierarchyPoseReset(&blend_out->poses[i], outPoseCount, NULL, NULL);
		blend_out->nodes[i].outPose = &blend_out->poses[i];
	}

	// allocate blend node input poses
	//blend_out->nodes[0].inputPoses[0].pose = (a3_SpatialPose*)blend_out->poses->pose + (POSE_IN_MAX * outPoseCount); // needs fix
	//for (a3ui32 i = 0; i < POSE_IN_MAX; ++i) {
	//	blend_out->nodes[i].inputPoses[j].pose = (a3_SpatialPose*)blend_out->poses->pose + (j * i); // needs fix

	//}

	// allocate blend node input poses
	for (a3ui32 i = 0; i < POSE_IN_MAX; ++i) // for pose
	{
		// assign pointers for this pose's spatial poses
		blend_out->nodes[i].inputPoses->pose = (a3_SpatialPose*)blend_out->poses + outPosePoseCount + i; // needs fix
	}

	// assign sposeOps
	blend_out->sposeOps = (a3_SpatialPoseBlendOp*)(blend_out->nodes + nodesCount); // possibly needs fix

	// assign pointers to sposeOps outPoses
	for (a3ui32 i = 0; i < sPoseOpsCount; i++)
	{
		blend_out->sposeOps[i].pose_out = (a3_SpatialPose*)(blend_out->sposeOps + i); // possibly needs fix
		blend_out->sposeOps[i].pose_out->transformMat = a3mat4_identity;
		blend_out->sposeOps[i].pose_out->rotate = a3vec4_zero;
		blend_out->sposeOps[i].pose_out->scale = a3vec4_one;
		blend_out->sposeOps[i].pose_out->translate = a3vec4_zero;
	}

	// assign pointers to hposeOps
	blend_out->hposeOps = (a3_HierarchyPoseBlendOp*)(blend_out->sposeOps->pose_out + sPoseOpsOutCount);

	// assign outPoses of hposeOps
	for (a3ui32 i = 0; i < NUM_TEMPS; i++)
	{
		blend_out->hposeOps[i].pose_out = (a3_HierarchyPose*)(blend_out->hposeOps + hPoseOpsCount); // possibly needs fix
	}

	// assign spatial poses for hposeOps' outPoses
	for (a3ui32 i = 0; i < NUM_TEMPS; i++)
	{
		blend_out->hposeOps[i].pose_out->pose = (a3_SpatialPose*)(blend_out->hposeOps->pose_out + hPoseOpsOutCount * i); // possibly needs fix

		// zero out all data for these spatial poses
		a3hierarchyPoseReset(blend_out->hposeOps[i].pose_out, hierarchy->numNodes, NULL, NULL);
	}

	// assign ikOps
	blend_out->ikOps = (a3_HierarchyStateBlendOp*)(blend_out->hposeOps->pose_out->pose + hPoseOpsOutSPoseCount);

	// done
	return 0;
}

inline a3ret a3freeBlendTree(a3_BlendTree* blend_in)
{
	// free temp operation data structures
	free(blend_in->poses);
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
			if (blendTree->nodes[i].myClipController) {
				// get the clip controller
		// copy the pose from the clip controller to the node's out pose
				a3hierarchyPoseCopy(blendTree->nodes[i].outPose,
					hierarchyPoseGroup_skel->hpose + blendTree->nodes[i].myClipController->keyframeIndex, // get deltas of a pose in frame keyframeIndex
					hierarchy_skel->numNodes);
			}

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
	a3_HierarchyPose*  inPosePtr = node->inputPoses;

	// if we rely on any additional inputs...
	if (node->numInputs > 0)
	{
		// for each input pose that still needs to be solved...
		for (a3ui32 i = 0; i < numOfInputs; i++)
		{
			// allocate space for the input poses
			node->inputPoses[i].pose = node->inputPoses[0].pose + heierarchy->numNodes * i;
			a3hierarchyPoseReset(&node->inputPoses[i], heierarchy->numNodes, NULL, NULL);


			// if there is an input node...
			if (node->inputNodes[i]) {
				// recurse solve the tree 
				a3executeBlendTree(tree, node->inputNodes[i], node->inputNodes[i]->numInputs, blendNodeCount, heierarchy); //calc children blend nodes
				a3hierarchyPoseCopy(&node->inputPoses[i], node->inputNodes[i]->outPose, heierarchy->numNodes); // copy children blend into temp data to be operated on
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
		// pick the right struct based on node type,
		// and pass the right struct to the operation
		//	 a3_SpatialPoseBlendOp
		//	 a3_HierarchyStateBlendOp
		//	 a3_HierarchyPoseBlendOp
		switch (node->opType)
		{
		case 1:		// spose
		{
			tree->sposeOps[0].pose_in[0] = inPosePtr->pose;
			tree->sposeOps[0].param[0] = node->opParams;

			node->poseOp(&tree->sposeOps[0], tree);
			node->outPose->pose = tree->sposeOps[0].pose_out;
			break;
		}
		case 2:		//IK_SOLVER
			tree->ikOps[0].pose_in[0] = inPosePtr;
			tree->ikOps[0].param_in = node->opParams;

			node->poseOp(&tree->ikOps[0], tree);
			node->outPose = tree->ikOps[0].pose_out;
			break;
		case 5:		// hpose
			//tree->hposeOps[0].pose_out = node->outPose;
			tree->hposeOps[0].pose_in[0] = &inPosePtr[0];
			tree->hposeOps[0].pose_in[1] = &inPosePtr[1];
			tree->hposeOps[0].param_in = node->opParams;
			tree->hposeOps[0].nodeCount = heierarchy->numNodes;

			node->poseOp(&tree->hposeOps[0], tree);
			node->outPose = tree->hposeOps[0].pose_out;
			break;
		default:
			break;
		}

		//node->poseOp(node->outPose, heierarchy->numNodes, inPosePtr, node->opParams);
	}
	else if(node->numInputs > 0) {
		a3hierarchyPoseCopy( node->outPose, &node->inputPoses[0], heierarchy->numNodes); // copy children blend into temp data to be operated on
	}
	//else// no operations on this node, so just copy the in pose into the out pose.
	//a3hierarchyPoseCopy(node->outPose, &inPoses[0], heierarchy->numNodes); // take the in pose directly

	// mask the animation
	a3maskNode(node);

	return node->outPose;
}

// ---------------------------------------------------------------------------------

#endif	// !__ANIMAL3D_BLENDTREE_INL
#endif	// __ANIMAL3D_BLENDTREE_H