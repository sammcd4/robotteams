//---------------------------------------------------------------------------
#include "system/CGlobals.h"
//---------------------------------------------------------------------------

#include <iostream>
#include <new>
#include <math.h>
#include "CGenericRobot.h"

// Initialize static variable
int cGenericRobot::numRobots = 0;

cGenericRobot::~cGenericRobot()
{
    // Decrement robot count
    numRobots--;

	// Deallocate memory
	deleteAndNullAllChildren();
}

//---------------------------------------------------------------------------

int cGenericRobot::getIdNumber()
{
    return m_idNumber;
}

//---------------------------------------------------------------------------

void cGenericRobot::setIdNumber(int a_idNumber)
{
	m_idNumber = a_idNumber;

	// also set value in the label for this id
	string str;
	str = "#" + to_string(m_idNumber);
	m_idLabel->setText(str);
}

//---------------------------------------------------------------------------

void cGenericRobot::setShowId(bool a_showId)
{ 
	if (m_showEnabled)
		m_showId = a_showId;
	else m_showId = false;

	m_idLabel->setShowEnabled(m_showId);
}

//---------------------------------------------------------------------------

int cGenericRobot::getNumRobots()
{
    return numRobots;
}

////---------------------------------------------------------------------------
//
//void cGenericRobot::setLocalPos(cVector3d a_pos)
//{ 
//	m_sphere->setLocalPos(a_pos);
//	if (m_useSensor)
//		setSensorPosition(a_pos);
//}

//---------------------------------------------------------------------------

double cGenericRobot::getMass()
{
	return m_mass;
}

//---------------------------------------------------------------------------

// get previous position
cVector3d cGenericRobot::getPrevPos()
{
	// return previous position stored in cGenericObject class
	return m_prevPos;
}

//---------------------------------------------------------------------------

// set previous position
void cGenericRobot::setPrevPos(cVector3d a_prevPos)
{
	m_prevPos = a_prevPos;
}

//---------------------------------------------------------------------------

void cGenericRobot::setRadius(double a_radius) 
{
	// set sphere radius
	m_sphere->setRadius(a_radius);

	if (m_useSensor)
	{
		// set sensor radius and position
		m_sensor->setLightRadius(m_sensorRadiusRatio*a_radius);
		m_sensor->setLocalPos(0, 0, m_sphere->getRadius());
	}
}

//---------------------------------------------------------------------------

// set previous position
void cGenericRobot::setPrevPos(double prev_x, double prev_y, double prev_z)
{
	m_prevPos.x(prev_x);
	m_prevPos.y(prev_y);
	m_prevPos.z(prev_z);
}

//---------------------------------------------------------------------------

void cGenericRobot::setVel(cVector3d& a_vel)
{
	// Set member velocity vector
    m_velocity = a_vel;

	// Set member speed scalar
	m_speed = m_velocity.length();
}

//---------------------------------------------------------------------------

void cGenericRobot::setVel(double x_vel, double y_vel, double z_vel)
{
	// Set member velocity vector
	m_velocity.x(x_vel);
	m_velocity.y(y_vel);
	m_velocity.z(z_vel);

	// Set member speed scalar
	m_speed = m_velocity.length();
}

//---------------------------------------------------------------------------

void cGenericRobot::setSpeed(double speed)
{
	// Set speed in the direction of current heading
	//m_velocity = speed*m_heading;

	//// Set member speed scalar
	//m_speed = speed;
}

//---------------------------------------------------------------------------

// Set linear acceleration vector
void cGenericRobot::setAcc(cVector3d& a_acc)
{
	m_acceleration = a_acc;
}

//---------------------------------------------------------------------------

// Set rotational velocity vector
void cGenericRobot::setRotVel(cVector3d& a_rotVel)
{
	m_rotVel = a_rotVel;
}

//---------------------------------------------------------------------------

// Set rotation velocity vector
void cGenericRobot::setRotVel(double x_rotVel, double y_rotVel, double z_rotVel)
{
	m_rotVel.x(x_rotVel);
	m_rotVel.y(y_rotVel);
	m_rotVel.z(z_rotVel);
}

//---------------------------------------------------------------------------

// Set linear acceleration vector
void cGenericRobot::setAcc(double x_acc, double y_acc, double z_acc)
{
	m_acceleration.x(x_acc);
	m_acceleration.y(y_acc);
	m_acceleration.z(z_acc);
}

//---------------------------------------------------------------------------

// Set rotational acceleration vector 
void cGenericRobot::setRotAcc(cVector3d& a_rotAcc)
{
	m_rotAcc = a_rotAcc;
}

//---------------------------------------------------------------------------

