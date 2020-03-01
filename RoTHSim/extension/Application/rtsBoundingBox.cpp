//---------------------------------------------------------------------------
#include "rtsBoundingBox.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
 Constructor of rtsBoundingBox.
 
 \fn     rtsBoundingBox::rtsBoundingBox()
 \param
 */
//===========================================================================
rtsBoundingBox::rtsBoundingBox()
{

}

rtsBoundingBox::rtsBoundingBox(double a_width, double a_height, double a_lineWidth, cColorf a_color, double a_startTime, double a_minDuration, double a_maxDuration, 
								cVector3d a_localPos, string a_responseType, double a_fadeDuration, double a_shiftScale)
{
	m_width = a_width;
	m_height = a_height;
	m_lineWidth = a_lineWidth;
	setLocalPos(a_localPos);
	initTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);

	// create edges of box
	cVector3d blc = a_localPos;
	cVector3d brc = a_localPos + cVector3d(a_width, 0, 0);
	cVector3d trc = a_localPos + cVector3d(a_width, a_height, 0);
	cVector3d tlc = a_localPos + cVector3d(0, a_height, 0);
	cVector3d yShift = cVector3d(0, a_lineWidth / (2 * a_shiftScale), 0); // shift scale to convert from world coordinates to pixels, if needed
	cVector3d xShift = cVector3d(a_lineWidth / (2 * a_shiftScale), 0, 0);

	cShapeLine* bottom = new cShapeLine(blc + yShift, brc + yShift);
	bottom->m_colorPointA = a_color;
	bottom->m_colorPointB = a_color;
	bottom->setLineWidth(a_lineWidth);
	addChild(bottom);

	cShapeLine* right = new cShapeLine(brc - xShift, trc - xShift);
	right->m_colorPointA = a_color;
	right->m_colorPointB = a_color;
	right->setLineWidth(a_lineWidth);
	addChild(right);

	cShapeLine* top = new cShapeLine(trc - yShift, tlc - yShift);
	top->m_colorPointA = a_color;
	top->m_colorPointB = a_color;
	top->setLineWidth(a_lineWidth);
	addChild(top);

	cShapeLine* left = new cShapeLine(tlc + xShift, blc + xShift);
	left->m_material->m_ambient = a_color;
	left->m_colorPointA = a_color;
	left->m_colorPointB = a_color;
	left->setLineWidth(a_lineWidth);
	addChild(left);

	m_edges.push_back(bottom);
	m_edges.push_back(right);
	m_edges.push_back(top);
	m_edges.push_back(left);
}


rtsBoundingBox::~rtsBoundingBox()
{
    // deallocate memory
    deallocate();
}

void rtsBoundingBox::resizeToPanel(int a_width, int a_height)
{
	// create edges of box
	cVector3d localPos = getLocalPos();
	cVector3d blc = localPos;
	cVector3d brc = localPos + cVector3d(a_width, 0, 0);
	cVector3d trc = localPos + cVector3d(a_width, a_height, 0);
	cVector3d tlc = localPos + cVector3d(0, a_height, 0);
	cVector3d yShift = cVector3d(0, m_lineWidth / 2, 0);
	cVector3d xShift = cVector3d(m_lineWidth / 2, 0, 0);

	m_edges[0]->m_pointA = blc + yShift;
	m_edges[0]->m_pointB = brc + yShift;

	m_edges[1]->m_pointA = brc - xShift;
	m_edges[1]->m_pointB = trc - xShift;

	m_edges[2]->m_pointA = trc - yShift;
	m_edges[2]->m_pointB = tlc - yShift;

	m_edges[3]->m_pointA = tlc + xShift;
	m_edges[3]->m_pointB = blc + xShift;
}

void rtsBoundingBox::deallocate()
{
	int numEdges = m_edges.size();
	for (int i = 0; i < numEdges; i++)
	{
		removeChild(m_edges[i]);
		delete m_edges[i];
		m_edges[i] = NULL;
	}
}