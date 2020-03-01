//---------------------------------------------------------------------------
#include "system/CGlobals.h"
//---------------------------------------------------------------------------

#include <iostream>
#include <new>
#include <math.h>
#include "rtsRobotTeam.h"

// Initialize static variable
int rtsRobotTeam::numRobotTeams = 0;

rtsRobotTeam::~rtsRobotTeam()
{
    // Decrement robot team count
    numRobotTeams--;

	// remove robots from parent world's list of children
	removeRobotsFromChildren();

	// Deallocate memory
	deleteAndNullAllRobots();
}

//---------------------------------------------------------------------------

int rtsRobotTeam::getIdNumber()
{
    return idNumber;
}

//---------------------------------------------------------------------------

int rtsRobotTeam::getNumRobotTeams()
{
    return numRobotTeams;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::addRobot(rtsRobot* a_robot) 
{
	// add robot to list of robots
	m_team.push_back(a_robot);

	// add robot position to list of robot positions
	m_robotPositions.push_back(a_robot->getLocalPos());

	// set show option
	a_robot->setShowEnabled(m_showEnabled);

	// calculate the convex hull solution
	computeConvexHull_GiftWrap(m_robotPositions);
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setRobotPositions(vector<cVector3d> a_robotPositions)
{
	// set all robot positions
	if (a_robotPositions.size() == m_team.size())
	{
		// copy over vector of positions
		m_robotPositions = a_robotPositions;
		
		// iterate through team and assign positions
		for (unsigned int i = 0; i < a_robotPositions.size(); i++)
		{
			m_team[i]->setLocalPos(a_robotPositions[i]);
		}
	}

	// if in travel mode, calculate convex hull
	if (m_travelMode)
	{
		// calculate the convex hull solution
		computeConvexHull_GiftWrap(m_robotPositions);
		//computeConvexHull_Andrews(m_robotPositions);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setRobotPositions(vector<cVector3d> a_robotPositions, vector<int> a_ids)
{
	// set all robot positions
	if (a_robotPositions.size() == m_team.size() && a_ids.size() == m_team.size())
	{
		// copy over vector of positions
		int numRobotPositions = a_robotPositions.size();
		cVector3d prevPos;
		for (int i = 0; i < numRobotPositions; ++i)
		{
			// id and position of current robot
			int id = a_ids[i];
			cVector3d pos = a_robotPositions[i];
			
			// assign and set positions
			m_robotPositions[id] = pos;
			m_team[id]->setLocalPos(pos);

			// update surround lines
			if (m_surroundMode && i > 0)
			{
				m_surroundLines[i-1]->m_pointA = prevPos;
				m_surroundLines[i-1]->m_pointB = pos;
			}
			prevPos = pos;
		}

		// set last surround line to complete ring
		if (m_surroundMode)
		{
			m_surroundLines[numRobotPositions - 1]->m_pointA = prevPos;
			m_surroundLines[numRobotPositions - 1]->m_pointB = a_robotPositions[0];
		}
	}

	// if in travel mode, calculate convex hull
	if (m_travelMode)
	{
		// calculate the convex hull solution
		computeConvexHull_GiftWrap(a_robotPositions); // need to except the raw positions, not ordered
		//computeConvexHull_Andrews(a_robotPositions);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setRobotColors(cColorf a_color)
{
	for (unsigned int i = 0; i < m_team.size(); i++)
	{
		m_team[i]->setColor(a_color);
	}
}

void rtsRobotTeam::setSensorColors(cColorf a_color)
{
	for (unsigned int i = 0; i < m_team.size(); i++)
	{
		m_team[i]->setSensorColor(a_color);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setModeColors()
{
	if (m_travelMode)
	{
		setRobotColors(m_travelColor);
	}
	else if (m_surroundMode)
	{
		setRobotColors(m_surroundColor);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setRadii(double a_radius)
{
	for (unsigned int i = 0; i < m_team.size(); i++)
	{
		m_team[i]->setRadius(a_radius);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::computeConvexHull_Andrews(vector<cVector3d> a_robotPositions)
{

	int n = a_robotPositions.size(), k = 0;
	vector<cVector3d> H(2 * n);

	// convert cVector3d to rtsVector3d for sorting
	vector<rtsVector3d> sortPositions(n);
	for (int i = 0; i < n; i++)
	{
		sortPositions[i].x = a_robotPositions[i].x();
		sortPositions[i].y = a_robotPositions[i].y();
		sortPositions[i].z = a_robotPositions[i].z();
	}

	// Sort points lexicographically
	sort(sortPositions.begin(), sortPositions.end());

	// convert rtsVector3d to cVector3d
	for (int i = 0; i < n; i++)
	{
		a_robotPositions[i].x(sortPositions[i].x);
		a_robotPositions[i].y(sortPositions[i].y);
		a_robotPositions[i].z(sortPositions[i].z);
	}

	// Build lower hull
	for (int i = 0; i < n; ++i) {
		cVector3d vec1 = H[k - 1] - H[k - 2];
		cVector3d vec2 = a_robotPositions[i] - H[k - 1];
		while (k >= 2 && cCross(vec1, vec2).z() <= 0)
		{
			cVector3d vec1 = H[k - 1] - H[k - 2];
			cVector3d vec2 = a_robotPositions[i] - H[k - 1];
			k--;
		}
		H[k++] = a_robotPositions[i];
	}

	// Build upper hull
	for (int i = n - 2, t = k + 1; i >= 0; i--) {
		cVector3d vec1 = H[k - 1] - H[k - 2];
		cVector3d vec2 = a_robotPositions[i] - H[k - 1];
		while (k >= t && cCross(vec1, vec2).z() <= 0)
		{
			cVector3d vec1 = H[k - 1] - H[k - 2];
			cVector3d vec2 = a_robotPositions[i] - H[k - 1];
			k--;
		}
		H[k++] = a_robotPositions[i];
	}

	// set convex hull positions
	m_convexHullRobotPositions = H;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::computeConvexHull_GiftWrap(vector<cVector3d> a_robotPositions)
{
	int n = a_robotPositions.size();

	// can't have less than 3 points
	if (n < 3)
		return;

	// initialize convex hull positions
	vector<cVector3d> result;
	vector<int> mapResult;
	int mapIndex = 0;

	// find the far left point (must be a point on convex hull)
	int left = 0;
	for (int i = 1; i < n; i++)
	if (a_robotPositions[i].x() < a_robotPositions[left].x())
	{
		left = i;
	}

	// add far left point to result
	result.push_back(a_robotPositions[left]);

	// store far left point mapping
	mapResult.push_back(left);
	mapIndex++;

	// move counterclockwise from left point, building convex hull
	int current = left;
	int next;
	do
	{
		next = (current + 1) % n;
		for (int i = 0; i < n; i++)
		{
			cVector3d vec1 = a_robotPositions[i] - a_robotPositions[current];
			cVector3d vec2 = a_robotPositions[next] - a_robotPositions[i];
			if (cCross(vec1, vec2).z() > 0)
			{
				next = i;
			}
		}

		// add next to result
		result.push_back(a_robotPositions[next]);

		// add next to index map
		mapResult.push_back(next);
		mapIndex++;

		// set current for next iteration
		current = next;

		// prevent infinite loop
		if (result.size() > a_robotPositions.size()+1)
			break;

	} while (current != left);

	// store result
	m_convexHullRobotPositions = result;
	m_convexHullMapping = mapResult;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setDistributionMode(int a_mode)
{
	// if in standby mode, then initialize to new mode
	if (m_standbyMode)
	{
		// if change to travel
		if (a_mode == 0)
		{
			// set mode
			setMode(a_mode);

			// set colors to travel colors
			setRobotColors(m_travelColor);

			// create the lines that represent the convex hull
			createConvexHullLines();
		}

		// if change to surround
		else if (a_mode == 1)
		{
			// set mode
			setMode(a_mode);

			// set colors to surround colors
			setRobotColors(m_surroundColor);

			int numLines = m_convexHullLines.size();
			for (int i = 0; i < numLines; i++)
			{
				m_convexHullLines[i]->setShowEnabled(false);
			}

			deleteConvexHullLines();
		}
		else if (a_mode == 2)
		{
			// set mode
			setMode(a_mode);
		}
	}

	// check changes is distribution modes
	if (a_mode != m_mode)
	{
		setMode(m_defaultMode);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setMode(int a_mode)
{
	// reset modes
	m_standbyMode = false;
	m_travelMode = false;
	m_surroundMode = false;

	// set appropriate mode
	if (a_mode == -1)
	{
		m_standbyMode = true;
	}
	else if (a_mode == 0)
	{
		m_travelMode = true;
	}
	else if (a_mode == 1)
	{
		m_surroundMode = true;
	}

	// set member mode
	m_mode = a_mode;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setShowConvexHull(bool a_show) 
{ 
	int numLines = m_convexHullLines.size();
	for (int i = 0; i < numLines; i++)
	{
		m_convexHullLines[i]->setShowEnabled(a_show);
	}

	// set member variable
	m_showConvexHull = a_show;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setShowId(bool a_showId)
{
	m_showId = a_showId;

	int numRobots = m_team.size();
	for (int i = 0; i < numRobots; ++i)
	{
		m_team[i]->setShowId(m_showId);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::createConvexHullLines()
{
	deleteConvexHullLines();

	if (!m_convexHullRobotPositions.empty())
	{
		cVector3d current;
		cVector3d next;
		int numCHRobotPositions = m_convexHullRobotPositions.size();
		for (int i = 0; i <  numCHRobotPositions - 1; i++)
		{
			// set current position
			current = m_convexHullRobotPositions[i];

			// set next positions
			next = m_convexHullRobotPositions[i + 1];

			// shift lines up in z direction
			current = current + cVector3d(0, 0, 0.01);
			next = next + cVector3d(0, 0, 0.01);

			// create line
			cShapeLine* line = new cShapeLine(current, next);
			line->setLineStipple(1, 3);
			m_world->addChild(line);

			// store line
			m_convexHullLines.push_back(line);

			// set show enabled
			m_convexHullLines[i]->setShowEnabled(m_showConvexHull);
		}

		// set color of lines
		setConvexHullLineColor(cColorf(1.0, 0.0, 0.0, 1.0f));
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::updateConvexHullLines()
{
	if (!m_convexHullLines.empty())
	{
		if (m_prevConvexHullSize == m_convexHullRobotPositions.size())
		{
			cVector3d current;
			cVector3d next;
			int numCHRobotPositions = m_convexHullRobotPositions.size();
			for (int i = 0; i < numCHRobotPositions - 1; i++)
			{
				// set current position
				current = m_convexHullRobotPositions[i];

				// set next positions
				next = m_convexHullRobotPositions[i + 1];

				// shift lines up in z direction
				current = current + cVector3d(0, 0, 0.01);
				next = next + cVector3d(0, 0, 0.01);

				// update the line
				m_convexHullLines[i]->m_pointA = current;
				m_convexHullLines[i]->m_pointB = next;
			}
		}
		else
		{
			// re create convex hull lines in order to update
			createConvexHullLines();
		}

		// update previous convex hull size
		m_prevConvexHullSize = m_convexHullRobotPositions.size();
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setConvexHullLineColor(cColorf a_color)
{
	if (!m_convexHullLines.empty())
	{
		int numLines = m_convexHullLines.size();
		for (int i = 0; i < numLines; i++)
		{
			// update the line color by coloring both points on line
			m_convexHullLines[i]->m_colorPointA = a_color;
			m_convexHullLines[i]->m_colorPointB = a_color;
		}
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::deleteConvexHullLines()
{
	int numLines = m_convexHullLines.size();
	for (int i = 0; i < numLines; i++)
	{
		m_world->removeChild(m_convexHullLines[i]);

		// delete the line (clean up memory)
		delete m_convexHullLines[i];
		m_convexHullLines[i] = NULL;
	}

	m_convexHullLines.clear();
}

void rtsRobotTeam::createSurroundLines()
{
	for (int i = 0; i < this->size(); i++)
	{
		cVector3d zero(0, 0, 0);
		cShapeLine* line = new cShapeLine(zero, zero);
		line->setLineStipple(1, 2);
		line->m_colorPointA = cColorf(0, 0, 1.0);
		line->m_colorPointB = cColorf(0, 0, 1.0);
		line->setUseTransparency(true);
		line->setTransparencyLevel(0.2, true, true);
		line->setShowEnabled(false);
		m_world->addChild(line);
		m_surroundLines.push_back(line);
	}
}

void rtsRobotTeam::setShowSurroundLines(bool a_show)
{
	int numLines = m_surroundLines.size();
	for (int i = 0; i < numLines; i++)
	{
		m_surroundLines[i]->setShowEnabled(a_show);
	}
}

//---------------------------------------------------------------------------

void rtsRobotTeam::toggleShowEnabled(bool a_affectChildren)
{
	setShowEnabled(!m_showEnabled, a_affectChildren);
}

//---------------------------------------------------------------------------

void rtsRobotTeam::toggleShowId()
{
	setShowId(!m_showId);
}

vector<double> rtsRobotTeam::updateSensorReadings(vector<rtsHotspotGroup*> a_activeTargetGroups, int a_numActiveTargets, vector<double> a_nsewBuildingBoundaries, double a_time, bool a_surround)
{
	vector<double> nsewSignals(4, 0.0);
	double targetWeight = 0.5;
	double north = a_nsewBuildingBoundaries[0];
	double south = a_nsewBuildingBoundaries[1];
	double east = a_nsewBuildingBoundaries[2];
	double west = a_nsewBuildingBoundaries[3];

	// reset found states for all targets
	int numTargetGroups = a_activeTargetGroups.size();
	for (int i = 0; i < numTargetGroups; i++)
	{
		a_activeTargetGroups[i]->setTargetFoundStates(false);
		if (!a_activeTargetGroups[i]->getFoundState())
		{
			a_activeTargetGroups[i]->setColor(cColorf(1.0f, 0.0f, 0.0f));
		}
	}

	// for each robot in the team
	int numRobots = m_team.size();
	for (int i = 0; i < numRobots; i++)
	{
		bool targetInRange = false;
		bool trueDetection = false;

		// update false detection rate for this robot's sensor based on number of targets to search
		//m_team[i]->updateSensorFalseDetectionRate(a_numActiveTargets);

		// reset sensor prior to checking all targets
		m_team[i]->resetSensorDetectionState();

		m_team[i]->startTrueDetection();

		// for each active target, check for true positive
		int numATGroups = a_activeTargetGroups.size();
		if (numATGroups != 0)
		{
			for (int j = 0; j < numATGroups; j++)
			{
				rtsHotspotGroup* group = a_activeTargetGroups[j];
				int numTargets = group->getTargets().size();
				for (int k = 0; k < numTargets; k++)
				{
					rtsHotspot* target = group->getTarget(k);
					//cVector3d targetPos = target->getLocalPos();
					cVector3d targetPos = target->getTrueLocalPos();
					bool detection = false;
					if (m_team[i]->checkTargetInRange(targetPos))
					{
						if (!targetInRange)
						{
							targetInRange = true;
						}

						// check for true positive detection from sensor because target is in range
						detection = m_team[i]->updateTrueSensorReading(targetPos);

						if (!trueDetection && detection)
						{
							trueDetection = true;
						}

						if (!group->getFoundState())
						{
							//target->updateFoundState(true);
							//target->setColor(cColorf(1.0f, 0.0f, 1.0f));
							target->updateFoundCounter(detection);
						}
					}
				}
			}
		}

		m_team[i]->endTrueDetection();

		// no active targets, or no targets in range of robot, so check for false positive
		if (!targetInRange)
		{
			bool sensorReading = m_team[i]->updateSensorReadingNoTargets();
		}

		// after each sensor has either detection or not detected these targets, update transparency level for user to see
		m_team[i]->updateSensorLightTransparency();

		// save robot detection state
		//m_sensorReadings[i] = m_team[i]->getSensorDetectionState();

		if (m_team[i]->getSensorDetectionState())
		{

			// add this sensor reading to signals with weighting scale factor
			if (m_team[i]->getLocalPos().y() >= north)
			{
				if (trueDetection)
					nsewSignals[0] += targetWeight;
				else nsewSignals[0] += 1.0;
				//cout << "north detection" << endl;
				//n = true;
			}
			else if (m_team[i]->getLocalPos().y() <= south)
			{
				if (trueDetection)
					nsewSignals[1] += targetWeight;
				else nsewSignals[1] += 1.0;
				//s = true;
			}
			if (m_team[i]->getLocalPos().x() >= east)
			{
				if (trueDetection)
					nsewSignals[2] += targetWeight;
				else nsewSignals[2] += 1.0;
				//e = true;
			}
			else if (m_team[i]->getLocalPos().x() <= west)
			{
				if (trueDetection)
					nsewSignals[3] += targetWeight;
				else nsewSignals[3] += 1.0;
				//w = true;
			}
			//printf("n: %d, s: %d, e: %d, w: %d\n", n, s, e, w);
		}
	}

	if (a_surround)
	{
		// after all robots have had a chance to "sense" every target, check state of active groups
		for (int i = 0; i < numTargetGroups; i++)
		{
			a_activeTargetGroups[i]->checkFoundStates(a_time); // if all targets in group found, group is found
		}
	}

	return nsewSignals;
}

void rtsRobotTeam::setSensorLightTransparencyLevels(double a_level)
{
	int numRobots = m_team.size();
	for (int i = 0; i < numRobots; i++)
	{
		m_team[i]->setSensorLightTransparency(a_level);
	}
}

void rtsRobotTeam::clearSensorReadings()
{
	// for each robot in the team
	int numRobots = m_team.size();
	for (int i = 0; i < numRobots; i++)
	{
		m_team[i]->clearSensorReading();
	}

}

vector<bool> rtsRobotTeam::getSensorReadings()
{
	vector<bool> readings;
	// for each robot in the team
	int numRobots = m_team.size();
	for (int i = 0; i < numRobots; i++)
	{
		readings.push_back(m_team[i]->getSensorDetectionState());
	}
	return readings;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::setShowEnabled(bool a_showEnabled, bool a_affectChildren)
{
	// iterate through team and enable or disable visibility
	int numRobots = m_team.size();
	for (int i = 0; i < numRobots; i++)
	{
		m_team[i]->setShowEnabled(a_showEnabled, a_affectChildren);
	}

	// set member variable
	m_showEnabled = a_showEnabled;
}

//---------------------------------------------------------------------------

void rtsRobotTeam::deleteAndNullAllRobots()
{
	// delete all children
	vector<rtsRobot*>::iterator it;
	for (it = m_team.begin(); it < m_team.end(); it++)
	{
		cGenericObject* nextObject = (*it);
		delete (nextObject);
		nextObject = NULL;
	}

	// clear my list of children
	m_team.clear();
}

void rtsRobotTeam::removeRobotsFromChildren()
{
	// remove all children from world
	vector<rtsRobot*>::iterator it;
	for (it = m_team.begin(); it < m_team.end(); it++)
	{
		cGenericObject* nextObject = (*it);
		m_world->removeChild(nextObject);
	}
}