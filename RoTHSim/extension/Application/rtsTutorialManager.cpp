//---------------------------------------------------------------------------
#include "rtsTutorialManager.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
 Constructor of rtsTutorialManager.
 
 \fn     rtsTutorialManager::rtsTutorialManager()
 \param
 */

//===========================================================================
rtsTutorialManager::rtsTutorialManager(int a_windowW, int a_windowH, cCamera* a_camera, cToolCursor* a_tool, rtsClay* a_playdoh, vector<rtsPolygon*> a_buildings,
							int a_windowW2, int a_windowH2, cCamera* a_camera2, vector<rtsScope*> a_scopes, cPanel* a_hqPanel, cPanel* a_userInputPanel,
								communicator* a_com, rtsHotspotManager* a_hotspotManager, bool a_zUpOriented, bool a_rightHanded)
{
	// initialize member variables
	m_camera = a_camera;
	m_windowW = a_windowW;
	m_windowH = a_windowH;
	m_tool = a_tool;
	m_buildings = a_buildings;
	m_playdoh = a_playdoh;
	m_camera2 = a_camera2;
	m_windowW2 = a_windowW2;
	m_windowH2 = a_windowH2;
	m_scopes = a_scopes;
	m_hqPanel = a_hqPanel;
	m_userInputPanel = a_userInputPanel;
	m_com = a_com;
	m_hotspotManager = a_hotspotManager;
	m_zUpOriented = a_zUpOriented;
	m_rightHanded = a_rightHanded;

	m_pauseCom = false;
	m_cordonMode = false;
	m_demoHapticsEnabled = false;
	m_demoShowTargets = false;
	m_demoPlaydohContacted = false;
	m_desiredLocationReached = false;
	m_cordonLocation1Reached = false;
	m_cordonLocation1Append = false;
	m_desiredRadius = 10.0;
	m_displayForceScale = 100.0;

	// current index of instructions
	m_instructionIndex = 0;
	m_instruction2Index = 0;

	m_userResponse = false;
	m_userResponseMade = false;
	m_userResponseMade2 = false;
	m_responseTimer.setTimeoutPeriodSeconds(0.1);
	bool showDetections2 = true;

	m_testStartTime = 0; // seconds
	m_skippedTime = 0.0;

	m_lineWidth = 5.0;
	m_highlightColor = cColorf(1.0f, 0.2f, 0.2f);

	bool showStartTimes = false;

	//-----------------------------
	// create instructions
	//-----------------------------

	m_instructionStartTime = 2.0;
	m_minDur = 2.0;
	m_maxDur = 5.5;
	m_waitDur = 0.50;

	// pause server so team doesn't move at beginning of tutorial
	pauseServer();

	// turn off playdoh haptics at beginning of tutorial
	m_playdoh->setHapticEnabled(false, true);
	m_playdohHapticsEnabled = false;

	// keep ground surface active
	m_demoHapticsEnabled = true; 

	// Welcome -------------------------

		// Welcome message
		string welcomeString = "Welcome to RoTHSim, the robot team haptic simulator";
		double wStrStart = m_instructionStartTime;
		addInstruction_DefaultDur(welcomeString, 0.1);

	// Environment --------------------

		// entire window view
		double envStrStart1 = m_instructionStartTime;
		addInstruction_DefaultDur("This is the robot team's environment, shown in bird's-eye view");

			double vEnvStart = envStrStart1;
			double vEnvMin = m_minDur;
			double vEnvMax = m_maxDur + m_waitDur;
			addCameraVisual(m_camera, vEnvStart, vEnvMin, vEnvMax);

		// buildings
		double bldgStrStart = m_instructionStartTime;
		addInstruction_DefaultDur("Each environment has a set of buildings, which are colored white", 1.0);

			// visual for bldg 1
			addBldgVisual(bldgStrStart, 1, 1);

			// visual for bldg 2
			addBldgVisual(bldgStrStart, 1, 2);

		addInstruction_DefaultDur("The ground surface is shown in gray");

	// Team --------------------
		
		// team instruction 1
		double teamStrStart1 = m_instructionStartTime;
		addInstruction_DefaultDur("This is the team of robots you will be controlling");

		// team instruction 2
		vector<string> teamMessages1;
		teamMessages1.push_back("Each dot inside this circle represents a robot that is capable of moving");
		teamMessages1.push_back("in any direction along the ground surface");
		double teamStrStart2 = m_instructionStartTime;
		addInstruction_DefaultDur(teamMessages1, 2.0);

			double vTeamStart = teamStrStart1;
			double vTeamMin = 3.0 * m_minDur;
			double vTeamMax = 3.0 * m_maxDur + m_waitDur;
			addPlaydohVisual(vTeamStart, vTeamMin, vTeamMax);

	// Tool --------------------

		// tool instruction 1
		double toolStrStart1 = m_instructionStartTime;
		addInstruction_DefaultDur("This is the virtual haptic cursor");

		// tool instruction 3
		double toolStrStart3 = m_instructionStartTime;
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 2 * m_minDur, 2 * m_maxDur, 
						"Take hold of the device and practice moving the cursor", "deviceMove");
		//m_instructionStartTime += 2 * m_maxDur;

			double vToolStart = toolStrStart1;
			double vToolMin = 3.0;
			double vToolMax = 10.0;
			addVisual(m_tool->m_hapticPoint->m_sphereProxy, 1, vToolStart, vToolMin, vToolMax);

		// tool instruction 4
		addInstruction_DefaultDur("You should feel a virtual force when in contact with the ground surface.");

		vector<string> toolMessages5;
		toolMessages5.push_back("When you are in contact with the ground surface, the cursor will turn black");
		toolMessages5.push_back("and you should feel the virtual force.");
		double toolStrStart5 = m_instructionStartTime;
		addInstruction_DefaultDur(toolMessages5);

	// Force feedback and mode training ---------------------
		
		if (showStartTimes)
			cout << "mode training start: " << m_instructionStartTime << endl;
		// feedback intro
		vector<string> ffm0;
		ffm0.push_back("The following instructions will teach you the two team modes,");
		ffm0.push_back("travel and surround");
		double ffmStart0 = m_instructionStartTime;
		addInstruction_DefaultDur(ffm0);

		//addInstruction_DefaultDur("You will also learn what force feedback to expect in each mode.");

		// notice patrol mode
		vector<string> ffm1;
		ffm1.push_back("Notice that the team is near a building,");
		ffm1.push_back("but it hasn't finished making its approach.");
		double ffmStart1 = m_instructionStartTime;
		addInstruction_DefaultDur(ffm1);

		// patrol mode explanation
		vector<string> ffm2;
		ffm2.push_back("When the team moves between buildings,");
		ffm2.push_back("that is referred to as a \"travel\" ");
		double ffmStrStart2 = m_instructionStartTime;
		addInstruction_DefaultDur(ffm2);

		// patrol force feedback explanation
		vector<string> ffm3;
		ffm3.push_back("The haptic device is capable of providing force feedback");
		ffm3.push_back("to help identify the overall shape of the team");
		double ffmStrStart3 = m_instructionStartTime;
		addInstruction_DefaultDur(ffm3);

		// patrol force feedback attempt
		vector<string> ffm4;
		ffm4.push_back("Move the cursor along the ground surface and near the team");
		ffm4.push_back("to feel a shape close to the ground surface");
		double ffm4Start = m_instructionStartTime;
		double ffm4Min = 6 * m_minDur;
		double ffm4Max = 10 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, ffm4Min, ffm4Max, ffm4, "playdohContacted");

		// transition to cordon mode
		vector<string> ffm5;
		ffm5.push_back("Once the robots are near a building,");
		ffm5.push_back("the robots will surround the building");
		double ffm5Start = m_instructionStartTime;
		double ffm5Min = 2 * m_minDur;
		double ffm5Max = 10 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, ffm5Min, ffm5Max, ffm5, "cordonCreated");
			
		// cordon mode acheived
		vector<string> ffm6;
		ffm6.push_back("The team has completely surrounded the building.");
		//ffm6.push_back("This is referred to as a \"surround\"");
		double ffm6Start = m_instructionStartTime;
		addInstruction_DefaultDur(ffm6);

		// cordon description
		vector<string> ffm7;
		ffm7.push_back("The haptic device provides feedback to inform you of");
		ffm7.push_back("the surrounding shape");
		double ffm7Start = m_instructionStartTime;
		addInstruction_DefaultDur(ffm7);

		// cordon force feedback attempt
		vector<string> ffm8;
		ffm8.push_back("Move the cursor along the ground surface and near the team");
		ffm8.push_back("to feel a ring shape that represents the team formation");
		double ffm8Start = m_instructionStartTime;
		double ffm8Min = 6 * m_minDur;
		double ffm8Max = 10 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, ffm8Min, ffm8Max, ffm8, "playdohContacted");

		addInstruction_DefaultDur("Please direct your attention to the other screen", 0.5);

		// Sensor/Message window training --------------------
	
		// explain message panel
		if (showStartTimes)
			cout << "message panel training start: " << m_instructionStartTime << endl;

		// hq panel1
		double hqPStrStart1 = m_instructionStartTime;
		addInstruction2_DefaultDur("This black portion is the Message Panel");

		// hq panel2
		vector<string> hqPMessages;
		hqPMessages.push_back("Additional task instructions and messages");
		hqPMessages.push_back("will be displayed here");
		double hqPStrStart2 = m_instructionStartTime;
		addInstruction2_DefaultDur(hqPMessages, 2.0);

		// message window panel visual
		double v2PanelStart = hqPStrStart1;
		addPanelVisual2(m_hqPanel, v2PanelStart, 2.0 * m_minDur, 2.0 * m_maxDur);

		// user input panel
		vector<string> uiMessages;
		uiMessages.push_back("When prompted, you will type responses here");
		double uiStart = m_instructionStartTime;
		addInstruction2_DefaultDur(uiMessages, 1.0);

		// message window panel visual
		double vUIPanelStart = uiStart;
		addPanelVisual2(m_userInputPanel, vUIPanelStart, m_minDur, m_maxDur);

	// Patrol training --------------------

		// message message
		if (showStartTimes)
			cout << "Move the team to the other building: " << m_instructionStartTime << endl;
		double pcMStart = m_instructionStartTime;
		addInstruction_DefaultDur("You have received a message!");

		// button description
		vector<string> p3;
		p3.push_back("On the device handle, there are four buttons.");
		p3.push_back("The TRAVEL button is the left button");
		double p3Start = m_instructionStartTime;
		addInstruction_DefaultDur(p3, 1.5);

			//addPictureVisual(m_camera->m_frontLayer, "../../bin/resources/images/falconHandle.bmp", p3Start, m_minDur, 4*m_maxDur);

		vector<string> p4;
		p4.push_back("To move the team to another building, first move the cursor near the team,");
		p4.push_back("press-and-hold the TRAVEL button and drag to the other building");
		double p4Start = m_instructionStartTime;
		double p4Min = 0 * m_minDur;
		double p4Max = 4 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, p4Start, p4Min, p4Max, p4, "commandPatrolButtonDown");

		addInstruction_DefaultDur("The device will slightly resist your motion, to help you sense the travel direction.");

		vector<string> p5;
		p5.push_back("When you've reached the other building, release the button");
		double p5Start = m_instructionStartTime;
		double p5Min = 0 * m_minDur;
		double p5Max = 4 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, p5Start, p5Min, p5Max, p5, "commandPatrolButtonUp");


		// Remember, you can feel the overall shape of the team while in a patrol

		vector<string> p6;
		p6.push_back("You have moved the team to another building!");
		p6.push_back("If the team has separated or is too far away from the desired building,");
		p6.push_back("redo the travel message steps to regroup the robots.");
		double p6Start = m_instructionStartTime;
		addInstruction(m_camera->m_frontLayer, p6Start, 5 * m_minDur, 5 * m_maxDur, p6, "desiredTeamLocation1");
		m_desiredLocation1 = m_bldg1Pos;

	// Cordon manipulation training --------------------

	// Detection training on 2 Window

		addInstruction_DefaultDur("Your main goal is to search for and find hotspots near the buildings", 1.5);

		double detection1 = m_instructionStartTime;
		addInstruction_DefaultDur("Each robot has an onboard sensor that can HELP YOU find hotspots", 1.5);

		if (showDetections2)
		{

			// detections hq
			vector<string> d4;
			d4.push_back("This area will show the surrounded building,");
			d4.push_back("with sensor readings from each individual robot.");
			double detection4 = m_instructionStartTime;
			addInstruction2_DefaultDur(d4, 2.0);

			cVector3d detectionsPos(0, 0, 0);
			if (!m_rightHanded)
			{
				detectionsPos = cVector3d(340, 0, 0); // hcval
			}

			rtsTutorialInstruction* detectionsVisual = new rtsBoundingBox(1000, 625, m_lineWidth, m_highlightColor, detection4, m_minDur, 2*m_maxDur, detectionsPos); // hcval

			detectionsVisual->setShowEnabled(false);
			m_camera2->m_frontLayer->addChild(detectionsVisual);
			m_visuals.push_back(detectionsVisual);

			// explain sensor readings in plain terms
			vector<string> d5;
			d5.push_back("Every time a sensor reading is shown,");
			d5.push_back("the corresponding robot has detected something at that location.");
			addInstruction2_DefaultDur(d5, 2.0);

			vector<string> d6;
			d6.push_back("Due to imperfections in the robot sensors,");
			d6.push_back("readings are NOT 100% accurate");
			addInstruction2_DefaultDur(d6, 2.0);

			vector<string> d7;
			d7.push_back("Currently, there are no hotspots present,");
			d7.push_back("but incorrect sensor readings are still appearing.");
			addInstruction2_DefaultDur(d7, 2.0);

			vector<string> d8;
			d8.push_back("You need to ignore these sensor readings until");
			d8.push_back("an accurate sensor reading is present.");
			addInstruction2_DefaultDur(d8, 2.0);

			if (showStartTimes)
				cout << "Example of hotspots present with dummy hotspot: " << m_instructionStartTime << endl;

			vector<string> d9;
			d9.push_back("An accurate sensor reading flickers faster or remains lit,");
			d9.push_back("because there is a hotspot nearby that robot");
			addInstruction2_DefaultDur(d9, 2.0);
		}
		else
		{

			// detection direction visual
			vector<string> d4;
			d4.push_back("Sections of this circle will light up when");
			d4.push_back("robots in that region have detected hotspots");
			double detection4 = m_instructionStartTime;
			addInstruction2_DefaultDur(d4, 2.0);

			addCircleVisual(m_camera2->m_frontLayer, detection4, m_minDur, 2.0*m_maxDur, cVector3d(500, 180, 0), 160.0, 3.0);

		}

		// scope 1
		vector<string> scopesMessages1;
		scopesMessages1.push_back("These plots show how many sensor readings are in the");
		scopesMessages1.push_back("cardinal (NSEW) directions around the building");
		double scopeStrStart1 = m_instructionStartTime;
		addInstruction2_DefaultDur(scopesMessages1, 2.0);

		// scopes visuals
		int numScopes = m_scopes.size();
		for (int i = 0; i < numScopes; i++)
		{
			double vScopeStart = scopeStrStart1;
			double vScopeMin = 2.0 * m_minDur;
			double vScopeMax = 2.0 * m_maxDur;
			addPanelVisual2(m_scopes[i], vScopeStart, vScopeMin, vScopeMax);
		}

		// scopes 2
		vector<string> scopesMessages2;
		scopesMessages2.push_back("Each plot corresponds to a region");
		scopesMessages2.push_back("around the surrounded building");
		double scopeStrStart2 = m_instructionStartTime;
		addInstruction2_DefaultDur(scopesMessages2, 2.0);

		// scopes individual visuals
		vector<double> scopeStarts;
		for (int i = 0; i < numScopes; i++)
		{
			double vScopeStart = scopeStrStart2 + i * 0.5*m_maxDur;
			scopeStarts.push_back(vScopeStart);
			addPanelVisual2(m_scopes[i], vScopeStart, 0.5*m_minDur, 0.5*m_maxDur);
		}

		// visual for bldg 2 North
		addBldgVisual(scopeStarts[0], 0.5, 1, 1);

		// visual for bldg 2 South
		addBldgVisual(scopeStarts[1], 0.5, 1, 2);

		// visual for bldg 2 East
		addBldgVisual(scopeStarts[2], 0.5, 1, 3);

		// visual for bldg 2 West
		addBldgVisual(scopeStarts[3], 0.5, 1, 4);

		// feedback arrow visual
		//addToolArrowVisual(ffm4Start, 10.0, recapStart - ffm4Start);

		// cordon intro
		vector<string> c1;
		c1.push_back("While surrounding the building, you need to move the team shape toward");
		c1.push_back("and away from the building in order to find these hotspots");
		double c1Start = m_instructionStartTime;
		addInstruction_DefaultDur(c1, 2.0);

		vector<string> c2;
		c2.push_back("The idea is to move the robots so that they are");
		c2.push_back("sitting on top of hotspots");
		double c2Start = m_instructionStartTime;
		addInstruction_DefaultDur(c2, 2.0);

		// move the cursor to center of team
		double c4Start = m_instructionStartTime;
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, m_minDur, m_maxDur, "Move the cursor to the center of the formation");

			double vc4Start = c4Start;
			double vc4Min = 3.0;
			double vc4Max = 4.0;
			addCircleVisual(m_camera->getParent(), vc4Start, vc4Min, vc4Max, m_bldg1Pos, 8.0, 1.0, "cordonLocation1");
			m_visuals.back()->setStartCondition(true);

		// buttons picture

		// button description
		vector<string> c5;
		c5.push_back("Look for the middle button on the device handle");
		c5.push_back("This is the SHAPE button");
		double c5Start = m_instructionStartTime;
		addInstruction_DefaultDur(c5, 1.5);

		vector<string> c6;
		c6.push_back("To adjust the shape of the team, get close to the ground surface,");
		c6.push_back("press-and-hold the SHAPE button and stretch the team away from the building");
		double c6Start = m_instructionStartTime;
		double c6Min = 3 * m_minDur;
		double c6Max = 5 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, c6Start, c6Min, c6Max, c6, "manipButtonDown");

		vector<string> c7;
		//c7.push_back("Press-and-hold the SHAPE button and push a section away from the building");
		c7.push_back("Once you have moved the team, release the button");
		double c7Start = m_instructionStartTime;
		double c7Min = 1 * m_minDur;
		double c7Max = 4 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, c7Start, c7Min, c7Max, c7, "manipButtonUp");

		vector<string> c71;
		c71.push_back("Now move the cursor along the outside the team perimeter,");
		c71.push_back("press-and-hold the SHAPE button and push the team toward the building");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 3 * m_minDur, 5 * m_maxDur, c71, "manipButtonDown");

		vector<string> c72;
		c72.push_back("Again, release the button once you have moved the team");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, m_minDur, 4 * m_maxDur, c72, "manipButtonUp");

		addInstruction_DefaultDur("Practice searching close to and 25m away from the building", 3.0);

		// manipulation reset
		vector<string> c8;
		c8.push_back("You can reset the team shape by double-clicking the SHAPE button");
		c8.push_back("Try this now.");
		double c8Start = m_instructionStartTime;
		double c8Min = 1 * m_minDur;
		double c8Max = 5 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, c8Start, c8Min, c8Max, c8, "manipButtonDoubleClick");

	// Cordon spacing manipulation training --------------------

		// buttons picture

		vector<string> csi1;
		csi1.push_back("You can also change the team spacing while keeping");
		csi1.push_back("the surrounding shape the same");
		double csi1Start = m_instructionStartTime;
		addInstruction_DefaultDur(csi1, 2.0);

		vector<string> csi2;
		csi2.push_back("To help you find hotspots, you can shift the spacing");
		csi2.push_back("of the team to place more coverage toward a location around the building.");
		double csi2Start = m_instructionStartTime;
		addInstruction_DefaultDur(csi2, 2.0);

		// move the cursor to center of team
		double cs1Start = m_instructionStartTime;
		addInstruction_DefaultDur("To do this, move the cursor once again to the center of the team");

			double vcs1Start = cs1Start;
			double vcs1Min = 3.0;
			double vcs1Max = 4.0;
			addCircleVisual(m_camera->getParent(), vcs1Start, vcs1Min, vcs1Max, m_bldg1Pos, 8.0, 1.0, "cordonLocation1");
			m_visuals.back()->setStartCondition(true);

		// button description
		vector<string> cs2;
		cs2.push_back("Look for the right button on the device handle");
		cs2.push_back("This is the SPACING button");
		double cs2Start = m_instructionStartTime;
		addInstruction_DefaultDur(cs2, 1.5);

		vector<string> cs3;
		cs3.push_back("To adjust the spacing of the team,");
		cs3.push_back("press-and-hold the SPACING button and pull to one side of the team formation.");
		double cs3Start = m_instructionStartTime;
		double cs3Min = 3 * m_minDur;
		double cs3Max = 5 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, cs3Start, cs3Min, cs3Max, cs3, "manipSpacingButtonDown");

		vector<string> cs4;
		cs4.push_back("To see a more noticeable change, pull until the cursor is outside the team perimeter.");
		cs4.push_back("Once you have adjusted the team spacing, release the button");
		double cs4Start = m_instructionStartTime;
		double cs4Min = 1 * m_minDur;
		double cs4Max = 5 * m_maxDur;
		addInstruction(m_camera->m_frontLayer, cs4Start, cs4Min, cs4Max, cs4, "manipSpacingButtonUp");

		// feedback
		vector<string> csf;
		csf.push_back("As you change SPACING, you will feel a virtual force toward");
		csf.push_back("the center of the team.");
		addInstruction_DefaultDur(csf, 2.0);

		// spacing independant of shape
		addInstruction_DefaultDur("Notice how changes in team spacing are independent of changes in team shape");

		addInstruction_DefaultDur("Practice placing more coverage to various sides of the building with this method", 3.0);
		addInstruction_DefaultDur("You can use SHAPE and SPACING to find hotspots near the building");

		// hotspots training
		if (showStartTimes)
			cout << "hotspots detections training start: " << m_instructionStartTime << endl;

		// show hidden hotspots
		vector<string> h1;
		h1.push_back("Hotspots will normally be hidden around the buildings");
		h1.push_back("To help you understand what you will be looking for, they are shown now.");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 2 * m_minDur, 2 * m_maxDur, h1, "showTargets");

		if (showStartTimes)
			cout << "showing first individual hotspots, unhidden: " << m_instructionStartTime << endl;

		addInstruction_DefaultDur("Hotspots not found are red squares. Once they are found, they turn green.");
		addInstruction_DefaultDur("Adjust the SHAPE of the team to search the area where hotspots are located.");
		addInstruction_DefaultDur("You may also use SPACING to place more coverage near the hotspots.");
		addInstruction_DefaultDur("Practice finding hotspots as they become available.", 4.0);
		addInstruction_DefaultDur("So far, only individual hotspots have been active at a given time.");


		vector<string> hs3;
		hs3.push_back("Many hotspots are hidden in groups. To successfully find these hotspots,");
		hs3.push_back("you must find each hotspot, in that group, at the same time.");
		double hotspots3Start = m_instructionStartTime;
		addInstruction_DefaultDur(hs3, 2.0);

		if (showStartTimes)
			cout << "group of two hotspots(shown) start: " << m_instructionStartTime << endl;

		vector<string> hs4;
		hs4.push_back("Notice the group of two hotspots hidden near the building.");
		hs4.push_back("Adjust the SHAPE and/or SPACING of the team to find these hotspots.");
		hs4.push_back("If only one is detected, you need to change SHAPE and/or SPACING until both are found.");
		double hotspots4Start = m_instructionStartTime;
		addInstruction_DefaultDur(hs4, 4.0);

		if (showStartTimes)
			cout << "group of three hotspots(shown) start: " << m_instructionStartTime << endl;

		vector<string> h5;
		h5.push_back("Hotspots are hidden in groups of three at most.");
		h5.push_back("Practice finding this group of three hotspots.");
		hs4.push_back("Again, you need to change SHAPE and/or SPACING until all three are found.");
		addInstruction_DefaultDur(h5, 4.0);

		vector<string> hs2;
		hs2.push_back("Hotspots are only available for a limited amount of time.");
		hs2.push_back("You must act quickly to detect and find them.");
		double hotspots2Start = m_instructionStartTime;
		addInstruction_DefaultDur(hs2, 2.0);
		
		vector<string> h6;
		h6.push_back("These hotspot groups can be hidden anywhere around the building.");
		h6.push_back("You must rely on the sensor readings to identify areas to search further.");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 2 * m_minDur, 2 * m_maxDur, h6, "hideTargets");

		vector<string> h7;
		h7.push_back("Each group of hotspots expires after a certain amount of time.");
		h7.push_back("From now on, when hotspots expire, they flash red and fade away.");
		addInstruction2_DefaultDur(h7, 2.0);

		vector<string> h8;
		h8.push_back("In other words, from now on, if you see a red hotspot appear,");
		h8.push_back("that means that you were too late.");
		addInstruction2_DefaultDur(h8, 2.0);

		if (showStartTimes)
			cout << "hotspots (hidden)  start: " << m_instructionStartTime << endl;

		vector<string> h9;
		h9.push_back("As mentioned, the hotspots will be hidden and not visible until found.");
		h9.push_back("Practice finding hotspots by relying on the team's sensor readings.");
		addInstruction_DefaultDur(h9, 20.0);
		
		if (showStartTimes)
			cout << "hotspots (hidden) end: " << m_instructionStartTime << endl;

		addInstruction2_DefaultDur("It may be challenging to find some hotspots, which is expected.");

		vector<string> h10;
		double h10Start = m_instructionStartTime;
		h10.push_back("Your primary (most important) task:");
		h10.push_back("Continously search for and find as many hotspots as possible with the team.");
		addInstruction2_DefaultDur(h10, 3.0);

		addInstruction2_DefaultDur("All hotspots will be located within 0m to 40m from the building walls.");

		addInstruction2_DefaultDur("Please direct your attention to the other screen.");

		// hotspots activity
		vector<string> par1;
		par1.push_back("While detecting hotspots, you need to take note of");
		par1.push_back("the past activity in each region.");
		double par1Start = m_instructionStartTime;
		addInstruction_DefaultDur(par1, 2.0);

		addInstruction_DefaultDur("This will allow you build a mental model of hotspots found or not found");

		vector<string> par2;
		par2.push_back("Throughout the simulation, you will be asked several questions");
		par2.push_back("that test your mental model of hotspots found or not found");
		double par2Start = m_instructionStartTime;
		addInstruction_DefaultDur(par2, 2.0);

		vector<string> sa2;
		sa2.push_back("Some questions will ask you how many hotspots have been found.");
		sa2.push_back("Remember, found hotspots have been identified in green.");
		addInstruction_DefaultDur(sa2, 2.0);

		if (showStartTimes)
			cout << "how many hotspots question time: " << m_instructionStartTime << endl;
		addInstruction_DefaultDur("Respond to the new question by entering a number with the keypad", 2.0);

		vector<string> sa3;
		sa3.push_back("Other questions will ask you to report what percent of hotspots");
		sa3.push_back("have been found in a region around a building");
		addInstruction_DefaultDur(sa3, 2.0);

		if (showStartTimes)
			cout << "percent hotspots question time: " << m_instructionStartTime << endl;
		addInstruction_DefaultDur("Respond to the new question by entering a percentage (0-100) with the keypad", 2.0);

		vector<string> sa4;
		sa4.push_back("Other questions will ask you to report a location in the environment");
		sa4.push_back("where hotspots are most likely to be located in the future.");
		addInstruction_DefaultDur(sa4, 2.0);

		vector<string> sa5;
		sa5.push_back("For this type of question, you must consider both hotspots found AND not found,");
		sa5.push_back("in order to correctly predict a likelihood of activity in a region.");
		addInstruction_DefaultDur(sa5, 2.5);


		if (showStartTimes)
			cout << "where hotspots likely question time: " << m_instructionStartTime << endl;
		vector<string> sa6;
		sa6.push_back("Respond to the new question by moving the cursor to a location in the environment");
		sa6.push_back("and pressing ENTER without typing anything into the keypad.");
		addInstruction_DefaultDur(sa6, 2.0);


	// w/out haptics --------------------

		vector<string> wh1;
		wh1.push_back("Some environments will require you to perform the same actions");
		wh1.push_back("without haptic feedback enabled");
		addInstruction_DefaultDur(wh1);

		vector<string> wh2;
		wh2.push_back("The following instructions will help you to familiarize yourself");
		wh2.push_back("with the controls without force feedback");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 2 * m_minDur, 2 * m_maxDur, wh2, "disableHaptics");

		if (showStartTimes)
			cout << "move to building 2 message: " << m_instructionStartTime << endl;

		vector<string> wh3;
		wh3.push_back("Move the team to the other building");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 4*m_minDur, 4*m_maxDur, wh3, "commandPatrolButtonUp");

		// other messages
		vector<string> com1;
		com1.push_back("In addition to travel messages and questions to answer,");
		com1.push_back("other messages are given to help you find hotspots.");
		addInstruction_DefaultDur(com1, 2.0);

		if (showStartTimes)
			cout << "search message to find two hotspots to the EAST: " << m_instructionStartTime << endl;

		vector<string> com2;
		com2.push_back("Respond by adjusting the SHAPE of the team in that direction,");
		com2.push_back("and finding two hotspots before they expire.");
		addInstruction_DefaultDur(com2, 3.0);
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 5 * m_minDur, 6 * m_maxDur, com2, "manipButtonUp");

		if (showStartTimes)
			cout << "coverage message to the NORTH: " << m_instructionStartTime << endl;

		string com3 = "Respond by adjusting the SPACING of the team in that direction.";
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, 2 * m_minDur, 4 * m_maxDur, com3, "manipSpacingButtonUp");

		if (showStartTimes)
			cout << "coverage message to the NORTH end: " << m_instructionStartTime << endl;

		vector<string> wh6;
		wh6.push_back("The controls are exactly the same without force feedback.");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, m_minDur, m_maxDur, wh6);

		vector<string> td12;
		td12.push_back("Following messages is mandatory to completing the mission");
		td12.push_back("and doing so will increase your score");
		addInstruction_DefaultDur(td12, 3.0);

		vector<string> td13;
		td13.push_back("You must always follow relocation messages");
		td13.push_back("that require you to move the team to another building");
		addInstruction_DefaultDur(td13, 3.0);

		vector<string> td2;
		double td2Start = m_instructionStartTime;
		td2.push_back("This leads to your secondary task:");
		td2.push_back("Respond to and follow incoming messages.");
		addInstruction_DefaultDur(td2, 3.0);

		addInstruction_DefaultDur("Try your best to quickly respond to and follow these messages.");

		if (showStartTimes)
			cout << "hotspots (hidden) no haptics start: " << m_instructionStartTime << endl;

		vector<string> wh61;
		wh61.push_back("Practice finding hotspots by relying on the team's sensor readings.");
		addInstruction_DefaultDur(wh61, 20.0);

		if (showStartTimes)
			cout << "hotspots (hidden) no haptics end: " << m_instructionStartTime << endl;

		vector<string> wh7;
		wh7.push_back("You will be notified prior to starting whether haptic feedback will be enabled or disabled.");
		addInstruction_DefaultDur(wh7, 1.5);

		vector<string> wh8;
		wh8.push_back("You will have 6 minutes to complete each mission,");
		wh8.push_back("so try your best to control the team and respond to messages as quickly as possible.");
		addInstruction_DefaultDur(wh8, 2.0);

		vector<string> td3;
		td3.push_back("To review, these are your primary and secondary tasks:");
		td3.push_back(" 1. Continously search for and find as many hotspots as possible.");
		td3.push_back(" 2. Respond to and follow incoming messages.");
		addInstruction(m_camera->m_frontLayer, m_instructionStartTime, m_minDur, 2 * m_maxDur, td3);

		// end of instructions
		addInstruction_DefaultDur("End of Tutorial. Press ESC to exit");

		// end of instructions
		addInstruction2_DefaultDur("End of Tutorial. Press ESC to exit");
}

