//---------------------------------------------------------------------------
#include "system/CGlobals.h"
//---------------------------------------------------------------------------

#include <iostream>
#include <new>
#include <math.h>
#include "rtsRobot.h"

// Initialize static variable
int rtsRobot::numRobots = 0;

rtsRobot::~rtsRobot()
{
    // Decrement robot count
    numRobots--;

	// Deallocate memory
	deleteAndNullAllChildren();
}

//---------------------------------------------------------------------------

int rtsRobot::getIdNumber()
{
    return m_idNumber;
}

//---------------------------------------------------------------------------

void rtsRobot::setIdNumber(int a_idNumber)
{
	m_idNumber = a_idNumber;

	// also set value in the label for this id
	string str;
	str = "#" + to_string(m_idNumber);
	m_idLabel->setText(str);
}

//---------------------------------------------------------------------------

void rtsRobot::setShowId(bool a_showId)
{ 
	if (m_showEnabled)
		m_showId = a_showId;
	else m_showId = false;

	m_idLabel->setShowEnabled(m_showId);
}

//---------------------------------------------------------------------------

int rtsRobot::getNumRobots()
{
    return numRobots;
}

////---------------------------------------------------------------------------
//
//void rtsRobot::setLocalPos(cVector3d a_pos)
//{ 
//	m_sphere->setLocalPos(a_pos);
//	if (m_useSensor)
//		setSensorPosition(a_pos);
//}

//---------------------------------------------------------------------------

double rtsRobot::getMass()
{
	return m_mass;
}

//---------------------------------------------------------------------------

// get previous position
cVector3d rtsRobot::getPrevPos()
{
	// return previous position stored in cGenericObject class
	return m_prevPos;
}

//---------------------------------------------------------------------------

// set previous position
void rtsRobot::setPrevPos(cVector3d a_prevPos)
{
	m_prevPos = a_prevPos;
}

//---------------------------------------------------------------------------

void rtsRobot::setRadius(double a_radius) 
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
void rtsRobot::setPrevPos(double prev_x, double prev_y, double prev_z)
{
	m_prevPos.x(prev_x);
	m_prevPos.y(prev_y);
	m_prevPos.z(prev_z);
}

//---------------------------------------------------------------------------

void rtsRobot::setVel(cVector3d& a_vel)
{
	// Set member velocity vector
    m_velocity = a_vel;

	// Set member speed scalar
	m_speed = m_velocity.length();
}

//---------------------------------------------------------------------------

void rtsRobot::setVel(double x_vel, double y_vel, double z_vel)
{
	// Set member velocity vector
	m_velocity.x(x_vel);
	m_velocity.y(y_vel);
	m_velocity.z(z_vel);

	// Set member speed scalar
	m_speed = m_velocity.length();
}

//---------------------------------------------------------------------------

void rtsRobot::setSpeed(double speed)
{
	// Set speed in the direction of current heading
	//m_velocity = speed*m_heading;

	//// Set member speed scalar
	//m_speed = speed;
}

//---------------------------------------------------------------------------

// Set linear acceleration vector
void rtsRobot::setAcc(cVector3d& a_acc)
{
	m_acceleration = a_acc;
}

//---------------------------------------------------------------------------

// Set rotational velocity vector
void rtsRobot::setRotVel(cVector3d& a_rotVel)
{
	m_rotVel = a_rotVel;
}

//---------------------------------------------------------------------------

// Set rotation velocity vector
void rtsRobot::setRotVel(double x_rotVel, double y_rotVel, double z_rotVel)
{
	m_rotVel.x(x_rotVel);
	m_rotVel.y(y_rotVel);
	m_rotVel.z(z_rotVel);
}

//---------------------------------------------------------------------------

// Set linear acceleration vector
void rtsRobot::setAcc(double x_acc, double y_acc, double z_acc)
{
	m_acceleration.x(x_acc);
	m_acceleration.y(y_acc);
	m_acceleration.z(z_acc);
}

//---------------------------------------------------------------------------

