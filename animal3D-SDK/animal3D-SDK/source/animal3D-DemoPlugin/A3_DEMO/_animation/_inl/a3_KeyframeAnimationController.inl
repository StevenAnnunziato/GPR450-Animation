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
	//Pre Resolution
	clipCtrl->clipTime += dt;
	clipCtrl->keyframeTime += dt;


	//Resolve Time
	const a3_Clip currentClip = clipCtrl->clipPool->clip[clipCtrl->clipIndex];
	const a3_Keyframe currentKeyframe = currentClip.keyframePool->keyframe[clipCtrl->keyframeIndex];


	
	if (dt > 0) {
		//Time is greater than

		if(clipCtrl->keyframeTime >   )
			//Time is after the keyframe

				//Time is after the clip
	}
	else if (dt < 0) {
		//time is less than 

			//Time is before the keyframe	

				//Time is before the clip
	}
	else {
		//Time is unchanged

	}

	








	

	//Post Resolution


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
	clipCtrl->keyfreamParameter = 0.0f;

	return 0;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_INL
#endif	// __ANIMAL3D_KEYFRAMEANIMATIONCONTROLLER_H