
//---------------------------------------------------------------------------
#ifndef rtsClayH
#define rtsClayH
//---------------------------------------------------------------------------
//#include "chai3d.h"
#include "rtsClay3D.h"
#include "rtsClayStrip.h"
#include "rtsClayNode.h"
#include "Other/rtsPolygonBounds.h"
#include <list>
#include <algorithm>
#include <mutex>
//#include "CVector3dExt.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       rtsClay.h

    \brief 
    <b> Scenegraph </b> \n 
    Virtual rtsClay potential field
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsClay
    \ingroup    scenegraph

    \brief      
    Implementation of a virtual rtsClay field
*/
//===========================================================================
class rtsClay
{
  public:
    
	////-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	////-----------------------------------------------------------------------

	//! Constructor of rtsClay. (must call setConvexHull, setConvexHullMapping and initPlaydoh methods after instantiating)
	rtsClay(cWorld* a_world, vector<cVector3d> a_robotPositions, double a_toolRadius, double a_toolWorkspaceScaleFactor, double a_maxStiffness, bool a_useBuildingBounds = true);
    
	//! Destructor of cSphere.
	~rtsClay();

	////-----------------------------------------------------------------------
	// METHODS:
	////-----------------------------------------------------------------------
	
	// get parent world pointer
	cWorld* getWorld() { return m_world; }

	// get GEL world pointer
	rtsClayWorld* getPlaydohWorld() { return m_defWorld; }

	// get list of robot spheres for travel
	vector<rtsClayNode*> getRobotTravelSpheres() { return m_robotTravelSpheres; }

	// get list of robot spheres for surround
	vector<rtsClayNode*> getRobotSurroundSpheres() { return m_robotSurroundSpheres; }

	// get robot positions
	vector<cVector3d> getRobotPositions() { return m_robotPositions; }

	// initialize playdoh to create travel correctly
	void initPlaydoh();

	// set show playdoh
	void setShowEnabled(bool a_showEnabled, bool a_affectChildren);

	// get show playdoh
	bool getShowEnabled();

	// set haptic enabled
	void setHapticEnabled(bool a_hapticEnabled, bool a_affectChildren);

	// get haptic enabled
	bool getHapticEnabled() { return m_hapticEnabled; }

	// get spring stiffness
	double getSpringStiffness() { return m_kSpringElongation; }

	// get default spring stiffness
	double getDefaultSpringStiffness() { return m_default_kSpringElongation; }

	// set rtsClay stiffness
	void setStiffness(double a_stiffness);

	// update the robot positions directly
	void updateRobotPositions(vector<cVector3d> a_robotPositions);

	// set convex hull positions
	void setConvexHull(vector<cVector3d> a_convexHull);

	// set convex hull mapping
	void setConvexHullMapping(vector<int> a_convexHullMapping);

	// get convex hull positions
	vector<cVector3d> getConvexHull() { return m_convexHullRobotPositions; }

	// get playdoh force
	cVector3d getPlaydohForce() { return m_playdohForce;}

	// get previous playdoh force
	cVector3d getPrevPlaydohForce() { return m_prevPlaydohForce;}
	
	// get tool force
	cVector3d getToolForce() { return m_toolForce; }

	// get previous tool force
	cVector3d getPrevToolForce() { return m_prevToolForce; }

	// get default sphere spacing
	double getDefaultSphereSpacing() { return m_defaultSphereSpacing; }

	// get sphere spacing
	double getSphereSpacing() { return m_sphereSpacing; }

	// get default radius to spacing ratio
	double getRadiusToSpacingRatio() { return m_defaultRadiusToSpacingRatio; }

	// get default sphere radius
	double getDefaultSphereRadius() { return m_defaultSphereRadius; }

	// compute haptic force against playdoh
	cVector3d computeForce(const cVector3d& a_cursor, double a_cursorRadius, int a_closestIndex, 
						double a_modFactor, bool a_manip, bool a_manipSpacing);

	// update dynamics
	void updateDynamics(double a_timeInterval);

	// compute individual node force
	cVector3d computeNodeForce(const cVector3d& a_cursor,
		double a_cursorRadius,
		const cVector3d& a_spherePos,
		double a_radius,
		double a_stiffness);

	// compute idividual node force (cylinder)
	cVector3d computeCylNodeForce(const cVector3d& a_cursor,
		double a_cursorRadius,
		const cVector3d& a_spherePos,
		double a_radius,
		double a_stiffness);

	void setBuildingBounds(vector<rtsPolygonBounds> a_buildingBounds);

	// set mode (0 - travel, 1 - surround)
	void setDistributionMode(int a_mode);

	// get distribution mode
	int getDistributionMode() { return m_mode; }

	// set mutex
	void setMutex(std::mutex* a_mutex);

	// get mutex pointer
	std::mutex* getMutex() { return m_mutex; }

	// get workspace scale
	double getWorkspaceScale() { return m_workspaceScale; }

	// set workspace scale
	void setWorkspaceScale(double a_workspaceScale) { m_workspaceScale = a_workspaceScale; }

