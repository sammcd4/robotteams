#include "rtsDetections.h"
//------------------------------------------------------------------------------

//==============================================================================
/*!
    Constructor of rtsDetections.
*/
//==============================================================================
rtsDetections::rtsDetections(int a_numAgents, double a_radius, double a_agentRadius, bool a_showAgent, cColorf a_color)
{
	m_numAgents = a_numAgents;
	m_radius = a_radius;
	m_agentRadius = a_agentRadius;
	m_color = a_color;
	m_showAgent = a_showAgent;
	m_worldToPixel = 1.0;
	m_maxTransparencyLevel = 0.5;
	
	for (int i = 0; i < m_numAgents; i++)
	{
		cShapeSphere* light = new cShapeSphere(a_radius);
		light->setUseTransparency(true);
		light->setTransparencyLevel(0.0);
		light->m_material->m_ambient = a_color;
		light->m_material->m_emission = cColorf(0, 0, 0);
		light->m_material->m_specular = cColorf(0, 0, 0);
		light->m_material->m_diffuse = cColorf(0, 0, 0);

		// add agent radius
		cShapeSphere* agent = new cShapeSphere(a_agentRadius);
		agent->m_material->m_ambient = a_color;
		agent->setShowEnabled(false);
		agent->setLocalPos(0, 0, -m_agentRadius);

		addChild(light);
		m_lights.push_back(light);
		addChild(agent);
		m_agents.push_back(agent);

		cPrecisionClock clock;
		m_fadeTimers.push_back(clock);
	}
}

rtsDetections::~rtsDetections()
{
	deallocate();
}

void rtsDetections::deallocate()
{
	// lights
	int numLights = m_lights.size();
	for (int i = 0; i < numLights; i++)
	{
		cShapeSphere* light = m_lights[i];
		removeChild(light);
		delete light;
		light = NULL;
	}
	m_lights.clear();

	// agents
	int numAgents = m_agents.size();
	for (int i = 0; i < numAgents; i++)
	{
		cShapeSphere* agent = m_agents[i];
		removeChild(agent);
		delete agent;
		agent = NULL;
	}
	m_agents.clear();
}

void rtsDetections::setColor(cColorf a_color)
{
	m_color = a_color;
	for (int i = 0; i < m_numAgents; i++)
	{
		m_lights[i]->m_material->m_ambient = a_color;
		m_agents[i]->m_material->m_ambient = a_color;
	}
}

cColorf rtsDetections::getColor(int a_sectionNum)
{
	return m_color;
}

void rtsDetections::setShowLights(bool a_show)
{
	for (int i = 0; i < m_numAgents; i++)
	{
		m_lights[i]->setShowEnabled(a_show);
		if (m_showAgent)
			m_agents[i]->setShowEnabled(a_show);
	}
}

void rtsDetections::update(bool a_sampleSignal, vector<bool> a_sensorReadings, vector<cVector3d> a_sensorPositions)
{
	//bool sampleSignal = false;
	//if (!m_sampleTimer.on())
	//{
	//	m_sampleTimer.reset();
	//	m_sampleTimer.start();
	//	m_sampleTimer.setTimeoutPeriodSeconds(0.25);
	//}

	//if (m_sampleTimer.timeoutOccurred())
	//{
	//	sampleSignal = true;
	//	m_sampleTimer.stop();
	//}

	if (!m_showAgent)
	{
		for (int i = 0; i < m_numAgents; i++)
		{
			m_agents[i]->setShowEnabled(false);
		}
	}

	if (a_sensorReadings.size() == m_numAgents && a_sensorPositions.size() == m_numAgents)
	{
		for (int i = 0; i < m_numAgents; i++)
		{
			updateTransparency(true, i, a_sensorReadings[i], a_sensorPositions[i]);
		}
	}
}

void rtsDetections::updateTransparency(bool a_sampleSignal, int a_agentNum, bool a_sensorReading, cVector3d a_pos)
{
	if (a_sensorReading && a_sampleSignal)
	{
		// reset fade to baseline timer (even before timeout)
		m_fadeTimers[a_agentNum].stop();
		m_fadeTimers[a_agentNum].reset();

		// start fade to baseline
		m_fadeTimers[a_agentNum].setTimeoutPeriodSeconds(0.50);
		m_fadeTimers[a_agentNum].start();
	}
	else if (m_fadeTimers[a_agentNum].on())
	{
		double timeout = m_fadeTimers[a_agentNum].getTimeoutPeriodSeconds();
		double time = m_fadeTimers[a_agentNum].getCurrentTimeSeconds();

		double level = m_maxTransparencyLevel - time / timeout * m_maxTransparencyLevel;
		m_lights[a_agentNum]->setTransparencyLevel(level);
		m_lights[a_agentNum]->setLocalPos(m_worldToPixel*a_pos - m_buildingPos);
		m_agents[a_agentNum]->setLocalPos(m_worldToPixel*a_pos - m_buildingPos - cVector3d(0,0,-m_agentRadius));
		if (m_showAgent)
		{
			if (!m_agents[a_agentNum]->getShowEnabled())
			{
				m_agents[a_agentNum]->setShowEnabled(true);
			}
		}

		if (m_fadeTimers[a_agentNum].timeoutOccurred())
		{
			m_fadeTimers[a_agentNum].stop();
			m_fadeTimers[a_agentNum].reset();
			m_lights[a_agentNum]->setTransparencyLevel(0.0);
			m_agents[a_agentNum]->setShowEnabled(false);
		}
	}

}

void rtsDetections::zeroPositions()
{
	for (int i = 0; i < m_numAgents; i++)
	{
		m_lights[i]->setLocalPos(0, 0, 0);
		m_agents[i]->setLocalPos(0, 0, -m_agentRadius);

		// also hide all agents (not waiting until timer runs out)
		m_agents[i]->setShowEnabled(false);

		// reset timers
		m_fadeTimers[i].stop();
		m_fadeTimers[i].reset();
	}
}