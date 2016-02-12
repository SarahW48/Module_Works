
/*As the game scene is very huge, rendering the whole scene takes a long time.
So I create a BSP tree to render part of a scene each time, which could be more efficient in real-time game. */

#include "StdAfx.h"

#include "glitch/core/vector3d.h"
#include "BinarySearchTree.h"
#include <glitch/collada/CColladaDatabase.h>
#include <glitch/scene/CSceneManager.h>
#include "Application.h"
#include <glitch/core/math.h>
#include <glitch/glitch.h>
#include "CameraSceneNode.h"

BinarySeachTree::BinarySeachTree(glitch::scene::CSceneManager* _smgr, TiXmlNode* xmlNode)
	: IModuleManager(_smgr)
{
	THeight = 7;
	step = 10000.0f;
	position = core::vector3df(0.0f, 0.0f, 20000.0f);
	renderflag = false;
}

BinarySeachTree::~BinarySeachTree()
{
}

void
BinarySeachTree::Init()
{
	smgr->getVideoDriver()->setClearColor(glitch::video::SColor(255, 0, 0, 0));

	environment = collada::CColladaDatabase::constructScene(smgr->getVideoDriver(), "data/Streaming/miami.bdae");
	smgr->getRootSceneNode()->addChild(environment);

	environment->getSceneNodesFromType(ESNT_COLLADA_MESH, outNodes);

	glitch::f32 MinEdgeX = environment->getBoundingBox().MinEdge.getX();
    glitch::f32 MaxEdgeX = environment->getBoundingBox().MaxEdge.getX(); 
    glitch::f32 MinEdgeY = environment->getBoundingBox().MinEdge.getY();
    glitch::f32 MaxEdgeY = environment->getBoundingBox().MaxEdge.getY(); 
	glitch::f32 MinEdgeZ = environment->getBoundingBox().MinEdge.getZ();
    glitch::f32 MaxEdgeZ = environment->getBoundingBox().MaxEdge.getZ();

	rootnode = new BSPTreeNode();
	rootnode->nodes = outNodes;
	for(scene::SSceneNodeArray::iterator iter = outNodes.begin(); iter != outNodes.end(); ++iter)
			  {
				  (*iter)->setVisible(false);
			  }
	rootnode->min = vector3df(MinEdgeX, MinEdgeY, MinEdgeZ);
	rootnode->max = vector3df(MaxEdgeX, MaxEdgeY, MaxEdgeZ);
	rootnode->treeheight = 0;
    CreateBSPTree(rootnode);

	//---------appCamera--------------
	appCamera = irrnew CameraSceneNode(-1);
	appCamera->setNearValue(1.f);
	appCamera->setFarValue(5000000.f);
	appCamera->setPosition(position);
	appCamera->setTarget(core::vector3df(0.0f, 0.0f, 0.0f));

    //---------cube--------------
	video::CMaterialPtr cubeMat = smgr->getVideoDriver()->getMaterialRendererManager().createMaterialInstance(smgr->getVideoDriver(), video::EMT_UNLIT_TEXTURED_SOLID);
	glitch::scene::IMeshPtr cube = glitch::scene::createCubeMesh(1000, cubeMat);
	glitch::scene::ISceneNodePtr node = irrnew glitch::scene::CMeshSceneNode(cube);;

	appCamera->addChild(node);
	//---------------------------
	
	smgr->getRootSceneNode()->addChild(appCamera);
	smgr->setActiveCamera(appCamera);

    RenderBSPTree(rootnode, appCamera);

	renderflag = true;

	//---------mainCamera--------------
	mainCamera = irrnew CameraSceneNode(0);
	mainCamera->setNearValue(1.f);
	mainCamera->setFarValue(5000000.f);
	mainCamera->setPosition(core::vector3df(0.0f, 0.0f, 110000.0f));
	mainCamera->setTarget(core::vector3df(0.0f, 0.0f, 0.0f));
	smgr->getRootSceneNode()->addChild(mainCamera);
	smgr->setActiveCamera(mainCamera);


}

void
BinarySeachTree::UnInit()
{
	environment->remove();
	appCamera.reset();
	appCamera = NULL;
    smgr->getVideoDriver()->removeUnused();
}

void
BinarySeachTree::animatorJumpTo(f32 timeMs)
{
}

void
BinarySeachTree::animatorContinue()
{
}

void
BinarySeachTree::Update(f32 deltaTimeMs)
{
	if(renderflag)
	{
		for(scene::SSceneNodeArray::iterator iter = outNodes.begin(); iter != outNodes.end(); ++iter)
			  {
				  (*iter)->setVisible(false);
			  }
		RenderBSPTree(rootnode, appCamera);
	}
	renderflag = false;

}

