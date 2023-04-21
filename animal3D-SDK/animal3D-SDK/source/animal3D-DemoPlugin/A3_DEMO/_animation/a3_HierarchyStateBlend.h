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
#include "a3_BlendTree.h"
#include <math.h>


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3ret a3spatialPoseOpIdentity(a3_SpatialPoseBlendOp* data);

a3_SpatialPose* a3spatialPoseConstruct(a3_SpatialPose* pose_out, const a3vec4 eulers, const a3vec4 scale, const a3vec4 translate);
a3_SpatialPose* a3spatialPoseConstant(a3_SpatialPoseBlendOp* data);
a3_SpatialPose* a3spatialPoseInvert(a3_SpatialPoseBlendOp* data);
a3_SpatialPose* a3spatialPoseMerge(a3_SpatialPoseBlendOp* data);
a3_SpatialPose* a3spatialPoseNearest(a3_SpatialPoseBlendOp* data);

// pointer-based LERP operation for single spatial pose
a3ret a3spatialPoseOpLERP(a3_SpatialPoseBlendOp* data);

a3ret a3spatialPoseCubicBlend(a3_SpatialPoseBlendOp* data);
a3ret a3spatialPoseSplit(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs);
a3ret a3spatialPoseScale(a3_SpatialPoseBlendOp* data);
a3ret a3spatialPoseTriangularBlend(a3_SpatialPoseBlendOp* data);
a3ret a3spatialPoseBinearestBlend(a3_SpatialPoseBlendOp* data);
a3ret a3spatialPoseBilinearBlend(a3_SpatialPoseBlendOp* data);
a3ret a3spatialPoseBicubicBlend(a3_SpatialPoseBlendOp* data);


//-----------------------------------------------------------------------------

// data-based reset/identity
//a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
//a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------


// pointer-based reset/identity operation for single hierarchy pose

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <returns></returns>
a3ret a3hierarchyPoseOpIdentity(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount">uint32</param>
/// <param name="eulers">float4</param>
/// <param name="scale">float4</param>
/// <param name="translation">float4</param>
/// <returns></returns>
a3ret a3hierarchyPoseConstruct(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount">uint32</param>
/// <param name="pose_in"></param>
/// <returns></returns>
a3ret a3hierarchyPoseConstant(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="pose_in"></param>
/// <param name="nodeCount"></param>
/// <returns></returns>
a3ret a3hierarchyPoseInvert(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
a3ret a3hierarchyPoseMerge(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="blendParam"></param>
/// <returns></returns>
a3ret a3hierarchyPoseNearest(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="u"></param>
/// <returns></returns>
a3ret a3hierarchyPoseOpLERP(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose_in"></param>
/// <param name="target_start_index"></param>
/// <param name="u_x"></param>
/// <param name="u_y"></param>
/// <param name="u_z"></param>
/// <returns></returns>
a3ret a3hierarchyPoseOpLookAt( a3_HierarchyStateBlendOp* data );

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose0"></param>
/// <param name="pose1"></param>
/// <param name="u"></param>
/// <returns></returns>
a3ret a3hierarchyPoseOpChain(a3_HierarchyStateBlendOp* data );

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
a3ret a3hierarchyPoseCubicBlend(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
a3ret a3hierarchyPoseSplit(a3_HierarchyPoseBlendOp* data);

/// <summary>
/// 
/// </summary>
/// <param name="pose_out"></param>
/// <param name="nodeCount"></param>
/// <param name="pose_in"></param>
/// <param name="blendParam"></param>
/// <returns></returns>
a3ret a3hierarchyPoseScale(a3_HierarchyPoseBlendOp* data);

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
a3ret a3hierarchyPoseTriangularBlend(a3_HierarchyPoseBlendOp* data);

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
a3ret a3hierarchyPoseBinearestBlend(a3_HierarchyPoseBlendOp* data);

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
a3ret a3hierarchyPoseBilinearBlend(a3_HierarchyPoseBlendOp* data);

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
a3ret a3hierarchyPoseBicubicBlend(a3_HierarchyPoseBlendOp* data);

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