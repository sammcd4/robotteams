#pragma once

#include "chai3d.h"
#include <vector>
#include <string>

using namespace chai3d;
using namespace std;

class cData
{

public:
	// Constructor
	cData();

	// Destructor
	~cData();

	// set time
	void setTime(double a_time) { m_time = a_time; }

	// get time
	double getTime() { return m_time; }

	// set cursor position
	void setCursorPos(cVector3d a_cursorPos) { m_cursorPos = a_cursorPos; }

	// get cursor position
	cVector3d getCursorPos() { return m_cursorPos; }

	// set  haptic force
	void setHapticForce(cVector3d a_hapticForce) { m_hapticForce = a_hapticForce; }

	// get  haptic force
	cVector3d getHapticForce() { return m_hapticForce; }

	// set robot positions
	void setRobotPositions(vector<cVector3d> a_robotPositions) { m_robotPositions = a_robotPositions; }

	// get robot positions
	vector<cVector3d> getRobotPositions() { return m_robotPositions; }

	// set valid
	void setValid(bool a_valid) { m_valid = a_valid; }

	// get valid
	bool getValid() { return m_valid; }

private:
	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------
	
	// timestamp
	double m_time;

	// cursor position
	cVector3d m_cursorPos;

	// haptic force
	cVector3d m_hapticForce;

	// robot positions
	vector<cVector3d> m_robotPositions;

	// valid data
	bool m_valid;

};

