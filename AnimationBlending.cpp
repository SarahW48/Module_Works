
/* This is basicly playing animtion from bdae file and make blending between two animation clips */

#include "StdAfx.h"

#include "AnimationBlending.h"
#include "Application.h"
#include <iostream>

f32 BlendingTime = 1000.0f;
f32 BlendingCurrentTime = 0.0f;
u32 StartBlendingTime = 0;

AnimationBlending::AnimationBlending(CSceneManager* _smgr, TiXmlNode* xmlNode)
	: IModuleManager(_smgr)
	, Device(_smgr->getVideoDriver()->getDevice())
	, Driver(_smgr->getVideoDriver())
{
}

AnimationBlending::~AnimationBlending()
{
}


void
AnimationBlending::Init()
{
	smgr->getVideoDriver()->setClearColor(glitch::video::SColor(255, 255, 255, 0));

    scene::CCameraSceneNodePtr CameraTop = irrnew glitch::scene::CCameraSceneNode(-1,
                                               glitch::core::vector3df(0.f, 0.f, 300.f),
                                               glitch::core::vector3df(0.f, 0.f, 0.f)); 
    smgr->getRootSceneNode()->addChild(CameraTop);
    CameraTop->setUpVector(glitch::core::vector3df(0.f, 1.f, 0.f));
    smgr->setActiveCamera(CameraTop); 

	node = collada::CColladaDatabase::constructScene(smgr->getVideoDriver(), "data/AnimationBlending/MC_ver2_HIGH.bdae");
	node->setRotation(vector3df(-1.0f, 0.0f, 0.0f));
	smgr->getRootSceneNode()->addChild(node);

	AnimationSet.reset  (new collada::CAnimationSet);
	AnimationSet->addAnimationLibrary ("data/AnimationBlending/mc_ver2_anim.bdae");	
	AnimationSet->compile();

	Animator1.reset(new collada::CSceneNodeAnimatorSet(AnimationSet));
	Animator2.reset(new collada::CSceneNodeAnimatorSet(AnimationSet));

	int clipIndex1 = Animator1->getAnimationClipIndex( "idle_bat_active" );
	int clipIndex2 = Animator2->getAnimationClipIndex( "idle_chainsaw" );

	Animator1->getTimelineCtrl()->setClip(clipIndex1);
	Animator2->getTimelineCtrl()->setClip(clipIndex2);
	Animator1->getTimelineCtrl()->setLoop(true);
	Animator2->getTimelineCtrl()->setLoop(true);

	//create the animation with blending
	AnimatorBlender = new collada::CSceneNodeAnimatorBlender;
	
	AnimatorBlender->addAnimator(Animator1);
	AnimatorBlender->addAnimator(Animator2);

	AnimatorBlender->compile();
	node->addAnimator (AnimatorBlender);

	AnimatorBlender->setWeight(0, 1.f);
	AnimatorBlender->setWeight(1, 0.f);
	currentAnimation = 0;
	nextAnimation = 1;

}

void
AnimationBlending::UnInit()
{
	if(node)
	{
		node->remove();
	}
	node = NULL;
	Animator1 = Animator2 = NULL;
    smgr->getVideoDriver()->removeUnused();
	
}

void 
AnimationBlending::Cleanup()
{
}

void
AnimationBlending::animatorJumpTo(f32 timeMs)
{
	UnInit();
	Init();
	Update(timeMs);
}

void
AnimationBlending::animatorContinue()
{
}

void
AnimationBlending::Update(f32 deltaTimeMs)
{
	if(AnimatorBlender)
	{
		BlendingCurrentTime = fmod(BlendingCurrentTime + deltaTimeMs, BlendingTime) ;

		AnimatorBlender->setWeight(currentAnimation, 1.0f - BlendingCurrentTime/(f32)BlendingTime);
		AnimatorBlender->setWeight(nextAnimation, BlendingCurrentTime/(f32)BlendingTime);
	}
	node->onAnimate(deltaTimeMs);
    
}
