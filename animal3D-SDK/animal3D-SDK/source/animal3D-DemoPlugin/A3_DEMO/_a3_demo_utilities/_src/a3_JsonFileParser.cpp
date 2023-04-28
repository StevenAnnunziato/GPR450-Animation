#include "A3_DEMO/_a3_demo_utilities/a3_JsonFileParser.h"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <animal3D/a3/a3types_integer.h>
#include <A3_DEMO/_animation/a3_BlendTree.h>
#include <A3_DEMO/a3_DemoMode1_Animation.h>

using json = nlohmann::json;

#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_ANIM	A3_DEMO_RES_DIR"animdata/"

a3ui32 const rate = 24;
a3f64 const fps = (a3f64)rate;

void a3ReadBlendTreeFromFile(a3_BlendTree* out_blendTree, const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_DemoMode1_Animation* demoMode)
{

	static a3ui32 numOfClipControllers = 0;
	std::string filepath = std::string(A3_DEMO_ANIM) + std::string(fileName);
	std::ifstream f(filepath);
	if (f.is_open()) {
		json data = json::parse(f);
		std::string s = data.dump();
		printf(std::to_string(data["nodes"].size()).c_str());

		const a3ui32 numOfBlendNodes = (a3ui32)data["nodes"].size();


		/*
		* Create Blend Tree
		*
		* a3createBlendTree(out_pool, numOfBlendNodes, demoMode);
		*/
		out_blendTree->nodeCount = numOfBlendNodes;
		out_blendTree->poses = (a3_HierarchyPose*)malloc(sizeof(a3_HierarchyPose) * out_blendTree->nodeCount);
		out_blendTree->poses->pose = (a3_SpatialPose*)malloc(sizeof(a3_SpatialPose) * out_blendTree->nodeCount * demoMode->hierarchyState_skel->hierarchy->numNodes); 		// set up nodes to be use with outpose's

		// Resets pose data with identity matrix
		for (a3ui32 i = 0; i < demoMode->blendTree->nodeCount; ++i) {
			demoMode->blendTree->poses[i].pose = demoMode->blendTree->poses[0].pose + demoMode->hierarchyState_skel->hierarchy->numNodes * i;
			a3hierarchyPoseReset(&demoMode->blendTree->poses[i], demoMode->blendTree->nodeCount, NULL, NULL);
			demoMode->blendTree->nodes[i].outPose = &demoMode->blendTree->poses[i];
		}

		/*
		* Init Blend Nodes
		*
		*
		*/
		for (a3ui32 i = 0; i < numOfBlendNodes; i++)
		{
			a3ui32 id = data["Clips"][i]["id"].get<a3ui32>();
			a3ui32 nodeType = data["Clips"][i]["data"]["value"].get<a3ui32>();
			a3ui32 numInputs = data["Clips"][i]["data"]["inputs"].get<a3ui32>();
			a3ui32 numParams = data["Clips"][i]["data"]["params"].get<a3ui32>();

			// a3createBlendNode(&out_pool->nodes[i], nodeType, numInputs, numParams)

			// Set up graph connections with inputs
			for (a3ui32 j = 0; j < numInputs; j++) {
				a3ui32 targetID = data["Clips"][i]["inputs"][j]["index"].get<a3ui32>();

				// out_pool->nodes[i].inputNodes[j] = out_pool->nodes[targetID]; // TargetID may not corrospond correctly to array index
			}

			// Loop over params
			for (a3ui32 j = 0; j < numParams; j++) {

				//Checks if input node else treat input like a float
				switch (nodeType) {
				case 2:
				{
					demoMode->blendTree->nodes[i].opType = Operation::NONE;
					a3byte* param = (a3byte*)data["Clips"][i]["params"][j].get<std::string>().c_str();
					a3ui32 k = a3clipGetIndexInPool(demoMode->clipPool, param);
					a3clipControllerInit(&demoMode->blendTree->clipControllers[numOfClipControllers], "xbot_ctrl", demoMode->clipPool, k, rate, fps);

					demoMode->blendTree->nodes[i].myClipController = &demoMode->blendTree->clipControllers[numOfClipControllers];
					numOfClipControllers++;
				}
				break;
				case 3:
					demoMode->blendTree->nodes[i].opType = Operation::IK_SOLVER;
					break;
				default:
				{
					demoMode->blendTree->nodes[i].opType = Operation::SPOSE;
					a3real param = (a3real)data["Clips"][i]["params"][j].get<a3real>();
					// out_pool->nodes[i].opParams[j] = param; 
				}
				break;
				}
			}
			out_blendTree->clipCount = numOfClipControllers;
		}
	}

	/*printf("Clip pool init from file");
	printf(fileName);

	std::string filepath = std::string(A3_DEMO_ANIM) + std::string(fileName);
	printf(filepath.c_str());


	std::ifstream f(filepath);
	if (f.is_open()) {
		json data = json::parse(f);
		std::string s = data.dump();
		printf(std::to_string(data["Clips"].size()).c_str());
		//printf(s.c_str());

		a3clipPoolCreate(out_pool, (a3ui32)data["Clips"].size());

		// create clip
		for (int i = 0; i < (int)data["Clips"].size(); ++i)
		{
			a3_ClipTransition forward;
			a3_ClipTransition backward;

			forward.targetClipPool = out_pool;
			forward.targetClipIndex = data["Clips"][i]["forwardTransition"]["nextClipName"].get<a3ui32>();
			forward.playbackSpeed = data["Clips"][i]["forwardTransition"]["direction"].get<a3real>();
			backward.targetClipPool = out_pool;
			backward.targetClipIndex = data["Clips"][i]["backwardTransition"]["nextClipName"].get<a3ui32>();
			backward.playbackSpeed = data["Clips"][i]["backwardTransition"]["direction"].get<a3real>();
			out_pool->clip[i].index = i;
			printf((data["Clips"][i]["name"].get<std::string>()).c_str());
			printf(std::to_string(data["Clips"][i]["firstKeyframeIndex"].get<a3ui32>()).c_str());
			printf(std::to_string(data["Clips"][i]["lastKeyframeIndex"].get<a3ui32>()).c_str());

			a3clipInit(&out_pool->clip[i],
				(a3byte*)data["Clips"][i]["name"].get<std::string>().c_str(),
				(keypool),
				data["Clips"][i]["firstKeyframeIndex"].get<a3ui32>(),
				data["Clips"][i]["lastKeyframeIndex"].get<a3ui32>(),
				&forward,
				&backward);

		}

	}*/
}
