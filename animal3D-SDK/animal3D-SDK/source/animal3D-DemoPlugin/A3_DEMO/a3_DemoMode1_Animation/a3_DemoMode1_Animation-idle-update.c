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
	
	a3_DemoMode1_Animation-idle-update.c
	Demo mode implementations: animation scene.

	********************************************
	*** UPDATE FOR ANIMATION SCENE MODE      ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode1_Animation.h"
#include "../animal3D-DemoPlugin/A3_DEMO/_animation/a3_HierarchyState.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// UTILS

inline a3real4r a3demo_mat2quat_safe(a3real4 q, a3real4x4 const m)
{
	// ****TO-DO: 
	//	-> convert rotation part of matrix to quaternion
	//	-> NOTE: this is for testing dual quaternion skinning only; 
	//		quaternion data would normally be computed with poses

	a3real4SetReal4(q, a3vec4_w.v);

	// done
	return q;
}

inline a3real4x2r a3demo_mat2dquat_safe(a3real4x2 Q, a3real4x4 const m)
{
	// ****TO-DO: 
	//	-> convert matrix to dual quaternion
	//	-> NOTE: this is for testing dual quaternion skinning only; 
	//		quaternion data would normally be computed with poses

	a3demo_mat2quat_safe(Q[0], m);
	a3real4SetReal4(Q[1], a3vec4_zero.v);

	// done
	return Q;
}


//-----------------------------------------------------------------------------

a3_HierarchyPose* a3executeBlendTree(a3_BlendTreeNode* node, const a3ui32 numOfInputs, const a3ui32 blendNodeCount, const a3ui32 hNodeCount)
{
	// create an array of all input poses
	a3_HierarchyPose inPoses[8];
	a3_HierarchyPose* inPosePtr; // 8 max inputs
	inPoses[0].pose = malloc(sizeof(a3_SpatialPose) * hNodeCount * numOfInputs);
	inPosePtr = inPoses;

	// if we rely on any additional inputs...
	if (node->numInputs > 0)
	{
		// for each input pose that still needs to be solved...
		for (a3ui32 i = 0; i < numOfInputs; i++)
		{
			// allocate space for the input poses
			inPoses[i].pose = inPoses[0].pose + hNodeCount * i;
			a3hierarchyPoseReset(&inPoses[i], hNodeCount, NULL, NULL);


			// if there is an input node...
			if (node->inputNodes[i]) {
				// recurse solve the tree 
				a3executeBlendTree(node->inputNodes[i], node->inputNodes[i]->numInputs, blendNodeCount, hNodeCount); //calc children blend nodes
				a3hierarchyPoseCopy(&inPoses[i], node->inputNodes[i]->outPose, hNodeCount); // copy children blend into temp data to be operated on

			}
			
		}
	}
	else // if we don't rely on inputs and just need to sample a clip...
	{
		// nothing to do here - stop recursing
	}

	//operate on all inputs
	if (node->poseOp != 0)
		node->poseOp(node->outPose, hNodeCount, inPosePtr, node->opParams);
	else // no operations on this node, so just copy the in pose into the out pose.
		//a3hierarchyPoseCopy(node->outPose, &inPoses[0], nodeCount); // take the in pose directly

	free(inPoses[0].pose);

	// mask the animation
	a3maskNode(node);

	return node->outPose;
}

//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_DemoSceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_DemoSceneObject* obj_camera, a3_DemoSceneObject* obj_skybox);
void a3demo_update_pointLight(a3_DemoSceneObject* obj_camera, a3_DemoPointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_DemoSceneObject* sceneObject, a3real4x4p s);

