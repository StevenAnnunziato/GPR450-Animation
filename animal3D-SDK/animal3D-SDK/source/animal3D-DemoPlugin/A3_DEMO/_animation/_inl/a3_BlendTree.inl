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

inline a3ret a3initBlendTree(a3_BlendTree* blend_out, a3ui32 nodeCount, a3ui32 clipCount, a3ui32 hierarchyNodes)
{
	blend_out->nodeCount = 7;
	blend_out->clipCount = 4;
	blend_out->poses = malloc(sizeof(a3_HierarchyPose) * blend_out->nodeCount);

	blend_out->poses->pose = malloc(sizeof(a3_SpatialPose) * blend_out->nodeCount * hierarchyNodes); 		// set up nodes to be use with outpose's

	// Resets pose data with identity matrix
	for (a3ui32 i = 0; i < blend_out->nodeCount; ++i) {
		blend_out->poses[i].pose = blend_out->poses[0].pose + hierarchyNodes * i;
		a3hierarchyPoseReset(&blend_out->poses[i], blend_out->nodeCount, NULL, NULL);
		blend_out->nodes[i].outPose = &blend_out->poses[i];
	}
}

inline a3ret a3freeBlendTree(a3_BlendTree* blend_in)
{
	free(blend_in->poses->pose);
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

//inline a3ret a3updateBlendTree(a3_DemoMode1_Animation const* demoMode, const a3real dt)
//{
//	// Update clip controllers in the blend tree
//	for (a3ui32 i = 0; i < demoMode->blendTree->clipCount; i++)
//	{
//		a3clipControllerUpdate(&demoMode->blendTree->clipControllers[i], dt);
//	}
//
//	/*
//		first update the nodes which have no inputs and just sample from a clip
//		for each node in the blend tree...
//	*/
//	for (a3ui32 i = 0; i < demoMode->blendTree->nodeCount; i++)
//	{
//		// if the node is a clip node...
//		if (demoMode->blendTree->nodes[i].numInputs <= 0)
//		{
//			// get the clip controller
//			// copy the pose from the clip controller to the node's out pose
//			a3hierarchyPoseCopy(demoMode->blendTree->nodes[i].outPose,
//				demoMode->hierarchyPoseGroup_skel->hpose + demoMode->blendTree->nodes[i].myClipController->keyframeIndex, // get deltas of a pose in frame keyframeIndex
//				demoMode->hierarchy_skel->numNodes);
//		}
//	}
//	/*
//		finally execute the nodes of the blend tree in order
//	*/
//	const a3ui32 rootIndex = 0; // note: root index is assumed to be zero
//	a3executeBlendTree(&demoMode->blendTree->nodes[rootIndex], demoMode->blendTree->nodes[rootIndex].numInputs, demoMode->blendTree->nodeCount, demoMode->hierarchy_skel);
//}


// ---------------------------------------------------------------------------------

#endif	// !__ANIMAL3D_BLENDTREE_INL
#endif	// __ANIMAL3D_BLENDTREE_H