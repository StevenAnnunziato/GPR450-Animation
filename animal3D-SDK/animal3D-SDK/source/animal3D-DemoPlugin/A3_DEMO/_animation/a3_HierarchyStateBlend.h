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


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus
	

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

a3_SpatialPose* a3spatialPoseConstruct(a3_SpatialPose* pose_out, const a3vec3 eulers, const a3vec3 scale, const a3vec3 translation);
a3_SpatialPose* a3spatialPoseCopy(const a3_SpatialPose* pose_in);
a3_SpatialPose* a3spatialPoseInvert(const a3_SpatialPose* pose_in);
a3_SpatialPose* a3spatialPoseMerge(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs);
a3_SpatialPose* a3spatialPoseNearest(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3real blendParam);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);

a3_SpatialPose* a3spatialPoseCubicBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_pre, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3_SpatialPose* pose_post, const a3real blendParam);
a3_SpatialPose* a3spatialPoseDeconcat(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs);
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
a3_SpatialPose a3spatialPoseDOpIdentity();

// data-based LERP
a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single hierarchy pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out);

a3_HierarchyPose* a3hierarchyPoseConstruct(a3_HierarchyPose* pose_out, const a3vec3 eulers, const a3vec3 scale, const a3vec3 translation);
a3_HierarchyPose* a3hierarchyPoseCopy(const a3_HierarchyPose* pose_in);
a3_HierarchyPose* a3hierarchyPoseInvert(const a3_HierarchyPose* pose_in);
a3_HierarchyPose* a3hierarchyPoseMerge(a3_HierarchyPose* pose_out, const a3_HierarchyPose* lhs, const a3_HierarchyPose* rhs);
a3_HierarchyPose* a3hierarchyPoseNearest(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3real blendParam);

// pointer-based LERP operation for single hierarchy pose
a3_HierarchyPose* a3spatialPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u);

a3_HierarchyPose* a3hierarchyPoseCubicBlend(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_pre, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3_HierarchyPose* pose_post, const a3real blendParam);
a3_HierarchyPose* a3hierarchyPoseDeconcat(a3_HierarchyPose* pose_out, const a3_HierarchyPose* lhs, const a3_HierarchyPose* rhs);
a3_HierarchyPose* a3hierarchyPoseScale(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, const a3real blendParam);
a3_HierarchyPose* a3hierarchyPoseTriangularBlend(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3_HierarchyPose* pose2, const a3real u0, const a3real u1);
a3_HierarchyPose* a3hierarchyPoseBinearestBlend(a3_HierarchyPose* pose_out, const a3_HierarchyPose* init0, const a3_HierarchyPose* init1, const a3_HierarchyPose* final1, const a3_HierarchyPose* final2, const a3real u, const a3real u0, const a3real u1);
a3_HierarchyPose* a3hierarchyPoseBilinearBlend(a3_HierarchyPose* pose_out, const a3_HierarchyPose* init0, const a3_HierarchyPose* init1, const a3_HierarchyPose* final0, const a3_HierarchyPose* final1, const a3real u, const a3real u0, const a3real u1);
a3_HierarchyPose* a3hierarchyPoseBicubicBlend(a3_HierarchyPose* pose_out, const a3real blendTotal,
	const a3_HierarchyPose* pose_A0, const a3_HierarchyPose* pose_A1, const a3_HierarchyPose* pose_A2, const a3_HierarchyPose* pose_A3, const a3real blendA,
	const a3_HierarchyPose* pose_B0, const a3_HierarchyPose* pose_B1, const a3_HierarchyPose* pose_B2, const a3_HierarchyPose* pose_B3, const a3real blendB,
	const a3_HierarchyPose* pose_C0, const a3_HierarchyPose* pose_C1, const a3_HierarchyPose* pose_C2, const a3_HierarchyPose* pose_C3, const a3real blendC,
	const a3_HierarchyPose* pose_D0, const a3_HierarchyPose* pose_D1, const a3_HierarchyPose* pose_D2, const a3_HierarchyPose* pose_D3, const a3real blendD);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H