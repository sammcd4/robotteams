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
#include "CGELPlaydohMassParticle.h"
//---------------------------------------------------------------------------
using namespace chai3d;

//===========================================================================
// DEFINITION - DEFAULT VALUES:
//===========================================================================

// Physical properties:
double cGELPlaydohMassParticle::s_default_kDampingPos   = 5.00;
double cGELPlaydohMassParticle::s_default_mass          = 0.1;  // [kg]

// Graphic properties:
cColorf cGELPlaydohMassParticle::s_default_color(1.0f, 1.0f, 0.4f);

// Gravity field:
bool cGELPlaydohMassParticle::s_default_useGravity      = true;
cVector3d cGELPlaydohMassParticle::s_default_gravity(0.00, 0.00, -9.81);

// Default rendering mode
bool cGELPlaydohMassParticle::s_show_forces = true;
bool cGELPlaydohMassParticle::s_scale_force_vector_display = 0.01;


//===========================================================================
/*!
    Constructor of cGELPlaydohMassParticle.

    \fn       cGELPlaydohMassParticle::cGELPlaydohMassParticle()
*/
//===========================================================================
cGELPlaydohMassParticle::cGELPlaydohMassParticle()
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
    Destructor of cGELPlaydohMassParticle.

    \fn       cGELPlaydohMassParticle::~cGELPlaydohMassParticle()
*/
//===========================================================================
cGELPlaydohMassParticle::~cGELPlaydohMassParticle()
{

}


//===========================================================================
/*!
    Define the mass of the mass particle

    \fn       void cGELPlaydohMassParticle::render()
*/
//===========================================================================
void cGELPlaydohMassParticle::setMass(double a_mass)
{
    m_mass = a_mass;
}


