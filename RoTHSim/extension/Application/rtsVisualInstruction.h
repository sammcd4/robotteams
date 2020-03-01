
//---------------------------------------------------------------------------
#ifndef RTSVISUALINSTRUCTIONH
#define RTSVISUALINSTRUCTIONH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsTutorialInstruction.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;



//===========================================================================
/*!
    \file       rtsVisualInstruction.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Visual Instruction
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsVisualInstruction
    \ingroup    Application

    \brief      
    Class for displaying visual instructions
*/
//===========================================================================
class rtsVisualInstruction: public rtsTutorialInstruction
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsVisualInstruction.
	  rtsVisualInstruction();
	  rtsVisualInstruction(int a_visualType, double a_startTime, double a_minDuration, double a_maxDuration, 
							cVector3d a_localPos = cVector3d(0, 0, 0), double a_fadeDuration = 1.0);

	  //! Destructor of rtsVisualInstruction.
	  virtual ~rtsVisualInstruction();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
    

    
protected:

    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
    
	//// sphere object to display a circle around object
	//cShapeSphere* a_sphere;

	//// box object to display a box around object
	//cShapeBox* a_box;

	// arrow object to display an arrow from object
	cMesh* m_object;


    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSVISUALINSTRUCTIONH
//---------------------------------------------------------------------------
