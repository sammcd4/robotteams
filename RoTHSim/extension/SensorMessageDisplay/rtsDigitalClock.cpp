#include "rtsDigitalClock.h"
//------------------------------------------------------------------------------

rtsDigitalClock::rtsDigitalClock()
{
	initDigitalClock("", "", false, 0, false);
}

rtsDigitalClock::rtsDigitalClock(string a_prependMessage, string a_appendMessage)
{
	initDigitalClock(a_prependMessage, a_appendMessage, false, 0, false);
}

rtsDigitalClock::rtsDigitalClock(bool a_useTimeLimit, int a_timeLimitInSeconds, bool a_useCountDown)
{
	initDigitalClock("", "", a_useTimeLimit, a_timeLimitInSeconds, a_useCountDown);
}

rtsDigitalClock::rtsDigitalClock(string a_prependMessage, string a_appendMessage, bool a_useTimeLimit, int a_timeLimitInSeconds, bool a_useCountDown)
{
	initDigitalClock(a_prependMessage, a_appendMessage, a_useTimeLimit, a_timeLimitInSeconds, a_useCountDown);
}

rtsDigitalClock::~rtsDigitalClock()
{
	deallocate();
}

void rtsDigitalClock::initDigitalClock(string a_prependMessage, string a_appendMessage, bool a_useTimeLimit, int a_timeLimitInSeconds, bool a_useCountDown)
{
	// hours
	m_hours = 0;

	// minutes
	m_minutes = 0;

	// seconds
	m_seconds = 0;

	// milliseconds
	m_milliseconds = 0;

	// use hours
	m_useHours = false;

	// use milliseconds
	m_useMilliseconds = false;

	// warning hasn't happened yet
	m_warning = false;

	// count down
	m_useCountDown = a_useCountDown;

	// default time limit status
	m_useTimeLimit = a_useTimeLimit;
	m_timeLimit = a_timeLimitInSeconds;
	if (a_timeLimitInSeconds - 60 < 0)
	{
		if (a_timeLimitInSeconds - 30 < 0)
			m_warningTime = 0;
		else m_warningTime = a_timeLimitInSeconds - 30; // if a minute is too much time, make it 30 seconds
	}
	else m_warningTime = a_timeLimitInSeconds - 60; // default to a minute before time limit is up
	m_warningColor.setRedDark();

	// create cLabel and add as child
	cFont* font = NEW_CFONTCALIBRI20();
	m_digitalTime = new cLabel(font);
	m_digitalTime->setShowEnabled(true);
	m_digitalTime->m_fontColor.setWhite();
	addChild(m_digitalTime);

	m_prependMessage = a_prependMessage;
	m_digitalTimeStr = "";
	m_appendMessage = a_appendMessage;

	// set zero digital time
	if (m_useMilliseconds)
	{
		if (m_useHours)
			setDigitalTime("00:00:00:000");
		else
			setDigitalTime("00:00:000");
	}
	else if (m_useHours)
		setDigitalTime("00:00:00");
	else
		setDigitalTime("00:00");

	setMarginSize(3.0);
}

double rtsDigitalClock::getCurrentTimeSeconds()
{
	return m_clock.getCurrentTimeSeconds();
}

void rtsDigitalClock::initClock()
{
	m_clock.reset();
	m_clock.start();

	// set zero digital time
	if (m_useMilliseconds)
	{
		if (m_useHours)
			setDigitalTime("00:00:00:000");
		else
			setDigitalTime("00:00:000");
	}
	else if (m_useHours)
		setDigitalTime("00:00:00");
	else
		setDigitalTime("00:00");

	m_digitalTime->setLocalPos(0, 0);
}

void rtsDigitalClock::initClockHours()
{
	m_useHours = true;
	initClock();
}

void rtsDigitalClock::initClockHoursMilliseconds()
{
	m_useHours = true;
	m_useMilliseconds = true;
	initClock();
}

double rtsDigitalClock::getDigitalTimeWidth() 
{ 
	return m_digitalTime->getWidth();
}

double rtsDigitalClock::getDigitalTimeHeight() 
{ 
	return m_digitalTime->getHeight();
}

void rtsDigitalClock::setPrependMessage(string a_prependMessage)
{ 
	m_prependMessage = a_prependMessage;
	updateClockDisplay();
}

void rtsDigitalClock::setAppendMessage(string a_appendMessage) 
{ 
	m_appendMessage = a_appendMessage; 
}

void rtsDigitalClock::setMarginSize(double a_marginSize)
{
	m_marginSize = a_marginSize;
	updateSize();
}

void rtsDigitalClock::updateSize()
{
	setSize(m_digitalTime->getWidth() + 2 * m_marginSize, m_digitalTime->getHeight() + 2 * m_marginSize);
	m_digitalTime->setLocalPos(m_marginSize, m_marginSize);
}

void rtsDigitalClock::updateClock()
{
	convertSecondsToTime();
	updateTimeLabel();
}