void 
BinarySeachTree::PostDraw(E_MODULE_BUFFER_OUTPUT outBuffer)
{
	smgr->getVideoDriver()->draw3DLine(core::vector3df(0.0f, -10000.f, 0.0f), core::vector3df(0.0f, 10000.f, 0.0f), SColor(255, 255, 0, 0));
}

bool
BinarySeachTree::OnEvent(const glf::CoreEvent &evt)
{
	switch (evt.mType)
	{
		case glf::IET_KEY_PRESSED:	
		{		
			const glf::KeyboardEvent &kbEvt = static_cast<const glf::KeyboardEvent &>(evt);
			switch (kbEvt.mKey)
			{			
			case glf::Keyboard::KEY_A:
				{
					std::cout<<"A"<<std::endl;
					position = position + core::vector3df(-step, 0.0f, 0.0f);
					appCamera->setPosition(position);	
					renderflag = true;
					//RenderBSPTree(rootnode, appCamera);
				}
				break;
		    case glf::Keyboard::KEY_D:
				{
					std::cout<<"D"<<std::endl;
					position = position + core::vector3df(step, 0.0f, 0.0f);
					appCamera->setPosition(position);	

					renderflag = true;
					//RenderBSPTree(rootnode, appCamera);
				}
				break;
			 case glf::Keyboard::KEY_W:
				{
					std::cout<<"W"<<std::endl;
					position = position + core::vector3df(0.0f, step, 0.0f);
					appCamera->setPosition(position);	

					renderflag = true;
					//RenderBSPTree(rootnode, appCamera);
				}
				break;
			 case glf::Keyboard::KEY_S:
				{
					std::cout<<"S"<<std::endl;
					position = position + core::vector3df(0.0f, -step, 0.0f);
					appCamera->setPosition(position);		

					renderflag = true;
					//RenderBSPTree(rootnode, appCamera);
				}
				break;
			  default:
			    ;
			}
	    }
		break;
	default:
		;
	}
	return false;
}

