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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements

		// allocate everything (one malloc)
		//??? = (...)malloc(sz);
		poseGroup_out->spatialPosePool = malloc(sizeof(a3_SpatialPose) * poseCount * hierarchy->numNodes + sizeof(a3_HierarchyPose) * poseCount);

		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hierarchicalPoses = (a3_HierarchyPose*)((a3_SpatialPose*)(poseGroup_out->spatialPosePool) + poseCount * hierarchy->numNodes);
		poseGroup_out->poseCount = poseCount;
		

		for (a3ui32 i = 0; i < poseCount * hierarchy->numNodes; ++i) 
		{
			a3spatialPoseInit(&poseGroup_out->spatialPosePool[i], a3mat4_identity);
		}

		//doesn't do anything because spatial pose data isn't loaded
		for (a3ui32 j = 0, i = 0; i < poseCount && j < poseCount * hierarchy->numNodes; ++i, j+= hierarchy->numNodes) {
			poseGroup_out->hierarchicalPoses[i].spatialPose = &poseGroup_out->spatialPosePool[j];

		}

		// done
		return 0;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		//free(???);
		free(poseGroup->spatialPosePool);

		// reset pointers
		poseGroup->spatialPosePool = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// init hierarchy pose
a3ui32 a3hierarchyPoseInit(a3_HierarchyPose* pose_inout, const a3_SpatialPose* spatialPoseHead)
{
	pose_inout->spatialPose = (a3_SpatialPose*)spatialPoseHead;
	return 0;
}

//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	const a3ui8 NUM_OF_POSE_STATES = 3;

	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		// allocate everything (one malloc)
		// ??? = (...)malloc(sz);
		state_out->samplePose = malloc(sizeof(a3_HierarchyPose) * NUM_OF_POSE_STATES * hierarchy->numNodes);
		state_out->localSpacePose = (a3_HierarchyPose*)state_out->samplePose + 1 * hierarchy->numNodes;
		state_out->objectSpacePose = (a3_HierarchyPose*)state_out->samplePose + 2 * hierarchy->numNodes;

		// reset all data
		a3hierarchyPoseInit(state_out->samplePose, NULL);
		a3hierarchyPoseInit(state_out->localSpacePose, NULL);
		a3hierarchyPoseInit(state_out->objectSpacePose, NULL);

		// done
		return 0;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		// release everything (one free)
		//free(???);
		free(state->samplePose);

		// reset pointers
		//state->hierarchy = 0;

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->poseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->poseCount && hierarchy_out && !hierarchy_out->numNodes && resourceFilePath && *resourceFilePath)
	{

	}
	return -1;
}


//-----------------------------------------------------------------------------
