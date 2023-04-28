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
inline a3ret a3spatialPoseOpIdentity(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out)
	{
		data->pose_out->transformMat = a3mat4_identity;
		data->pose_out->rotate = a3vec4_one;
		data->pose_out->rotate = a3vec4_zero;
		data->pose_out->scale = a3vec4_one;
		data->pose_out->translate = a3vec4_zero;

		// done
		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseConstruct(a3_SpatialPose* pose_out, const a3vec4 eulers, const a3vec4 scale, const a3vec4 translate)
{
	if (pose_out)
	{
		pose_out->rotate = eulers;
		pose_out->translate = translate;
		pose_out->scale = scale;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseConstant(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0])
	{
		// set values
		a3spatialPoseConstruct(data->pose_out, data->pose_in[0]->rotate, data->pose_in[0]->scale, data->pose_in[0]->translate);

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseInvert(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[0]->scale.x > 0.0f
		&& data->pose_in[0]->scale.y > 0.0f
		&& data->pose_in[0]->scale.z > 0.0f)
	{
		data->pose_out->rotate.x = -data->pose_in[0]->rotate.x;
		data->pose_out->rotate.y = -data->pose_in[0]->rotate.y;
		data->pose_out->rotate.z = -data->pose_in[0]->rotate.z;

		data->pose_out->translate.x = -data->pose_in[0]->translate.x;
		data->pose_out->translate.y = -data->pose_in[0]->translate.y;
		data->pose_out->translate.z = -data->pose_in[0]->translate.z;

		data->pose_out->scale.x = 1.0f / data->pose_in[0]->scale.x;
		data->pose_out->scale.y = 1.0f / data->pose_in[0]->scale.y;
		data->pose_out->scale.z = 1.0f / data->pose_in[0]->scale.z;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseMerge(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1])
	{
		data->pose_out->rotate.y = data->pose_in[0]->rotate.y + data->pose_in[1]->rotate.y;
		data->pose_out->rotate.x = data->pose_in[0]->rotate.x + data->pose_in[1]->rotate.x;
		data->pose_out->rotate.z = data->pose_in[0]->rotate.z + data->pose_in[1]->rotate.z;

		data->pose_out->translate.x = data->pose_in[0]->translate.x + data->pose_in[1]->translate.x;
		data->pose_out->translate.y = data->pose_in[0]->translate.y + data->pose_in[1]->translate.y;
		data->pose_out->translate.z = data->pose_in[0]->translate.z + data->pose_in[1]->translate.z;

		data->pose_out->scale.x = data->pose_in[0]->scale.x * data->pose_in[1]->scale.x;
		data->pose_out->scale.y = data->pose_in[0]->scale.y * data->pose_in[1]->scale.y;
		data->pose_out->scale.z = data->pose_in[0]->scale.z * data->pose_in[1]->scale.z;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseNearest(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1])
	{
		if (*data->param[0] < 0.5f)
		{
			a3spatialPoseConstant(data, tree);
		}
		else
		{
			a3spatialPoseConstant(data, tree);
		}

		return 0;
	}

	return 1;
}

// pointer-based LERP operation for single spatial pose
inline a3ret a3spatialPoseOpLERP(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1])
	{
		a3real3Lerp(data->pose_out->rotate.v, data->pose_in[0]->rotate.v, data->pose_in[1]->rotate.v, *data->param[0]);

		a3real3Lerp(data->pose_out->scale.v, data->pose_in[0]->scale.v, data->pose_in[1]->scale.v, *data->param[0]);

		a3real3Lerp(data->pose_out->translate.v, data->pose_in[0]->translate.v, data->pose_in[1]->translate.v, *data->param[0]);

		// done
		return 0;
	}

	return 1;

}

