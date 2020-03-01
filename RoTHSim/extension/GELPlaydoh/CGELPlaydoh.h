
//---------------------------------------------------------------------------
#ifndef CGELPlaydohH
#define CGELPlaydohH
//---------------------------------------------------------------------------
//#include "chai3d.h"
#include "GELPlaydoh3D.h"
#include "CGELPlaydohStrip.h"
#include "cGELPlaydohSkeletonNode.h"
#include "rtsPolygonBounds.h"
#include <list>
#include <algorithm>
#include <mutex>
//#include "CVector3dExt.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    \file       cGELPlaydoh.h

    \brief 
    <b> Scenegraph </b> \n 
    Virtual GELPlaydoh potential field
*/
//===========================================================================

//===========================================================================
/*!
    \class      cGELPlaydoh
    \ingroup    scenegraph

    \brief      
    Implementation of a virtual GELPlaydoh field
*/
//===========================================================================
class cGELPlaydoh
{
  public:
    
	////-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	////-----------------------------------------------------------------------

	//! Constructor of cGELPlaydoh. (must call setConvexHull, setConvexHullMapping and initPlaydoh methods after instantiating)
	cGELPlaydoh(cWorld* a_world, vector<cVector3d> a_robotPositions, double a_toolRadius, double a_toolWorkspaceScaleFactor, double a_maxStiffness, bool a_useBuildingBounds = true);
    
	//! Destructor of cSphere.
	~cGELPlaydoh();

	////-----------------------------------------------------------------------
	// METHODS:
	////-----------------------------------------------------------------------
	
	// get parent world pointer
	cWorld* getWorld() { return m_world; }

	// get GEL world pointer
	cGELPlaydohWorld* getPlaydohWorld() { return m_defWorld; }

	// get list of robot spheres for patrol
	vector<cGELPlaydohSkeletonNode*> getRobotPatrolSpheres() { return m_robotPatrolSpheres; }

	// get list of robot spheres for cordon
	vector<cGELPlaydohSkeletonNode*> getRobotCordonSpheres() { return m_robotCordonSpheres; }

	// get robot positions
	vector<cVector3d> getRobotPositions() { return m_robotPositions; }

	// initialize playdoh to create patrol correctly
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

	// set GELPlaydoh stiffness
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

	// set mode (0 - patrol, 1 - cordon)
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
	
	// get list of potential spheres for patrol
	vector<cGELPlaydohSkeletonNode*> getPatrolSpheres() { return m_patrolSpheres; }
	
	// get playdoh mesh in GEL world for patrol
	cGELPlaydohMesh* getPatrolMesh() { return m_patrolMesh; }	
	
	// get number of spheres in GELPlaydoh for patrol
	int getNumPatrolSpheres();	
	
	// get patrol mode
	bool getPatrolMode() { return m_patrolMode; }

	// set patrol mode
	void setPatrolMode(bool a_patrolMode) { m_patrolMode = a_patrolMode; }

	// set use patrol links
	void setUsePatrolLinks(bool a_usePatrolLinks) {m_usePatrolLinks = a_usePatrolLinks;}

	// get use patrol links
	bool getUsePatrolLinks() { return m_usePatrolLinks;}

	// get reset patrol
	bool getResetPatrol() { return m_resetPatrol; }

	//-----------------------------------------------------------------------
	// CORDON METHODS:
	//-----------------------------------------------------------------------

	// get list of potential spheres for cordon
	vector<cGELPlaydohSkeletonNode*> getCordonSpheres() { return m_cordonSpheres; }

	// get playdoh mesh in GEL world for cordon
	cGELPlaydohMesh* getCordonMesh() { return m_cordonMesh; }

	// get number of spheres in GELPlaydoh for cordon
	int getNumCordonSpheres();

	// get cordon mode
	bool getCordonMode() { return m_cordonMode; }

	// set cordon mode
	void setCordonMode(bool a_cordonMode) { m_cordonMode = a_cordonMode; }

	// get perimeter
	double getPerimeter() { return m_perimeter; }

	// get large perimeter value (2 X geometry perimeter)
	double getLargePerimeter() { return m_largePerimeter; }

	// get static friction radius
	double getStaticFrictionRadius() { return m_staticFrictionR; }

