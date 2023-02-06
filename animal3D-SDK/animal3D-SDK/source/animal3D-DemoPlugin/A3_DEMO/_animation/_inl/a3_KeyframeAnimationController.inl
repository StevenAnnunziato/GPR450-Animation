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
	
	a3_KeyframeAnimationController.h
	inline definitions for keyframe animation controller.
*/

#ifdef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H
#ifndef __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#define __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL


//-----------------------------------------------------------------------------

// update clip controller
inline a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, const a3real dt)
{
	const a3real changeInTime = clipCtrl->playbackSpeed * dt;
	
	//Pre Resolution
	clipCtrl->clipTime += changeInTime;
	clipCtrl->keyframeTime += changeInTime;

	//Resolve Time
	a3_Clip currentClip = clipCtrl->clipPool->clip[clipCtrl->clipIndex];
	a3_Keyframe currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];

	// case 1: continue to advance the current interpolation
	if (clipCtrl->playbackSpeed > 0)
	{
		// case 2: the current keyframe is complete
		while (clipCtrl->keyframeTime >= currentKeyframe.duration)
		{
			//Increment to the next keyframe
			clipCtrl->keyframeTime -= currentKeyframe.duration;
			clipCtrl->keyframeIndex++;

			// case 3: the whole clip is done
			if (clipCtrl->keyframeIndex >= currentClip.keyframeCount)
			{
				// Terminus - refer to the current clip's forward transition
				// re-init clip controller data
				clipCtrl->clipPool = currentClip.forwardTransition->targetClipPool;
				clipCtrl->clipIndex = currentClip.forwardTransition->targetClipIndex;
				clipCtrl->playbackSpeed = currentClip.forwardTransition->playbackSpeed;
			}

			currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];
		}
		
	}
	// case 4: continue to advance the current interpolation
	else if (clipCtrl->playbackSpeed < 0)
	{
		// case 5: the current keyframe is complete
		while (clipCtrl->keyframeTime < 0)
		{
			// case 6: the whole clip is done
			if (clipCtrl->keyframeIndex == 0)
			{
				// Terminus - refer to the current clip's backward transition
				// re-init clip controller data
				clipCtrl->clipPool = currentClip.backwardTransition->targetClipPool;
				clipCtrl->clipIndex = currentClip.backwardTransition->targetClipIndex;
				clipCtrl->playbackSpeed = currentClip.backwardTransition->playbackSpeed;
			}
			else
			{
				//Decrement Keyframe index{
				clipCtrl->keyframeIndex--;
			}
			
		// resolve keyframe time
		currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];
		clipCtrl->keyframeTime += currentKeyframe.duration;
		}

	}
	// case 7: we are stopped
	else
	{
		// do nothing
	}

	// Post Resolution
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * currentKeyframe.inverseDuration;
	clipCtrl->clipParameter = clipCtrl->clipTime * currentClip.inverseDuration;

	return 0;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	(clipCtrl->clipPool) = (a3_ClipPool*)clipPool;

	clipCtrl->clipIndex = clipIndex_pool;
	clipCtrl->clipTime = 0.0f;
	clipCtrl->clipParameter = 0.0f;

	const a3_Clip currentClip = clipCtrl->clipPool->clip[clipCtrl->clipIndex];
	if (currentClip.keyframePool == 0) {
		return -1;
	}

	clipCtrl->keyframeIndex = currentClip.firstKeyframeIndex;
	clipCtrl->keyframeTime = 0.0f;
	clipCtrl->keyframeParameter = 0.0f;

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H