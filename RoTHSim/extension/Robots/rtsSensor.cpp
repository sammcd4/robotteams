#include "rtsSensor.h"

rtsSensor::rtsSensor(double a_robotRadius, double a_lowTrueDetectionRate, double a_highTrueDetectionRate,
					 double a_falseDetectionRate, double a_range, double a_ledRadius, bool a_showLight)
{
	// seed the random number
	srand(time(NULL));

	// reset detection state
	resetDetectionState();

	// initialize member variables
	m_lowTDR = a_lowTrueDetectionRate;
	m_highTDR = a_highTrueDetectionRate;
	setTrueDetectionRate(a_lowTrueDetectionRate);
	m_completeFalseDetectionRate = a_falseDetectionRate;
	setFalseDetectionRate(a_falseDetectionRate);
	setRange(a_range);
	m_light = new cShapeSphere(a_ledRadius);
	m_light->setLocalPos(0, 0, a_robotRadius); // located relative to robot position
	setLightColor(cColorf(1.0, 0, 0)); // default light color
	m_light->m_material->m_emission = cColorf(0, 0, 0);
	m_light->m_material->m_specular = cColorf(0, 0, 0);
	m_light->m_material->m_diffuse = cColorf(0, 0, 0);
	m_light->setShowEnabled(false);
	m_light->setUseTransparency(true);
	m_light->setTransparencyLevel(0.0);
	this->addChild(m_light);

	m_showLight = a_showLight;

	// fade timer
	m_tClock.setTimeoutPeriodSeconds(0.5);
	m_tClock.reset();
	m_tClock.stop();

	// true sample clock timeout period
	double timeout = 0.10;
	m_trueSampleClock.setTimeoutPeriodSeconds(timeout);
	m_trueSampleClock.reset();
	m_trueSampleClock.start();

	// false sample clock timeout period
	m_falseSampleClock.setTimeoutPeriodSeconds(timeout*5.0);
	m_falseSampleClock.reset();
	m_falseSampleClock.start();

	m_useVariableTransparency = false;

	m_maxTransparencyLevel = 0.50;
	if (m_useVariableTransparency)
	{
		// variable transparency default show light state
		//m_showLight = true;
		m_transparencyLevel = 0.0;
		m_transparencyInc = 0.20;
		m_minTransparencyLevel = 0.0;
		m_numDetectedTargets = 0;
		m_numUndetectedTargets = 0;
		m_tClock.reset();
		m_highNumDetections = 4;
	}
}

rtsSensor::~rtsSensor()
{
	this->deleteAndNullChild(m_light);
}


void rtsSensor::updateTrueDetectionRate(double a_distanceAway)
{
	m_trueDetectionRate = m_lowTDR + (m_highTDR - m_lowTDR) * gauss(a_distanceAway, 0, m_range / 3);
}

void rtsSensor::updateFalseDetectionRate(int a_numActiveTargets)
{
	if (a_numActiveTargets > 0)
		m_falseDetectionRate = m_completeFalseDetectionRate / a_numActiveTargets;
	else m_falseDetectionRate = m_completeFalseDetectionRate;
}

void rtsSensor::startTrueReading()
{
	if (m_trueSampleClock.timeoutOccurred())
	{
		// stop sample clock
		m_trueSampleClock.stop();
		m_trueSampleClock.reset();
	}
}

void rtsSensor::endTrueReading()
{
	if (!m_trueSampleClock.on())
		m_trueSampleClock.start();
}

bool rtsSensor::updateTrueReading(double a_distanceAway)
{
	bool detection = false;
	if (!m_trueSampleClock.on())
	{
		// random number for probability between 1 - 1000 for more accuracy
		int percentage = rand() % 1000 + 1;

		m_numTargetsInRange++;
		updateTrueDetectionRate(a_distanceAway);
		
		if (percentage < m_trueDetectionRate * 1000)
		{
			detection = true;
		}
		//detection = true;

		if (detection)
		{
			detectionEvent();
		}
		else noDetectionEvent();

		// set state variable only if there has been a change in detection
		if (!m_state && detection)
			m_state = detection;
	}
	return detection;
}