void rtsDigitalClock::setDisplayFontColor(const cColorf& a_color)
{
	m_digitalTime->m_fontColor.set(a_color.getR(), a_color.getG(), a_color.getB());
}

void rtsDigitalClock::togglePause()
{
	if (m_clock.on())
		pauseClock();
	else startClock();
}

void rtsDigitalClock::pauseClock()
{
	m_clock.stop();
}

void rtsDigitalClock::startClock()
{
	m_clock.start();
}


void rtsDigitalClock::setWarningColor(cColorf a_warningColor)
{
	m_warningColor = a_warningColor;
}

void rtsDigitalClock::setDigitalTime(string a_digitalTimeStr)
{
	m_digitalTimeStr = a_digitalTimeStr;
	updateClockDisplay();
}

void rtsDigitalClock::convertSecondsToTime()
{
	double currentTime = m_clock.getCurrentTimeSeconds(); // seconds
	int currentSeconds = (int)currentTime;

	if (m_useCountDown)
		currentSeconds = m_timeLimit - currentSeconds;


	if (m_useMilliseconds)
	{
		int currentMs = (int)(currentTime * 1000);

		if (m_useHours)
		{
			// convert seconds to hours, minutes, seconds, milliseconds
			m_hours = currentMs / (3600 * 1000);
			int remainingMs = currentMs % (3600 * 1000);
			m_minutes = remainingMs / (60 * 1000);
			int remainingMsForSeconds = remainingMs % (60 * 1000);
			m_seconds = remainingMsForSeconds / (1000);
			m_milliseconds = remainingMsForSeconds % (1000);
		}
		else
		{
			// convert seconds to minutes, seconds, milliseconds
			m_minutes = currentMs / (60 * 1000);
			int remainingMsForSeconds = currentMs % (60 * 1000);
			m_seconds = remainingMsForSeconds / (1000);
			m_milliseconds = remainingMsForSeconds % (1000);
		}
	}
	else if (m_useHours)
	{
		// convert seconds to hours, minutes, seconds
		m_hours = currentTime / (3600);
		int remainingSeconds = currentSeconds % (3600);
		m_minutes = remainingSeconds / 60;
		m_seconds = remainingSeconds % 60;
	}
	else
	{
		// only convert to minutes and seconds
		m_minutes = currentSeconds / 60;
		m_seconds = currentSeconds % 60;
	}

	// clamp hours, minutes, seconds, and milliseconds
	m_hours = cClamp(m_hours, 0, 99);
	m_minutes = cClamp(m_minutes, 0, 59);
	m_seconds = cClamp(m_seconds, 0, 59);
	m_milliseconds = cClamp(m_milliseconds, 0, 999);

}

void rtsDigitalClock::updateClockDisplay()
{
	string totalDisplay = m_prependMessage + m_digitalTimeStr + m_appendMessage;
	m_digitalTime->setText(totalDisplay);

	if (m_useTimeLimit && !m_warning && m_clock.getCurrentTimeSeconds() > m_warningTime)
	{
		m_warning = true;
		m_digitalTime->m_fontColor = m_warningColor;
	}
}

void rtsDigitalClock::updateTimeLabel()
{
	string hStr, mStr, sStr, msStr;

	// always minutes and seconds
	if (m_minutes < 10)
		mStr = "0" + to_string(m_minutes);
	else mStr = to_string(m_minutes);

	if (m_seconds < 10)
		sStr = "0" + to_string(m_seconds);
	else sStr = to_string(m_seconds);

	if (m_useMilliseconds)
	{
		if (m_milliseconds < 10)
			msStr = "00" + to_string(m_milliseconds);
		else if (m_milliseconds < 100)
			msStr = "0" + to_string(m_milliseconds);
		else msStr = to_string(m_milliseconds);

		if (m_useHours)
		{
			// write seconds to hours, minutes, seconds, milliseconds
			if (m_hours < 10)
				hStr = "0" + to_string(m_hours);
			else hStr = to_string(m_hours);

			m_digitalTimeStr = hStr + ":" + mStr + ":" + sStr + ":" + msStr;
		}
		else
		{
			// write seconds to minutes, seconds, milliseconds
			m_digitalTimeStr = mStr + ":" + sStr + ":" + msStr;
		}
	}
	else if (m_useHours)
	{
		// write seconds to hours, minutes, seconds
		if (m_hours < 10)
			hStr = "0" + to_string(m_hours);
		else hStr = to_string(m_hours);

		m_digitalTimeStr = hStr + ":" + mStr + ":" + sStr;
	}
	else
	{
		// write seconds to minutes and seconds
		m_digitalTimeStr = mStr + ":" + sStr;
	}

	updateClockDisplay();
}

void rtsDigitalClock::deallocate()
{
	removeChild(m_digitalTime);
	delete m_digitalTime;
	m_digitalTime = NULL;
}