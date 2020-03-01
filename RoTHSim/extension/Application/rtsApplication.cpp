//---------------------------------------------------------------------------
#include "rtsApplication.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of rtsApplication.

    \fn     rtsApplication::rtsApplication()
    \param  a_
*/
//===========================================================================
rtsApplication::rtsApplication(string worldFile, string userName, bool hapticsEnabled, bool visualEnabled, bool showSurroundOutline, bool showTravelOutline, bool useVisualDemo, bool useNASATLX, bool showHiddenHotspots, bool invertSensorMessage, bool useDetections, bool useDataCollection, bool zUpOriented)
{
	// initialize
	initApplication();

	// settings
	m_worldFile = worldFile;
	m_userName = userName;
	m_hapticsEnabled = hapticsEnabled;
	m_visualEnabled = visualEnabled;
	m_showSurroundOutline = showSurroundOutline;
	m_showTravelOutline = showTravelOutline;
	m_useVisualDemo = useVisualDemo;
	m_useNASATLX = useNASATLX;
	m_showHiddenHotspots = showHiddenHotspots;
	m_invertSensorMessage = invertSensorMessage;
	m_useDetections = useDetections;
	m_useDataCollection = useDataCollection;
	m_zUpOriented = zUpOriented;
}

rtsApplication::~rtsApplication()
{
    // deallocate memory
    deallocate();
}

void rtsApplication::initApplication()
{
	//------------------------------------------------------------------------------
	// SERVER VARIABLES
	//------------------------------------------------------------------------------

	// enable query mutex
	m_enableQueryMutex = false;

	m_swarmModeInt = 0;

	// minimum agent distance
	m_AIRadius = 25.0;
	m_swarmRadius = 0;

	// manipulation flag for server
	m_serverManipButton = false;

	m_serverContacted = false;

	// manipulation completed flag for server
	m_serverManipCompleted = false;

	// resize flag for server
	m_serverResizeButton = false;

	// resize completed flag for server
	m_serverResizeSurroundCompleted = false;

	// travel variables
	m_serverTravelCommanded = false;
	m_serverTravelDir1.zero();
	m_serverTravelDir2.zero();
	m_commandedTravelDir1.zero();
	m_commandedTravelDir2.zero();
	m_travelCount = 0;

	// radius variables
	m_radiusPriority = 0;
	m_serverAIRadius = m_AIRadius;

	m_pause = false;
	m_travelRadius = 3.0;
	m_surroundRadius = 45.0;
	m_radiusTimeout = 0.4;

	m_serverIncreaseRadius = false;
	m_serverResetToStartManip = false;
	m_serverResetToStartComplete = false;

	m_increaseRadius = false;
	m_newIncreaseRadiusCommand = false;
	m_startIncreaseRadius = false;

	//------------------------------------------------------------------------------
	// ROBOT TEAM VARIABLES
	//------------------------------------------------------------------------------

	// manipulation completed
	m_manipCompleted = false;

	// resize completed
	m_resizeSurroundCompleted = false;

	// reset flag
	m_manipStarted = false;
	m_resetToStartManip = false;

	//------------------------------------------------------------------------------
	// HAPTICS VARIABLES
	//------------------------------------------------------------------------------

	// distribution type variable (0 - travel, 1 - surround)
	m_distributionType = 0;

	// resize percentage
	m_resizePercentage = 1.00;

	// total resize percentage
	m_totalResizePercentage = 1.0;

	// maximum resize percentage
	m_maxResizePercentage = 1.5;

	// minimum resize percentage
	m_minResizePercentage = 0.001;


	//------------------------------------------------------------------------------
	// TOOL VARIABLES
	//------------------------------------------------------------------------------

	// number of haptic devices detected
	m_numHapticDevices = 0;

	// set min haptic rate
	#if defined(_DEBUG)
		m_minStartupHapticRate = 75;
		m_minStartupHapticRateNoDevice = 50;
	#else
		m_minStartupHapticRate = 900;
		m_minStartupHapticRateNoDevice = 400;
	#endif
	
	// simulation defaults to not ready until startup haptic rate is met
	m_simReady = false;

	//------------------------------------------------------------------------------
	// DATA VARIABLES
	//------------------------------------------------------------------------------

	// enabler for data mutex
	m_enableDataMutex = false;

	// time intervals for each iteration
	m_checkTimeIntervals = false;

	// check time interval of specific regions
	m_checkRegions = false;

	//------------------------------------------------------------------------------
	// 2 FEED VARIABLES
	//------------------------------------------------------------------------------
	m_minScopeH = 40;
	m_maxScopeH = 200;

	vector<double> zeros(4, 0.0);
	m_nsewValues = zeros;
	m_nsewCMAValues = zeros;
	m_pastnsewValues = zeros;
	m_servernsewValues = zeros;

	m_nsewValuesReady = false;
	m_sensorReadingsReady = false;
	m_closestBldgIndexSet = false;

	m_blankUserInput = ">> ";

	// user submitted message
	m_newUserMessage = false;
	m_newBlankUserMessage = false;

	m_showDetections = true;


	//------------------------------------------------------------------------------
	// WORLD VARIABLES
	//------------------------------------------------------------------------------

	// indicates if the haptic simulation currently running
	m_simulationRunning = false;

	// indicates if the haptic simulation has terminated
	m_simulationFinished = true;


	m_testSpeed = true;
	m_showToolPos = false;

	m_isNewSwarm = false;

	m_travelCommanded = false;
	m_spanningCycle = false;
	m_foundCycle = false;

	m_beginSim = false;

	m_surroundIter1 = false;
	m_surroundIter2 = false;
	m_surroundStarted = false;

	m_userExit = false;

	//------------------------------------------------------------------------------
	// DECLARED MACROS
	//------------------------------------------------------------------------------

	// convert to resource path
	#define RESOURCE_PATH(p)    (char*)((m_resourceRoot+string(p)).c_str())
}

