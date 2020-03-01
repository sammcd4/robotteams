#pragma once

#include "chai3d.h"
#include <vector>

using namespace chai3d;
using namespace std;

// target type is specific type of sphere object
class rtsNASATLXDefinitions: public cGenericObject
{
public:

	// Constructor
	rtsNASATLXDefinitions(cCamera* a_camera, int a_windowW, int a_windowH);

	virtual ~rtsNASATLXDefinitions();

	// start nasa tlx
	void start();

	// resize
	void resize(int a_w, int a_h);

	// update weights
	void updateWeights(bool a_right);

	// get running state
	bool getRunning() { return m_running; }

	bool finished() { return m_finished; }

private:

	// delete and point NULL all children
	void deallocate();

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------
	
	// tlx running
	bool m_running;

	// tlx finished
	bool m_finished;

	// Graphics

	// main panel
	cPanel* m_panel2;

	// rating scale definitions
	vector<vector<cLabel*>> m_scaleDefs;

	int m_windowW;
	int m_windowH;
};

