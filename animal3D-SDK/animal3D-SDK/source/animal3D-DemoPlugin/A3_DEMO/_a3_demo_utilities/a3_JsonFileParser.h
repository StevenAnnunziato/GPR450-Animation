/*
	This Could Take All Week
	By Michael Bowen, Steven Annunziato

	a3_FileParser.h
	Util for json file parsersing
*/


#ifndef __A3DEMO_FILEPARSER_H
#define __A3DEMO_FILEPARSER_H

//json library



//-----------------------------------------------------------------------------
#include "A3_DEMO/_animation/a3_KeyframeAnimation.h"
#ifdef __cplusplus
extern "C"
{


#else	// !__cplusplus


#endif	// __cplusplus

struct a3_BlendTree;
struct a3_DemoMode1_Animation;

//-----------------------------------------------------------------------------


//void a3ReadBlendTreeFromFile(const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_ClipPool* out_pool, a3ui32* out_size, const a3_KeyframePool* keypool);
void a3ReadBlendTreeFromFile(a3_BlendTree* out_blendTree, const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_DemoMode1_Animation* demoMode);


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOSCENEOBJECT_H