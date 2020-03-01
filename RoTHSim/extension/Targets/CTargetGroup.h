#pragma once

#include "chai3d.h"
#include <algorithm>
#include <vector>
#include "CTarget.h"

using namespace chai3d;
using namespace std;

// target type is specific type of sphere object
class cTargetGroup: public cGenericObject
{
public:

	// Constructor
	cTargetGroup(int a_id);

	virtual ~cTargetGroup();

	// add target to group
	void addTarget(cTarget* a_target);

	void initTargetGroup();

	// get targets
	vector<cTarget*> getTargets() { return m_targets; }

	// get target
	cTarget* getTarget(int a_index) { return m_targets[a_index]; }

	// set found state
	void setFoundState(bool a_foundState);

	// get found state
	bool getFoundState() { return m_found; }

	// set individual target found states
	void setTargetFoundStates(bool a_foundState);

	// set color
	void setColor(cColorf a_color);

	// set show targets
	void setShowTargets(bool a_show) { m_showTargets = a_show; }

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
	void updateActiveState(double a_simTime, int a_currentBldgId);

	// check all found states of grouped targets
	void checkFoundStates(double a_time);

	// set found time
	void setFoundTime(double a_time) { m_foundTime = a_time; }

	// get found time
	double getFoundTime() { return m_foundTime; }

	// set found color of targets
	void setFoundColor(cColorf a_color) { m_foundColor = a_color; }

	// get found color of targets
	cColorf getFoundColor() { return m_foundColor; }

	// get group id
	int getGroupId() { return m_groupId; }

	// set bldg id
	void setBldgId(int a_bldgId) { m_bldgId = a_bldgId; }

	// get bldg id
	int getBldgId() { return m_bldgId; }

	// begin fade out
	void beginFadeOut();

	void fadeOut();

	// set difficulty level
	void setDifficulty(int a_difficulty) { m_difficulty = a_difficulty; }

	// get difficulty level
	int getDifficulty() { return m_difficulty; }

	// get number of targets
	int getNumTargets() { return m_targets.size(); }

private:

	// delete and point NULL all children
	void deallocate();

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// targets
	vector<cTarget*> m_targets;

	// group id
	int m_groupId;

	// bldg id
	int m_bldgId;

	// difficulty
	int m_difficulty;

	// state
	bool m_found;

	// found time
	double m_foundTime;

	// color of found targets
	cColorf m_foundColor;

	// partial found color
	cColorf m_partialFoundColor;

	// option to show finding state
	bool m_showFindingState;

	// color of initial targets
	cColorf m_initialColor;

	bool m_sphereActive;

	// start time (in seconds)
	double m_startTime;

	// end time (in seconds)
	double m_endTime;

	// active state
	bool m_active;

	// passed state (simulation time > end time)
	bool m_passed;

	// fade complete
	bool m_faded;

	// show targets flag
	bool m_showTargets;

	// fade out timer
	cPrecisionClock m_fadeTimer;

	// group found timer
	cPrecisionClock m_foundTimer;
};