rtsTutorialManager::~rtsTutorialManager()
{
    // deallocate memory
    deallocate();
}

void rtsTutorialManager::addInstruction_DefaultDur(string a_messageText, double a_multiplier)
{
	addInstruction(m_camera->m_frontLayer, m_instructionStartTime, a_multiplier*m_minDur, a_multiplier*m_maxDur, a_messageText);
	//m_instructionStartTime += a_multiplier*m_maxDur + m_waitDur;
}

void rtsTutorialManager::addInstruction_DefaultDur(vector<string> a_messages, double a_multiplier)
{
	addInstruction(m_camera->m_frontLayer, m_instructionStartTime, a_multiplier*m_minDur, a_multiplier*m_maxDur, a_messages);
	//m_instructionStartTime += a_multiplier*m_maxDur + m_waitDur;
}


void rtsTutorialManager::addInstruction(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, string a_messageText, 
										string a_responseType, int a_negVertOffset, double a_fadeDuration)
{
	rtsTutorialInstruction* instruction = new rtsTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_messageText, m_windowW, m_windowH, 
																a_responseType, a_negVertOffset, a_fadeDuration);
	a_parentObject->addChild(instruction);
	instruction->setShowEnabled(false, true);
	m_instructions.push_back(instruction);

	// if first instruction, show it
	if (m_instructions.size() == 1)
		instruction->setShowEnabled(true, true);

	// increment instruction start time for next instruction
	m_instructionStartTime += a_maxDuration + m_waitDur;
}

