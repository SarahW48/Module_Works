
#pragma once
#ifndef __GLITCH_SCENE_GEOMETRYCREATOR_H__
#define __GLITCH_SCENE_GEOMETRYCREATOR_H__

#include <glitch/CompileConfig.h>
#if defined(GLITCH_SCENE_ENABLE_GEOMERTY_CREATOR_MODULE)

#include <glitch/scene/IMesh.h>
#include <glitch/video/CImage.h>
#include <glitch/video/CMaterial.h>
#include <glitch/video/CMaterialVertexAttributeMap_fwd.h>

namespace glitch
{
namespace video
{

class IVideoDriver;

}

namespace scene
{

IMeshPtr createTriangleMesh(
	u32 vertexAttributes,
	video::IVideoDriver* driver,
	f32 size,
	const video::SColor& color = video::SColor(255,255,255,255),
    video::E_BUFFER_USAGE usage = video::EBU_NONE
);

inline
IMeshPtr
createTriangleMesh(
	f32 size,
	const video::SColor& color,
	const video::CMaterialPtr& material,
	const video::CMaterialVertexAttributeMapPtr& materialVertexAttributeMap = NULL,
    video::E_BUFFER_USAGE usage = video::EBU_NONE
)

{
	GLITCH_ASSERT(material);
	IMeshPtr mesh = createTriangleMesh(
		material->getVertexAttributeMask(),
		material->getMaterialRenderer()->getVideoDriver(),
		size,
		color,
        usage
	);
	mesh->setMaterial(0, material, materialVertexAttributeMap);
	return mesh;
}

IMeshPtr createCubeMesh(
	u32 vertexAttributes,
	video::IVideoDriver* driver,
	f32 size,
	const video::SColor& color = video::SColor(255,255,255,255),
    video::E_BUFFER_USAGE usage = video::EBU_NONE
);

inline
IMeshPtr
createCubeMesh(
	f32 size,
	const video::CMaterialPtr& material,
	const video::CMaterialVertexAttributeMapPtr& materialVertexAttributeMap = NULL,

	const video::SColor& color = video::SColor(0xff,0xff,0xff,0xff),
    video::E_BUFFER_USAGE usage = video::EBU_NONE
)
{
	GLITCH_ASSERT(material);
	IMeshPtr mesh = createCubeMesh(
		material->getVertexAttributeMask(),
		material->getMaterialRenderer()->getVideoDriver(),
		size,
		color,
        usage
	);
	mesh->setMaterial(0, material, materialVertexAttributeMap);
	return mesh;
}


} // end namespace scene
} // end namespace glitch

#endif // defined(GLITCH_SCENE_ENABLE_GEOMERTY_CREATOR_MODULE)
#endif //__GLITCH_SCENE_GEOMETRYCREATOR_H__