bool rtsApplication::init(int argc, char* argv[], int windowW, int windowH, bool fullscreen, int windowW2, int windowH2, bool fullscreen2)
{
	m_windowW = windowW;
	m_windowH = windowW;
	m_windowW2 = windowW2;
	m_windowH2 = windowH2;
	m_fullscreen = fullscreen;
	m_fullscreen2 = fullscreen2;

	//-----------------------------------------------------------------------
	// INITIALIZATION
	//-----------------------------------------------------------------------

	m_showHotspotsWithTeam = m_useVisualDemo;
	
	// subject name
	string userDescriptor;
	userDescriptor = m_userName;
	userDescriptor += "_" + m_worldFile + "_";
	m_worldFile = m_worldFile + ".txt";

	// if this is a tutorial program, initialize variables
	if (!m_worldFile.compare("tutorial.txt"))
	{
		m_useTutorial = true;
	}
	else m_useTutorial = false;

	m_worldFile = "worlds/" + m_worldFile;

	m_outputFolder = "outputs/" + userDescriptor;

	// output files for user data
	string outputFile = m_outputFolder + "teamData.txt";
	string eventOutputFile = m_outputFolder + "buttonEvents.txt";

	m_dataManager = new rtsDataManager(m_worldFile, outputFile, eventOutputFile);

	// create mutex for data manager
	m_dataMutex = new std::mutex;
	m_dataManager->setMutex(m_dataMutex);

	// read in server input file name
	string serverInputFile;
	string temp;
	*m_dataManager->getFin() >> temp >> serverInputFile;

	// read in time limit (minutes)
	int minutes;
	*m_dataManager->getFin() >> temp >> minutes;
	m_timeLimit = minutes * 60; // seconds

	// read in number of buildings
	*m_dataManager->getFin() >> temp >> m_defaultBldgNum;

	// read in number of polygons per building
	*m_dataManager->getFin() >> temp;
	for (int i = 0; i < m_defaultBldgNum; i++)
	{
		int ppb;
		*m_dataManager->getFin() >> ppb;
		m_polygonsPerBldg.push_back(ppb);
	}

	// read in building bounds (nsew)
	for (int i = 0; i < m_defaultBldgNum; i++)
	{
		double n, s, e, w;
		*m_dataManager->getFin() >> temp >> n >> s >> e >> w;
		vector<double> buildingBounds;
		buildingBounds.push_back(n);
		buildingBounds.push_back(s);
		buildingBounds.push_back(e);
		buildingBounds.push_back(w);
		m_nsewBuildingBounds.push_back(buildingBounds);
	}
	
	// set current building bounds
	m_closestBuildingIndex = m_defaultBldgNum - 1;

	// read in sensorMessage feed folder
	string feedFolder;
	*m_dataManager->getFin() >> temp >> feedFolder;

	// read in camera height in world
	double cameraHeight;
	*m_dataManager->getFin() >> temp >> cameraHeight;

	// read in workspace scale factor for device
	*m_dataManager->getFin() >> temp >> m_virtualWorkspaceRadius;

	// read in device radius
	*m_dataManager->getFin() >> temp >> m_deviceRadius;

	// read in robot radius
	double robotRadius;
	*m_dataManager->getFin() >> temp >> robotRadius;

	// read in sensor radius
	double sensorRadius;
	*m_dataManager->getFin() >> temp >> sensorRadius;

	// read in sample rate
	double sampleRate;
	*m_dataManager->getFin() >> temp >> sampleRate;
	m_dataManager->setSampleRate(sampleRate);

	// start the server program
	ShellExecute(
			NULL,
			"runas", "start.exe",
			serverInputFile.c_str(),
			" ",
			SW_SHOWDEFAULT);

	// create server communicator
	m_com = new communicator("localhost");

	// verify server connection
	if (!m_com->is_valid()) {
		printf("couldn't connect to server!\n");
		system("pause");
		return false;
	}

	// pause server until simulation starts
	m_com->pause();

	// retrieve swarm from server for a first time
	m_swarm = m_com->swarm();

	// create query mutex
	m_queryMutex = new std::mutex;

	// read AI Radius from server
	m_swarmRadius = m_com->swarm()->radius();
	m_AIRadius = m_swarmRadius;

	// user instructions
	printf("\n----------------------------------------------\n");
	printf("Robot Team Haptic Simulator (RoTHSim)");
	if (m_useTutorial)
	{
		printf(": Tutorial");
	}
	printf("\n----------------------------------------------\n");
	printf("\n\nKeyboard Options:\n\n");
	if (m_useTutorial)
	{
		printf("[1] - Previous instruction\n");
		printf("[2] - Next instruction\n");
	}
	printf("[f] - Toggle fullscreen\n");
	printf("[h] - Toggle haptics\n");
	printf("[p] - Pause/unpause server\n");
	printf("[r] - Toggle show robot team\n");
	printf("[s] - Toggle show playdoh\n");
	printf("[t] - Enable/disable travel\n");
	if (m_useTutorial)
	{
		printf("[w] - Pause or unpause tutorial\n");
	}
	printf("[ESC] - Exit\n");

	printf("\nDevice buttons:\n\n");
	printf("[left] TRAVEL\n");
	printf("[middle] SHAPE\n");
	printf("[right] SPACING\n");
	printf("\n\n");

	// parse first arg to try and locate resources
	m_resourceRoot = string(argv[0]).substr(0, string(argv[0]).find_last_of("/\\") + 1);

	//--------------------------------------------------------------------------
	// WORLD - CAMERA - LIGHTING FOR 2 FEED
	//--------------------------------------------------------------------------

	// create world for 2 feed
	m_world2 = new cWorld();

	// set the background color of the environment
	m_world2->m_backgroundColor.setGray();

	// create a camera and insert it into the virtual world
	m_camera2 = new cCamera(m_world2);
	m_world2->addChild(m_camera2);

	// position and oriente the camera
	m_camera2->set(cVector3d(0.0, 0.0, 1.0),    // camera position (eye)
		cVector3d(0.0, 0.0, 0.0),    // lookat position (hotspot)
		cVector3d(0.0, 1.0, 0.0));   // direction of the "up" vector

	m_camera2->setClippingPlanes(0.01, 100);

	// create a light source
	m_light2 = new cDirectionalLight(m_world2);

	// add light to world
	m_world2->addChild(m_light2);

	// enable light source
	m_light2->setEnabled(true);

	// define the direction of the light beam
	m_light2->setDir(-1.0, 0.0, -0.5);

	// set lighting conditions
	m_light2->m_ambient.set(0.4f, 0.4f, 0.4f);
	m_light2->m_diffuse.set(0.8f, 0.8f, 0.8f);
	m_light2->m_specular.set(1.0f, 1.0f, 1.0f);

	//--------------------------------------------------------------------------
	// WORLD - CAMERA - LIGHTING
	//--------------------------------------------------------------------------

	// create a new world.
	m_world = new cWorld();

	// set the background color of the environment
	m_world->m_backgroundColor.setGray();

	// create a camera and insert it into the virtual world
	m_camera = new cCamera(m_world);
	m_world->addChild(m_camera);

	// position and oriente the camera
	m_camera->set(cVector3d(0.0, 0.0, cameraHeight),    // camera position (eye)
		cVector3d(0.0, 0.0, 0.0),    // lookat position (hotspot)
		cVector3d(0.0, 1.0, 0.0));   // direction of the "up" vector (defines camera z direction)

	// position camera projection sphere
	m_cameraProjSphere = new cShapeSphere(1.0);
	m_camera->addChild(m_cameraProjSphere);
	cVector3d camProjVec = m_camera->getLocalPos();

	// orient camera projection sphere
	double zShift = 40; // for greater workspace above ground plane
	if (m_zUpOriented)
	{
		cVector3d offset(-11, -22.5, 8); // specific to Novint Falcon
		m_cameraProjSphere->translate(cVector3d(zShift-cameraHeight, 0, 0)+offset);
		m_cameraProjSphere->rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);
	}
	else
	{
		cVector3d offset(-10, -10, -10); // specific to Novint Falcon
		m_cameraProjSphere->translate(cVector3d(zShift - cameraHeight, 0, 0) + offset);
		//m_cameraProjSphere->rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);
		//m_cameraProjSphere->rotateAboutLocalAxisDeg(cVector3d(0, 0, 1), 90);
	}
	//m_cameraProjSphere->setLocalPos(camProjVec);
	m_cameraProjSphere->setShowEnabled(false, false);
	m_cameraProjSphere->m_material->m_ambient.setGray();

	// set the near and far clipping planes of the camera
	// anything in front/behind these clipping planes will not be rendered
	double clipDist = 0.25*cameraHeight;
	double closeDist = -(cameraHeight + clipDist);
	double farDist = (cameraHeight + clipDist);
	m_camera->setClippingPlanes(closeDist, farDist);

	m_worldWidth = 2.5 * m_virtualWorkspaceRadius;
	m_camera->setOrthographicView(m_worldWidth);

	// disable multi-pass rendering to handle transparent objects
	m_camera->setUseMultipassTransparency(true);

	// create a light source
	m_light = new cDirectionalLight(m_world);

	// add light to world
	m_world->addChild(m_light);

	// enable light source
	m_light->setEnabled(true);

	m_light->setLocalPos(cVector3d(2.0*cameraHeight, 0.5*cameraHeight, 1.0*cameraHeight));  // position the light source

	// define the direction of the light beam
	m_light->setDir(-1.0, 0.0, -0.5);

	//--------------------------------------------------------------------------
	// HAPTIC DEVICES / TOOLS
	//--------------------------------------------------------------------------

	// create a haptic device handler
	m_handler = new cHapticDeviceHandler();

	// get access to the first available haptic device
	m_handler->getDevice(m_hapticDevice, 0);

	// get number of haptic devices
	m_numHapticDevices = m_handler->getNumDevices();

	// retrieve information about the current haptic device
	cHapticDeviceInfo hapticDeviceInfo = m_hapticDevice->getSpecifications();

	// create a tool (cursor) and insert into the world
	m_tool = new cToolCursor(m_world);
	m_cameraProjSphere->addChild(m_tool);

	// connect the haptic device to the tool
	m_tool->setHapticDevice(m_hapticDevice);

	// define a radius for the tool
	double toolRadius = 0.1;

	// set tool radius
	m_tool->setRadius(m_deviceRadius);

	// map the physical workspace of the haptic device to a larger virtual workspace.
	m_tool->setWorkspaceRadius(m_virtualWorkspaceRadius);

	// start the haptic tool
	m_tool->start();


	// read the scale factor between the physical workspace of the haptic
	// device and the virtual workspace defined for the tool
	m_toolWorkspaceScaleFactor = m_tool->getWorkspaceScaleFactor();

	// maximum stiffness property
	m_maxStiffness = hapticDeviceInfo.m_maxLinearStiffness / m_toolWorkspaceScaleFactor;

	// maximum damping property
	m_maxDamping = hapticDeviceInfo.m_maxLinearDamping / m_toolWorkspaceScaleFactor;

	if (hapticDeviceInfo.m_sensedRotation)
	{
		// display a reference frame
		m_tool->setShowFrame(true);

		// set the size of the reference frame
		m_tool->setFrameSize(2 * m_deviceRadius);
	}

	// set a color for haptic point (cursor)
	m_tool->m_hapticPoint->m_sphereProxy->m_material->setColor(cColorf(1, 1, 1));

	//--------------------------------------------------------------------------
	// WIDGETS
	//--------------------------------------------------------------------------

	// create a font
	cFont *font = NEW_CFONTCALIBRI20();

	// create a label to display the haptic rate of the simulation
	m_labelHapticRate = new cLabel(font);
	m_camera->m_frontLayer->addChild(m_labelHapticRate);

	// set font color
	m_labelHapticRate->m_fontColor.setGrayLevel(0.4f);


	// create map scale widget
	vector<int> markerVals;
	for (int i = 0; i < 6; i++)
	{
		markerVals.push_back(i * 10);
	}
	m_mapScale = new rtsMapScale(m_worldWidth, markerVals, m_windowW, m_windowH, 10, 2, "m");
	m_camera->m_frontLayer->addChild(m_mapScale);
	m_mapScale->setLocalPos(20, 20, 0);

	//// create a label with a small message
	//m_labelMessage = new cLabel(font);
	//m_camera->m_frontLayer->addChild(m_labelMessage);

	//// set font color
	//m_labelMessage->m_fontColor.setGrayLevel(0.4f);

	//// set text message
	//m_labelMessage->setText("move device cursor to interact with team");

	// simulation ready message
	m_simReadyPanel = new cPanel();
	m_simReadyPanel->setLocalPos(0, 0);
	int simReadyPanelHeight = 50;
	m_simReadyPanel->setSize(m_windowW, simReadyPanelHeight);
	cColorf colorR;
	colorR.setGrayDarkSlate();
	m_simReadyPanel->setCornerColors(colorR, colorR, colorR, colorR);
	m_simReadyPanel->setCornerRadius(10, 10, 10, 10);
	m_camera->m_frontLayer->addChild(m_simReadyPanel);

	m_simReadyMessage = new cLabel(font);
	m_simReadyMessage->setText("Loading simulation...");
	m_simReadyMessage->m_fontColor.setWhite();
	m_simReadyMessage->setLocalPos((m_windowW - m_simReadyMessage->getTextWidth()) / 2, (simReadyPanelHeight - m_simReadyMessage->getTextHeight()) / 2);
	m_simReadyPanel->addChild(m_simReadyMessage);

	m_simReadyPanel->setShowEnabled(true);

	// simulation esc message
	m_simEscPanel = new cPanel();
	m_simEscPanel->setLocalPos(0, m_windowH / 2);
	double simEscPanelHeight = 50.0;
	m_simEscPanel->setSize(m_windowW, simEscPanelHeight);
	cColorf colorE;
	colorE.setRedDark();
	m_simEscPanel->setCornerColors(colorE, colorE, colorE, colorE);
	m_simEscPanel->setCornerRadius(10, 10, 10, 10);
	m_camera->m_frontLayer->addChild(m_simEscPanel);

	m_simEscMessage = new cLabel(font);
	m_simEscMessage->setText("Are you sure? Press ESC to exit.");
	m_simEscMessage->m_fontColor.setWhite();
	m_simEscMessage->setLocalPos((m_windowW - m_simEscMessage->getTextWidth()) / 2, (simEscPanelHeight - m_simEscMessage->getTextHeight()) / 2);
	m_simEscPanel->addChild(m_simEscMessage);

	m_simEscPanel->setShowEnabled(false);

	//--------------------------------------------------------------------------
	// CREATE ROBOT TEAM
	//--------------------------------------------------------------------------

	// retrieve robot positions from server
	m_robotPositions = swarm2Vec(m_swarm, false);

	m_playdohRobotPositions = m_robotPositions;

	// create robot team
	m_robotTeam = new rtsRobotTeam(m_world);

	// create playdoh robot team
	m_playdohRobotTeam = new rtsRobotTeam(m_world);

	// add robots to team
	for (unsigned int i = 0; i < m_robotPositions.size(); i++)
	{
		// create robot object and add it to world
		rtsRobot* robot = new rtsRobot(robotRadius, m_robotPositions[m_ids[i]]);
		m_world->addChild(robot);
		
		// create sensor
		double sensorToRadiusRatio = 4.0;
		robot->initSensor(0.50, 0.80, 0.10, sensorRadius, sensorToRadiusRatio*robotRadius, m_useVisualDemo);
		robot->setSensorRatio(sensorToRadiusRatio);

		// add the robot object to the team
		m_robotTeam->addRobot(robot);
		m_robotTeam->getRobot(i)->setIdNumber(m_ids[i]);

		// create a playdoh robot and add it to world
		rtsRobot* pRobot = new rtsRobot(robotRadius, m_robotPositions[m_ids[i]]);
		m_world->addChild(pRobot);
		pRobot->setShowEnabled(false);

		// add the robot object to the playdoh team
		m_playdohRobotTeam->addRobot(pRobot);
		m_playdohRobotTeam->getRobot(i)->setIdNumber(m_ids[i]);
	}

	// set robot positions to compute convex hull positions
	m_robotTeam->setRobotPositions(m_robotPositions, m_ids);
	
	// set color of all robots
	m_robotTeam->setRobotColors(cColorf(1.0, 0.0, 1.0, 1.0f));

	// show or hide convex hull lines
	m_robotTeam->setShowConvexHull(m_showTravelOutline);

	// create surround lines
	m_robotTeam->createSurroundLines();

	// show id numbers
	m_robotTeam->setShowId(false);

	// set color of all playdoh robots
	m_playdohRobotTeam->setRobotColors(cColorf(0.0, 1.0, 0.0, 1.0f));
	m_playdohRobotTeam->setShowEnabled(false);
	m_playdohRobotTeam->setShowId(false);

	// create controller
	double kp = 0.030; // 0.025
	double kd = -0.000;
	m_controller = new rtsController(m_robotPositions.size(), m_world, m_com, kp, kd, m_toolWorkspaceScaleFactor);

	// enable travel command control at first
	m_travelEnabled = true;
	m_travelEnabledClock.setTimeoutPeriodSeconds(30.0);
	m_travelEnabledClock.reset();

	//--------------------------------------------------------------------------
	// HAPTIC FORCE NODE MODEL (PLAYDOH)
	//--------------------------------------------------------------------------

	m_playdoh = new rtsClay(m_world, m_robotPositions, m_deviceRadius, m_toolWorkspaceScaleFactor, m_maxStiffness);

	// set convex hull in playdoh
	m_playdoh->setConvexHull(m_robotTeam->getConvexHull());
	m_playdoh->setConvexHullMapping(m_robotTeam->getConvexHullMapping());

	// initialize playdoh
	m_playdoh->initPlaydoh();

	// set up mutex
	m_mtx = new std::mutex;
	m_playdoh->setMutex(m_mtx);

	// choose to show playdoh at startup
	m_playdoh->setShowEnabled(false, true);
	m_playdoh->setHapticEnabled(false, true);

	// adjust robotRadius from playdoh scale factor
	double agentRadius = agentRadius = m_robotTeam->getRobot(0)->getRadius();
	//m_robotTeam->setRadii(agentRadius);

	m_playdohRobotTeam->setRadii(agentRadius);
	
	//-----------------------------------------------------------------------
	// DETECTIONS
	//-----------------------------------------------------------------------

	m_worldToPixel = 5.65;
	if (m_useDetections)
	{
		m_detections = new rtsDetections(m_robotTeam->size(), m_worldToPixel*sensorRadius, m_worldToPixel*agentRadius, true);
		m_detections->setWorldToPixelFactor(m_worldToPixel);
		m_camera2->m_frontLayer->addChild(m_detections);
		m_detections->setShowEnabled(false);
	}

	//-----------------------------------------------------------------------
	// CREATE TARGETS IN WORLD
	//-----------------------------------------------------------------------

	if (m_dataManager->fin_is_open())
	{
		double hotspotRadius = 2.0;
		if (m_showHotspotsWithTeam)
		{
			m_hotspotManager = new rtsHotspotManager(m_world);
		}
		else
		{
			m_hotspotParent2 = new cGenericObject();
			m_camera2->m_frontLayer->addChild(m_hotspotParent2);
			m_hotspotManager = new rtsHotspotManager(m_hotspotParent2);
			hotspotRadius *= m_worldToPixel;
		}

		// read all the hotspot start times, end times, and locations
        vector<rtsHotspot*> allTargets;
        vector<int> changeGroupIdIndices;
        int currentId = 1;
        int previousId = -1;
        int i = 0;
		int largestGroupNum = 1;
		while (!m_dataManager->getFin()->eof())
		{
			int b, g;
			double d, f, s, e, x, y, z;
			*m_dataManager->getFin() >> b >> g >> d >> f >> s >> e >> x >> y >> z;

            rtsHotspot* hotspot = new rtsHotspot(hotspotRadius, s, e);
			if (m_showHotspotsWithTeam)
			{
				hotspot->setLocalPos(x, y, z);
			}
			else
			{
				hotspot->setLocalPos(m_worldToPixel*x, m_worldToPixel*y, m_worldToPixel*z);
			}
			hotspot->setTrueLocalPos(cVector3d(x, y, z));
            hotspot->setGroupId(g);
			hotspot->setBldgId(b);
            allTargets.push_back(hotspot);
            
            currentId = g;
            
            if (currentId != previousId)
            {
                changeGroupIdIndices.push_back(i);
            }

			// assign largest group number
			if (currentId > largestGroupNum)
				largestGroupNum = currentId;

            previousId  = currentId;
            i++;
		}
        
        // sort all hotspots into groups
        vector<rtsHotspotGroup*> allTargetGroups;
		for (int i = 1; i <= largestGroupNum; i++)
		{
			allTargetGroups.push_back(new rtsHotspotGroup(i, m_showHiddenHotspots));
		}

        int j = 0;
        int numAllTargets = allTargets.size();
		for (int i = 0; i < numAllTargets; i++)
		{
			int hotspotGroupIndex = allTargets[i]->getGroupId() - 1;
			if (hotspotGroupIndex >= 0)
			{ 
				allTargetGroups[hotspotGroupIndex]->addTarget(allTargets[i]);

				// assuming all hotspots in group have the same start and end time
				allTargetGroups[hotspotGroupIndex]->setStartAndEndTimes(allTargets[i]->getStartTime(), allTargets[i]->getEndTime());

				allTargetGroups[hotspotGroupIndex]->setDifficulty(allTargets[i]->getDifficulty());

				// assuming all hotspots in group have the same bldg id
				allTargetGroups[hotspotGroupIndex]->setBldgId(allTargets[i]->getBldgId());
			}
		}
        
        // add hotspot groups to hotspot manager
        m_hotspotManager->setTargetGroups(allTargetGroups);
        allTargetGroups.clear();
        allTargets.clear();
	}
	else
	{
		printf("Target locations couldn't load from specified file.\n");

		return 1;
	}

	// close input file
	m_dataManager->fin_close();

	//-----------------------------------------------------------------------
	// COMPOSE THE GROUND
	//-----------------------------------------------------------------------

	m_ground = new cMesh();

	// create a plane for ground
	cCreatePlane(m_ground, 5 * m_virtualWorkspaceRadius, 5 * m_virtualWorkspaceRadius);

	// create collision detector
	m_ground->createBruteForceCollisionDetector();

	m_ground->setLocalPos(0, 0, 0);

	m_ground->setShowEnabled(false);

	// create texture property
	cTexture2dPtr texture = cTexture2d::create();
	m_ground->setTexture(texture);

	// set haptic properties
	cColorf groundColor;
	groundColor.setGray();
	m_ground->m_material->setColor(groundColor);
	if (m_useServerThread)
		m_ground->m_material->setStiffness(0.50 * m_maxStiffness);
	else m_ground->m_material->setStiffness(0.25 * m_maxStiffness);

	m_ground->m_material->setStaticFriction(0.0);
	m_ground->m_material->setDynamicFriction(0.0);

	//ground->setVertexColor(cColorf(0.3, 0.0, 0.0));


	// add object to world
	m_world->addChild(m_ground);

	//-----------------------------------------------------------------------
	// POPULATE WITH BUILDINGS
	//-----------------------------------------------------------------------

	// set a building height
	double buildingHeight = 10.0;

	// retrieve robot positions from server
	vector<vector<cVector3d>> polygons = polygons2Vec(m_com->polygons());

	for (unsigned int k = 0; k < polygons.size(); ++k)
	{
		// get polygon nodes for this building
		vector<cVector3d> buildingNodes;
		buildingNodes = polygons[k];

		//
		rtsPolygonBounds bounds(buildingNodes);
		m_buildingBounds.push_back(bounds);

		// compute triangle node order
		vector<cVector3d> triangleNodes;
		bool convex = true; // assume convex buildings for now
		if (convex)
		{
			for (unsigned int i = 1; i < buildingNodes.size() - 1; i++)
			{
				triangleNodes.push_back(buildingNodes[0]);
				triangleNodes.push_back(buildingNodes[i]);
				triangleNodes.push_back(buildingNodes[i + 1]);
			}
		}

		rtsPolygon* newBuilding = new rtsPolygon(buildingNodes, triangleNodes, buildingHeight);
		newBuilding->createPolygon();
		m_world->addChild(newBuilding);
		newBuilding->setColor(cColorf(0.6f, 0.6f, 0.6f, 1.0f));
		newBuilding->setHapticEnabled(false);
		m_buildings.push_back(newBuilding);

		if (m_useDetections)
		{		
			// convert building nodes from world to pixel coordinates
			int numNodes = buildingNodes.size();
			for (int i = 0; i < numNodes; i++)
			{
				buildingNodes[i] = m_worldToPixel*buildingNodes[i];
			}

			// compute triangle node order
			vector<cVector3d> triangleNodesD;
			bool convex = true; // assume convex buildings for now
			if (convex)
			{
				for (unsigned int i = 1; i < buildingNodes.size() - 1; i++)
				{
					triangleNodesD.push_back(buildingNodes[0]);
					triangleNodesD.push_back(buildingNodes[i]);
					triangleNodesD.push_back(buildingNodes[i + 1]);
				}
			}

			// create buildings to use in sensorMessage display with detections
			rtsPolygon* newBuildingD = new rtsPolygon(buildingNodes, triangleNodesD, buildingHeight*m_worldToPixel);
			newBuildingD->createPolygon();
			m_detections->addChild(newBuildingD);
			newBuildingD->setColor(cColorf(0.6f, 0.6f, 0.6f, 1.0f));
			newBuildingD->setHapticEnabled(false);
			newBuildingD->setShowEnabled(false);
			m_buildingsD.push_back(newBuildingD);
		}
	}

	//--------------------------------------------------------------------------
	// MESSAGE FEED
	//--------------------------------------------------------------------------

	string filename = feedFolder + "messages.txt";
	m_feed = new rtsMessageFeed(filename);

	//--------------------------------------------------------------------------
	// SCOPES AND WIDGETS FOR 2 FEED
	//--------------------------------------------------------------------------

	if (m_useTutorial)
	{
		m_simClock = new rtsDigitalClock("Simulation Time - ", "", true, m_timeLimit, false);
	}
	else m_simClock = new rtsDigitalClock("Simulation Time - ", "", true, m_timeLimit, true);
	cColorf dcColor; dcColor.setGrayDark();
	m_simClock->setCornerColors(dcColor, dcColor, dcColor, dcColor);
	m_simClock->setCornerRadius(4, 4, 4, 4);
	m_simClock->setMarginSize(3);

	double simClockW = m_simClock->getWidth();
	double simClockH = m_simClock->getHeight();
	m_camera2->m_frontLayer->addChild(m_simClock);
	m_simClock->setSize(simClockW, simClockH);

	double vSpacing = 10;
	double hSpacing = 10;
	double totalScopeVSpace = windowH2 - (vSpacing + simClockH);
	double totalScopeHSpace = 0.6 * windowW2;

	double scopeW = (totalScopeHSpace - 5 * hSpacing) / 2;
	double scopeH = totalScopeVSpace / 7;
	scopeH = cClamp(scopeH, m_minScopeH, m_maxScopeH);

	double shiftV = 90.0;

	// create a font
	cFont* scopeFont = NEW_CFONTCALIBRI24();
	for (int i = 0; i < 4; i++)
	{
		// create a label for this scope
		cLabel* label = new cLabel(scopeFont);
		string labelText;

		// create scope
		rtsScope* scope = new rtsScope();
		m_camera2->m_frontLayer->addChild(scope);

		switch (i)
		{
		case 0:
			scope->setLocalPos(totalScopeHSpace / 2 - scopeW / 2, totalScopeVSpace / 2 + scopeH / 2 + vSpacing + shiftV);
			label->setText("North");
			label->setLocalPos((scopeW - label->getWidth()) / 2, scopeH);
			break;
		case 1:
			scope->setLocalPos(totalScopeHSpace / 2 - scopeW / 2, totalScopeVSpace / 2 - 3 * scopeH / 2 - vSpacing + shiftV);
			label->setText("South");
			label->setLocalPos((scopeW - label->getWidth()) / 2, -label->getHeight());
			break;
		case 2:
			scope->setLocalPos(hSpacing * 4 + scopeW, totalScopeVSpace / 2 - scopeH / 2 + shiftV);
			label->setText("East");
			label->setLocalPos(scopeW - label->getWidth() - hSpacing, scopeH);
			break;
		case 3:
			scope->setLocalPos(hSpacing, totalScopeVSpace / 2 - scopeH / 2 + shiftV);
			label->setText("West");
			label->setLocalPos(hSpacing, scopeH);
			break;
		}

		cColorf scopeColor; scopeColor.setWhiteBeige();
		scope->setBaselineFadeColor(scopeColor);
		scope->setCornerColors(scopeColor, scopeColor, scopeColor, scopeColor);
		scope->setSize(scopeW, scopeH);
		scope->setRange(0.0, 1.0);
		scope->setSignalEnabled(true, false, false, false);
		scope->setTransparencyLevel(0.7f);
		scope->setLineWidth(3.0);
		scope->m_colorSignal0 = cColorf(0.8f, 0.0f, 0.0f);
		m_scopes.push_back(scope);

		// add label to scope list of children
		scope->addChild(label);
		m_scopeLabels.push_back(label);

	}

	if (!m_useDetections)
	{
		m_signalRing = new rtsSignalRing(75.0, 75.0);
		m_signalRing->setLocalPos(totalScopeHSpace / 2, 2 * shiftV, 0);
		m_camera2->m_frontLayer->addChild(m_signalRing);
		m_signalRing->rotateAboutLocalAxisDeg(cVector3d(0, 0, 1), -45);
		cColorf ringColor; ringColor.setWhite();
		m_signalRing->setBaselineFadeColor(ringColor);
		m_signalRing->setColors(ringColor, ringColor, ringColor, ringColor);
	}

	m_simClock->setLocalPos((totalScopeHSpace - m_simClock->getWidth()) / 2, m_windowH2 - simClockH - vSpacing);


	// panel parameters
	double userInputPanelPosY = vSpacing;
	double userInputPanelH = 35;
	double panelW = windowW2 - totalScopeHSpace;
	double panelH = windowH2 - 2.5 * vSpacing - userInputPanelH;
	double panelPosX = totalScopeHSpace;
	double panelPosY = vSpacing + userInputPanelH + 0.5*vSpacing;


	//--------------------------------------------------------------------------
	// PANEL FOR USER INPUT
	//--------------------------------------------------------------------------

	m_userInputPanel = new cPanel();
	m_userInputPanel->setLocalPos(panelPosX, userInputPanelPosY);
	m_userInputPanel->setSize(panelW, userInputPanelH);
	cColorf panelColor;
	panelColor.setBlack();
	m_userInputPanel->setCornerColors(panelColor, panelColor, panelColor, panelColor);
	m_userInputPanel->setCornerRadius(10, 10, 10, 10);
	m_camera2->m_frontLayer->addChild(m_userInputPanel);

	// set message font
	m_messageFont = NEW_CFONTCALIBRI24();

	// set user input label
	m_userInput = m_blankUserInput;
	m_userInputLabel = new cLabel(m_messageFont);
	m_userInputLabel->setLocalPos(hSpacing, vSpacing, 0);
	m_userInputLabel->setText(m_userInput);
	m_userInputPanel->addChild(m_userInputLabel);

	//--------------------------------------------------------------------------
	// PANEL FOR 2 FEED
	//--------------------------------------------------------------------------

	m_sensorMessagePanel = new cPanel();
	m_sensorMessagePanel->setLocalPos(panelPosX, panelPosY);
	m_sensorMessagePanel->setSize(panelW, panelH);
	m_sensorMessagePanel->setCornerColors(panelColor, panelColor, panelColor, panelColor);
	m_camera2->m_frontLayer->addChild(m_sensorMessagePanel);
	m_sensorMessagePanel->setCornerRadius(10, 10, 10, 10);

	//--------------------------------------------------------------------------
	// DEMO
	//--------------------------------------------------------------------------

	if (m_useTutorial)
	{
		m_tutorialManager = new rtsTutorialManager(m_windowW, m_windowH, m_camera, m_tool, m_playdoh, m_buildings,
											m_windowW2, m_windowH2, m_camera2, m_scopes, m_sensorMessagePanel, m_userInputPanel, m_com, m_hotspotManager, m_zUpOriented, !m_invertSensorMessage);
	}

	//--------------------------------------------------------------------------
	// NASA TLX
	//--------------------------------------------------------------------------
	m_tlx = new rtsNASATLX(m_camera, m_windowW, m_windowH, m_outputFolder + "weights.txt", 
															m_outputFolder + "ratings.txt", 
															m_outputFolder + "workloadScore.txt");

	m_tlxDefs = new rtsNASATLXDefinitions(m_camera2, m_windowW2, m_windowH2);
	
	return true;
}