// Set rotation acceleration vector
void cGenericRobot::setRotAcc(double x_rotAcc, double y_rotAcc, double z_rotAcc)
{
	m_rotAcc.x(x_rotAcc);
	m_rotAcc.y(y_rotAcc);
	m_rotAcc.z(z_rotAcc);
}

//---------------------------------------------------------------------------

cVector3d cGenericRobot::getVel()
{
    return m_velocity;
}

//---------------------------------------------------------------------------

cVector3d cGenericRobot::getAcc()
{
	return m_acceleration;
}

//---------------------------------------------------------------------------

cVector3d cGenericRobot::getRotVel()
{
	return m_rotVel;
}

//---------------------------------------------------------------------------

cVector3d cGenericRobot::getRotAcc()
{
	return m_rotAcc;
}

//---------------------------------------------------------------------------

double cGenericRobot::getSpeed()
{
    return m_speed;
}

//---------------------------------------------------------------------------

cColorf cGenericRobot::getColor()
{
	return m_color;
}

//---------------------------------------------------------------------------

void cGenericRobot::setColor(cColorf a_color)
{
	m_color = a_color;
	m_sphere->m_material->m_ambient = m_color;
	m_sphere->m_material->m_diffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
	m_sphere->m_material->m_specular.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_sphere->m_material->m_emission.set(0.0f, 0.0f, 0.0f, 1.0f);
}

//---------------------------------------------------------------------------

//void cGenericRobot::setShowEnabled(bool a_showEnabled)
//{ 
//	m_showEnabled = a_showEnabled;
//	//m_sphere->setShowEnabled(a_showEnabled, true);
//	if (m_useSensor)
//	{
//		m_sensor->setShowLight(a_showEnabled);
//		//m_sensor->setShowLight(false);
//	}
//	// m_showId = a_showEnabled;
//	if (m_showId)
//		m_sphere->setShowEnabled(a_showEnabled);
//	else m_sphere->setShowEnabled(a_showEnabled, false);
//}

void cGenericRobot::initSensor(double a_lowTrueDetectionRate, double a_highTrueDetectionRate, double a_falseDetectionRate, double a_range, double a_ledRadius, bool a_showSensorLight)
{
	m_useSensor = true;
	m_sensor = new rtsSensor(m_sphere->getRadius(), a_lowTrueDetectionRate, a_highTrueDetectionRate, a_falseDetectionRate, a_range, a_ledRadius, a_showSensorLight);
	this->addChild(m_sensor);
}

void cGenericRobot::resetSensorDetectionState()
{
	m_sensor->resetDetectionState();
}

bool cGenericRobot::checkTargetInRange(cVector3d& a_pos)
{
	if (cDistance(this->getLocalPos(), a_pos) <= m_sensor->getRange())
		return true;
	else return false;
}

void cGenericRobot::startTrueDetection()
{
	m_sensor->startTrueReading();
}

void cGenericRobot::endTrueDetection()
{
	m_sensor->endTrueReading();
}

bool cGenericRobot::updateTrueSensorReading(cVector3d a_targetPos)
{
	bool detection = false;
	double dist = cDistance(this->getLocalPos(), a_targetPos);
	if (m_useSensor)
	{
		detection = m_sensor->updateTrueReading(dist);
	}

	return detection;
}

bool cGenericRobot::updateSensorReadingNoTargets()
{
	bool targetDetected = false;
	if (m_useSensor)
	{
		targetDetected = m_sensor->updateReadingNoTargets();
	}

	if (targetDetected)
	{

	}
	return targetDetected;
}

void cGenericRobot::updateSensorFalseDetectionRate(int a_numActiveTargets)
{
	m_sensor->updateFalseDetectionRate(a_numActiveTargets);
}

void cGenericRobot::clearSensorReading()
{
	m_sensor->resetDetectionState();
	m_sensor->noDetectionEvent();
}

void cGenericRobot::updateSensorLightTransparency()
{
	m_sensor->updateLightTransparency();
}

void cGenericRobot::setSensorColor(cColorf a_color)
{
	m_sensor->setLightColor(a_color);
}

void cGenericRobot::setSensorLightTransparency(double a_level)
{
	m_sensor->setLightTransparencyLevel(a_level);
}

void cGenericRobot::setSensorPosition(cVector3d a_position)
{
	m_sensor->setPosition(a_position);
}

void cGenericRobot::setSensorRatio(double a_sensorRatio)
{
	m_sensorRadiusRatio = a_sensorRatio;
}

bool cGenericRobot::deleteAndNullChild(cGenericObject* a_object)
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

void cGenericRobot::deleteAndNullAllChildren()
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