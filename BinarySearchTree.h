#pragma once
#ifndef __BSPTREEMIAMI__
#define __BSPTREEMIAMI__

#include "IModuleManager.h"

#include <glf/glf.h>
#include <glitch/types.h>
#include <glitch/glitch.h>

class BinarySeachTree
	: public IModuleManager
{
public:

	BinarySeachTree(glitch::scene::CSceneManager* smgr, TiXmlNode* xmlNode);
	~BinarySeachTree();
	void Init();
	void UnInit();
	void animatorJumpTo(f32 timeMs);
	void animatorContinue();
	void Update(f32 deltaTimeMs);	
	void PostDraw(E_MODULE_BUFFER_OUTPUT outBuffer);
	bool OnEvent(const glf::CoreEvent &evt);

private:

	 struct BSPTreeNode
	{
		 BSPTreeNode(){lchild = NULL; rchild = NULL;}
		 //BSPTreeNode(){containorigin = false; lchild = NULL; rchild = NULL;}
		 //std::vector<ISceneNodePtr> nodes;
		 std::vector<ISceneNodePtr, core::SAllocator<ISceneNodePtr, memory::EMH_NONE> > nodes;
		 core::vector3df min;
		 core::vector3df max;
		 int treeheight;
		 //bool containorigin;
		 BSPTreeNode* lchild;
		 BSPTreeNode* rchild;
	};

	void CreateBSPTree(BSPTreeNode* root);

	void DeleteBSPTree();
	/*void DeleteBSPTree(BSPTreeNode* root);*/

	void RenderBSPTree(BSPTreeNode* root, scene::ICameraSceneNodePtr camera);


	bool JudgeIntersection(scene::ICameraSceneNodePtr camera, BSPTreeNode* treenode);

	int scale;
	int THeight;
	float step;

	bool renderflag;

	core::vector3df position;

	BSPTreeNode*          rootnode;
	
	glitch::IDevicePtr              device;
    glitch::video::IVideoDriver*    driver;

	glitch::scene::ISceneNodePtr    environment;
	scene::SSceneNodeArray outNodes;

	glitch::scene::ICameraSceneNodePtr mainCamera;
	glitch::scene::ICameraSceneNodePtr appCamera;

	glitch::scene::ISceneNodePtr newnode;


};

#endif