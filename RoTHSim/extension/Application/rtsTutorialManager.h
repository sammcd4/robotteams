
//---------------------------------------------------------------------------
#ifndef RTSTUTORIALMANAGERH
#define RTSTUTORIALMANAGERH
//---------------------------------------------------------------------------


#include "Connection/comm.h"
#include "Connection/command.pb.h"
#include "Connection/swarm.pb.h"

#include "chai3d.h"
#include "Clay\rtsClay.h"
#include "Other/rtsPolygon.h"
#include "SensorMessageDisplay/rtsScope.h"
#include "Hotspots\rtsHotspotManager.h"
#include "rtsTutorialInstruction.h"
#include "rtsPictureInstruction.h"
#include "rtsVisualInstruction.h"
#include "rtsBoundingBox.h"
#include "rtsBoundingCircle.h"
#include "rtsArrow.h"

//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

//===========================================================================
/*!
    \file       rtsTutorialManager.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Application
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsTutorialManager
    \ingroup    Application

    \brief      
    Framework for robot team simulator demo for learning
*/
//===========================================================================
class rtsTutorialManager
{
public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsTutorialManager.
	rtsTutorialManager(int a_windowW, int a_windowH, cCamera* a_camera, cToolCursor* a_tool, rtsClay* a_playdoh, vector<rtsPolygon*> a_buildings,
					int a_windowW2, int a_windowH2, cCamera* a_camera2, vector<rtsScope*> a_scopes, cPanel* a_hqPanel, cPanel* a_userInputPanel,
					communicator* a_com, rtsHotspotManager* a_hotspotManager, bool a_zUpOriented, bool a_rightHanded);

	//! Destructor of rtsTutorialManager.
	virtual ~rtsTutorialManager();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------

	// get current time in seconds
	double getCurrentTimeSeconds();

	// initialize clock
	void initClock();

	// pause clock
	void pauseClock();

	// start clock
	void startClock();

	// check for pause to wait for user during instruction
	void checkForPause(bool a_response);

	// retrieve user input for current instruction
	void retrieveUserInput(bool a_deviceMove, 
							bool a_manipButtonDown, bool a_manipButtonUp, bool a_manipButtonDoubleClick, 
							bool a_manipSpacingButtonDown, bool a_manipSpacingButtonUp,
							bool a_commandPatrolButtonDown, bool a_commandPatrolButtonUp, 
							bool a_resizeButtonDown, bool a_resizeButtonUp);

	// update instructions
	void updateInstructionsHaptics();

	// update visuals
	void updateVisualsHaptics();

	// update instructions for hq
	void updateInstructions2();

	// update visuals for hq
	void updateVisuals2();

	// manually move forward or backward through instructions
	void moveToNextOrPrevInstruction(bool a_next);

	// get instruction index
	int getInstructionIndex() { return m_instructionIndex; }

	// resize instructions
	void resizeInstructions(int a_windowW, int a_windowH);

	// resize instructions for hq
	void resizeInstructions2(int a_windowW2, int a_windowH2);

	// resize visuals
	void resizeVisuals(int a_windowW, int a_windowH);

	// resize visuals for hq
	void resizeVisuals2(int a_windowW2, int a_windowH2);

	// set z up oriented
	void setZUpOriented(bool a_zUpOriented) { m_zUpOriented = a_zUpOriented; }

	// toggle pause demo
	void togglePauseTutorial();

	// update arrows showing feedback forces
	void updateArrows();

	// get haptics enabled
	bool getHapticsEnabled() { return m_demoHapticsEnabled; }

	// set haptics enabled
	void setHapticsEnabled(bool a_hapticsEnabled) { m_demoHapticsEnabled = a_hapticsEnabled; }

protected:

	void addInstruction_DefaultDur(string a_messageText, double a_multiplier = 1.0);
	
	void addInstruction_DefaultDur(vector<string> a_messages, double a_multiplier = 1.0);


	void addInstruction(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, string a_messageText,
							string a_responseType = "", int a_negVertOffset = 100, double a_fadeDuration = 1.0);

	void addInstruction(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, vector<string> a_messages,
							string a_responseType = "", int a_negVertOffset = 100, double a_fadeDuration = 1.0);



	void addInstruction2_DefaultDur(string a_messageText, double a_multiplier = 1.0);

	void addInstruction2_DefaultDur(vector<string> a_messages, double a_multiplier = 1.0);

	void addInstruction2(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, string a_messageText,
							string a_responseType = "", int a_negVertOffset = 100, double a_fadeDuration = 1.0);

	void addInstruction2(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, vector<string> a_messages,
		string a_responseType = "", int a_negVertOffset = 100, double a_fadeDuration = 1.0);


	// add visual for building 1
	void addBldg1Visual(double a_startTime);

	// add visual for building 2 with box around direction region
	void addBldgVisual(double a_startTime, double a_durFactor, int a_bldg = 1, int a_direction = 0);

