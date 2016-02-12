//-*-c++-*-
#pragma once
#ifndef __ANIMATION_TEST__
#define __ANIMATION_TEST__

#include <glitch/glitch.h>
#include "IModuleManager.h"

using namespace glitch;
using namespace scene;

class AnimationBlending
	: public IModuleManager
{
public:

	AnimationBlending(CSceneManager* _smgr, TiXmlNode* xmlNode);
	~AnimationBlending();
	void Init();
	void UnInit();
	void Cleanup();
	void animatorJumpTo(f32 timeMs);
	void animatorContinue();
	void Update(f32 deltaTimeMs); 

protected:
	IDevice*				        Device;
	video::IVideoDriver*	        Driver;

private:
	ISceneNodePtr node;
	collada::CAnimationSetPtr		  AnimationSet;
	collada::CSceneNodeAnimatorBlenderPtr AnimatorBlender;
	collada::ISceneNodeAnimatorPtr Animator1;
	collada::ISceneNodeAnimatorPtr Animator2;
	int currentAnimation;
	int nextAnimation;



};

#endif