inline a3ret a3spatialPoseCubicBlend(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1] && data->pose_in[2] && data->pose_in[3])
	{
		// make blend params
		const a3real t = *data->param[0];
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
		a3real4ProductS(out1, data->pose_in[0]->rotate.v, s1);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, data->pose_in[1]->rotate.v, s2);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, data->pose_in[2]->rotate.v, s3);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, data->pose_in[3]->rotate.v, s4);
		a3real4Add(sum, out1);
		a3real4ProductS(sum, sum, 0.5f);

		data->pose_out->rotate.v[0] = sum[0];
		data->pose_out->rotate.v[1] = sum[1];
		data->pose_out->rotate.v[2] = sum[2];

		// translate
		a3real4ProductS(out1, data->pose_in[0]->translate.v, s1);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, data->pose_in[1]->translate.v, s2);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, data->pose_in[2]->translate.v, s3);
		a3real4Add(sum, out1);
		a3real4ProductS(out1, data->pose_in[3]->translate.v, s4);
		a3real4Add(sum, out1);
		a3real4ProductS(sum, sum, 0.5f);

		data->pose_out->translate.v[0] = sum[0];
		data->pose_out->translate.v[1] = sum[1];
		data->pose_out->translate.v[2] = sum[2];

		// scale
		s1, s2, s3, s4 *= 0.5f;
		a3real4Pow(out1, data->pose_in[0]->scale.v, s1);
		a3real4MulComp(sum, out1);
		a3real4Pow(out1, data->pose_in[1]->scale.v, s2);
		a3real4MulComp(sum, out1);
		a3real4Pow(out1, data->pose_in[2]->scale.v, s3);
		a3real4MulComp(sum, out1);
		a3real4Pow(out1, data->pose_in[3]->scale.v, s4);
		a3real4MulComp(sum, out1);
		a3real4Pow(sum, sum, 0.5f);

		data->pose_out->scale.v[0] = sum[0];
		data->pose_out->scale.v[1] = sum[1];
		data->pose_out->scale.v[2] = sum[2];

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseSplit(a3_SpatialPose* pose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs)
{
	if (pose_out && lhs && rhs && rhs->scale.x > 0.0f
		&& rhs->scale.y > 0.0f
		&& rhs->scale.z > 0.0f)
	{
		pose_out->rotate.x = lhs->rotate.x - rhs->rotate.x;
		pose_out->rotate.y = lhs->rotate.y - rhs->rotate.y;
		pose_out->rotate.z = lhs->rotate.z - rhs->rotate.z;

		pose_out->translate.x = lhs->translate.x - rhs->translate.x;
		pose_out->translate.y = lhs->translate.y - rhs->translate.y;
		pose_out->translate.z = lhs->translate.z - rhs->translate.z;

		pose_out->scale.x = lhs->scale.x / rhs->scale.x;
		pose_out->scale.y = lhs->scale.y / rhs->scale.y;
		pose_out->scale.z = lhs->scale.z / rhs->scale.z;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseScale(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0])
	{
		a3spatialPoseOpLERP(data, tree);

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseTriangularBlend(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1] && data->pose_in[2])
	{
		a3real u0 = 1 - *data->param[0] - *data->param[1];

		tree->sposeOps[1].pose_in[0] = data->pose_in[0];
		tree->sposeOps[1].param[0] = &u0;

		tree->sposeOps[2].pose_in[0] = data->pose_in[1];
		tree->sposeOps[2].param[0] = data->param[0];

		tree->sposeOps[3].pose_in[0] = data->pose_in[2];
		tree->sposeOps[3].param[0] = data->param[1];

		// scale the poses for weighted average calculation
		a3spatialPoseScale(&tree->sposeOps[1], tree); // will have to pass a a3_HierarchyPoseBlendOp* as data?
		a3spatialPoseScale(&tree->sposeOps[2], tree);
		a3spatialPoseScale(&tree->sposeOps[3], tree);
		//a3spatialPoseScale(data);
		//a3spatialPoseScale(data);
		//Possibly unroll the operation (triangle here) to be the multiple operations that make it up in the tree

		// concat all
		tree->sposeOps[4].pose_in[0] = tree->sposeOps[1].pose_out;
		tree->sposeOps[4].pose_in[1] = tree->sposeOps[2].pose_out;
		tree->sposeOps[5].pose_in[0] = tree->sposeOps[4].pose_out;
		tree->sposeOps[5].pose_in[1] = tree->sposeOps[3].pose_out;

		a3spatialPoseMerge(&tree->sposeOps[4], tree);
		a3spatialPoseMerge(&tree->sposeOps[5], tree);

		data->pose_out = tree->sposeOps[5].pose_out;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseBinearestBlend(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1] && data->pose_in[2] && data->pose_in[3])
	{
		tree->sposeOps[1].pose_in[0] = data->pose_in[0];
		tree->sposeOps[1].pose_in[1] = data->pose_in[1];
		tree->sposeOps[1].param[0] = data->param[1];

		tree->sposeOps[2].pose_in[0] = data->pose_in[2];
		tree->sposeOps[2].pose_in[1] = data->pose_in[3];
		tree->sposeOps[2].param[0] = data->param[2];

		tree->sposeOps[3].pose_in[0] = tree->sposeOps[1].pose_out;
		tree->sposeOps[3].pose_in[1] = tree->sposeOps[2].pose_out;
		tree->sposeOps[3].param[0] = data->param[0];

		a3spatialPoseNearest(&tree->sposeOps[1], tree);
		a3spatialPoseNearest(&tree->sposeOps[2], tree);
		a3spatialPoseNearest(&tree->sposeOps[3], tree);

		data->pose_out = tree->sposeOps[3].pose_out;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseBilinearBlend(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1] && data->pose_in[2] && data->pose_in[3])
	{
		tree->sposeOps[1].pose_in[0] = data->pose_in[0];
		tree->sposeOps[1].pose_in[1] = data->pose_in[1];
		tree->sposeOps[1].param[0] = data->param[1];

		tree->sposeOps[2].pose_in[0] = data->pose_in[2];
		tree->sposeOps[2].pose_in[1] = data->pose_in[3];
		tree->sposeOps[2].param[0] = data->param[2];

		tree->sposeOps[3].pose_in[0] = tree->sposeOps[1].pose_out;
		tree->sposeOps[3].pose_in[1] = tree->sposeOps[2].pose_out;
		tree->sposeOps[3].param[0] = data->param[0];

		a3spatialPoseOpLERP(&tree->sposeOps[1], tree);
		a3spatialPoseOpLERP(&tree->sposeOps[2], tree);
		a3spatialPoseOpLERP(&tree->sposeOps[3], tree);

		data->pose_out = tree->sposeOps[3].pose_out;

		return 0;
	}

	return 1;
}

