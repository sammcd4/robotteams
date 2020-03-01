#pragma once

#include "chai3d.h"
#include <algorithm>
#include <vector>
#include "Hotspots/rtsHotspot.h"
#include "rtsData.h"
#include <fstream>
#include <string>
#include <iomanip>
#include <mutex>

using namespace chai3d;
using namespace std;

class rtsDataManager
{

public:
	// Constructor
	rtsDataManager(string a_inputFile, string a_outputFile, string a_eventOutputFile);

	// Destructor
	~rtsDataManager();

	// start clock
	void startClock() { m_clock.start(); }

	// reset clock
	void resetClock() {m_clock.reset(); }

	// stop clock
	void stopClock() { m_clock.stop(); }

	// get current time
	double getCurrentTimeSeconds() { return m_clock.getCurrentTimeSeconds(); }

	// collect and write data
	bool collectAndWriteData(cVector3d a_cursorPos, 
					 cVector3d a_feedbackForce, 
					 int a_teamMode,
					 vector<cVector3d> a_robotPositions);

	// collect and write event data
	bool collectAndWriteEventData(bool a_manipButtonDown, bool a_manipButtonUp, bool a_manipButtonDoubleClick,
								bool a_manipSpacingButtonDown, bool a_manipSpacingButtonUp,
								bool a_commandPatrolButtonDown, bool a_commandPatrolButtonUp,
								bool a_resizeButtonDown, bool a_resizeButtonUp);

	// collect data
	rtsData collectData(cVector3d a_cursorPos,
						cVector3d a_feedbackForce,
						vector<cVector3d> a_robotPositions);

	// write data
	bool writeData(rtsData a_data);

	// read user input
	string readUserInput(string a_message);

	// get input file strea
	std::ifstream* getFin();

	// check input file stream is open
	bool fin_is_open() { return m_fin->is_open(); }

	// close input file stream
	void fin_close() { m_fin->close(); }

	// set sample rate (in seconds)
	void setSampleRate(double a_sampleRate){ m_sampleRate = a_sampleRate; }

	// get sample rate (in seconds)
	double getSampleRate() { return m_sampleRate; }

	// set mutex
	void setMutex(std::mutex* a_mutex) { m_mtx = a_mutex; }

	// get mutex
	std::mutex* getMutex() { return m_mtx; }

private:
	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// enable writing data
	bool m_enableWriting;
	bool m_enableEventWriting;

	// input file stream
	ifstream* m_fin;

	// output file name
	string m_outputFile;

	// output file stream
	ofstream* m_fout;

	// event output file name
	string m_eventOutputFile;

	// output file stream for events
	ofstream* m_eventFout;

	// running clock
	cPrecisionClock m_clock;

	// sample timer
	cPrecisionClock m_timer;

	// sample rate (in seconds)
	double m_sampleRate;

	// mutex
	std::mutex* m_mtx;
};

