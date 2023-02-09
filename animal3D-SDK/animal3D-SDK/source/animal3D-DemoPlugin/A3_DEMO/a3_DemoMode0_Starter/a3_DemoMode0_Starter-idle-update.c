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
	
	a3_DemoMode0_Starter_idle-update.c
	Demo mode implementations: starter scene.

	********************************************
	*** UPDATE FOR STARTER SCENE MODE        ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Starter.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);

void a3starter_update(a3_DemoState* demoState, a3_DemoMode0_Starter* demoMode, a3f64 const dt)
{
	a3ui32 i;
	a3_DemoModelMatrixStack matrixStack[starterMaxCount_sceneObject];

	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(demoState, dt,
		demoMode->object_scene, starterMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_camera, starterMaxCount_cameraObject, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	a3demo_update_defaultAnimation(demoState, dt, demoMode->obj_box, 6, 2);

	// apply scales to starter objects
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);

	// update matrix stack data
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			demoMode->object_scene[i].modelMat.m, a3mat4_identity.m);
	}

	// update clip controller, tick keyframes etc
	a3clipControllerUpdate(&demoMode->clipController, (a3real)dt);

	// apply animation controller values to scene objects
	a3_Clip currentClip = demoMode->clipPool.clip[demoMode->clipController.clipIndex];
	a3ui32 currentKeyframeIndex = demoMode->clipController.keyframeIndex;
	a3_Keyframe currentKeyframe = currentClip.keyframePool->keyframe[currentKeyframeIndex];

	// get data from this and next keyframes
	a3real x0 = currentKeyframe.val_x;
	a3real y0 = currentKeyframe.val_y;
	a3real z0 = currentKeyframe.val_z;
	a3real x1, y1, z1 = 0;

	// cache parametric time
	a3real t = demoMode->clipController.keyframeParameter;

	// moving forward
	if (demoMode->clipController.playbackSpeed > 0)
	{
		// if this is the last frame
		if (currentKeyframeIndex >= currentClip.lastKeyframeIndex)
		{
			a3_Clip nextClip = demoMode->clipPool.clip[currentClip.forwardTransition.targetClipIndex];

			// determine target keyframe index based on direction of the new clip
			a3ui32 targetKeyframeIndex;
			if (currentClip.forwardTransition.playbackSpeed > 0)
				targetKeyframeIndex = nextClip.firstKeyframeIndex;
			else if (currentClip.forwardTransition.playbackSpeed < 0)
				targetKeyframeIndex = nextClip.lastKeyframeIndex;

			// set data
			x1 = nextClip.keyframePool->keyframe[targetKeyframeIndex].val_x;
			y1 = nextClip.keyframePool->keyframe[targetKeyframeIndex].val_y;
			z1 = nextClip.keyframePool->keyframe[targetKeyframeIndex].val_z;
		}
		else // not the last keyframe
		{
			x1 = currentClip.keyframePool->keyframe[currentKeyframeIndex + 1].val_x;
			y1 = currentClip.keyframePool->keyframe[currentKeyframeIndex + 1].val_y;
			z1 = currentClip.keyframePool->keyframe[currentKeyframeIndex + 1].val_z;
		}
	}
	else if (demoMode->clipController.playbackSpeed < 0) // moving backward
	{ 
		// if this is the last frame
		if (currentKeyframeIndex <= currentClip.firstKeyframeIndex)
		{
			a3_Clip nextClip = demoMode->clipPool.clip[currentClip.backwardTransition.targetClipIndex];

			// determine target keyframe index based on direction of the new clip
			a3ui32 targetKeyframeIndex;
			if (currentClip.forwardTransition.playbackSpeed > 0)
				targetKeyframeIndex = nextClip.firstKeyframeIndex;
			else if (currentClip.forwardTransition.playbackSpeed < 0)
				targetKeyframeIndex = nextClip.lastKeyframeIndex;

			// set data
			x1 = nextClip.keyframePool->keyframe[targetKeyframeIndex].val_x;
			y1 = nextClip.keyframePool->keyframe[targetKeyframeIndex].val_y;
			z1 = nextClip.keyframePool->keyframe[targetKeyframeIndex].val_z;
		}
		else // not the last keyframe
		{
			x1 = currentClip.keyframePool->keyframe[currentKeyframeIndex - 1].val_x;
			y1 = currentClip.keyframePool->keyframe[currentKeyframeIndex - 1].val_y;
			z1 = currentClip.keyframePool->keyframe[currentKeyframeIndex - 1].val_z;
		}

		// flip t value if needed
		t = 1 - t;
	}

	// set the torus's position
	demoMode->obj_torus->position.x = lerp(x0, x1, t);
	demoMode->obj_torus->position.y = lerp(y0, y1, t);
	demoMode->obj_torus->position.z = lerp(z0, z1, t);

	/*demoMode->obj_torus->position.x = x0;
	demoMode->obj_torus->position.y = y0;
	demoMode->obj_torus->position.z = z0;*/

	// print debug info of which frame we're on
	printf("keyframe index %i\n", demoMode->clipController.keyframeIndex);
}


//-----------------------------------------------------------------------------