void a3animation_update(a3_DemoState* demoState, a3_DemoMode1_Animation* demoMode, a3f64 const dt)
{
	a3ui32 i, j;
	a3_DemoModelMatrixStack matrixStack[animationMaxCount_sceneObject];

	a3_HierarchyState* activeHS = demoMode->hierarchyState_skel + 1, * baseHS = demoMode->hierarchyState_skel;

	// active camera
	a3_DemoProjector const* activeCamera = demoMode->projector + demoMode->activeCamera;
	a3_DemoSceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(demoState, dt,
		demoMode->object_scene, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_scene_ctrl, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(demoState, dt,
		demoMode->object_camera, animationMaxCount_cameraObject, 1, 0);

	a3demo_updateProjectorViewProjectionMat(demoMode->proj_camera_main);

	// apply scales to objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3demo_applyScale_internal(demoMode->object_scene + i, scaleMat.m);
		a3demo_applyScale_internal(demoMode->object_scene_ctrl + i, scaleMat.m);
	}

	// update skybox
	a3demo_update_bindSkybox(demoMode->obj_camera_main, demoMode->obj_skybox);
	
	// update scene graph local transforms
	j = 0;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = a3mat4_identity;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = demoMode->object_camera->modelMat;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = a3mat4_identity;
	demoMode->sceneGraphState->localSpace->pose[j++].transformMat = demoMode->obj_skeleton_ctrl->modelMat;
	// start of scene objects
	demoMode->sceneGraphState->localSpace->pose[j + 0].transformMat = demoMode->obj_skybox->modelMat;
	demoMode->sceneGraphState->localSpace->pose[j + 1].transformMat = demoMode->obj_skeleton->modelMat;
	a3kinematicsSolveForward(demoMode->sceneGraphState);
	a3hierarchyStateUpdateObjectInverse(demoMode->sceneGraphState);

	// update matrix stack data using scene graph
	for (i = 0; i < animationMaxCount_sceneObject; ++i, ++j)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m,
			demoMode->sceneGraphState->objectSpace->pose[demoMode->obj_camera_main->sceneGraphIndex].transformMat.m,
			demoMode->sceneGraphState->objectSpaceInv->pose[demoMode->obj_camera_main->sceneGraphIndex].transformMat.m,
			demoMode->sceneGraphState->objectSpace->pose[demoMode->obj_skeleton->sceneGraphIndex].transformMat.m,
			a3mat4_identity.m);
	}


	// skeletal
	if (demoState->updateAnimation)
	{
		a3real const dtr = (a3real)dt;
		//a3_ClipController* clipCtrl = &demoMode->blendTree->clipControllers[0];
		//a3_ClipController* clipCtrlA = &demoMode->blendTree->clipControllers[1];
		//a3_ClipController* clipCtrlB = &demoMode->blendTree->clipControllers[2];

		// update clip controllers in the blend tree
		for (a3ui32 i = 0; i < demoMode->blendTree->clipCount; i++)
		{
			a3clipControllerUpdate(&demoMode->blendTree->clipControllers[i], dt);
		}

		// first update the nodes which have no inputs and just sample from a clip
		// for each node in the blend tree...
		for (a3ui32 i = 0; i < demoMode->blendTree->nodeCount; i++)
		{
			// if the node is a clip node...
			if (demoMode->blendTree->nodes[i].numInputs <= 0)
			{
				// get the clip controller
				// copy the pose from the clip controller to the node's out pose
				a3hierarchyPoseCopy(demoMode->blendTree->nodes[i].outPose,
										demoMode->hierarchyPoseGroup_skel->hpose + demoMode->blendTree->nodes[i].myClipController->keyframeIndex, // get deltas of a pose in frame keyframeIndex
										demoMode->hierarchy_skel->numNodes);
			}
		}

		// finally execute the nodes of the blend tree in order
		const a3ui32 rootIndex = 0; // note: root index is assumed to be zero
		a3executeBlendTree(&demoMode->blendTree->nodes[rootIndex], demoMode->blendTree->nodes[rootIndex].numInputs, demoMode->blendTree->nodeCount, demoMode->hierarchy_skel->numNodes);

		// FK pipeline
		/*a3_HierarchyPose fkInputs[2] = (a3_HierarchyPose[]){
			baseHS->localSpace, // holds base pose
				demoMode->blendTree->nodes[rootIndex].outPose // output from the tree recurse
		};*/
		a3_HierarchyPose fkInputs[2];
		a3_HierarchyPose* fkInputPtr = fkInputs;
		fkInputs[0] = *baseHS->localSpace;
		fkInputs[1] = *demoMode->blendTree->nodes[rootIndex].outPose;
		
		a3hierarchyPoseConcat(activeHS->localSpace,	// goal to calculate
			demoMode->hierarchy_skel->numNodes,
			fkInputPtr, // holds current sample pose
			NULL);
		a3hierarchyPoseConvert(activeHS->localSpace,
			demoMode->hierarchy_skel->numNodes,
			demoMode->hierarchyPoseGroup_skel->channel,
			demoMode->hierarchyPoseGroup_skel->order);
		a3kinematicsSolveForward(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

		// ****TO-DO: 
		// process input

		// apply input
		// switch on input mode and move the character
		switch (demoMode->ctrl_position)
		{
			// direct assignment of position
		case animation_input_direct:
			break;
			// Euler integration (integrate velocity into position)
		case animation_input_euler:
			demoMode->pos.y = demoMode->pos.y + demoMode->vel.y * (a3real)dt;
			break;
			// kinematic integration (integrate acceleration into velocity, and velocity into position)
		case animation_input_kinematic:
			demoMode->vel.y = demoMode->vel.y + demoMode->acc.y * (a3real)dt;
			demoMode->pos.y = demoMode->pos.y + demoMode->vel.y * (a3real)dt;
			break;
			// interpolate to target value
		case animation_input_interpolate1:
			{
				a3vec2 target;
				target.y = demoMode->vel.y*5.0f;
				demoMode->pos.y = a3lerp(demoMode->pos.y, target.y, (a3real)dt);
			}
			break;
			// interpolate to target velocity
		case animation_input_interpolate2:
			{
				a3vec2 target;
				target.y = demoMode->acc.y * 5.0f;
				demoMode->vel.y = a3lerp(demoMode->vel.y, target.y, (a3real)dt);
				demoMode->pos.y = demoMode->pos.y + demoMode->vel.y * (a3real)dt;
			}
			break;
		default:
			break;
		}

		demoMode->obj_skeleton_ctrl->position.x = a3real4x4MulTransform() + (demoMode->pos.x);
		demoMode->obj_skeleton_ctrl->position.y = +(demoMode->pos.y);

		switch (demoMode->ctrl_rotation)
		{
			// direct assignment of position
		case animation_input_direct:
			break;
			// Euler integration (integrate velocity into position)
		case animation_input_euler:
			demoMode->rot = demoMode->rot + demoMode->velr * (a3real)dt * 90.0f;
			break;
			// kinematic integration (integrate acceleration into velocity, and velocity into position)
		case animation_input_kinematic:
			demoMode->velr = demoMode->velr + demoMode->accr * (a3real)dt * 90.0f;
			demoMode->rot = demoMode->rot + demoMode->velr * (a3real)dt;
			break;
			// interpolate to target value
		case animation_input_interpolate1:
			{
				a3real target;
				target = demoMode->velr * 90.0f;
				demoMode->rot = a3lerp(demoMode->rot, target, (a3real)dt);
			}
			break;
			// interpolate to target velocity
		case animation_input_interpolate2:
			{
				a3real target;
				target = demoMode->accr * 90.0f;
				demoMode->velr = a3lerp(demoMode->velr, target, (a3real)dt);
				demoMode->rot = demoMode->rot + demoMode->velr * (a3real)dt;

			}
			break;
		default:
			break;
		}


		demoMode->obj_skeleton_ctrl->euler.z = -a3trigValid_sind(demoMode->rot);
	}


	// prepare and upload graphics data
	{
		a3addressdiff const skeletonIndex = demoMode->obj_skeleton - demoMode->object_scene;
		a3ui32 const mvp_size = demoMode->hierarchy_skel->numNodes * sizeof(a3mat4);
		a3ui32 const t_skin_size = sizeof(demoMode->t_skin);
		a3ui32 const dq_skin_size = sizeof(demoMode->dq_skin);
		a3mat4 const mvp_obj = matrixStack[skeletonIndex].modelViewProjectionMat;
		a3mat4* mvp_joint, * mvp_bone, * t_skin;
		a3dualquat* dq_skin;
		a3index i;
		a3i32 p;
		
		// update joint and bone transforms
		for (i = 0; i < demoMode->hierarchy_skel->numNodes; ++i)
		{
			mvp_joint = demoMode->mvp_joint + i;
			mvp_bone = demoMode->mvp_bone + i;
			t_skin = demoMode->t_skin + i;
			dq_skin = demoMode->dq_skin + i;

			// joint transform
			a3real4x4SetScale(scaleMat.m, a3real_quarter);
			a3real4x4Concat(activeHS->objectSpace->pose[i].transformMat.m, scaleMat.m);
			a3real4x4Product(mvp_joint->m, mvp_obj.m, scaleMat.m);
			
			// bone transform
			p = demoMode->hierarchy_skel->nodes[i].parentIndex;
			if (p >= 0)
			{
				// position is parent joint's position
				scaleMat.v3 = activeHS->objectSpace->pose[p].transformMat.v3;

				// direction basis is from parent to current
				a3real3Diff(scaleMat.v2.v,
					activeHS->objectSpace->pose[i].transformMat.v3.v, scaleMat.v3.v);

				// right basis is cross of some upward vector and direction
				// select 'z' for up if either of the other dimensions is set
				a3real3MulS(a3real3CrossUnit(scaleMat.v0.v,
					a3real2LengthSquared(scaleMat.v2.v) > a3real_zero
					? a3vec3_z.v : a3vec3_y.v, scaleMat.v2.v), a3real_quarter);
			
				// up basis is cross of direction and right
				a3real3MulS(a3real3CrossUnit(scaleMat.v1.v,
					scaleMat.v2.v, scaleMat.v0.v), a3real_quarter);
			}
			else
			{
				// if we are a root joint, make bone invisible
				a3real4x4SetScale(scaleMat.m, a3real_zero);
			}
			a3real4x4Product(mvp_bone->m, mvp_obj.m, scaleMat.m);

			// get base to current object-space
			*t_skin = activeHS->objectSpaceBindToCurrent->pose[i].transformMat;

			// calculate DQ
			a3demo_mat2dquat_safe(dq_skin->Q, t_skin->m);
		}
		
		// upload
		a3bufferRefill(demoState->ubo_transformMVP, 0, mvp_size, demoMode->mvp_joint);
		a3bufferRefill(demoState->ubo_transformMVPB, 0, mvp_size, demoMode->mvp_bone);
		a3bufferRefill(demoState->ubo_transformBlend, 0, t_skin_size, demoMode->t_skin);
		a3bufferRefillOffset(demoState->ubo_transformBlend, 0, t_skin_size, dq_skin_size, demoMode->dq_skin);
	}
}


//-----------------------------------------------------------------------------
