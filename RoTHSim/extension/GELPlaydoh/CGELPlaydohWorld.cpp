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
#include "CGELPlaydohWorld.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

//==========================================================================
/*!
    Extends cGELPlaydohWorld to support collision detection.

    \fn     bool cGELPlaydohWorldCollision::computeCollision(cVector3d& a_segmentPointA,
                                        cVector3d& a_segmentPointB,
                                        cCollisionRecorder& a_recorder,
                                        cCollisionSettings& a_settings)
    \param    a_segmentPointA  Start point of segment.
    \param    a_segmentPointB  End point of segment.
    \param    a_recorder  Stores all collision events
    \param    a_settings  Contains collision settings information.
    \return   Return \b true if a collision has occurred.
*/
//===========================================================================
bool cGELPlaydohWorldCollision::computeCollision(cVector3d& a_segmentPointA,
                                          cVector3d& a_segmentPointB,
                                          cCollisionRecorder& a_recorder,
                                          cCollisionSettings& a_settings)
{
    list<cGELPlaydohMesh*>::iterator i;
    bool result = false;
    for(i = m_gelWorld->m_gelMeshes.begin(); i != m_gelWorld->m_gelMeshes.end(); ++i)
    {
        cGELPlaydohMesh *nextItem = *i;
        bool collide = nextItem->computeCollisionDetection(a_segmentPointA, a_segmentPointB, a_recorder,
                        a_settings);
        if (collide) { result = true; }
    }
    return (result);
}


//==========================================================================
/*!
    Constructor of cDefWorld.

    \fn       cGELPlaydohWorld::cGELPlaydohWorld()
    \return   Return pointer to new cGELPlaydohWorld instance.
*/
//===========================================================================
cGELPlaydohWorld::cGELPlaydohWorld()
{
    // reset simulation time.
    m_simulationTime = 0.0;

    // create a collision detector for world
    m_collisionDetector = new cGELPlaydohWorldCollision(this);

	// choose to use efficient updating method
	m_useEfficientUpdating = false;
}


//===========================================================================
/*!
    Destructor of cGELPlaydohWorld.

    \fn       cGELPlaydohWorld::~cGELPlaydohWorld()
*/
//===========================================================================
cGELPlaydohWorld::~cGELPlaydohWorld()
{
    m_gelMeshes.clear();
}


//===========================================================================
/*!
This method render this world in OpenGL.

\param  a_options  Rendering options.
*/
//===========================================================================
void cGELPlaydohWorld::render(cRenderOptions& a_options)
{
	list<cGELPlaydohMesh*>::iterator i;

	for (i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
	{
		cGELPlaydohMesh* nextItem = *i;
		nextItem->renderSceneGraph(a_options);
	}
}

//===========================================================================
/*!
    Clear external forces on all deformable objects in scene.

    \fn       void cGELPlaydohWorld::clearExternalForces()
*/
//===========================================================================
void cGELPlaydohWorld::clearExternalForces()
{
    list<cGELPlaydohMesh*>::iterator i;

    for(i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
    {
        cGELPlaydohMesh *nextItem = *i;
        nextItem->clearExternalForces();
    }
}


//===========================================================================
/*!
    Compute simulation for a_time time interval.

    \fn       void cGELPlaydohWorld::updateDynamics(double a_time)
*/
//===========================================================================
void cGELPlaydohWorld::updateDynamics(double a_timeInterval)
{
	if (m_useEfficientUpdating)
	{
		list<cGELPlaydohMesh*>::iterator i;

		// clear all internal forces of each model
		for (i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
		{
			cGELPlaydohMesh *nextItem = *i;
			nextItem->updateAllDynamics(a_timeInterval);
		}
	}
	else
	{
		list<cGELPlaydohMesh*>::iterator i;

		// clear all internal forces of each model
		for (i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
		{
			cGELPlaydohMesh *nextItem = *i;
			nextItem->clearForces();
		}

		// compute all internal forces for each model
		for (i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
		{
			cGELPlaydohMesh *nextItem = *i;
			nextItem->computeForces();
		}

		// compute next pose of model
		for (i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
		{
			cGELPlaydohMesh *nextItem = *i;
			nextItem->computeNextPose(a_timeInterval);
		}

		// apply next pose
		for (i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
		{
			cGELPlaydohMesh *nextItem = *i;
			nextItem->applyNextPose();
		}

	}

	// update simulation time
	m_simulationTime = m_simulationTime + a_timeInterval;
}

//===========================================================================
/*!
    Update vertices of all objects.

    \fn       void cGELPlaydohWorld::updateDynamics(double a_time)
*/
//===========================================================================
void cGELPlaydohWorld::updateSkins(bool a_updateNormals)
{
    // update surface mesh to latest skeleton configuration
    list<cGELPlaydohMesh*>::iterator i;
    for(i = m_gelMeshes.begin(); i != m_gelMeshes.end(); ++i)
    {
        cGELPlaydohMesh *nextItem = *i;
        nextItem->updateVertexPosition();

		if (a_updateNormals)
		{
			nextItem->computeAllNormals();
		}
    }
}

