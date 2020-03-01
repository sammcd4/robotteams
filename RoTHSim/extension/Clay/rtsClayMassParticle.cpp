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
#include "rtsClayMassParticle.h"
//---------------------------------------------------------------------------
using namespace chai3d;

//===========================================================================
// DEFINITION - DEFAULT VALUES:
//===========================================================================

// Physical properties:
double rtsClayMassParticle::s_default_kDampingPos   = 5.00;
double rtsClayMassParticle::s_default_mass          = 0.1;  // [kg]

// Graphic properties:
cColorf rtsClayMassParticle::s_default_color(1.0f, 1.0f, 0.4f);

// Gravity field:
bool rtsClayMassParticle::s_default_useGravity      = true;
cVector3d rtsClayMassParticle::s_default_gravity(0.00, 0.00, -9.81);

// Default rendering mode
bool rtsClayMassParticle::s_show_forces = true;
bool rtsClayMassParticle::s_scale_force_vector_display = 0.01;


//===========================================================================
/*!
    Constructor of rtsClayMassParticle.

    \fn       rtsClayMassParticle::rtsClayMassParticle()
*/
//===========================================================================
rtsClayMassParticle::rtsClayMassParticle()
{
    m_pos.zero();
    m_color         = s_default_color;
    m_externalForce.zero();
    m_force.zero();
    m_acc.zero();
    m_vel.zero();
    m_kDampingPos   = s_default_kDampingPos;
    m_gravity       = s_default_gravity;
    m_useGravity    = s_default_useGravity;
    m_fixed         = false;
    setMass(s_default_mass);
}


//===========================================================================
/*!
    Destructor of rtsClayMassParticle.

    \fn       rtsClayMassParticle::~rtsClayMassParticle()
*/
//===========================================================================
rtsClayMassParticle::~rtsClayMassParticle()
{

}


//===========================================================================
/*!
    Define the mass of the mass particle

    \fn       void rtsClayMassParticle::render()
*/
//===========================================================================
void rtsClayMassParticle::setMass(double a_mass)
{
    m_mass = a_mass;
}