void rtsTutorialManager::addInstruction(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, vector<string> a_messages,
										string a_responseType, int a_negVertOffset, double a_fadeDuration)
{
	rtsTutorialInstruction* instruction = new rtsTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_messages, m_windowW, m_windowH,
		a_responseType, a_negVertOffset, a_fadeDuration);
	a_parentObject->addChild(instruction);
	instruction->setShowEnabled(false, true);
	m_instructions.push_back(instruction);

	// if first instruction, show it
	if (m_instructions.size() == 1)
		instruction->setShowEnabled(true, true);

	// increment instruction start time for next instruction
	m_instructionStartTime += a_maxDuration + m_waitDur;
}

void rtsTutorialManager::addInstruction2(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, string a_messageText,
										string a_responseType, int a_negVertOffset, double a_fadeDuration)
{
	double vSpacing = 10;
	double remainingH2 = m_windowH2 - vSpacing * 2;
	double remainingW2 = m_windowW2 - m_hqPanel->getWidth();
	


	rtsTutorialInstruction* instruction = new rtsTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_messageText, remainingW2, remainingH2,
																a_responseType, a_negVertOffset, a_fadeDuration);
	a_parentObject->addChild(instruction);

	instruction->setShowEnabled(false, true);
	m_instructions2.push_back(instruction);

	// increment instruction start time for next instruction
	m_instructionStartTime += a_maxDuration + m_waitDur;
}