inline a3ret a3spatialPoseBicubicBlend(a3_SpatialPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->pose_in[1] && data->pose_in[2] && data->pose_in[3] &&
		data->pose_in[4] && data->pose_in[5] && data->pose_in[6] && data->pose_in[7] &&
		data->pose_in[8] && data->pose_in[9] && data->pose_in[10] && data->pose_in[11] &&
		data->pose_in[12] && data->pose_in[13] && data->pose_in[14] && data->pose_in[15])
	{
		tree->sposeOps[1].pose_in[0] = data->pose_in[0];
		tree->sposeOps[1].pose_in[1] = data->pose_in[1];
		tree->sposeOps[1].pose_in[2] = data->pose_in[2];
		tree->sposeOps[1].pose_in[3] = data->pose_in[3];
		tree->sposeOps[1].param[0] = data->param[1];

		tree->sposeOps[2].pose_in[0] = data->pose_in[4];
		tree->sposeOps[2].pose_in[1] = data->pose_in[5];
		tree->sposeOps[2].pose_in[2] = data->pose_in[6];
		tree->sposeOps[2].pose_in[3] = data->pose_in[7];
		tree->sposeOps[2].param[0] = data->param[2];
		
		tree->sposeOps[3].pose_in[0] = data->pose_in[8];
		tree->sposeOps[3].pose_in[1] = data->pose_in[9];
		tree->sposeOps[3].pose_in[2] = data->pose_in[10];
		tree->sposeOps[3].pose_in[3] = data->pose_in[11];
		tree->sposeOps[3].param[0] = data->param[3];
		
		tree->sposeOps[4].pose_in[0] = data->pose_in[12];
		tree->sposeOps[4].pose_in[1] = data->pose_in[13];
		tree->sposeOps[4].pose_in[2] = data->pose_in[14];
		tree->sposeOps[4].pose_in[3] = data->pose_in[15];
		tree->sposeOps[4].param[0] = data->param[4];

		tree->sposeOps[5].pose_in[0] = tree->sposeOps[1].pose_out;
		tree->sposeOps[5].pose_in[1] = tree->sposeOps[2].pose_out;
		tree->sposeOps[5].pose_in[2] = tree->sposeOps[3].pose_out;
		tree->sposeOps[5].pose_in[3] = tree->sposeOps[4].pose_out;
		tree->sposeOps[5].param[0] = data->param[0];

		a3spatialPoseCubicBlend(&tree->sposeOps[1], tree);
		a3spatialPoseCubicBlend(&tree->sposeOps[2], tree);
		a3spatialPoseCubicBlend(&tree->sposeOps[3], tree);
		a3spatialPoseCubicBlend(&tree->sposeOps[4], tree);
		a3spatialPoseCubicBlend(&tree->sposeOps[5], tree);

		data->pose_out = tree->sposeOps[5].pose_out;

		return 0;
	}

	return 1;
}

