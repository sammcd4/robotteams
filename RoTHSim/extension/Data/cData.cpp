#include "CData.h"

cData::cData()
{
	// set defaults
	m_time = 0;
	m_hapticForce = cVector3d(0, 0, 0);
	m_cursorPos = cVector3d(0, 0, 0);
	m_valid = false;
}

cData::~cData()
{

}