	// add overlapping visual to accompany instructions (a_visualType: 0 - rectangle, 1 - circle, 2 - arrow, 3 - picture)
	void addVisual(cGenericObject* a_parentObject, int a_visualType, double a_startTime, double a_minDuration, double a_maxDuration,
					cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	void addCircleVisual(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration,
						cVector3d a_localPos = cVector3d(0, 0, 0), double a_circleRadius = 20.0, double a_width = 5.0, string a_responseType = "", double a_fadeDuration = 1.0);

	// add overlapping visual to accompany instructions, specificially for playdoh
	void addPlaydohVisual(double a_startTime, double a_minDuration, double a_maxDuration,
		cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	// add overlapping rectangle visual to accompany instructions, specifically on camera objects
	void addCameraVisual(cCamera* a_parentCamera, double a_startTime, double a_minDuration, double a_maxDuration,
		cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	// add overlapping rectangle visual to accompany instructions, specifically on camera objects
	void addCameraVisual2(cCamera* a_parentCamera, double a_startTime, double a_minDuration, double a_maxDuration,
		cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	// add overlapping visual to accompany instructions (a_visualType: 0 - rectangle, 1 - circle, 2 - arrow, 3 - picture)
	void addVisual2(cGenericObject* a_parentObject, int a_visualType, double a_startTime, double a_minDuration, double a_maxDuration,
		cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	// add overlapping rectangle visual to accompany hq instructions, specifically on panels
	void addPanelVisual2(cPanel* a_parentPanel, double a_startTime, double a_minDuration, double a_maxDuration,
		cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	// add overlapping picture visual to accompany instructions
	void addPictureVisual(cGenericObject* a_parentObject, string a_filename, double a_startTime, double a_minDuration, double a_maxDuration,
						string a_responseType = "", double a_fadeDuration = 1.0);

	// add tool arrow visual
	void addToolArrowVisual(double a_startTime, double a_minDuration, double a_maxDuration,
							cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	// pause server
	void pauseServer();

	// unpause server
	void unpauseServer();

	// shift all visuals back to account for increasing max time of instruction
	void shiftVisualsBack(double a_shiftTime);

	void checkCurrentInstructionStartCondition();
	void checkVisualStartCondition(rtsTutorialInstruction* a_visual);

    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
    
	// haptics camera
	cCamera* m_camera;

	// window width
	int m_windowW;

	// window height
	int m_windowH;

	// haptic device proxy
	cToolCursor* m_tool;

	// playdoh
	rtsClay* m_playdoh;
	
	// buildings in world
	vector<rtsPolygon*> m_buildings;

	// hotspot manager
	rtsHotspotManager* m_hotspotManager;

	// hq feed camera
	cCamera* m_camera2;

	// window2 width
	int m_windowW2;

	// window2 height
	int m_windowH2;

	// scopes for hq feed
	vector<rtsScope*> m_scopes;
	
	// hq panel for messages
	cPanel* m_hqPanel;

	// user input panel
	cPanel* m_userInputPanel;

	// communicator
	communicator* m_com;

	bool m_pauseCom;

	// clock
	cPrecisionClock m_clock;

	// response timer
	cPrecisionClock m_responseTimer;

	// list of instructions for haptic window
	vector<rtsTutorialInstruction*> m_instructions;

	// list of overlapping visuals
	vector<rtsTutorialInstruction*> m_visuals;

	// list of arrows for haptic devices
	vector<rtsArrow*> m_toolArrows;

	double m_displayForceScale;

	// list of instructions for hq window
	vector<rtsTutorialInstruction*> m_instructions2;

	// list of overlapping visuals for hq window
	vector<rtsTutorialInstruction*> m_visuals2;

	// instruction index
	int m_instructionIndex;

	// instruction index for hq
	int m_instruction2Index;

	double m_testStartTime;
	
	// skipped time when getting user response early
	double m_skippedTime;

	double m_instructionStartTime;
	double m_minDur;
	double m_maxDur;
	double m_waitDur;

	// line width
	double m_lineWidth;

	// highlight color
	cColorf m_highlightColor;

	// user input
	bool m_userResponse;

	// any user response made
	bool m_userResponseMade;
	bool m_userResponseMade2;

	// z up oriented
	bool m_zUpOriented;

	// cordon mode
	bool m_cordonMode;

	// playdoh contacted
	bool m_demoPlaydohContacted;

	// desired location flag
	bool m_desiredLocationReached;
	cVector3d m_desiredLocation1;
	bool m_cordonLocation1Reached;
	bool m_cordonLocation1Append;
	cVector3d m_bldg1Pos;
	cVector3d m_bldg2Pos;
	double m_desiredRadius;

	// haptics enabled
	bool m_demoHapticsEnabled;
	bool m_playdohHapticsEnabled;
	
	// show hotspots
	bool m_demoShowTargets;

	bool m_rightHanded;
    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSTUTORIALMANAGERH
//---------------------------------------------------------------------------
