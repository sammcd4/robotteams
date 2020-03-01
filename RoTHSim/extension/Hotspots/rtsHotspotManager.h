#pragma once

#include "chai3d.h"
#include <list>
#include <algorithm>
#include <vector>
#include "rtsHotspotGroup.h"
#include <fstream>

using namespace chai3d;
using namespace std;

class rtsHotspotManager
{
public:
	// Constructor
	rtsHotspotManager(cGenericObject* a_parent);

	// Destructor
	~rtsHotspotManager();

	// add target group
	void addTargetGroup(double a_radius, int a_trueTarget, int a_bldgId, double a_startTime, double a_endTime, vector<cVector3d> a_targetPos, int a_numTargets, int a_groupId);

	// set target groups
	void setTargetGroups(vector<rtsHotspotGroup*> a_targetGroups);

	// get target list size
	int getNumTargetGroups() { return m_targetGroups.size(); }

	// set initial color of targets
	void setInitialColor(cColorf a_color) { m_initialColor = a_color; }

	// get initial color of targest
	cColorf getInitialColor() { return m_initialColor; }

	// set found color of targets
	void setFoundColor(cColorf a_color) { m_foundColor = a_color; }

	// get found color of targets
	cColorf getFoundColor() { return m_foundColor; }

	// set found radius
	void setFoundRadius(double a_foundRadius) { m_foundRadius = a_foundRadius; }

	// get found radius
	double getFoundRadius() { return m_foundRadius; }

	// check for found targets
	void updateTargetStates(vector<cVector3d> a_agentPositions);

	// get active target positions
	vector<cVector3d> getActiveTargetPositions();

	// get active target groups
	vector<rtsHotspotGroup*> getActiveTargetGroups();

	// get number of active targets
	int getNumActiveTargets() { return m_numActiveTargets; }
	
	// update active target states
	void updateActiveTargetStates(double a_simTime, int a_currentBldgId);

	// write target found data to file
	bool writeTargetFoundData(string a_filename);

	void setShowTargets(bool a_show);

private:

	// deallocate
	void deallocate();

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// target world
	cGenericObject* m_parent;

	// vector of target groups
	vector<rtsHotspotGroup*> m_targetGroups;

	// number of active targets
	int m_numActiveTargets;

	// vector of unfound target indices in m_targets
	vector<int> m_unfoundTargetMap;

	// color of initial targets
	cColorf m_initialColor;

	// color of found targets
	cColorf m_foundColor;

	// found radius
	double m_foundRadius;

	bool m_showTargets;


};

