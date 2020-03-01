
//---------------------------------------------------------------------------
#include "CGELPlaydoh.h"
//---------------------------------------------------------------------------
typedef GLUquadric GLUquadricObj;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of cGELPlaydoh.

    \fn     cGELPlaydoh::cGELPlaydoh()
    \param  a_world
	\param  a_robotPositions
*/
//===========================================================================
cGELPlaydoh::cGELPlaydoh(cWorld* a_world, vector<cVector3d> a_robotPositions, double a_toolRadius, double a_toolWorkspaceScaleFactor, double a_maxStiffness, bool a_useBuildingBounds)
{
	// set parent world
	m_world = a_world;

	// create deformable world
	m_defWorld = new cGELPlaydohWorld();
	m_world->addChild(m_defWorld);
	
	// set robot positions
	m_robotPositions = a_robotPositions;

	// set tool radius
	m_toolRadius = a_toolRadius;

	// set workspace scale factor
	m_toolWorkspaceScaleFactor = a_toolWorkspaceScaleFactor;

	// set max stiffness
	m_maxNodeStiffness = a_maxStiffness;

	// initialize cordon parameters
	initCordonParams();

	// create a deformable playdoh mesh for cordon
	m_cordonMesh = new cGELPlaydohMesh(m_world);

	// add cordon mesh to deformable world meshes
	m_defWorld->m_gelMeshes.push_back(m_cordonMesh);

	// force cordon mode to initialize cordon
	setMode(1);

	// create cordon
	initCordon();

	// create a deformable playdoh mesh for patrol
	m_patrolMesh = new cGELPlaydohMesh(m_world);

	// add patrol mesh to deformable world meshes
	m_defWorld->m_gelMeshes.push_back(m_patrolMesh);

	// create static cordon mesh
	m_cordonStaticResizeMesh = new cGELPlaydohMesh(m_world);
	m_defWorld->m_gelMeshes.push_back(m_cordonStaticResizeMesh);

	// set default mode
	m_defaultMode = -1;
	//setMode(m_defaultMode);

	// patrol links enable
	m_usePatrolLinks = true;

	// reset patrol
	m_resetPatrol = false;
	m_resetPatrolCounter = 0;

	// set patrol node color
	m_patrolNodeColor = cColorf(0.5, 0.0, 0.0, 1.0f);

	// set cordon node color
	m_cordonNodeColor = cColorf(
		0 / 256.0,
		85 / 256.0,
		256 / 256.0, 1);

	// set resize cordon
	m_resizeCordon = false;

	// contact state
	m_contacted = false;

	// previous contact state
	m_prevContacted = false;

	// use building bounds
	m_useBuildingBounds = a_useBuildingBounds;

	// set spacing factor
	m_spacingTol = 0.20;

	// transition timeout
	m_transitionTimeout = 1.0;
	m_transitionTimer.setTimeoutPeriodSeconds(m_transitionTimeout);

	//// set transition force factor time interval
	//m_transitionForceFactorTimeInterval = m_transitionTimeout / 100;

	// transition
	m_transition = false;
	m_prevTransition = false;

	// initialize forces
	m_toolForce = cVector3d(0, 0, 0);
	m_prevToolForce = cVector3d(0, 0, 0);
	m_playdohForce = cVector3d(0, 0, 0);
	m_prevPlaydohForce = cVector3d(0, 0, 0);
	m_transitionForce = cVector3d(0, 0, 0);

	// center of mass initialization
	m_centerOfMass = cVector3d(0, 0, 0);
	int numRobots = m_robotPositions.size();
	for (int i = 0; i < numRobots; i++)
		m_centerOfMass += m_robotPositions[i];
	m_centerOfMass.div(numRobots);

	// initialize spacing timer
	m_spacingClock.setTimeoutPeriodSeconds(0.1);
	m_spacingClock.reset();
	m_spacingClock.start();
}

//---------------------------------------------------------------------------

cGELPlaydoh::~cGELPlaydoh()
{

}

//---------------------------------------------------------------------------

