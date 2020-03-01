//---------------------------------------------------------------------------
#include "rtsMapScale.h"
//---------------------------------------------------------------------------

rtsMapScale::rtsMapScale(double a_worldWidth, vector<int> a_markerVals, int a_windowWidth, int a_windowHeight, double a_markerHeight, double a_lineWidth, string a_unit)
{
	m_worldWidth = a_worldWidth;
	m_markerVals = a_markerVals;
	m_distToPixels = a_windowWidth / m_worldWidth;
	m_markerH = a_markerHeight;
	m_lineWidth = a_lineWidth;
	m_unit = a_unit;

	// create main line
	double mainLineL = a_markerVals.back() * m_distToPixels;
	m_mainLine = new cShapeLine(cVector3d(0, 0, 0), cVector3d(mainLineL, 0, 0));
	m_mainLine->setLineWidth(m_lineWidth);
	addChild(m_mainLine);

	int numMarkers = m_markerVals.size();
	for (int i = 0; i < numMarkers; i++)
	{
		// position marker
		double pixelXPos = m_markerVals[i] * m_distToPixels;
		double pixelYPos = -m_markerH / 2;
		cShapeLine* markerLine = new cShapeLine(cVector3d(pixelXPos, pixelYPos, 0), cVector3d(pixelXPos, pixelYPos + m_markerH, 0));
		m_markerLines.push_back(markerLine);
		addChild(markerLine);

		// label for marker
		cFont* markerFont = NEW_CFONTCALIBRI20();
		cLabel* markerLabel = new cLabel(markerFont);
		markerLabel->setText(to_string(m_markerVals[i]) + a_unit);
		markerLabel->setLocalPos(pixelXPos - markerLabel->getWidth() / 2, m_markerH / 2, 0);
		m_labels.push_back(markerLabel);
		addChild(markerLabel);

	}

}

rtsMapScale::~rtsMapScale()
{
	deallocate();
}

void rtsMapScale::resize(int a_w, int a_h)
{
	m_distToPixels = a_w / m_worldWidth;

	// resize main line
	double mainLineL = m_markerVals.back() * m_distToPixels;
	m_mainLine->m_pointA = cVector3d(0, 0, 0);
	m_mainLine->m_pointB = cVector3d(mainLineL, 0, 0);

	int numMarkers = m_markerVals.size();
	for (int i = 0; i < numMarkers; i++)
	{
		// resize position marker
		double pixelXPos = m_markerVals[i] * m_distToPixels;
		double pixelYPos = -m_markerH / 2;
		m_markerLines[i]->m_pointA = cVector3d(pixelXPos, pixelYPos, 0);
		m_markerLines[i]->m_pointB = cVector3d(pixelXPos, pixelYPos + m_markerH, 0);

		// resize label for marker
		m_labels[i]->setLocalPos(pixelXPos - m_labels[i]->getWidth() / 2, m_markerH / 2, 0);
	}
}

void rtsMapScale::deallocate()
{

	// remove scale lines
	int numLines = m_markerLines.size();
	for (int i = 0; i < numLines; i++)
	{
		removeChild(m_markerLines[i]);

		delete m_markerLines[i];
		m_markerLines[i] = NULL;
	}
	m_markerLines.clear();

	// remove scale labels
	int numLabels = m_labels.size();
	for (int i = 0; i < numLines; i++)
	{
		removeChild(m_labels[i]);

		delete m_labels[i];
		m_labels[i] = NULL;
	}
	m_labels.clear();

	// remove main line
	removeChild(m_mainLine);
	delete m_mainLine;
	m_mainLine = NULL;
}