bool rtsApplication::startSim()
{

	//--------------------------------------------------------------------------
	// START SIMULATION
	//--------------------------------------------------------------------------

	// create a thread which starts the main haptics rendering loop
	m_simulationFinished = false;

	// turn server funcionality off until user decides to begin simulation
	m_useServer = false;

	// begin simulation automatically, if hard-coded to do so
	if (m_beginSim)
	{
		m_com->unpause();
		m_useServer = true;
	}

	return true;
}

bool rtsApplication::startClock()
{
	//// start data manager clock
	//m_dataManager->resetClock();
	//m_dataManager->startClock();

	// start server timer
	double serverTimeout = 0.02; // hcval
	m_serverTimer.setTimeoutPeriodSeconds(serverTimeout);
	m_serverTimer.reset();
	m_serverTimer.start();

	// start cycle counter
	double cycleTimeout = 2.0; // hcval
	m_cycleCounter.setTimeoutPeriodSeconds(cycleTimeout);
	//m_cycleCounter.reset();
	//m_cycleCounter.start();

	return true;
}

void rtsApplication::updateServer()
{
    // initialize server frequency counter
    m_serverFreqCounter.reset();
    
    while (m_simulationRunning)
    {
        if (m_enableQueryMutex)
        {
            m_queryMutex->lock();
            m_enableQueryMutex = false;
            m_queryMutex->unlock();
        }
        else
        {
            bool serverUpdate = false;
            // query server at a certain frequency
            if (m_serverTimer.timeoutOccurred())
            {
                serverUpdate = true;
                m_serverTimer.reset();
                m_serverTimer.start();
            }
            
            if (serverUpdate)
            {
                // query server for swarm pointer
                m_swarm = m_com->swarm();
                if (!m_isNewSwarm)
                    m_isNewSwarm = true;
                
            }
            
			if (m_serverIncreaseRadius)
			{
				m_com->radius(m_radiusPriority, 0, m_serverAIRadius);
				m_radiusPriority++;
			}

            if (m_serverTravelCommanded)
            {                
                m_com->patrol(m_travelCount, m_serverTravelId1, m_serverTravelDir1.x(), m_serverTravelDir1.y(), m_serverTravelDir1.z());
                m_travelCount++;
                
                m_com->patrol(m_travelCount, m_serverTravelId2, m_serverTravelDir2.x(), m_serverTravelDir2.y(), m_serverTravelDir2.z());
                m_travelCount++;
                
                m_com->radius(m_radiusPriority, 0, m_serverAIRadius);
                m_radiusPriority++;
                
                m_serverTravelCommanded = false;
            }
			else if (m_serverResetToStartManip)
			{
				m_serverResetToStartComplete = m_controller->updateForces(m_serverDesiredPositions, m_serverCurrentPositions);
				if (m_serverResetToStartComplete)
				{
					m_controller->resetForces();
				}
				
			}
            else if ((m_serverManipButton && m_serverContacted) || m_serverManipCompleted)
			//else if (m_serverManipButton || m_serverManipCompleted)
            {
                // update controller with manip desired positions
                m_controller->updateForces(m_serverDesiredPositions, m_serverCurrentPositions);
            }
			else if (m_serverResizeButton || m_serverResizeSurroundCompleted)
			{
				// update controller with resized desired positions
				m_controller->updateForces(m_serverDesiredPositions, m_serverCurrentPositions);
			}
            else
            {
                m_controller->zeroLines();
            }
        }
    }
}
//---------------------------------------------------------------------------

void rtsApplication::dataCollection()
{
    while (m_simulationRunning)
    {
        if (m_enableDataMutex)
        {
            m_dataMutex->lock();
            m_enableDataMutex = false;
            m_dataMutex->unlock();
        }
        else
        {
            // write and pop back the oldest piece of data
            if (!m_dataStack.empty())
            {
                m_dataManager->writeData(m_dataStack[0]);
                m_dataStack.pop_front();
            }
        }
    }
}

//------------------------------------------------------------------------------

void rtsApplication::resizeWindow2(int w, int h)
{
    m_windowW2 = w;
    m_windowH2 = h;
    
	// simulation clock
	double simClockW = m_simClock->getWidth();
	double simClockH = m_simClock->getHeight();
	m_simClock->updateSize();

    // update size and positions of scopes and labels
    double vSpacing = 10;
    double hSpacing = 10;
	double totalScopeVSpace = m_windowH2 - (vSpacing + simClockH);
	double totalScopeHSpace = 0.6 * m_windowW2;

	double scopeW = (totalScopeHSpace - 5 * hSpacing) / 2;
	double scopeH = totalScopeVSpace / 10;
	scopeH = cClamp(scopeH, m_minScopeH, m_maxScopeH);

	// panel sizing parameters
	double userInputPanelPosY = vSpacing;
	double userInputPanelH = 35;
	double panelW = m_windowW2 - totalScopeHSpace;
	double panelH = m_windowH2 - 2.5 * vSpacing - userInputPanelH;

	double panelPosX;
	double scopeShiftX;
	double panelPosY = vSpacing + userInputPanelH + 0.5*vSpacing;
	if (m_invertSensorMessage)
	{
		panelPosX = 0.0;
		scopeShiftX = panelW;
	}
	else
	{
		panelPosX = totalScopeHSpace;
		scopeShiftX = 0.0;
	}

	double shiftV = 90.0;
	if (m_useDetections)
	{
		//shiftV = -(totalScopeVSpace / 2 - 3 * scopeH / 2 - vSpacing) + m_scopeLabels[0]->getHeight(); // allign to bottom
		shiftV = (totalScopeVSpace / 2 - 3 * scopeH / 2 - vSpacing) - m_scopeLabels[0]->getHeight(); // allign to top
	}

    int numScopes = m_scopes.size();
    for (int i = 0; i < numScopes; i++)
    {
		//m_scopeLabels[i]->setLocalPos(-m_scopeLabels[i]->getWidth() - hSpacing, (scopeH - m_scopeLabels[i]->getHeight()) / 2);

		switch (i)
		{
		case 0:
			m_scopes[i]->setLocalPos(scopeShiftX + totalScopeHSpace / 2 - scopeW / 2, totalScopeVSpace / 2 + scopeH / 2 + vSpacing + shiftV);
			m_scopeLabels[i]->setLocalPos((scopeW - m_scopeLabels[i]->getWidth()) / 2, scopeH);
			break;
		case 1:
			m_scopes[i]->setLocalPos(scopeShiftX + totalScopeHSpace / 2 - scopeW / 2, totalScopeVSpace / 2 - 3 * scopeH / 2 - vSpacing + shiftV);
			m_scopeLabels[i]->setLocalPos((scopeW - m_scopeLabels[i]->getWidth()) / 2, -m_scopeLabels[i]->getHeight());
			break;
		case 2:
			m_scopes[i]->setLocalPos(scopeShiftX + hSpacing * 4 + scopeW, totalScopeVSpace / 2 - scopeH / 2 + shiftV);
			m_scopeLabels[i]->setLocalPos(scopeW - m_scopeLabels[i]->getWidth() - hSpacing, scopeH);
			break;
		case 3:
			m_scopes[i]->setLocalPos(scopeShiftX + hSpacing, totalScopeVSpace / 2 - scopeH / 2 + shiftV);
			m_scopeLabels[i]->setLocalPos(hSpacing, scopeH);
			break;
		}

		//m_scopes[i]->setLocalPos(2 * hSpacing + m_scopeLabels[i]->getWidth(), totalScopeVSpace - (i + 1)*(scopeH + vSpacing));
		m_scopes[i]->setSize(scopeW, scopeH);
    }

	if (m_useDetections)
	{
		if (m_useTutorial)
			m_detections->setLocalPos(scopeShiftX + totalScopeHSpace / 2, (totalScopeVSpace - scopeH * 3 - m_scopeLabels[0]->getHeight() * 2 - 2 * vSpacing) / 2 - 20);
		else m_detections->setLocalPos(scopeShiftX + totalScopeHSpace / 2, (totalScopeVSpace - scopeH * 3 - m_scopeLabels[0]->getHeight() * 2 - 2 * vSpacing) / 2);
	}
	else
	{
		m_signalRing->setLocalPos(scopeShiftX + totalScopeHSpace / 2, 2 * shiftV, 0);
	}

	m_simClock->setLocalPos(scopeShiftX + (totalScopeHSpace - m_simClock->getWidth()) / 2, m_windowH2 - simClockH - vSpacing);


    
    // update user input panel size and position
    m_userInputPanel->setLocalPos(panelPosX, userInputPanelPosY);
    m_userInputPanel->setSize(panelW, userInputPanelH);
    
    // update user input label
    m_userInputLabel->setLocalPos(hSpacing, vSpacing, 0);
    m_userInputLabel->setText(m_userInput);
    
    // update sensorMessage message panel size and position
    m_sensorMessagePanel->setLocalPos(panelPosX, panelPosY);
    m_sensorMessagePanel->setSize(panelW, panelH);
    
    // resize message labels
    int num2Messages = m_messages.size();
    for (int i = 0; i < num2Messages; i++)
    {
        m_messages[i]->setLocalPos(hSpacing, panelH - (i + 1) * m_messages[i]->getHeight());
    }

	if (m_useTutorial)
	{
		m_tutorialManager->resizeInstructions2(w, h);
		m_tutorialManager->resizeVisuals2(w, h);
	}
}

void rtsApplication::resizeWindow(int w, int h)
{
    m_windowW = w;
    m_windowH = h;

	m_simReadyPanel->setLocalPos(0, 0);
	int simReadyPanelHeight = 50;
	m_simReadyPanel->setSize(m_windowW, simReadyPanelHeight);

	m_simReadyMessage->m_fontColor.setWhite();
	m_simReadyMessage->setLocalPos((m_windowW - m_simReadyMessage->getTextWidth()) / 2, (simReadyPanelHeight - m_simReadyMessage->getTextHeight()) / 2);

	if (m_useTutorial)
	{
		m_tutorialManager->resizeInstructions(w, h);
		m_tutorialManager->resizeVisuals(w, h);
	}

	if (m_tlx->getRunning())
		m_tlx->resize(w, h);

	m_simEscPanel->setLocalPos(0.0, m_windowH / 2);
	double simEscPanelHeight = 50.0;
	m_simEscPanel->setSize(m_windowW, simEscPanelHeight);
	m_simEscMessage->setLocalPos((m_windowW - m_simEscMessage->getTextWidth()) / 2, (simEscPanelHeight - m_simEscMessage->getTextHeight()) / 2);

	m_mapScale->resize(w, h);
}

//------------------------------------------------------------------------------

void rtsApplication::escRoutine(bool& exiting)
{
	// exit application
	if (!m_userExit)
	{
		m_userExit = true;
		m_simEscPanel->setShowEnabled(true);
	}
	else if (m_tlx->getRunning())
	{
		exiting = true;
	}
	else if (m_tlx->finished() || m_useTutorial || !m_useNASATLX)
	{
		exiting = true;
	}
	else
	{
		m_userInputMessages.push_back("ESC");
		m_userInputTimes.push_back(m_simClock->getCurrentTimeSeconds());

		// hide detections
		m_showDetections = false;

		// tlx on team display
		m_tlx->start();
		m_tlx->resize(m_windowW, m_windowH);

		// definitions on sensorMessage display
		m_tlxDefs->start();
		m_tlxDefs->resize(m_windowW2, m_windowH2);
	}
}

bool rtsApplication::keySelect2(unsigned char key, int x, int y)
{
	bool exiting = false;
	// option ESC: exit
	if ((key == 27))
	{
		escRoutine(exiting);
	}

    // fullscreen key
    if (key == 'f')
    {
        if (m_fullscreen2)
        {
            windowPosX2 = glutGet(GLUT_INIT_WINDOW_X);
            windowPosY2 = glutGet(GLUT_INIT_WINDOW_Y);
            m_windowW2 = glutGet(GLUT_INIT_WINDOW_WIDTH);
            m_windowH2 = glutGet(GLUT_INIT_WINDOW_HEIGHT);
            glutPositionWindow(windowPosX2, windowPosY2);
            glutReshapeWindow(m_windowW2, m_windowH2);
            m_fullscreen2 = false;
        }
        else
        {
            glutFullScreen();
            m_fullscreen2 = true;
        }
    }
	else if (key == 'h')
	{
		m_hapticsEnabled = !m_hapticsEnabled;
		if (m_useTutorial)
		{
			m_tutorialManager->setHapticsEnabled(m_hapticsEnabled);
		}
		m_playdoh->setHapticEnabled(!m_playdoh->getHapticEnabled(), true);
		if (m_hapticsEnabled)
			cout << "haptics enabled\n";
		else cout << "haptics disabled\n";
	}
	else if (key == 'i')
	{
		m_robotTeam->toggleShowId();
	}
	else if (key == 'p')
	{
		if (m_useTutorial)
		{
			m_tutorialManager->togglePauseTutorial();
			m_simClock->togglePause();
		}
		else
		{
			m_pause = !m_pause;
			if (m_pause)
				m_com->pause();
			else
				m_com->unpause();
		}
	}
	else if (key == 'r')
	{
		m_robotTeam->toggleShowEnabled();
		m_robotTeam->setShowId(false);
	}
	else if (key == 's')
	{
		m_playdoh->setShowEnabled(!m_playdoh->getShowEnabled(), true);
	}
	else if (key == 't')
	{
		m_travelEnabled = !m_travelEnabled;
		if (m_travelEnabled)
			cout << "travel enabled" << endl;
		else cout << "travel disabled" << endl;
	}
	else if (key == 'v')
	{
		m_useServer = !m_useServer;
		if (m_useServer)
			std::cout << "useServer: true" << std::endl;
		else std::cout << "useServer: false" << std::endl;
	}
	else if (key == 'w')
	{
		if (m_useTutorial)
			m_tutorialManager->togglePauseTutorial();
	}
    else if (key == '\r')
    {
		// begin simulation only once
		if (m_simReady && !m_beginSim)
		{
			//m_com->unpause();
			m_beginSim = true;
			//m_useServer = true;
		}
		else if (m_userExit)
		{

		}
		else if (m_userInput == m_blankUserInput)
		{
			m_newBlankUserMessage = true;
			m_newUserMessage = true;
		}
		else if (m_userInput != m_blankUserInput)
			m_newUserMessage = true;
		else if (m_userInput == "\r")
		{
			m_userInput = m_blankUserInput;
		}
    }
    else if (key == '\b')
    {
        if (m_userInput != m_blankUserInput)
            m_userInput.pop_back();
    }
	else if (key != 27)
    {
        m_userInput += key;
    }

	if (key != 27)
	{
		m_userExit = false;
		m_simEscPanel->setShowEnabled(false);
	}
    return exiting;
}