void cGELPlaydoh::initPlaydoh()
{
	// hide visual and haptics
	setShowEnabled(false, true);
	setHapticEnabled(false, true);

	// force patrol mode to initialize patrol
	setMode(0);

	// create patrol
	initPatrol();

	// set initial previous convex hull mapping
	m_prevConvexHullMapping = m_convexHullMapping;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setShowEnabled(bool a_showEnabled, bool a_affectChildren) 
{ 
	// set member variable
	m_showEnabled = a_showEnabled;

	if (m_cordonMode)
		m_cordonMesh->setShowEnabled(a_showEnabled, a_affectChildren);
	else if (m_patrolMode)
		m_patrolMesh->setShowEnabled(a_showEnabled, a_affectChildren);

}

//---------------------------------------------------------------------------

bool cGELPlaydoh::getShowEnabled()
{
	if (m_cordonMode)
		return m_cordonMesh->getShowEnabled();
	else if (m_patrolMode)
		return m_patrolMesh->getShowEnabled();
	else return false;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setHapticEnabled(bool a_hapticEnabled, bool a_affectChildren)
{
	m_affectHapticChildren = a_affectChildren;
	m_hapticEnabled = a_hapticEnabled;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setStiffness(double a_stiffness)
{
	cGELPlaydohSkeletonNode* temp;
	vector<cGELPlaydohSkeletonNode*>::iterator iter;
	for (iter = m_cordonSpheres.begin(); iter != m_cordonSpheres.end(); iter++)
	{
		// dereference iterator
		temp = *iter;

		// set stiffness for the sphere
		temp->setStiffness(a_stiffness);
	}
}

//---------------------------------------------------------------------------

void cGELPlaydoh::updateRobotPositions(vector<cVector3d> a_robotPositions)
{
	if (!m_standbyMode && !m_resizeCordon)
	{
		// update the vector storing robot positions
		m_robotPositions = a_robotPositions;

		cGELPlaydohSkeletonNode* temp;
		cVector3d pos;
		vector<cGELPlaydohSkeletonNode*>::iterator iter;
		vector<cVector3d>::iterator iterPos;

		vector<cGELPlaydohSkeletonNode*> a_robotSpheres;
		if (m_cordonMode)
		{
			// beginning robot position
			iterPos = m_robotPositions.begin();

			a_robotSpheres = m_robotCordonSpheres;
		}
		else if (m_patrolMode)
		{
			// beginning robot position
			iterPos = m_convexHullRobotPositions.begin();

			a_robotSpheres = m_robotPatrolSpheres;
		}


		// update the physical position of robot-containing nodes
		for (iter = a_robotSpheres.begin(); iter != a_robotSpheres.end(); iter++)
		{
			// dereference iterators
			temp = *iter;
			pos = *iterPos;

			// set position of node
			temp->m_nextPos = pos;
			temp->applyNextPose();
			//temp->m_pos = pos;

			// increment vector iterator
			iterPos++;
		}

		// assign member variables to new positions
		if (m_patrolMode)
		{
			m_robotPatrolSpheres = a_robotSpheres;
		}
		else if (m_cordonMode)
		{
			m_robotCordonSpheres = a_robotSpheres;
		}

	}
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setConvexHull(vector<cVector3d> a_convexHull)
{
	// set convex hull
	m_convexHullRobotPositions = a_convexHull;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setConvexHullMapping(vector<int> a_convexHullMapping)
{
	// set member variable
	m_convexHullMapping = a_convexHullMapping;

	//m_resetPatrol = false;
	if (m_convexHullMapping != m_prevConvexHullMapping)
	{
		// call for a reset in the patrol
		m_resetPatrol = true;

		// start transition
		startTransition();
	}

	// store previous convex hull mapping for next iteration
	m_prevConvexHullMapping = m_convexHullMapping;
}

//---------------------------------------------------------------------------

cVector3d cGELPlaydoh::computeForce(const cVector3d& a_cursor, double a_cursorRadius, int a_closestIndex, double a_modFactor, bool a_manip, bool a_manipSpacing)
{
	// zero force
	cVector3d force;
	force.zero();
	//a_manip = false;
	//a_manipSpacing = true;
	// set previous playdoh force
	m_prevPlaydohForce = m_playdohForce;

	// set previous tool force
	m_prevToolForce = m_toolForce;

	// set previous contact state
	m_prevContacted = m_contacted;

	// reset contact state of playdoh
	m_contacted = false;

	// object pointers
	cGELPlaydohSkeletonNode* current;
	cGELPlaydohSkeletonNode* next;
	cGELPlaydohSkeletonNode* prev;
	cGELPlaydohSkeletonLink* currentLink;
	cGELPlaydohSkeletonLink* nextLink;
	//cGELPlaydohSkeletonLink* prevLink;

	// iterators
	vector<cGELPlaydohSkeletonNode*>::iterator iter;
	vector<cGELPlaydohSkeletonNode*>::iterator iterNext;
	vector<cGELPlaydohSkeletonNode*>::iterator iterPrev;
	vector<cGELPlaydohSkeletonNode*>::iterator iterPrevRobotNode;
	vector<cGELPlaydohSkeletonNode*>::iterator iterCurrentRobotNode;
	cGELPlaydohSkeletonNode* prevRobotNode;
	cGELPlaydohSkeletonNode* currentRobotNode;
	int currentRobotNodeIndex = 0;
	int prevRobotNodeIndex = 0;

	if (!m_robotCordonSpheres.empty())
	{
		prevRobotNode = m_robotCordonSpheres[currentRobotNodeIndex];
		currentRobotNode = prevRobotNode;
	}
	double totalInnerDistSum = 0;
	double totalSpringForceSum = 0;

	list<cGELPlaydohSkeletonLink*>::iterator iterLink;
	list<cGELPlaydohSkeletonLink*>::iterator iterLinkNext;
	list<cGELPlaydohSkeletonLink*>::iterator iterLinkPrev;
	list<cGELPlaydohSkeletonNode*>::iterator meshNode;
	list<cGELPlaydohSkeletonNode*>::iterator prevMeshNode;

	// set local spheres and mesh variables
	vector<cGELPlaydohSkeletonNode*> a_spheres;
	cGELPlaydohMesh* a_mesh;
	if (m_cordonMode)
	{
		a_spheres = m_cordonSpheres;
		a_mesh = m_cordonMesh;
	}
	else if (m_patrolMode)
	{
		a_spheres = m_patrolSpheres;
		a_mesh = m_patrolMesh;
	}

	// check spacing timeout so that nodes aren't moving as fast as computeForce is called (1kHz)
	bool spacingTimeout = false;
	if (m_spacingClock.timeoutOccurred())
	{
		m_spacingClock.stop();
		spacingTimeout = true;
	}

	// winding number variable
	int wn = 0;
	double penetrationDist = (a_cursor.z() - a_cursorRadius) - m_patrolHeight;

	// if resizing is being done, then skip this function.
	// no force computation is needed to resize
	if (m_resizeCordon)
	{
		// resize cordon instead of calculate force
		resizeCordon();
	}
	else if (m_resetPatrol)
	{
		// need to set force to previously calculated tool force to avoid a zero force between iterations
		force = m_prevToolForce;

		// set current playdoh force
		m_playdohForce = -1 * force;

		// set current tool force
		m_toolForce = force;

		// reset variable
		m_resetPatrolCounter++;
		if (m_resetPatrolCounter > 1)
		{
			m_resetPatrol = false;
			m_resetPatrolCounter = 0;
		}
	}
	// compute force, if mesh exists
	else if(!a_spheres.empty())
	{
		// starting iterator values
		iterPrev = a_spheres.end();
		--iterPrev;
		prev = *iterPrev;

		meshNode = a_mesh->m_nodes.begin();
		prevMeshNode = a_mesh->m_nodes.end();
		--prevMeshNode;

		if(m_cordonMode || (m_patrolMode && m_usePatrolLinks))
		{
			iterLink = a_mesh->m_links.begin();
			iterLinkPrev = a_mesh->m_links.end();
			--iterLinkPrev;
		}

		double tempPerimeter;
		if (m_cordonMode)
		{
			// temporary perimeter
			tempPerimeter = 0;
		}

		// set a flag that triggers once a contacted sphere is reached
		bool manipFlag = false;
		int numFollowingManipRobots = 0;

		cVector3d xyzBar;
		xyzBar.zero();
		double totalMass = 0;
		bool externalForceApplied = false;

		// inner node distances
		bool innerDistStart = false;
		double innerDistSum = 0;

		// update spacing factors
		if (a_manipSpacing)
			updateSpacingFactors(a_closestIndex, a_modFactor);

		for (iter = a_spheres.begin(); iter != a_spheres.end(); iter++)
		{
			// set current sphere
			current = *iter;

			// set next sphere
			iterNext = iter;
			if ((iterNext != a_spheres.end()) && ((++iterNext) == a_spheres.end()))
			{
				next = *(a_spheres.begin());
			}
			else
			{
				next = *iterNext;
			}

			// set prev sphere
			if (iter == a_spheres.begin())
			{
				prev = a_spheres.back();
			}

			// set link iterators
			if (m_cordonMode || (m_patrolMode && m_usePatrolLinks))
			{
				// set current link
				currentLink = *iterLink;

				// set next link
				iterLinkNext = iterLink;
				if ((iterLinkNext != a_mesh->m_links.end()) && ((++iterLinkNext) == a_mesh->m_links.end()))
				{
					nextLink = *(a_mesh->m_links.begin());
				}
				else
				{
					nextLink = *iterLinkNext;
				}
			}

			// calculate mass times x and y
			double mass = current->m_mass;
			xyzBar.x(xyzBar.x() + mass * current->m_pos.x());
			xyzBar.y(xyzBar.y() + mass * current->m_pos.y());
			xyzBar.z(xyzBar.z() + mass * current->m_pos.z());
			totalMass += mass;

			// current and next positions
			cVector3d currentPos = current->m_pos;
			cVector3d nextPos = next->m_pos;

			// calculate playdoh bounding box
			//--------------------------------
			if (iter == a_spheres.begin())
			{
				m_boundingBoxPos = currentPos;
				m_boundingBoxNeg = currentPos;
			}
			if ((currentPos.x() - current->m_radius) < m_boundingBoxNeg.x())
			{
				m_boundingBoxNeg.x(currentPos.x() - current->m_radius);
			}
			if ((currentPos.y() - current->m_radius) < m_boundingBoxNeg.y())
			{
				m_boundingBoxNeg.y(currentPos.y() - current->m_radius);
			}
			if ((currentPos.z() - current->m_radius) < m_boundingBoxNeg.z())
			{
				m_boundingBoxNeg.z((currentPos.z() - current->m_radius));
			}
			if ((currentPos.x() + current->m_radius) > m_boundingBoxPos.x())
			{
				m_boundingBoxPos.x(currentPos.x() + current->m_radius);
			}
			if ((currentPos.y() + current->m_radius) > m_boundingBoxPos.y())
			{
				m_boundingBoxPos.y(currentPos.y() + current->m_radius);
			}
			if ((currentPos.z() + current->m_radius) > m_boundingBoxPos.z())
			{
				m_boundingBoxPos.z((currentPos.z() + current->m_radius));
			}
			//--------------------------------

			// calculate change in perimeter, and if needed, add spheres in the list
			if (m_cordonMode)
			{
				// calculate addition to perimeter
				cVector3d diff = nextPos - currentPos;
				tempPerimeter = tempPerimeter + diff.length();
			}

			// Adjust node stiffness based on exploration or manipulation
			if (a_manip)
			{
				current->m_stiffness = m_cordonNodeStiffness;
			}
			else current->m_stiffness = m_cordonNodeStiffness / 2;

			// compute external force on node
			cVector3d nodePos = current->m_pos;
			double nodeRadius = current->m_radius;
			double nodeStiffness = current->m_stiffness;
			cVector3d f = computeCylNodeForce(a_cursor, a_cursorRadius, nodePos, nodeRadius, nodeStiffness);
			cVector3d tmpfrc = cNegate(f);
			if (tmpfrc.length() > 0)
			{
				if (!externalForceApplied)
				{
					externalForceApplied = true;
				}

				// set contact state of node
				current->m_contacted = true;

				// set contact state of playdoh
				if (!m_contacted)
				{
					m_contacted = true;
				}
			}
			else
			{
				current->m_contacted = false;
			}

			// update spacing between robot nodes (only if manipulating playdoh)
			if (m_cordonMode && (a_manipSpacing && spacingTimeout))
			{
				bool rnFlag = false;
				if (current->m_atRobot && !current->m_newAtRobot)
				{
					if (!innerDistStart)
					{
						innerDistStart = true;
						iterPrevRobotNode = iter;
						currentRobotNodeIndex = 0;
					}
					else
					{
						// set current robot iterator
						iterCurrentRobotNode = iter;
						currentRobotNodeIndex++;

						// compare node distance to equilibrium distance
						if (innerDistSum > (1.0 + m_spacingTol) * m_uniformRobotSpacing * m_spacingFactors[currentRobotNodeIndex])
						{
							// move previous and current robot nodes closer to each other, if possible
							if (iterPrevRobotNode != a_spheres.begin())
							{
								//cGELPlaydohSkeletonNode* node = *(iterPrevRobotNode+1);
								//if (!node->m_atRobot)
								//{
								//	m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = false;
								//	m_robotCordonSpheres[prevRobotNodeIndex] = *(++iterPrevRobotNode);
								//	m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = true;
								//	iterPrevRobotNode++;
								//}
							}

							if (!prev->m_atRobot)
							{
								m_robotCordonSpheres[currentRobotNodeIndex]->m_atRobot = false;
								prev->m_atRobot = true;
								m_robotCordonSpheres[currentRobotNodeIndex] = prev;
								iterCurrentRobotNode--;
							}
						}
						else if (innerDistSum < (1.0 - m_spacingTol) * m_uniformRobotSpacing * m_spacingFactors[currentRobotNodeIndex])
						{
							if (!next->m_atRobot)
							{
								m_robotCordonSpheres[currentRobotNodeIndex]->m_atRobot = false;
								next->m_atRobot = true;
								next->m_newAtRobot = true; // so this node is skipped on next iteration
								m_robotCordonSpheres[currentRobotNodeIndex] = next;
								iterCurrentRobotNode++;
							}

							// move previous and current robot nodes away from each other
							//if (iterPrevRobotNode != a_spheres.begin())
							//{
							//	cGELPlaydohSkeletonNode* node = *(iterPrevRobotNode-1);
							//	if (!node->m_atRobot)
							//	{
							//		m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = false;
							//		m_robotCordonSpheres[prevRobotNodeIndex] = node;
							//		m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = true;
							//		iterPrevRobotNode--;
							//	}
							//}
						}

						// set previous robot iterator
						iterPrevRobotNode = iterCurrentRobotNode;
						prevRobotNodeIndex++;

						rnFlag = true;
					}

					innerDistSum += cDistance(prev->m_pos, current->m_pos);
				}
				// if no more robot nodes because reach end of list
				else if (next == *(a_spheres.begin()))
				{
					// compare node distance to average node distance
					if (innerDistSum > (1.0 + m_spacingTol) * m_uniformRobotSpacing * m_spacingFactors[currentRobotNodeIndex])
					{
						cGELPlaydohSkeletonNode* node = *(iterPrevRobotNode+1);
						if (!node->m_atRobot)
						{
							// just move previous robot node closer
							m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = false;
							m_robotCordonSpheres[prevRobotNodeIndex] = node;
							m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = true;
							iterPrevRobotNode++;
						}
						//cout << "end list spacing greater" << endl;
					}
					else if (innerDistSum < (1.0 - m_spacingTol) * m_uniformRobotSpacing * m_spacingFactors[currentRobotNodeIndex])
					{
						cGELPlaydohSkeletonNode* node = *(iterPrevRobotNode-1);
						if (!node->m_atRobot)
						{
							// just move previous robot node away
							m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = false;
							m_robotCordonSpheres[prevRobotNodeIndex] = node;
							m_robotCordonSpheres[prevRobotNodeIndex]->m_atRobot = true;
							iterPrevRobotNode--;
						}
					}

					innerDistSum += cDistance(prev->m_pos, current->m_pos);
					rnFlag = true;
				}
				else
				{
					if (current->m_newAtRobot)
					{
						current->m_newAtRobot = false;
					}
					innerDistSum += cDistance(prev->m_pos, current->m_pos);
				}

				if (rnFlag)
				{
					totalInnerDistSum += innerDistSum;

					// reset inner node distance sum
					innerDistSum = 0;

					rnFlag = false;
				}
			}

			// keep playdoh static when manipulating spacing
			if (a_manipSpacing || (!a_manip && m_contacted))
			{
				current->m_fixed = true;
				tmpfrc.zero();
			}
			else
			{
				current->m_fixed = false;
			}

			// check if node is moving inside building
			if (m_cordonMode && m_useBuildingBounds)
			{
				int numBounds = m_buildingBounds.size();
				bool insideBounds = false;
				for (int i = 0; i < numBounds; i++)
				{
					if (m_buildingBounds[i].isInside(current->m_pos))
					{
						insideBounds = true;
						break;
					}
				}

				if (insideBounds)
				{
					// if previously not inside bldg
					if (!current->m_insideBldg)
					{
						// store entry vector
						current->m_entryVec = cNormalize(current->m_pos - current->m_prevPos);
						current->m_insideBldg = true;
					}

					// apply small force to computed force away from building
					cVector3d insideF = 0.0001*m_toolWorkspaceScaleFactor*(-current->m_entryVec);
					tmpfrc.add(insideF);
				}
				else current->m_insideBldg = false;
			}

			// set external force on current node
			tmpfrc.z(0);
			current->setExternalForce(tmpfrc);

			// add node force to haptic force, if haptic effect is enabled
			if (m_hapticEnabled)
			{
				force.add(f);
			}

			// compute inner force for patrol
			// (point in polygon algorithm)
			if (m_patrolMode)
			{
				// check if the cursor is even low enough to contact the patrol
				if (penetrationDist < 0)
				{
					if (current->m_pos.y() <= a_cursor.y()) {          // start y <= P.y()
						if (next->m_pos.y() > a_cursor.y())      // an upward crossing
						{
							cVector3d vec1(next->m_pos - current->m_pos);
							vec1.z(0);
							cVector3d vec2(a_cursor - next->m_pos);
							vec2.z(0);
							double val = cCross(vec1, vec2).z();
							if (val > 0)		 // point is left of edge
							{
								++wn;            // have  a valid up intersect
							}
						}
					}
					else {                        // start y > P.y() (no test needed)
						if (next->m_pos.y() <= a_cursor.y())     // a downward crossing
						{
							cVector3d vec1(next->m_pos - current->m_pos);
							vec1.z(0);
							cVector3d vec2(a_cursor - next->m_pos);
							vec2.z(0);
							double val = cCross(vec1, vec2).z();
							if (val < 0)		// P right of  edge
							{
								--wn;            // have  a valid down intersect
							}
						}
					}
				}
			}

			// always set node to align with xy plane
			current->m_pos.z(0);

			//-----------------------------------------------------------------------
			// increment variables for next iteration
			//-----------------------------------------------------------------------

			// increment the link iterator
			if(m_cordonMode || (m_patrolMode && m_usePatrolLinks))
			{
				iterLink++;
			}

			// store previous iterators for next iterate
			iterPrev = iter;
			prevMeshNode = meshNode;
			if(m_cordonMode || (m_patrolMode && m_usePatrolLinks))
			{
				iterLinkPrev = iterLink;
			}

			// store previous sphere for next iterate
			prev = current;

			// increment the mesh node
			meshNode++;
		}

		// calculate center of mass
		m_centerOfMass = xyzBar / totalMass;

		// set uniform spacing value
		m_uniformRobotSpacing = m_perimeter / m_robotCordonSpheres.size();


		// add inner force for patrol
		if (m_patrolMode)
		{
			// if cursor is inside the patrol polygon, then add force
			if (m_hapticEnabled && wn > 0 && penetrationDist < 0)
			{
				force.add(penetrationDist*m_patrolStiffness*cVector3d(0,0,-1));
			}
		}

		// set current playdoh force
		m_playdohForce = -1 * force;

		// set current tool force
		m_toolForce = force;
	}


	// store initial transition force
	m_prevTransition = m_transition;

	// account for transitioning
	if (m_transition)
	{
		// limit current computed force
		double transitionForceFactor = m_transitionTimer.getCurrentTimeSeconds() / m_transitionTimeout;
		force.mul(transitionForceFactor);

		// maintain past force
		force.add(cMul((1.0 - transitionForceFactor), m_transitionForce));

		if (m_transitionTimer.timeoutOccurred())
		{
			endTransition();
		}
	}

	// reset spacing timeout
	if (spacingTimeout)
	{
		m_spacingClock.reset();
		m_spacingClock.start();
	}

	// return the haptic force
	return force;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::updateDynamics(double a_timeInterval)
{
	// update the deformable world
	m_defWorld->updateDynamics(a_timeInterval);

	cGELPlaydohSkeletonNode* temp;
	cVector3d pos;
	vector<cGELPlaydohSkeletonNode*>::iterator iter;
	vector<cVector3d>::iterator iterPos;

	vector<cGELPlaydohSkeletonNode*> a_robotSpheres;
	if (m_cordonMode)
	{
		// beginning robot position
		iterPos = m_robotPositions.begin();

		a_robotSpheres = m_robotCordonSpheres;
	}
	else if (m_patrolMode)
	{
		// beginning robot position
		iterPos = m_convexHullRobotPositions.begin();

		a_robotSpheres = m_robotPatrolSpheres;
	}


	// set robot positions based on dynamics
	int i = 0;
	for (iter = a_robotSpheres.begin(); iter != a_robotSpheres.end(); iter++)
	{
		// dereference iterators
		temp = *iter;
		pos = *iterPos;

		// store new robot position
		if (m_cordonMode)
		{
			m_robotPositions[i] = temp->m_pos;
		}
		else if (m_patrolMode)
		{
			int index = m_convexHullMapping[i];
			m_robotPositions[index] = temp->m_pos;
		}

		iterPos++;
		i++;
	}
}

//---------------------------------------------------------------------------

cVector3d cGELPlaydoh::computeNodeForce(const cVector3d& a_cursor,
										double a_cursorRadius,
										const cVector3d& a_spherePos,
										double a_radius,
										double a_stiffness)
{

	// compute the reaction forces between the tool and the ith sphere.
	cVector3d force;
	force.zero();
	cVector3d vSphereCursor = a_cursor - a_spherePos;

	// check if both objects are intersecting
	if (vSphereCursor.length() < 0.0000001)
	{
		return (force);
	}

	if (vSphereCursor.length() > (a_cursorRadius + a_radius))
	{
		return (force);
	}

	// compute penetration distance between tool and surface of sphere
	double penetrationDistance = (a_cursorRadius + a_radius) - vSphereCursor.length();
	cVector3d forceDirection = cNormalize(vSphereCursor);
	force = cMul(penetrationDistance * a_stiffness, forceDirection);

	// return result
	return (force);
}

//---------------------------------------------------------------------------

cVector3d cGELPlaydoh::computeCylNodeForce(const cVector3d& a_cursor,
	double a_cursorRadius,
	const cVector3d& a_spherePos,
	double a_radius,
	double a_stiffness)
{

	// compute the reaction forces between the tool and the ith sphere.
	cVector3d force;
	force.zero();
	cVector3d vSphereCursor = a_cursor - a_spherePos;

	// project all vectors to the xy plane
	double vHeight = vSphereCursor.z();
	vSphereCursor.z(0);

	// check if both objects are intersecting
	if (vSphereCursor.length() < 0.0000001)
	{
		return (force);
	}

	if (vSphereCursor.length() > (a_cursorRadius + a_radius))
	{
		return (force);
	}

	double cutOffHeight = 1.0;
	if (m_cordonMode)
	{
		cutOffHeight = m_cordonHeight - a_radius;
	}
	else if (m_patrolMode)
	{
		cutOffHeight = m_patrolHeight - a_radius;
	}


	if (vHeight > cutOffHeight)
	{
		cVector3d a_newSpherePos = a_spherePos;
		a_newSpherePos.z(a_newSpherePos.z() + cutOffHeight);
		return computeNodeForce(a_cursor, a_cursorRadius, a_newSpherePos, a_radius, a_stiffness);
	}

	// compute penetration distance between tool and surface of sphere
	double penetrationDistance = (a_cursorRadius + a_radius) - vSphereCursor.length();
	cVector3d forceDirection = cNormalize(vSphereCursor);
	force = cMul(penetrationDistance * a_stiffness, forceDirection);

	// return result
	return (force);
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setBuildingBounds(vector<rtsPolygonBounds> a_buildingBounds)
{
	m_buildingBounds = a_buildingBounds;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setDistributionMode(int a_mode)
{
	// if in standby mode, then initialize to new mode
	if (m_standbyMode)
	{
		if (m_enableMutex)
		{
			m_mutex->lock();
			m_cordonMesh->m_enableMutex = true;
			m_patrolMesh->m_enableMutex = true;

			// if change to patrol
			if (a_mode == 0)
			{
				// set mode
				setMode(a_mode);

				// initialize patrol
				initPatrol();
			}

			// if change to cordon
			else if (a_mode == 1)
			{
				// set mode
				setMode(a_mode);

				// initialize cordon
				initCordon();
			}
			else if (a_mode == 2)
			{
				// set mode
				setMode(a_mode);
			}

			m_mutex->unlock();
			m_enableMutex = false;
		}
	}

	// check changes in distribution modes
	if (a_mode != m_mode || (m_patrolMode && m_resetPatrol))
	{
		// start transition procedure
		if (a_mode != m_mode)
		{
			startTransition();
		}

		// hide visual and haptics
		if (m_cordonMode)
		{
			m_cordonMesh->setShowEnabled(false, true);
			m_cordonMesh->m_showSkeletonModel = false;
		}
		else if (m_patrolMode)
		{
			m_patrolMesh->setShowEnabled(false, true);
			m_patrolMesh->m_showSkeletonModel = false;
		}

		//setHapticEnabled(false, true);

		// since there was a change in mode, set to standby mode
		setMode(m_defaultMode);
		m_enableMutex = true;
	}
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setMutex(std::mutex* a_mutex)
{
	// set mutex
	m_mutex = a_mutex;

	// set mutex in playdoh meshes
	m_cordonMesh->setMutex(m_mutex);
	m_patrolMesh->setMutex(m_mutex);

	// initialize enable value
	m_enableMutex = false;
}

//---------------------------------------------------------------------------

bool cGELPlaydoh::insideBoundingBox(cVector3d a_pos)
{
	if (a_pos.x() >= m_boundingBoxNeg.x() && a_pos.x() <= m_boundingBoxPos.x()
		&& a_pos.y() >= m_boundingBoxNeg.y() && a_pos.y() <= m_boundingBoxPos.y()
		&& a_pos.z() >= m_boundingBoxNeg.z() && a_pos.z() <= m_boundingBoxPos.z())
	{
		return true;
	}
	else return false;
}

//---------------------------------------------------------------------------

int cGELPlaydoh::getNumPatrolSpheres()
{
	return m_patrolSpheres.size();
}

//---------------------------------------------------------------------------

int cGELPlaydoh::getNumCordonSpheres()
{
	return m_cordonSpheres.size();
}

void cGELPlaydoh::setEnableResizeCordon(bool a_resizeCordon) 
{ 
	if (!m_resizeCordon && a_resizeCordon)
	{
		m_resizeStart = true;
	}

	// if done resizing cordon, create links again between nodes
	if (m_resizeCordon && !a_resizeCordon)
	{
		// delete the static mesh
		// disable visibility of node model
		m_cordonStaticResizeMesh->m_useSkeletonModel = false;
		m_cordonStaticResizeMesh->m_showSkeletonModel = false;

		// deallocate memory to each node in playdoh mesh
		cGELPlaydohSkeletonNode* tempNode;
		list<cGELPlaydohSkeletonNode*>::iterator node;
		for (node = m_cordonStaticResizeMesh->m_nodes.begin(); node != m_cordonStaticResizeMesh->m_nodes.end(); node++)
		{
			// dereference iteratorr
			tempNode = *node;

			// Deallocate memory
			delete tempNode;
			tempNode = NULL;
		}

		// clear the list of nodes in playdoh mesh
		m_cordonStaticResizeMesh->m_nodes.clear();

		// remove the static mesh from the list of meshes in the defermable world
		//m_defWorld->m_gelMeshes.pop_back();

		//setMode(m_defaultMode);
		//setDistributionMode(1);

		//createNodeLinks();

		// show playdoh mesh
		m_cordonMesh->m_useSkeletonModel = true;
		m_cordonMesh->m_showSkeletonModel = true;
		m_cordonMesh->setShowEnabled(m_showEnabled, true);
	}

	m_resizeCordon = a_resizeCordon;
}
//---------------------------------------------------------------------------

void cGELPlaydoh::setResizePercentage(double a_resizePercentage)
{
	// set resize percentage
	m_resizePercentage = a_resizePercentage;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::setMode(int a_mode)
{
	// reset modes
	m_standbyMode = false;
	m_patrolMode = false;
	m_cordonMode = false;

	// set appropriate mode
	if (a_mode == -1)
	{
		m_standbyMode = true;
	}
	else if (a_mode == 0)
	{
		m_patrolMode = true;
	}
	else if (a_mode == 1)
	{
		m_cordonMode = true;
	}

	// set member mode
	m_mode = a_mode;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::createStraightNodes(vector<cVector3d> a_robotPositions)
{
	int size = a_robotPositions.size();
	for (int i = 0; i < size; i++)
	{
		// define desired vector locations
		unsigned int current = i;
		unsigned int next = (i + 1) % size;

		// set starting and ending positions
		cVector3d p1 = a_robotPositions[current];
		cVector3d p2 = a_robotPositions[next];

		// vector from robot1 to robot2
		cVector3d diff12 = p2 - p1;
		cVector3d norm12 = cNormalize(diff12);
		double distance = diff12.length();

		// define number of spheres
		int numSpheres = (int)(diff12.length() / m_sphereSpacing);

		// add spheres between p1 and p2
		for (int j = 0; j < numSpheres; j++)
		{
			// create sphere and add it to the GELPlaydoh's world
			cGELPlaydohSkeletonNode* sphere = new cGELPlaydohSkeletonNode();

			cVector3d result = p1 + (double)j / (double)(numSpheres)*distance*norm12;
			sphere->m_pos = result;

			// initialize entry vector (for building boundary forces)
			sphere->m_entryVec.zero();
			sphere->m_insideBldg = true;

			// set color
			if(m_cordonMode)
				sphere->m_color = m_cordonNodeColor;
			else if (m_patrolMode)
			{
				sphere->m_color = m_patrolNodeColor;
			}

			// set the first sphere to be at robot location
			if (j == 0)
			{
				sphere->m_atRobot = true;
				//sphere->m_fixed = true;
				//sphere->m_color.set(0.5, 0.0, 0.0, 1.0f);

				// store the indexed location in m_robotPositions for this node
				// and add to the end of the robot spheres list
				if (m_cordonMode)
				{
					sphere->m_IdNum = i;
					m_robotCordonSpheres.push_back(sphere);
				}
				else if (m_patrolMode)
				{
					sphere->m_IdNum = m_convexHullMapping[i];
					m_robotPatrolSpheres.push_back(sphere);
				}

			}

			//// set frame size for node
			//sphere->setFrameScale(1.0);

			if (m_cordonMode)
			{
				// add this sphere to the list of cordon playdoh spheres
				m_cordonSpheres.push_back(sphere);

				// add this sphere to the list of cordon GEL mesh nodes
				m_cordonMesh->m_nodes.push_back(sphere);
			}
			else if (m_patrolMode)
			{
				// add this sphere to the list of patrol playdoh spheres
				m_patrolSpheres.push_back(sphere);

				// add this sphere to the list of patrol GEL mesh nodes
				m_patrolMesh->m_nodes.push_back(sphere);
			}

		}

	}

	// initialize perimeter
	initPerimeter();

	// enabled haptic effect
	//m_hapticEnabled = true;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::initEntryVecs()
{
	// compute center of mass
	cVector3d com(0, 0, 0);
	int numSpheres = m_cordonSpheres.size();
	for (int i = 0; i < numSpheres; i++)
	{
		com.add(m_cordonSpheres[i]->m_pos);
	}
	com.div((double)numSpheres);

	// assign entryVec to be toward the center of mass for each node
	for (int i = 0; i < numSpheres; i++)
	{
		m_cordonSpheres[i]->m_entryVec = cNormalize(com - m_cordonSpheres[i]->m_pos);
	}
}

//---------------------------------------------------------------------------

void cGELPlaydoh::createCurvedNodes(vector<cVector3d> a_robotPositions)
{
	// local robot spheres list
	list<cGELPlaydohSkeletonNode*> a_robotSpheres;

	int size = a_robotPositions.size();
	for (int i = 0; i < size; i++)
	{

		// define desired vector locations
		unsigned int current = i;
		unsigned int next = (i + 1) % size;
		unsigned int previous = (i + size - 2) % size;
		unsigned int future = (i + 2) % size;

		// set previous position
		cVector3d p0 = a_robotPositions[previous];

		// set starting and ending positions
		cVector3d p1 = a_robotPositions[current];
		cVector3d p2 = a_robotPositions[next];

		// set future position
		cVector3d p3 = a_robotPositions[future];

		// vector from robot1 to robot2
		cVector3d diff12 = p2 - p1;
		cVector3d norm12 = cNormalize(diff12);
		double distance = diff12.length();

		// normal vectors for curving
		cVector3d norm01 = p1 - p0;
		norm01.normalize();
		cVector3d norm32 = p2 - p3;
		norm32.normalize();

		// define number of spheres
		int numSpheres = (int)(diff12.length() / m_sphereSpacing);

		// add spheres between p1 and p2
		for (int j = 0; j < numSpheres; j++)
		{
			// create sphere and add it to the GELPlaydoh's world
			cGELPlaydohSkeletonNode* sphere = new cGELPlaydohSkeletonNode();
			//m_world->addChild(sphere);

			// set position of sphere along curved path between robot points
			cVector3d curveVec = norm12 + (double)(numSpheres - j) / (double)(numSpheres)*(norm01 + norm32);
			curveVec.normalize();
			//cVector3d curveVec2 = -1*norm12 + (double)(numSpheres - i) / (double)(numSpheres)*norm32;
			//curveVec2.normalize();

			cVector3d result = p1 + (double)j / (double)(numSpheres)*distance*curveVec;
			//cVector3d result2 = a_p2 + (double)i / (double)(numSpheres)*distance*curveVec2;
			sphere->m_pos = result;

			// set color
			if(m_cordonMode)
				sphere->m_color = m_cordonNodeColor;
			else if (m_patrolMode)
			{
				sphere->m_color = m_patrolNodeColor;
			}

			// set the first sphere to be at robot location
			if (j == 0)
			{
				sphere->m_atRobot = true;
				//sphere->m_fixed = true;
				//sphere->m_color.set(0.5, 0.0, 0.0, 1.0f);

				// store the indexed location in m_robotPositions for this node
				// and add to the end of the robot spheres list
				if (m_cordonMode)
				{
					sphere->m_IdNum = i;
					m_robotCordonSpheres.push_back(sphere);
				}
				else if (m_patrolMode)
				{
					sphere->m_IdNum = m_convexHullMapping[i];
					m_robotPatrolSpheres.push_back(sphere);
				}

			}

			// set frame size for node
			sphere->setFrameScale(1.0);

			if (m_cordonMode)
			{
				// add this sphere to the list of cordon playdoh spheres
				m_cordonSpheres.push_back(sphere);

				// add this sphere to the list of cordon GEL mesh nodes
				m_cordonMesh->m_nodes.push_back(sphere);
			}
			else if (m_patrolMode)
			{
				// add this sphere to the list of patrol playdoh spheres
				m_patrolSpheres.push_back(sphere);

				// add this sphere to the list of patrol GEL mesh nodes
				m_patrolMesh->m_nodes.push_back(sphere);
			}

		}

	}

	// initialize perimeter
	initPerimeter();

	// enabled haptic effect
	//m_hapticEnabled = true;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::createSplinedNodes(vector<cVector3d> a_robotPositions)
{
	int size = a_robotPositions.size();
	for (int i = 0; i < size; i++)
	{

		// define desired vector locations
		unsigned int current = i;
		unsigned int next = (i + 1) % size;
		unsigned int previous = (i + size - 2) % size;
		unsigned int future = (i + 2) % size;

		// set previous position
		cVector3d p0 = a_robotPositions[previous];

		// set starting and ending positions
		cVector3d p1 = a_robotPositions[current];
		cVector3d p2 = a_robotPositions[next];

		// set future position
		cVector3d p3 = a_robotPositions[future];

		// vector from robot1 to robot2
		cVector3d diff12 = p2 - p1;
		cVector3d norm12 = cNormalize(diff12);
		double distance = diff12.length();

		// define number of spheres
		int numSpheres = (int)(diff12.length() / m_sphereSpacing);

		// add spheres between p1 and p2
		for (int j = 0; j < numSpheres; j++)
		{
			// create sphere and add it to the GELPlaydoh's world
			cGELPlaydohSkeletonNode* sphere = new cGELPlaydohSkeletonNode();

			// calculate Catmull Rom spline
			double x = (double)i / (double)(numSpheres);
			cVector3d q0 = (x*((2 - x)*x - 1))*p0;
			cVector3d q1 = (x*x*(3*x - 5) + 2)*p1;
			cVector3d q2 = (x*((4 - 3*x) + 1))*p2;
			cVector3d q3 = ((x-1)*x*x)*p3;
			cVector3d q = 0.5 * (q0 + q1 + q2 + q3);

			// set sphere position
			sphere->m_pos = q;

			// set color
			if(m_cordonMode)
				sphere->m_color = m_cordonNodeColor;
			else if (m_patrolMode)
			{
				sphere->m_color = m_patrolNodeColor;
			}

			// set the first sphere to be at robot location
			if (j == 0)
			{
				sphere->m_atRobot = true;
				//sphere->m_fixed = true;
				//sphere->m_color.set(0.5, 0.0, 0.0, 1.0f);


				if (m_cordonMode)
				{
					sphere->m_IdNum = i;
					m_robotCordonSpheres.push_back(sphere);
				}
				else if (m_patrolMode)
				{
					sphere->m_IdNum = m_convexHullMapping[i];
					m_robotPatrolSpheres.push_back(sphere);
				}

			}

			// set frame size for node
			sphere->setFrameScale(1.0);

			if (m_cordonMode)
			{
				// add this sphere to the list of cordon playdoh spheres
				m_cordonSpheres.push_back(sphere);

				// add this sphere to the list of cordon GEL mesh nodes
				m_cordonMesh->m_nodes.push_back(sphere);
			}
			else if (m_patrolMode)
			{
				// add this sphere to the list of patrol playdoh spheres
				m_patrolSpheres.push_back(sphere);

				// add this sphere to the list of patrol GEL mesh nodes
				m_patrolMesh->m_nodes.push_back(sphere);
			}

		}

	}

	// initialize perimeter
	initPerimeter();

	// enabled haptic effect
	//m_hapticEnabled = true;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::createNodeLinks()
{
	vector<cGELPlaydohSkeletonNode*> a_spheres;
	cGELPlaydohMesh* a_mesh;
	if (m_cordonMode)
	{
		a_spheres = m_cordonSpheres;
		a_mesh = m_cordonMesh;
	}
	else if (m_patrolMode)
	{
		a_spheres = m_patrolSpheres;
		a_mesh = m_patrolMesh;
	}


	cGELPlaydohSkeletonNode* current;
	cGELPlaydohSkeletonNode* next;
	vector<cGELPlaydohSkeletonNode*>::iterator iter;
	vector<cGELPlaydohSkeletonNode*>::iterator iterNext;
	for (iter = a_spheres.begin(); iter != a_spheres.end(); iter++)
	{
		// dereference iterator
		current = *iter;

		// get next element in list
		iterNext = iter;
		if ((iterNext != a_spheres.end()) && ((++iterNext) == a_spheres.end()))
		{
			// dereference iterator
			next = *(a_spheres.begin());
		}
		else
		{
			// dereference iterator
			next = *iterNext;
		}

		// create links
		cGELPlaydohSkeletonLink* newLink = new cGELPlaydohSkeletonLink(current, next);

		// check if link connects to robot node
		if (current->m_atRobot || next->m_atRobot)
		{
			// make spring flexion zero 
			newLink->m_kSpringFlexion = 0.0;
		}

		// add link to mesh
		a_mesh->m_links.push_back(newLink);
	}
}

void cGELPlaydoh::deleteNodeLinks()
{
	if (m_cordonMode)
	{
		// deallocate memory to each link in playdoh mesh
		cGELPlaydohSkeletonLink* tempLink;
		list<cGELPlaydohSkeletonLink*>::iterator link;
		for (link = m_cordonMesh->m_links.begin(); link != m_cordonMesh->m_links.end(); link++)
		{
			// dereference iterator
			tempLink = *link;

			// Deallocate memory
			delete tempLink;
			tempLink = NULL;
		}

		// clear the list of links in playdoh mesh
		m_cordonMesh->m_links.clear();

	}
	else if (m_patrolMode)
	{
		// deallocate memory to each link in playdoh mesh
		cGELPlaydohSkeletonLink* tempLink;
		list<cGELPlaydohSkeletonLink*>::iterator link;
		for (link = m_patrolMesh->m_links.begin(); link != m_patrolMesh->m_links.end(); link++)
		{
			// dereference iterator
			tempLink = *link;

			// Deallocate memory
			delete tempLink;
			tempLink = NULL;
		}

		// clear the list of links in playdoh mesh
		m_patrolMesh->m_links.clear();
	}
}

//---------------------------------------------------------------------------

void cGELPlaydoh::initPatrolParams()
{
	// compute playdoh radius scale parameter
	double perimeter = 0;
	int numCHRobotPositions = m_convexHullRobotPositions.size();
	for (int i = 0; i < numCHRobotPositions; i++)
	{
		int next;
		if (i < numCHRobotPositions - 1)
		{
			next = i + 1;
		}
		else
		{
			next = 0;
		}
		perimeter += (m_convexHullRobotPositions[next] - m_convexHullRobotPositions[i]).length();
	}

	// set robot perimeter
	m_robotPerimeter = perimeter;

	// calculate workspace radius based on perimeter
	double playdohWorkspaceRadius = perimeter / (2 * C_PI);

	// set default sphere spacing
	m_defaultRadiusToSpacingRatio = 4;

	m_workspaceScale = perimeter;

	// set default sphere radius (based on approx playdoh radius)
	// and clamp to minimum and maximum radius
	m_defaultSphereRadius = cClamp(0.20*playdohWorkspaceRadius, m_toolRadius, 2 * m_toolRadius);

	// set sphere spacing
	m_sphereSpacing = m_defaultSphereRadius / m_defaultRadiusToSpacingRatio;

	// setup default values for nodes
	cGELPlaydohSkeletonNode::s_default_radius = m_defaultSphereRadius;
	cGELPlaydohSkeletonNode::s_default_kDampingPos = 10;
	cGELPlaydohSkeletonNode::s_default_kDampingRot = 0.5;
	cGELPlaydohSkeletonNode::s_default_muStaticFrictionPos = 0.5;
	cGELPlaydohSkeletonNode::s_default_muStaticFrictionRot = 0.5;
	cGELPlaydohSkeletonNode::s_default_muDynamicFrictionPos = 0.2;
	cGELPlaydohSkeletonNode::s_default_muDynamicFrictionRot = 0.2;
	cGELPlaydohSkeletonNode::s_default_mass = 0.0001;  // [kg]
	cGELPlaydohSkeletonNode::s_default_showFrame = false;
	//cGELPlaydohSkeletonNode::s_default_color.set(1.0, 0.6, 0.6);
	cGELPlaydohSkeletonNode::s_default_useGravity = true;
	cGELPlaydohSkeletonNode::s_default_gravity.set(0.00, 0.00, -9.81);
	//cGELPlaydohSkeletonNode::s_default_stiffness = 0.20 / m_workspaceScale; // [N/m]
	cGELPlaydohSkeletonNode::s_default_stiffness = 0.4 * (1 / m_defaultRadiusToSpacingRatio) * m_maxNodeStiffness; // [N/m]

	// set max stiffness
	if (cGELPlaydohSkeletonNode::s_default_stiffness > m_maxNodeStiffness)
	{
		cGELPlaydohSkeletonNode::s_default_stiffness = m_maxNodeStiffness;
	}

	// set inner patrol stiffness
	m_patrolStiffness = cGELPlaydohSkeletonNode::s_default_stiffness * m_defaultRadiusToSpacingRatio;

	// set max patrol stiffness
	if (m_patrolStiffness > m_maxNodeStiffness)
	{
		m_patrolStiffness = m_maxNodeStiffness;
	}

	//std::cout << "patrol stiffness: " << cGELPlaydohSkeletonNode::s_default_stiffness << std::endl;

	// setup default values for links
	cGELPlaydohSkeletonLink::s_default_kSpringElongation = 10000.0 / m_toolWorkspaceScaleFactor; // [N/m]
	cGELPlaydohSkeletonLink::s_default_kSpringFlexion = 5.0;   // [Nm/RAD]
	cGELPlaydohSkeletonLink::s_default_kSpringTorsion = 0.0;   // [Nm/RAD]
	//cGELPlaydohSkeletonLink::s_default_color = m_patrolNodeColor;

	// set patrol height
	m_patrolHeight = 1.0*m_defaultSphereRadius;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::initPatrol()
{
	//std::cout << "initPatrol called" << std::endl;

	//// delete patrol first
	deletePatrol();

	// initialize parameters
	initPatrolParams();

	//// add mesh to deformable world meshes
	//m_defWorld->m_gelMeshes.push_back(m_patrolMesh);

	// set static friction radius
	m_staticFrictionR = 1.0;

	// set default playdoh stiffness
	m_default_kSpringElongation = cGELPlaydohSkeletonLink::s_default_kSpringElongation;

	// set playdoh stiffness
	m_kSpringElongation = m_default_kSpringElongation;

	// zero playdoh forces
	m_playdohForce.zero();
	m_prevPlaydohForce.zero();

	// haptic enabled
	m_affectHapticChildren = true;
	//m_hapticEnabled = false;

	// create nodes connecting robot along convex hull
	//createSplinedNodes(m_convexHullRobotPositions);
	vector<cVector3d> positions = m_convexHullRobotPositions;
	positions.pop_back();
	createStraightNodes(positions);

	// create links between each node
	if(m_usePatrolLinks)
		createNodeLinks();

	// create remaining nodes for robots inside convex hull??


	// show playdoh mesh
	m_patrolMesh->m_useSkeletonModel = true;
	m_patrolMesh->m_showSkeletonModel = true;
	m_patrolMesh->setShowEnabled(m_showEnabled, true);
}

void cGELPlaydoh::initPatrol(bool a_show)
{
	//// delete patrol first
	deletePatrol();

	//// add mesh to deformable world meshes
	//m_defWorld->m_gelMeshes.push_back(m_patrolMesh);

	// set default sphere spacing
	m_defaultRadiusToSpacingRatio = 4;

	// set default sphere radius
	m_defaultSphereRadius = 0.25;

	// set sphere spacing
	m_sphereSpacing = m_defaultSphereRadius / m_defaultRadiusToSpacingRatio;


	// set static friction radius
	m_staticFrictionR = 1.0;


	// set default playdoh stiffness
	m_default_kSpringElongation = cGELPlaydohSkeletonLink::s_default_kSpringElongation;

	// set playdoh stiffness
	m_kSpringElongation = m_default_kSpringElongation;

	// zero playdoh forces
	m_playdohForce.zero();
	m_prevPlaydohForce.zero();

	// haptic enabled
	m_affectHapticChildren = true;
	//m_hapticEnabled = false;

	// create nodes connecting robot along convex hull
	//createSplinedNodes(m_convexHullRobotPositions);
	vector<cVector3d> positions = m_convexHullRobotPositions;
	positions.pop_back();
	createStraightNodes(positions);

	// create links between each node
	if (m_usePatrolLinks)
		createNodeLinks();

	// show playdoh mesh
	m_patrolMesh->m_useSkeletonModel = true;
	m_patrolMesh->m_showSkeletonModel = a_show;
	m_patrolMesh->setShowEnabled(a_show, true);
}

//---------------------------------------------------------------------------

void cGELPlaydoh::deletePatrol()
{
	// disable visibility of node model
	m_patrolMesh->m_useSkeletonModel = false;
	m_patrolMesh->m_showSkeletonModel = false;

	// deallocate memory to each link in playdoh mesh
	cGELPlaydohSkeletonLink* tempLink;
	list<cGELPlaydohSkeletonLink*>::iterator link;
	for (link = m_patrolMesh->m_links.begin(); link != m_patrolMesh->m_links.end(); link++)
	{
		// dereference iterator
		tempLink = *link;

		// Deallocate memory
		delete tempLink;
		tempLink = NULL;
	}

	// clear the list of links in playdoh mesh
	m_patrolMesh->m_links.clear();

	// deallocate memory to each node in playdoh mesh
	cGELPlaydohSkeletonNode* tempNode;
	list<cGELPlaydohSkeletonNode*>::iterator node;
	for (node = m_patrolMesh->m_nodes.begin(); node != m_patrolMesh->m_nodes.end(); node++)
	{
		// dereference iterator
		tempNode = *node;

		// Deallocate memory
		delete tempNode;
		tempNode = NULL;
	}

	// clear the list of nodes in playdoh mesh
	m_patrolMesh->m_nodes.clear();

	//// remove playdoh mesh from deformable world mesh
	//m_defWorld->m_gelMeshes.clear();

	// clear list of playdoh spheres
	m_patrolSpheres.clear();

	// clear list of robot playdoh spheres
	m_robotPatrolSpheres.clear();
}

//---------------------------------------------------------------------------

void cGELPlaydoh::initCordonParams()
{
	// compute playdoh radius scale parameter
	double perimeter = 0;
	int numRobotPositions = m_robotPositions.size();
	for (int i = 0; i < numRobotPositions; i++)
	{
		int next;
		if (i < numRobotPositions - 1)
		{
			next = i + 1;
		}
		else
		{
			next = 0;
		}
		perimeter += (m_robotPositions[next] - m_robotPositions[i]).length();
	}

	// set robot perimeter
	m_robotPerimeter = perimeter;

	// calculate workspace radius based on perimeter
	double playdohWorkspaceRadius = perimeter / (2 * C_PI);

	// calculate average spacing
	m_uniformRobotSpacing = perimeter / m_robotPositions.size();

	// initialize spacing factors
	m_spacingFactors.clear();
	for (int i = 0; i < numRobotPositions; i++)
	{
		m_spacingFactors.push_back(1.0);
	}

	// set default sphere spacing
	m_defaultRadiusToSpacingRatio = 4;

	m_workspaceScale = perimeter;

	// set default sphere radius (based on approx playdoh radius)
	// and clamp to minimum and maximum radius
	m_defaultSphereRadius = cClamp(0.20*playdohWorkspaceRadius, m_toolRadius, 2 * m_toolRadius);

	// set sphere spacing
	m_sphereSpacing = m_defaultSphereRadius / m_defaultRadiusToSpacingRatio;

	// setup default values for nodes
	cGELPlaydohSkeletonNode::s_default_radius = m_defaultSphereRadius;
	cGELPlaydohSkeletonNode::s_default_kDampingPos = 0.6 * m_workspaceScale; // (hcval)
	cGELPlaydohSkeletonNode::s_default_kDampingRot = 0.5 * m_workspaceScale;

	// static friction coefficient
	cGELPlaydohSkeletonNode::s_default_muStaticFrictionPos = 0.10;

	cGELPlaydohSkeletonNode::s_default_muDynamicFrictionPos = 0.0;
	cGELPlaydohSkeletonNode::s_default_muStaticFrictionRot = 0.50;
	cGELPlaydohSkeletonNode::s_default_muDynamicFrictionRot = 0.2;
	cGELPlaydohSkeletonNode::s_default_mass = 0.002 / m_workspaceScale;  // [kg]
	cGELPlaydohSkeletonNode::s_default_showFrame = false;
	//cGELPlaydohSkeletonNode::s_default_color.set(1.0, 0.6, 0.6);
	cGELPlaydohSkeletonNode::s_default_useGravity = true;
	cGELPlaydohSkeletonNode::s_default_gravity.set(0.00, 0.00, -9.81*m_toolWorkspaceScaleFactor); // [m / s^2]
	//cGELPlaydohSkeletonNode::s_default_stiffness = 0.50/m_workspaceScale; // [N/m]
	cGELPlaydohSkeletonNode::s_default_stiffness = 0.45 * (1 / m_defaultRadiusToSpacingRatio) * m_maxNodeStiffness; // [N/m]
	m_cordonNodeStiffness = cGELPlaydohSkeletonNode::s_default_stiffness;

	// set max stiffness
	if (cGELPlaydohSkeletonNode::s_default_stiffness > m_maxNodeStiffness)
	{
		cGELPlaydohSkeletonNode::s_default_stiffness = m_maxNodeStiffness;
	}

	// setup default values for links
	cGELPlaydohSkeletonLink::s_default_kSpringElongation = 2000.0 / m_toolWorkspaceScaleFactor; // [N/m] to [N/vir] conversion with factor
	cGELPlaydohSkeletonLink::s_default_kSpringFlexion = 2.0;   // [Nm/RAD]
	cGELPlaydohSkeletonLink::s_default_kSpringTorsion = 0.0;   // [Nm/RAD]
	cGELPlaydohSkeletonLink::s_default_color = m_cordonNodeColor;

	// set cordon height	
	m_cordonHeight = 1.0*m_defaultSphereRadius;

	// reset resize percentage
	m_resizePercentage = 1.00;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::initCordon()
{
	// delete cordon first
	deleteCordon();

	// initialize parameters
	initCordonParams();

	// add mesh to deformable world meshes
	//m_defWorld->m_gelMeshes.push_back(m_cordonMesh);

	// zero perimeter
	m_perimeter = 0;

	// zero large perimeter
	m_largePerimeter = 0;

	// set static friction radius
	m_staticFrictionR = 1.0;

	// zero geometry perimeter
	m_geomPerimeter = 0;

	// set default playdoh stiffness
	m_default_kSpringElongation = cGELPlaydohSkeletonLink::s_default_kSpringElongation;

	// set playdoh stiffness
	m_kSpringElongation = m_default_kSpringElongation;

	// zero playdoh forces
	m_playdohForce.zero();
	m_prevPlaydohForce.zero();

	// haptic enabled
	m_affectHapticChildren = true;
	//m_hapticEnabled = false;

	// create nodes connecting robots
	//createSplinedNodes(m_robotPositions);
	createStraightNodes(m_robotPositions);

	// create links between each node
	createNodeLinks();

	// initialize entry vectors in cordon mode
	initEntryVecs();

	// show playdoh mesh
	m_cordonMesh->m_useSkeletonModel = true;
	m_cordonMesh->m_showSkeletonModel = true;
	m_cordonMesh->setShowEnabled(m_showEnabled, true);
}

//---------------------------------------------------------------------------

void cGELPlaydoh::deleteCordon()
{
	// disable visibility of node model
	m_cordonMesh->m_useSkeletonModel = false;
	m_cordonMesh->m_showSkeletonModel = false;

	// deallocate memory to each link in playdoh mesh
	cGELPlaydohSkeletonLink* tempLink;
	list<cGELPlaydohSkeletonLink*>::iterator link;
	for (link = m_cordonMesh->m_links.begin(); link != m_cordonMesh->m_links.end(); link++)
	{
		// dereference iterator
		tempLink = *link;

		// Deallocate memory
		delete tempLink;
		tempLink = NULL;
	}

	// clear the list of links in playdoh mesh
	m_cordonMesh->m_links.clear();

	// deallocate memory to each node in playdoh mesh
	cGELPlaydohSkeletonNode* tempNode;
	list<cGELPlaydohSkeletonNode*>::iterator node;
	for (node = m_cordonMesh->m_nodes.begin(); node != m_cordonMesh->m_nodes.end(); node++)
	{
		// dereference iterator
		tempNode = *node;

		// Deallocate memory
		delete tempNode;
		tempNode = NULL;
	}

	// clear the list of nodes in playdoh mesh
	m_cordonMesh->m_nodes.clear();
	
	//// remove playdoh mesh from deformable world mesh
	//m_defWorld->m_gelMeshes.clear();

	// clear list of playdoh spheres
	m_cordonSpheres.clear();

	// clear list of robot playdoh spheres
	m_robotCordonSpheres.clear();

	// zero perimeter
	m_perimeter = 0;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::initPerimeter(){

	// initialize playdoh perimeter
	cGELPlaydohSkeletonNode* current;
	cGELPlaydohSkeletonNode* next;
	vector<cGELPlaydohSkeletonNode*>::iterator iter;
	vector<cGELPlaydohSkeletonNode*>::iterator iterNext;
	for (iter = m_cordonSpheres.begin(); iter != m_cordonSpheres.end(); iter++)
	{
		// set current sphere
		current = *iter;

		// set next sphere
		iterNext = iter;
		if ((iterNext != m_cordonSpheres.end()) && ((++iterNext) == m_cordonSpheres.end()))
		{
			next = *(m_cordonSpheres.begin());
		}
		else
		{
			next = *iterNext;
		}

		// calculate positions
		cVector3d currentPos = current->m_pos;
		cVector3d nextPos = next->m_pos;
		cVector3d diff = nextPos - currentPos;
		m_perimeter = m_perimeter + diff.length();
	}

	// set geometry perimeter as initial playdoh perimeter
	m_geomPerimeter = m_perimeter;

	// set large perimeter as twice that of the geom perimeter
	m_largePerimeter = 2 * m_geomPerimeter;
}

//---------------------------------------------------------------------------

void cGELPlaydoh::resizeCordon()
{
	// delete cordon node links
	//deleteNodeLinks();

	//// create static mesh to show original size before resize
	if (m_resizeStart)
	{
		m_cordonStaticResizeMesh->m_useSkeletonModel = true;
		m_cordonStaticResizeMesh->m_showSkeletonModel = true;
		m_cordonStaticResizeMesh->setMutex(m_mutex);
		m_cordonStaticResizeMesh->m_enableMutex = false;
	}

	// object pointers
	cGELPlaydohSkeletonNode* current;
	cGELPlaydohSkeletonNode* next;
	cGELPlaydohSkeletonNode* prev;

	// iterators
	vector<cGELPlaydohSkeletonNode*>::iterator iter;
	vector<cGELPlaydohSkeletonNode*>::iterator iterNext;
	vector<cGELPlaydohSkeletonNode*>::iterator iterPrev;

	// set local spheres and mesh variables
	vector<cGELPlaydohSkeletonNode*> a_spheres;
	cGELPlaydohMesh* a_mesh;
	if (m_cordonMode)
	{
		a_spheres = m_cordonSpheres;
		a_mesh = m_cordonMesh;
	}
	else if (m_patrolMode)
	{
		a_spheres = m_patrolSpheres;
		a_mesh = m_patrolMesh;
	}

	int robotIndex = 0;
	int numRobots = m_robotPositions.size();
	if (!a_spheres.empty())
	{
		// starting iterator values
		iterPrev = a_spheres.end();
		--iterPrev;
		prev = *iterPrev;

		cVector3d xyzBar;
		xyzBar.zero();
		double totalMass = 0;
		for (iter = a_spheres.begin(); iter != a_spheres.end(); iter++)
		{
			// set current sphere
			current = *iter;

			// set next sphere
			iterNext = iter;
			if ((iterNext != a_spheres.end()) && ((++iterNext) == a_spheres.end()))
			{
				next = *(a_spheres.begin());
			}
			else
			{
				next = *iterNext;
			}

			// set prev sphere
			if (iter == a_spheres.begin())
			{
				prev = a_spheres.back();
			}

			if (m_resizeStart)
			{
				m_cordonStaticResizeMesh->m_showSkeletonModel = m_showEnabled;
				cGELPlaydohSkeletonNode* sphere = new cGELPlaydohSkeletonNode();
				sphere->m_pos = current->m_pos;
				sphere->m_radius = current->m_radius;
				sphere->m_fixed = true;
				m_cordonStaticResizeMesh->m_nodes.push_back(sphere);
				sphere->m_color.set(0.0, 0.0, 0.5, 1.0f);
			}

			// calculate mass times x and y
			double mass = current->m_mass;
			xyzBar.x(xyzBar.x() + mass * current->m_pos.x());
			xyzBar.y(xyzBar.y() + mass * current->m_pos.y());
			xyzBar.z(xyzBar.z() + mass * current->m_pos.z());
			totalMass += mass;

			// resize this node from center of mass
			cVector3d pos = current->m_pos - m_centerOfMass;
			pos = m_resizePercentage*pos;
			current->m_pos = pos + m_centerOfMass;
			current->m_pos.z(0); // force the z to zero

			// resize node radius
			//current->m_radius = cClamp(m_resizePercentage * current->m_radius, m_toolRadius, 2 * m_toolRadius);

			// change color
			current->m_color.set(0.1f, 0.0f, 0.9f, 1.0f);

			// update robot positions variable
			if (current->m_atRobot && robotIndex < numRobots)
			{
				m_robotPositions[robotIndex] = current->m_pos;
				robotIndex++;
			}
			
			//-----------------------------------------------------------------------
			// increment variables for next iteration
			//-----------------------------------------------------------------------

			// store previous iterators for next iterate
			iterPrev = iter;

			// store previous sphere for next iterate
			prev = current;
		}
		
		// calculate center of mass (uncomment if center of mass is changing during resize)
		//m_centerOfMass = xyzBar / totalMass;
	}

	if (m_resizeStart)
	{
		m_cordonStaticResizeMesh->setRenderNodes();
		m_resizeStart = false;
	}
}

//---------------------------------------------------------------------------

void cGELPlaydoh::updateSpacingFactors(int a_mod1, double a_modFactor)
{
	int N = m_robotCordonSpheres.size();
	int curIndex = a_mod1;
	int incEnd = N / 2;
	for (int i = 0; i < N; i++)
	{
		// ensure bounds are met
		curIndex = curIndex % N;

		// compute max factor
		double M = N;
		double maxFactor = 2 - a_modFactor;

		// set spacing factor
		m_spacingFactors[curIndex] = computeLinearSpacingFactor(a_modFactor, maxFactor, N / 2, i % (N / 2), (i < incEnd));

		// increment
		curIndex++;
	}
	//cout << "spacingFactors: ";
	//double sum = 0;
	//for (int i = 0; i < N; i++)
	//{
	//	cout << m_spacingFactors[i] << " ";
	//	sum += m_spacingFactors[i];
	//}
	//cout << endl;
	//cout << "spacingFactors sum: " << sum << endl;
}

void cGELPlaydoh::resetSpacingFactors()
{
	updateSpacingFactors(0, 1);
}

double cGELPlaydoh::computeLinearSpacingFactor(double a_minSpacing, double a_maxSpacing, int a_numRegions, int a_regionNum, bool a_increasing)
{
	if (a_increasing)
		return cClamp(a_minSpacing + (a_maxSpacing - a_minSpacing) / a_numRegions * a_regionNum, a_minSpacing, a_maxSpacing);
	else return cClamp(a_maxSpacing - (a_maxSpacing - a_minSpacing) / a_numRegions * a_regionNum, a_minSpacing, a_maxSpacing);
}

void cGELPlaydoh::startTransition()
{
	if (!m_transition)
	{
		m_transition = true;
		m_transitionTimer.reset();
		m_transitionTimer.start();
		m_transitionForce = m_toolForce;
	}
}

void cGELPlaydoh::endTransition()
{
	m_transitionTimer.stop();
	m_transition = false;
	m_prevTransition = false;
	m_transitionForce.zero();
}