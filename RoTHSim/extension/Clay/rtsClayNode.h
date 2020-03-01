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
#ifndef rtsClayNodeH
#define rtsClayNodeH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "Other/rtsPolygonBounds.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       rtsClayNode.h

    \brief 
    <b> GEL Module </b> \n 
    Skeleton Node.
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsClayNode
    \ingroup    GEL

    \brief      
    rtsClayNode defines a dynamic node within the skeleton.
*/
//===========================================================================
class rtsClayNode
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of rtsClayNode.
    rtsClayNode();

    //! Destructor of rtsClayNode.
    ~rtsClayNode();


	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    //! Set the mass of the node.
    void setMass(double a_mass);

    //! Add force to node.
    inline void addForce(cVector3d &a_force)
    {
        m_force.add(a_force);
    }

    //! Add torque to node.
    inline void addTorque(cVector3d &a_torque)
    {
        m_torque.add(a_torque);
    }

    //! Set an external force to node.
    inline void setExternalForce(cVector3d &a_force)
    {
        m_externalForce = a_force;
    }

    //! Set an external torque to node.
    inline void setExternalTorque(cVector3d &a_torque)
    {
        m_externalTorque = a_torque;
    }

    //! Compute next position.
    inline void computeNextPose(double a_timeInterval)
    {
        if (!m_fixed)
        {
            // Euler double integration for position
            cVector3d damping;
            m_vel.mulr(-m_kDampingPos * m_mass, damping);
            m_force.add(damping);
			cVector3d force = cAdd(m_force, m_externalForce);

			// Ensure node overcomes static friction
			double staticFrictionForce = m_muStaticFrictionPos*m_mass*m_gravity.length();
			if (!m_useStaticFriction)
			{
				m_acc = cDiv(m_mass, force);
				m_vel = cAdd(m_vel, cMul(a_timeInterval, m_acc));
				m_nextPos = cAdd(m_pos, cMul(a_timeInterval, m_vel));
			}
			else if( force.length() > staticFrictionForce)
			{	
				//// Subtract dynamic friction force
				//if (m_useDynamicFriction)
				//{
				//	double dynamicFrictionForce = m_muDynamicFrictionPos*m_mass*m_gravity.length();
				//	double result = force.length() - dynamicFrictionForce;
				//	if (result < 0)
				//		force.zero();
				//	else force = cMul(result, cNormalize(force));
				//}

				m_acc = cDiv(m_mass, force );
				m_vel = cAdd(m_vel, cMul(a_timeInterval, m_acc));
				m_nextPos = cAdd(m_pos, cMul(a_timeInterval, m_vel));
			}
			else
			{
				m_nextPos = m_pos;
			}

			//if (m_atRobot)
			//	m_fixed = true;

        }
        else
        {
            m_nextPos = m_pos;
        }

        // Euler double integration for rotation
        cVector3d dampingAng;
        m_angVel.mulr(-m_kDampingRot * m_mass, dampingAng);
        m_torque.add(dampingAng);
        m_angAcc = cMul((1/m_inertia), m_torque);
        m_angVel = cAdd(m_angVel, cMul(a_timeInterval, m_angAcc));

        double normAngVel = m_angVel.length();
        if (normAngVel < 0.000001)
        {
            m_nextRot = m_rot;
        }
        else
        {
			m_nextRot = m_rot;
			m_nextRot.rotateAboutGlobalAxisRad(m_angVel, a_timeInterval * normAngVel);
		}
    }

    //! Update pose with new computed values.
    inline void applyNextPose()
    {
		m_nextPos.z(0); // on xy plane only
		m_prevPos = m_pos;
		m_pos = m_nextPos;
		m_rot = m_nextRot;
    }

    //! Clear forces and torques.
    inline void clearForces()
    {
        if (m_useGravity)
        {
            m_force = m_gravity;
            m_force.mul(m_mass);
        }
        else
        {
            m_force.zero();
        }
        m_torque.zero();
    }

    //! Clear external forces and torques.
    inline void clearExternalForces()
    {
        m_externalForce.zero();
        m_externalTorque.zero();
    }

    //! Render node in OpenGL.
    inline void render()
    {
        // set pose
		chai3d::cTransform mat;
		mat.set(m_pos, m_rot);
		glPushMatrix();
		glMultMatrixd((const double *)mat.getData());

        // draw node
        m_color.render();
        chai3d::cDrawSphere(m_radius, 12, 12);

        // draw frame
        if (s_default_showFrame == true)
        {
            chai3d::cDrawFrame(m_frameScale * m_radius);
        }

		// pop OpenGL matrix
		glPopMatrix();

        // render external forces
        glColor4fv( (const float *)&m_color);
        cVector3d v = cAdd(m_pos, cMul(1.0/50.0, m_externalForce));
        glBegin(GL_LINES);
          glVertex3dv( (const double *)&m_pos);
          glVertex3dv( (const double *)&v);
        glEnd();
    }

	// set stiffness
	void setStiffness(double a_stiffness) { m_stiffness = a_stiffness; }

	// set frameScale
	void setFrameScale(double a_frameScale) { m_frameScale = a_frameScale; }

	//-----------------------------------------------------------------------
    // MEMBERS - GRAPHICAL PROPERTIES:
    //-----------------------------------------------------------------------

    //! Color used to display nodes.
    cColorf m_color;

	// frame scale
	double m_frameScale;

	//-----------------------------------------------------------------------
    // MEMBERS - PHYSICAL PROPERTIES:
    //-----------------------------------------------------------------------

    //! Radius of mass node.
    double m_radius;

    //! Mass property.
    double m_mass;

	// contact state
	bool m_contacted;

	// inside building
	bool m_insideBldg;

    //! Current force applied on node.
    cVector3d m_force;

    //! Current torque applies on node.
    cVector3d m_torque;

    //! Instant acceleration at node.
    cVector3d m_acc;

    //! Instant angular acceleration at node.
    cVector3d m_angAcc;

    //! Instant velocity at node.
    cVector3d m_vel;

    //! Instant angular velocity at node.
    cVector3d m_angVel;

    //! Linear damping.
    double m_kDampingPos;

	// Linear static friction
	double m_muStaticFrictionPos;

	// Linear dynamic friction
	double m_muDynamicFrictionPos;

    //! Angular damping.
    double m_kDampingRot;

	// Angular static friction
	double m_muStaticFrictionRot;

	// Angular dynamic friction
	double m_muDynamicFrictionRot;

	// stiffness
	double m_stiffness;

    //! Inertia (to be completed).
    double m_inertia;

    //! If \b true, then mass is fixed in space and can not move.
    bool m_fixed;

    //! f \b true, then gravity is enabled.
    bool m_useGravity;

	// enable or disable static friction
	bool m_useStaticFriction;

	// enable or disable dynamic friction
	bool m_useDynamicFriction;

    //! Gravity field.
    cVector3d m_gravity;



	//-----------------------------------------------------------------------
    // MEMBERS - POSITION & ORIENTATION:
    //-----------------------------------------------------------------------

    //! Position computed.
    cVector3d m_pos;

	//! Previous position computed
	cVector3d m_prevPos;

	//! Entry to building direction
	cVector3d m_entryVec;

    //! Rotation computed.
    cMatrix3d m_rot;

    //! Next position computed.
    cVector3d m_nextPos;

    //! Next rotation computed.
    cMatrix3d m_nextRot;

	// located at robot location
	bool m_atRobot;

	// just been changed to at robot location
	bool m_newAtRobot;

	// id number
	int m_IdNum;

	// fixed from manipulation
	bool m_manipFixed;

	// temporarily fixed prior to manipulation
	bool m_tempManipFixed;

  private:

	//-----------------------------------------------------------------------
    // MEMBERS - EXTERNAL FORCES:
    //-----------------------------------------------------------------------
      
    //! External force.
    cVector3d m_externalForce;

    //! External torque.
    cVector3d m_externalTorque;

  public:

	//-----------------------------------------------------------------------
    // MEMBERS - DEFAULT SETTINGS:
    //-----------------------------------------------------------------------

    //! Default value for node radius.
    static double s_default_radius;

    //! Default value for linear damping.
    static double s_default_kDampingPos;

	// Default value for linear static friction
	static double s_default_muStaticFrictionPos;

	// Default value for linear dynamics friction
	static double s_default_muDynamicFrictionPos;

    //! Default value for revolute damping.
    static double s_default_kDampingRot;

	// Default value for angular static friction
	static double s_default_muStaticFrictionRot;

	// Default value for angular dynamics friction
	static double s_default_muDynamicFrictionRot;

    //! Default value for node mass.
    static double s_default_mass;

	// Default stiffness
	static double s_default_stiffness;

    //! Defualt value - Is gravity field enabled?
    static bool s_default_useGravity;

    //! Default value - Gravity field magnitude and direction.
    static cVector3d s_default_gravity;

    //! Default value - Is the node reference frame displayed?
    static bool s_default_showFrame;

	// Default frame scale
	static double s_default_frameScale;

    //! Default color used to render the node.
    static cColorf s_default_color;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------


