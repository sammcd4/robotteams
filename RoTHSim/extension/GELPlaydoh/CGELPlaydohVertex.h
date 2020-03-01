//===========================================================================
/*
    This file is part of the GEL dynamics engine.
    Copyright (C) 2003-2009 by Francois Conti, Stanford University.
    All rights reserved.

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   2.0.0 $Rev: 251 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CGELPlaydohVertexH
#define CGELPlaydohVertexH
//---------------------------------------------------------------------------
#include "CGELPlaydohMassParticle.h"
#include "CGELPlaydohSkeletonLink.h"
#include "CGELPlaydohSkeletonNode.h"
#include "chai3d.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       cGELPlaydohVertex.h

    \brief 
    <b> GEL Module </b> \n 
    Deformable Mesh Vertex.
*/
//===========================================================================

//===========================================================================
/*!
    \class      cGELPlaydohVertex
    \ingroup    GEL

    \brief      
    cGELPlaydohVertex implement a cVertex class with a mass particle for 
    simulating deformable objects
*/
//===========================================================================
class cGELPlaydohVertex
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of cGELPlaydohVertex.
	  cGELPlaydohVertex(chai3d::cMesh* a_mesh, unsigned int a_vertexIndex);

    //! Destructor of cGELPlaydohVertex.
    virtual ~cGELPlaydohVertex();


	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

	//! Vertex index.
	unsigned int m_vertexIndex;

	//! Mesh object
	chai3d::cMesh* m_mesh;

    //! Mass particle for current vertex.
    cGELPlaydohMassParticle* m_massParticle;

    //! Skeleton link to which this vertex may be linked to.
    cGELPlaydohSkeletonLink* m_link;

    //! Skeleton node to which this vertex may be linked to.
    cGELPlaydohSkeletonNode* m_node;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
