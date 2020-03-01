
//---------------------------------------------------------------------------
#ifndef RTSBOUNDINGCIRCLEH
#define RTSBOUNDINGCIRCLEH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsTutorialInstruction.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;



//===========================================================================
/*!
    \file       rtsBoundingCircle.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Visual Instruction
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsBoundingCircle
    \ingroup    Application

    \brief      
    Class for displaying visual instructions
*/
//===========================================================================
class rtsBoundingCircle: public rtsTutorialInstruction
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsBoundingCircle.
	  rtsBoundingCircle();
	  rtsBoundingCircle(const double& a_innerRadius, const double& a_outerRadius, const unsigned int a_numSides, const unsigned int a_numRings,
						  const cMatrix3d& a_rot, const cColorf& a_color,
						  double a_startTime, double a_minDuration, double a_maxDuration,
						cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	  //! Destructor of rtsBoundingCircle.
	  virtual ~rtsBoundingCircle();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
    

    
protected:

    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
    
	// sphere object to display a circle around object
	cShapeTorus* m_torus;

	//// box object to display a box around object
	//cShapeBox* a_box;

	//// arrow object to display an arrow from object
	//cMesh* m_object;



    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSBOUNDINGCIRCLEH
//---------------------------------------------------------------------------