bool rtsApplication::keySelect(unsigned char key, int x, int y)
{
    bool exiting = false;
    // option ESC: exit
    if ((key == 27) || (key == 'x'))
    {        
		escRoutine(exiting);
    }

	if (key == '\r')
	{
		// begin simulation only once
		if (m_simReady && !m_beginSim)
		{
			m_beginSim = true;
		}
	}
    
    // option 1:
    if (key == '1')
    {
		if (m_useTutorial)
		{
			m_tutorialManager->moveToNextOrPrevInstruction(false);
		}
    }
    
    // option 2:
    if (key == '2')
    {
		if (m_useTutorial)
		{
			m_tutorialManager->moveToNextOrPrevInstruction(true);
		}
    }

    // convex hull show
    if (key == 'c')
    {
        if (m_playdoh->getTravelMode())
            m_robotTeam->setShowConvexHull(!m_robotTeam->getShowConvexHull());
    }
	// test speed variable
	if (key == 'd')
	{
		m_testSpeed = true;
	}
    // option f: toggle fullscreen
    if (key == 'f')
    {
        if (m_fullscreen)
        {
            windowPosX = glutGet(GLUT_INIT_WINDOW_X);
            windowPosY = glutGet(GLUT_INIT_WINDOW_Y);
            m_windowW = glutGet(GLUT_INIT_WINDOW_WIDTH);
            m_windowH = glutGet(GLUT_INIT_WINDOW_HEIGHT);
            glutPositionWindow(windowPosX, windowPosY);
            glutReshapeWindow(m_windowW, m_windowH);
            m_fullscreen = false;
        }
        else
        {
            glutFullScreen();
            m_fullscreen = true;
        }
    }
	else if (key == 'h')
	{
		m_hapticsEnabled = !m_hapticsEnabled;
		if (m_useTutorial)
		{
			m_tutorialManager->setHapticsEnabled(m_hapticsEnabled);
		}
		m_playdoh->setHapticEnabled(!m_playdoh->getHapticEnabled(), true);
		if (m_hapticsEnabled)
			cout << "haptics enabled\n";
		else cout << "haptics disabled\n";
	}
	else if (key == 'i')
	{
		m_robotTeam->toggleShowId();
	}
	//if (key == 'o')
	//{
	//    m_showToolPos = !m_showToolPos;
	//    cVector3d toolPos = m_tool->getLocalPos();
	//    cVector3d globalPos = m_tool->getGlobalPos();
	//}
    else if (key == 'p')
    {
		if (m_useTutorial)
		{
			m_tutorialManager->togglePauseTutorial();
			m_simClock->togglePause();
		}
		else
		{
			m_pause = !m_pause;
			if (m_pause)
				m_com->pause();
			else
				m_com->unpause();
		}
    }
	else if (key == 'r')
	{
		m_robotTeam->toggleShowEnabled();
		m_robotTeam->setShowId(false);
	}
	else if (key == 's')
	{
		m_playdoh->setShowEnabled(!m_playdoh->getShowEnabled(), true);
	}
	else if (key == 't')
	{
		m_travelEnabled = !m_travelEnabled;
		if (m_travelEnabled)
			cout << "travel enabled" << endl;
		else cout << "travel disabled" << endl;
	}
	else if (key == 'v')
	{
		m_useServer = !m_useServer;
		if (m_useServer)
			std::cout << "useServer: true" << std::endl;
		else std::cout << "useServer: false" << std::endl;
	}
	else if (key == 'w')
	{
		if (m_useTutorial)
			m_tutorialManager->togglePauseTutorial();
	}

	if (key != 27)
	{
		m_userExit = false;
		m_simEscPanel->setShowEnabled(false);
	}
    return exiting;
}


void rtsApplication::mouseClick(int button, int state, int x, int y)
{
	if (m_tlx->getRunning())
	{
		// mouse button down
		if (state == GLUT_DOWN)
		{
			m_tlx->updateButtons(m_camera, m_windowW, m_windowH, x, y, true);
		}
		else if (state == GLUT_UP)
		{
			m_tlx->updateButtons(m_camera, m_windowW, m_windowH, x, y, false);
		}
	}
}

//---------------------------------------------------------------------------

void rtsApplication::mouseMove(int x, int y)
{
	if (m_tlx->getRunning())
	{
		m_tlx->updateSliders(x, y);
	}
}

//------------------------------------------------------------------------------

void rtsApplication::close(void)
{
    // finish writing data
    if (m_useDataThread && m_useDataCollection && !m_enableDataMutex)
    {
        m_dataMutex->lock();
        m_enableDataMutex = true;
        
        int m_tempDataStackSize = m_tempDataStack.size();
        for (int i = 0; i < m_tempDataStackSize; i++)
        {
            m_dataStack.push_back(m_tempDataStack[i]);
        }
        m_tempDataStack.clear();
        
        m_dataMutex->unlock();
    }

	if (m_useDataCollection)
	{
		// write hotspot found times to file with group id numbers
		m_hotspotManager->writeTargetFoundData(m_outputFolder + "hotspotData.txt");
	}

    // time intervals check
    if (m_checkTimeIntervals)
    {
		string outputFile = m_outputFolder + "timeIntervals.txt";
        ofstream fout;
        fout.open(outputFile.c_str());
        
        int numTimeIntervals = m_timeIntervals.size();
        for (int i = 0; i < numTimeIntervals; i++)
        {
            fout << m_timeIntervals[i] << "\n";
        }
        
        m_timeIntervals.clear();
        
        fout.close();
    }
    
    // time intervals check
    if (m_checkRegions)
    {
		string outputFile = m_outputFolder + "regionTimeIntervals.txt";
        ofstream fout;
        fout.open(outputFile.c_str());
        
        int numRegionTimeIntervals = m_regionTimeIntervals.size();
        for (int i = 0; i < numRegionTimeIntervals; i++)
        {
            fout << m_regionTimeIntervals[i] << "\n";
        }
        
        m_regionTimeIntervals.clear();
        
        fout.close();
    }

	if (m_useDataCollection)
	{
		// write user input to file
		string userMessageOutputFile = m_outputFolder + "userInput.txt";
		ofstream foutM;
		foutM.open(userMessageOutputFile.c_str());

		int numMessages = m_userInputMessages.size();
		for (int i = 0; i < numMessages; i++)
		{
			foutM << m_userInputTimes[i] << " " << m_userInputMessages[i] << "\n";
		}

		m_userInputMessages.clear();
		m_userInputTimes.clear();
		foutM.close();
	}

	// stop the simulation
	m_simulationRunning = false;
    
    // wait for graphics and haptics loops to terminate
    while (!m_simulationFinished) { cSleepMs(100); }

	// quit server
	m_com->quit();
    
    // close haptic device
    m_tool->stop();
}

void rtsApplication::deallocate()
{
	//-----------------------------------------
	// DEMO
	//-----------------------------------------

	if (m_useTutorial)
	{
		delete m_tutorialManager;
		m_tutorialManager = NULL;
	}

    //-----------------------------------------
    // SERVER
    //-----------------------------------------
    
    delete m_queryMutex;
    m_queryMutex = NULL;
    
    delete m_swarm;
    m_swarm = NULL;
    
    delete m_com;
    m_com = NULL;
    
    //-----------------------------------------
    // ENVIRONMENT
    //-----------------------------------------
    m_world->removeChild(m_ground);
    delete m_ground;
    m_ground = NULL;
    
    // buildings
    vector<rtsPolygon*>::iterator polyIt;
    for (polyIt = m_buildings.begin(); polyIt < m_buildings.end(); polyIt++)
    {
        rtsPolygon* nextPoly = (*polyIt);
        m_world->removeChild(nextPoly);
        delete (nextPoly);
        nextPoly = NULL;
    }
    m_buildings.clear();
    
    //-----------------------------------------
    // ROBOT TEAM
    //-----------------------------------------
    
    // mutex
    delete m_mtx;
    m_mtx = NULL;
    
    // robot team
    delete m_robotTeam;
    m_robotTeam = NULL;
    
    // playdoh robot team
    delete m_playdohRobotTeam;
    m_playdohRobotTeam = NULL;
    
    // controller
    delete m_controller;
    m_controller = NULL;
    
    //-----------------------------------------
    // HAPTICS
    //-----------------------------------------
    
    delete m_playdoh;
    m_playdoh = NULL;
    
    //-----------------------------------------
    // TOOL
    //-----------------------------------------
    
    // tool
    m_cameraProjSphere->removeChild(m_tool);
    delete m_tool;
    m_tool = NULL;
    
    delete m_handler;
    m_handler = NULL;
    
    //delete m_labelMessage;
    //m_labelMessage = NULL;
    
    delete m_labelHapticRate;
    m_labelHapticRate = NULL;
    
    //-----------------------------------------
    // DATA
    //-----------------------------------------
    
    delete m_dataManager;
    m_dataManager = NULL;
    
    delete m_dataMutex;
    m_dataMutex = NULL;
    
	//-----------------------------------------
	// NASA TLX
	//-----------------------------------------
	m_camera->m_frontLayer->removeChild(m_tlx);
	delete m_tlx;
	m_tlx = NULL;

	m_camera2->m_frontLayer->removeChild(m_tlxDefs);
	delete m_tlxDefs;
	m_tlxDefs = NULL;

    //-----------------------------------------
    // TARGET
    //-----------------------------------------
    
    delete m_hotspotManager;
    m_hotspotManager = NULL;
    
    //-----------------------------------------
    // 2 FEED
    //-----------------------------------------
    

    delete m_feed;
    m_feed = NULL;

	// sim clock
	m_camera2->m_frontLayer->removeChild(m_simClock);
	delete m_simClock;
	m_simClock = NULL;
    
    // scopes and labels
    vector<rtsScope*>::iterator scopeIt;
	vector<cLabel*>::iterator scopeLabelIt;
	scopeLabelIt = m_scopeLabels.begin();
    for (scopeIt = m_scopes.begin(); scopeIt < m_scopes.end(); scopeIt++)
    {
		rtsScope* nextScope = (*scopeIt);

		// remove and delete label from this scope
		cLabel* nextLabel = *scopeLabelIt;
		nextScope->removeChild(nextLabel);
		delete nextLabel;
		nextLabel = NULL;
  
        m_world2->removeChild(nextScope);
        delete (nextScope);
        nextScope = NULL;

		// increment scope label iterator
		scopeLabelIt++;
    }
    m_scopes.clear();
    
    // user input label
    m_userInputPanel->removeChild(m_userInputLabel);
    delete m_userInputLabel;
    m_userInputLabel = NULL;
    
    // user input panel
    m_camera2->m_frontLayer->removeChild(m_userInputPanel);
    delete m_userInputPanel;
    m_userInputPanel = NULL;
    
    // sensorMessage messages labels
    deque<cLabel*>::iterator sensorMessageLabelsIt;
    for (sensorMessageLabelsIt = m_messages.begin(); sensorMessageLabelsIt < m_messages.end(); sensorMessageLabelsIt++)
    {
        cLabel* nextLabel = (*sensorMessageLabelsIt);
        m_sensorMessagePanel->removeChild(nextLabel);
        delete (nextLabel);
        nextLabel = NULL;
    }
    m_buildings.clear();
    
    // sensorMessage panel
    m_camera2->m_frontLayer->removeChild(m_sensorMessagePanel);
    delete m_sensorMessagePanel;
    m_sensorMessagePanel = NULL;
    
    // message font
    delete m_messageFont;
    m_messageFont = NULL;
    
    // sensorMessage light
    m_world2->removeChild(m_light2);
    delete m_light2;
    m_light2 = NULL;
    
    // sensorMessage camera
    m_world2->removeChild(m_camera2);
    delete m_camera2;
    m_camera2 = NULL;
    
    // sensorMessage world
    delete m_world2;
    m_world2 = NULL;

    //-----------------------------------------
    // WORLD
    //-----------------------------------------
    
    // projection sphere for tool
    m_camera->removeChild(m_cameraProjSphere);
    delete m_cameraProjSphere;
    m_cameraProjSphere = NULL;
    
    // light
    m_world->removeChild(m_light);
    delete m_light;
    m_light = NULL;
    
    // camera
    m_world->removeChild(m_camera);
    delete m_camera;
    m_camera = NULL;
    
    // world
    delete m_world;
    m_world = NULL;
}

//------------------------------------------------------------------------------

void rtsApplication::graphicsTimer2(int data)
{

}

void rtsApplication::graphicsTimer(int data)
{

}

//------------------------------------------------------------------------------

void rtsApplication::updateGraphics2(void)
{
    
    /////////////////////////////////////////////////////////////////////
    // UPDATE WIDGETS
    /////////////////////////////////////////////////////////////////////

	if (m_beginSim)
	{
		// update simulation clock
		m_simClock->updateClock();

		bool sampleSignal = false;
		if (!m_nsewClock.on())
		{
			m_nsewClock.reset();
			m_nsewClock.start();
			m_nsewClock.setTimeoutPeriodSeconds(0.50);
		}

		if (m_nsewClock.timeoutOccurred())
		{
			sampleSignal = true;
			m_nsewClock.stop();
		}

		bool useCMAVals = true;

		if (useCMAVals)
		{
			// compute cumulative moving average of data
			if (m_nsewValuesReady)
			{
				int numSignals = m_nsewValues.size();
				for (int i = 0; i < numSignals; i++)
				{
					m_nsewCMAValues[i] = (m_nsewValues[i] + m_nCMA * m_nsewCMAValues[i]) / double(m_nCMA + 1);
				}
				m_nCMA++;
			}

			if (sampleSignal)
				m_nCMA = 0;
		}

		// build test sensorMessage data for manual setting
		rtsSignalData nextData;

		//if (sampleSignal)
		//{
		nextData.data.clear();
		for (int i = 0; i < 4; i++)
		{
			rtsSignalDataPair pair;
			pair.time = 0;
			if (useCMAVals)
			{
				pair.signalVal = m_nsewCMAValues[i];
			}
			else
			{
				if (m_nsewValuesReady)
					pair.signalVal = m_nsewValues[i];
				else pair.signalVal = m_pastnsewValues[i];
			}

			nextData.data.push_back(pair);
		}
	//}
		if (m_useDetections && m_showDetections)
			m_detections->update(sampleSignal, m_sensorReadings, m_sensorPositions);
		
		if (m_nsewValuesReady)
		{
			m_pastnsewValues = m_nsewValues;
			m_nsewValuesReady = false;
		}

		vector<double> nsewVals;
		vector<cColorf> nsewColors;
		int numScopes = m_scopes.size();
		for (int i = 0; i < numScopes; i++)
		{
			//double time = nextData.data[i].time;
			double val = nextData.data[i].signalVal;
			nsewVals.push_back(val);
			m_scopes[i]->setSampledSignalValues(sampleSignal, val);

			cColorf color(1.0, 1.0 - val, 1.0 - val);
			nsewColors.push_back(color);
			//m_scopes[i]->updateColor(sampleSignal, val, color);

			//double maxT = 5.0;
			//double minT = 1.0;
			//if (val > 0.00001)
			//{
			//	//color = m_scopes[i]->getBaselineFadeColor();
			//	m_scopes[i]->fadeToColor(sampleSignal, color, maxT - (maxT - minT)*val);
			//}

		}

		if (!m_useDetections)
		{
			//cColorf colorRing(1, 0, 0);
			//m_signalRing->setColors(colorRing, colorRing, colorRing, colorRing);
			m_signalRing->updateColorsFromNSEW(sampleSignal, nsewVals, nsewColors);
		}


		// retrieve sensorMessage message data from feed according to simulation time
		double vSpacing = 10;
		double hSpacing = 10;
		double scopeW = 0.5*m_windowW2;
		double scopeH = (m_windowH2 - vSpacing*(m_scopes.size() + 1)) / m_scopes.size();
		scopeH = cClamp(scopeH, m_minScopeH, m_maxScopeH);

		double userInputPanelPosX = hSpacing + m_scopeLabels[0]->getWidth() + scopeW + 2 * m_scopes[0]->getScopeMargin();
		double userInputPanelPosY = vSpacing;
		double userInputPanelH = 35;
		double panelH = m_windowH2 - 2.5 * vSpacing - userInputPanelH;
		double panelW = m_windowW2 - (scopeW + 2 * hSpacing + m_scopeLabels[0]->getWidth() + 2 * m_scopes[0]->getScopeMargin());


		rtsMessagePair* nextMessage;
		if (m_useTutorial)
		{
			nextMessage = m_feed->getNextMessagePair(m_tutorialManager->getCurrentTimeSeconds());
		}
		else
		{
			nextMessage = m_feed->getNextMessagePair(m_simClock->getCurrentTimeSeconds());
		}

		bool newMessage = m_feed->nextMessageReached();

		// define maximum feed messages to display
		int maxFeedMessages;
		if (!m_messages.empty())
		{
			maxFeedMessages = (int)(panelH / m_messages[0]->getHeight());
		}
		else maxFeedMessages = 10;
		if (newMessage || m_newUserMessage)
		{
			cLabel* messageLabel = new cLabel(m_messageFont);
			string displayMessage;
			if (newMessage && m_newUserMessage)
			{
				// in rare occurance that both occur in same iteration
				cLabel* userMessageLabel = new cLabel(m_messageFont);
				string userDisplayMessage;

				// erase blank message prior to adding to display text
				m_userInput.erase(0, m_blankUserInput.size());

				if (m_newBlankUserMessage)
				{
					// create string for recording position
					string posStr = "pos recorded - x: " + to_string(m_devicePos.x()) +
						" y: " + to_string(m_devicePos.y());
					
					userDisplayMessage = "User: " + posStr;
					m_newBlankUserMessage = false;
				}
				else userDisplayMessage = "User: " + m_userInput;
				m_userInput = m_blankUserInput;
				m_newUserMessage = false;
				userMessageLabel->m_fontColor.setGreen();
				userMessageLabel->setText(userDisplayMessage);
				//m_sensorMessagePanel->addChild(userMessageLabel);
				//m_messages.push_back(userMessageLabel);

				// record user input
				m_userInputMessages.push_back(displayMessage);
				m_userInputTimes.push_back(m_simClock->getCurrentTimeSeconds());

				displayMessage = nextMessage->message; // "2:" +
				messageLabel->setText(displayMessage);
			}
			else if (m_newUserMessage)
			{
				// erase blank message prior to adding to display text
				m_userInput.erase(0, m_blankUserInput.size());

				if (m_newBlankUserMessage)
				{
					string posStr = "pos recorded - x: " + to_string(m_devicePos.x()) +
						" y: " + to_string(m_devicePos.y());

					displayMessage = "User: " + posStr;
					m_newBlankUserMessage = false;
				}
				else displayMessage = "User: " + m_userInput;
				m_userInput = m_blankUserInput;
				m_newUserMessage = false;
				messageLabel->m_fontColor.setGreen();
				messageLabel->setText(displayMessage);

				// record user input
				m_userInputMessages.push_back(displayMessage);
				m_userInputTimes.push_back(m_simClock->getCurrentTimeSeconds());
			}
			else
			{
				displayMessage = nextMessage->message; // "2:" +
				messageLabel->setText(displayMessage);
			}

			bool isOverflow = false;
			if (!m_tlx->getRunning())
			{
				// if travel command, create separating blank label
				if (messageLabel->getText().find("Move") != string::npos)
				{
					cLabel* spacer = new cLabel(m_messageFont);
					spacer->setText("");
					m_messages.push_back(spacer);

					// set move command to red
					messageLabel->m_fontColor.setRed();

					// enabled travel command
					m_travelEnabled = true;
					m_travelEnabledClock.reset();
					m_travelEnabledClock.start();
				}


				m_sensorMessagePanel->addChild(messageLabel);
				if (messageLabel->getWidth() <= m_sensorMessagePanel->getWidth())
					m_messages.push_back(messageLabel);
				else
				{
					// message is too long for the panel, so create another label for another line
					isOverflow = true;
					cLabel* overflowMessageLabel = new cLabel(m_messageFont);

					// find longest string for the first line
					unsigned int maxStrSize = 65; //hcval
					size_t p;
					string m1 = displayMessage;
					string m2 = "";
					while (m1.size() > maxStrSize)
					{
						p = m1.find_last_of(" ");
						m2 = m1.substr(p) + m2;
						m1 = m1.substr(0,p);
					}
					m2 = "      " + m2;
					messageLabel->setText(m1);
					m_messages.push_back(messageLabel);
					
					overflowMessageLabel->setText(m2);
					m_sensorMessagePanel->addChild(overflowMessageLabel);
					m_messages.push_back(overflowMessageLabel);
				}
			}

			// check for timeout of travel enabled clock
			if (m_travelEnabledClock.timeoutOccurred())
			{
				m_travelEnabled = false;
				m_travelEnabledClock.stop();
				m_travelEnabledClock.reset();
			}

			// if number of current messages too many, delete latest
			if ((int)(m_messages.size()) > maxFeedMessages)
			{
				int k = 1;
				if (isOverflow)
					k = 2;

				for (int r = 0; r < k; r++)
				{
					cLabel* labelToDelete = m_messages[0];
					m_messages.pop_front();

					// delete the label
					m_sensorMessagePanel->removeChild(labelToDelete);
					delete labelToDelete;
					labelToDelete = NULL;
				}
			}

			// set position of all messages in feed
			int num2Messages = m_messages.size();
			for (int i = 0; i < num2Messages; i++)
			{
				m_messages[i]->setLocalPos(hSpacing, panelH - (i + 1) * m_messages[i]->getHeight());
			}
		}

		// flash the command panel
		if (newMessage)
		{
			m_messageAlertClock.setTimeoutPeriodSeconds(2.0);
			m_messageAlertClock.reset();
			m_messageAlertClock.start();
			cColorf red(0.2f, 0.0f, 0.0f);
			cColorf black(0.3f, 0.3f, 0.3f);
			//m_sensorMessagePanel->setCornerColors(red, red, black, black);
			m_sensorMessagePanel->setCornerColors(red, red, red, red);
		}

		if (m_messageAlertClock.timeoutOccurred())
		{
			cColorf black(0, 0, 0);
			m_sensorMessagePanel->setCornerColors(black, black, black, black);
			m_messageAlertClock.stop();
		}
		else if (m_messageAlertClock.on())
		{
			// fade colors
		}


		// Display user keyboard input
		m_userInputLabel->setLocalPos(hSpacing, vSpacing, 0);
		m_userInputLabel->setText(m_userInput);

		if (m_useTutorial)
		{
			m_tutorialManager->updateInstructions2();
			m_tutorialManager->updateVisuals2();
		}

	}

    /////////////////////////////////////////////////////////////////////
    // RENDER SCENE
    /////////////////////////////////////////////////////////////////////
    
    // update shadow maps (if any)
	bool mirroredDisplay = false;
    m_world2->updateShadowMaps(false, mirroredDisplay);
    
    // render world
    m_camera2->renderView(m_windowW2, m_windowH2);
    
    // swap buffers
    glutSwapBuffers();
    
    // wait until all GL commands are completed
    glFinish();
    
    // check for any OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) cout << "Error: " << gluErrorString(err) << endl;
    
}

