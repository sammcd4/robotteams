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
#include "rtsClayLink.h"
#include "rtsClayMesh.h"
//---------------------------------------------------------------------------

//===========================================================================
// DEFINITION - DEFAULT VALUES:
//===========================================================================

// Physical properties:
double rtsClayLink::s_default_kSpringElongation = 100.0; // [N/m]
double rtsClayLink::s_default_kSpringFlexion    = 0.1;   // [Nm/RAD]
double rtsClayLink::s_default_kSpringTorsion   = 0.1;    // [Nm/RAD]

// Graphical properties:
cColorf rtsClayLink::s_default_color(0.2f, 0.2f, 1.0f);


//===========================================================================
/*!
    Constructor of rtsClayLink.

    \fn     rtsClayLink::rtsClayLink(rtsClayNode* a_node0,
                                               rtsClayNode* a_node1)
    \param  a_node0  Node 0.
    \param  a_node1  Node 1.
*/
//===========================================================================
rtsClayLink::rtsClayLink(rtsClayNode* a_node0, rtsClayNode* a_node1)
{
    // Set nodes:
    m_node0 = a_node0;
    m_node1 = a_node1;
	m_enabled = true;

    // set position references
    m_wLink01 = cSub(m_node1->m_pos, m_node0->m_pos);
    m_wLink10 = cSub(m_node0->m_pos, m_node1->m_pos);

	if (m_wLink01.length() == 0)
	{
		m_enabled = false;
	}

    m_wzLink01 = m_wLink01;
    m_wzLink10 = m_wLink10;

    m_nzLink01 = cMul(cTranspose(m_node0->m_rot), m_wzLink01);
    m_nzLink10 = cMul(cTranspose(m_node1->m_rot), m_wzLink10);

    // compute reference frames
    cVector3d v(1.0, 0.0, 0.0);
    double ang = cAngle(v, m_wLink01);
    if ((ang < 0.01) || (ang > 3.13)) { v.set(0.0, 1.0, 0.0); }
	
	cVector3d A0 = cNormalize(cCross(m_wLink01, v));
	cVector3d A1 = A0;
	cVector3d B0 = cNormalize(cCross(m_wLink01, A0));

	m_A0 = cMul(cTranspose(m_node0->m_rot), A0);
	m_B0 = cMul(cTranspose(m_node0->m_rot), B0);
	m_A1 = cMul(cTranspose(m_node1->m_rot), A1);

    // set default color
    m_color = s_default_color;

    // compute initial length
    m_length0 = cDistance(m_node1->m_pos, m_node0->m_pos);
    m_length = m_length0;

    // set elongation spring constant [N/M]
    m_kSpringElongation = s_default_kSpringElongation;

    // set flexion angular spring constant [NM/RAD]
    m_kSpringFlexion = s_default_kSpringFlexion;

    // set torsion angular spring constant [NM/RAD]
    m_kSpringTorsion = s_default_kSpringTorsion;
}


//===========================================================================
/*!
     Destructor of rtsClayLink.

      \fn       rtsClayLink::~rtsClayLink()
*/
//===========================================================================
rtsClayLink::~rtsClayLink()
{

}

