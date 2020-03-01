
//------------------------------------------------------------------------------
#ifndef RTSDIGITALCLOCKH
#define RTSDIGITALCLOCKH
//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

class rtsDigitalClock : public cPanel
{    
    //--------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //--------------------------------------------------------------------------

public:

    //! Constructor of rtsDigitalClock.
	rtsDigitalClock();

	// Constructor of rtsDigitalClock to specify prepend append messages
	rtsDigitalClock(string a_prependMessage, string a_appendMessage);

	// Constructor of rtsDigitalClock for a time limit
	rtsDigitalClock(bool a_useTimeLimit, int a_timeLimitInSeconds, bool a_useCountDown);

	// Constructor of rtsDigitalClock for a time limit and specify prepend append messages
	rtsDigitalClock(string a_prependMessage, string a_appendMessage, bool a_useTimeLimit, int a_timeLimitInSeconds, bool a_useCountDown);

    //! Destructor of cScope.
	virtual ~rtsDigitalClock();

    //--------------------------------------------------------------------------
    // PUBLIC METHODS:
    //--------------------------------------------------------------------------

public:

	// get current time in seconds
	double getCurrentTimeSeconds();

	// initialize clock (00:00) minutes and seconds
	void initClock();

	// initialize clock (00:00:00) hours, minutes, seconds
	void initClockHours();

	// initialize clock (00:00:00:00) hours, minutes, seconds
	void initClockHoursMilliseconds();

	// get digital time width
	double getDigitalTimeWidth();

	// get digital time height
	double getDigitalTimeHeight();

	// set prepended message to digital time
	void setPrependMessage(string a_prependMessage);

	// get prepended message to digital time
	string getPrependMessage() { return m_prependMessage; }

	// set appended message to digital time
	void setAppendMessage(string a_appendMessage);

	// get appended message to digital time
	string getAppendMessage() { return m_appendMessage; }

	// get margin size
	double getMarginSize() { return m_marginSize; }

	// set margine size
	void setMarginSize(double a_marginSize);

	// update size
	void updateSize();

	// update clock
	void updateClock();

	// set display font color
	void setDisplayFontColor(const cColorf& a_color);

	// set warning color
	void setWarningColor(cColorf a_warningColor);

	// get warning color
	cColorf getWarningColor() { return m_warningColor; }

	// toggle pause clock
	void togglePause();

	// start clock
	void startClock();

	// pause clock
	void pauseClock();

    //--------------------------------------------------------------------------
    // PROTECTED MEMBERS:
    //--------------------------------------------------------------------------
protected:

	// clock
	cPrecisionClock m_clock;

	// label to display digital time
	cLabel* m_digitalTime;

	// string for digital time
	string m_digitalTimeStr;

	// appended message to digital time
	string m_appendMessage;

	// prepended message to digital time
	string m_prependMessage;

	// margin size
	double m_marginSize;

	// hours
	int m_hours;

	// minutes
	int m_minutes;

	// seconds
	int m_seconds;

	// milliseconds
	int m_milliseconds;

	// use hours
	bool m_useHours;

	// use milliseconds
	bool m_useMilliseconds;

	// use time limit
	bool m_useTimeLimit;

	// time limit (seconds)
	int m_timeLimit;

	// warning time (seconds);
	int m_warningTime;

	// warning color
	cColorf m_warningColor;

	// warning flag
	bool m_warning;

	// use count down
	bool m_useCountDown;

    //--------------------------------------------------------------------------
    // PROTECTED METHODS:
    //--------------------------------------------------------------------------

protected:

	// initialize digital clock
	void initDigitalClock(string a_prependMessage, string a_appendMessage, bool a_useTimeLimit, int a_timeLimitInSeconds, bool a_useCountDown);

	// convert seconds to hours, minutes, seconds, milliseconds
	void convertSecondsToTime();

	// set digital time
	void setDigitalTime(string a_digitalTimeStr);

	// update clock display
	void updateClockDisplay();

	// update time label
	void updateTimeLabel();

	// deallocate memory
	void deallocate();

};


//------------------------------------------------------------------------------
#endif // RTSDIGITALCLOCKH
//------------------------------------------------------------------------------