void rtsTutorialManager::addInstruction2(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration, vector<string> a_messages,
										string a_responseType, int a_negVertOffset, double a_fadeDuration)
{
	double vSpacing = 10;
	double remainingH2 = m_windowH2 - vSpacing * 2;
	double remainingW2 = m_windowW2 - m_hqPanel->getWidth();

	rtsTutorialInstruction* instruction = new rtsTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_messages, remainingW2, remainingH2,
		a_responseType, a_negVertOffset, a_fadeDuration);
	a_parentObject->addChild(instruction);
	instruction->setShowEnabled(false, true);
	m_instructions2.push_back(instruction);

	// increment instruction start time for next instruction
	m_instructionStartTime += a_maxDuration + m_waitDur;
}

void rtsTutorialManager::addInstruction2_DefaultDur(string a_messageText, double a_multiplier)
{
	addInstruction2(m_camera2->m_frontLayer, m_instructionStartTime, a_multiplier*m_minDur, a_multiplier*m_maxDur, a_messageText);
	//m_instructionStartTime += a_multiplier*m_maxDur + m_waitDur;
}

void rtsTutorialManager::addInstruction2_DefaultDur(vector<string> a_messages, double a_multiplier)
{
	addInstruction2(m_camera2->m_frontLayer, m_instructionStartTime, a_multiplier*m_minDur, a_multiplier*m_maxDur, a_messages);
	//m_instructionStartTime += a_multiplier*m_maxDur + m_waitDur;
}

