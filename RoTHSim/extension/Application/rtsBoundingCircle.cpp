//---------------------------------------------------------------------------
#include "rtsBoundingCircle.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
 Constructor of rtsBoundingCircle.
 
 \fn     rtsBoundingCircle::rtsBoundingCircle()
 \param
 */
//===========================================================================
rtsBoundingCircle::rtsBoundingCircle()
{

}

rtsBoundingCircle::rtsBoundingCircle(const double& a_innerRadius, const double& a_outerRadius, const unsigned int a_numSides, const unsigned int a_numRings,
									const cMatrix3d& a_rot, const cColorf& a_color, 
									double a_startTime, double a_minDuration, double a_maxDuration,
									cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{

	//m_object = new cMesh();
	//cCreateRing(m_object, a_innerRadius, a_outerRadius, a_numSides, a_numRings, cVector3d(0,0,0), a_rot, a_color);
	//addChild(m_object);

	this->setLocalPos(a_localPos);

	m_torus = new cShapeTorus(a_innerRadius, a_outerRadius);

	// don't want it to look 3d because it's a widget
	cColorf noRelfect(0, 0, 0);
	m_torus->m_material->m_emission = noRelfect;
	m_torus->m_material->m_specular = noRelfect;
	m_torus->m_material->m_diffuse = noRelfect;


	m_torus->setLocalRot(a_rot);
	m_torus->setLocalPos(0,0,0);
	m_torus->m_material->m_ambient = a_color;
	addChild(m_torus);
	initTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);
}


rtsBoundingCircle::~rtsBoundingCircle()
{
    // deallocate memory
    deallocate();
}

void rtsBoundingCircle::deallocate()
{
	removeChild(m_torus);
	delete m_torus;
	m_torus = NULL;
}