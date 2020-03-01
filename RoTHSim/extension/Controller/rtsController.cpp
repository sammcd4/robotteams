#include "rtsController.h"

rtsController::rtsController(int a_numAgents, cWorld* a_world, communicator* a_com, double a_kp, double a_kd, double a_workspaceScaleFactor)
{
	// set number of agents
	m_numAgents = a_numAgents;

	// set communicator
	m_com = a_com;

	// priority
	m_priority = 0;

	// set world for gravity wells
	m_world = a_world;

	// set proportional gain
	m_kp = a_kp;

	// set derivate gain
	m_kd = a_kd;

	// set default add to world status
	m_addToWorld = true;

	// set transparency level
	m_transparencyLevel = 0.25;

	// set default show
	m_show = false;

	// line construction
	for (int i = 0; i < m_numAgents; i++)
	{
		cShapeLine* forceLine = new cShapeLine(cVector3d(0, 0, 0), cVector3d(0, 0, 0));
		forceLine->setShowEnabled(m_show);
		if (m_addToWorld)
		{
			m_world->addChild(forceLine);
		}
		m_forceLines.push_back(forceLine);
		
	}

	// set default color
	m_color = cColorf(0.1f, 0.1f, 0.1f, (float)(m_transparencyLevel));

	// reset timer
	m_timer.reset();
	m_timer.stop();

	// default timout sample rate (run as fast as possible)
	m_sampleRate = 0.0;

	// set default minimal error threshold
	m_minimalError = 0.005* a_workspaceScaleFactor;
}

rtsController::~rtsController()
{
	deallocate();
}

void rtsController::deallocate()
{
	// force lines
	int numLines = m_forceLines.size();
	for (int i = 0; i < numLines; i++)
	{
		m_world->removeChild(m_forceLines[i]);
		delete m_forceLines[i];
		m_forceLines[i] = NULL;
	}
	m_forceLines.clear();
}

// add force to agent
void rtsController::setForce(int a_id, double a_x, double a_y, double a_z)
{
	m_com->set_force(m_priority, a_id, a_x, a_y, a_z);
	m_priority++;
}

// reset force on individual agent
void rtsController::resetForce(int a_id)
{
	setForce(a_id, 0.0, 0.0, 0.0);
}

// reset all forces on agents
void rtsController::resetForces()
{
	//cout << "resetForces " << m_numAgents << endl;

	for (int i = 0; i < m_numAgents; i++)
	{
		resetForce(i);
	}
}

// update forces on agents
bool rtsController::updateForces(vector<cVector3d> a_desiredPositions, vector<cVector3d> a_currentPositions)
{
	if (!m_timer.on())
	{
		m_timer.reset();
		m_timer.start();
	}

	double totalError = 100;

	if (m_timer.timeoutOccurred() && (a_desiredPositions.size() == a_currentPositions.size()))
	{
		//cout << "updateForces timeout" << endl;

		// get the current time
		m_timer.stop();
		double time = m_timer.getCurrentTimeSeconds();

		// current forces
		vector<cVector3d> currentForces;
		totalError = 0;
		// calculate and apply forces
		for (int i = 0; i < m_numAgents; i++)
		{
			cVector3d force(0,0,0);

			// error in position
			cVector3d errorPos(a_desiredPositions[i] - a_currentPositions[i]);

			// numerical derivative
			if (m_prevPositions.empty())
			{
				m_prevPositions = a_currentPositions;
			}
			cVector3d diffPos(a_currentPositions[i] - m_prevPositions[i]);
			diffPos.div(time);

			// calculate proportional force
			force.add(m_kp * errorPos);

			// calculate derivative force
			force.add(m_kd * diffPos);

			// add to force vector
			currentForces.push_back(force);

			// update force line
			m_forceLines[i]->m_pointA = a_currentPositions[i];
			m_forceLines[i]->m_pointB = a_currentPositions[i] + errorPos;

			// collect error for this agent
			totalError += errorPos.length();
		}

		// store previous positions
		m_prevPositions = a_currentPositions;

		// store previous forces
		m_prevForces = currentForces;

		// apply forces to server
		for (int i = 0; i < m_numAgents; i++)
		{
			setForce(i, currentForces[i].x(), currentForces[i].y(), currentForces[i].z());
		}

		// restart timer
		m_timer.reset();
		m_timer.start();
	}

	return (totalError < m_minimalError);
}

void rtsController::toggleShowEnabled()
{
	setShowEnabled(!m_show);
}

void rtsController::zeroLines()
{
	int numLines = m_forceLines.size();
	for (int i = 0; i < numLines; i++)
	{
		m_forceLines[i]->m_pointA.set(0, 0, 0);
		m_forceLines[i]->m_pointB.set(0, 0, 0);
	}
}

void rtsController::setShowEnabled(bool a_show)
{
	// apply changes to world children only if a change in show
	if (a_show != m_show)
	{
		// remove lines from world
		int numLines = m_forceLines.size();
		for (int i = 0; i < numLines; i++)
		{
			m_forceLines[i]->setShowEnabled(a_show);
		}
	}

	// set member variable
	m_show = a_show;
}

void rtsController::setAddToWorld(bool a_addToWorld)
{ 
	// apply changes to world children
	if (m_addToWorld && !a_addToWorld)
	{
		// remove lines from world	
		int numLines = m_forceLines.size();
		for (int i = 0; i < numLines; i++)
		{
			m_world->removeChild(m_forceLines[i]);
		}
	}
	else if (!m_addToWorld && a_addToWorld)
	{
		// add lines to world
		int numLines = m_forceLines.size();
		for (int i = 0; i < numLines; i++)
		{
			m_world->addChild(m_forceLines[i]);
		}
	}

	// set member variable
	m_addToWorld = a_addToWorld; 
}