void rtsTutorialManager::addBldg1Visual(double a_startTime)
{
	cVector3d com1 = m_buildings[1]->getCenterOfMass();
	m_bldg1Pos = com1;
	cVector3d comToBlc1(-27.5, -7.5, 0);
	double width1 = 55.0;
	double height1 = 15.0;
	rtsTutorialInstruction* vBldg1 = new rtsBoundingBox(width1, height1, 0.5*m_lineWidth, m_highlightColor, a_startTime, m_minDur, m_maxDur,
		(com1 + comToBlc1) / 2, "", 1.0, 4.5);
	vBldg1->setShowEnabled(false, true);
	m_buildings[1]->addChild(vBldg1);
	m_visuals.push_back(vBldg1);
}

void rtsTutorialManager::addBldgVisual(double a_startTime, double a_durFactor, int a_bldg, int a_direction)
{
	cVector3d com;
	double width;
	double height;
	if (a_bldg == 1)
	{
		com = m_buildings[0]->getCenterOfMass();
		m_bldg1Pos = com;
		width = 55.0;
		height = 15.0;
	}
	else
	{
		com = m_buildings[1]->getCenterOfMass();
		m_bldg2Pos = com;
		width = 25.0;
		height = 40.0;
	}

	cVector3d comToBlc2(-width/2, -height/2, 0);
	cVector3d boxLoc = (com + comToBlc2) / 2;
	rtsTutorialInstruction* vBldg;

	double regionW = 30.0;
	double regionL;

	double minDur = a_durFactor* m_minDur;
	double maxDur = a_durFactor* m_maxDur;

	cVector3d loc(0, 0, 0);
	switch (a_direction)
	{
	case 0:
		vBldg = new rtsBoundingBox(width, height, 0.5*m_lineWidth, m_highlightColor, a_startTime, minDur, maxDur,
			boxLoc, "", 1.0, 4.5);
		break;
	case 1:
		// north box
		regionL = width + regionW * 2;
		loc = (boxLoc * 2 + cVector3d(-regionW, height, 0)) / 2;
		vBldg = new rtsBoundingBox(regionL, regionW, 0.5*m_lineWidth, m_highlightColor, a_startTime, minDur, maxDur,
			loc, "", 1.0, 4.5);
		break;
	case 2:
		// south box
		regionL = width + regionW * 2;
		loc = (boxLoc*2 + cVector3d(-regionW, -regionW, 0)) / 2;
		vBldg = new rtsBoundingBox(regionL, regionW, 0.5*m_lineWidth, m_highlightColor, a_startTime, minDur, maxDur,
			loc, "", 1.0, 4.5);
		break;
	case 3:
		// east box
		regionL = height + regionW * 2;
		loc = (boxLoc*2 + cVector3d(width, -regionW, 0)) / 2;
		vBldg = new rtsBoundingBox(regionW, regionL, 0.5*m_lineWidth, m_highlightColor, a_startTime, minDur, maxDur,
			loc, "", 1.0, 4.5);
		break;
	case 4:
		// west box
		regionL = height + regionW * 2;
		loc = (boxLoc*2 + cVector3d(-regionW, -regionW, 0))/2;
		vBldg = new rtsBoundingBox(regionW, regionL, 0.5*m_lineWidth, m_highlightColor, a_startTime, minDur, maxDur,
			loc, "", 1.0, 4.5);
		break;
	}

	vBldg->setShowEnabled(false, true);
	if (a_bldg == 1)
		m_buildings[0]->addChild(vBldg);
	else m_buildings[1]->addChild(vBldg);
	m_visuals.push_back(vBldg);
}

