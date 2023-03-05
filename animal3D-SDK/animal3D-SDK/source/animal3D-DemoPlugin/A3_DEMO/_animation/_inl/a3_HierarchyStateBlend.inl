#include "a3_HierarchyStateBlend.h"
#include <stdlib.h>

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
	
	a3_HierarchyStateBlend.inl
	Implementation of inline hierarchical blend operations.
*/


#ifdef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_INL
#define __ANIMAL3D_HIERARCHYSTATEBLEND_INL


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out)
{
	if (pose_out)
	{
		pose_out->transform = a3mat4_identity;
		pose_out->orientation = a3vec4_one;
		pose_out->angles = a3vec4_zero;
		pose_out->scale = a3vec4_one;
		pose_out->translation = a3vec4_zero;

		// done
		return pose_out;
	}
	
	return 0;
}

inline a3_SpatialPose* a3spatialPoseConstruct(a3_SpatialPose* pose_out, const a3vec4 eulers, const a3vec4 scale, const a3vec4 translation)
{
	if (pose_out)
	{
		pose_out->angles = eulers;
		pose_out->translation = translation;
		pose_out->scale = scale;

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseConstant(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in)
{
	if (pose_out && pose_in)
	{
		// set values
		a3spatialPoseConstruct(pose_out, pose_in->angles, pose_in->scale, pose_in->translation);

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseInvert(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in)
{
	if (pose_out && pose_in)
	{
		pose_out->angles.x = pose_out->angles.x - pose_in->angles.x;
		pose_out->angles.y = pose_out->angles.y - pose_in->angles.y;
		pose_out->angles.z = pose_out->angles.z - pose_in->angles.z;

		pose_out->translation.x = pose_out->translation.x - pose_in->translation.x;
		pose_out->translation.y = pose_out->translation.y - pose_in->translation.y;
		pose_out->translation.z = pose_out->translation.z - pose_in->translation.z;

		pose_out->scale.x = pose_out->scale.x / pose_in->scale.x;
		pose_out->scale.y = pose_out->scale.y / pose_in->scale.y;
		pose_out->scale.z = pose_out->scale.z / pose_in->scale.z;

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseMerge(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs)
{
	if (pose_out && lhs && rhs)
	{
		pose_out->angles.x = lhs->angles.x + rhs->angles.x;
		pose_out->angles.y = lhs->angles.y + rhs->angles.y;
		pose_out->angles.z = lhs->angles.z + rhs->angles.z;

		pose_out->translation.x = lhs->translation.x + rhs->translation.x;
		pose_out->translation.y = lhs->translation.y + rhs->translation.y;
		pose_out->translation.z = lhs->translation.z + rhs->translation.z;

		pose_out->scale.x = lhs->scale.x * rhs->scale.x;
		pose_out->scale.y = lhs->scale.y * rhs->scale.y;
		pose_out->scale.z = lhs->scale.z * rhs->scale.z;

		return pose_out;
	}
	
	return 0;
}

inline a3_SpatialPose* a3spatialPoseNearest(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3real blendParam)
{
	if (pose_out && pose0 && pose1)
	{
		if (blendParam < 0.5f)
		{
			a3spatialPoseConstant(pose_out, pose0);
		}
		else
		{
			a3spatialPoseConstant(pose_out, pose1);
		}

		return pose_out;
	}

	return 0;
}

// pointer-based LERP operation for single spatial pose
inline a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u)
{
	if (pose_out && pose0 && pose1)
	{
		a3real3Lerp(pose_out->angles.v, pose0->angles.v, pose1->angles.v, u);

		a3real3Lerp(pose_out->scale.v, pose0->scale.v, pose1->scale.v, u);

		a3real3Lerp(pose_out->translation.v, pose0->translation.v, pose1->translation.v, u);

		// done
		return pose_out;
	}

	return 0;
	
}

inline a3_SpatialPose* a3spatialPoseCubicBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_pre, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3_SpatialPose* pose_post, const a3real blendParam)
{
	// construct a matrix with all the control inputs
	/*a3mat4 rotationControlMat;
	rotationControlMat.v0 = pose_pre->angles;
	rotationControlMat.v1 = pose0->angles;
	rotationControlMat.v2 = pose1->angles;
	rotationControlMat.v3 = pose_post->angles;
	a3mat4 translationControlMat;
	translationControlMat.v0 = pose_pre->translation;
	translationControlMat.v1 = pose0->translation;
	translationControlMat.v2 = pose1->translation;
	translationControlMat.v3 = pose_post->translation;
	a3mat4 scaleControlMat;
	scaleControlMat.v0 = pose_pre->scale;
	scaleControlMat.v1 = pose0->scale;
	scaleControlMat.v2 = pose1->scale;
	scaleControlMat.v3 = pose_post->scale;

	// construct the catmull-rom kernel matrix
	const a3mat4 kernel;// = a3mat4(0.0, +2.0, 0.0, 0.0,
						//		   -1.0, 0.0, +1.0, 0.0,
						//		   +2.0, -5.0, +4.0, -1.0,
						//		   -1.0, +3.0, -3.0, +1.0);
	a3real4x4Set(&kernel, 0.0, +2.0, 0.0, 0.0, -1.0, 0.0, +1.0, 0.0, +2.0, -5.0, +4.0, -1.0, -1.0, +3.0, -3.0, +1.0);

	// find blend parameter for each set of points
	float t0 = 0.5;
	float t1 = t0 * blendParam;
	float t2 = t1 * blendParam;
	float t3 = t2 * blendParam;
	
	//pose_out->rotation = (controlMat * (kernel * vec4(t0, t1, t2, t3)));
	// put times in an array
	a3mat4 kernelTime;
	a3real timeVec[] = {t0, t1, t2, t3};
	a3real4Real4x4ProductR(kernelTime.m, kernel.m, timeVec);

	// finalize pose out
	a3real outArray[4];
	a3real4Real4x4ProductR(outArray, rotationControlMat.m, kernelTime.m);
	pose_out->angles.x = outArray[0]; pose_out->angles.y = outArray[1]; pose_out->angles.z = outArray[2];
	a3real4Real4x4ProductR(outArray, translationControlMat.m, kernelTime.m);
	pose_out->translation.x = outArray[0]; pose_out->translation.y = outArray[1]; pose_out->translation.z = outArray[2];
	a3real4Real4x4ProductR(outArray, scaleControlMat.m, kernelTime.m);
	pose_out->scale.x = outArray[0]; pose_out->scale.y = outArray[1]; pose_out->scale.z = outArray[2];*/

	if (pose_out && pose_pre && pose0 && pose1 && pose_post)
	{
		// make blend params
		const a3real t = blendParam;
		const a3real t2 = t * t;
		const a3real t3 = t2 * t;

		// make scalars
		a3real s1 = -t + 2.0f * t2 - t3;
		a3real s2 = 2.0f - 5.0f * t2 + 3.0f * t3;
		a3real s3 = t + 4.0f * t2 - 3.0f * t3;
		a3real s4 = -t2 + t3;

		// temp arrays
		a3real sum[4];
		a3real out1[4];

		// rotation
		a3real4ProductS(out1, pose_pre->angles.v, s1);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, pose0->angles.v, s2);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, pose1->angles.v, s3);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, pose_post->angles.v, s4);
		a3real4Add(sum, out1);
		a3real4ProductS(sum, sum, 0.5f);

		pose_out->angles.v[0] = sum[0];
		pose_out->angles.v[1] = sum[1];
		pose_out->angles.v[2] = sum[2];

		// translation
		a3real4ProductS(out1, pose_pre->translation.v, s1);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, pose0->translation.v, s2);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, pose1->translation.v, s3);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, pose_post->translation.v, s4);
		a3real4Add(sum, out1);
		a3real4ProductS(sum, sum, 0.5f);

		pose_out->translation.v[0] = sum[0];
		pose_out->translation.v[1] = sum[1];
		pose_out->translation.v[2] = sum[2];

		// scale
		s1, s2, s3, s4 *= 0.5f;
		a3real4Pow(out1, pose_pre->scale.v, s1);
		a3real4MulComp(sum, out1);
		a3real4Pow(out1, pose0->scale.v, s2);
		a3real4MulComp(sum, out1);
		a3real4Pow(out1, pose1->scale.v, s3);
		a3real4MulComp(sum, out1);
		a3real4Pow(out1, pose_post->scale.v, s4);
		a3real4MulComp(sum, out1);
		a3real4Pow(sum, sum, 0.5f);

		pose_out->scale.v[0] = sum[0];
		pose_out->scale.v[1] = sum[1];
		pose_out->scale.v[2] = sum[2];

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseSplit(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs)
{
	if (pose_out && lhs && rhs)
	{
		pose_out->angles.x = lhs->angles.x - rhs->angles.x;
		pose_out->angles.y = lhs->angles.y - rhs->angles.y;
		pose_out->angles.z = lhs->angles.z - rhs->angles.z;

		pose_out->translation.x = lhs->translation.x - rhs->translation.x;
		pose_out->translation.y = lhs->translation.y - rhs->translation.y;
		pose_out->translation.z = lhs->translation.z - rhs->translation.z;

		pose_out->scale.x = lhs->scale.x / rhs->scale.x;
		pose_out->scale.y = lhs->scale.y / rhs->scale.y;
		pose_out->scale.z = lhs->scale.z / rhs->scale.z;

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseScale(a3_SpatialPose* pose_out, const a3_SpatialPose* pose_in, const a3real blendParam)
{
	if (pose_out && pose_in)
	{
		a3spatialPoseOpLERP(pose_out, a3spatialPoseOpIdentity(pose_out), pose_in, blendParam);

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseTriangularBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* pose0, const a3_SpatialPose* pose1, const a3_SpatialPose* pose2, const a3real u1, const a3real u2)
{
	if (pose_out && pose0 && pose1 && pose2)
	{
		a3real u0 = 1 - u1 - u2;

		// allocate spatial poses for internal calculations
		a3_SpatialPose* s0 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* s1 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* s2 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* c1 = malloc(sizeof(a3_SpatialPose));

		// scale the poses for weighted average calculation
		a3spatialPoseScale(s0, pose0, u0);
		a3spatialPoseScale(s1, pose1, u1);
		a3spatialPoseScale(s2, pose2, u2);

		// concat all
		a3spatialPoseMerge(c1, s0, s1);
		a3spatialPoseMerge(pose_out, c1, s2);

		// free memory
		free(s0);
		free(s1);
		free(s2);
		free(c1);

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseBinearestBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* init0, const a3_SpatialPose* init1, const a3_SpatialPose* final0, const a3_SpatialPose* final1, const a3real u, const a3real u0, const a3real u1)
{
	if (pose_out && init0 && init1 && final0 && final1)
	{
		a3_SpatialPose* b0 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* b1 = malloc(sizeof(a3_SpatialPose));

		a3spatialPoseNearest(b0, init0, init1, u0);
		a3spatialPoseNearest(b1, final0, final1, u1);
		a3spatialPoseNearest(pose_out, b0, b1, u);

		free(b0);
		free(b1);

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseBilinearBlend(a3_SpatialPose* pose_out, const a3_SpatialPose* init0, const a3_SpatialPose* init1, const a3_SpatialPose* final0, const a3_SpatialPose* final1, const a3real u, const a3real u0, const a3real u1)
{
	if (pose_out && init0 && init1 && final0 && final1)
	{
		a3_SpatialPose* b0 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* b1 = malloc(sizeof(a3_SpatialPose));

		a3spatialPoseOpLERP(b0, init0, init1, u0);
		a3spatialPoseOpLERP(b1, final0, final1, u1);
		a3spatialPoseOpLERP(pose_out, b0, b1, u);

		free(b0);
		free(b1);

		return pose_out;
	}

	return 0;
}

inline a3_SpatialPose* a3spatialPoseBicubicBlend(a3_SpatialPose* pose_out, const a3real blendTotal, const a3_SpatialPose* pose_A0, const a3_SpatialPose* pose_A1, const a3_SpatialPose* pose_A2, const a3_SpatialPose* pose_A3, const a3real blendA, const a3_SpatialPose* pose_B0, const a3_SpatialPose* pose_B1, const a3_SpatialPose* pose_B2, const a3_SpatialPose* pose_B3, const a3real blendB, const a3_SpatialPose* pose_C0, const a3_SpatialPose* pose_C1, const a3_SpatialPose* pose_C2, const a3_SpatialPose* pose_C3, const a3real blendC, const a3_SpatialPose* pose_D0, const a3_SpatialPose* pose_D1, const a3_SpatialPose* pose_D2, const a3_SpatialPose* pose_D3, const a3real blendD)
{
	if (pose_out && pose_A0 && pose_A1 && pose_A2 && pose_A3 &&
					pose_B0 && pose_B1 && pose_B2 && pose_B3 &&
					pose_C0 && pose_C1 && pose_C2 && pose_C3 &&
					pose_D0 && pose_D1 && pose_D2 && pose_D3)
	{
		a3_SpatialPose* b0 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* b1 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* b2 = malloc(sizeof(a3_SpatialPose));
		a3_SpatialPose* b3 = malloc(sizeof(a3_SpatialPose));

		a3spatialPoseCubicBlend(b0, pose_A0, pose_A1, pose_A2, pose_A3, blendA);
		a3spatialPoseCubicBlend(b1, pose_B0, pose_B1, pose_B2, pose_B3, blendB);
		a3spatialPoseCubicBlend(b2, pose_C0, pose_C1, pose_C2, pose_C3, blendC);
		a3spatialPoseCubicBlend(b3, pose_D0, pose_D1, pose_D2, pose_D3, blendD);
		a3spatialPoseCubicBlend(pose_out, b0, b1, b2, b3, blendTotal);

		free(b0);
		free(b1);
		free(b2);
		free(b3);

		return pose_out;
	}

	return 0;
}


//-----------------------------------------------------------------------------

//data-based reset/identity
inline a3_SpatialPose a3spatialPoseDOpIdentity()
{
	a3_SpatialPose const result = { a3mat4_identity, a3vec4_one, a3vec4_zero, a3vec4_one, a3vec4_zero };
	return result;
}

// data-based LERP
inline a3_SpatialPose a3spatialPoseDOpLERP(a3_SpatialPose const pose0, a3_SpatialPose const pose1, a3real const u)
{
	a3_SpatialPose result = { 0 };
	// ...

	// done
	return result;
}


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_inout, a3ui32 nodeCount)
{
	if (pose_inout && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseOpIdentity(pose_inout->pose + i);
		return pose_inout;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseConstruct(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3vec4 eulers, const a3vec4 scale, const a3vec4 translation)
{
	if (pose_out && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseConstruct(pose_out->pose + i, eulers, scale, translation);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseConstant(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, a3ui32 nodeCount)
{
	if (pose_out && pose_in && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseConstant(pose_out->pose + i, pose_in->pose + i);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseInvert(a3_HierarchyPose* pose_out, const a3_HierarchyPose* pose_in, a3ui32 nodeCount)
{
	if (pose_out && pose_in && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseInvert(pose_out->pose + i, pose_in->pose + i);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseMerge(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* lhs, const a3_HierarchyPose* rhs)
{
	if (pose_out && lhs && rhs && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseMerge(pose_out->pose + i, lhs->pose + i, rhs->pose + i);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseNearest(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3real blendParam)
{
	if (pose_out && pose0 && pose1 && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseNearest(pose_out->pose + i, pose0->pose + i, pose1->pose + i, blendParam);
		return pose_out;
	}
	return 0;
}

// pointer-based LERP operation for hierarchical pose
inline a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3ui32 nodeCount, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u)
{
	if (pose_out && pose0 && pose1 && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseLerp(pose_out->pose + i, pose0->pose + i, pose1->pose + i, u);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseCubicBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose_pre, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3_HierarchyPose* pose_post, const a3real blendParam)
{
	if (pose_out && pose0 && pose1 && pose_pre  && pose_post && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseCubicBlend(pose_out->pose + i, pose_pre->pose + i, pose0->pose + i, pose1->pose + i, pose_post->pose + i, blendParam);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseDeconcat(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* lhs, const a3_HierarchyPose* rhs)
{
	if (pose_out && lhs && rhs && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseSplit(pose_out->pose + i, lhs->pose + i, rhs->pose + i);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseScale(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose_in, const a3real blendParam)
{
	if (pose_out && pose_in && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseScale(pose_out->pose + i, pose_in->pose + i, blendParam);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseTriangularBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* pose0, const a3_HierarchyPose* pose1, const a3_HierarchyPose* pose2, const a3real u0, const a3real u1)
{
	if (pose_out && pose0 && pose1 && pose2 && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseTriangularBlend(pose_out->pose + i, pose0->pose + i, pose1->pose + i, pose2->pose + i, u0, u1);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseBinearestBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* init0, const a3_HierarchyPose* init1, const a3_HierarchyPose* final1, const a3_HierarchyPose* final2, const a3real u, const a3real u0, const a3real u1)
{
	if (pose_out && init0 && init1 && final1 && final2 && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseBinearestBlend(pose_out->pose + i, init0->pose + i, init1->pose + i, final1->pose + i, final2->pose + i, u, u0, u1);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseBilinearBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3_HierarchyPose* init0, const a3_HierarchyPose* init1, const a3_HierarchyPose* final0, const a3_HierarchyPose* final1, const a3real u, const a3real u0, const a3real u1)
{
	if (pose_out && init0 && init1 && final0 && final1 && nodeCount)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseBilinearBlend(pose_out->pose + i, init0->pose + i, init1->pose + i, final0->pose + i, final1->pose + i, u, u0, u1);
		return pose_out;
	}
	return 0;
}

inline a3_HierarchyPose* a3hierarchyPoseBicubicBlend(a3_HierarchyPose* pose_out, a3ui32 nodeCount, const a3real blendTotal, const a3_HierarchyPose* pose_A0, const a3_HierarchyPose* pose_A1, const a3_HierarchyPose* pose_A2, const a3_HierarchyPose* pose_A3, const a3real blendA, const a3_HierarchyPose* pose_B0, const a3_HierarchyPose* pose_B1, const a3_HierarchyPose* pose_B2, const a3_HierarchyPose* pose_B3, const a3real blendB, const a3_HierarchyPose* pose_C0, const a3_HierarchyPose* pose_C1, const a3_HierarchyPose* pose_C2, const a3_HierarchyPose* pose_C3, const a3real blendC, const a3_HierarchyPose* pose_D0, const a3_HierarchyPose* pose_D1, const a3_HierarchyPose* pose_D2, const a3_HierarchyPose* pose_D3, const a3real blendD)
{
	if (pose_out && nodeCount	&& pose_A0 && pose_A1 && pose_A2 && pose_A3 && blendA
								&& pose_B0 && pose_B1 && pose_B2 && pose_B3 && blendB
								&& pose_C0 && pose_C1 && pose_C2 && pose_C3 && blendC
								&& pose_D0 && pose_D1 && pose_D2 && pose_D3 && blendD)
	{
		a3index i;
		for (i = 0; i < nodeCount; ++i)
			a3spatialPoseBicubicBlend(pose_out->pose + i, blendTotal,
				pose_A0->pose + i, pose_A1->pose + i, pose_A2->pose + i, pose_A3->pose + i, blendA,
				pose_B0->pose + i, pose_B1->pose + i, pose_B2->pose + i, pose_B3->pose + i, blendB,
				pose_C0->pose + i, pose_C1->pose + i, pose_C2->pose + i, pose_C3->pose + i, blendC,
				pose_D0->pose + i, pose_D1->pose + i, pose_D2->pose + i, pose_D3->pose + i, blendD);
		return pose_out;
	}
	return 0;
}

inline a3ui32 a3real4Pow(a3real* v_out, const a3real* v, const a3real p)
{
	v_out[0] = (a3real)pow(v[0], p);
	v_out[1] = (a3real)pow(v[1], p);
	v_out[2] = (a3real)pow(v[2], p);
	v_out[3] = (a3real)pow(v[3], p);
	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H