void rtsApplication::updateGraphics(void)
{
    /////////////////////////////////////////////////////////////////////
    // UPDATE WIDGETS
    /////////////////////////////////////////////////////////////////////
	if (m_beginSim)
	{
		// update haptic rate data
		m_labelHapticRate->setText(cStr(m_frequencyCounter.getFrequency(), 0) + " Hz");

		// update position of haptic rate label
		m_labelHapticRate->setLocalPos((int)(0.5 * (m_windowW - m_labelHapticRate->getWidth())), 15);

		// update position of message label
		//m_labelMessage->setLocalPos((int)(0.5 * (m_windowW - m_labelMessage->getWidth())), 50);

		// update tutorial instructions
		if (m_useTutorial)
		{
			m_tutorialManager->updateInstructionsHaptics();
			m_tutorialManager->updateVisualsHaptics();
		}

		if (m_tlx->getRunning())
		{

		}

	}
 //   
	//bool sampleSignal = false;
	//if (!m_nsewClock.on())
	//{
	//	m_nsewClock.reset();
	//	m_nsewClock.start();
	//	m_nsewClock.setTimeoutPeriodSeconds(0.50);
	//}

	//if (m_nsewClock.timeoutOccurred())
	//{
	//	sampleSignal = true;
	//	m_nsewClock.stop();
	//}

	//bool sampleSignal2 = true;

	//if (m_useDetections && m_showDetections && m_showDetectionsWithTeam)
	//	m_detections->update(sampleSignal2, m_sensorReadings, m_sensorPositions);

    /////////////////////////////////////////////////////////////////////
    // RENDER SCENE
    /////////////////////////////////////////////////////////////////////
    
    // update shadow maps (if any)
	bool mirroredDisplay = false;
    m_world->updateShadowMaps(false, mirroredDisplay);
    
    // render world
    m_camera->renderView(m_windowW, m_windowH);
    
    // swap buffers
    glutSwapBuffers();
    
    // wait until all GL commands are completed
    glFinish();
    
    // check for any OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) cout << "Error: " << gluErrorString(err) << endl;
}

//------------------------------------------------------------------------------

