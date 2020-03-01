
//---------------------------------------------------------------------------
#ifndef ROBOTTEAMSH
#define ROBOTTEAMSH
//---------------------------------------------------------------------------

//===========================================================================
/*!
\file       robotteams.h

\brief
<b> ROBOT TEAM SIMULATION </b> \n
Main Header File.
*/
//===========================================================================

#include <deque>

//---------------------------------------------------------------------------
//!     \defgroup   Connection
//---------------------------------------------------------------------------
#include "Connection/comm.h"
#include "Connection/command.pb.h"
#include "Connection/swarm.pb.h"
#include "chai3d.h"

//---------------------------------------------------------------------------
//!     \defgroup   SensorMessageDisplay  Message Feed
//---------------------------------------------------------------------------
#include "SensorMessageDisplay/rtsMessageFeed.h"
#include "SensorMessageDisplay/rtsScope.h"
#include "SensorMessageDisplay/rtsDigitalClock.h"
#include "SensorMessageDisplay/rtsSignalRing.h"
#include "SensorMessageDisplay/rtsDetections.h"

//---------------------------------------------------------------------------
//!     \defgroup   Controller Controller
//---------------------------------------------------------------------------
#include "Controller\rtsController.h"

//---------------------------------------------------------------------------
//!     \defgroup   system
//---------------------------------------------------------------------------
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------------------------------------------------------------
//!     \defgroup   chai3d Chai3d
//---------------------------------------------------------------------------
#include "chai3d.h"

//---------------------------------------------------------------------------
//!     \defgroup   rtsClay CrtsClay 
//---------------------------------------------------------------------------
#include "Clay/rtsClay3D.h"
#include "Clay/rtsClay.h"

//---------------------------------------------------------------------------
//!     \defgroup   other useful tools
//---------------------------------------------------------------------------
#include "Other/rtsPolygon.h"
#include "Other/rtsPolygonBounds.h"

//---------------------------------------------------------------------------
//!     \defgroup   Robots Robots
//---------------------------------------------------------------------------
#include "Robots/rtsRobotTeam.h"

//---------------------------------------------------------------------------
//!     \defgroup   Hotspots Hotspots
//---------------------------------------------------------------------------
#include "Hotspots/rtsHotspotManager.h"

//---------------------------------------------------------------------------
//!     \defgroup   Data Data
//---------------------------------------------------------------------------
#include "Data/rtsDataManager.h"

//---------------------------------------------------------------------------
//!     \defgroup   Application Tutorial Instructions
//---------------------------------------------------------------------------

#include "Application\rtsTutorialManager.h"
#include "Application\rtsTutorialInstruction.h"
#include "Application\rtsPictureInstruction.h"

#include "Application\rtsMapScale.h"

//---------------------------------------------------------------------------
//!     \defgroup   other
//---------------------------------------------------------------------------
#include <fstream>
#include <mutex>
#include <string>
#include <iostream>
#include <windows.h>

//---------------------------------------------------------------------------
//!     \defgroup   other rtThread
//---------------------------------------------------------------------------
#include "Other/rtsThread.h"

//---------------------------------------------------------------------------
//!     \defgroup   NASATLX rtsNASATLX
//---------------------------------------------------------------------------
#include "NASATLX\rtsNASATLX.h"
#include "NASATLX\rtsNASATLXDefinitions.h"

//------------------------------------------------------------------------------
#ifndef MACOSX
#include "GL/glut.h"
#else
#include "GLUT/glut.h"
#endif
//------------------------------------------------------------------------------


//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
