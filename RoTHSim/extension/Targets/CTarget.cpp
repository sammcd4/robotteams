#include "CTarget.h"

cTarget::cTarget()
{
	initTarget();
}

cTarget::cTarget(const double& a_radius)
{
	initTarget();

	// create sphere
	//m_visual = new cShapeSphere(a_radius);
	m_visual = new cShapeBox(1.5*a_radius, 1.5*a_radius, 1.5 * a_radius);
	m_visual->setLocalPos(0, 0, 0);
	m_visual->setShowEnabled(m_active);
	this->addChild(m_visual);

	// flag for sphere active
	m_visualActive = true;
}

cTarget::cTarget(const double& a_radius, double a_startTime, double a_endTime)
{
	initTarget();

	// create sphere
	//m_visual = new cShapeSphere(a_radius);
	m_visual = new cShapeBox(1.5*a_radius, 1.5*a_radius, 1.5 * a_radius);
	m_visual->setLocalPos(0, 0, 0);
	m_visual->setShowEnabled(m_active);
	this->addChild(m_visual);

	// flag for sphere active
	m_visualActive = true;

	// set start and end times
	setStartAndEndTimes(a_startTime, a_endTime);
}

cTarget::~cTarget()
{
	deallocate();
}

void cTarget::initTarget()
{
	// set default found state
	m_found = false;

	// set active state
	m_active = false;

	// default sphere activity
	m_visualActive = false;

	// default start time
	m_startTime = 0;

	// default end time
	m_endTime = C_LARGE;

	// default passed state
	m_passed = false;

	m_detectionCounter = 0;
	m_maxDetectionCount = 40;
	m_groupFound = false;

	m_minDetectionToFound = 6;
}

void cTarget::setFoundState(bool a_foundState)
{ 
	m_found = a_foundState;
}

void cTarget::setGroupFoundState(bool a_groupFoundState)
{
	m_groupFound = a_groupFoundState;
}

void cTarget::updateFoundCounter(bool a_trueDetection)
{
	if (!m_found && !m_groupFound)
	{
		// increment counter if true detection
		if (a_trueDetection)
		{
			m_detectionCounter++;
		}
	}
}

bool cTarget::isDetected()
{
	return (m_detectionCounter > 0);
}

bool cTarget::isSemiFound()
{
	return (m_detectionCounter >= m_minDetectionToFound);
}

void cTarget::resetDetectionCounter()
{
	m_detectionCounter = 0;
}

void cTarget::setColor(cColorf a_color, bool a_show3D)
{
	if (m_visualActive)
	{
		// set ambient color
		m_visual->m_material->setColorf(a_color.getR(),
										a_color.getG(),
										a_color.getB(),
										a_color.getA());
		if (!a_show3D)
		{
			m_visual->m_material->m_emission = cColorf(0, 0, 0);
			m_visual->m_material->m_specular = cColorf(0, 0, 0);
		}
	}
}

bool cTarget::deleteAndNullChild(cGenericObject* a_object)
{
	// sanity check
	if (a_object == NULL) { return (false); }

	// remove object from list
	bool result = removeChild(a_object);

	// if operation succeeds, delete the object
	if (result)
	{
		delete (a_object);
		a_object = NULL;
	}

	// return result
	return result;
}

void cTarget::deallocate()
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

void cTarget::setActiveState(bool a_active)
{
	m_active = a_active;
}

void cTarget::setStartTime(double a_startTime)
{
	m_startTime = a_startTime;
}

void cTarget::setEndTime(double a_endTime)
{
	m_endTime = a_endTime;
}

void cTarget::setStartAndEndTimes(double a_startTime, double a_endTime)
{
	m_startTime = a_startTime;
	m_endTime = a_endTime;
}

void cTarget::updateActiveState(double a_simTime)
{
	if (m_passed)
	{
		// do nothing (m_active = false);
	}
	else if (!m_active && a_simTime > m_startTime)
	{
		m_active = true;
		m_visual->setShowEnabled(m_active);
	}
	else if (m_active && a_simTime > m_endTime)
	{
		m_active = false;
		m_visual->setShowEnabled(m_active);
		m_passed = true;
	}
}
