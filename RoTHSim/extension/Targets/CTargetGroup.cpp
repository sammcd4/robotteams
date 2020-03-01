#include "cTargetGroup.h"

cTargetGroup::cTargetGroup(int a_id)
{
	m_groupId = a_id;
	initTargetGroup();
}

cTargetGroup::~cTargetGroup()
{
	deallocate();
}

void cTargetGroup::addTarget(cTarget* a_target)
{
	a_target->setColor(m_initialColor);
	a_target->setGroupId(m_groupId);

	m_targets.push_back(a_target);
	addChild(a_target);
}

void cTargetGroup::initTargetGroup()
{
	// set default found state
	m_found = false;

	// set active state
	m_active = false;

	// default sphere activity
	m_sphereActive = false;

	// default start time
	m_startTime = C_LARGE;

	// default end time
	m_endTime = C_LARGE;

	// default passed state
	m_passed = false;

	// default faded state
	m_faded = false;

	m_foundColor = cColorf(0.0f, 1.0f, 0.0f);
	m_initialColor = cColorf(1.0f, 0.0f, 0.0f);
	m_partialFoundColor = cColorf(1.0f, 0.0f, 1.0f);
	m_showFindingState = true;

	// choose to show hidden targets
	m_showTargets = false;

	// set timout period for all targets to be found
	m_foundTimer.setTimeoutPeriodSeconds(2.0);
}

void cTargetGroup::setFoundState(bool a_foundState)
{ 
	int numTargets = m_targets.size();
	for (int i = 0; i < numTargets; i++)
	{
		m_targets[i]->setFoundState(a_foundState);
	}
}

void cTargetGroup::setTargetFoundStates(bool a_foundState)
{
	int numTargets = m_targets.size();
	for (int i = 0; i < numTargets; i++)
	{
		m_targets[i]->setFoundState(a_foundState);
	}
}

void cTargetGroup::setColor(cColorf a_color)
{
	int numTargets = m_targets.size();
	for (int i = 0; i < numTargets; i++)
	{
		m_targets[i]->setColor(a_color, false);
	}
}

void cTargetGroup::deallocate()
{
	// delete all children
	vector<cGenericObject*>::iterator it;
	for (it = m_children.begin(); it < m_children.end(); it++)
	{
		cGenericObject* nextObject = (*it);
		delete (nextObject);
		nextObject = NULL;
	}

	// clear my list of children
	m_children.clear();
}

void cTargetGroup::setActiveState(bool a_active)
{
	m_active = a_active;
}

void cTargetGroup::setStartTime(double a_startTime)
{
	m_startTime = a_startTime;
}

void cTargetGroup::setEndTime(double a_endTime)
{
	m_endTime = a_endTime;
}

void cTargetGroup::setStartAndEndTimes(double a_startTime, double a_endTime)
{
	m_startTime = a_startTime;
	m_endTime = a_endTime;
}

void cTargetGroup::updateActiveState(double a_simTime, int a_currentBldgId)
{
	if (m_faded)
	{
		// do nothing
	}
	else if (m_passed)
	{
		fadeOut();
	}
	// check for correct bldg
	else if (a_currentBldgId != m_bldgId)
	{
		// hide all targets if an incorrect bldg
		int numTargets = m_targets.size();
		for (int i = 0; i < numTargets; i++)
		{
			m_targets[i]->setShowEnabled(false);
		}

		if (a_simTime < m_endTime)
			m_active = false;
	}
	else if (!m_active && a_simTime > m_startTime)
	{
		m_active = true;
		if (m_showTargets)
		{
			int numTargets = m_targets.size();
			for (int i = 0; i < numTargets; i++)
			{
				m_targets[i]->setShowEnabled(true);
			}
		}
	}
	else if (m_active && a_simTime > m_endTime)
	{
		m_active = false;
		int numTargets = m_targets.size();
		for (int i = 0; i < numTargets; i++)
		{
			m_targets[i]->setShowEnabled(true);
		}
		beginFadeOut();
		m_passed = true;
	}
}

void cTargetGroup::checkFoundStates(double a_time)
{
	// if all targets are found, set group to found
	if (!m_found)
	{
		int numTargets = m_targets.size();
		if (m_foundTimer.on())
		{
			bool allFound = true;
			for (int i = 0; i < numTargets; i++)
			{
				if (!m_targets[i]->isSemiFound())
				{
					allFound = false;
					break;
				}
			}
			m_found = allFound;

			if (m_foundTimer.timeoutOccurred())
			{
				m_foundTimer.stop();
				for (int i = 0; i < numTargets; i++)
				{
					m_targets[i]->resetDetectionCounter();
				}
			}
		}
		else
		{
			// check if any of the targets have a true detection
			bool startTimer = false;
			for (int i = 0; i < numTargets; i++)
			{
				if (m_targets[i]->isDetected())
				{
					startTimer = true;
					break;
				}
			}
			if (startTimer)
			{
				m_foundTimer.reset();
				m_foundTimer.start();
			}
		}

		if (m_found)
		{
			setColor(m_foundColor);

			// set all group found variables to true for each target
			for (int i = 0; i < numTargets; i++)
			{
				m_targets[i]->setGroupFoundState(true);
				m_targets[i]->setShowEnabled(true);
			}
			setFoundTime(a_time);
			setFoundState(m_found);
			cout << "New Target Group Found: " << m_groupId << " " << a_time << endl;
		}
		else
		{
			if (m_showFindingState)
			{
				setColor(m_initialColor);
			}
		}
	}
}

void cTargetGroup::beginFadeOut()
{
	m_fadeTimer.setTimeoutPeriodSeconds(5.0);
	m_fadeTimer.reset();
	m_fadeTimer.start();
	int numTargets = m_targets.size();
	for (int i = 0; i < numTargets; i++)
	{
		m_targets[i]->setUseTransparency(true, true);
	}
}

void cTargetGroup::fadeOut()
{
	if (!m_fadeTimer.timeoutOccurred())
	{
		double transparency = 1.0 - 1.0 / m_fadeTimer.getTimeoutPeriodSeconds() * m_fadeTimer.getCurrentTimeSeconds();
		int numTargets = m_targets.size();
		for (int i = 0; i < numTargets; i++)
		{
			m_targets[i]->setTransparencyLevel(transparency, false, true);
		}
	}
	else
	{
		m_fadeTimer.stop();
		int numTargets = m_targets.size();
		for (int i = 0; i < numTargets; i++)
		{
			m_targets[i]->setShowEnabled(false);
		}
		m_faded = true;
	}
}