//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
inline a3ret a3hierarchyPoseOpIdentity(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = data->pose_in[0]->pose + i;
			tree->sposeOps[1].param[0] = &data->param_in[i];
			a3spatialPoseOpIdentity(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseConstruct(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3vec4	eulers = (a3vec4){ data->param_in[0],data->param_in[1],data->param_in[2],data->param_in[3] },
		scale = (a3vec4){ data->param_in[4],data->param_in[5],data->param_in[6],data->param_in[7] },
		translate = (a3vec4){ data->param_in[8],data->param_in[9],data->param_in[10],data->param_in[11] };
	if (data->pose_out && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
			a3spatialPoseConstruct(data->pose_out->pose + i, eulers, scale, translate);
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseConstant(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = data->pose_in[0]->pose + i;

			//a3spatialPoseConstant(data->pose_out->pose + i, data->pose_in[0]->pose + i);
			a3spatialPoseConstant(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseInvert(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	if (data->pose_out && data->pose_in[0] && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = data->pose_in[0]->pose + i;

			//a3spatialPoseInvert(data->pose_out->pose + i, data->pose_in[0]->pose + i);
			a3spatialPoseInvert(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseMerge(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* lhs = data->pose_in[0];
	const a3_HierarchyPose* rhs = data->pose_in[1];

	if (data->pose_out && lhs && rhs && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = lhs->pose + i;
			tree->sposeOps[1].pose_in[1] = rhs->pose + i;

			//a3spatialPoseMerge(data->pose_out->pose + i, lhs->pose + i, rhs->pose + i);
			a3spatialPoseMerge(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseNearest(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* pose0 = data->pose_in[0];
	const a3_HierarchyPose* pose1 = data->pose_in[1];
	const a3real blendParam = data->param_in[0];

	if (data->pose_out && pose0 && pose1 && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = pose0->pose + i;
			tree->sposeOps[1].pose_in[1] = pose1->pose + i;
			tree->sposeOps[1].param[0] = &blendParam;

			//a3spatialPoseNearest(data->pose_out->pose + i, pose0->pose + i, pose1->pose + i, blendParam);
			a3spatialPoseNearest(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

// pointer-based LERP operation for hierarchical pose
inline a3ret a3hierarchyPoseOpLERP(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* pose0 = data->pose_in[0];
	const a3_HierarchyPose* pose1 = data->pose_in[1];
	const a3real blendParam = data->param_in[0];
	if (data->pose_out && pose0 && pose1 && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = pose0->pose + i;
			tree->sposeOps[1].pose_in[1] = pose1->pose + i;
			tree->sposeOps[1].param[0] = &blendParam;

			//a3spatialPoseLerp(data->pose_out->pose + i, pose0->pose + i, pose1->pose + i, blendParam);
			a3spatialPoseOpLERP(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseOpLookAt(a3_HierarchyStateBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* pose0 = &data->pose_in[0]; // Local space Hierarchy Pose

	const a3real target_index = data->param_in[0];
	const a3real x = data->param_in[1];
	const a3real y = data->param_in[2];
	const a3real z = data->param_in[3];

	//a3_HierarchyPose* poseObjInv = malloc(sizeof(a3_HierarchyPose));

	if (data->pose_out && pose0 && data->hierarchyState) {

		// Solve object space for effector and heierarchy; a3kinematicsSolveForward;
		const a3_HierarchyNode* itr = data->hierarchyState->hierarchy->nodes + (a3ui32)target_index;
		const a3_HierarchyNode* const end = itr + data->hierarchyState->hierarchy->numNodes;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3real4x4Product(data->hierarchyState->objectSpace->pose[itr->index].transformMat.m,
					data->hierarchyState->objectSpace->pose[itr->parentIndex].transformMat.m,
					data->hierarchyState->localSpace->pose[itr->index].transformMat.m);
			else
				data->hierarchyState->objectSpace->pose[itr->index] = data->hierarchyState->localSpace->pose[itr->index];
		}

		////Get Object-Space Inverse for IK later
		//a3index i;
		//for (i = 0; i < data->hierarchyState->hierarchy->numNodes; ++i)
		//	a3real4x4TransformInverse(poseObjInv->pose[i].transformMat.m,
		//		data->pose_out->pose[i].transformMat.m);

		
		//Perform look at
		a3real3 target;
		target[0] = x;
		target[1] = y;
		target[2] = z;

		//Should I be calling a spacialPoseConvert to get node to matrix and then restore later?
		a3real3 neck;
		neck[0] = pose0->pose[(data->hierarchyState->hierarchy->nodes + (a3ui32)target_index)->index].rotate.x;
		neck[1] = pose0->pose[(data->hierarchyState->hierarchy->nodes + (a3ui32)target_index)->index].rotate.y;
		neck[2] = pose0->pose[(data->hierarchyState->hierarchy->nodes + (a3ui32)target_index)->index].rotate.z;

		a3real3 dir;

		a3real3Diff(dir, target, neck);

		a3real3 vx, vy, vz;

		a3real3SetReal3(vz, dir);
		a3real3Normalize(vz);

		a3real3 up;
		a3real3Set(up, 0, 1, 0);

		a3real3Cross(vx, up, vz);

		a3real3Normalize(vx);

		a3real3Cross(vy, vz, vx);

		//pose_out->pose[(hierarchyPose->nodes + (a3ui32)target_index)->index].rotate = R[vx, vy, vz];
		


		/*
		* 1) solve above: gives matrix in this form:
		*	[vx, vy, vz, t]
		* 2) IK: my local = parent object inv * my object (the thing solved above)
		*
		*/

		a3_SpatialPose* affectedPose = data->pose_out->pose + (data->hierarchyState->hierarchy->nodes + (a3ui32)target_index)->index;

		// Solve for world space; a3kinematicsSolveInversePartial
		const a3_HierarchyNode* itr = data->hierarchyState->hierarchy->nodes + (a3ui32)target_index;
		const a3_HierarchyNode* const end = itr + data->hierarchyState->hierarchy->numNodes;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3real4x4Product(data->hierarchyState->localSpace->pose[itr->index].transformMat.m,
					data->hierarchyState->objectSpaceInv->pose[itr->parentIndex].transformMat.m,
					data->hierarchyState->objectSpace->pose[itr->index].transformMat.m);
			else
				data->hierarchyState->localSpace->pose[itr->index] = data->hierarchyState->objectSpace->pose[itr->index];
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseOpChain(a3_HierarchyStateBlendOp* data, a3_BlendTree* tree)
{
	return 1;
}

inline a3ret a3hierarchyPoseCubicBlend(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* pose_pre = data->pose_in[0];
	const a3_HierarchyPose* pose0 = data->pose_in[1];
	const a3_HierarchyPose* pose1 = data->pose_in[2];
	const a3_HierarchyPose* pose_post = data->pose_in[3];
	const a3real blendParam = data->param_in[0];

	if (data->pose_out && pose0 && pose1 && pose_pre && pose_post && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = pose_pre->pose + i;
			tree->sposeOps[1].pose_in[1] = pose0->pose + i;
			tree->sposeOps[1].pose_in[2] = pose1->pose + i;
			tree->sposeOps[1].pose_in[2] = pose_post->pose + i;
			tree->sposeOps[1].param[0] = &blendParam;

			//a3spatialPoseCubicBlend(data->pose_out->pose + i, pose_pre->pose + i, pose0->pose + i, pose1->pose + i, pose_post->pose + i, blendParam);
			a3spatialPoseCubicBlend(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseSplit(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* lhs = data->pose_in[0];
	const a3_HierarchyPose* rhs = data->pose_in[1];

	if (data->pose_out && lhs && rhs && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			a3spatialPoseSplit(data->pose_out->pose + i, lhs->pose + i, rhs->pose + i);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseScale(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3real blendParam = data->param_in[0];

	if (data->pose_out && data->pose_in[0] && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = data->pose_in[0]->pose + i;
			tree->sposeOps[1].param[0] = &blendParam;

			//a3spatialPoseScale(data->pose_out->pose + i, data->pose_in[0]->pose + i, blendParam);
			a3spatialPoseScale(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseTriangularBlend(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* pose0 = data->pose_in[0];
	const a3_HierarchyPose* pose1 = data->pose_in[1];
	const a3_HierarchyPose* pose2 = data->pose_in[2];
	const a3real blendParam0 = data->param_in[0];
	const a3real blendParam1 = data->param_in[1];

	if (data->pose_out && pose0 && pose1 && pose2 && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = pose0->pose + i;
			tree->sposeOps[1].pose_in[1] = pose1->pose + i;
			tree->sposeOps[1].pose_in[2] = pose2->pose + i;
			tree->sposeOps[1].param[0] = &blendParam0;
			tree->sposeOps[1].param[1] = &blendParam1;

			//a3spatialPoseTriangularBlend(data->pose_out->pose + i, pose0->pose + i, pose1->pose + i, pose2->pose + i, blendParam0, blendParam1);
			a3spatialPoseTriangularBlend(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseBinearestBlend(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* init0 = data->pose_in[0];
	const a3_HierarchyPose* init1 = data->pose_in[1];
	const a3_HierarchyPose* final1 = data->pose_in[2];
	const a3_HierarchyPose* final2 = data->pose_in[3];
	const a3real blendParam0 = data->param_in[0];
	const a3real blendParam1 = data->param_in[1];
	const a3real blendParam2 = data->param_in[2];

	if (data->pose_out && init0 && init1 && final1 && final2 && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = init0->pose + i;
			tree->sposeOps[1].pose_in[1] = init1->pose + i;
			tree->sposeOps[1].pose_in[2] = final1->pose + i;
			tree->sposeOps[1].pose_in[3] = final2->pose + i;
			tree->sposeOps[1].param[0] = &blendParam0;
			tree->sposeOps[1].param[1] = &blendParam1;
			tree->sposeOps[1].param[2] = &blendParam2;

			//a3spatialPoseBinearestBlend(data->pose_out->pose + i, init0->pose + i, init1->pose + i, final1->pose + i, final2->pose + i, blendParam0, blendParam1, blendParam2);
			a3spatialPoseBinearestBlend(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseBilinearBlend(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3_HierarchyPose* init0 = data->pose_in[0];
	const a3_HierarchyPose* init1 = data->pose_in[1];
	const a3_HierarchyPose* final0 = data->pose_in[2];
	const a3_HierarchyPose* final1 = data->pose_in[3];
	const a3real blendParam0 = data->param_in[0];
	const a3real blendParam1 = data->param_in[1];
	const a3real blendParam2 = data->param_in[2];

	if (data->pose_out && init0 && init1 && final0 && final1 && data->nodeCount)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = init0->pose + i;
			tree->sposeOps[1].pose_in[1] = init1->pose + i;
			tree->sposeOps[1].pose_in[2] = final0->pose + i;
			tree->sposeOps[1].pose_in[3] = final1->pose + i;
			tree->sposeOps[1].param[0] = &blendParam0;
			tree->sposeOps[1].param[1] = &blendParam1;
			tree->sposeOps[1].param[2] = &blendParam2;

			//a3spatialPoseBilinearBlend(data->pose_out->pose + i, init0->pose + i, init1->pose + i, final0->pose + i, final1->pose + i, blendParam0, blendParam1, blendParam2);
			a3spatialPoseBilinearBlend(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

inline a3ret a3hierarchyPoseBicubicBlend(a3_HierarchyPoseBlendOp* data, a3_BlendTree* tree)
{
	const a3real	blendTotal	= data->param_in[0],
					blendA		= data->param_in[1], 
					blendB		= data->param_in[2], 
					blendC		= data->param_in[3],
					blendD		= data->param_in[4];

	a3_HierarchyPose	const* pose_A0 = data->pose_in[0];
	a3_HierarchyPose	const* pose_A1 = data->pose_in[1];
	a3_HierarchyPose	const* pose_A2 = data->pose_in[2];
	a3_HierarchyPose	const* pose_A3 = data->pose_in[3];
	a3_HierarchyPose	const* pose_B0 = data->pose_in[4];
	a3_HierarchyPose	const* pose_B1 = data->pose_in[5];
	a3_HierarchyPose	const* pose_B2 = data->pose_in[6];
	a3_HierarchyPose	const* pose_B3 = data->pose_in[7];
	a3_HierarchyPose	const* pose_C0 = data->pose_in[8];
	a3_HierarchyPose	const* pose_C1 = data->pose_in[9];
	a3_HierarchyPose	const* pose_C2 = data->pose_in[10];
	a3_HierarchyPose	const* pose_C3 = data->pose_in[11];
	a3_HierarchyPose	const* pose_D0 = data->pose_in[12];
	a3_HierarchyPose	const* pose_D1 = data->pose_in[13];
	a3_HierarchyPose	const* pose_D2 = data->pose_in[14];
	a3_HierarchyPose	const* pose_D3 = data->pose_in[15];
	if (data->pose_out && data->nodeCount	&& pose_A0 && pose_A1 && pose_A2 && pose_A3 && blendA
								&& pose_B0 && pose_B1 && pose_B2 && pose_B3 && blendB
								&& pose_C0 && pose_C1 && pose_C2 && pose_C3 && blendC
								&& pose_D0 && pose_D1 && pose_D2 && pose_D3 && blendD)
	{
		a3index i;
		for (i = 0; i < data->nodeCount; ++i)
		{
			tree->sposeOps[1].pose_out = data->pose_out->pose + i;
			tree->sposeOps[1].pose_in[0] = pose_A0->pose + i;
			tree->sposeOps[1].pose_in[1] = pose_A1->pose + i;
			tree->sposeOps[1].pose_in[2] = pose_A2->pose + i;
			tree->sposeOps[1].pose_in[3] = pose_A3->pose + i;
			tree->sposeOps[1].pose_in[4] = pose_B0->pose + i;
			tree->sposeOps[1].pose_in[5] = pose_B1->pose + i;
			tree->sposeOps[1].pose_in[6] = pose_B2->pose + i;
			tree->sposeOps[1].pose_in[7] = pose_B3->pose + i;
			tree->sposeOps[1].pose_in[8] = pose_C0->pose + i;
			tree->sposeOps[1].pose_in[9] = pose_C1->pose + i;
			tree->sposeOps[1].pose_in[10] = pose_C2->pose + i;
			tree->sposeOps[1].pose_in[11] = pose_C3->pose + i;
			tree->sposeOps[1].pose_in[12] = pose_D0->pose + i;
			tree->sposeOps[1].pose_in[13] = pose_D1->pose + i;
			tree->sposeOps[1].pose_in[14] = pose_D2->pose + i;
			tree->sposeOps[1].pose_in[15] = pose_D3->pose + i;
			tree->sposeOps[1].param[0] = &blendTotal;
			tree->sposeOps[1].param[1] = &blendA;
			tree->sposeOps[1].param[2] = &blendB;
			tree->sposeOps[1].param[3] = &blendC;
			tree->sposeOps[1].param[4] = &blendD;

			/*a3spatialPoseBicubicBlend(data->pose_out->pose + i, blendTotal,
				pose_A0->pose + i, pose_A1->pose + i, pose_A2->pose + i, pose_A3->pose + i, blendA,
				pose_B0->pose + i, pose_B1->pose + i, pose_B2->pose + i, pose_B3->pose + i, blendB,
				pose_C0->pose + i, pose_C1->pose + i, pose_C2->pose + i, pose_C3->pose + i, blendC,
				pose_D0->pose + i, pose_D1->pose + i, pose_D2->pose + i, pose_D3->pose + i, blendD);*/

			a3spatialPoseBicubicBlend(&tree->sposeOps[1], tree);
		}
		return 0;
	}
	return 1;
}

// additional helper functions
//-----------------------------------------------------------------------------

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