void rtsApplication::updateHaptics(void)
{
    m_frequencyCounter.reset();
	m_simulationRunning = true;
	m_simulationFinished = false;

	//---------------------------------------------------------------
	// START SIM ROUTINE
	//---------------------------------------------------------------

	if (!m_simReady)
		startSimRoutine();

    // reset clock
    cPrecisionClock clock;
    clock.reset();

	// set radius clock timeout
	m_radiusClock.setTimeoutPeriodSeconds(m_radiusTimeout);
	m_newRadiusCommandClock.setTimeoutPeriodSeconds(m_radiusTimeout);
    
    // resize button variables
    bool resizeButton = false;
    bool prevResizeButton = false;
    cVector3d resizeToolStartPos;
    resizeToolStartPos.zero();
    
    // command travel button variables
    bool commandTravelButton = false;
    bool prevCommandTravelButton = false;
	bool endTravelForce = false;
	double endTravelTimeout = 1.0;
	cPrecisionClock endTravelClock;
	endTravelClock.setTimeoutPeriodSeconds(endTravelTimeout);
	endTravelClock.reset();
    cVector3d travelToolStartPos;
    travelToolStartPos.zero();
    
    // manipulation button variables
    bool manipButton = false;
    bool prevManipButton = false;
	bool playdohManipContact = false;
    bool manipSpacingButton = false;
	bool prevManipSpacingButton = false;
    cVector3d manipToolStartPos;
    manipToolStartPos.zero();

	// double click manip button variables
	cPrecisionClock manipDoubleClickClock;
	manipDoubleClickClock.reset();
	manipDoubleClickClock.setTimeoutPeriodSeconds(0.5); // 500 ms
	bool manipButtonDoubleClick1 = false;
	bool manipButtonDoubleClick2 = false;
	bool manipButtonDoubleClick3 = false;
	bool manipButtonDoubleClick4 = true;
	bool manipButtonDoubleClick = false;

	// manip spacing variables
	cVector3d manipSpacingCenter;
	double maxSpacingRadius = 0;
	double minSpacingRadius = 0;

	bool startSpacingForce = false;
	double startSpacingTimeout = 1.0;
	cPrecisionClock startSpacingClock;
	startSpacingClock.setTimeoutPeriodSeconds(startSpacingTimeout);
	startSpacingClock.reset();

	cPrecisionClock spacingDurClock;
	spacingDurClock.setTimeoutPeriodSeconds(0.25);
	spacingDurClock.reset();

	bool endSpacingForce = false;
	double endSpacingTimeout = 1.0;
	cPrecisionClock endSpacingClock;
	endSpacingClock.setTimeoutPeriodSeconds(endSpacingTimeout);
	endSpacingClock.reset();

	bool startResizeForce = false;
	double startResizeTimeout = 1.0;
	cPrecisionClock startResizeClock;
	startResizeClock.setTimeoutPeriodSeconds(startResizeTimeout);
	startResizeClock.reset();

	bool endResizeForce = false;
	double endResizeTimeout = 1.0;
	cPrecisionClock endResizeClock;
	endResizeClock.setTimeoutPeriodSeconds(endResizeTimeout);
	endResizeClock.reset();
    
    // manipulation variables
    cVector3d prevToolPos(0, 0, 0);
    
    //int m_swarmModeInt = 0;
    int prevSwarmModeInt = 0;
    //bool increaseRadius = false;
    
    bool useButtons = true;
    
    // main haptic simulation loop
    while (m_simulationRunning)
    {

        /////////////////////////////////////////////////////////////////////
        // SIMULATION TIME
        /////////////////////////////////////////////////////////////////////
            
        // stop the simulation clock
        clock.stop();
            
        // read the time increment in seconds
        double timeInterval = clock.getCurrentTimeSeconds();
            
        // store measured time interval between iterations
        if (m_checkTimeIntervals)
        {
            m_timeIntervals.push_back(timeInterval);
        }
        
        // set max time interval for playdoh dynamics
        if (timeInterval > 0.001) { timeInterval = 0.001; }
            
        // restart the simulation clock
        clock.reset();
        clock.start();
            
        /////////////////////////////////////////////////////////////////////
        // HAPTIC FORCE COMPUTATION
        /////////////////////////////////////////////////////////////////////
            
        // compute global reference frames for each object
        m_world->computeGlobalPositions(true);
            
        // update position and orientation of tool
        m_tool->updateFromDevice();
            
        // compute interaction forces
        m_tool->computeInteractionForces();
            
        // get interaction forces magnitude
        double toolForce = m_tool->getDeviceGlobalForce().length();
                        
        // read position from haptic device
        cVector3d pos = m_tool->getDeviceGlobalPos();
        
		if (m_numHapticDevices == 0)
		{
			pos = cVector3d(1000, 1000, 1000);
		}

        if (m_showToolPos)
        {
			cout << m_tool->getDeviceLocalPos() << endl;
        }
        
        // set device pos vector
        m_devicePos = pos;
            
		// change color if in contact with ground surface
		if ((pos.z() - m_deviceRadius) < 0)
		{
			m_tool->m_hapticPoint->m_sphereProxy->m_material->setColor(cColorf(0.6f,0.6f,0.6f));
		}
		else
		{
			m_tool->m_hapticPoint->m_sphereProxy->m_material->setColor(cColorf(1,1,1));
		}

        //---------------------------------------------------
        // User buttons
        //---------------------------------------------------
            
			
		// BUTTON PRIORITY 1 - PATROL
		// if user switch 1 is pressed, enable command travel button
		if (useButtons && m_tool->getUserSwitch(1) /*&& m_travelEnabled*/) // uncomment travel enabled to prevent accidental travel presses
		{
			//cout << "switch 2" << endl;
			commandTravelButton = true;
		}
		else
		{
			commandTravelButton = false;
				
			// BUTTON PRIORITY 2 - RESIZE RADIUS
			// if user switch 2 is pressed, resize the AIRadius
			bool useResize = false; // set true to turn on this interaction method
			if (useButtons && m_tool->getUserSwitch(2) && useResize)
			{
				//cout << "switch 1" << endl;
				if (m_distributionType == 1)
				{
					resizeButton = true;

					// reset manip completed to allow resize
					m_manipCompleted = false;
				}
			}
			else
			{
				resizeButton = false;

				// BUTTON PRIORITY 3 - MANIP SPACING
				// if user switch 3 is pressed, user can mainpulate agent spacing within the surround
				if (useButtons && m_tool->getUserSwitch(3))
				{
					//cout << "switch 3" << endl;
					if (m_distributionType == 1)
					{

						manipSpacingButton = true;
					}
				}
				else
				{
					manipSpacingButton = false;

					// BUTTON PRIORITY 4 - MANIP FORMATION
					// if user switch 0 is pressed, user can manipulate the surround
					if (useButtons && m_tool->getUserSwitch(0))
					{
						//cout << "switch 0" << endl;
						if (m_distributionType == 1)
						{
							manipButton = true;

							// set double click clock
							if (!manipDoubleClickClock.on() && !manipButtonDoubleClick1 && manipButtonDoubleClick4)
							{
								//cout << "down" << endl;
								manipDoubleClickClock.reset();
								manipDoubleClickClock.start();
								manipButtonDoubleClick1 = true;
								manipButtonDoubleClick2 = false;
								//manipButtonDoubleClick3 = false;
							}

							if (manipButtonDoubleClick2 && !manipButtonDoubleClick3)
							{
								//cout << "down regardless of timer status" << endl;
								if (!manipDoubleClickClock.timeoutOccurred())
								{
									manipButtonDoubleClick3 = true;
									manipButtonDoubleClick4 = false;
									//cout << "manip double click!" << endl;
									manipButtonDoubleClick = true;
								}
								else
								{
									//cout << "timeout occurred" << endl;
									manipDoubleClickClock.stop();
								}
							}
						}
					}
					else manipButton = false;
				} // end spacing button priority
			} // end resize button priority
		} // end travel button priority

		/// Resize routine
		//----------------------------------------------------------------

		// resize button down event
		if (resizeButton && !prevResizeButton && false)
        {
			startResizeForce = true;
			startResizeClock.reset();
			startResizeClock.start();

            m_playdoh->setEnableResizeSurround(true);
                
            // set tool start position
            resizeToolStartPos = pos;
                
            // reset percentage
            m_resizePercentage = 1.00;
        }
		// resize button release event
		else if (!resizeButton && prevResizeButton && false)
        {
			endResizeForce = true;
			endResizeClock.reset();
			endResizeClock.start();

            m_playdoh->setEnableResizeSurround(false);
                
			// flag for resize completed
			m_resizeSurroundCompleted = true;

			// store resized desired positions
			m_resizeSurroundCompletedPositions = m_playdohRobotTeam->getRobotPositions();
			//m_resizeSurroundCompletedPositions = m_playdohRobotPositions;

            // resize the minimum distance between agents
			if (!m_useServerThread)
			{
				m_com->radius(m_radiusPriority, 0, m_AIRadius);
				m_radiusPriority++;
			}

			// quick fix for resize adjustment due to playdoh dynamics when button clicked
			m_totalResizePercentage *= 0.95;
        }
            
        /// Command travel routine
        //----------------------------------------------------------------
            
		// commandTravel button down event
		if (commandTravelButton && !prevCommandTravelButton)
        {
            // set tool start position
            travelToolStartPos = pos;
        }
		// commandTravel button release event
		else if (!commandTravelButton && prevCommandTravelButton)
        {
			endTravelForce = true;
			endTravelClock.reset();
			endTravelClock.start();
                
            // the find the agents (2) nearest the direction of interest
            int travel_id = 0;
            int travel_id2 = 1;
                
			findNearestTwoAgents(travelToolStartPos, travel_id, travel_id2);
                    
            // define travel direction
			bool relativeToTeam = true;

            cVector3d travelDir1;
            cVector3d travelDir2;
			if (relativeToTeam)
			{
				travelDir1 = pos - m_robotTeam->getRobot(travel_id)->getLocalPos();
				travelDir1.z(0);
				travelDir2 = pos - m_robotTeam->getRobot(travel_id2)->getLocalPos();
				travelDir2.z(0);
			}
			else
			{
				// travel direction relative to start travel tool start position
				travelDir1 = pos - travelToolStartPos;
				travelDir1.z(0);
				travelDir2 = pos - travelToolStartPos;
				travelDir2.z(0);
			}
                
			// store server variables for command, or command here if no server thread
            if (m_useServerThread)
            {
                m_commandedTravelDir1 = travelDir1;
                m_commandedTravelDir2 = travelDir2;
                m_travelId1 = travel_id;
                m_travelId2 = travel_id2;
            }
            else
            {
                m_com->patrol(m_travelCount, travel_id, travelDir1.x(), travelDir1.y(), travelDir1.z());
                m_travelCount++;
                        
                m_com->patrol(m_travelCount, travel_id2, travelDir2.x(), travelDir2.y(), travelDir2.z());
                m_travelCount++;
            }
                
			// don't try to maintain manip positions anymore
            m_manipCompleted = false;

			// don't try to maintain resize positions anymore
			m_resizeSurroundCompleted = false;

			// reset manip started
			m_manipStarted = false;
			m_manipStartedPositions.clear();

			// change color of detections
			if (m_useDetections)
			{
				m_detections->setColor(cColorf(1.0, 0, 0));
			}
			int numScopes = m_scopes.size();
			for (int i = 0; i < numScopes; i++)
			{
				m_scopes[i]->m_colorSignal0 = cColorf(0.8f, 0.0f, 0.0f);
			}
				
			// hide detections and hotspots
			if (m_useDetections)
			{
				m_detections->setShowEnabled(false);
				m_detections->zeroPositions();
			}

			m_robotTeam->setSensorLightTransparencyLevels(0.0);
			m_robotTeam->setSensorColors(cColorf(1.0, 0.0, 0.0));

			// decrease AIRadius
            m_AIRadius = m_travelRadius * (m_toolWorkspaceScaleFactor / 3125); // hcval
                
			// if not server thread, command radius change here
            if (!m_useServerThread)
            {
                m_com->radius(m_radiusPriority, 0, m_AIRadius);
                m_radiusPriority++;
            }

			m_robotTeam->setShowSurroundLines(false);
                
			// flag travel commanded
            m_travelCommanded = true;

			// reset resize percentage for next surround formation
			m_totalResizePercentage = 1.0;

			// closest building needs to be set upon travel destination arrival
			m_closestBldgIndexSet = false;

			// hide detection lights
			if (m_useDetections)
			{
				m_detections->setShowLights(false);
			}

			// hide hotspots until arrival at building
			if (!m_showHotspotsWithTeam)
				m_hotspotParent2->setShowEnabled(false);
                

            // if adjusting radius from previous travel movement, reset
            m_increaseRadius = false;
			m_startIncreaseRadius = false;
			m_radiusClock.stop();
			m_radiusClock.reset();
                
                
            // set distribution type to travel mode
            m_distributionType = 0;
        }
            
        //----------------------------------------------------------------
        /// End command travel routine
            
		// manip button down event
		if (manipButton && !prevManipButton)
        {
            // set tool start position
            manipToolStartPos = pos;

			playdohManipContact = false;
                
            // store robot positions before manipulation (in id order)
            m_prevManipPositions = m_robotTeam->getRobotPositions();

			// store very first manipulation of team (after last travel command)
			if (!m_manipStarted)
			{
				// store robot positions (in id order)
				m_manipStartedPositions = m_robotTeam->getRobotPositions();
				m_manipCompletedPositions = m_manipStartedPositions;
				m_manipCompleted = true;
				m_manipStarted = true;
			}
                
        }
		// manip button release event
		else if (!manipButton && prevManipButton)
        {   
            // flag for manip completed
            m_manipCompleted = true;
                
            // store manipulated desired positions, if manipulated
			if (!m_manipStarted)
			{
				m_manipStarted = true;
				m_manipCompletedPositions = m_manipStartedPositions;
			}
				
			if (playdohManipContact)
			{
				m_manipCompletedPositions = m_playdohRobotTeam->getRobotPositions();
				//m_manipCompletedPositions = m_robotTeam->getRobotPositions();
			}

			// double click logic
			if (manipButtonDoubleClick1 && !manipButtonDoubleClick2)
			{
				//cout << "up" << endl;
				manipButtonDoubleClick1 = false;
				manipButtonDoubleClick2 = true;
			}
			else if (manipButtonDoubleClick3 && !manipButtonDoubleClick4)
			{
				//cout << "up reset" << endl;
				manipButtonDoubleClick4 = true;
				manipButtonDoubleClick3 = false;
			}
        }

		// manipSpacing button down event
		if (manipSpacingButton && !prevManipSpacingButton)
		{
			startSpacingForce = true;
			startSpacingClock.reset();
			startSpacingClock.start();

			// record length of duration
			spacingDurClock.start();

			if (!m_manipCompleted)
			{
				m_manipCompleted = true;
				m_manipCompletedPositions = m_playdohRobotTeam->getRobotPositions();
			}
				
			// store very first manipulation of team (after last travel command)
			if (!m_manipStarted)
			{
				// store robot positions (in id order)
				m_manipStartedPositions = m_robotTeam->getRobotPositions();
				m_manipStarted = true;
			}

			// get center of team
			manipSpacingCenter = m_playdoh->getCenterOfMass();
			int nearestToCenter;
			int farthestToCenter;
			getNearestAndFarthestSurroundIndices(manipSpacingCenter, nearestToCenter, farthestToCenter);
			//minSpacingRadius = cDistance(manipSpacingCenter, m_robotPositions[nearestToCenter]);
			maxSpacingRadius = cDistance(manipSpacingCenter, m_robotPositions[farthestToCenter]);
			minSpacingRadius = 0.25*maxSpacingRadius;

			// adjust minSpacingRadius
			//minSpacingRadius *= 0.10;

			// adjust maxSpacing
			maxSpacingRadius *= 0.75;
		}
		// manipSpacing button release event
		else if (!manipSpacingButton && prevManipSpacingButton)
		{
			endSpacingForce = true;
			endSpacingClock.reset();
			endSpacingClock.start();

			if (spacingDurClock.timeoutOccurred())
			{
				// flag for manip completed
				m_manipCompleted = true;

				// store manipulated desired positions
				m_manipCompletedPositions = m_playdohRobotTeam->getRobotPositions();
			}
			spacingDurClock.stop();
			spacingDurClock.reset();
		}

		// update tutorial manager fo user responses
		if (m_useTutorial)
		{
			cVector3d toolVelocity = getToolVelocity();
			bool move = false;
			if (toolVelocity.length() > 0.3)
			{
				move = true;
			}
			m_tutorialManager->retrieveUserInput(move, (manipButton && !prevManipButton), (!manipButton && prevManipButton), manipButtonDoubleClick,
												(manipSpacingButton && !prevManipSpacingButton), (!manipSpacingButton && prevManipSpacingButton),
												(commandTravelButton && !prevCommandTravelButton), (!commandTravelButton && prevCommandTravelButton),
												(resizeButton && !prevResizeButton), (!resizeButton && prevResizeButton));
		}

        //---------------------------------------------------
        // Retrieve information from robot distribution methods
        //---------------------------------------------------

		retrieveRobotData(manipButton, manipSpacingButton, resizeButton);
            
		// if playdoh contacted, set manip playdoh contacted
		if (manipButton && !playdohManipContact && m_playdoh->getContactState())
		{
			playdohManipContact = true;
		}

        //---------------------------------------------------
        // Compute haptic feedback for given interaction method
        //---------------------------------------------------

		if (m_useTutorial)
		{
			m_hapticsEnabled = m_tutorialManager->getHapticsEnabled();
		}

		computeHapticFeedback(pos, timeInterval, commandTravelButton, endTravelForce, endTravelClock, travelToolStartPos, endTravelTimeout,
								resizeButton, resizeToolStartPos, startResizeForce, startResizeClock,
								endResizeForce, endResizeClock,
								manipButton, manipSpacingButton,
								startSpacingForce, startSpacingClock, endSpacingForce, endSpacingClock, 
								manipSpacingCenter, endSpacingTimeout, startSpacingTimeout, minSpacingRadius, maxSpacingRadius);

		// record force sent to device
		cVector3d force;
		force = m_tool->getDeviceGlobalForce();

		if (!m_hapticsEnabled)
		{
			m_tool->setDeviceGlobalForce(0, 0, 0);
		}

		// send forces to haptic device
		m_tool->applyToDevice();

		//-----------------------------------------------------------
		// TEAM MIN RADIUS ADJUSTMENT
		//-----------------------------------------------------------
            
		// check swarm mode for radius adjustment
		if (prevSwarmModeInt == 0 && m_swarmModeInt == 1)
		{
			m_startIncreaseRadius = true;
			m_radiusClock.reset();
			m_radiusClock.start();

			// set closest building index for sensing
			if (!m_closestBldgIndexSet)
			{
				setClosestBldgIndex();
				m_closestBldgIndexSet = true;
				//cout << "closest building index found" << endl;
			}

			if (m_useDetections)
			{
				if (!m_showHotspotsWithTeam)
				{
					m_hotspotParent2->setLocalPos(m_detections->getLocalPos() - m_worldToPixel*m_closestBuildingPos);
				}
				m_detections->setBuildingPos(m_closestBuildingPos);
				m_detections->setShowEnabled(true);

				m_currentBuildingBounds.clear();

				// reset detections building display
				int k = 0;
				for (int i = 0; i < m_defaultBldgNum; i++)
				{
					int numPolygons = m_polygonsPerBldg[i];
					for (int j = 0; j < numPolygons; j++)
					{
						m_buildingsD[k]->setShowEnabled(false);

						if (i == m_closestBuildingIndex)
						{
							m_buildingsD[k]->setLocalPos(-m_detections->getBuildingPos());
							m_buildingsD[k]->setShowEnabled(true);
							m_currentBuildingBounds.push_back(m_buildingBounds[k]);
						}
						k++;
					}
				}
			}
		}

		if (m_startIncreaseRadius && m_radiusClock.timeoutOccurred())
		{
			m_startIncreaseRadius = false;
			m_radiusClock.stop();

			m_increaseRadius = true;
			m_newRadiusCommandClock.reset();
			m_newRadiusCommandClock.start();

			m_radiusClock.reset();
			m_radiusClock.start();
		}

		if (m_increaseRadius && !m_radiusClock.timeoutOccurred())
		{
			// changed mode from travel to surround, so increase swarm radius
			m_AIRadius = (m_surroundRadius - m_travelRadius) / m_radiusTimeout * m_radiusClock.getCurrentTimeSeconds();
			m_newIncreaseRadiusCommand = true;

			if (!m_useServerThread && m_newIncreaseRadiusCommand)
			{
				m_com->radius(m_radiusPriority, 0, m_AIRadius);
				m_radiusPriority++;
			}

			double maxRadius = m_surroundRadius * (m_toolWorkspaceScaleFactor / 3125.0); // hcval
			if (m_AIRadius > maxRadius)
			{
				m_AIRadius = maxRadius;
				m_increaseRadius = false;
				m_radiusClock.stop();
			}
		}
		else
		{
			if (m_radiusClock.timeoutOccurred())
			{
				m_increaseRadius = false;
				m_radiusClock.stop();
			}
			m_newIncreaseRadiusCommand = false;
		}
            
        //---------------------------------------------------
        // Send information to robot distribution methods on the server
        //---------------------------------------------------
            
        // extract new robot positions from playdoh model when user is manipulating playdoh
        m_playdohRobotPositions = m_playdoh->getRobotPositions();

            
        // collect data
        if (m_useDataCollection)
        {
            // if threads aren't used, collect data here
            if (!m_useDataThread)
            {
                // data collection and write to file
                m_dataManager->collectAndWriteData(pos, force, m_distributionType, m_robotTeam->getRobotPositions());
				m_dataManager->collectAndWriteEventData((manipButton && !prevManipButton), (!manipButton && prevManipButton), manipButtonDoubleClick,
														(manipSpacingButton && !prevManipSpacingButton), (!manipSpacingButton && prevManipSpacingButton),
														(commandTravelButton && !prevCommandTravelButton), (!commandTravelButton && prevCommandTravelButton),
														(resizeButton && !prevResizeButton), (!resizeButton && prevResizeButton));

            }
            else
            {
                // collect data at a sample rate set by m_dataManager
                rtsData data = m_dataManager->collectData(pos, force, m_robotTeam->getRobotPositions());
                if (data.getValid())
                {
                    m_tempDataStack.push_back(data);
                }
                    
                if (m_tempDataStack.size() > 10 && !m_enableDataMutex)
                {
                    m_dataMutex->lock();
                    m_enableDataMutex = true;
                        
                    int m_tempDataStackSize = m_tempDataStack.size();
                    for (int i = 0; i < m_tempDataStackSize; i++)
                    {
                        m_dataStack.push_back(m_tempDataStack[i]);
                    }
                    m_tempDataStack.clear();
                        
                    //std::cout << "data being added" << std::endl;
                        
                    m_dataMutex->unlock();
                }
            }
        }

		// manip double click reset after buttons are recorded
		if (manipButtonDoubleClick)
		{
			manipDoubleClickRoutine();
			manipButtonDoubleClick = false;
		}
            
        ////// update hotspot manager
        //if (m_useServer)
        //    m_hotspotManager->updateTargetStates(m_robotTeam->getRobotPositions());

		if (!m_nsewValuesReady)
		{
			double currentTime;
			if (m_useTutorial)
				currentTime = m_tutorialManager->getCurrentTimeSeconds();
			else currentTime = m_simClock->getCurrentTimeSeconds();

			if (m_closestBldgIndexSet)
			{
				// update robot sensors
				m_hotspotManager->updateActiveTargetStates(currentTime, m_closestBuildingIndex + 1);

				vector<rtsHotspotGroup*> activeTargetGroups = m_hotspotManager->getActiveTargetGroups();
				int numActiveTargets = m_hotspotManager->getNumActiveTargets();
				bool type = false;
				if (m_distributionType == 1)
				{
					type = true;
				}
				m_nsewValues = m_robotTeam->updateSensorReadings(activeTargetGroups, numActiveTargets, m_nsewBuildingBounds[m_closestBuildingIndex], currentTime, type);
				if (m_useDetections)
				{
					m_sensorReadings = m_robotTeam->getSensorReadings();
					m_sensorPositions = m_robotTeam->getRobotPositions();
				}
			}
			else
			{
				vector<double> zeros(4, 0.0);
				m_nsewValues = zeros; // set to zeros
				//cout << "zero nsew values\n";
			}
			m_nsewValuesReady = true;
		}
        
        // set previous resize button for next iteration
        prevResizeButton = resizeButton;
        
        // set previous command travel button for next iteration
        prevCommandTravelButton = commandTravelButton;
        
        // set previous manip button for next iteration
        prevManipButton = manipButton;
		prevManipSpacingButton = manipSpacingButton;
        prevSwarmModeInt = m_swarmModeInt;
        prevToolPos = m_devicePos;
        
        // update frequency counter
        m_frequencyCounter.signal(1);
    }
    
    // exit haptics thread
    m_simulationFinished = true;
}

//------------------------------------------------------------------------------

cVector3d rtsApplication::getToolVelocity()
{
	cVector3d linearVelocity;
	m_tool->getHapticDevice()->getLinearVelocity(linearVelocity);
	return cVector3d(-linearVelocity.y(), -linearVelocity.z(), -linearVelocity.x()); // I think
}

//------------------------------------------------------------------------------

cVector3d rtsApplication::computeTravelForce(const cVector3d& a_devicePos, const cVector3d& a_travelToolStatPos)
{
	// compute tool force (virtual spring)
	double travelK = m_maxStiffness / 25.0;
	cVector3d travelForce = -travelK * (a_devicePos - a_travelToolStatPos);
	travelForce.z(0);

	// compute tool force (damping)
	double travelB = m_maxDamping / 10;
	cVector3d forceD = cMul(travelB * m_toolWorkspaceScaleFactor, getToolVelocity());
	travelForce.add(forceD);

	return travelForce;
}

cVector3d rtsApplication::computeSpacingForce(cVector3d a_devicePos, cVector3d a_manipSpacingCenter, 
											  const double& a_minSpacingRadius, const double& a_maxSpacingRadius)
{
	// create copies of vectors with zero z component
	cVector3d devicePos = a_devicePos;
	devicePos.z(0);
	cVector3d manipSpacingCenter = a_manipSpacingCenter;
	manipSpacingCenter.z(0);

	cVector3d totalSpacingForce(0, 0, 0);
	double spacingK = m_maxStiffness / 10.0;
	double innerSpacingK = 0.5 * spacingK;
	cVector3d centerToTool = devicePos - manipSpacingCenter;
	double distFromCenter = centerToTool.length();
	double innerPenetration = cClamp(distFromCenter, 0.0, a_minSpacingRadius);
	//cVector3d centerForce = -innerSpacingK * innerPenetration * cNormalize(centerToTool);

	// centering force during spacing manipulation
	cVector3d centerForce = -innerSpacingK * centerToTool;

	// add centering force to total force
	//totalSpacingForce.add(centerForce);

	// compute stiffness force for spacing
	double deltaSpacing = cClamp(distFromCenter - a_minSpacingRadius, 0.0, a_maxSpacingRadius - a_minSpacingRadius);
	cVector3d spacingForce = -spacingK * deltaSpacing * cNormalize(centerToTool);

	// compute ramp force to signal too far away
	double outsideSpacingDist = cClamp(distFromCenter - a_maxSpacingRadius, 0.0, 1.25 * a_maxSpacingRadius);
	cVector3d boundForce = -0.15*m_maxStiffness * outsideSpacingDist * cNormalize(centerToTool);
	totalSpacingForce.add(boundForce);

	// compute damping force for spacing
	double spacingB = 0.50*m_maxDamping;
	cVector3d spacingDamping = cMul(spacingB * m_toolWorkspaceScaleFactor, getToolVelocity());
	totalSpacingForce.add(spacingDamping);

	totalSpacingForce.add(spacingForce);

	return totalSpacingForce;
}

cVector3d rtsApplication::computeResizeForce(cVector3d a_devicePos, cVector3d a_resizeCenter)
{
	double posVariance = 0.01;
	double negVariance = 0.01;
	double variance;
	//m_resizePercentage = m_resizePercentage + 0.0000001 * (a_devicePos.z() - a_resizeToolStartPos.z() / m_toolWorkspaceScaleFactor);
	if ((a_devicePos.z() - a_resizeCenter.z()) >= 0)
	{
		variance = posVariance;
	}
	else variance = negVariance;

	m_resizePercentage = 1.0 + variance * (a_devicePos.z() - a_resizeCenter.z()) / m_toolWorkspaceScaleFactor;
	//cout << "resize P: " << m_resizePercentage << endl;

	
	m_totalResizePercentage *= m_resizePercentage;

	if (m_totalResizePercentage > m_maxResizePercentage || m_totalResizePercentage < m_minResizePercentage)
	{
		m_totalResizePercentage = cClamp(m_totalResizePercentage, m_minResizePercentage, m_maxResizePercentage);
		m_resizePercentage = 1.0;
	}

	//cout << "total resize P: " << m_totalResizePercentage << endl;


	m_AIRadius = m_AIRadius*m_resizePercentage;

	//std::cout << m_virtualWorkspaceRadius << std::endl;
	m_playdoh->setResizePercentage(m_resizePercentage);
	//std::cout << resizeToolStartPos.z() << std::endl;

	//// store previous z value
	//prevToolZ = a_devicePos.z();

	// compute tool force (virtual spring)
	double kv = m_maxStiffness / 7.0;
	cVector3d resizeForce;
	resizeForce = cVector3d(0.0, 0.0, -kv*(a_devicePos.z() - a_resizeCenter.z()));

	// compute tool force (damping)
	double resizeB = 0.25*m_maxDamping;
	cVector3d forceD = cMul(resizeB, getToolVelocity());
	resizeForce.add(forceD);

	//cout << "resizeForce: " << resizeForce << endl;

	return resizeForce;
}


