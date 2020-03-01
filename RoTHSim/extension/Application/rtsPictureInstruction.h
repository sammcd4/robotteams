
//---------------------------------------------------------------------------
#ifndef RTSPICTUREINSTRUCTIONH
#define RTSPICTUREINSTRUCTIONH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsTutorialInstruction.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;



//===========================================================================
/*!
    \file       rtsPictureInstruction.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Application
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsPictureInstruction
    \ingroup    Application

    \brief      
    Framework for robot team simulator demo for learning
*/
//===========================================================================
class rtsPictureInstruction: public rtsTutorialInstruction
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsPictureInstruction.
	  rtsPictureInstruction(string a_filename, double a_startTime, double a_minDuration, double a_maxDuration, int a_windowW, int a_windowH, 
							string a_responseType = "", double a_fadeDuration = 1.0);
    
	//! Destructor of rtsPictureInstruction.
	  virtual ~rtsPictureInstruction();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
    

    
protected:

    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
    
    // member to show a picture on screen
    cBitmap* m_picture;
    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSPICTUREINSTRUCTIONH
//---------------------------------------------------------------------------
