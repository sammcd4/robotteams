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
#ifndef rtsClayVertexH
#define rtsClayVertexH
//---------------------------------------------------------------------------
#include "rtsClayMassParticle.h"
#include "rtsClayLink.h"
#include "rtsClayNode.h"
#include "chai3d.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       rtsClayVertex.h

    \brief 
    <b> GEL Module </b> \n 
    Deformable Mesh Vertex.
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsClayVertex
    \ingroup    GEL

    \brief      
    rtsClayVertex implement a cVertex class with a mass particle for 
    simulating deformable objects
*/
//===========================================================================
class rtsClayVertex
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of rtsClayVertex.
	  rtsClayVertex(chai3d::cMesh* a_mesh, unsigned int a_vertexIndex);

    //! Destructor of rtsClayVertex.
    virtual ~rtsClayVertex();


	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

	//! Vertex index.
	unsigned int m_vertexIndex;

	//! Mesh object
	chai3d::cMesh* m_mesh;

    //! Mass particle for current vertex.
    rtsClayMassParticle* m_massParticle;

    //! Skeleton link to which this vertex may be linked to.
    rtsClayLink* m_link;

    //! Skeleton node to which this vertex may be linked to.
    rtsClayNode* m_node;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