void rtsApplication::manipDoubleClickRoutine()
{
	// reset team formation to original formation around building
	m_resetToStartManip = true;

	m_manipCompleted = false;
	m_resizeSurroundCompleted = false;
}

// compute robot team haptic feedback forces
void rtsApplication::computeHapticFeedback(const cVector3d& a_devicePos, const double& a_timeInterval,
											const bool& a_travelButton, bool& a_endTravelForce, cPrecisionClock& a_endTravelClock,
											const cVector3d& a_travelToolStartPos, const double& a_endPTimeout,
											const bool& a_resizeButton, const cVector3d& a_resizeToolStartPos, bool& a_startResizeForce, cPrecisionClock& a_startResizeClock,
											bool& a_endResizeForce, cPrecisionClock& a_endResizeClock,
											const bool& a_manipButton,
											const bool& a_manipSpacingButton, bool& a_startSpacingForce, cPrecisionClock& a_startSpacingClock,
											bool& a_endSpacingForce, cPrecisionClock& a_endSpacingClock,
											const cVector3d& a_manipSpacingCenter, const double a_eSTimout, const double a_sSTimeout,
											const double& a_minSpacingRadius, const double& a_maxSpacingRadius)
{
	// if the user is commanding a travel, compute the travel force
	if (a_travelButton)
	{
		// compute travel force
		cVector3d travelForce = computeTravelForce(a_devicePos, a_travelToolStartPos);

		// add additional force to calculated force from tool
		m_tool->addDeviceGlobalForce(travelForce);
	}
	// if the user is resizing, compute that force
	else if (a_resizeButton)
	{
		cVector3d resizeForce = computeResizeForce(a_devicePos, a_resizeToolStartPos);

		if (a_startResizeForce)
		{
			if (a_startResizeClock.timeoutOccurred())
			{
				a_startResizeForce = false;
				a_startResizeClock.stop();
			}
			else
			{
				double startResizeForceFactor = a_startResizeClock.getCurrentTimeSeconds() / a_startResizeClock.getTimeoutPeriodSeconds();
				resizeForce.mul(startResizeForceFactor);
			}
		}


		// use resize force as only tool force
		m_tool->setDeviceGlobalForce(0, 0, 0);
		m_tool->addDeviceGlobalForce(resizeForce);

		// needed for node updating, but not to retrieve feedback force
		double modFactor = 1.0;
		cVector3d playdohForce = m_playdoh->computeForce(a_devicePos, m_deviceRadius, 0, modFactor, a_manipButton, a_manipSpacingButton);
		//m_playdoh->updateDynamics(a_timeInterval);
	}
	else if (a_manipSpacingButton)
	{
		//cout << "spacing else if in haptic feedback" << endl;
		int nearestSpacingIndex, farthestSpacingIndex;
		getNearestAndFarthestSurroundIndices(a_devicePos, nearestSpacingIndex, farthestSpacingIndex);

		// solve for modification spacing factor (ranging from lowFactor < factor < 1.0)
		double lowFactor = 0.25; // hcval
		double spacingPen = cDistance(a_devicePos, a_manipSpacingCenter) - a_minSpacingRadius;
		double modFactor = 1.0 - (spacingPen) / (a_maxSpacingRadius - a_minSpacingRadius) * (1.0 - lowFactor);

		modFactor = cClamp(modFactor, lowFactor, 1.0);
		//cout << modFactor << endl;

		// update playdoh to update spacing between robot nodes
		cVector3d playdohForce = m_playdoh->computeForce(a_devicePos, m_deviceRadius, nearestSpacingIndex, modFactor, false, a_manipSpacingButton);
		//force.add(playdohForce);
		m_playdoh->updateDynamics(a_timeInterval);


		// compute force from spacing manipulation
		cVector3d spacingForce = computeSpacingForce(a_devicePos, a_manipSpacingCenter, a_minSpacingRadius, a_maxSpacingRadius);

		if (a_startSpacingForce)
		{
			if (a_startSpacingClock.timeoutOccurred())
			{
				a_startSpacingForce = false;
				a_startSpacingClock.stop();
			}
			else
			{
				double startSpacingForceFactor = a_startSpacingClock.getCurrentTimeSeconds() / a_startSpacingClock.getTimeoutPeriodSeconds();
				spacingForce.mul(startSpacingForceFactor);
			}
		}

		// add additional force to calculated force from tool
		m_tool->addDeviceGlobalForce(spacingForce);
	}
	else
	{
		//cout << "else in computeHapticFeedback" << endl;
		int nearestIndex = getNearestSurroundIndex(a_devicePos);
		double modFactor = 1.0;
		// update playdoh
		cVector3d playdohForce = m_playdoh->computeForce(a_devicePos, m_deviceRadius, nearestIndex, modFactor, a_manipButton, a_manipSpacingButton);

		if (a_manipButton)
		{
			if (m_playdoh->getContactState())
				m_playdoh->updateDynamics(a_timeInterval);
		}
		else m_playdoh->updateDynamics(a_timeInterval);
		// scale force based on m_virtualWorkspaceRadius, to adjust magnitude of force to several workspace scale factors
		if (m_distributionType == 0)
		{
			// travel force adjustment
			//force = (35 / m_virtualWorkspaceRadius)*force; // (hcval)
			//cVector3d toolVel(m_tool->getWorkspaceScaleFactor() * m_tool->getDeviceLocalLinVel());
			// compute travel force
			cVector3d relativeForce = computeTravelForce(a_devicePos, m_playdoh->getCenterOfMass());

			// add additional force to calculated force from tool
			m_tool->addDeviceGlobalForce(relativeForce);
		}
		else if (m_distributionType == 1)
		{
			// surround force adjustment
			//force = 1.00*force;
		}

		// gradually increase playdoh force if completing a spacing routine
		if (a_endSpacingForce)
		{
			double endSpacingForceFactor = a_endSpacingClock.getCurrentTimeSeconds() / a_endSpacingClock.getTimeoutPeriodSeconds();
			playdohForce.mul(endSpacingForceFactor);
		}

		// gradually increase playdoh force if completing a resize routine
		if (a_endResizeForce)
		{
			double endResizeForceFactor = a_endResizeClock.getCurrentTimeSeconds() / a_endResizeClock.getTimeoutPeriodSeconds();
			playdohForce.mul(endResizeForceFactor);
		}

		// add additional force to calculated force from tool
		m_tool->addDeviceGlobalForce(playdohForce);
	}

	// add on zeroing travel force
	if (a_endTravelForce)
	{
		// if done with travel force, gradually zero it out
		if (a_endTravelClock.timeoutOccurred())
		{
			a_endTravelForce = false;
			a_endTravelClock.stop();
		}
		else
		{
			// update travel to zero factor
			double endTravelForceFactor = 1.0 - a_endTravelClock.getCurrentTimeSeconds() / a_endTravelClock.getTimeoutPeriodSeconds();

			// compute travel force
			cVector3d travelForce = computeTravelForce(a_devicePos, a_travelToolStartPos);
			travelForce.mul(endTravelForceFactor);

			// add additional force to calculated force from tool
			m_tool->addDeviceGlobalForce(travelForce);
		}
	}

	// add on zeroing spacing force
	if (a_endSpacingForce)
	{
		// if done with travel force, gradually zero it out
		if (a_endSpacingClock.timeoutOccurred())
		{
			a_endSpacingForce = false;
			//a_endSpacingForceFactor = 1.0;
			a_endSpacingClock.stop();
		}
		else
		{
			// update travel to zero factor
			double endSpacingForceFactor = 1.0 - a_endSpacingClock.getCurrentTimeSeconds() / a_endSpacingClock.getTimeoutPeriodSeconds();

			// compute force from spacing manipulation
			cVector3d spacingForce = computeSpacingForce(a_devicePos, a_manipSpacingCenter, a_minSpacingRadius, a_maxSpacingRadius);

			// compute spacing force to zero
			spacingForce.mul(endSpacingForceFactor);

			// add additional force to calculated force from tool
			m_tool->addDeviceGlobalForce(spacingForce);
		}
	}

	if (a_endResizeForce)
	{
		// if done with travel force, gradually zero it out
		if (a_endResizeClock.timeoutOccurred())
		{
			a_endResizeForce = false;
			//a_endSpacingForceFactor = 1.0;
			a_endResizeClock.stop();
		}
		else
		{
			// update travel to zero factor
			double endResizeForceFactor = 1.0 - a_endResizeClock.getCurrentTimeSeconds() / a_endResizeClock.getTimeoutPeriodSeconds();

			// compute force from spacing manipulation
			cVector3d resizeForce = computeResizeForce(a_devicePos, a_resizeToolStartPos);

			// compute spacing force to zero
			resizeForce.mul(endResizeForceFactor);

			// gradually increase ground force
			cVector3d toolForce = m_tool->getDeviceGlobalForce();
			m_tool->setDeviceGlobalForce(0, 0, 0);
			toolForce.mul(1.0 - endResizeForceFactor);
			m_tool->addDeviceGlobalForce(toolForce);

			// add additional force to calculated force from tool
			m_tool->addDeviceGlobalForce(resizeForce);
		}
	}

}


//------------------------------------------------------------------------------

void rtsApplication::startSimRoutine()
{
	m_robotTeam->setShowEnabled(false);
	m_playdohRobotTeam->setShowEnabled(false);

	cPrecisionClock readyClock;
	bool readyClockStarted = false;
	readyClock.setTimeoutPeriodSeconds(0.5);

	// wait for high enough haptic rate before starting simulation
	bool enteredSimReadyLoop = false;
	while (!m_simReady)
	{

		if (!enteredSimReadyLoop)
		{
			m_robotTeam->clearSensorReadings();
			enteredSimReadyLoop = true;
		}

		// compute global reference frames for each object
		m_world->computeGlobalPositions(true);

		// update position and orientation of tool
		m_tool->updateFromDevice();

		// compute interaction forces
		m_tool->computeInteractionForces();

		//m_tool->setDeviceGlobalForce(0, 0, 0);
		m_tool->applyToDevice();

		m_frequencyCounter.signal(1);

		// check haptic rate value
		int hRate = m_frequencyCounter.getFrequency();
		if (m_numHapticDevices > 0)
		{
			if (hRate > m_minStartupHapticRate)
			{
				if (!readyClockStarted)
				{
					readyClock.reset();
					readyClock.start();
					readyClockStarted = true;
				}
				else
				{
					if (readyClock.timeoutOccurred())
						m_simReady = true;
				}
			}
		}
		else if (hRate > m_minStartupHapticRateNoDevice)
		{
			if (!readyClockStarted)
			{
				readyClock.reset();
				readyClock.start();
				readyClockStarted = true;
			}
			else
			{
				if (readyClock.timeoutOccurred())
					m_simReady = true;
			}
		}

		if (m_simReady)
		{
			m_simReadyMessage->setText("Press Enter to begin");
		}

	}

	// wait for user to press button to begin
	if (m_simReady && !m_beginSim)
	{
		bool flag = false;
		while (m_simulationRunning && !flag)
		{
			// compute global reference frames for each object
			m_world->computeGlobalPositions(true);

			// update position and orientation of tool
			m_tool->updateFromDevice();

			// compute interaction forces
			m_tool->computeInteractionForces();

			//m_tool->setDeviceGlobalForce(0, 0, 0);
			m_tool->applyToDevice();

			if (m_beginSim)
			{
				// simulation ready, so unpause server
				if (!m_useTutorial)
					m_com->unpause();
				m_useServer = true;
				
				// start simulation clock
				m_simClock->initClock();

				// start tutorial clock
				if (m_useTutorial)
				{
					m_tutorialManager->initClock();
				}

				// turn on haptics
				if (!m_useTutorial)
				{
					m_playdoh->setHapticEnabled(true, true);
				}
				
				// update size
				m_simClock->updateSize();

				// reset speed clock for testing
				m_speedClock.reset();

				// turn off ready message
				m_simReadyPanel->setShowEnabled(false, true);

				// decide to show team
				//m_robotTeam->setShowEnabled(true);
				//if (m_useVisualDemo)
				//{
				if (m_visualEnabled)
				{
					m_robotTeam->setShowEnabled(true);
				}
				else
				{
					m_robotTeam->setShowEnabled(m_visualEnabled, false);
				}
				//}
				//else
				//{
				//	m_robotTeam->setShowEnabled(true);
				//}
				m_robotTeam->setShowId(false);

				// start data manager clock
				m_dataManager->resetClock();
				m_dataManager->startClock();

				flag = true;
			}
		}
	}
}

//------------------------------------------------------------------------------

void rtsApplication::retrieveRobotData(bool a_manipButton, bool a_manipSpacingButton, bool a_resizeButton)
{
    if (m_useServer)
    {
        comm::Swarm thisSwarm;
        bool serverUpdate = false;
        if (m_useServerThread)
        {
            // retrieve the swarm from server
            if (!m_enableQueryMutex)
            {
                if (m_isNewSwarm)
                {
                    m_queryMutex->lock();

						m_enableQueryMutex = true;
                    
						// get swarm
						thisSwarm = *m_swarm;
                    
						m_isNewSwarm = false;

						m_servernsewValues = m_nsewValues;
						
						//m_serverManipButton = a_manipButton;
						//m_serverManipCompleted = m_manipCompleted;

						m_serverManipButton = a_manipButton;
						m_serverContacted = m_playdoh->getContactState();
						m_serverManipCompleted = m_manipCompleted;

						m_serverResizeButton = a_resizeButton;
						m_serverResizeSurroundCompleted = m_resizeSurroundCompleted;

						m_serverTravelCommanded = m_travelCommanded;
						m_serverTravelDir1 = m_commandedTravelDir1;
						m_serverTravelDir2 = m_commandedTravelDir2;
						m_serverTravelId1 = m_travelId1;
						m_serverTravelId2 = m_travelId2;
						m_serverAIRadius = m_AIRadius;
						m_serverIncreaseRadius = m_newIncreaseRadiusCommand;

						m_serverResetToStartManip = m_resetToStartManip;
						if (m_serverResetToStartComplete)
						{
							m_resetToStartManip = false;
							m_serverResetToStartComplete = false;
							m_manipCompleted = true; // keep positions at the start position
							m_manipCompletedPositions = m_manipStartedPositions;
							m_resizeSurroundCompleted = false;
							//cout << "completed reset to start position" << endl;

							// reset to uniform spacing
							m_playdoh->resetSpacingFactors();
						}
                    
						// if team is being manipulated, or has been since the last travel command
						if (m_resetToStartManip)
						{
							//cout << "attempting to reset to start position" << endl;
							m_serverDesiredPositions = m_manipStartedPositions;
							m_serverCurrentPositions = m_robotTeam->getRobotPositions();
						}
						else if (a_manipButton || a_manipSpacingButton || a_resizeButton)
						{
							//cout << "choosing playdoh positions as desired" << endl;
							m_serverDesiredPositions = m_playdohRobotTeam->getRobotPositions();
							//m_serverDesiredPositions = m_robotPositions;
							m_serverCurrentPositions = m_robotTeam->getRobotPositions();
						}
						else if (m_manipCompleted)
						{
							//cout << "choosing completed positions as desired" << endl;
							m_serverDesiredPositions = m_manipCompletedPositions;
							m_serverCurrentPositions = m_robotTeam->getRobotPositions();
						}
						else if (m_resizeSurroundCompleted)
						{
							//cout << "resizeSurroundCompletedPositions given to server" << endl;
							m_serverDesiredPositions = m_resizeSurroundCompletedPositions;
							m_serverCurrentPositions = m_robotTeam->getRobotPositions();
						}
                    
                    m_queryMutex->unlock();
                    
                    serverUpdate = true;
                }
            }
            
        }
        else
        {
            // query server at a certain frequency
            if (m_serverTimer.timeoutOccurred())
            {
                m_swarm = m_com->swarm();
                thisSwarm = *m_swarm;
                
                serverUpdate = true;
                m_serverTimer.reset();
                m_serverTimer.start();
            }
            
			//cout << "entered else statement for no threads in retrieveRobotData" << endl;
            if (a_manipButton)
            {
                //cout << "updateForces" << endl;
                
                // update controller
                m_controller->updateForces(m_playdohRobotTeam->getRobotPositions(),
                                         m_robotTeam->getRobotPositions());

				//m_controller->updateForces(m_playdohRobotPositions,
				//							m_robotTeam->getRobotPositions());
            }
        }
        
        // if the server is set to update this iteration
        if (serverUpdate)
        {
            
            //std::cout << "cycle: " << thisSwarm.cycle() << " " << "swarm mode: " << m_swarmModeInt << std::endl;
            
            m_swarmRadius = thisSwarm.radius();
            //std::cout << "radius from swarm: " << m_swarmRadius << std::endl;
            
            auto cycle = thisSwarm.cycle();
            bool boolCycle = !cycle.empty();
            
            
            // if travel has been commanded this iteration, cycle is not found
            if (m_travelCommanded)
            {
                m_foundCycle = false;
            }
            
            // if a cycle has already been found,
            if (m_foundCycle)
            {
                // no travel has been commanded
                m_spanningCycle = true;
                
            }
            // else if travel not commanded and server finds a cycle,
            else if (!m_travelCommanded && boolCycle && m_swarmModeInt == 1)
            {
                if (m_cycleCounter.timeoutOccurred()) //hcval
                {
                    m_spanningCycle = true;
                    m_foundCycle = true;
                    m_cycleCounter.reset();
                    m_cycleCounter.start();
                }
            }
            else
            {
                m_spanningCycle = false;
                m_cycleCounter.reset();
                m_cycleCounter.start();
            }


            
            if (m_travelCommanded)
            {
                // reset m_travelCommanded
                m_travelCommanded = false;
            }
            
            //std::cout << "m_foundCycle: " << m_foundCycle << " " << "spanningCycle: " << spanningCycle << std::endl;
            

            
            if (m_spanningCycle)
            {
                
                // if switching from travel mode to surround mode
                // surround Ids should be empty if making this switch
                if (m_distributionType == 0 && !m_surroundStarted)
                {
                    // set the ids for this surround
                    setSurroundIds(&thisSwarm);
                    
                    // on first iteration
                    m_surroundIter1 = true;
                    
                    //printVector(m_surroundIds);
                    
                    m_surroundStarted = true;
                }
                
                if (m_surroundStarted)
                {
                    // set to surround distribution type
                    m_distributionType = 1;
                    
					// change color of detections to match surround color
					if (m_useDetections)
					{
						m_detections->setColor(cColorf(0.0f, 0.2f, 1.0f));
					}

					m_robotTeam->setSensorColors(cColorf(0.0f, 0.2f, 1.0f));

					int numScopes = m_scopes.size();
					for (int i = 0; i < numScopes; i++)
					{
						m_scopes[i]->m_colorSignal0 = cColorf(0.0f, 0.5f, 1.0f);
					}

					// set building bounds in playdoh class (ultimately for node class)
					m_playdoh->setBuildingBounds(m_currentBuildingBounds);

					// show surround lines
					m_robotTeam->setShowSurroundLines(m_showSurroundOutline);

					// disable travel capability until travel command is given
					m_travelEnabled = false;

                    // reset surround started
                    m_surroundStarted = false;
                }
            }
            else
            {                
                // set distribution type to travel mode
                m_distributionType = 0;
                
                // clear surround idsk
                m_surroundIds.clear();
                
                //printVector(m_ids);
            }

			// query agent locations, setting order if in surround mode
			m_robotPositions = swarm2Vec(&thisSwarm, m_spanningCycle);
            
            // retrieve raw robot positions from server
            m_rawRobotPositions = swarm2Vec(&thisSwarm, false);
            
            // set colors according to mode type
            //setAgentModeColors(&thisSwarm);
            
        } // end of server update
        
    } // end of use server
    
    // set distribution mode (0 - travel, 1 - surround, 2 - travel and surround)
    m_robotTeam->setDistributionMode(m_distributionType);
    
    if (m_distributionType == 1)
    {
        m_robotTeam->setRobotPositions(m_robotPositions, m_surroundIds);
        //printVector(m_surroundIds);
        
        // set playdoh robot team positions
        m_playdohRobotTeam->setRobotPositions(m_playdohRobotPositions, m_surroundIds);
    }
    else if (m_distributionType == 0)
    {
        m_robotTeam->setRobotPositions(m_robotPositions, m_ids);
        //printVector(m_ids);

		m_playdohRobotTeam->setRobotPositions(m_playdohRobotPositions, m_ids);
    }
        
    if (m_showTravelOutline)
    {
        // set convex hull properties
        m_robotTeam->updateConvexHullLines();
    }
    
    // set convex hull in playdoh
    m_playdoh->setConvexHull(m_robotTeam->getConvexHull());
    m_playdoh->setConvexHullMapping(m_robotTeam->getConvexHullMapping());
    
    //printVector(m_robotTeam->getConvexHullMapping());
    
    // if in the process of creating a new surround
    if (m_surroundIter1 || m_surroundIter2)
    {
        // update agent positions in playdoh model before applying dynamics
        if (!a_manipButton)
            m_playdoh->updateRobotPositions(m_robotPositions);
        //else m_playdoh->updateRobotPositions(m_playdohRobotPositions);
        
        // set distribution mode (0 - travel, 1 - surround, 2 - travel and surround)
        // (this is where playdoh haptics are initialized for each mode)
        // this must be set AFTER updateRobotPositions is called, due to standby mode skipping until next iteration!!
        m_playdoh->setDistributionMode(m_distributionType);
        
        // update agent positions in playdoh model before applying dynamics
        // for some reason, this is needed after setting distribution mode to create colors correctly
        if (!a_manipButton)
            m_playdoh->updateRobotPositions(m_robotPositions);
        //else m_playdoh->updateRobotPositions(m_playdohRobotPositions);
    }
    else
    {
        // set distribution mode (0 - travel, 1 - surround, 2 - travel and surround)
        // (this is where playdoh haptics are initialized for each mode)
        // this must be set AFTER updateRobotPositions is called, due to standby mode skipping until next iteration!!
        m_playdoh->setDistributionMode(m_distributionType);
        
        // update agent positions in playdoh model before applying dynamics
		if (!a_manipButton && !a_resizeButton && !a_manipSpacingButton)
		{
			m_playdoh->updateRobotPositions(m_robotPositions);
		}
    }
    
    // surround iteration reset
    if (m_surroundIter1)
    {
        // on second iteration
        m_surroundIter2 = true;
        m_surroundIter1 = false;
    }
    else if (m_surroundIter2)
    {
        // reset both iteration sequences
        m_surroundIter1 = false;
        m_surroundIter2 = false;
    }
}

