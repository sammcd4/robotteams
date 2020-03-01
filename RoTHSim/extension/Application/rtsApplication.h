
//---------------------------------------------------------------------------
#ifndef RTSAPPLICATIONH
#define RTSAPPLICATIONH
//---------------------------------------------------------------------------
#include "robotteams.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

//===========================================================================
/*!
    \file       rtsApplication.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Application
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsApplication
    \ingroup    Application

    \brief      
    Framework for robot team simulator application
*/
//===========================================================================
class rtsApplication
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsApplication.
	  rtsApplication(string worldFile, string userName, bool hapticsEnabled, bool visualEnabled, bool showSurroundOutline, bool showTravelOutline, bool useVisualDemo, bool useNASATLX, bool showHiddenHotspots, bool invertSensorMessage, bool useDetections, bool useDataCollection, bool zUpOriented);
    
	//! Destructor of rtsApplication.
	virtual ~rtsApplication();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------

	bool init(int argc, char* argv[], int windowW, int windowH, bool fullscreen, int windowW2, int windowH2, bool fullscreen2);

	bool startSim();

	bool startClock();

	// MEMBERS

	// presets
	bool m_useDetections = true;			// use sensor readings shown as detections around surrounded building
	bool m_showHotspotsWithTeam;		// show found hotspots on team display
	bool m_showHiddenHotspots;
	bool m_useServerThread = true;			// need server thread to ensure haptic rate 1kHz
	bool m_useDataThread = false;			// shouldn't need a whole other thread for infrequently writing to file
	bool m_useDataCollection = true;
	bool m_zUpOriented = false;

	// SETTINGS

	string m_worldFile;
	string m_userName;

    //------------------------------------------------------------------------------
    // DECLARED FUNCTIONS
    //------------------------------------------------------------------------------
    
    // callback when the second window display is resized
    void resizeWindow2(int w, int h);
    
    // callback when the environment window display is resized
    void resizeWindow(int w, int h);
    
    // callback when a key is pressed for 2
    bool keySelect2(unsigned char key, int x, int y);
    
    // callback when a key is pressed for environment window
    bool keySelect(unsigned char key, int x, int y);

	// ESC routine
	void escRoutine(bool& exiting);
    
    // callback to render headquarters feed
    void updateGraphics2(void);
    
    // callback to render graphic scene
    void updateGraphics(void);
    
    // callback for GLUT timer 2
    void graphicsTimer2(int data);
    
    // callback of GLUT timer
    void graphicsTimer(int data);
    
    // function that closes the application
    void close(void);
    
    // main haptics simulation loop
    void updateHaptics(void);
    
    // callback to handle mouse click
    void mouseClick(int button, int state, int x, int y);
    
    // callback to handle mouse motion
    void mouseMove(int x, int y);

	// query from server and update server
	void updateServer();

	// data collection thread function
	void dataCollection();

private:
    
	// initialize application
	void initApplication();

    // deallocate memory
    void deallocate();

	// start simulation routine
	void startSimRoutine();

	// get tool velocity
	cVector3d getToolVelocity();

	// compute travel force
	cVector3d computeTravelForce(const cVector3d& a_devicePos, const cVector3d& a_travelToolStatPos);

	// compute spacing force
	cVector3d computeSpacingForce(cVector3d a_devicePos, cVector3d a_manipSpacingCenter, 
								  const double& a_minSpacingRadius, const double& a_maxSpacingRadius);

	cVector3d computeResizeForce(cVector3d a_devicePos, cVector3d a_resizeCenter);

	// manip double click routine
	void manipDoubleClickRoutine();

	// manip spacing double click routine
	void manipSpacingDoubleClickRoutine();

	// compute robot team haptic feedback forces
	void computeHapticFeedback(const cVector3d& a_devicePos, const double& a_timeInterval,
								const bool& a_travelButton, bool& a_endTravelForce, cPrecisionClock& a_endTravelClock,
								const cVector3d& a_travelToolStartPos, const double& a_endTravelTimeout,
								const bool& a_resizeButton, const cVector3d& a_resizeToolStartPos, bool& a_startResizeForce, cPrecisionClock& a_startResizeClock,
								bool& a_endResizeForce, cPrecisionClock& a_endResizeClock,
								const bool& a_manipButton,
								const bool& a_manipSpacingButton, bool& a_startSpacingForce, cPrecisionClock& a_startSpacingClock, 
								bool& a_endSpacingForce, cPrecisionClock& a_endSpacingClock,
								const cVector3d& a_manipSpacingCenter, const double a_endSpacingTimeout, const double a_startSpacingTimeout,
								const double& a_minSpacingRadius, const double& a_maxSpacingRadius);

	// retrieve robot data
	void retrieveRobotData(bool a_manipButton, bool a_manipSpacingButton, bool a_resizeButton);

	// convert agent data to vector of positions, optionally setting id order
	vector<cVector3d> swarm2Vec(comm::Swarm* a_swarm, bool a_setIdOrder);

	// convert polygon point data to vector of polygon points
	vector<cVector3d> polygon2Vec(comm::PolygonList_Polygon* a_polygon);

	// convert polygon data to vector of vector of polygon points
	vector<vector<cVector3d>> polygons2Vec(comm::PolygonList* a_polygons);

	// set surround ids only
	void setSurroundIds(comm::Swarm* a_swarm);

	// print a vector (for debugging)
	void printVector(vector<int> a_vector);

	// find nearest agent
	int findNearestAgent(cVector3d a_pos, bool useServerPositions);

	// fin nearest two agents
	void findNearestTwoAgents(const cVector3d& a_pos, int& a_closest, int& a_secondClosest);

	// find farthest agent
	int findFarthestAgent(cVector3d a_pos, bool useServerPositions);

	// set agent mode colors
	void setAgentModeColor(comm::Swarm_Agent* a_swarmAgent);

	// find nearest surround index
	int getNearestSurroundIndex(cVector3d a_pos);

	// find nearest and farthest surround indices
	void getNearestAndFarthestSurroundIndices(cVector3d a_pos, int& a_nearest, int& a_farthest);

	// start speed clock
	void startSpeedClock();

	// stop speed clock
	void stopSpeedClock();

	// set closest building index
	void setClosestBldgIndex();

