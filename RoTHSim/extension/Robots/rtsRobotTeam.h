#ifndef CGENERICROBOTTEAM_H
#define CGENERICROBOTTEAM_H
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsRobot.h"
#include <vector>
#include "Other/rtsVector3d.h"
#include "Hotspots\rtsHotspotGroup.h"
//---------------------------------------------------------------------------

using namespace chai3d;
using namespace std;

class rtsRobotTeam
{

public:

	// constructor of rtsRobotTeam
    rtsRobotTeam(cWorld* a_world)
	{
		// set world
		m_world = a_world;

		// Increment robot team count
		numRobotTeams++;

		// Assign id number
		idNumber = numRobotTeams;

		// set default mode
		m_defaultMode = -1;
		//setMode(m_defaultMode);

		// set surround color
		m_surroundColor = cColorf(0.0, 0.0, 0.5);

		// set travel color
		m_travelColor = cColorf(0.5, 0.0, 0.0);

		// show convex hull
		m_showConvexHull = false;

		// show robot team
		m_showEnabled = true;

		// initialize previous convex hull size
		m_prevConvexHullSize = 0;

		// initialize mode
		m_travelMode = true;
		m_surroundMode = false;
	}

    // destructor of rtsRobotTeam
    virtual ~rtsRobotTeam();

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

    // get id number of robot team
    int getIdNumber();

	// get total number of robot teams
    int getNumRobotTeams();

	// add a robot to the team
	void addRobot(rtsRobot* a_robot);

	// get robot team
	vector<rtsRobot*> getRobotTeam() { return m_team; }

	// get a robot in the team
	rtsRobot* getRobot(int a_robotIndex) { return m_team[a_robotIndex]; }

	// set all robot positions ( if order of ids are correct )
	void setRobotPositions(vector<cVector3d> a_robotPositions);

	// set all robot positions, given random positions and their ids
	void setRobotPositions(vector<cVector3d> a_robotPositions, vector<int> a_ids);

	// get all robot positions
	vector<cVector3d> getRobotPositions() { return m_robotPositions; }

	// set all robot colors to a specific color
	void setRobotColors(cColorf a_color);

	// set all robot sensor colors
	void setSensorColors(cColorf a_color);

	// set robots to travel colors
	void setModeColors();

	// set all robot radii
	void setRadii(double a_radius);

	// compute the convex hull of robot positions (Andrew's monotone chain algorithm)
	void computeConvexHull_Andrews(vector<cVector3d> a_robotPositions);

	// compute the convex hull of robot positions (Gift Wrapping)
	void computeConvexHull_GiftWrap(vector<cVector3d> a_robotPositions);

	// get convex hull positions
	vector<cVector3d> getConvexHull() { return m_convexHullRobotPositions; }

	// get convex hull mapping
	vector<int> getConvexHullMapping() { return m_convexHullMapping; }

	// set mode (0 - travel, 1 - surround, 2 - travel and surround)
	void setDistributionMode(int a_mode);

	// create lines of convex hull
	void createConvexHullLines();

	// update lines of convex hull
	void updateConvexHullLines();

	// set convex hull line color
	void setConvexHullLineColor(cColorf a_color);

	// delete lines of convex hull
	void deleteConvexHullLines();

	// create surround lines
	void createSurroundLines();

	// show surround lines
	void setShowSurroundLines(bool a_show);

	// show convex hull path
	void setShowConvexHull(bool a_show);

	// get show convex Hull
	bool getShowConvexHull() { return m_showConvexHull; }

	// SHOW TEAM

	// set show robot team
	void setShowEnabled(bool a_showEnabled, bool a_affectChildren = true);

	// get show robot team
	bool getShowEnabled() { return m_showEnabled; }

	// toggle show enabled
	void toggleShowEnabled(bool a_affectChildren = true);

	// SHOW ID

	// set show id
	void setShowId(bool a_showId);

	// get show id
	bool getShowId(){ return m_showId; }

	// toggle show id
	void toggleShowId();

	// SENSING
	vector<double> updateSensorReadings(vector<rtsHotspotGroup*> a_activeHotspotGroups, int a_numActiveTargets, vector<double> a_nsewBuildingBoundaries, double a_time, bool a_surround);

	// get sensor readings
	vector<bool> getSensorReadings();

	// clear sensor readings
	void clearSensorReadings();

	// get team size
	int size() { return m_team.size(); }

	// delete and NULL all robots in team
	void deleteAndNullAllRobots();

	// remove robots from parent world's list of children
	void removeRobotsFromChildren();

	void setSensorLightTransparencyLevels(double a_level);

protected:

	// reset all modes and set mode
	void setMode(int a_mode);

	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

	// parent world
	cWorld* m_world;

	// robot team id number
	int idNumber;

	// total number of robot teams
	static int numRobotTeams;

	// robot team
	vector<rtsRobot*> m_team;

	// robot positions
	vector<cVector3d> m_robotPositions;

	// convex hull robot positions vector
	vector<cVector3d> m_convexHullRobotPositions;

	// mapping of convex hull robot positions indices to robot positions indices
	vector<int> m_convexHullMapping;

	// show convex hull
	bool m_showConvexHull;

	// previous convex hull size
	int m_prevConvexHullSize;

	// lines representing convex hull
	vector<cShapeLine*> m_convexHullLines;

	// lines representing the inter-agent edges
	vector<cShapeLine*> m_surroundLines;

	// travel mode
	bool m_travelMode;

	// surround mode
	bool m_surroundMode;

	// show robot team
	bool m_showEnabled;

	// show id
	bool m_showId;

	// distribution mode
	int m_mode;

	// default mode
	int m_defaultMode;

	// standby mode
	bool m_standbyMode;

	// surround color
	cColorf m_surroundColor;

	// travel color
	cColorf m_travelColor;
};

#endif // CGENERICROBOTTEAM_H