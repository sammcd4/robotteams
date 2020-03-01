#include "CTargetManager.h"

cTargetManager::cTargetManager(cGenericObject* a_parent)
{
	// set world for targets
	m_parent = a_parent;

	// set default colors
	m_initialColor = cColorf(0, 0, 0);
	m_foundColor = cColorf(0, 0, 0);

	// set default found radius
	m_foundRadius = 15.0;

	m_numActiveTargets = 0;

	m_showTargets = false;
}

cTargetManager::~cTargetManager()
{
	deallocate();
}

void cTargetManager::addTargetGroup(double a_radius, int a_trueTarget, int a_bldgId, double a_startTime, double a_endTime, vector<cVector3d> a_targetPos, int a_numTargets, int a_groupId)
{
	cTargetGroup* targetGroup = new cTargetGroup(a_groupId);
	for (int i = 0; i < a_numTargets; i++)
	{
		// create new target
		cTarget* target = new cTarget(a_radius, a_startTime, a_endTime);
		target->setLocalPos(a_targetPos[i]);
		targetGroup->addTarget(target);
		targetGroup->setStartTime(a_startTime);
		targetGroup->setEndTime(a_endTime);
	}

	// set color of target
	//targetGroup->setColor(m_initialColor);

	// add target to the vector of targets
	m_targetGroups.push_back(targetGroup);

	// add target index to the unfound target map
	m_unfoundTargetMap.push_back(m_targetGroups.size() - 1);

	// add target to the world of targets
	m_parent->addChild(targetGroup);
}

void cTargetManager::setTargetGroups(vector<cTargetGroup*> a_targetGroups)
{
	m_targetGroups = a_targetGroups;
	int numGroups = m_targetGroups.size();
	for (int i = 0; i < numGroups; i++)
	{
		m_parent->addChild(m_targetGroups[i]);
	}
}

void cTargetManager::updateTargetStates(vector<cVector3d> a_agentPositions)
{
	int numUnfoundTargets = m_unfoundTargetMap.size();
	if (numUnfoundTargets != 0)
	{
		vector<int> toErase;
		for (int i = 0; i < numUnfoundTargets; ++i)
		{
			int numTargets = m_targetGroups[m_unfoundTargetMap[i]]->getTargets().size();
			for (int j = 0; j < numTargets; j++)
			{
				cVector3d targetPos = m_targetGroups[m_unfoundTargetMap[i]]->getTarget(j)->getLocalPos();
				int numAgentPositions = a_agentPositions.size();
				for (int k = 0; k < numAgentPositions; ++k)
				{
					cVector3d agentPos = a_agentPositions[k];

					// check if distance between points is less than found radius
					if (cDistance(targetPos, agentPos) <= m_foundRadius)
					{
						if (!m_targetGroups[m_unfoundTargetMap[i]]->getTarget(j)->getFoundState())
						{
							// set target state to true
							//m_targetGroups[m_unfoundTargetMap[i]]->getTarget(j)->setFoundState(true);

							// set target color to found color
							m_targetGroups[m_unfoundTargetMap[i]]->getTarget(j)->setColor(m_foundColor);

							// add element from unfound target map to remove queue
							toErase.push_back(i);
						}
					}
				}
			}
			//std::cout << m_unfoundTargetMap[i] << ", ";
		}
		//std::cout << std::endl;

		// remove the found elements from unfound target map
		int numToEraseSize = toErase.size();
		for (int i = 0; i < numToEraseSize; ++i)
		{
			m_unfoundTargetMap.erase(m_unfoundTargetMap.begin() + toErase[i]);
			for (int j = i; j < numToEraseSize; ++j)
			{
				if (toErase[j] > toErase[i])
				{
					toErase[j] = toErase[j] - 1;
				}
			}
		}

		// clear vector
		toErase.clear();
	}
}

vector<cVector3d> cTargetManager::getActiveTargetPositions()
{
	vector<cVector3d> positions;
	int numGroups = m_targetGroups.size();
	for (int i = 0; i < numGroups; i++)
	{
		int numTargets = m_targetGroups[i]->getTargets().size();
		for (int j = 0; j < numTargets; j++)
		{
			if (m_targetGroups[i]->getTarget(j)->getActiveState())
			{
				positions.push_back(m_targetGroups[i]->getTarget(j)->getLocalPos());
			}
		}

	}

	return positions;
}

vector<cTargetGroup*> cTargetManager::getActiveTargetGroups()
{
	vector<cTargetGroup*> activeGroups;
	int numGroups = m_targetGroups.size();
	for (int i = 0; i < numGroups; i++)
	{
		if (m_targetGroups[i]->getActiveState())
		{
			activeGroups.push_back(m_targetGroups[i]);
		}
	}
	return activeGroups;
}


void cTargetManager::updateActiveTargetStates(double a_simTime, int a_currentBldgId)
{
	m_numActiveTargets = 0;
	int numGroups = m_targetGroups.size();
	for (int i = 0; i < numGroups; i++)
	{
		m_targetGroups[i]->updateActiveState(a_simTime, a_currentBldgId);
		if (m_targetGroups[i]->getActiveState())
			m_numActiveTargets += m_targetGroups[i]->getNumTargets();
	}
}

bool cTargetManager::writeTargetFoundData(string a_filename)
{
	ofstream fout(a_filename.c_str());

	int numTargetGroups = m_targetGroups.size();
	for (int i = 0; i < numTargetGroups; i++)
	{
		if (m_targetGroups[i]->getFoundState())
		{
			fout << m_targetGroups[i]->getGroupId() << " " << m_targetGroups[i]->getFoundTime() << endl;
		}
	}

	// check ofstream failbit
	if (fout.fail())
		return false;
	else return true;
}

void cTargetManager::setShowTargets(bool a_show)
{
	int numTargetGroups = m_targetGroups.size();
	for (int i = 0; i < numTargetGroups; i++)
	{
		m_targetGroups[i]->setShowTargets(a_show);
	}
	m_showTargets = a_show;
}


void cTargetManager::deallocate()
{
	// delete all targets
	vector<cTargetGroup*>::iterator it;
	for (it = m_targetGroups.begin(); it < m_targetGroups.end(); it++)
	{
		cTargetGroup* nextTargetGroup = (*it);
		m_parent->removeChild(nextTargetGroup);
		delete (nextTargetGroup);
		nextTargetGroup = NULL;
	}

	// clear my list of children
	m_targetGroups.clear();
}