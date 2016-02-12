#pragma once
#ifndef __TRIANGLETEST__
#define __TRIANGLETEST__

#include "IModuleManager.h"

#include <glf/glf.h>
#include <glitch/types.h>
#include <glitch/glitch.h>

class TriangleMaterial
	: public IModuleManager
{
public:

	TriangleMaterial(glitch::scene::CSceneManager* smgr, TiXmlNode* xmlNode);
	~TriangleMaterial();
	void Init();
	void UnInit();
	void animatorJumpTo(f32 timeMs);
	void animatorContinue();
	void Update(f32 deltaTimeMs);
	void PostDraw(E_MODULE_BUFFER_OUTPUT outBuffer);

private:

	glitch::IDevicePtr              device;
    glitch::video::IVideoDriver*    driver;

	glitch::scene::ISceneNodePtr    node;

	glitch::scene::ISceneNode* scene;
	glitch::scene::ICameraSceneNodePtr appCamera;
	std::string bdaeSourceName;

};

#endif