// Set rotational acceleration vector 
void rtsRobot::setRotAcc(cVector3d& a_rotAcc)
{
	m_rotAcc = a_rotAcc;
}

//---------------------------------------------------------------------------

// Set rotation acceleration vector
void rtsRobot::setRotAcc(double x_rotAcc, double y_rotAcc, double z_rotAcc)
{
	m_rotAcc.x(x_rotAcc);
	m_rotAcc.y(y_rotAcc);
	m_rotAcc.z(z_rotAcc);
}

//---------------------------------------------------------------------------

cVector3d rtsRobot::getVel()
{
    return m_velocity;
}

//---------------------------------------------------------------------------

cVector3d rtsRobot::getAcc()
{
	return m_acceleration;
}

//---------------------------------------------------------------------------

cVector3d rtsRobot::getRotVel()
{
	return m_rotVel;
}

//---------------------------------------------------------------------------

cVector3d rtsRobot::getRotAcc()
{
	return m_rotAcc;
}

//---------------------------------------------------------------------------

double rtsRobot::getSpeed()
{
    return m_speed;
}

//---------------------------------------------------------------------------

cColorf rtsRobot::getColor()
{
	return m_color;
}

//---------------------------------------------------------------------------

void rtsRobot::setColor(cColorf a_color)
{
	m_color = a_color;
	m_sphere->m_material->m_ambient = m_color;
	m_sphere->m_material->m_diffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
	m_sphere->m_material->m_specular.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_sphere->m_material->m_emission.set(0.0f, 0.0f, 0.0f, 1.0f);
}

//---------------------------------------------------------------------------

//void rtsRobot::setShowEnabled(bool a_showEnabled)
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

void rtsRobot::initSensor(double a_lowTrueDetectionRate, double a_highTrueDetectionRate, double a_falseDetectionRate, double a_range, double a_ledRadius, bool a_showSensorLight)
{
	m_useSensor = true;
	m_sensor = new rtsSensor(m_sphere->getRadius(), a_lowTrueDetectionRate, a_highTrueDetectionRate, a_falseDetectionRate, a_range, a_ledRadius, a_showSensorLight);
	this->addChild(m_sensor);
}

void rtsRobot::resetSensorDetectionState()
{
	m_sensor->resetDetectionState();
}

bool rtsRobot::checkTargetInRange(cVector3d& a_pos)
{
	if (cDistance(this->getLocalPos(), a_pos) <= m_sensor->getRange())
		return true;
	else return false;
}

void rtsRobot::startTrueDetection()
{
	m_sensor->startTrueReading();
}

void rtsRobot::endTrueDetection()
{
	m_sensor->endTrueReading();
}

bool rtsRobot::updateTrueSensorReading(cVector3d a_targetPos)
{
	bool detection = false;
	double dist = cDistance(this->getLocalPos(), a_targetPos);
	if (m_useSensor)
	{
		detection = m_sensor->updateTrueReading(dist);
	}

	return detection;
}

bool rtsRobot::updateSensorReadingNoTargets()
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

void rtsRobot::updateSensorFalseDetectionRate(int a_numActiveTargets)
{
	m_sensor->updateFalseDetectionRate(a_numActiveTargets);
}

void rtsRobot::clearSensorReading()
{
	m_sensor->resetDetectionState();
	m_sensor->noDetectionEvent();
}

void rtsRobot::updateSensorLightTransparency()
{
	m_sensor->updateLightTransparency();
}

void rtsRobot::setSensorColor(cColorf a_color)
{
	m_sensor->setLightColor(a_color);
}

void rtsRobot::setSensorLightTransparency(double a_level)
{
	m_sensor->setLightTransparencyLevel(a_level);
}

void rtsRobot::setSensorPosition(cVector3d a_position)
{
	m_sensor->setPosition(a_position);
}

void rtsRobot::setSensorRatio(double a_sensorRatio)
{
	m_sensorRadiusRatio = a_sensorRatio;
}

bool rtsRobot::deleteAndNullChild(cGenericObject* a_object)
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

void rtsRobot::deleteAndNullAllChildren()
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