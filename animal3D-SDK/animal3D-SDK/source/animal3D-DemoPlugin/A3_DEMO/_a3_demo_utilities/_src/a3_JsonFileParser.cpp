#include "A3_DEMO/_a3_demo_utilities/a3_JsonFileParser.h"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <animal3D/a3/a3types_integer.h>
#include <A3_DEMO/_animation/a3_BlendTree.h>
#include <A3_DEMO/a3_DemoMode1_Animation.h>

using json = nlohmann::json;

#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_ANIM	A3_DEMO_RES_DIR""

a3ui32 const rate = 24;
a3f64 const fps = (a3f64)rate;

void a3ReadBlendTreeFromFile(a3_BlendTree* out_blendTree, const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_DemoMode1_Animation* demoMode)
{

	static a3ui32 numOfClipControllers = 0;
	std::string filepath = std::string(A3_DEMO_ANIM) + std::string(fileName);
	std::ifstream f(filepath);
	if (f.is_open()) 
	{
		// Parse JSON
		json data = json::parse(f);

		// Get blend tree init params
		const a3ui32 numOfBlendNodes = (a3ui32)data["nodes"].size();

		/*
		* Create Blend Tree
		*
		*/
		a3initBlendTree(out_blendTree, numOfBlendNodes, demoMode->hierarchy_skel);

		/*
		* Init Blend Nodes
		*
		*/
		for (a3ui32 i = 0; i < numOfBlendNodes; i++)
		{
			// Read in node Data
			a3ui32	id			= (a3ui32)stoi(data["nodes"][i]["id"].get<std::string>()), // Get Node ID
					nodeType	,
					numInputs	,
					numParams	;

			// Set node data
			if (id == 0) 
			{
				// is root node
				nodeType = -1;
				numInputs = 1;
				numParams = 0;
			}
			else 
			{
				// Is not the root node
				nodeType = data["nodes"][i]["data"]["value"].get<a3ui32>(); // Get Node Type
				numInputs = data["nodes"][i]["data"]["inputs"].get<a3ui32>(); // Get Number of node Inputs
				numParams = data["nodes"][i]["data"]["params"].get<a3ui32>(); // Get Number of node Params
			}

			// Init Common node data
			out_blendTree->nodes[id].numInputs = numInputs; // Num of inputs

			// Set up pointers to inputs nodes
			for (a3ui32 j = 0; j < numInputs; j++) 
			{
				a3ui32 targetID = (a3ui32)stoi(data["nodes"][i]["inputs"][j]["index"].get<std::string>()); // Get Target Index
				out_blendTree->nodes[id].inputNodes[j] = &out_blendTree->nodes[targetID]; // Set pointer to targeted inputs
			}

			// Handle node setup based on Node Type
			switch (nodeType) 
			{
				case -1:	// UNKOWN Node
					demoMode->blendTree->nodes[id].opType = Operation::NONE;
					break;
				case 0:		// CONCAT Node
				{
					demoMode->blendTree->nodes[id].opType = Operation::HPOSE; // OpType
					demoMode->blendTree->nodes[id].poseOp = (a3_BlendFunc)(&a3hierarchyPoseMerge); // Op Fucntion Pointer
					break;
				}
				case 1:		// LERP Node
				{
					demoMode->blendTree->nodes[id].opType = Operation::HPOSE; // OpType
					demoMode->blendTree->nodes[id].poseOp = (a3_BlendFunc)(&a3hierarchyPoseOpLERP); // Op Function Pointer

					// Init Op Parameters
					for (a3ui32 j = 0; j < numParams; j++) 
					{
						a3real param = (a3real)stof(data["nodes"][i]["params"][j].get<std::string>()); // Get Param
						out_blendTree->nodes[id].opParams[j] = param; // Set Op param
					}
					break;
				}
				case 2:		// INPUT Node / sample animation, no BlendFunc required
				{
					demoMode->blendTree->nodes[id].opType = Operation::NONE; // No OpType for Input nodes

					// Get Input Parameter(Animation Name)
					std::string str = data["nodes"][i]["params"][0].get<std::string>(); 
					a3byte* param = (a3byte*)str.c_str();
					
					// Init Clip
					a3ui32 k = a3clipGetIndexInPool(demoMode->clipPool, param);
					a3clipControllerInit(&demoMode->blendTree->clipControllers[numOfClipControllers], "xbot_ctrl", demoMode->clipPool, k, rate, fps); // Init controller

					// Set Pointer to ClipContoller on Node
					demoMode->blendTree->nodes[id].myClipController = &demoMode->blendTree->clipControllers[numOfClipControllers]; 

					numOfClipControllers++;
					break;
				}
				case 3:		// IK Node
					demoMode->blendTree->nodes[id].opType = Operation::IK_SOLVER;
					break;
				default:
					break;
			}

			a3ui32 maskRange[2];
			a3ui32 numOfMasks = data["nodes"][i]["maskNodes"].size();
			out_blendTree->nodes[id].numMaskBones = 0;

			if (numOfMasks > 1) 
			{
				for (a3ui32 j = 0; j < 2; j++)
				{
					maskRange[j] = (a3ui32)stoi(data["nodes"][i]["maskNodes"][j].get<std::string>());
				}

		
				for (a3ui32 j = 0; j < 128; j++)
				{
					if (j >= maskRange[0] && j < maskRange[1]) {
						//mask nodes
						out_blendTree->nodes[id].baskBoneIndices[j] = j;
					}
				}

				out_blendTree->nodes[id].numMaskBones = maskRange[1] - maskRange[0];

			}
			if (numOfMasks > 0) 
			{
				if ((a3ui32)stoi(data["nodes"][i]["maskNodes"][0].get<std::string>()) == 0)
				{
					out_blendTree->nodes[id].baskBoneIndices[0] = 0;
					out_blendTree->nodes[id].numMaskBones = 1;

				}
			}
			

		}

		// Relove clip count
		out_blendTree->clipCount = numOfClipControllers;

		

		// Mask Node Setup
	}
}