bool rtsSensor::updateReadingNoTargets()
{
	bool detection = false;

	if (m_falseSampleClock.timeoutOccurred())
	{
		// stop sample clock
		m_falseSampleClock.stop();
		m_falseSampleClock.reset();

		// random number for probability
		int percentage = rand() % 1000 + 1;

		// compute detection based on false detection rate
		if (percentage < m_falseDetectionRate * 1000)
		{
			detection = true;
		}

		if (detection)
		{
			detectionEvent();
		}
		else noDetectionEvent();

		// set state variable only if there has been a change in detection
		if (!m_state && detection)
			m_state = detection;

		// restart sample clock
		m_falseSampleClock.start();
	}

	return detection;
}

void rtsSensor::resetDetectionState()
{
	m_state = false;
	m_numPrevDetectedTargets = m_numDetectedTargets;
	m_numPrevUndetectedTargets = m_numUndetectedTargets;
	m_numDetectedTargets = 0;
	m_numUndetectedTargets = 0;
	m_numTargetsInRange = 0;
}

void rtsSensor::detectionEvent()
{
	if (!m_useVariableTransparency)
	{
		// immediately show that a target was detected
		if (m_showLight)
		{
			m_light->setShowEnabled(true);
			//m_light->m_material->m_ambient.setRed();
		}
	}
	if (m_showLight)
	{
		startTransparencyFade();
	}

	m_numDetectedTargets++;
}

void rtsSensor::noDetectionEvent()
{
	if (!m_useVariableTransparency)
	{
		//// if haven't detected any targets, hide the light
		//if (!m_state)
		//	m_light->setShowEnabled(false);
	}
	m_numUndetectedTargets++;
}

void rtsSensor::startTransparencyFade()
{
	//m_light->setShowEnabled(true);
	m_tClock.stop();
	m_tClock.reset();
	m_tClock.start();
	//m_light->setTransparencyLevel(m_maxTransparencyLevel);
}

void rtsSensor::updateLightTransparency()
{
	//if (m_useVariableTransparency)
	//{
	//	// set transparency
	//	m_transparencyLevel = (m_maxTransparencyLevel / m_highNumDetections) * m_numDetectedTargets;
	//	applyLightTransparencyLevel();
	//}

	if (m_tClock.on())
	{
		double timeout = m_tClock.getTimeoutPeriodSeconds();
		double time = m_tClock.getCurrentTimeSeconds();
		double level = m_maxTransparencyLevel - time / timeout * m_maxTransparencyLevel;
		m_light->setTransparencyLevel(level);
		//cout << "level: " << level << endl;

		if (m_tClock.timeoutOccurred())
		{
			m_tClock.stop();
			m_tClock.reset();
			m_light->setTransparencyLevel(0.0);
		}
	}
}

void rtsSensor::setLightTransparencyLevel(double a_level)
{
	m_light->setTransparencyLevel(a_level);
}

void rtsSensor::setPosition(cVector3d a_position)
{
	m_light->setLocalPos(a_position);
}

cColorf rtsSensor::getLightColor()
{
	return m_lightColor;
}

//---------------------------------------------------------------------------

void rtsSensor::setLightColor(cColorf a_color)
{
	m_lightColor = a_color;
	m_light->m_material->m_ambient = a_color;
}

void rtsSensor::setLightRadius(double a_lightRadius)
{
	m_lightRadius = a_lightRadius;
	m_light->setRadius(m_lightRadius);
}

void rtsSensor::setShowLight(bool a_showLight) 
{
	m_showLight = a_showLight;
	m_light->setShowEnabled(m_showLight);
}

void rtsSensor::applyLightTransparencyLevel()
{
	m_transparencyLevel = cClamp(m_transparencyLevel, m_minTransparencyLevel, m_maxTransparencyLevel);
	m_light->setTransparencyLevel(m_transparencyLevel);
}



bool rtsSensor::deleteAndNullChild(cGenericObject* a_object)
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

void rtsSensor::deleteAndNullAllChildren()
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

double rtsSensor::gauss(double a_x, double a_mu, double a_sig)
{
	return exp(-(a_x - a_mu) * (a_x - a_mu) / (2 * a_sig * a_sig));
}
