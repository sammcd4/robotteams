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
#ifndef CGELPlaydohMeshH
#define CGELPlaydohMeshH
//---------------------------------------------------------------------------
#include "CGELPlaydohSkeletonNode.h"
#include "CGELPlaydohSkeletonLink.h"
#include "CGELPlaydohLinearSpring.h"
#include "CGELPlaydohVertex.h"
#include "chai3d.h"
#include <typeinfo>
#include <vector>
#include <list>
#include <mutex>
//---------------------------------------------------------------------------
using std::vector;
using namespace chai3d;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       cGELPlaydohMesh.h

    \brief 
    <b> GEL Module </b> \n 
    Deformable Mesh.
*/
//===========================================================================

//===========================================================================
/*!
    \class      cGELPlaydohMesh
    \ingroup    GEL

    \brief      
    cGELPlaydohMesh inherits from cMesh and integrate a skeleton model for 
    deformation simulation.
*/
//===========================================================================
class cGELPlaydohMesh : public cMultiMesh
{

  public:

    //-----------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of cMesh.
    cGELPlaydohMesh(cWorld* a_world):cMultiMesh(){ 
		initialise(); 
		//a_world->addChild(this);
	};

    //! Destructor of cMesh.
    virtual ~cGELPlaydohMesh() {};


	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    //! Build dynamic vertices for deformable mesh.
    void buildVertices();

    //! Connect each vertex to skeleton.
    void connectVerticesToSkeleton(bool a_connectToNodesOnly);

    //! Update position of vertices connected to skeleton.
    void updateVertexPosition();

    //! Clear forces.
    void clearForces();

    //! Clear external forces.
    void clearExternalForces();

    //! Compute forces.
    void computeForces();

    //! Compute next pose.
    void computeNextPose(double a_timeInterval);

    //! Apply new computed pose.
    void applyNextPose();

	//! Update all dynamics in single pass
	void updateAllDynamics(double a_timeInterval);

    //! Render deformable mesh.
	virtual void render(cRenderOptions& a_options);

	// set mutex
	void setMutex(std::mutex* a_mutex);

	// get mutex pointer
	std::mutex* getMutex() { return m_mutex; }

	// set render nodes
	void setRenderNodes() { m_renderNodes = m_nodes; }

	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

    //! List of nodes composing the skeleton.
    list<cGELPlaydohSkeletonNode*> m_nodes;

	//! List of nodes composing the skeleton (used to render)
	list<cGELPlaydohSkeletonNode*> m_renderNodes;

    //! List of links connecting the different nodes.
    list<cGELPlaydohSkeletonLink*> m_links;

	//! List of links connecting the different nodes (used to render)
	list<cGELPlaydohSkeletonLink*> m_renderLinks;

    //! List of linear springs connecting vertices together.
    list<cGELPlaydohLinearSpring*> m_linearSprings;

    //! List of deformable vertices.
    vector<cGELPlaydohVertex> m_gelVertices;

    //! If \b true then display skeleton.
    bool m_showSkeletonModel;

    //! If \b true then display mass particle model.
    bool m_showMassParticleModel;

    //! Use skeleton model.
    bool m_useSkeletonModel;

    //! Use vertex mass particle model.
    bool m_useMassParticleModel;

	// mutex enable
	bool m_enableMutex;

	// mutex pointer
	std::mutex* m_mutex;

  private:

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    //! Initialize deformable mesh.
    void initialise();
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
