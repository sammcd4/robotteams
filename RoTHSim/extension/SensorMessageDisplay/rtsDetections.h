//------------------------------------------------------------------------------
#ifndef RTSDETECTIONSH
#define RTSDETECTIONSH
//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

class rtsDetections: public cGenericObject
{    
    //--------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //--------------------------------------------------------------------------

public:

    //! Constructor
	rtsDetections(int a_numAgents, double a_radius, double a_agentRadius, bool a_showAgent = true, cColorf a_color = cColorf(1.0, 0.0, 0.0));

    //! Destructor
	virtual ~rtsDetections();

    //--------------------------------------------------------------------------
    // PUBLIC METHODS:
    //--------------------------------------------------------------------------

public:

	void setColor(cColorf a_color);

	cColorf getColor(int a_sectionNum);

	void setShowLights(bool a_show);

	void setWorldToPixelFactor(double a_factor) { m_worldToPixel = a_factor; }

	void setBuildingPos(cVector3d a_pos) { m_buildingPos = m_worldToPixel*a_pos; }

	cVector3d getBuildingPos() { return m_buildingPos; }

	void update(bool a_sampleSignal, vector<bool> a_sensorReadings, vector<cVector3d> a_sensorPositions);

	void zeroPositions();

    //--------------------------------------------------------------------------
    // PROTECTED MEMBERS:
    //--------------------------------------------------------------------------
protected:

	int m_numAgents;
	double m_radius;
	vector<cShapeSphere*> m_lights;
	vector<cShapeSphere*> m_agents;
	bool m_showAgent;
	double m_agentRadius;
	cColorf m_color;
	double m_worldToPixel;
	double m_maxTransparencyLevel;
	cVector3d m_buildingPos;

	// fade timers
	vector<cPrecisionClock> m_fadeTimers;
	cPrecisionClock m_sampleTimer;
    //--------------------------------------------------------------------------
    // PROTECTED METHODS:
    //--------------------------------------------------------------------------

	void updateTransparency(bool a_sampleSignal, int a_agentNum, bool a_sensorReading, cVector3d a_pos);

	void deallocate();
};


//------------------------------------------------------------------------------
#endif // rtsDetectionsH
//------------------------------------------------------------------------------
