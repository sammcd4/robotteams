
//---------------------------------------------------------------------------
#ifndef RTSARROWH
#define RTSARROWH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include "rtsTutorialInstruction.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

//===========================================================================
/*!
\file       rtsArrow.h

\brief
<b> Application </b> \n
Robot Team Simulator Visual Instruction
*/
//===========================================================================

//===========================================================================
/*!
\class      rtsArrow
\ingroup    Application

\brief
Class for displaying visual instructions
*/
//===========================================================================
class rtsArrow : public rtsTutorialInstruction
{
public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsArrow.
	rtsArrow();

	rtsArrow(double a_width, cColorf a_color, double a_startTime, double a_minDuration, double a_maxDuration,
				cVector3d a_localPos, string a_responseType, double a_fadeDuration);

	rtsArrow(double a_length, double a_radiusShaft, double a_lengthTip, double a_radiusTip, bool a_includeTipsAtBothExtremities, unsigned int a_numSides,
			const cVector3d& a_direction, const cVector3d& a_pos, const cColorf& a_color, double a_startTime, double a_minDuration, double a_maxDuration,
			cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);

	//! Destructor of rtsArrow.
	virtual ~rtsArrow();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------

	void updateArrowPoints(cVector3d a_pointA, cVector3d a_pointB);


protected:

	//-----------------------------------------------------------------------
	// MEMBERS:
	//-----------------------------------------------------------------------

	//// sphere object to display a circle around object
	//cShapeSphere* a_sphere;

	//// box object to display a box around object
	//cShapeBox* a_box;

	// arrow object to display an arrow from object
	cMesh* m_arrow;

	// simple line instead of arrow
	cShapeLine* m_line;

	bool m_isArrow;


private:

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------

	void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSARROWH
//---------------------------------------------------------------------------
