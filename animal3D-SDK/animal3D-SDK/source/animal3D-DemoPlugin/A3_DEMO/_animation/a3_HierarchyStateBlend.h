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

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H


#include "a3_HierarchyState.h"

#include "a3_Kinematics.h"
#include <math.h>


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	
typedef a3_SpatialPose* (*a3_spatialPoseOp) (
	a3_SpatialPose* pose_out,
	a3_SpatialPose const* pose_in[],
	a3real const param_in[]
);

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
a3_SpatialPose* a3spatialPoseSplit(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs);
a3_SpatialPose* a3spatialPoseScale(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in, const a3real blendParam);
a3_SpatialPose* a3spatialPoseTriangularBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3_SpatialPose* pose2, const a3real u0, const a3real u1);
a3_SpatialPose* a3spatialPoseBinearestBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* init0, const a3_SpatialPose* init1, const a3_SpatialPose* final1, const a3_SpatialPose* final2, const a3real u, const a3real u0, const a3real u1);
a3_SpatialPose* a3spatialPoseBilinearBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* init0, const a3_SpatialPose* init1, const a3_SpatialPose* final0, const a3_SpatialPose* final1, const a3real u, const a3real u0, const a3real u1);
a3_SpatialPose* a3spatialPoseBicubicBlend(		  a3_SpatialPose* pose_out, const a3real blendTotal,
											const a3_SpatialPose* pose_A0, const a3_SpatialPose* pose_A1, const a3_SpatialPose* pose_A2, const a3_SpatialPose* pose_A3, const a3real blendA,
											const a3_SpatialPose* pose_B0, const a3_SpatialPose* pose_B1, const a3_SpatialPose* pose_B2, const a3_SpatialPose* pose_B3, const a3real blendB,
											const a3_SpatialPose* pose_C0, const a3_SpatialPose* pose_C1, const a3_SpatialPose* pose_C2, const a3_SpatialPose* pose_C3, const a3real blendC,
											const a3_SpatialPose* pose_D0, const a3_SpatialPose* pose_D1, const a3_SpatialPose* pose_D2, const a3_SpatialPose* pose_D3, const a3real blendD);

a3_SpatialPose* a3spatialPoseSmoothstep(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3real u);
a3_SpatialPose* a3spatialPoseDescale(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in, const a3real u);
a3_SpatialPose* a3spatialPoseConvert(a3_SpatialPose* pose_inout);
a3_SpatialPose* a3spatialPoseRestore(a3_SpatialPose* pose_inout);

//-----------------------------------------------------------------------------

// data-based reset/identity
//a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
//a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out, a3ui32 nodeCount);

a3_HierarchyPose* a3hierarchyPoseConstruct(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3vec4 eulers, const a3vec4 scale, const a3vec4 translation);
a3_HierarchyPose* a3hierarchyPoseConstant(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, a3ui32 nodeCount);
a3_HierarchyPose* a3hierarchyPoseInvert(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, a3ui32 nodeCount);
a3_HierarchyPose* a3hierarchyPoseMerge(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* lhs, const a3_HierarchyPose* rhs);
a3_HierarchyPose* a3hierarchyPoseNearest(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3real blendParam);

// pointer-based LERP operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3ui32 nodeCount, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u);

a3_HierarchyPose* a3hierarchyPoseCubicBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose_pre, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3_HierarchyPose* pose_post, const a3real blendParam);
a3_HierarchyPose* a3hierarchyPoseDeconcat(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* lhs, const a3_HierarchyPose* rhs);
a3_HierarchyPose* a3hierarchyPoseScale(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose_in, const a3real blendParam);
a3_HierarchyPose* a3hierarchyPoseTriangularBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3_HierarchyPose* pose2, const a3real u0, const a3real u1);
a3_HierarchyPose* a3hierarchyPoseBinearestBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* init0, const a3_HierarchyPose* init1, const a3_HierarchyPose* final1, const a3_HierarchyPose* final2, const a3real u, const a3real u0, const a3real u1);
a3_HierarchyPose* a3hierarchyPoseBilinearBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* init0, const a3_HierarchyPose* init1, const a3_HierarchyPose* final0, const a3_HierarchyPose* final1, const a3real u, const a3real u0, const a3real u1);
a3_HierarchyPose* a3hierarchyPoseBicubicBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3real blendTotal,
	const a3_HierarchyPose* pose_A0, const a3_HierarchyPose* pose_A1, const a3_HierarchyPose* pose_A2, const a3_HierarchyPose* pose_A3, const a3real blendA,
	const a3_HierarchyPose* pose_B0, const a3_HierarchyPose* pose_B1, const a3_HierarchyPose* pose_B2, const a3_HierarchyPose* pose_B3, const a3real blendB,
	const a3_HierarchyPose* pose_C0, const a3_HierarchyPose* pose_C1, const a3_HierarchyPose* pose_C2, const a3_HierarchyPose* pose_C3, const a3real blendC,
	const a3_HierarchyPose* pose_D0, const a3_HierarchyPose* pose_D1, const a3_HierarchyPose* pose_D2, const a3_HierarchyPose* pose_D3, const a3real blendD);

a3_HierarchyPose* a3hierarchyPoseSmoothstep(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3real u);
a3_HierarchyPose* a3hierarchyPoseDescale(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, const a3real u);
a3_HierarchyPose* a3hierarchyPoseConvert(a3_HierarchyPose* pose_inout);
a3_HierarchyPose* a3hierarchyPoseRestore(a3_HierarchyPose* pose_inout);
// a3hierarchyPoseDoFK:
// 1. a3_Hierarchy to contextualize the operation
// 2. the pose to output to in the target object space
// 3. the local input pose to convert
a3_HierarchyPose* a3hierachyPoseDoFK(const a3_Hierarchy* hierarchy, a3_HierarchyPose* hierarchyPose_object, const a3_HierarchyPose* hierarchyPose_local);
// a3hierarchyPoseDoIK:
// 1. a3_Hierarchy to contextualize the operation
// 2. the input object-space pose
// 3. the local pose which will be used as output
a3_HierarchyPose* a3hierachyPoseDoIK(const a3_Hierarchy* hierarchy, const a3_HierarchyPose* hierarchyPose_object, a3_HierarchyPose* hierarchyPose_local);

//-----------------------------------------------------------------------------

a3ui32 a3real4Pow(a3real* v_out, const a3real* v, const a3real p);

#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H