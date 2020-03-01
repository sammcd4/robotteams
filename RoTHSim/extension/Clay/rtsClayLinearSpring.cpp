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
#include "rtsClayLinearSpring.h"
#include "rtsClayMesh.h"
//---------------------------------------------------------------------------
using namespace chai3d;

//===========================================================================
// DEFINITION - DEFAULT VALUES:
//===========================================================================

// Physical properties:
double rtsClayLinearSpring::s_default_kSpringElongation = 100.0; // [N/m]

// Graphic properties:
cColorf rtsClayLinearSpring::s_default_color(1.0f, 0.2f, 0.2f);


//===========================================================================
/*!
    Constructor of rtsClayLinearSpring.

    \fn     rtsClayLinearSpring::rtsClayLinearSpring(rtsClayMassParticle* a_node0, 
                                                rtsClayMassParticle* a_node1)
    \param  a_node0  Node 0.
    \param  a_node1  Node 1.
*/
//===========================================================================
rtsClayLinearSpring::rtsClayLinearSpring(rtsClayMassParticle* a_node0, rtsClayMassParticle* a_node1)
{
    // set nodes:
    m_node0 = a_node0;
    m_node1 = a_node1;
	m_enabled = true;

    // set default color
    m_color = s_default_color;

    // compute initial length
    m_length0 = cDistance(m_node1->m_pos, m_node0->m_pos);

    // set elongation spring constant [N/M]
    m_kSpringElongation = s_default_kSpringElongation;
}


//===========================================================================
/*!
    Destructor of rtsClayLinearSpring.

    \fn       rtsClayLinearSpring::~rtsClayLinearSpring()
*/
//===========================================================================
rtsClayLinearSpring::~rtsClayLinearSpring()
{
}



