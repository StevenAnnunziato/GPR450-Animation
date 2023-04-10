/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_HierarchyStateBlend.h
	Hierarchy blend operations.
*/

#pragma once

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_BlendTreeNode				a3_BlendTreeNode;
typedef struct a3_BlendTree					a3_BlendTree;
#endif	// __cplusplus

//#include "a3_HierarchyState.h"
#include "a3_KeyframeAnimationController.h"

#include "a3_Kinematics.h"
#include <math.h>


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

a3_SpatialPose* a3spatialPoseConstruct(a3_SpatialPose* pose_out, const a3vec4 eulers, const a3vec4 scale, const a3vec4 translation);
a3_SpatialPose* a3spatialPoseConstant(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in);
a3_SpatialPose* a3spatialPoseInvert(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in);
a3_SpatialPose* a3spatialPoseMerge(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs);
a3_SpatialPose* a3spatialPoseNearest(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3real blendParam);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);

a3_SpatialPose* a3spatialPoseCubicBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_pre, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3_SpatialPose* pose_post, const a3real blendParam);
//a3_SpatialPose* a3spatialPoseSplit(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs);
a3_SpatialPose* a3spatialPoseScale(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in, const a3real blendParam);
a3_SpatialPose* a3spatialPoseTriangularBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3_SpatialPose* pose2, const a3real u0, const a3real u1);
a3_SpatialPose* a3spatialPoseBinearestBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* init0, const a3_SpatialPose* init1, const a3_SpatialPose* final1, const a3_SpatialPose* final2, const a3real u, const a3real u0, const a3real u1);
a3_SpatialPose* a3spatialPoseBilinearBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* init0, const a3_SpatialPose* init1, const a3_SpatialPose* final0, const a3_SpatialPose* final1, const a3real u, const a3real u0, const a3real u1);
a3_SpatialPose* a3spatialPoseBicubicBlend(		  a3_SpatialPose* pose_out, const a3real blendTotal,
											const a3_SpatialPose* pose_A0, const a3_SpatialPose* pose_A1, const a3_SpatialPose* pose_A2, const a3_SpatialPose* pose_A3, const a3real blendA,
											const a3_SpatialPose* pose_B0, const a3_SpatialPose* pose_B1, const a3_SpatialPose* pose_B2, const a3_SpatialPose* pose_B3, const a3real blendB,
											const a3_SpatialPose* pose_C0, const a3_SpatialPose* pose_C1, const a3_SpatialPose* pose_C2, const a3_SpatialPose* pose_C3, const a3real blendC,
											const a3_SpatialPose* pose_D0, const a3_SpatialPose* pose_D1, const a3_SpatialPose* pose_D2, const a3_SpatialPose* pose_D3, const a3real blendD);


//-----------------------------------------------------------------------------

// data-based reset/identity
//a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
//a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------

typedef a3_HierarchyPose* (*a3_HierarchyPoseOp) (
	a3_HierarchyPose* pose_out,
	a3ui32 nodeCount,
	a3_HierarchyPose const* pose_in,
	a3real const param_in[]
	);


// pointer-based reset/identity operation for single hierarchy pose

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseOpIdentity(
	a3_HierarchyPose* pose_out, 
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[], 
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount">uint32</param>
/// <param name="eulers">float4</param>
/// <param name="scale">float4</param>
/// <param name="translation">float4</param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseConstruct(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount">uint32</param>
/// <param name="pose_in"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseConstant(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="pose_in"></param>
/// <param name="nodeCount"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseInvert(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseMerge(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="blendParam"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseNearest(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="u"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseOpLERP(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in,
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose_pre"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="pose_post"></param>
/// <param name="blendParam"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseCubicBlend(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseSplit(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose_in"></param>
/// <param name="blendParam"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseScale(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="pose2"></param>
/// <param name="u0"></param>
/// <param name="u1"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseTriangularBlend(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="init0"></param>
/// <param name="init1"></param>
/// <param name="final1"></param>
/// <param name="final2"></param>
/// <param name="u"></param>
/// <param name="u0"></param>
/// <param name="u1"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseBinearestBlend(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="init0"></param>
/// <param name="init1"></param>
/// <param name="final0"></param>
/// <param name="final1"></param>
/// <param name="u"></param>
/// <param name="u0"></param>
/// <param name="u1"></param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseBilinearBlend(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount">uint32</param>
/// <param name="pose_A0"></param>
/// <param name="pose_A1"></param>
/// <param name="pose_A2"></param>
/// <param name="pose_A3"></param>
/// <param name="pose_B0"></param>
/// <param name="pose_B1"></param>
/// <param name="pose_B2"></param>
/// <param name="pose_B3"></param>
/// <param name="pose_C0"></param>
/// <param name="pose_C1"></param>
/// <param name="pose_C2"></param>
/// <param name="pose_C3"></param>
/// <param name="pose_D0"></param>
/// <param name="pose_D1"></param>
/// <param name="pose_D2"></param>
/// <param name="pose_D3"></param>
/// <param name="blendTotal">float</param>
/// <param name="blendA">float</param>
/// <param name="blendB">float</param>
/// <param name="blendC">float</param>
/// <param name="blendD">float</param>
/// <returns></returns>
a3_HierarchyPose* a3hierarchyPoseBicubicBlend(
	a3_HierarchyPose* pose_out,
	a3ui32  nodeCount,
	a3_HierarchyPose const* pose_in[],
	a3real const param_in[]);

// ----------------------------------------------------------------------------
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
	a3ui32 baskBoneIndices[128]; // TODO: this should be a dynamic array
	a3ui32 numMaskBones;
};

struct a3_BlendTree
{
	// pose 0 is the base pose
	// all further poses are used for intermediate blending
	// TODO: the number of poses in here will be determined at runtime based on how many nodes the animator uses
	a3_HierarchyPose* poses;

	// keyframe animation controllers to update each animation used for blending
	a3_ClipController clipControllers[4];
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







//-----------------------------------------------------------------------------

/// <summary>
/// 
/// </summary>
/// <param name="v_out"></param>
/// <param name="v"></param>
/// <param name="p"></param>
/// <returns></returns>
a3ui32 a3real4Pow(a3real* v_out, const a3real* v, const a3real p);


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H