#ifndef CGENERICROBOT_H
#define CGENERICROBOT_H
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsSensor.h"
#include <vector>
//---------------------------------------------------------------------------

using namespace chai3d;
using namespace std;

class cGenericRobot: public cGenericObject
{

public:

	// Constructor of cGenericRobot
    cGenericRobot(double a_radius, cVector3d a_pos)
	{	
		// create robot sphere
		m_sphere = new cShapeSphere(a_radius);

		// set position at robot position
		m_sphere->setLocalPos(0,0,0);

		// add the sphere to list of children
		this->addChild(m_sphere);

		// create label
		cFont* font = NEW_CFONTCALIBRI16();
		m_idLabel = new cLabel(font);
		m_sphere->addChild(m_idLabel);
		m_idLabel->setLocalPos(0.00, 1.25*a_radius, 0.00);
		m_idLabel->m_fontColor.set(1.0, 1.0, 1.0);

		// show id option
		setShowId(false);

		// Increment robot count
		numRobots++;

		// Set mass
		m_mass = 0.01;

		// Set initial velocity
		m_velocity = cVector3d(0.0, 0.0, 0.0);

		// Set rotation velocity
		m_rotVel = cVector3d(0.0, 0.0, 0.0);

		// Set initial accleration
		m_acceleration = cVector3d(0.0, 0.0, 0.0);

		// Default speed
		m_speed = m_velocity.length();
		defaultSpeed = 5.0;

		// Default color
		m_color = cColorf(0.0, 0.0, 0.0);

		m_useSensor = false;
	}

    // Destructor of cGenericRobot.
    virtual ~cGenericRobot();

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

	// Get mass of robot
	double getMass();

    // Get id number of robot
    int getIdNumber();

	// Set id number of robot
	void setIdNumber(int a_idNumber);

	// set show id
	void setShowId(bool a_showId);

	// get show id
	bool getShowId() { return m_showId; }

	// Get total number of robots
    int getNumRobots();
	
	// set previous position
	cVector3d getPrevPos();

	// set previous position
	void setPrevPos(cVector3d a_prevPos);

	// set previous position
	void setPrevPos(double prev_x, double prev_y, double prev_z);

	// set radius
	void setRadius(double a_radius);

	// get radius
	double getRadius() { return m_sphere->getRadius(); }

    // Set velocity of robot
    void setVel(cVector3d& a_vel);

	// Set velocity of robot
    void setVel(double x_vel, double y_vel, double z_vel);

	// Set speed of robot
    void setSpeed(double speed);

	// Set linear acceleration vector
	void setAcc(cVector3d& a_acc);

	// Set linear acceleration vector
	void setAcc(double x_acc, double y_acc, double z_acc);

	// Set rotational velocity vector
	void setRotVel(cVector3d& a_rotVel);

	// Set rotation velocity vector
	void setRotVel(double x_rotVel, double y_rotVel, double z_rotVel);

	// Set rotational acceleration vector
	void setRotAcc(cVector3d& a_rotAcc);

	// Set rotation acceleration vector
	void setRotAcc(double x_rotAcc, double y_rotAcc, double z_rotAcc);

	// Get linear velocity vector of robot
    cVector3d getVel();

	// Get linear accleration vector of robot
	cVector3d getAcc();

	// Get rotational velocity vector of robot
	cVector3d getRotVel();

	// Get rotational acceleration vector of robot
	cVector3d getRotAcc();

	// Get speed of robot
    double getSpeed();

	// get robot color
	cColorf getColor();

	// set robot color
	void setColor(cColorf a_color);

	//// set show robot
	//void setShowEnabled(bool a_showEnabled);

	//// get show robot
	//bool getShowEnabled() { return m_sphere->getShowEnabled(); }

	// initialize robot sensor
	void initSensor(double a_lowTrueDetectionRate, double a_highTrueDetectionRate, double a_falseDetectionRate, double a_range, double a_ledRadius, bool a_showSensorLight = true);

	// reset sensor detection state
	void resetSensorDetectionState();

	// check if target position is in sensor range
	bool checkTargetInRange(cVector3d& a_pos);

	// start true detection
	void startTrueDetection();

	// end true detection
	void endTrueDetection();

	// update sensor reading when target is in range (determined by team's updateSensorReadings method)
	bool updateTrueSensorReading(cVector3d a_targetPos);

	// update sensor reading
	bool updateSensorReadingNoTargets();

	// get sensor detection state
	bool getSensorDetectionState() { return m_sensor->getDetectionState(); }

	void updateSensorFalseDetectionRate(int a_numActiveTargets);

	// clear sensor reading
	void clearSensorReading();

	// update sensor light transparency
	void updateSensorLightTransparency();

	// set sensor light transparency
	void setSensorLightTransparency(double a_level);

	// set sensor light color
	void setSensorColor(cColorf a_color);

	// set sensor position (relative to robot parent world)
	void setSensorPosition(cVector3d a_position);

	// set sensor to robot radius ratio
	void setSensorRatio(double a_sensorRatio);

	// get sensor to robot radius ratio
	double getSensorRatio() { return m_sensorRadiusRatio; }

	// delete and point NULL a child object
	bool deleteAndNullChild(cGenericObject* a_object);

	// delete and point NULL all children
	void deleteAndNullAllChildren();

protected:
	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
	
	// set all vertex locations
	void setVertLocations();

	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------

	// robot sphere
	cShapeSphere* m_sphere;

	// Robot id number
	int m_idNumber;

	// id label
	cLabel* m_idLabel;

	// sensor
	rtsSensor* m_sensor;

	// sensor to robot radius ratio
	double m_sensorRadiusRatio;

	// use sensor
	bool m_useSensor;

	// Total number of robots
	static int numRobots;

	// Robot mass
	double m_mass;

	// previous position
	cVector3d m_prevPos;

    // Robot linear velocity
	cVector3d m_velocity;

	// Robot linear acceleration
	cVector3d m_acceleration;

	// Robot rotational velocity
	cVector3d m_rotVel;

	// Robot rotational acceleration
	cVector3d m_rotAcc;

	// Robot speed
	double m_speed;

	// Default robot speed
    double defaultSpeed;

	// Robot color
	cColorf m_color;

	// show id
	bool m_showId;
};

#endif // CGENERICROBOT_H
