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
#include "CGELPlaydohSkeletonNode.h"
//---------------------------------------------------------------------------
using namespace chai3d;

//===========================================================================
// DEFINITION - DEFAULT VALUES:
//===========================================================================

// Physical properties:
double cGELPlaydohSkeletonNode::s_default_radius				 = 0.05;
double cGELPlaydohSkeletonNode::s_default_kDampingPos			 = 5.00;
double cGELPlaydohSkeletonNode::s_default_muStaticFrictionPos	 = 10.0;
double cGELPlaydohSkeletonNode::s_default_muStaticFrictionRot	 = 0.5;
double cGELPlaydohSkeletonNode::s_default_muDynamicFrictionPos = 0.2;
double cGELPlaydohSkeletonNode::s_default_muDynamicFrictionRot = 0.2;
double cGELPlaydohSkeletonNode::s_default_kDampingRot			 = 0.1;
double cGELPlaydohSkeletonNode::s_default_mass				 = 0.1;  // [kg]
double cGELPlaydohSkeletonNode::s_default_stiffness			 = 10.0;

// Graphic properties:
bool cGELPlaydohSkeletonNode::s_default_showFrame       = true;
cColorf cGELPlaydohSkeletonNode::s_default_color(1.0f, 0.4f, 0.4f);
double cGELPlaydohSkeletonNode::s_default_frameScale = 1.0;

// Gravity field:
bool cGELPlaydohSkeletonNode::s_default_useGravity      = true;
cVector3d cGELPlaydohSkeletonNode::s_default_gravity(0.00, 0.00, -9.81);


//===========================================================================
/*!
    Constructor of cGELPlaydohSkeletonNode.

    \fn       cGELPlaydohSkeletonNode::cGELPlaydohSkeletonNode()
*/
//===========================================================================
cGELPlaydohSkeletonNode::cGELPlaydohSkeletonNode()
{
    m_pos.zero();
    m_rot.identity();
    m_radius        = s_default_radius;
    m_color         = s_default_color;
    m_externalForce.zero();
    m_force.zero();
    m_torque.zero();
    m_acc.zero();
    m_angAcc.zero();
    m_vel.zero();
    m_angVel.zero();
    m_kDampingPos   = s_default_kDampingPos;
	m_muStaticFrictionPos = s_default_muStaticFrictionPos;
	m_muDynamicFrictionPos = s_default_muDynamicFrictionPos;
	m_muStaticFrictionRot = s_default_muStaticFrictionRot;
	m_muDynamicFrictionRot = s_default_muDynamicFrictionRot;
    m_kDampingRot   = s_default_kDampingRot;
    m_gravity       = s_default_gravity;
    m_useGravity    = s_default_useGravity;
	m_stiffness		= s_default_stiffness;
    m_fixed         = false;
    setMass(s_default_mass);
	m_atRobot = false;
	m_newAtRobot = false;
	m_manipFixed = false;
	m_tempManipFixed = true;
	m_frameScale = s_default_frameScale;
	m_contacted = false;
	m_insideBldg = false;
	m_useStaticFriction = true;
	m_useDynamicFriction = false;
	m_IdNum = -1;
}


//===========================================================================
/*!
    Destructor of cGELPlaydohSkeletonNode.

    \fn       cGELPlaydohSkeletonNode::~cGELPlaydohSkeletonNode()
*/
//===========================================================================
cGELPlaydohSkeletonNode::~cGELPlaydohSkeletonNode()
{

}


//===========================================================================
/*!
    Define the mass of the node. The inertia is computed accordingly.

    \fn       void cGELPlaydohSkeletonNode::render()
*/
//===========================================================================
void cGELPlaydohSkeletonNode::setMass(double a_mass)
{
    m_mass = a_mass;
    m_inertia = (2.0 / 5.0) * m_mass * m_radius * m_radius ;
}


