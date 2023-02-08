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
	keyframePool_out->count = count;

	// make sure the data was allocated successfully
	if (keyframePool_out->keyframe == NULL)
	{
		return -1;
	}

	// initialize all to default values
	for (a3ui32 i = 0; i < count; i++)
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
	// allocate clip array
	clipPool_out->clip = (a3_Clip*)malloc(sizeof(a3_Clip) * count);
	clipPool_out->count = count;

	// make sure the clips were allocated successfully
	if (clipPool_out->clip == NULL)
	{
		return -1;
	}

	// initialize clip data
	for (a3ui32 i = 0; i < count; i++)
	{
		//a3clipInit(&clipPool_out->clip[i], "", NULL, 0, 0);
	}

	return 0;
}

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool)
{
	// TODO: check that clipPool is valid and not null

	free(clipPool->clip);
	return 0;
}

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex, const a3_ClipTransition* forwardTransition, const a3_ClipTransition* backwardTransition)
{
	*(clip_out->name) = *clipName;
	clip_out->keyframePool = (a3_KeyframePool*)keyframePool;
	clip_out->keyframeCount = keyframePool->count;
	clip_out->firstKeyframeIndex = firstKeyframeIndex;
	clip_out->lastKeyframeIndex = finalKeyframeIndex;
	a3clipCalculateDuration(clip_out);

	// set clip transitions
	a3clipTransitionCopy(&clip_out->forwardTransition, forwardTransition);
	a3clipTransitionCopy(&clip_out->backwardTransition, backwardTransition);


	return 0;
}

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax])
{
	// linear search for the given clipName
	for (a3ui32 i = 0; i < clipPool->count; i++)
	{
		if (clipPool->clip[i].name == clipName) // TODO: fix
			return i;
	}

	// no clip found
	return -1;
}

a3i32 a3clipTransitionInit(a3_ClipTransition* transition_out, const a3_ClipPool* targetClipPool, const a3ui32 targetClipIndex, const a3real playbackSpeed)
{
	transition_out->targetClipPool = (a3_ClipPool*)targetClipPool;
	transition_out->targetClipIndex = targetClipIndex;
	transition_out->playbackSpeed = playbackSpeed;

	return 0;
}

a3i32 a3clipTransitionCopy(a3_ClipTransition* transition_out, const a3_ClipTransition* transition_in)
{
	return a3clipTransitionInit(transition_out, transition_in->targetClipPool, transition_in->targetClipIndex, transition_in->playbackSpeed);
}

//-----------------------------------------------------------------------------
