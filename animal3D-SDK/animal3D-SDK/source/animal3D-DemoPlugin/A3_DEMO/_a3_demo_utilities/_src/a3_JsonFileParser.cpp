#include "A3_DEMO/_a3_demo_utilities/a3_JsonFileParser.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_ANIM	A3_DEMO_RES_DIR"animdata/"

void a3_InitClipPoolFromFile(const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_ClipPool* out_pool, a3ui32* out_size)
{
	printf("Clip pool init from file");
	printf(fileName);

	std::string filepath = std::string(A3_DEMO_ANIM) + std::string(fileName);
	printf(filepath.c_str());


	std::ifstream f(filepath); 
	if (f.is_open()) {
		json data = json::parse(f);
		std::string s = data.dump();
		printf(s.c_str());
	}
	
}