void rtsTutorialManager::addVisual(cGenericObject* a_parentObject, int a_visualType, double a_startTime, double a_minDuration, double a_maxDuration,
								cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	rtsTutorialInstruction* visual;

	if (a_visualType == 0)
	{
		cVector3d cen = a_parentObject->getBoundaryCenter();
		cVector3d max = a_parentObject->getBoundaryMax();
		double width = 1.0 * cDistance(cen, max);
		double height = width;
		visual = new rtsBoundingBox(width, height, m_lineWidth, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	}
	else if (a_visualType == 1)
	{
		cVector3d cen = a_parentObject->getBoundaryCenter();
		cVector3d max = a_parentObject->getBoundaryMax();
		double outer = 0.75 * cDistance(cen, max);
		double inner = 0.05 * outer;
		cMatrix3d rot;
		rot.identity();
		if (m_zUpOriented)
			rot.rotateAboutLocalAxisDeg(cVector3d(0, 0, 1), 90);
		else rot.rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);

		visual = new rtsBoundingCircle(inner, outer, 32, 32, rot, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	}
	else if (a_visualType == 2)
	{
		visual = new rtsArrow(3.0, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	}

	visual->setShowEnabled(false, true);
	a_parentObject->addChild(visual);

	m_visuals.push_back(visual);
}


void rtsTutorialManager::addCircleVisual(cGenericObject* a_parentObject, double a_startTime, double a_minDuration, double a_maxDuration,
										cVector3d a_localPos, double a_circleRadius, double a_width, string a_responseType, double a_fadeDuration)
{
	double outer = a_circleRadius;
	double inner = a_width;
	cMatrix3d rot;
	rot.identity();
	//if (m_zUpOriented)
	//	rot.rotateAboutLocalAxisDeg(cVector3d(0, 0, 1), 90);
	//else rot.rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);

	rtsTutorialInstruction* visual = new rtsBoundingCircle(inner, outer, 32, 32, rot, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);

	visual->setShowEnabled(false, true);
	a_parentObject->addChild(visual);

	m_visuals.push_back(visual);

}

void rtsTutorialManager::addToolArrowVisual(double a_startTime, double a_minDuration, double a_maxDuration,
											cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{

	rtsArrow* toolArrow = new rtsArrow(10.0, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	m_toolArrows.push_back(toolArrow);
	m_tool->m_hapticPoint->m_sphereProxy->addChild(toolArrow);
}

void rtsTutorialManager::addPlaydohVisual(double a_startTime, double a_minDuration, double a_maxDuration,
										cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	cVector3d cen = m_playdoh->getCenterOfMass();
	double outer = 20.0;
	double inner = 0.05 * outer;
	cMatrix3d rot;
	rot.identity();
	//if (m_zUpOriented)
	//	rot.rotateAboutLocalAxisDeg(cVector3d(0, 0, 1), 90);
	//else rot.rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);

	rtsTutorialInstruction* visual = new rtsBoundingCircle(inner, outer, 32, 32, rot, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, cen, a_responseType, a_fadeDuration);

	visual->setShowEnabled(false, true);
	m_camera->getParent()->addChild(visual);

	m_visuals.push_back(visual);
}

void rtsTutorialManager::addCameraVisual(cCamera* a_parentCamera, double a_startTime, double a_minDuration, double a_maxDuration,
									cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	rtsTutorialInstruction* visual = new rtsBoundingBox(m_windowW, m_windowH, m_lineWidth, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	visual->setResizeByCamera(true);
	visual->setShowEnabled(false, true);
	a_parentCamera->m_frontLayer->addChild(visual);
	m_visuals.push_back(visual);
}

void rtsTutorialManager::addCameraVisual2(cCamera* a_parentCamera, double a_startTime, double a_minDuration, double a_maxDuration,
										cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	rtsTutorialInstruction* visual = new rtsBoundingBox(m_windowW, m_windowH, m_lineWidth, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	visual->setResizeByCamera(true);
	visual->setShowEnabled(false, true);
	a_parentCamera->m_frontLayer->addChild(visual);
	m_visuals2.push_back(visual);
}

void rtsTutorialManager::addVisual2(cGenericObject* a_parentObject, int a_visualType, double a_startTime, double a_minDuration, double a_maxDuration,
	cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	rtsTutorialInstruction* visual;
	//visual = new rtsVisualInstruction(a_visualType, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_fadeDuration);
	//a_parentObject->addChild(visual);
	//m_visuals.push_back(visual);
	if (a_visualType == 0)
	{
		cVector3d cen = a_parentObject->getBoundaryCenter();
		cVector3d max = a_parentObject->getBoundaryMax();
		double width = 1.0 * cDistance(cen, max);
		double height = width;
		visual = new rtsBoundingBox(width, height, m_lineWidth, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	}
	else if (a_visualType == 1)
	{
		cVector3d cen = a_parentObject->getBoundaryCenter();
		cVector3d max = a_parentObject->getBoundaryMax();
		double outer = 0.85 * cDistance(cen, max);
		double inner = 0.10 * outer;
		cMatrix3d rot;
		rot.identity();
		rot.rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);
		visual = new rtsBoundingCircle(inner, outer, 32, 32, rot, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
		a_parentObject->addChild(visual);
	}
	else if (a_visualType == 2)
	{

	}
	visual->setShowEnabled(false, true);
m_visuals2.push_back(visual);
}

void rtsTutorialManager::addPanelVisual2(cPanel* a_parentPanel, double a_startTime, double a_minDuration, double a_maxDuration,
	cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	rtsTutorialInstruction* visual;

	double width = 1.0 * a_parentPanel->getWidth();
	double height = 1.0 * a_parentPanel->getHeight();
	if (a_parentPanel == m_userInputPanel)
		width = 670; // hcval hack fix
	visual = new rtsBoundingBox(width, height, m_lineWidth, m_highlightColor, a_startTime, a_minDuration, a_maxDuration, a_localPos, a_responseType, a_fadeDuration);
	visual->setResizeByCamera(true);
	visual->setShowEnabled(false, true);
	a_parentPanel->addChild(visual);

	//cShapeSphere* sphere = new cShapeSphere(30);
	//sphere->setLocalPos(0, 0, 0);
	//sphere->setShowEnabled(true);
	//a_parentPanel->addChild(sphere);

	m_visuals2.push_back(visual);
}

void rtsTutorialManager::addPictureVisual(cGenericObject* a_parentObject, string a_filename, double a_startTime, double a_minDuration, double a_maxDuration,
	string a_responseType, double a_fadeDuration)
{
	rtsTutorialInstruction* visual;
	visual = new rtsPictureInstruction(a_filename, a_startTime, a_minDuration, a_maxDuration, m_windowW, m_windowH, a_responseType, a_fadeDuration);
	a_parentObject->addChild(visual);
	visual->setShowEnabled(false, true);
	m_visuals.push_back(visual);
}

void rtsTutorialManager::pauseServer()
{
	// only pause when server is running
	if (!m_pauseCom)
	{
		m_com->pause();
		m_pauseCom = true;
	}
}

void rtsTutorialManager::unpauseServer()
{
	// only unpause when server is paused
	if (m_pauseCom)
	{
		m_com->unpause();
		m_pauseCom = false;
	}
}

void rtsTutorialManager::shiftVisualsBack(double a_shiftTime)
{
	int numVisuals = m_visuals.size();
	for (int i = 0; i < numVisuals; i++)
	{
		m_visuals[i]->setStartTime(m_visuals[i]->getStartTime() + a_shiftTime);
	}

	int numVisuals2 = m_visuals2.size();
	for (int i = 0; i < numVisuals2; i++)
	{
		m_visuals2[i]->setStartTime(m_visuals2[i]->getStartTime() + a_shiftTime);
	}
}

void rtsTutorialManager::checkCurrentInstructionStartCondition()
{
	rtsTutorialInstruction* current = m_instructions[m_instructionIndex];
	string response = current->getResponseType().c_str();
	if (current->getStartCondition())
	{
		if (!m_cordonLocation1Reached && !response.compare("cordonLocation1"))
		{
			if (cDistance(m_playdoh->getCenterOfMass(), m_bldg1Pos) < m_desiredRadius)
			{
				m_cordonLocation1Reached = true;
				m_instructionIndex++;
				//shiftVisualsBack(-(current->getMaxEndTime() - getCurrentTimeSeconds()));
			}
		}

	}
}

void rtsTutorialManager::checkVisualStartCondition(rtsTutorialInstruction* a_visual)
{
	if (a_visual->getStartCondition())
	{
		string response = a_visual->getResponseType().c_str();
		if (!m_cordonLocation1Reached && !response.compare("cordonLocation1"))
		{
			if (cDistance(m_playdoh->getCenterOfMass(), m_bldg1Pos) < m_desiredRadius)
			{
				a_visual->setLocalPos(m_bldg1Pos);
			}
			else if (cDistance(m_playdoh->getCenterOfMass(), m_bldg2Pos) < m_desiredRadius)
			{
				a_visual->setLocalPos(m_bldg2Pos);
			}
		}

	}
}


void rtsTutorialManager::deallocate()
{
	// instructions
	int numInstructions = m_instructions.size();
	for (int i = 0; i < numInstructions; i++)
	{
		m_camera->m_frontLayer->removeChild(m_instructions[i]);

		delete m_instructions[i];
		m_instructions[i] = NULL;
	}
	m_instructions.clear();

	// hq instructions
	int numInstructions2 = m_instructions2.size();
	for (int i = 0; i < numInstructions2; i++)
	{
		m_hqPanel->removeChild(m_instructions2[i]);

		delete m_instructions2[i];
		m_instructions2[i] = NULL;
	}
	m_instructions2.clear();

	// visuals
	int numVisuals = m_visuals.size();
	for (int i = 0; i < numVisuals; i++)
	{
		rtsTutorialInstruction* visual = m_visuals[i];
		if (visual->getParent() == m_camera->m_frontLayer)
			m_camera->m_frontLayer->removeChild(visual);

		else if (visual->getParent() == m_camera->getParent())
			m_camera->getParent()->removeChild(visual);

		else if (visual->getParent() == m_tool->m_hapticPoint->m_sphereProxy)
			m_tool->m_hapticPoint->m_sphereProxy->removeChild(visual);

		else
		{
			int numBuildings = m_buildings.size();
			for (int i = 0; i < numBuildings; i++)
			{
				if (visual->getParent() == m_buildings[i])
					m_buildings[i]->removeChild(visual);
			}
		}

		delete visual;
		visual = NULL;
	}
	m_visuals.clear();

	// tool arrow visuals
	int numToolArrows = m_toolArrows.size();
	for (int i = 0; i < numToolArrows; i++)
	{
		m_tool->m_hapticPoint->m_sphereProxy->removeChild(m_toolArrows[i]);
		delete m_toolArrows[i];
		m_toolArrows[i] = NULL;
	}

	// visuals hq
	int numVisuals2 = m_visuals2.size();
	for (int i = 0; i < numVisuals2; i++)
	{
		rtsTutorialInstruction* visual = m_visuals2[i];
		if (visual->getParent() == m_camera2->m_frontLayer)
			m_camera->m_frontLayer->removeChild(visual);
		else if (visual->getParent() == m_hqPanel)
			m_hqPanel->removeChild(visual);
		else if (visual->getParent() == m_userInputPanel)
			m_userInputPanel->removeChild(visual);
		else
		{
			int numScopes = m_scopes.size();
			for (int i = 0; i < numScopes; i++)
			{
				if (visual->getParent() == m_scopes[i])
					m_scopes[i]->removeChild(visual);
			}
		}

		delete visual;
		visual = NULL;
	}
	m_visuals2.clear();
}

double rtsTutorialManager::getCurrentTimeSeconds()
{
	//cout << "current demo time: " << m_testStartTime + m_clock.getCurrentTimeSeconds() + m_skippedTime << endl;
	return m_testStartTime + m_clock.getCurrentTimeSeconds() + m_skippedTime;
}

void rtsTutorialManager::initClock()
{
	m_clock.reset();
	m_clock.start();
}

void rtsTutorialManager::pauseClock()
{
	m_clock.stop();
	cout << "tutorial paused\n";
}

void rtsTutorialManager::startClock()
{
	m_clock.start();
	cout << "tutorial running\n";
}

void rtsTutorialManager::checkForPause(bool a_response)
{
	//double currentTime = m_clock.getCurrentTimeSeconds();
	//if (currentTime > m_instructions[m_instructionIndex]->getStartTime()
	//	&& currentTime < m_instructions)
	//{
	//	
	//}
}

// retrieve user input for current instruction
void rtsTutorialManager::retrieveUserInput(bool a_deviceMove, 
										bool a_manipButtonDown, bool a_manipButtonUp, bool a_manipButtonDoubleClick, 
										bool a_manipSpacingButtonDown, bool a_manipSpacingButtonUp,
										bool a_commandPatrolButtonDown, bool a_commandPatrolButtonUp, 
										bool a_resizeButtonDown, bool a_resizeButtonUp)
{
	rtsTutorialInstruction* current = m_instructions[m_instructionIndex];
	string response = current->getResponseType().c_str();
	bool userResponse = false;
	if (a_deviceMove && !response.compare("deviceMove"))
	{
		userResponse = true;
		m_responseTimer.setTimeoutPeriodSeconds(5.0);
		//cout << "device move" << endl;
	}
	else if (a_manipButtonDown && !response.compare("manipButtonDown"))
	{
		userResponse = true;

	}
	else if (a_manipButtonUp && !response.compare("manipButtonUp"))
	{
		userResponse = true;

	}
	else if (a_manipButtonDoubleClick && !response.compare("manipButtonDoubleClick"))
	{
		userResponse = true;

	}
	else if (a_manipSpacingButtonDown && !response.compare("manipSpacingButtonDown"))
	{
		userResponse = true;

	}
	else if (a_manipSpacingButtonUp && !response.compare("manipSpacingButtonUp"))
	{
		userResponse = true;

	}
	else if (a_commandPatrolButtonDown && !response.compare("commandPatrolButtonDown"))
	{
		userResponse = true;
		m_cordonMode = false;
	}
	else if (a_commandPatrolButtonUp && !response.compare("commandPatrolButtonUp"))
	{
		userResponse = true;

	}
	else if (a_resizeButtonDown && !response.compare("resizeButtonDown"))
	{
		userResponse = true;

	}
	else if (a_resizeButtonUp && !response.compare("resizeButtonUp"))
	{
		userResponse = true;

	}
	else if (!m_desiredLocationReached && !response.compare("desiredTeamLocation1"))
	{
		if (cDistance(m_playdoh->getCenterOfMass(), m_desiredLocation1) < m_desiredRadius)
		{
			m_desiredLocationReached = true;
			userResponse = true;
		}
	}
	//else if (!m_cordonLocation1Reached && !response.compare("cordonLocation1"))
	//{
	//	if (cDistance(m_playdoh->getCenterOfMass(), m_bldg1Pos) < m_desiredRadius)
	//	{
	//		m_cordonLocation1Reached = true;
	//		userResponse = true;

	//		//shiftVisualsBack(-(current->getMaxEndTime() - getCurrentTimeSeconds()));
	//	}
	//	//else
	//	//{
	//	//	if (!m_cordonLocation1Append)
	//	//	{
	//	//		//// add message to instruction saying that patrol team is being placed back on bldg1
	//	//		//current->appendToMessage("Return to bldg1 for cordon instruction", m_windowW, m_windowH);
	//	//		//current->addToMaxDuration(4 * m_maxDur);

	//	//		//shiftVisualsBack(4 * m_maxDur);

	//	//		////m_com->patrol(0, 0, m_bldg1Pos.x(), m_bldg1Pos.y(), 0);
	//	//		//m_cordonLocation1Append = true;
	//	//	}
	//	//}
	//}
	else if (m_demoHapticsEnabled && !response.compare("disableHaptics"))
	{
		m_playdoh->setHapticEnabled(false, true);
		m_demoHapticsEnabled = false;
		cout << "disabled haptics" << endl;
	}
	else if (!m_demoShowTargets && !response.compare("showTargets"))
	{
		m_hotspotManager->setShowTargets(true);
		m_demoShowTargets = true;
	}
	else if (m_demoShowTargets && !response.compare("hideTargets"))
	{
		m_hotspotManager->setShowTargets(false);
		m_demoShowTargets = false;
	}
	else if (m_pauseCom && !response.compare("cordonCreated"))
	{
		// unpause server to be able to wait for cordon to be formed while server running
		unpauseServer();
	}
	// if there has been a change from patrol to cordon
	else if (!m_cordonMode && !response.compare("cordonCreated") && m_playdoh->getDistributionMode() == 1)
	{
		m_cordonMode = true;
		userResponse = true;
		//cout << "cordon created" << endl;
	}
	else if (!response.compare("playdohContacted") && !m_playdohHapticsEnabled)
	{
		m_playdoh->setHapticEnabled(true, true);
		m_playdohHapticsEnabled = true;
		//cout << "haptics turned on" << endl;

	}
	else if (!m_demoPlaydohContacted && !response.compare("playdohContacted") && m_playdoh->getContactState())
	{
		userResponse = true;
		m_demoPlaydohContacted = true;
		//cout << "contacted playdoh" << endl;

	}

	//if (!m_playdoh->getHapticEnabled())
	//{
	//	printf("haptics is not enabled\n");
	//}

	//// timer for user response
	//if (userResponse && !m_responseTimer.on())
	//{
	//	m_responseTimer.reset();
	//	m_responseTimer.start();
	//}

	//if (m_responseTimer.timeoutOccurred())
	//{
	//	m_responseTimer.stop();
	//	m_userResponse = true;
	//}

	// update arrow for tool
	updateArrows();

	if (!m_userResponse && userResponse)
		m_userResponse = true;

	//if (m_userResponse && m_instructionIndex == 21)
	//{
	//	printf("ready to hide instruction: %d\n", m_instructionIndex);
	//}
}

void rtsTutorialManager::togglePauseTutorial()
{
	if (m_clock.on())
		pauseClock();
	else startClock();
}

void rtsTutorialManager::updateArrows()
{
	int numArrows = m_toolArrows.size();
	for (int i = 0; i < numArrows; i++)
	{
		//cVector3d pos = m_tool->getDeviceGlobalPos();
		//cVector3d pos2 = m_displayForceScale * m_tool->getDeviceGlobalForce();
		cVector3d pos2 = cVector3d(0, 50, 0);
		m_toolArrows[i]->updateArrowPoints(cVector3d(0,0,0), pos2);
	}
}

void rtsTutorialManager::updateInstructionsHaptics()
{
	if (m_instructionIndex < m_instructions.size())
	{
		rtsTutorialInstruction* current = m_instructions[m_instructionIndex];

		double currentTime = getCurrentTimeSeconds();

		bool userResponse = false;
		if ((currentTime > current->getMinEndTime() && m_userResponse))
		{
			//printf("userResponse processed for instruction: %d\n", m_instructionIndex);
			userResponse = true;
		}
		else if (m_userResponse)
		{
			//printf("waiting for currentTime > minEndTime for instruction: %d\n", m_instructionIndex);
		}

		bool maxTimeReached = false;
		if (currentTime > current->getMaxEndTime())
			maxTimeReached = true;

		if (userResponse || maxTimeReached)
		{
			// hide current instruction
			if (current->getShowEnabled())
			{
				//if (userResponse)
				//	printf("user response, hiding instruction: %d\n", m_instructionIndex);
				//else if (maxTimeReached)
				//	printf("maxTime reached, hiding instruction: %d\n", m_instructionIndex);

				current->setShowEnabled(false, true);
			}

			//// turn on clock if it was previously paused
			//if (!m_clock.on())
			//	startClock();

			// increment instruction index
			if (m_instructionIndex + 1 < m_instructions.size())
			{
				//printf("instruction %d was hidden\n", m_instructionIndex);
				m_instructionIndex++;
				m_demoPlaydohContacted = false;

			}

			// adjust time for next instruction, if needed
			if (m_instructionIndex < m_instructions.size())
			{
				rtsTutorialInstruction* next = m_instructions[m_instructionIndex];

				bool isNext2 = false;
				rtsTutorialInstruction* next2;
				if (m_instruction2Index < m_instructions2.size())
				{
					isNext2 = true;
					next2 = m_instructions2[m_instruction2Index];
				}

				// change start time to current time
				if (m_userResponse)
				{
					m_userResponse = false;
					if (isNext2 && next2->getStartTime() > next->getStartTime())
						m_skippedTime += next->getStartTime() - getCurrentTimeSeconds() - m_waitDur;
					else m_skippedTime += next2->getStartTime() - getCurrentTimeSeconds() - m_waitDur;

					// record if past response has been made
					if (!m_userResponseMade)
						m_userResponseMade = true;
				}
				else if (m_userResponseMade)
				{
					if (isNext2 && next2->getStartTime() > next->getStartTime())
						m_skippedTime += next->getStartTime() - getCurrentTimeSeconds() - m_waitDur;
					else m_skippedTime += next2->getStartTime() - getCurrentTimeSeconds() - m_waitDur;
				}


				// show current instruction
				if (currentTime > next->getStartTime())
				{
					if (!next->getShowEnabled())
					{
						next->setShowEnabled(true, true);
						//printf("immediate showing new instruction: %d\n", m_instructionIndex);
					}
				}
			}
		}
		// show current instruction
		else if (currentTime > current->getStartTime())
		{
			if (!current->getShowEnabled())
			{
				current->setShowEnabled(true, true);
				//printf("showing new instruction: %d\n", m_instructionIndex);
			}
		}
	}
}

void rtsTutorialManager::moveToNextOrPrevInstruction(bool a_next)
{
	rtsTutorialInstruction* current = m_instructions[m_instructionIndex];
	rtsTutorialInstruction* current2 = m_instructions2[m_instruction2Index];

	// check next index for haptics window instructions
	int index = m_instructionIndex;
	int index2 = m_instruction2Index;
	bool haptics = false;
	bool hq = false;
	if (a_next)
	{
		if (index + 1 < m_instructions.size())
		{
			if (index2 + 1 < m_instructions2.size())
			{
				// compute which instruction starts first
				rtsTutorialInstruction* next = m_instructions[index + 1];
				rtsTutorialInstruction* next2 = m_instructions2[index2 + 1];

				if (next->getStartTime() < next2->getStartTime())
				{
					haptics = true;
					m_instructionIndex++;
				}
				else
				{
					hq = true;
					m_instruction2Index++;
				}
			}
			else
			{
				// next instruction is haptics instruction
				haptics = true;
				m_instructionIndex++;
			}
		}
	}
	else
	{
		if (index - 1 > 0)
		{
			if (index2 - 1 > 0)
			{
				// compute which instruction starts first
				rtsTutorialInstruction* next = m_instructions[index - 1];
				rtsTutorialInstruction* next2 = m_instructions2[index2 - 1];

				if (next->getStartTime() > next2->getStartTime())
				{
					haptics = true;
					m_instructionIndex--;
				}
				else
				{
					hq = true;
					m_instruction2Index--;
				}
			}
			else
			{
				// next instruction is haptics instruction
				haptics = true;
				m_instructionIndex--;
			}
		}
	}

	if (haptics)
	{
		current->setShowEnabled(false);
		// adjust time for next instruction, if needed
		rtsTutorialInstruction* next = m_instructions[m_instructionIndex];

		// change start time to current time
		if (a_next)
			m_skippedTime += next->getStartTime() - getCurrentTimeSeconds() - m_waitDur;
		else m_skippedTime -= (getCurrentTimeSeconds() - current->getStartTime() + m_waitDur + next->getMaxEndTime() - next->getStartTime());
		next->setShowEnabled(true);

		// record if past response has been made
		if (!m_userResponseMade)
			m_userResponseMade = true;

	}
	else if (hq)
	{
		current2->setShowEnabled(false);

		// adjust time for next instruction, if needed
		rtsTutorialInstruction* next = m_instructions2[m_instruction2Index];

		// change start time to current time
		if (a_next)
			m_skippedTime += next->getStartTime() - getCurrentTimeSeconds() - m_waitDur;
		else m_skippedTime -= (getCurrentTimeSeconds() - current->getStartTime() + m_waitDur + next->getMaxEndTime() - next->getStartTime());
		next->setShowEnabled(true);

		// record if past response has been made
		if (!m_userResponseMade)
			m_userResponseMade = true;
	}
}


void rtsTutorialManager::updateVisualsHaptics()
{
	double currentTime = getCurrentTimeSeconds();

	int numVisuals = m_visuals.size();
	for (int i = 0; i < numVisuals; i++)
	{
		rtsTutorialInstruction* current = m_visuals[i];

		// display visual
		if (currentTime > current->getStartTime() && currentTime < current->getMaxEndTime())
		{
			checkVisualStartCondition(current);
			current->setShowEnabled(true, true);
		}
		// hide visual
		else if ((currentTime > current->getMinEndTime() && m_userResponse)
			|| currentTime > current->getMaxEndTime())
		{
			// hide current visual
			current->setShowEnabled(false, true);
		}
	}
}

void rtsTutorialManager::updateInstructions2()
{
	if (m_instruction2Index < m_instructions2.size())
	{
		rtsTutorialInstruction* current = m_instructions2[m_instruction2Index];
		double currentTime = getCurrentTimeSeconds();


		if ((currentTime > current->getMinEndTime() && m_userResponse)
			|| currentTime > current->getMaxEndTime())
		{
			// hide current instruction
			if (current->getShowEnabled())
				current->setShowEnabled(false, true);

			//// turn on clock if it was previously paused
			//if (!m_clock.on())
			//	startClock();

			// increment instrucion index
			if (m_instruction2Index + 1 < m_instructions2.size())
				m_instruction2Index++;

			// adjust time for next instruction, if needed
			if (m_instruction2Index < m_instructions2.size())
			{
				rtsTutorialInstruction* next = m_instructions2[m_instruction2Index];
				
				//// change start time to current time
				//if (m_userResponse)
				//{
				//	m_userResponse = false;
				//	next->setStartTime(currentTime + m_waitDur);

				//	// record if past response has been made
				//	if (!m_userResponseMade2)
				//		m_userResponseMade2 = true;
				//}
				//else if (m_userResponseMade2)
				//{
				//	next->setStartTime(currentTime + m_waitDur);
				//}

				//m_skippedTime += next->getStartTime() - getCurrentTimeSeconds() - m_waitDur;

				// show current instruction
				if (currentTime > next->getStartTime())
				{
					if (!next->getShowEnabled())
					{
						next->setShowEnabled(true, true);
					}
				}
			}
		}
		// show current instruction
		else if (currentTime > current->getStartTime())
		{
			if (!current->getShowEnabled())
			{
				current->setShowEnabled(true, true);
				//printf("showing new instruction: %d\n", m_instructionIndex);
			}
		}
	}
}

void rtsTutorialManager::updateVisuals2()
{
	double currentTime = getCurrentTimeSeconds();

	int numVisuals2 = m_visuals2.size();
	for (int i = 0; i < numVisuals2; i++)
	{
		rtsTutorialInstruction* current = m_visuals2[i];

		// display visual
		if (!current->getShowEnabled() && currentTime > current->getStartTime() && currentTime < current->getMaxEndTime())
		{
			current->setShowEnabled(true, true);
		}
		// hide visual
		else if ((currentTime > current->getMinEndTime() && m_userResponse)
			|| currentTime > current->getMaxEndTime())
		{
			// hide current visual
			current->setShowEnabled(false, true);
		}
	}
}

void rtsTutorialManager::resizeInstructions(int a_windowW, int a_windowH)
{
	m_windowW = a_windowW;
	m_windowH = a_windowH;
	int numInstructions = m_instructions.size();
	for (int i = 0; i < numInstructions; i++)
	{
		m_instructions[i]->resize(a_windowW, a_windowH);
	}
}

void rtsTutorialManager::resizeInstructions2(int a_windowW2, int a_windowH2)
{
	m_windowW2 = a_windowW2;
	m_windowH2 = a_windowH2;
	double vSpacing = 10;
	double remainingH2 = m_windowH2 - vSpacing * 2;
	double remainingW2 = m_windowW2 - m_hqPanel->getWidth();
	int numInstructions2 = m_instructions2.size();
	for (int i = 0; i < numInstructions2; i++)
	{
		m_instructions2[i]->resizeWithHandedness(remainingW2, remainingH2, m_rightHanded);
	}
}

void rtsTutorialManager::resizeVisuals(int a_windowW, int a_windowH)
{
	m_windowW = a_windowW;
	m_windowH = a_windowH;
	int numVisuals = m_visuals.size();
	for (int i = 0; i < numVisuals; i++)
	{
		if (m_visuals[i]->getParent() == m_camera->m_frontLayer)
		{
			m_visuals[i]->resizeToPanel(a_windowW, a_windowH);
		}
		m_visuals[i]->resize(a_windowW, a_windowH);
	}
}

void rtsTutorialManager::resizeVisuals2(int a_windowW2, int a_windowH2)
{
	m_windowW2 = a_windowW2;
	m_windowH2 = a_windowH2;
	int numVisuals2 = m_visuals2.size();
	for (int i = 0; i < numVisuals2; i++)
	{
		if (m_visuals2[i]->getParent() == m_camera2->m_frontLayer)
		{
			m_visuals2[i]->resizeToPanel(a_windowW2, a_windowH2);
		}
		// if a panel type visual, then update via panel method
		else if (m_visuals2[i]->getParent() == m_hqPanel)
		{
			m_visuals2[i]->resizeToPanel(m_hqPanel->getWidth(), m_hqPanel->getHeight());
		}
		else
		{
			int numScopes = m_scopes.size();
			for (int j = 0; j < numScopes; j++)
			{
				if (m_visuals2[i]->getParent() == m_scopes[j])
				{
					m_visuals2[i]->resizeToPanel(m_scopes[j]->getWidth(), m_scopes[j]->getHeight());
					break;
				}
			}
		}

		// resize any messages
		m_visuals2[i]->resize(a_windowW2, a_windowH2);
	}
}


