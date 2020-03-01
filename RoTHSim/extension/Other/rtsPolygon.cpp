#include <iostream>
#include <new>
#include <math.h>
#include "rtsPolygon.h"

rtsPolygon::~rtsPolygon()
{
	
}

//---------------------------------------------------------------------------

void rtsPolygon::createPolygon()
{
	// build the polygon vertices and triangles
	int j = 0;
	int vertices[3];
	int numTriangleNodes = m_triangleNodes.size();
	for (int i = 0; i < numTriangleNodes; i++)
	{
		// create vertex
		vertices[j] = newVertex(m_triangleNodes[i]);

		// if a triangle is ready, create the triangle
		if (j == 2)
		{
			newTriangle(vertices[0], vertices[1], vertices[2]);
			j = 0;
		}
		else
		{
			j++;
		}
	}

	// calculate center of mass
	cVector3d com1(0, 0, 0);
	for (int i = 0; i < m_polygonNodes.size(); i++)
	{
		com1.add(m_polygonNodes[i]);
	}
	com1.div(m_polygonNodes.size());
	m_centerOfMass = com1;
}

//---------------------------------------------------------------------------

cVector3d rtsPolygon::getCornerVector(int a_corner)
{
	return cornerVectors[a_corner];
}

//---------------------------------------------------------------------------

void rtsPolygon::setColor(cColorf a_color)
{
	m_material->m_ambient = a_color;
	m_material->m_diffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
	m_material->m_specular.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_material->m_emission.set(0.0f, 0.0f, 0.0f, 1.0f);
}
//---------------------------------------------------------------------------

//===========================================================================
/*!
Render this mesh in OpenGL.  This method actually just prepares some
OpenGL state, and uses renderMesh to actually do the rendering.

\fn       void cMesh::render(const int a_renderMode)
\param    a_renderMode  Rendering mode (see cGenericObject)
*/
//===========================================================================
void rtsPolygon::render(cRenderOptions& a_options)
{
	if (m_showMesh)
	{
		/////////////////////////////////////////////////////////////////////////
		// Render parts that are always opaque
		/////////////////////////////////////////////////////////////////////////
		if (SECTION_RENDER_OPAQUE_PARTS_ONLY(a_options))
		{
			// render normals
			// (note that we will not render the normals if we are currently creating
			// a shadow map). 
			if (m_showNormals && !a_options.m_creating_shadow_map)
			{
				renderNormals(a_options);
			}

			// render edges
			if (m_showEdges && !a_options.m_creating_shadow_map)
			{
				renderEdges(a_options);
			}
		}

		/////////////////////////////////////////////////////////////////////////
		// Render parts that use material properties
		/////////////////////////////////////////////////////////////////////////
		if (SECTION_RENDER_PARTS_WITH_MATERIALS(a_options, m_useTransparency))
		{
			if (m_showTriangles)
			{
				renderMesh(a_options);
			}
		}
	}
}