	// set tool workspace scale factor (defined by haptic device used)
	void setToolWorkspaceScaleFactor(double a_toolWorkspaceScaleFactor) { m_toolWorkspaceScaleFactor = a_toolWorkspaceScaleFactor; }

	// get tool workspace scale factor
	double getToolWorkspaceScaleFactor() { return m_toolWorkspaceScaleFactor; }

	// get center of mass of playdoh
	cVector3d getCenterOfMass() { return m_centerOfMass; }

	// check if position is inside bounding box
	bool insideBoundingBox(cVector3d a_pos);

	// get contact state
	bool getContactState() { return m_contacted; }

	// get previos contact state
	bool getPrevContactState() { return m_prevContacted; }

	//-----------------------------------------------------------------------
	// PATROL METHODS:
	//-----------------------------------------------------------------------
	
	// get list of potential spheres for travel
	vector<rtsClayNode*> getTravelSpheres() { return m_travelSpheres; }
	
	// get playdoh mesh in GEL world for travel
	rtsClayMesh* getTravelMesh() { return m_travelMesh; }	
	
	// get number of spheres in rtsClay for travel
	int getNumTravelSpheres();	
	
	// get travel mode
	bool getTravelMode() { return m_travelMode; }

	// set travel mode
	void setTravelMode(bool a_travelMode) { m_travelMode = a_travelMode; }

	// set use travel links
	void setUseTravelLinks(bool a_useTravelLinks) {m_useTravelLinks = a_useTravelLinks;}

	// get use travel links
	bool getUseTravelLinks() { return m_useTravelLinks;}

	// get reset travel
	bool getResetTravel() { return m_resetTravel; }

	//-----------------------------------------------------------------------
	// CORDON METHODS:
	//-----------------------------------------------------------------------

	// get list of potential spheres for surround
	vector<rtsClayNode*> getSurroundSpheres() { return m_surroundSpheres; }

	// get playdoh mesh in GEL world for surround
	rtsClayMesh* getSurroundMesh() { return m_surroundMesh; }

	// get number of spheres in rtsClay for surround
	int getNumSurroundSpheres();

	// get surround mode
	bool getSurroundMode() { return m_surroundMode; }

	// set surround mode
	void setSurroundMode(bool a_surroundMode) { m_surroundMode = a_surroundMode; }

	// get perimeter
	double getPerimeter() { return m_perimeter; }

	// get large perimeter value (2 X geometry perimeter)
	double getLargePerimeter() { return m_largePerimeter; }

	// get static friction radius
	double getStaticFrictionRadius() { return m_staticFrictionR; }

	// get geometry perimeter
	double getGeomPerimeter() { return m_geomPerimeter; }

	// set resize surround
	void setEnableResizeSurround(bool a_resizeSurround);

	// get resize surround
	bool getEnableResizeSurround() { return m_resizeSurround; }

	// set resize percentage
	void setResizePercentage(double a_resizePercentage);

	// get resize percentage
	double getResizePercentage() { return m_resizePercentage; }

	// compute virtual polygon
	void computeVirtualPolygon();

	// get virtual polygon during surround movement
	vector<cVector3d> getVirtualPolygon();

	// compute attraction point
	void computeAttractionPt();

	// reset spacing factors
	void resetSpacingFactors();


  protected:

	////-----------------------------------------------------------------------
	// METHODS:
	////-----------------------------------------------------------------------

	// reset all modes and set mode
	void setMode(int a_mode);
	
	// create straight-line nodes connecting robots
	void createStraightNodes(vector<cVector3d> a_robotPositions);

	// initialize entry vectors for nodes
	void initEntryVecs();

	// create curved nodes connecting robots
	void createCurvedNodes(vector<cVector3d> a_robotPositions);

	// create nodes connecting robots (Catmull Rom spline)
	void createSplinedNodes(vector<cVector3d> a_robotPositions);

	// create links
	void createNodeLinks();

	// delete links
	void deleteNodeLinks();

	//-----------------------------------------------------------------------
	// PATROL METHODS:
	//-----------------------------------------------------------------------
	
	// initialize playdoh sphere parameters for travel
	void initTravelParams();

	// initialize playdoh spheres for travel
	void initTravel();

	// initialize playdoh spheres for travel
	void initTravel(bool a_show);

	// delete playdoh spheres for travel
	void deleteTravel();

	//-----------------------------------------------------------------------
	// CORDON METHODS:
	//-----------------------------------------------------------------------
	
	// initialize playdoh sphere parameters for surround
	void initSurroundParams();

	// initialize playdoh spheres for surround
	void initSurround();

	// delete playdoh spheres for surround
	void deleteSurround();

	// initialize playdoh perimeter
	void initPerimeter();

	// resize surround
	void resizeSurround();

	// update all spacing factors
	void updateSpacingFactors(int a_mod1, double a_modFactor);

	// compute a linear spacing factor
	double computeLinearSpacingFactor(double a_minSpacing, double a_maxSpacing, int a_numRegions, int a_regionNum, bool a_increasing);

	// start a transition period
	void startTransition();

	// end a transition period
	void endTransition();

    ////-----------------------------------------------------------------------
    // MEMBERS:
    ////-----------------------------------------------------------------------

