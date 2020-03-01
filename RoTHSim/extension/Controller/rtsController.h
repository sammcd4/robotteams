#pragma once

#include "Connection/comm.h"
#include "Connection/command.pb.h"
#include "Connection/swarm.pb.h"
#include "chai3d.h"

using namespace chai3d;
using namespace std;

#include <list>
#include <algorithm>
#include <mutex>

class rtsController
{

public:
	// Constructor
	rtsController(int a_numAgents, cWorld* a_world, communicator* a_com, double a_kp, double a_kd, double a_workspaceScaleFactor);

	// Destructor
	~rtsController();

	// add force to agent
	void setForce(int a_id, double a_x, double a_y, double a_z);

	// reset force on individual agent
	void resetForce(int a_id);

	// reset all forces on agents
	void resetForces();

	// update forces on agents (return true if minimal error)
	bool updateForces(vector<cVector3d> a_desiredPositions, vector<cVector3d> a_currentPositions);

	// get number of desired wells
	int getNumAgents() { return m_numAgents; }

	// set color of wells
	void setColor(cColorf a_color) { m_color = a_color; }

	// set show enabled for all gravity wells
	void setShowEnabled(bool a_show);

	// get show enabled for all gravity wells
	bool getShowEnabled() { return m_show; }

	// toggle show enabled
	void toggleShowEnabled();

	// get color of wells
	cColorf getColor() { return m_color; }

	// zero line lengths
	void zeroLines();

	// set mutex
	void setMutex(std::mutex* a_mtx) { m_mtx = a_mtx; }

	// set sample rate (in seconds)
	void setSampleRate(double a_sampleRate){ m_sampleRate = a_sampleRate; }

	// get sample rate (in seconds)
	double getSampleRate() { return m_sampleRate; }

	// set add to world status
	void setAddToWorld(bool a_addToWorld);

	// get add to world status
	bool getAddToWorld() { return m_addToWorld; }


private:

	// deallocate all children
	void deallocate();

private:



	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// number of agents
	int m_numAgents;

	// communicator
	communicator* m_com;

	// priority counter
	int m_priority;

	// gravity well world
	cWorld* m_world;

	// proportional gain (positive value)
	double m_kp;

	// derivative gain (negative value)
	double m_kd;

	// previous positions
	vector<cVector3d> m_prevPositions;

	// previous forces
	vector<cVector3d> m_prevForces;

	// lines to display forces in world
	vector<cShapeLine*> m_forceLines;

	// choose to add the well to world
	bool m_addToWorld;

	// color of initial wells
	cColorf m_color;

	// transparency level
	double m_transparencyLevel;

	// show enabled
	bool m_show;

	// mutex
	std::mutex* m_mtx;

	// sample timer
	cPrecisionClock m_timer;

	// sample rate (in seconds)
	double m_sampleRate;

	// minimal error threshold
	double m_minimalError;
};

