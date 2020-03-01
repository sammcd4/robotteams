
//---------------------------------------------------------------------------
#ifndef rtsClayStripH
#define rtsClayStripH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsClay3D.h"
#include "rtsClayNode.h"
#include <list>
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       rtsClayStrip.h

    \brief 
    <b> Scenegraph </b> \n 
    Virtual rtsClay potential field
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsClayStrip
    \ingroup    scenegraph

    \brief      
    Implementation of a virtual rtsClay field
*/
//===========================================================================
class rtsClayStrip
{
  public:
    
	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsClayStrip.
	rtsClayStrip(cWorld* a_world, rtsClayMesh* a_playdohMesh, bool a_isCurved, bool a_isPrevStrip, bool a_isFutureStrip, cVector3d a_p0, cVector3d a_p1, cVector3d a_p2, cVector3d a_p3);
    
	//! Destructor of cSphere.
	virtual ~rtsClayStrip() {};

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
	
	// get size of strip sphere list
	int getNumSpheres() { return m_spheres.size(); }

	// set rtsClay strip stiffness
	void setStiffness(double a_stiffness);

	// update clay strip given new robot positions
	void updateClayStrip(cVector3d a_p0, cVector3d a_p1, cVector3d a_p2, cVector3d a_p3);

	// compute haptic force against playdoh
	cVector3d computeForce(const cVector3d& a_cursor, double a_cursorRadius);

	// compute individual node force
	cVector3d computeNodeForce(const cVector3d& a_cursor,
		double a_cursorRadius,
		const cVector3d& a_spherePos,
		double a_radius,
		double a_stiffness);

	// create links between nodes
	void createLinks();

  protected:

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------

	// create rtsClay strip from p1 to p2 and append spheres to list
	void createStrip(cVector3d a_p1, cVector3d a_p2);

	// create curved rtsClay strip from p1 to p2 and append spheres to list
	// p0 and p3 dictate the direction of the curve.
	void createCurvedStrip();

	// create straight line rtsClay strip from p1 to p2 and append spheres to list
	void createLineStrip();

	// set curvature
	void setIsCurved(bool a_isCurved);

	// get curvature
	bool getIsCurved() { return m_isCurved; }

	// add a sphere between two spheres in the list, sphere goes before iterator
	void addSphere(rtsClayNode* a_sphere1, rtsClayNode* a_sphere2, list<rtsClayNode*>::iterator iter);

	// remove a sphere between two spheres in the list
	list<rtsClayNode*>::iterator removeSphere(list<rtsClayNode*>::iterator iter);



    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

	// parent strip world
	cWorld* m_world;

	// GEL deformable world
	rtsClayMesh* m_playdohMesh;

	// Potential field sphere elements that build the rtsClay strip
	list<rtsClayNode*> m_spheres;

	// previous robot position
	cVector3d m_p0;

	// first robot position of strip
	cVector3d m_p1;

	// second robot position of strip
	cVector3d m_p2;

	// future robot position
	cVector3d m_p3;

	// is this strip curved?
	bool m_isCurved;

	// is connected to a previous strip?
	bool m_isPrevStrip;

	// is connected to a future strip?
	bool m_isFutureStrip;
	
	// default spacing between spheres
	double m_defaultRadiusToSpacingRatio;

	// sphere spacing
	double m_sphereSpacing;

	// default sphere radius
	double m_defaultSphereRadius;

	// minimum sphere radius to keep rtsClay intact (split is possible)
	double m_minSphereRadius;

};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
