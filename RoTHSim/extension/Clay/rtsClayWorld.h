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
#ifndef rtsClayWorldH
#define rtsClayWorldH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsClayMesh.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       rtsClayWorld.h

    \brief 
    <b> GEL Module </b> \n 
    Virtual World.
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsClayWorld
    \ingroup    GEL

    \brief      
    rtsClayWorld implements a world to handle deformable objects within CHAI 3D.
*/
//===========================================================================
class rtsClayWorld : public cGenericObject
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of rtsClayWorld.
    rtsClayWorld();

    //! Destructor of rtsClayWorld.
    ~rtsClayWorld();


	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

	//! This method integrates the dynamic simulation over a time period passed as argument.
	void updateDynamics(double a_timeInterval);

	//! This method clears all external forces applied on all deformable objects.
	void clearExternalForces();

	//! This method updates the mesh of all deformable objects.
	void updateSkins(bool a_updateNormals = true);



	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    //! List of deformable solids.
    list<rtsClayMesh*> m_gelMeshes;

    //! Current time of simulation.
    double m_simulationTime;

    //! Gravity constant.
    cVector3d m_gravity;

	// use efficient updating method
	bool m_useEfficientUpdating;

  private:

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
	
	//! This method renders graphically all deformable objects contained in the world.
	virtual void render(chai3d::cRenderOptions& a_options);
};


//===========================================================================
/*!
    \class      rtsClayWorldCollision
    \ingroup    GEL

    \brief      
    rtsClayWorldCollision provides a collision detection model to support 
    deformable objects.
*/
//===========================================================================
class rtsClayWorldCollision : public cGenericCollision
{
  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of rtsClayWorldCollision.
    rtsClayWorldCollision(rtsClayWorld* a_gelWorld) {m_gelWorld = a_gelWorld;}

    //! Destructor of rtsClayWorldCollision.
    virtual ~rtsClayWorldCollision() {};


	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    //! Do any necessary initialization, such as building trees.
    virtual void initialize() {};

    //! Provide a visual representation of the method.
    virtual void render() {};

    //! Return the nearest triangle intersected by the given segment, if any.
    virtual bool computeCollision(cVector3d& a_segmentPointA,
                                  cVector3d& a_segmentPointB,
                                  cCollisionRecorder& a_recorder,
                                  cCollisionSettings& a_settings);

  private:


	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    //! Deformable world
    rtsClayWorld* m_gelWorld;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------


