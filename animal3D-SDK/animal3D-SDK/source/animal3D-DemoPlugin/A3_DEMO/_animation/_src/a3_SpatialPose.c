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
	
	a3_SpatialPose.c
	Implementation of spatial pose.
*/

#include "../a3_SpatialPose.h"


//-----------------------------------------------------------------------------

// general init
a3i32 a3spatialPoseInit(a3_SpatialPose* spatialPose, const a3mat4 transform_mat)
{
	if (spatialPose)
	{
		spatialPose->transform = transform_mat;
		
		a3spatialPoseSetScale(spatialPose, 1.0f, 1.0f, 1.0f);
		a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, 0.0f);
		a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, 0.0f);
		
		return 0;
	}

	return -1;
}

//-----------------------------------------------------------------------------

a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* lhs, const a3_SpatialPose* rhs)
{
	// TODO: set transform matrix
	// TODO: check flags
	spatialPose_out->eulerX = lhs->eulerX + rhs->eulerX; // validate sum within range
	spatialPose_out->eulerY = lhs->eulerY + rhs->eulerY;
	spatialPose_out->eulerZ = lhs->eulerZ + rhs->eulerZ;

	spatialPose_out->translationX = lhs->translationX + rhs->translationX;
	spatialPose_out->translationY = lhs->translationY + rhs->translationY;
	spatialPose_out->translationZ = lhs->translationZ + rhs->translationZ;

	spatialPose_out->scaleX = lhs->scaleX * rhs->scaleX;
	spatialPose_out->scaleY = lhs->scaleY * rhs->scaleY;
	spatialPose_out->scaleZ = lhs->scaleZ * rhs->scaleZ;

	return -1;
}