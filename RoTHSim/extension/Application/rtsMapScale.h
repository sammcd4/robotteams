
//---------------------------------------------------------------------------
#ifndef RTSMAPSCALEH
#define RTSMAPSCALEH

//---------------------------------------------------------------------------
#include "chai3d.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;



//===========================================================================
/*!
    \file       rtsMapScale.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Application
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsMapScale
    \ingroup    Application

    \brief      
    Framework for robot team simulator demo for learning
*/
//===========================================================================
class rtsMapScale: public cGenericObject
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsMapScale.
	  rtsMapScale(double a_worldWidth ,vector<int> a_markerVals, int a_windowWidth, int a_windowHeight, double a_markerHeight, double a_lineWidth, string a_unit);
    
	//! Destructor of rtsMapScale.
	  virtual ~rtsMapScale();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
    
	  void resize(int a_w, int a_h);

    
protected:


    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
    
	// main horizontal line
	cShapeLine* m_mainLine;

	// ends and check marks between
	vector<cShapeLine*> m_markerLines;

	// word label for each marker
	vector<cLabel*> m_labels;

	// values for each marker
	vector<int> m_markerVals;

	// marker height
	double m_markerH;

	// distance to pixel conversion factor
	double m_distToPixels;

	// line width
	double m_lineWidth;

	// line color
	cColorf m_lineColor;

	// save world width for resize
	double m_worldWidth;

	// unit
	string m_unit;


    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSMAPSCALEH
//---------------------------------------------------------------------------