private:
	//------------------------------------------------------------------------------
	// SERVER MEMBERS
	//------------------------------------------------------------------------------

	// use server
	bool m_useServer;

	// server communicator
	communicator* m_com;

	// swarm
	comm::Swarm* m_swarm;

	// query mutex
	mutex* m_queryMutex;

	// enable query mutex
	bool m_enableQueryMutex;

	int m_swarmModeInt;

	// minimum agent distance
	double m_AIRadius;
	double m_travelRadius;
	double m_surroundRadius;
	double m_radiusTimeout;
	double m_swarmRadius;

	bool m_pause;

	// query iteration frequency
	cFrequencyCounter m_serverFreqCounter;

	// server timer
	cPrecisionClock m_serverTimer;

	// cycle counter
	cPrecisionClock m_cycleCounter;

	// radius counter
	cPrecisionClock m_radiusClock;

	// desired positions for server thread
	vector<cVector3d> m_serverDesiredPositions;

	// current positions for server thread
	vector<cVector3d> m_serverCurrentPositions;

	// manipulation flag for server
	bool m_serverManipButton;

	// playdoh contacted flag for server
	bool m_serverContacted;

	// manipulation completed flag for server
	bool m_serverManipCompleted;

	// resize flag for server
	bool m_serverResizeButton;

	// resize surround completed flag for server
	bool m_serverResizeSurroundCompleted;

	// travel variables
	bool m_serverTravelCommanded;
	cVector3d m_serverTravelDir1;
	cVector3d m_serverTravelDir2;
	cVector3d m_commandedTravelDir1;
	cVector3d m_commandedTravelDir2;
	int m_travelCount;
	int m_serverTravelId1;
	int m_serverTravelId2;
	int m_travelId1;
	int m_travelId2;

	// radius variables
	int m_radiusPriority;
	double m_serverAIRadius;
	bool m_serverIncreaseRadius;

	bool m_serverResetToStartManip;
	bool m_serverResetToStartComplete;

	bool m_useVisualDemo;
	bool m_visualEnabled;
	bool m_showSurroundOutline;
	bool m_showTravelOutline;

	//------------------------------------------------------------------------------
	// ENVIRONMENT VARIABLES
	//------------------------------------------------------------------------------

	// camera projection sphere
	cShapeSphere* m_cameraProjSphere;

	// virtual ground
	cMesh* m_ground;

	// virtual polygon (building)
	vector<rtsPolygon*> m_buildings;

	// number of polygons per building
	vector<int> m_polygonsPerBldg;

	//------------------------------------------------------------------------------
	// ROBOT TEAM VARIABLES
	//------------------------------------------------------------------------------

	// mutex for graphics
	std::mutex* m_mtx;

	// create robot locations
	vector<cVector3d> m_robotPositions;

	// create playdoh robot locations
	vector<cVector3d> m_playdohRobotPositions;

	// raw robot positions from server
	vector<cVector3d> m_rawRobotPositions;

	// robot positions before manipulation occurs
	vector<cVector3d> m_prevManipPositions;

	// create robot ids
	vector<int> m_ids;

	// create surround ids
	vector<int> m_surroundIds;

	// robot team
	rtsRobotTeam* m_robotTeam;

	// playdoh robot team
	rtsRobotTeam* m_playdohRobotTeam;

	// controller
	rtsController* m_controller;

	// manipulation completed
	bool m_manipCompleted;

	// positions of agents upon completion
	vector<cVector3d> m_manipCompletedPositions;

	// manipulation started
	bool m_manipStarted;

	// reset to start manip positions flag
	bool m_resetToStartManip;

	// original positions of agents before manipulation
	vector<cVector3d> m_manipStartedPositions;

	// resize surround completed
	bool m_resizeSurroundCompleted;

	// positions of agents upon resize completion
	vector<cVector3d> m_resizeSurroundCompletedPositions;

	// enable travel command
	bool m_travelEnabled;
	cPrecisionClock m_travelEnabledClock;

	//------------------------------------------------------------------------------
	// HAPTICS VARIABLES
	//------------------------------------------------------------------------------

	// playdoh mesh
	rtsClay* m_playdoh;

	// distribution type variable (0 - travel, 1 - surround)
	int m_distributionType;

	// resize status
	bool m_flagResize;

	// resize percentage
	double m_resizePercentage;

	// total resize percentage
	double m_totalResizePercentage;

	// maximum resize percentage
	double m_maxResizePercentage;

	// minimum resize percentage
	double m_minResizePercentage;

	// haptics enabled
	bool m_hapticsEnabled;


	//------------------------------------------------------------------------------
	// TOOL VARIABLES
	//------------------------------------------------------------------------------

	// a haptic device handler
	cHapticDeviceHandler* m_handler;

	// a pointer to the current haptic device
	cGenericHapticDevicePtr m_hapticDevice;

	// a virtual tool representing the haptic device in the scene
	cToolCursor* m_tool;

	// a label to explain what is happening
	cLabel* m_labelMessage;

	// a label to display the rate [Hz] at which the simulation is running
	cLabel* m_labelHapticRate;

	// number of haptic devices detected
	int m_numHapticDevices;

	// device radius
	double m_deviceRadius;

	// device position
	cVector3d m_devicePos;

	// max linear damping coefficient
	double m_maxDamping;

	// max stiffness
	double m_maxStiffness;

	// virtual radius of workspace
	double m_virtualWorkspaceRadius;

	// scale factor between the device workspace and cursor workspace
	double m_toolWorkspaceScaleFactor;

	// desired workspace radius of the virtual cursor
	double m_cursorWorkspaceRadius;


	//------------------------------------------------------------------------------
	// DATA VARIABLES
	//------------------------------------------------------------------------------

	// data manager
	rtsDataManager* m_dataManager;

	// data stack
	deque<rtsData> m_dataStack;

	// temp data stack
	vector<rtsData> m_tempDataStack;

	// mutex for data
	std::mutex* m_dataMutex;

	// enabler for data mutex
	bool m_enableDataMutex;

	// simulation clock
	rtsDigitalClock* m_simClock;

	// test speed clock
	cPrecisionClock m_speedClock;

	// time intervals for each iteration
	bool m_checkTimeIntervals;
	vector<double> m_timeIntervals;

	// check time interval of specific regions
	bool m_checkRegions;
	vector<double> m_regionTimeIntervals;

	string m_outputFolder;

	//------------------------------------------------------------------------------
	// TARGET VARIABLES
	//------------------------------------------------------------------------------

	// hotspot manager
	rtsHotspotManager* m_hotspotManager;

	//------------------------------------------------------------------------------
	// SECOND DISPLAY VARIABLES
	//------------------------------------------------------------------------------

	// message feed
	rtsMessageFeed* m_feed;

	// world for sensor message display
	cWorld* m_world2;

	// hotspot world
	cGenericObject* m_hotspotParent2;

	double m_worldToPixel;

	// camera for 2 feed
	cCamera* m_camera2;
	  
	// a light source to illuminate the objects in the world
	cDirectionalLight* m_light2;

	// vector of scopes to show info from each building
	vector<rtsScope*> m_scopes;

	// signal ring to show team signal info
	rtsSignalRing* m_signalRing;

	// detections
	rtsDetections* m_detections;

	// virtual buildings for detections
	vector<rtsPolygon*> m_buildingsD;

	// limits on scope height
	double m_minScopeH;
	double m_maxScopeH;

	// vector of scope labels
	vector<cLabel*> m_scopeLabels;

	// panel for sensorMessage feed
	cPanel* m_sensorMessagePanel;

	// message alert clock
	cPrecisionClock m_messageAlertClock;

	// panel for user input
	cPanel* m_userInputPanel;

	// vector of labels for message feed
	deque<cLabel*> m_messages;

	// font for messages
	cFont* m_messageFont;

	// user input string
	string m_userInput;

	// all user input
	vector<string> m_userInputMessages;

	// all user input times
	vector<double> m_userInputTimes;

	// blank user input
	string m_blankUserInput;

	// user submitted message
	bool m_newUserMessage;
	bool m_newBlankUserMessage;

	// label of user input
	cLabel* m_userInputLabel;

	// simulation time limit
	int m_timeLimit;

	bool m_invertSensorMessage;

	bool m_showDetections;

	//------------------------------------------------------------------------------
	// TUTORIAL VARIABLES
	//------------------------------------------------------------------------------
	  
	// tutorial manager
	rtsTutorialManager* m_tutorialManager;

	// use tutorial
	bool m_useTutorial;

	//------------------------------------------------------------------------------
	// NASA TLX VARIABLES
	//------------------------------------------------------------------------------
	rtsNASATLX* m_tlx;

	rtsNASATLXDefinitions* m_tlxDefs;

	bool m_useNASATLX;

	//------------------------------------------------------------------------------
	// WORLD VARIABLES
	//------------------------------------------------------------------------------

	// a world that contains all objects of the virtual environment
	cWorld* m_world;

	// a camera to render the world in the window display
	cCamera* m_camera;

	// a light source to illuminate the objects in the world
	cDirectionalLight* m_light;

	double m_worldWidth;

	rtsMapScale* m_mapScale;

