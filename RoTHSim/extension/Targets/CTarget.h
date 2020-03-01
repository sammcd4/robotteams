#pragma once

#include "chai3d.h"
#include <algorithm>
#include <vector>

using namespace chai3d;
using namespace std;

// target type is specific type of sphere object
class cTarget: public cGenericObject
{
public:

	// Constructor
	cTarget();

	// Constructor for adding a sphere child
	cTarget(const double& a_radius);

	// Constructor for adding a sphere child, plus start and endTimes
	cTarget(const double& a_radius, double a_startTime, double a_endTime);

	virtual ~cTarget();

	// initialize target
	void initTarget();

	// set bldg id number
	void setBldgId(int a_bldgId) { m_bldgId = a_bldgId; }

	// get bldg id number
	int getBldgId() { return m_bldgId; }

	// set found state
	void setFoundState(bool a_foundState);

	// get found state
	bool getFoundState() { return m_found; }

	// set group found state
	void setGroupFoundState(bool a_groupFoundState);

	// get group found state
	bool getGroupFoundState() { return m_groupFound; }

	// update found counter
	void updateFoundCounter(bool a_detection);

	// check semi found state
	bool isSemiFound();

	// check if any detections have occurred
	bool isDetected();

	// reset detection counter
	void resetDetectionCounter();

	// set color
	void setColor(cColorf a_color, bool a_show3D = false);

	// set active state
	void setActiveState(bool a_active);

	// get active state
	bool getActiveState() { return m_active; }

	// set start time (in seconds)
	void setStartTime(double a_startTime);

	// get start time (in seconds)
	double getStartTime() { return m_startTime; }

	// set end time (in seconds)
	void setEndTime(double a_endTime);

	// get end time (in seconds)
	double getEndTime() { return m_endTime; }

	// set start and end times (in seconds)
	void setStartAndEndTimes(double a_startTime, double a_endTime);

	// update active state
	void updateActiveState(double a_simTime);
    
    // set group id
    void setGroupId(int a_groupId) { m_groupId = a_groupId; }
    
    // get group id
    int getGroupId() { return m_groupId; }

	// set difficulty level
	void setDifficulty(int a_difficulty) { m_difficulty = a_difficulty; }

	// get difficulty level
	int getDifficulty() { return m_difficulty; }

	// get true local pos
	cVector3d getTrueLocalPos() { return m_trueLocalPos; }

	// set true local pos
	void setTrueLocalPos(cVector3d a_localPos) { m_trueLocalPos = a_localPos; }

private:

	// delete and point NULL a child object
	bool deleteAndNullChild(cGenericObject* a_object);

	// delete and point NULL all children
	void deallocate();

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// sphere
	//cShapeSphere* m_visual;
	cShapeBox* m_visual;

	// true local position
	cVector3d m_trueLocalPos;

	// id for building
	int m_bldgId;
    
    // group id
    int m_groupId;

	// group difficulty
	int m_difficulty;

	// sphere active
	bool m_visualActive;

	// state
	bool m_found;

	// group found state
	bool m_groupFound;

	// start time (in seconds)
	double m_startTime;

	// end time (in seconds)
	double m_endTime;

	// active state
	bool m_active;

	// passed state (simulation time > end time)
	bool m_passed;

	int m_detectionCounter;
	int m_maxDetectionCount;

	int m_minDetectionToFound;
	int m_resetDetectionCounter;

};

