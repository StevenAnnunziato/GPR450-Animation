#pragma once

#ifndef __ANIMAL3D_BLENDTREE_H
#define __ANIMAL3D_BLENDTREE_H

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_BlendTreeNode				a3_BlendTreeNode;
typedef struct a3_BlendTree					a3_BlendTree;
#endif	// __cplusplus

#include "a3_KeyframeAnimationController.h"
#include "a3_Kinematics.h"

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus

// ---------------------------------------------------------------------------------

enum Operation { NONE = 0, SPATIAL = 1, IK = 2, FLOAT = 3, };

// generalized yet somewhat specialized approach
//	-> general approach for multiple sets of specific purposes

typedef a3ret (*a3_BlendFunc) (void const* op);

// identity (0 or 1) [out] - single
// copy (unary +) [out, in0] - single
// negate (unary -) [out, in0] - single
// concat (binary +) [out, in0, in1] - single
// unconcat (binary -) [out, in0, in1] - single
// scale
// unscale
// lerp [out, in0, in1, param0] - single
// loglerp []...
// unloglerp[]...
// unlerp - single
// bilerp - single
#define POSE_IN_MAX 4
#define PARAM_MAX 3
typedef struct a3_SpatialPoseBlendOp
{
	a3_SpatialPose* pose_out;
	a3_SpatialPose const* pose_in[POSE_IN_MAX];
	a3real const* param[PARAM_MAX];
} a3_SpatialPoseBlendOp;

a3ret a3blendFuncSpatialPoseCopy(a3_SpatialPoseBlendOp const* op)
{
	// implements copy
}

a3ret a3blendFuncSpatialPoseLerp(a3_SpatialPoseBlendOp const* op)
{
	// implements lerp
}

// identity - float - additive or multiplicative
// copy - float
// negate - float
// concat - float
// unconcat - float
// scale - add or mult
// unscale - additive or mult
// lerp - float - additive (lerp) or multiplicative (loglerp)
//	-> x = lerp[x0,x1](u) = (x1 - x0) * u + x0
// lerp inv - float
//	-> u = lerp^-1[x0,x1](x) = (x - x0) / (x1 - x0)
//		****problem: no op defined for (pose * pose) or (pose / pose)
// bilerp
// CR
// H
// B

#define VALUE_IN_MAX 2
typedef struct a3_FloatBlendOp
{
	a3real *value_out;
	//a3_SpatialPose const* pose_in[POSE_IN_MAX]; // does not map mathematically so not good
	a3real const* value_in[VALUE_IN_MAX]; // x
	a3real const* param[PARAM_MAX]; // u
} a3_FloatBlendOp;

// could use float op to do individual CHANNELS
//	-> build for rotation X, Y, Z, translation X, Y, Z, scale X, Y, Z

// identity - hierarchy
//	-> all of the above apply to this

// convert
typedef struct a3_PoseToMatBlendOp
{
	a3mat4 *mat_out;
	a3_SpatialPose const* pose_in;
} a3_PoseToMatBlendOp;

// revert
typedef struct a3_MatToPoseBlendOp
{
	a3_SpatialPose* pose_out;
	a3mat4 const* mat_in;
} a3_MatToPoseBlendOp;

// forward kinematics
// inverse kinematics
typedef struct a3_MatrixBlendOp
{
	a3mat4 *mat_out;
	a3mat4 const* mat_in[POSE_IN_MAX];
} a3_MatrixBlendOp;

// identity
// copy
// inv
// concat
// lerp
typedef struct a3_QuaternionBlendOp
{
	a3quat* quat_out;
	a3quat const* quat_in[POSE_IN_MAX];
} a3_QuaternionBlendOp;

////typedef struct a3_HierarchyStateBlendOp
////{
////
////} a3_HierarchyStateBlendOp;


// ---------------------------------------------------------------------------------

// function pointer typedef for blend operations
typedef a3_HierarchyPose* (*a3_HierarchyPoseOp) (
	a3_HierarchyPose* pose_out,
	a3ui32 nodeCount,
	a3_HierarchyPose const* pose_in,
	a3real const param_in[],
	a3_Hierarchy const* heierarchy
	);

// blend tree operations and structures
struct a3_BlendTreeNode;

// blend tree node
struct a3_BlendTreeNode
{
	// function pointer to my particular operation
	a3_HierarchyPoseOp poseOp;
	a3real opParams[32];

	// store the hierarchy pose after poseOp is completed
	// this is the most important part of the blend node
	a3_HierarchyPose* outPose;

	// child blend nodes in the tree which this node relies on for input
	a3_BlendTreeNode* inputNodes[16];
	a3ui32 numInputs;

	// clip controller for nodes with no additional input
	a3_ClipController* myClipController;

	// info for masking
	a3ui32 baskBoneIndices[128];
	a3ui32 numMaskBones;
};

struct a3_BlendTree
{
	// pose 0 is the base pose
	// all further poses are used for intermediate blending
	// TODO: the number of poses in here will be determined at runtime based on how many nodes the animator uses
	a3_HierarchyPose* poses;

	// keyframe animation controllers to update each animation used for blending
	a3_ClipController clipControllers[16];
	a3ui32 clipCount;

	// hierarchy of nodes defined by the animator
	// root is the final output pose
	a3_BlendTreeNode nodes[32];
	a3ui32 nodeCount;
};

// mask node: takes in a hierarchy pose and an array of spatial poses, then
// zeros out the deltas of the specified spatial poses.
a3ui32 a3maskNode(a3_BlendTreeNode* node_inout);

void a3initBlendTree(a3_BlendTree* blend_out, a3ui32 nodeCount, a3ui32 clipCount, a3ui32 hierarchyNodes);

void a3freeBlendTree(a3_BlendTree* blend_in);

void a3initBlendNode(a3_BlendTreeNode* node_out, a3_BlendTree* blendTree, const a3byte clipName[a3keyframeAnimation_nameLenMax]);

void a3initBlendFunction(a3_BlendTreeNode* node_out, a3_BlendTree* blendTree, a3_HierarchyPoseOp poseOp, a3_BlendTreeNode* inputNodes[]);

void a3freeBlendNode(a3_BlendTreeNode* node_in);

void a3maskBlendNode(a3_BlendTreeNode* node_out, a3ui32 maskindecies1[128]);

void a3updateBlendTree(const a3_BlendTree* blend_in);

void a3ReadBlendTreeFromFile();


// ---------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_BlendTree.inl"


#endif	// !__ANIMAL3D_BLENDTREE_H