	// get geometry perimeter
	double getGeomPerimeter() { return m_geomPerimeter; }

	// set resize cordon
	void setEnableResizeCordon(bool a_resizeCordon);

	// get resize cordon
	bool getEnableResizeCordon() { return m_resizeCordon; }

	// set resize percentage
	void setResizePercentage(double a_resizePercentage);

	// get resize percentage
	double getResizePercentage() { return m_resizePercentage; }

	// compute virtual polygon
	void computeVirtualPolygon();

	// get virtual polygon during cordon movement
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
	
	// initialize playdoh sphere parameters for patrol
	void initPatrolParams();

	// initialize playdoh spheres for patrol
	void initPatrol();

	// initialize playdoh spheres for patrol
	void initPatrol(bool a_show);

	// delete playdoh spheres for patrol
	void deletePatrol();

	//-----------------------------------------------------------------------
	// CORDON METHODS:
	//-----------------------------------------------------------------------
	
	// initialize playdoh sphere parameters for cordon
	void initCordonParams();

	// initialize playdoh spheres for cordon
	void initCordon();

	// delete playdoh spheres for cordon
	void deleteCordon();

	// initialize playdoh perimeter
	void initPerimeter();

	// resize cordon
	void resizeCordon();

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
	cGELPlaydohWorld* m_defWorld;

	// robot positions vector
	vector<cVector3d> m_robotPositions;

	// spheres that contain robots
	list<cGELPlaydohSkeletonNode*> m_robotSpheres;

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

	// Potential field GELPlaydoh nodes that build the GELPlaydoh potential field for patrol
	vector<cGELPlaydohSkeletonNode*> m_patrolSpheres;

	// spheres that contain robots for patrol
	vector<cGELPlaydohSkeletonNode*> m_robotPatrolSpheres;

	// GELPlaydoh mesh in GEL world for patrol
	cGELPlaydohMesh* m_patrolMesh;	

	// patrol height
	double m_patrolHeight;

	// patrol mode
	bool m_patrolMode;

	// reset patrol mode
	bool m_resetPatrol;

	// patrol stiffness
	double m_patrolStiffness;

	// convex hull robot positions vector
	vector<cVector3d> m_convexHullRobotPositions;

	// mapping of convex hull robot positions indices to robot positions indices
	vector<int> m_convexHullMapping;

	// previous convex hull mapping
	vector<int> m_prevConvexHullMapping;

	// adjusted previous convex hull mapping (size is same as current,
	// to account for lost or added agents along convex hull)
	vector<int> m_adjPrevConvexHullMapping;

	// use patrol links
	bool m_usePatrolLinks;

	// patrol node color
	cColorf m_patrolNodeColor;

	// reset patrol counter
	int m_resetPatrolCounter;

	//-----------------------------------------------------------------------
	// CORDON MEMBERS:
	//-----------------------------------------------------------------------

	// Potential field GELPlaydoh nodes that build the GELPlaydoh potential field for cordon
	vector<cGELPlaydohSkeletonNode*> m_cordonSpheres;

	// spheres that contain robots for cordon
	vector<cGELPlaydohSkeletonNode*> m_robotCordonSpheres;

	// GELPlaydoh mesh in GEL world for cordon
	cGELPlaydohMesh* m_cordonMesh;

	// GELPlaydoh mesh in GEL world for static cordon when resizing
	cGELPlaydohMesh* m_cordonStaticResizeMesh;
	
	// cordon modem
	bool m_cordonMode;

	// cordon height
	double m_cordonHeight;

	// playdoh perimeter (measured from node to node)
	double m_perimeter;

	// large perimeter value
	double m_largePerimeter;

	// static friction radius
	// (radius around haptic proxy where friction is enabled)
	double m_staticFrictionR;

	// gemoetry perimeter
	double m_geomPerimeter;

	// cordon node color
	cColorf m_cordonNodeColor;

	// cordon node stiffness default
	double m_cordonNodeStiffness;

	// resize flag
	bool m_resizeCordon;

	// resize start flag
	bool m_resizeStart;

	// resize percentage
	double m_resizePercentage;

	cPrecisionClock m_spacingClock;
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
