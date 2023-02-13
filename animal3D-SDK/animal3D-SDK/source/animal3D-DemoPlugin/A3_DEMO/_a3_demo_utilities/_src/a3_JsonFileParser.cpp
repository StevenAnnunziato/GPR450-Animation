#include "A3_DEMO/_a3_demo_utilities/a3_JsonFileParser.h"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_ANIM	A3_DEMO_RES_DIR"animdata/"

void a3_InitClipPoolFromFile(const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_ClipPool* out_pool, a3ui32* out_size, const a3_KeyframePool* keypool)
{
	printf("Clip pool init from file");
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

	}
}
