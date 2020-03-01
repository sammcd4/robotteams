//===========================================================================
/*
    This file is part of the GEL dynamics engine.
    Copyright (C) 2003-2009 by Francois Conti, Stanford University.
    All rights reserved.

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   2.0.0 $Rev: 248 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "CGELPlaydohVertex.h"
//---------------------------------------------------------------------------
using namespace chai3d;

//===========================================================================
/*!
Constructor of cGELVertex.

\param  a_mesh         Parent mesh.
\param  a_vertexIndex  Index number of mesh vertex.
*/
//==========================================================================
cGELPlaydohVertex::cGELPlaydohVertex(chai3d::cMesh* a_mesh, unsigned int a_vertexIndex)
{
	m_mesh = a_mesh;
	m_vertexIndex = a_vertexIndex;
	m_massParticle = new cGELPlaydohMassParticle();
	m_link = NULL;
	m_node = NULL;
	m_massParticle->m_pos = a_mesh->m_vertices->getLocalPos(a_vertexIndex);
}


//===========================================================================
/*!
    Destructor of cGELPlaydohVertex.

    \fn       cGELPlaydohVertex::~cGELPlaydohVertex()
*/
//==========================================================================
cGELPlaydohVertex::~cGELPlaydohVertex()
{

}
