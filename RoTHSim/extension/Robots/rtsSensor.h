#ifndef RTSSENSOR_H
#define RTSSENSOR_H
//---------------------------------------------------------------------------
#include "chai3d.h"
#include <vector>
#include <random>
#include <time.h>
//---------------------------------------------------------------------------

using namespace chai3d;
using namespace std;

class rtsSensor: public cGenericObject
{

public:

	// Constructor of rtsSensor
	rtsSensor(double a_robotRadius, double a_lowTrueDetectionRate, double a_highTrueDetectionRate,
			  double a_falseDetectionRate, double a_range, double a_ledRadius, bool a_showLight = true);

    // Destructor of rtsSensor.
    virtual ~rtsSensor();

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------

	// set range
	void setRange(double a_range) { m_range = a_range; }

	// get range
	double getRange() { return m_range; }

	// set true detection rate
	void setTrueDetectionRate(double a_tdr) { m_trueDetectionRate = a_tdr; }

	// get true detection rate
	double getTrueDetectionRate() { return m_trueDetectionRate; }

	// update true detection rate by gaussian
	void updateTrueDetectionRate(double a_distanceAway);

	// set false detection rate
	void setFalseDetectionRate(double a_fdr) { m_falseDetectionRate = a_fdr; }

	// get false detection rate
	double getFalseDetectionRate() { return m_falseDetectionRate; }

	// update false detection rate by number of active targets
	void updateFalseDetectionRate(int a_numActiveTargets);

	bool updateTrueReading(double a_distanceAway);

	// start true reading
	void startTrueReading();

	// end true reading
	void endTrueReading();

	// detection state with no targets
	bool updateReadingNoTargets();

	// reset detection state
	void resetDetectionState();

	// detection event
	void detectionEvent();

	// no detevtion event
	void noDetectionEvent();

	// get detection state
	bool getDetectionState() { return m_state; }

	// get number of targets in range (after updateSensorReading called)
	int getNumTargetsInRange() { return m_numTargetsInRange; }

	// get number pf detected targets (after updateSensorReading called)
	int getNumDetectedTargets() { return m_numDetectedTargets; }

	// set sensor position
	void setPosition(cVector3d a_position);

	// get led light color
	cColorf getLightColor();

	// set led light color
	void setLightColor(cColorf a_color);

	// get led light radius
	double getLightRadius() { return m_lightRadius; }

	// set led light radius
	void setLightRadius(double a_lightRadius);

	// set show light
	void setShowLight(bool a_showLight);

	// set Transparency level of light
	void applyLightTransparencyLevel();

	// update light transparency
	void updateLightTransparency();

	// get variable transparency state
	bool getUseVariableTransparency() { return m_useVariableTransparency; }

	void setLightTransparencyLevel(double a_level);

private:

	// compute gaussian function value
	double gauss(double a_x, double a_mu, double a_sig);

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
		
	// delete and point NULL a child object
	bool deleteAndNullChild(cGenericObject* a_object);

	// delete and point NULL all children
	void deleteAndNullAllChildren();

	void startTransparencyFade();


	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
	
	// detection state
	bool m_state;

	// sensor range (distance)
	double m_range;

	// true detection rate ( P(O_true|S_true) )
	double m_trueDetectionRate;

	// high true detection rate
	double m_highTDR;

	// low true detection rate
	double m_lowTDR;

	// false detection rate ( P(O_true|S_false) )
	double m_falseDetectionRate;

	// complete false detection rate
	double m_completeFalseDetectionRate;

	// high false detection rate
	double m_highFDR;

	// low false detection rate
	double m_lowFDR;

	// number of detected targets (either true or false positives)
	int m_numDetectedTargets;

	// number of previously detect targets( either true or flase positives)
	int m_numPrevDetectedTargets;

	// number of undetected targets
	int m_numUndetectedTargets;

	// number of previously undetected targets
	int m_numPrevUndetectedTargets;

	// number of true targets in range
	int m_numTargetsInRange;

	// led light
	cShapeSphere* m_light;

	// led light color
	cColorf m_lightColor;

	// led transparency level
	float m_transparencyLevel;

	// minimum led transparency level
	float m_minTransparencyLevel;

	// maximum led transparency level
	float m_maxTransparencyLevel;

	// led transparency increment
	float m_transparencyInc;

	// use variable transparency
	bool m_useVariableTransparency;

	// high number of detections
	int m_highNumDetections;

	// transparency change clock
	cPrecisionClock m_tClock;

	// sample clock
	cPrecisionClock m_trueSampleClock;
	cPrecisionClock m_falseSampleClock;

	// show light
	bool m_showLight;

	// led light radius
	double m_lightRadius;
};

#endif // RTSSENSOR_H
