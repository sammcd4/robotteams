//---------------------------------------------------------------------------
#include "rtsVisualInstruction.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
 Constructor of rtsVisualInstruction.
 
 \fn     rtsVisualInstruction::rtsVisualInstruction()
 \param
 */
//===========================================================================
rtsVisualInstruction::rtsVisualInstruction()
{

}

rtsVisualInstruction::rtsVisualInstruction(int a_visualType, double a_startTime, double a_minDuration, double a_maxDuration, cVector3d a_localPos, double a_fadeDuration)
{
	//// rectangle
	//if (a_visualType == 0)
	//{
	//	m_object = new cMesh();
	//	cCreateRing(m_object, a_innerRadius, a_outerRadius)
	//}
	//// circle
	//else if (a_visualType == 1)
	//{

	//}
	//// arrow
	//else if (a_visualType == 2)
	//{

	//}
	//initDemoInstruction(a_startTime, a_minDuration, a_maxDuration, a_fadeDuration);
}


rtsVisualInstruction::~rtsVisualInstruction()
{
    // deallocate memory
    deallocate();
}

void rtsVisualInstruction::deallocate()
{

}