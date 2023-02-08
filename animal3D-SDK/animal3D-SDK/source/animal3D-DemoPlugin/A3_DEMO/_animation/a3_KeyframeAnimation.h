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
	
	a3_KeyframeAnimation.h
	Data structures for fundamental keyframe animation.
*/

#ifndef __ANIMAL3D_KEYFRAMEANIMATION_H
#define __ANIMAL3D_KEYFRAMEANIMATION_H


#include "animal3D-A3DM/a3math/a3vector.h"
#include "animal3D-A3DM/a3math/a3interpolation.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
	struct a3_ClipTransition;
#else	// !__cplusplus
typedef struct a3_Keyframe					a3_Keyframe;
typedef struct a3_KeyframePool				a3_KeyframePool;
typedef struct a3_Clip						a3_Clip;
typedef struct a3_ClipPool					a3_ClipPool;
typedef struct a3_ClipTransition			a3_ClipTransition;
#endif	// __cplusplus

//-----------------------------------------------------------------------------



// constant values
enum
{
	a3keyframeAnimation_nameLenMax = 32,
};


// description of single keyframe
// metaphor: moment
struct a3_Keyframe
{
	// index in keyframe pool
	a3ui32 index;

	// duration of keyframe
	a3real duration;
	a3real inverseDuration;

	// data to store in the keyframe
	a3i32 val_x;
	a3i32 val_y;
	a3i32 val_z;
};

// pool of keyframe descriptors
struct a3_KeyframePool
{
	// array of keyframes
	a3_Keyframe *keyframe;

	// number of keyframes
	a3ui32 count;
};

// allocate keyframe pool
a3i32 a3keyframePoolCreate(a3_KeyframePool* keyframePool_out, const a3ui32 count);

// release keyframe pool
a3i32 a3keyframePoolRelease(a3_KeyframePool* keyframePool);

// initialize keyframe
a3i32 a3keyframeInit(a3_Keyframe* keyframe_out, const a3real duration, const a3ui32 value_x, const a3ui32 value_y, const a3ui32 value_z);


//-----------------------------------------------------------------------------

// description of single clip
// metaphor: timeline
struct a3_Clip
{
	// clip name
	a3byte name[a3keyframeAnimation_nameLenMax];

	// index in clip pool
	a3ui32 index;

	// duration of clip - sum of all keyframe durations
	a3real duration;
	a3real inverseDuration;

	// keyframe info
	a3ui32 keyframeCount;
	a3ui32 firstKeyframeIndex;
	a3ui32 lastKeyframeIndex;
	a3_KeyframePool* keyframePool;

	// transition info
	a3_ClipTransition* forwardTransition;
	a3_ClipTransition* backwardTransition;
};

// group of clips
struct a3_ClipPool
{
	// array of clips
	a3_Clip* clip;

	// number of clips
	a3ui32 count;
};

// transition from clip to clip
struct a3_ClipTransition
{
	a3_ClipPool* targetClipPool;
	a3ui32 targetClipIndex;
	a3real playbackSpeed;
};

// allocate clip pool
a3i32 a3clipPoolCreate(a3_ClipPool* clipPool_out, const a3ui32 count);

// release clip pool
a3i32 a3clipPoolRelease(a3_ClipPool* clipPool);

// initialize clip with first and last indices
a3i32 a3clipInit(a3_Clip* clip_out, const a3byte clipName[a3keyframeAnimation_nameLenMax], const a3_KeyframePool* keyframePool, const a3ui32 firstKeyframeIndex, const a3ui32 finalKeyframeIndex, const a3_ClipTransition* forwardTransition, const a3_ClipTransition* backwardTransition);

// get clip index from pool
a3i32 a3clipGetIndexInPool(const a3_ClipPool* clipPool, const a3byte clipName[a3keyframeAnimation_nameLenMax]);

// calculate clip duration as sum of keyframes' durations
a3i32 a3clipCalculateDuration(a3_Clip* clip);

// calculate keyframes' durations by distributing clip's duration
a3i32 a3clipDistributeDuration(a3_Clip* clip, const a3real newClipDuration);

// initialize a clip transition
a3i32 a3clipTransitionInit(a3_ClipTransition* transition_out, const a3_ClipPool* targetClipPool, const a3ui32 targetClipIndex, const a3real playbackSpeed);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_KeyframeAnimation.inl"


#endif	// !__ANIMAL3D_KEYFRAMEANIMATION_H