
//---------------------------------------------------------------------------
#ifndef RTSBOUNDINGBOXH
#define RTSBOUNDINGBOXH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsTutorialInstruction.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;



//===========================================================================
/*!
    \file       rtsBoundingBox.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Visual Instruction
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsBoundingBox
    \ingroup    Application

    \brief      
    Class for displaying a bounding box to accompany instructions
*/
//===========================================================================
class rtsBoundingBox: public rtsTutorialInstruction
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsBoundingBox.
	  rtsBoundingBox();
	  rtsBoundingBox(double a_width, double a_height, double a_lineWidth, cColorf a_color, double a_startTime, double a_minDuration, double a_maxDuration,
		  cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0, double a_shiftScale = 1.0);

	  //! Destructor of rtsBoundingBox.
	  virtual ~rtsBoundingBox();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
    
	  // resize to panel
	  virtual void resizeToPanel(int a_width, int a_height);

    
protected:

    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
	
	// edges of box
	vector<cShapeLine*> m_edges;

	double m_width;
	double m_height;

	// line width
	double m_lineWidth;


    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSVISUALINSTRUCTIONH
//---------------------------------------------------------------------------