public:
	// indicates if the haptic simulation currently running
	bool m_simulationRunning;

	// indicates if the haptic simulation has terminated
	bool m_simulationFinished;

private:

	// frequency counter to measure the simulation haptic rate
	cFrequencyCounter m_frequencyCounter;

	// root resource path
	string m_resourceRoot;

	// panel to display simulation ready message
	cPanel* m_simReadyPanel;

	// label to tell user when simulation is ready
	cLabel* m_simReadyMessage;

	// indicates if the haptic simulation is ready
	bool m_simReady;

	// panel to display simulation ESC message
	cPanel* m_simEscPanel;

	// label to notify user of exit
	cLabel* m_simEscMessage;


	//------------------------------------------------------------------------------
	// BOOLEANS
	//------------------------------------------------------------------------------
	bool m_testSpeed;
	bool m_showToolPos;
	bool m_isNewSwarm;

	bool m_showConvexHull;
	bool m_beginSim;

	bool m_surroundIter1;
	bool m_surroundIter2;

	bool m_surroundStarted;

	bool m_travelCommanded;
	bool m_foundCycle;
	bool m_spanningCycle;


	// OPENGL VARIABLES
	int m_windowW2;
	int m_windowH2;
	int m_windowW;
	int m_windowH;

	// haptics fullscreen mode
	bool m_fullscreen;

	// secondary fullscreen mode
	bool m_fullscreen2;

	int windowPosX;
	int windowPosY;

	int windowPosX2;
	int windowPosY2;

	// minimum startup haptic rate (Hz)
	int m_minStartupHapticRate;

	// minimum startup haptic rate (Hz) for no device
	int m_minStartupHapticRateNoDevice;

	// increase radius when approaching a building
	bool m_increaseRadius;

	// new increase in radius
	bool m_newIncreaseRadiusCommand;

	// new increase in radius clock
	cPrecisionClock m_newRadiusCommandClock;

	// start increase radius process
	bool m_startIncreaseRadius;

	vector<double> m_nsewValues;
	vector<double> m_nsewCMAValues;
	int m_nCMA = 0;
	vector<double> m_pastnsewValues;
	vector<double> m_servernsewValues;
	vector<vector<double>> m_nsewBuildingBounds;
	vector<rtsPolygonBounds> m_currentBuildingBounds;
	vector<rtsPolygonBounds> m_buildingBounds;
	int m_closestBuildingIndex;
	cVector3d m_closestBuildingPos;
	int m_defaultBldgNum;
	cPrecisionClock m_nsewClock;
	volatile bool m_nsewValuesReady;
	volatile bool m_sensorReadingsReady;
	vector<bool> m_sensorReadings;
	vector<cVector3d> m_sensorPositions;
	bool m_closestBldgIndexSet;

	bool m_userExit;
};

//---------------------------------------------------------------------------
#endif //RTSAPPLICATIONH
//---------------------------------------------------------------------------
