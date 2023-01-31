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
	
	a3_KeyframeAnimation.c
	Implementation of keyframe animation interfaces.
*/

#include "../a3_KeyframeAnimation.h"

#include <stdlib.h>
#include <string.h>


// macros to help with names
#define A3_CLIP_DEFAULTNAME		("unnamed clip")
#define A3_CLIP_SEARCHNAME		((clipName && *clipName) ? clipName : A3_CLIP_DEFAULTNAME)


//-----------------------------------------------------------------------------

// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count)
{
	// allocate keyframe array on the heap
	keyframePool_out->keyframe = (a3_Keyframe*)malloc(sizeof(a3_Keyframe) * count);

	// make sure the data was allocated successfully
	if (keyframePool_out->keyframe == NULL)
	{
		return -1;
	}

	// initialize all to default values
	for (int i = keyframePool_out->keyframe; i < keyframePool_out->keyframe + count; i++)
	{
		a3keyframeInit(&keyframePool_out->keyframe[i], 0.0f, 0);
	}

	return 0;
}

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool)
{
	free(keyframePool->keyframe);
	return 0;
}

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x)
{
	keyframe_out->duration = duration;
	keyframe_out->inverseDuration = 1.0f / duration;
	keyframe_out->data = value_x;
	return 0;
}


// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 count)
{
	clipPool_out->count = count;

	// allocate clip array
	clipPool_out->clip = (a3_Clip*)malloc(sizeof(a3_Clip) * count);

	// make sure the clips were allocated successfully
	if (clipPool_out->clip == NULL)
	{
		return -1;
	}

	// initialize clip data
	for (int i = clipPool_out->clip; i < clipPool_out->clip + count; i++)
	{
		a3clipInit(&clipPool_out->clip[i], NULL, NULL, 0, 0);
	}

	return 0;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	free(clipPool->clip);
	return 0;
}

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex)
{
	*(clip_out->name) = clipName;
	clip_out->keyframePool = keyframePool;
	clip_out->firstKeyframeIndex = firstKeyframeIndex;
	clip_out->lastKeyframeIndex = finalKeyframeIndex;
	return 0;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	// linear search for the given clipName
	for (int i = 0; i < clipPool->count; i++)
	{
		if (clipPool->clip[i].name == clipName)
			return i;
	}

	// no clip found
	return -1;
}

// calculate clip duration as sum of keyframes' durations
a3i32 a3clipCalculateDuration(a3_Clip* clip)
{
	// find the sum of all keyframe durations
	a3real sum = 0;
	for (int i = clip->firstKeyframeIndex; i <= clip->lastKeyframeIndex; i++)
	{
		sum += clip->keyframePool->keyframe[i].duration;
	}

	clip->duration = sum;
	clip->inverseDuration = 1.0f / sum;

	return 0;
}

// calculate keyframes' durations by distributing clip's duration
a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration)
{
	// set the new clip duration
	clip->duration = newClipDuration;
	clip->inverseDuration = 1.0f / newClipDuration;

	// assign a uniform time to all keyframes
	a3real keyframeDuration = newClipDuration / clip->keyframeCount;
	a3real invDuration = 1.0f / keyframeDuration;
	for (int i = clip->firstKeyframeIndex; i <= clip->lastKeyframeIndex; i++)
	{
		clip->keyframePool->keyframe[i].duration = keyframeDuration;
		clip->keyframePool->keyframe[i].inverseDuration = invDuration;
	}

	return 0;
}

//-----------------------------------------------------------------------------
