//---------------------------------------------------------------------------
#include "rtsArrow.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
Constructor of rtsArrow.

\fn     rtsArrow::rtsArrow()
\param
*/
//===========================================================================
rtsArrow::rtsArrow()
{

}

rtsArrow::rtsArrow(double a_width, cColorf a_color, double a_startTime, double a_minDuration, double a_maxDuration,
					cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	m_line = new cShapeLine(a_localPos, a_localPos);
	m_line->m_colorPointA = a_color;
	m_line->m_colorPointB = a_color;
	addChild(m_line);
	m_line->rotateAboutLocalAxisDeg(cVector3d(0, 1, 0), 90);
	m_line->rotateAboutLocalAxisDeg(cVector3d(0, 0, 1), 90);
	m_isArrow = false;
}

rtsArrow::rtsArrow(double a_length, double a_radiusShaft, double a_lengthTip, double a_radiusTip, bool a_includeTipsAtBothExtremities, unsigned int a_numSides,
					const cVector3d& a_direction, const cVector3d& a_pos, const cColorf& a_color, double a_startTime, double a_minDuration, double a_maxDuration,
					cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{

	m_arrow = new cMesh();
	cCreateArrow(m_arrow, a_length, a_radiusShaft, a_lengthTip, a_radiusTip, a_includeTipsAtBothExtremities, a_numSides, a_direction, a_pos, a_color);

	initTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);
	m_isArrow = true;
}


rtsArrow::~rtsArrow()
{
	// deallocate memory
	deallocate();
}

void rtsArrow::updateArrowPoints(cVector3d a_pointA, cVector3d a_pointB)
{
	if (m_isArrow)
	{

	}
	else
	{
		m_line->m_pointA = a_pointA;
		m_line->m_pointB = a_pointB;
	}
}


void rtsArrow::deallocate()
{
	if (m_isArrow)
	{ 
		removeChild(m_arrow);
		delete m_arrow;
		m_arrow = NULL;
	}
	else
	{
		removeChild(m_line);
		delete m_line;
		m_line = NULL;
	}
}