#include "a3_BlendTree.h"
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

inline void a3initBlendTree(a3_BlendTree* blend_out, a3ui32 nodeCount, a3ui32 clipCount, a3ui32 hierarchyNodes)
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

inline void a3freeBlendTree(a3_BlendTree* blend_in)
{
	free(blend_in->poses->pose);
	free(blend_in->poses);
}

inline void a3initBlendNode(a3_BlendTreeNode* node_out, a3_BlendTree* blendTree, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
}

inline void a3initBlendFunction(a3_BlendTreeNode* node_out, a3_BlendTree* blendTree, a3_HierarchyPoseOp poseOp, a3_BlendTreeNode* inputNodes[])
{
}

inline void a3freeBlendNode(a3_BlendTreeNode* node_in)
{
}

inline void a3maskBlendNode(a3_BlendTreeNode* node_out, a3ui32 maskindecies1[128])
{
}

inline void a3updateBlendTree(const a3_BlendTree* blend_in)
{
}


// ---------------------------------------------------------------------------------

#endif	// !__ANIMAL3D_BLENDTREE_INL
#endif	// __ANIMAL3D_BLENDTREE_H