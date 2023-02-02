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
	const a3real changeInTime = clipCtrl->playbackDirection * dt;
	
	//Pre Resolution
	clipCtrl->clipTime += changeInTime;
	clipCtrl->keyframeTime += changeInTime;

	//Resolve Time
	a3_Clip currentClip = clipCtrl->clipPool->clip[clipCtrl->clipIndex];
	a3_Keyframe currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];

	// case 1: continue to advance the current interpolation
	if (clipCtrl->playbackDirection > 0)
	{
		// case 2: the current keyframe is complete
		while (clipCtrl->keyframeTime >= currentKeyframe.duration)
		{
			//Increment to the next keyframe
			clipCtrl->keyframeTime -= currentKeyframe.duration;
			clipCtrl->keyframeIndex++;
			currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];
			// case 3: the whole clip is done
			if (clipCtrl->clipTime >= currentClip.duration)
			{
				//Terminus action
				

				if (clipCtrl->clipIndex >= clipCtrl->clipPool->count - 1)
				{
					// the clip is done playing - call terminus
				}
				// there are more clips, so advance to the next one
				else
				{
					clipCtrl->clipTime -= currentClip.duration;
					clipCtrl->clipIndex++;
					currentClip = clipCtrl->clipPool->clip[clipCtrl->clipIndex];
				}
			}
		}
		
	}
	// case 4: continue to advance the current interpolation
	else if (clipCtrl->playbackDirection < 0)
	{
		// case 5: the current keyframe is complete
		while (clipCtrl->keyframeTime < 0)
		{
			// move to previous keyframe
			clipCtrl->keyframeIndex--;
			currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];
			clipCtrl->keyframeTime += currentKeyframe.duration;
		}
		// case 6: the whole clip is done
		if (clipCtrl->clipTime < 0)
		{
			// if there are no more clips
			if (clipCtrl->clipIndex <= 0)
			{
				// the clip is done playing - call terminus
			}
			// there are more clips, so advance to the next one
			else
			{
				clipCtrl->clipIndex--;
				currentClip = clipCtrl->clipPool->clip[clipCtrl->clipIndex];
				clipCtrl->clipTime -= currentClip.duration;
			}
		}
	}
	// if we are stopped
	else
	{
	}

	// Post Resolution
	clipCtrl->keyframeParameter = clipCtrl->keyframeTime * currentKeyframe.inverseDuration;
	clipCtrl->clipParameter = clipCtrl->clipTime * currentClip.inverseDuration;

	


	return -1;
}

// set clip to play
inline a3i32 a3clipControllerSetClip(a3_ClipController* clipCtrl, const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool)
{
	clipCtrl->clipPool = clipPool;

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