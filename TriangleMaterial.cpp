
/* I create a triangle, create texture and material render, then create a shader*/

#include "StdAfx.h"

#include "glitch/core/vector4d.h"
#include "TriangleMaterial.h"
#include <glitch/collada/CColladaDatabase.h>
#include <glitch/scene/CSceneManager.h>
#include "Application.h"
#include "ColladaFactoryHwSkinVBO.h"

#include <glitch/core/math.h>
#include <glitch/glitch.h>

#include <glitch/SCreationParameters.h>
#include <glitch/io/CMemoryReadFile.h>
#include <glitch/video/CCgShaderManager.h>
#include <glitch/video/CGLSLShaderManager.h>
#include <glitch/video/CFixedGLShaderManager.h>
#include <glitch/collada/CColladaResFileManager.h>
#include <glitch/video/CCgShaderManager.h>
#include <glitch/video/CMaterialRenderer.h>

TriangleMaterial::TriangleMaterial(glitch::scene::CSceneManager* _smgr, TiXmlNode* xmlNode)
	: IModuleManager(_smgr)
	, scene(NULL)
{
}

TriangleMaterial::~TriangleMaterial()
{
}

void
TriangleMaterial::Init()
{
   driver = smgr->getVideoDriver();

   smgr->getVideoDriver()->setClearColor(glitch::video::SColor(255, 255, 255, 0));

    scene::CCameraSceneNodePtr CameraTop = irrnew glitch::scene::CCameraSceneNode(-1,
                                               glitch::core::vector3df(20.f, 20.f, 200.f),
                                               glitch::core::vector3df(20.f, 10.f, 0.f)); 
    smgr->getRootSceneNode()->addChild(CameraTop);
    CameraTop->setUpVector(glitch::core::vector3df(0.f, 1.f, 0.f));
    smgr->setActiveCamera(CameraTop); 

	glitch::video::IShaderPtr shader;

	// Create Shaders
	char* vertexShader =
            "attribute highp   vec4 Vertex;\n" \
			"attribute highp   vec2 TexCoord0;\n" \
            "uniform   mediump mat4 WorldViewProjectionMatrix;\n" \
			"varying highp     vec2 vTexCoord0;\n" \
            "void main(void)\n" \
            "{\n" \
            "    gl_Position = WorldViewProjectionMatrix * Vertex;\n" \
			"    vTexCoord0 = TexCoord0;\n" \
            "}\n";
    char* fragmentShader =
            " uniform lowp vec4 diffuseColor;\n" \
			" uniform lowp sampler2D texture;\n" \
			" varying highp vec2 vTexCoord0;\n" \
            "void main(void)\n" \
            "{\n" \
            "    lowp vec4 color = texture2D(texture, vTexCoord0);\n" \
			"    gl_FragColor = color * diffuseColor ;\n" \
            "}\n";

        glitch::video::CGLSLShaderManager* shaderManager =
            static_cast< glitch::video::CGLSLShaderManager* >(driver->getShaderManager());

        glitch::io::IReadFilePtr readVertex = irrnew glitch::io::CMemoryReadFile(vertexShader, strlen(vertexShader), "nullvertex", false);
        glitch::io::IReadFilePtr readFrag = irrnew glitch::io::CMemoryReadFile(fragmentShader, strlen(fragmentShader), "nullfarg", false);
        shader = shaderManager->createShader("FlatColor",
                "nullvertex",
                "",
                "nullfarg",
                "",
                readVertex.get(),
                readFrag.get());

    // Create texture
    glitch::video::ITexturePtr tex;
	tex = driver->getTextureManager()->getTexture("data/car_texture_d.tga");
	
	glitch::video::CMaterialRendererManager& mrm = driver->getMaterialRendererManager();
    // Then we generate a material renderer with the shader
	glitch::video::CMaterialRendererPtr mr;
	glitch::video::CMaterialRendererManager::SCreationContext ctx;

	u16 rendererId = mrm.beginMaterialRenderer(ctx, driver, "MaterialRender");
    if(ctx.isValid())
		{
        ctx.beginTechnique("Tech");
        ctx.addRenderPass(shader);
        ctx.endTechnique(true);
		// Get created material rendrerer
		mr = mrm.getMaterialRenderer(mrm.endMaterialRenderer(ctx));
	    }
	else
		{
		// Get existing material rendrerer
		mr = mrm.getMaterialRenderer(rendererId);
		} 

	  //  mr = mrm.getMaterialRenderer(mrm.createMaterialRenderer(driver, video::EMT_UNLIT_TEXTURED_SOLID)); 

	    // Create a material
	    glitch::video::CMaterialPtr m;

		m = video::CMaterial::allocate( mr );

		
		glitch::u16 texID = m->getMaterialRenderer()->getParameterID(glitch::video::ESPT_TEXTURE);
        m->setParameter(texID, video::ESPVT_TEXTURE_SAMPLER_2D, &tex );
        
		glitch::u16 diffuseColor = m->getParameterID(glitch::video::ESPT_DIFFUSE_COLOR);
		m->setParameter(diffuseColor, glitch::core::vector4df(0.9f, 0.7f, 0.7f, 0.5f)); 

    glitch::scene::IMeshPtr mesh = glitch::scene::createTriangleMesh(200, video::SColor(0xff,0xff,0xff,0xff), m);

	node = irrnew glitch::scene::CMeshSceneNode(mesh);

	node->setRotation(vector3df(1.0f, 0.0f, 0.0f));
    
    smgr->getRootSceneNode()->addChild(node);


}

void
TriangleMaterial::UnInit()
{
}

void
TriangleMaterial::animatorJumpTo(f32 timeMs)
{
}

void
TriangleMaterial::animatorContinue()
{
}

void
TriangleMaterial::Update(f32 deltaTimeMs)
{
}

void 
TriangleMaterial::PostDraw(E_MODULE_BUFFER_OUTPUT outBuffer)
{
}