vector<cVector3d> rtsApplication::swarm2Vec(comm::Swarm* a_swarm, bool a_setIdOrder)
{
	// clear list of m_ids first
	m_ids.clear();

	unsigned int num_agents = a_swarm->agents_size();
	vector<cVector3d> orderedPositions(num_agents, cVector3d(0, 0, 0));
	vector<cVector3d> positions(num_agents, cVector3d(0, 0, 0));
	vector<int> ids(num_agents, 0);

	// store the positions in an ordered list or order them according to m_surroundIds
	int numSurroundIds = m_surroundIds.size();
	bool modeInt = 1;
	for (unsigned int i = 0; i < num_agents; ++i) 
	{
		// set ordered positions
		comm::Swarm_Agent current = a_swarm->agents(i);
		ids[current.id()] = current.id();
		orderedPositions[current.id()] = cVector3d(current.x(), current.y(), 0); // force z to be zero!

		// set overall swarm mode
		if (modeInt == 1 && current.mode() == 2 || current.mode() == 3)
		{
			modeInt = 0;
		}

		// set individual swarm mode colors
		setAgentModeColor(&current);
	}
	
	m_swarmModeInt = modeInt;
	if (a_setIdOrder)
	{
		int numSurroundIds = m_surroundIds.size();
		for (int i = 0; i < numSurroundIds; ++i)
		{
			positions[i] = orderedPositions[m_surroundIds[i]];
		}
		return positions;
	}
	else
	{
		m_ids = ids;
		return orderedPositions;
	}
}

vector<cVector3d> rtsApplication::polygon2Vec(comm::PolygonList_Polygon* a_polygon)
{
    vector<cVector3d> positions; 
    unsigned int num_points = a_polygon->vertices_size();
    for (unsigned int j = 0; j < num_points; ++j) {
        comm::PolygonList_Polygon_Vertex vert = a_polygon->vertices(j);
        positions.push_back(cVector3d(vert.x(), vert.y(), vert.z()));
    }
    
    return positions;
}

vector<vector<cVector3d>> rtsApplication::polygons2Vec(comm::PolygonList* a_polygons)
{
    vector<vector<cVector3d>> polygonPositions; 
    unsigned int num_polygons = a_polygons->polygons_size();
    for (unsigned int i = 0; i < num_polygons; ++i) {
        comm::PolygonList_Polygon poly = a_polygons->polygons(i); 
        polygonPositions.push_back(polygon2Vec(&poly));
    }
    
    return polygonPositions;
}

void rtsApplication::setSurroundIds(comm::Swarm* a_swarm)
{
    // clear list of ids first
    m_surroundIds.clear();
    unsigned int num_agents = a_swarm->agents_size();
    for (unsigned int i = 0; i < num_agents; ++i) 
	{
        m_surroundIds.push_back(a_swarm->cycle(i));
    }
}

void rtsApplication::printVector(vector<int> a_vector)
{
    for (unsigned int k = 0; k < a_vector.size(); k++)
    {
        std::cout << a_vector[k] << ", ";
        if (k == a_vector.size() - 1)
            std::cout << std::endl;
    }
}

int rtsApplication::findNearestAgent(cVector3d a_pos, bool useServerPositions)
{
    int nearestId = 0;
    int nearestIndex = 0;
    
    // command from surround mode
    if (m_distributionType == 1 && !m_surroundIds.empty())
    {
        // choose which vector to use
        vector<cVector3d> positions;
        if (useServerPositions)
            positions = m_robotPositions;
        else positions = m_playdohRobotPositions;
        
        // trusting the ids and m_robotPositions order
        nearestId = m_surroundIds[nearestIndex];
        for (unsigned int k = 0; k < m_surroundIds.size(); ++k)
        {
            if ((a_pos - positions[k]).length() < (a_pos - positions[nearestIndex]).length())
            {
                // set closest id
                nearestIndex = k;
                nearestId = m_surroundIds[nearestIndex];
            }
        }
    }
    else if (m_distributionType == 0 && !m_ids.empty())
    {
        nearestId = m_ids[nearestIndex];
        for (unsigned int k = 0; k < m_ids.size(); ++k)
        {
            if ((a_pos - m_robotPositions[k]).length() < (a_pos - m_robotPositions[nearestIndex]).length())
            {
                // set closest id
                nearestIndex = k;
                nearestId = m_ids[nearestIndex];
            }
        }
    }
    
    return nearestId;
}

void rtsApplication::findNearestTwoAgents(const cVector3d& a_pos, int& a_closest, int& a_secondClosest)
{
	int index = 0;
	int index2 = 1;

	// command from surround mode
	if (m_distributionType == 1 && !m_surroundIds.empty())
	{
		// trusting the ids and m_robotPositions order
		a_closest = m_surroundIds[index];
		a_secondClosest = m_surroundIds[index2];
		for (unsigned int k = 0; k < m_surroundIds.size(); ++k)
		{
			if (cDistance(a_pos, m_robotPositions[k]) < cDistance(a_pos, m_robotPositions[index]))
			{
				// set second closest id
				index2 = index;
				a_secondClosest = a_closest;

				// set closest id
				index = k;
				a_closest = m_surroundIds[index];
			}
			else if (cDistance(a_pos, m_robotPositions[k]) < cDistance(a_pos, m_robotPositions[index2]))
			{
				// set second closest id
				if (index != k)
				{
					index2 = k;
					a_secondClosest = m_surroundIds[index2];
				}
			}
		} // end for loop on m_surroundIds

	}
	// command from travel mode
	else if (m_distributionType == 0 && !m_ids.empty())
	{
		//vector<int> travelIds;

		// trusting the m_ids and m_robotPositions order
		a_closest = m_ids[index];
		a_secondClosest = m_ids[index2];
		for (unsigned int k = 0; k < m_ids.size(); ++k)
		{
			if (cDistance(a_pos, m_robotPositions[k]) < cDistance(a_pos, m_robotPositions[index]))
			{
				// set second closest id
				a_secondClosest = a_closest;
				index2 = index;

				// set closest id
				index = k;
				a_closest = m_ids[index];
			}
			else if (cDistance(a_pos, m_robotPositions[k]) < cDistance(a_pos, m_robotPositions[index2]))
			{
				// set second closest id
				if (index != k)
				{
					index2 = k;
					a_secondClosest = m_ids[index2];
				}
			}
		} // end for loop on m_ids

	}
}


int rtsApplication::findFarthestAgent(cVector3d a_pos, bool useServerPositions)
{
    int farthestId = 0;
    int farthestIndex = 0;
    
    // command from surround mode
    if (m_distributionType == 1 && !m_surroundIds.empty())
    {
        // choose which vector to use
        vector<cVector3d> positions;
        if (useServerPositions)
            positions = m_robotPositions;
        else positions = m_playdohRobotPositions;
        
        // trusting the ids and m_robotPositions order
        farthestId = m_surroundIds[farthestIndex];
        for (unsigned int k = 0; k < m_surroundIds.size(); ++k)
        {
            if ((a_pos - positions[k]).length() >(a_pos - positions[farthestIndex]).length())
            {
                // set closest id
                farthestIndex = k;
                farthestId = m_surroundIds[farthestIndex];
            }
        }
    }
    else if (m_distributionType == 0 && !m_ids.empty())
    {
        farthestId = m_ids[farthestIndex];
        for (unsigned int k = 0; k < m_ids.size(); ++k)
        {
            if ((a_pos - m_robotPositions[k]).length() >(a_pos - m_robotPositions[farthestIndex]).length())
            {
                // set closest id
                farthestIndex = k;
                farthestId = m_ids[farthestIndex];
            }
        }
    }
    
    return farthestId;
}

void rtsApplication::setAgentModeColor(comm::Swarm_Agent* a_swarmAgent)
{       
    // set colors of each mode
    cColorf surroundColor1 = cColorf(	0 / 256.0,
									85 / 256.0,
									256 / 256.0, 1);

    cColorf surroundColor = cColorf(255 / 256.0,
                                    0 / 256.0,
                                    0 / 256.0, 1);
	cColorf travelLeadColor = cColorf(255 / 256.0,
		0 / 256.0,
		0 / 256.0, 1);

	cColorf travelFollowColor = cColorf(255 / 256.0,
		0 / 256.0,
		0 / 256.0, 1);

	if (m_beginSim)
	{
		if (m_surroundIds.empty())
		{
			// set the mode color for current agent
			if (a_swarmAgent->mode() == 1)
				m_robotTeam->getRobot(a_swarmAgent->id())->setColor(surroundColor);
			else if (a_swarmAgent->mode() == 2)
				m_robotTeam->getRobot(a_swarmAgent->id())->setColor(travelLeadColor);
			else if (a_swarmAgent->mode() == 3)
				m_robotTeam->getRobot(a_swarmAgent->id())->setColor(travelFollowColor);
		}
		else
		{
			m_robotTeam->getRobot(a_swarmAgent->id())->setColor(surroundColor1);
		}
	}
}

void rtsApplication::getNearestAndFarthestSurroundIndices(cVector3d a_pos, int& a_nearest, int& a_farthest)
{
	//int nearestId = 0;
	int nearestIndex = 0;
	int farthestIndex = 0;

	// command from surround mode
	if (m_distributionType == 1 && !m_surroundIds.empty())
	{
		// choose which vector to use
		vector<cVector3d> positions;
		positions = m_playdohRobotPositions;

		// trusting the ids and m_robotPositions order
		//nearestId = m_surroundIds[nearestIndex];
		for (unsigned int k = 0; k < m_surroundIds.size(); ++k)
		{
			if (cDistance(a_pos, positions[k]) < cDistance(a_pos, positions[nearestIndex]))
			{
				// set closest id
				nearestIndex = k;
				//nearestId = m_surroundIds[nearestIndex];
			}

			if (cDistance(a_pos, positions[k]) > cDistance(a_pos, positions[farthestIndex]))
			{
				// set farthest index
				farthestIndex = k;
			}
		}
	}
	else if (m_distributionType == 0 && !m_ids.empty())
	{
		//nearestId = m_ids[nearestIndex];
		for (unsigned int k = 0; k < m_ids.size(); ++k)
		{
			if ((a_pos - m_robotPositions[k]).length() < (a_pos - m_robotPositions[nearestIndex]).length())
			{
				// set closest id
				nearestIndex = k;
				//nearestId = m_ids[nearestIndex];
			}

			if ((a_pos - m_robotPositions[k]).length() > (a_pos - m_robotPositions[farthestIndex]).length())
			{
				// set farthest index
				farthestIndex = k;
			}
		}
	}

	a_nearest = nearestIndex;
	a_farthest = farthestIndex;
}

int rtsApplication::getNearestSurroundIndex(cVector3d a_pos)
{
    int nearestId = 0;
    int nearestIndex = 0;
    
    // command from surround mode
    if (m_distributionType == 1 && !m_surroundIds.empty())
    {
        // choose which vector to use
        vector<cVector3d> positions;
        positions = m_playdohRobotPositions;
        
        // trusting the ids and m_robotPositions order
        nearestId = m_surroundIds[nearestIndex];
        for (unsigned int k = 0; k < m_surroundIds.size(); ++k)
        {
            if ((a_pos - positions[k]).length() < (a_pos - positions[nearestIndex]).length())
            {
                // set closest id
                nearestIndex = k;
                nearestId = m_surroundIds[nearestIndex];
            }
        }
    }
    else if (m_distributionType == 0 && !m_ids.empty())
    {
        nearestId = m_ids[nearestIndex];
        for (unsigned int k = 0; k < m_ids.size(); ++k)
        {
            if ((a_pos - m_robotPositions[k]).length() < (a_pos - m_robotPositions[nearestIndex]).length())
            {
                // set closest id
                nearestIndex = k;
                nearestId = m_ids[nearestIndex];
            }
        }
    }
    
    return nearestIndex;
}

void rtsApplication::startSpeedClock()
{
	if (m_testSpeed)
	{
		m_speedClock.reset();
		m_speedClock.start();
	}
}

void rtsApplication::stopSpeedClock()
{
	if (m_testSpeed)
	{
		m_speedClock.stop();

		if (m_checkRegions)
		{
			m_regionTimeIntervals.push_back(m_speedClock.getCurrentTimeSeconds());
		}

		//std::cout << m_speedClock.getCurrentTimeSeconds() << std::endl;
		//m_testSpeed = false;
		m_speedClock.reset();
	}
}

void rtsApplication::setClosestBldgIndex()
{
	int index = 0;
	double shortest = 1000.0;
	cVector3d buildingPos;
	int numBounds = m_nsewBuildingBounds.size();
	for (int i = 0; i < numBounds; i++)
	{
		// check distance to com of each building
		cVector3d com(0, 0, 0);
		double north = m_nsewBuildingBounds[i][0];
		double south = m_nsewBuildingBounds[i][1];
		double east = m_nsewBuildingBounds[i][2];
		double west = m_nsewBuildingBounds[i][3];
		cVector3d nw(west, north, 0);
		cVector3d ne(east, north, 0);
		cVector3d sw(west, south, 0);
		cVector3d se(east, south, 0);
		com.add(nw);
		com.add(ne);
		com.add(sw);
		com.add(se);
		com.div(4);

		if (cDistance(com, m_playdoh->getCenterOfMass()) < shortest)
		{
			index = i;
			shortest = cDistance(com, m_playdoh->getCenterOfMass());
			buildingPos = com;
		}
	}
	m_closestBuildingIndex = index;
	m_closestBuildingPos = buildingPos;
	//cout << "closest building is: " << index << endl;
}