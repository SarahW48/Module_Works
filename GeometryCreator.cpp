#include <glitch/StdAfx.h>

#include <glitch/scene/GeometryCreator.h>
#if defined(GLITCH_SCENE_ENABLE_GEOMERTY_CREATOR_MODULE)

#include <glitch/scene/CMeshBuffer.h>
#include <glitch/scene/CMesh.h>
#include <glitch/scene/GeometryCreator_impl.h>
#include <glitch/video/IVideoDriver.h>
#include <glitch/video/ITexture_fwd.h>
#include <glitch/video/CImage.h>
#include <glitch/os/os.h>
#include <glitch/core/plane3d.h>

#include <math.h>

namespace glitch
{
namespace scene
{

namespace
{

IMeshPtr 
createTriangleMesh(
	u32 vertexAttributes,
	video::IVideoDriver* driver,
	f32 size,
	const video::SColor& color,
    video::E_BUFFER_USAGE usage
)
{
	static const u16 u[3] = { 0,2,1,};

	CMeshBufferPtr buffer = createMeshBuffer(vertexAttributes,
											 driver,
											 3,
											 3,
                                             usage);
	{
		video::CPrimitiveStream::SMapBuffer<u16> indices(buffer->getPrimitiveStream(),
														 video::EBMA_WRITE);
		memcpy(indices.get(), &u[0], 3 * sizeof(u16));
	}

	static const struct
	{
		core::vector3df Pos;
		core::vector3df Normal;
		core::vector2df TCoords;
	} triData[] =
		  {
		
			  { core::vector3df(-.5f, .0f,-.5f), core::vector3df( 0, 1, 0), core::vector2df(1, 1) },
			  { core::vector3df( .5f, .0f,-.5f), core::vector3df( 0, 1, 0), core::vector2df(1, 0) },
			  { core::vector3df( .5f, .0f, .5f), core::vector3df( 0, 1, 0), core::vector2df(0, 0) } 

		  };

	{
		video::SVertexStream::SMapBuffer<core::vector3df> position;
		video::SVertexStream::SMapBuffer<core::vector2df> texCoord;
		video::SVertexStream::SMapBuffer<core::vector3df> normal;
		video::SVertexStream::SMapBuffer<video::SColor> color0;
		mapMeshBuffer(buffer, &position, &texCoord, &normal, &color0);

		for(u32 i = 0; i < 3; ++i)
		{
			position[i] = triData[i].Pos * size;
			if(texCoord)
			{
				texCoord[i] = triData[i].TCoords;
			}
			if(normal)
			{
				normal[i] = triData[i].Normal;
			}
			if(color0)
			{
				color0[i] = color;
			}
		}
	}
	
	CMeshPtr mesh = irrnew CMesh;
	mesh->addMeshBuffer(buffer, NULL, NULL);

	mesh->recalculateBoundingBox();

	return mesh;
}

IMeshPtr
createCubeMesh(
	u32 vertexAttributes,
	video::IVideoDriver* driver,
	f32 size,
	const video::SColor& color,
    video::E_BUFFER_USAGE usage
)
{
	static const u16 u[36] = { 0,1,2,    0,2,3,
							   4,5,6,    4,6,7,   
							   8,9,10,   8,10,11,
							   12,13,14, 12,14,15,
							   16,17,18, 16,18,19,
							   20,21,22, 20,22,23};

	CMeshBufferPtr buffer = createMeshBuffer(vertexAttributes,
											 driver,
											 24,
											 36,
                                             usage);
	{
		video::CPrimitiveStream::SMapBuffer<u16> indices(buffer->getPrimitiveStream(),
														 video::EBMA_WRITE);
		memcpy(indices.get(), &u[0], 36 * sizeof(u16));
	}

	video::SColor clr(color);

	static const struct
	{
		core::vector3df Pos;
		core::vector3df Normal;
		core::vector2df TCoords;
	} cubeData[] =
		  {
			  // Back Face
			  { core::vector3df(-.5f,-.5f,-.5f), core::vector3df( 0, 0,-1), core::vector2df(1, 1) },
			  { core::vector3df(-.5f, .5f,-.5f), core::vector3df( 0, 0,-1), core::vector2df(1, 0) },
			  { core::vector3df( .5f, .5f,-.5f), core::vector3df( 0, 0,-1), core::vector2df(0, 0) },
			  { core::vector3df( .5f,-.5f,-.5f), core::vector3df( 0, 0,-1), core::vector2df(0, 1) },

			  // Right Face
			  { core::vector3df( .5f,-.5f,-.5f), core::vector3df( 1, 0, 0), core::vector2df(1, 1) },
			  { core::vector3df( .5f, .5f,-.5f), core::vector3df( 1, 0, 0), core::vector2df(1, 0) },
			  { core::vector3df( .5f, .5f, .5f), core::vector3df( 1, 0, 0), core::vector2df(0, 0) },
			  { core::vector3df( .5f,-.5f, .5f), core::vector3df( 1, 0, 0), core::vector2df(0, 1) },

			  // Back Face
			  { core::vector3df( .5f,-.5f, .5f), core::vector3df( 0, 0, 1), core::vector2df(1, 1) },
			  { core::vector3df( .5f, .5f, .5f), core::vector3df( 0, 0, 1), core::vector2df(1, 0) },
			  { core::vector3df(-.5f, .5f, .5f), core::vector3df( 0, 0, 1), core::vector2df(0, 0) },
			  { core::vector3df(-.5f,-.5f, .5f), core::vector3df( 0, 0, 1), core::vector2df(0, 1) },
			  
			  // Left Face
			  { core::vector3df(-.5f,-.5f, .5f), core::vector3df(-1, 0, 0), core::vector2df(1, 1) },
			  { core::vector3df(-.5f, .5f, .5f), core::vector3df(-1, 0, 0), core::vector2df(1, 0) },
			  { core::vector3df(-.5f, .5f,-.5f), core::vector3df(-1, 0, 0), core::vector2df(0, 0) },
			  { core::vector3df(-.5f,-.5f,-.5f), core::vector3df(-1, 0, 0), core::vector2df(0, 1) },

			  // Top Face
			  { core::vector3df( .5f, .5f, .5f), core::vector3df( 0, 1, 0), core::vector2df(1, 1) },
			  { core::vector3df( .5f, .5f,-.5f), core::vector3df( 0, 1, 0), core::vector2df(1, 0) },
			  { core::vector3df(-.5f, .5f,-.5f), core::vector3df( 0, 1, 0), core::vector2df(0, 0) },
			  { core::vector3df(-.5f, .5f, .5f), core::vector3df( 0, 1, 0), core::vector2df(0, 1) },

			  // Bottom Face
			  { core::vector3df(-.5f,-.5f,-.5f), core::vector3df( 0,-1, 0), core::vector2df(1, 1) },
			  { core::vector3df( .5f,-.5f,-.5f), core::vector3df( 0,-1, 0), core::vector2df(1, 0) },
			  { core::vector3df( .5f,-.5f, .5f), core::vector3df( 0,-1, 0), core::vector2df(0, 0) },
			  { core::vector3df(-.5f,-.5f, .5f), core::vector3df( 0,-1, 0), core::vector2df(0, 1) }
		  };

	{
		video::SVertexStream::SMapBuffer<core::vector3df> position;
		video::SVertexStream::SMapBuffer<core::vector2df> texCoord;
		video::SVertexStream::SMapBuffer<core::vector3df> normal;
		video::SVertexStream::SMapBuffer<video::SColor> color0;
		mapMeshBuffer(buffer, &position, &texCoord, &normal, &color0);

		for(u32 i = 0; i < 24; ++i)
		{
			position[i] = cubeData[i].Pos * size;
			if(texCoord)
			{
				texCoord[i] = cubeData[i].TCoords;
			}
			if(normal)
			{
				normal[i] = cubeData[i].Normal;
			}
			if(color0)
			{
				color0[i] = clr;
			}
		}
	}
	
	CMeshPtr mesh = irrnew CMesh;
	mesh->addMeshBuffer(buffer, NULL, NULL);

	mesh->recalculateBoundingBox();

	return mesh;
}


} // end namespace scene
} // end namespace glitch

#endif // defined(GLITCH_SCENE_ENABLE_GEOMERTY_CREATOR_MODULE)