void
BinarySeachTree::CreateBSPTree(BSPTreeNode* root)
{


  if((root->treeheight <= THeight)&&(root->nodes.size()!=0)&&(root->nodes.size()!=1))
  {

	    root->lchild = new BSPTreeNode();
		root->lchild->treeheight = root->treeheight + 1;
		root->rchild = new BSPTreeNode();
		root->rchild->treeheight = root->treeheight + 1;

		if(root->treeheight%2 == 0) scale = 0;
		else if (root->treeheight%2 == 1) scale = 1;

        if (scale == 0) // plane is parallel to X axis
		{
			   root->lchild->min.set(root->min);
			   root->lchild->max.set((root->min[0] + root->max[0])/2, root->max[1], root->max[2]);
			   root->rchild->min.set((root->min[0] + root->max[0])/2, root->min[1], root->min[2]);
			   root->rchild->max.set(root->max);

			   for( scene::SSceneNodeArray::iterator iter = root->nodes.begin(); iter != root->nodes.end(); ++iter)
			 {		
				   /* std::cout<<"object_minX:"<<((*iter)->getBoundingBox().MinEdge.getX())<<std::endl;
					std::cout<<"object_maxX:"<<((*iter)->getBoundingBox().MaxEdge.getX())<<std::endl;
					std::cout<<"object_centerX:"<<((*iter)->getBoundingBox().getCenter().getX())<<std::endl;
					std::cout<<"object_absoluteMaxX:"<<((*iter)->getPosition().getX() + (*iter)->getBoundingBox().MaxEdge.getX())<<std::endl;
					std::cout<<"object_absoluteMinX:"<<((*iter)->getPosition().getX() + (*iter)->getBoundingBox().MinEdge.getX())<<std::endl;
					std::cout<<"middleX:"<<((root->min[0] + root->max[0])/2)<<std::endl;*/

					f32 absoluteMaxX = (*iter)->getAbsolutePosition().getX() + (*iter)->getBoundingBox().MaxEdge.getX();
					f32 absoluteMinX = (*iter)->getAbsolutePosition().getX() + (*iter)->getBoundingBox().MinEdge.getX();
					if( absoluteMaxX < (root->min[0] + root->max[0])/2 )
						//lefttree
					{
						/* std::cout<<"Xleft"<<std::endl;*/
						 root->lchild->nodes.push_back(*iter);
					}
					else if( absoluteMinX > (root->min[0] + root->max[0])/2)
						//righttree
					{
						 /*std::cout<<"Xright"<<std::endl;*/
						 root->rchild->nodes.push_back(*iter);
					}
					else 
						//both
					{ 
					   /*std::cout<<"Xboth"<<std::endl;*/
					   root->lchild->nodes.push_back(*iter);
					   root->rchild->nodes.push_back(*iter);
					}
			  }
             CreateBSPTree(root->lchild);
			 CreateBSPTree(root->rchild);
		}

		else if (scale == 1) // plane is parallel to Y axis
		{
			 root->lchild->min.set(root->min);
			 root->lchild->max.set(root->max[0], (root->min[1] + root->max[1])/2, root->max[2]);
			 root->rchild->min.set(root->min[0], (root->min[1] + root->max[1])/2, root->min[2]);
			 root->rchild->max.set(root->max);

			 for( scene::SSceneNodeArray::iterator iter = root->nodes.begin(); iter != root->nodes.end(); ++iter)
			 {		
				   /* std::cout<<"object_minY:"<<((*iter)->getBoundingBox().MinEdge.getY())<<std::endl;
					std::cout<<"object_maxY:"<<((*iter)->getBoundingBox().MaxEdge.getY())<<std::endl;
					std::cout<<"object_absoluteMaxY:"<<((*iter)->getPosition().getY() + (*iter)->getBoundingBox().MaxEdge.getY())<<std::endl;
					std::cout<<"object_absoluteMinY:"<<((*iter)->getPosition().getY() + (*iter)->getBoundingBox().MinEdge.getY())<<std::endl;
					std::cout<<"middleY:"<<((root->min[1] + root->max[1])/2)<<std::endl;*/

					f32 absoluteMaxY = (*iter)->getAbsolutePosition().getY() + (*iter)->getBoundingBox().MaxEdge.getY();
					f32 absoluteMinY = (*iter)->getAbsolutePosition().getY() + (*iter)->getBoundingBox().MinEdge.getY();
					if( absoluteMaxY < (root->min[1] + root->max[1])/2 )
						//lefttree
					{
						 /*std::cout<<"Yleft"<<std::endl;*/
						 root->lchild->nodes.push_back(*iter);	
					}
					else if( absoluteMinY > (root->min[1] + root->max[1])/2)
						//righttree
					{
						 /*std::cout<<"Yright"<<std::endl;*/
						 root->rchild->nodes.push_back(*iter);			
					}
					else 
						//both
					{ 
					   /*std::cout<<"Yboth"<<std::endl;*/
					   root->lchild->nodes.push_back(*iter);			   
					   root->rchild->nodes.push_back(*iter);
					   
					}
			  }
			  CreateBSPTree(root->lchild);
			  CreateBSPTree(root->rchild);
		}
   }
   return;
}


bool
BinarySeachTree::JudgeIntersection(scene::ICameraSceneNodePtr camera, BSPTreeNode* treenode)
{
	if(treenode != NULL)
	{
	std::cout<<"--------------------------------------------------"<<std::endl;
    std::cout<<"size:"<<(treenode->nodes.size())<<std::endl;
	aabbox3d<f32> Box = aabbox3d<f32>(treenode->min, treenode->max);
	
	int cullingresult = camera->getViewFrustum()->intersectsEx(Box);
	std::cout<<"cullingresult:"<<(camera->getViewFrustum()->intersectsEx(Box))<<std::endl;
	if(cullingresult == 0) return false;
	else  return true;
	}	
	return false;
}

void
BinarySeachTree::RenderBSPTree(BSPTreeNode* root, scene::ICameraSceneNodePtr camera)
{
 if(JudgeIntersection(camera, root)&&(root != NULL))
  {
	  if(root->treeheight == THeight+1) //9
		  {
			  std::cout<<"-----------------------render---------------------------"<<std::endl;
			  std::cout<<"treeheight:"<<root->treeheight<<std::endl;
			  std::cout<<"size:"<<(root->nodes.size())<<std::endl;
			  for(scene::SSceneNodeArray::iterator iter = root->nodes.begin(); iter != root->nodes.end(); ++iter)
			  {
				  (*iter)->setVisible(true);
			  }
			  std::cout<<"end"<<std::endl;
	      }
		  std::cout<<"--------------------------------------------------"<<std::endl;
		  std::cout<<"treeheight:"<<root->treeheight<<std::endl;
		  std::cout<<"Left"<<std::endl;
		  RenderBSPTree(root->lchild, camera);

		  std::cout<<"--------------------------------------------------"<<std::endl;
		  std::cout<<"treeheight:"<<root->treeheight<<std::endl;
		  std::cout<<"Right"<<std::endl;
		  RenderBSPTree(root->rchild, camera);
  }

 
 }

void 
BinarySeachTree::DeleteBSPTree()
{
}


