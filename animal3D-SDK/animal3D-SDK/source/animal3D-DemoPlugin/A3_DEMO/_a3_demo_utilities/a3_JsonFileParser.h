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

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus


#endif	// __cplusplus
enum
{
	a3BlendName_nameLenMax = 32,
};
//-----------------------------------------------------------------------------

//void a3ReadBlendTreeFromFile(const a3byte fileName[a3keyframeAnimation_nameLenMax], a3_ClipPool* out_pool, a3ui32* out_size, const a3_KeyframePool* keypool);
void a3ReadBlendTreeFromFile();


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOSCENEOBJECT_H