	// parent world
	cWorld* m_world;

	// building bounds
	vector<rtsPolygonBounds> m_buildingBounds;

	// GEL world
	rtsClayWorld* m_defWorld;

	// robot positions vector
	vector<cVector3d> m_robotPositions;

	// spheres that contain robots
	list<rtsClayNode*> m_robotSpheres;

	// robot perimeter (measured from robot to robot)
	double m_robotPerimeter;

	// default playdoh stiffness
	double m_default_kSpringElongation;

	// global playdoh stiffness
	double m_kSpringElongation;

	// playdoh force
	cVector3d m_playdohForce;

	// previous playdoh force
	cVector3d m_prevPlaydohForce;

	// total tool force
	cVector3d m_toolForce;

	// tool radius
	double m_toolRadius;

	// previous total tool force
	cVector3d m_prevToolForce;

	// default spacing between spheres
	double m_defaultSphereSpacing;

	// sphere spacing
	double m_sphereSpacing;

	// default spacing between spheres
	double m_defaultRadiusToSpacingRatio;

	// default sphere radius
	double m_defaultSphereRadius;

	// affect children haptically
	bool m_affectHapticChildren;

	// show enable
	bool m_showEnabled;

	// haptic effect enabled for playdoh
	bool m_hapticEnabled;

	// distribution mode
	int m_mode;

	// default mode
	int m_defaultMode;

	// standby mode
	bool m_standbyMode;

	// mutex enable
	bool m_enableMutex;

	// mutex pointer
	std::mutex* m_mutex;

	// workspace scale factor
	double m_workspaceScale;

	// tool workspace scale factor
	double m_toolWorkspaceScaleFactor;

	// max node stiffness
	double m_maxNodeStiffness;

	// center of mass location
	cVector3d m_centerOfMass;

	// playdoh bounding box positive values
	cVector3d m_boundingBoxPos;

	// playdoh bounding box negative values
	cVector3d m_boundingBoxNeg;

	// contact state of playdoh
	bool m_contacted;

	// previous contact state of playdoh
	bool m_prevContacted;

	// use building bounds
	bool m_useBuildingBounds;


	// spacing

	// uniform robot spacing
	double m_uniformRobotSpacing;

	// vector of spacing factors
	vector<double> m_spacingFactors;

	// spacing factor
	double m_spacingTol;

	// transition timing
	cPrecisionClock m_transitionTimer;

	// transition timeout
	double m_transitionTimeout;

	// transition check
	bool m_transition;

	// previous transition check
	bool m_prevTransition;

	// transition force
	cVector3d m_transitionForce;


	//-----------------------------------------------------------------------
	// PATROL MEMBERS:
	//-----------------------------------------------------------------------

	// Potential field rtsClay nodes that build the rtsClay potential field for travel
	vector<rtsClayNode*> m_travelSpheres;

	// spheres that contain robots for travel
	vector<rtsClayNode*> m_robotTravelSpheres;

	// rtsClay mesh in GEL world for travel
	rtsClayMesh* m_travelMesh;	

	// travel height
	double m_travelHeight;

	// travel mode
	bool m_travelMode;

	// reset travel mode
	bool m_resetTravel;

	// travel stiffness
	double m_travelStiffness;

	// convex hull robot positions vector
	vector<cVector3d> m_convexHullRobotPositions;

	// mapping of convex hull robot positions indices to robot positions indices
	vector<int> m_convexHullMapping;

	// previous convex hull mapping
	vector<int> m_prevConvexHullMapping;

	// adjusted previous convex hull mapping (size is same as current,
	// to account for lost or added agents along convex hull)
	vector<int> m_adjPrevConvexHullMapping;

	// use travel links
	bool m_useTravelLinks;

	// travel node color
	cColorf m_travelNodeColor;

	// reset travel counter
	int m_resetTravelCounter;

	//-----------------------------------------------------------------------
	// CORDON MEMBERS:
	//-----------------------------------------------------------------------

	// Potential field rtsClay nodes that build the rtsClay potential field for surround
	vector<rtsClayNode*> m_surroundSpheres;

	// spheres that contain robots for surround
	vector<rtsClayNode*> m_robotSurroundSpheres;

	// rtsClay mesh in GEL world for surround
	rtsClayMesh* m_surroundMesh;

	// rtsClay mesh in GEL world for static surround when resizing
	rtsClayMesh* m_surroundStaticResizeMesh;
	
	// surround modem
	bool m_surroundMode;

	// surround height
	double m_surroundHeight;

	// playdoh perimeter (measured from node to node)
	double m_perimeter;

	// large perimeter value
	double m_largePerimeter;

	// static friction radius
	// (radius around haptic proxy where friction is enabled)
	double m_staticFrictionR;

	// gemoetry perimeter
	double m_geomPerimeter;

	// surround node color
	cColorf m_surroundNodeColor;

	// surround node stiffness default
	double m_surroundNodeStiffness;

	// resize flag
	bool m_resizeSurround;

	// resize start flag
	bool m_resizeStart;

	// resize percentage
	double m_resizePercentage;

	cPrecisionClock m_spacingClock;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
