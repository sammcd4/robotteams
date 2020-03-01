#include "rtsScope.h"
//------------------------------------------------------------------------------

//==============================================================================
/*!
    Constructor of rtsScope.
*/
//==============================================================================
rtsScope::rtsScope()
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < C_SCOPE_MAX_SAMPLES; i++)
		{
			m_signalActive[j][i] = false;
		}
	}
	
	m_scopeMargin = 10;

	// initialize grid
	m_showGrid = true;
	//m_numGridLinesH = 10;
	//m_numGridLinesV = 4;
	m_minDistBetweenGridLines = 25; // pixels
	m_gridLineColor.setGrayDark();
	m_gridCreated = false;

	// vertical line divisions
	m_gridDivisionsV.push_back(5);
	m_gridDivisionsV.push_back(2);
	m_gridDivisionsV.push_back(1);


	// horizontal line divisions
	m_gridDivisionsH.push_back(10);
	m_gridDivisionsH.push_back(5);
	m_gridDivisionsH.push_back(2);
	m_gridDivisionsV.push_back(1);

	m_fadeSignalVal = 0.0;

}

//! This method sets values for signals 0, 1, 2, and 3.
void rtsScope::setSampledSignalValues(bool a_sample, 
										const double a_signalValue0,
										const double a_signalValue1,
										const double a_signalValue2,
										const double a_signalValue3)
{
	m_index1 = (m_index1 + 1) % C_SCOPE_MAX_SAMPLES;

	if (m_index1 == m_index0)
	{
		m_index0 = (m_index0 + 1) % C_SCOPE_MAX_SAMPLES;
	}


	// set signal 0
	if (m_signalEnabled[0])
	{
		double value = cClamp(a_signalValue0, m_minValue, m_maxValue);
		m_signals[0][m_index1] = (int)((double)(m_scopeHeight)* (value - m_minValue) / (m_maxValue - m_minValue));
		//if (!m_signalActive[0][m_index1])
			m_signalActive[0][m_index1] = a_sample;
	}
	else
	{
		m_signals[0][m_index1] = 0;
	}

	// set signal 1
	if (m_signalEnabled[1])
	{
		double value = cClamp(a_signalValue1, m_minValue, m_maxValue);
		m_signals[1][m_index1] = (int)((double)(m_scopeHeight)* (value - m_minValue) / (m_maxValue - m_minValue));
		//if (!m_signalActive[1][m_index1])
			m_signalActive[1][m_index1] = a_sample;
	}
	else
	{
		m_signals[1][m_index1] = 0;
	}

	// set signal 2
	if (m_signalEnabled[2])
	{
		double value = cClamp(a_signalValue2, m_minValue, m_maxValue);
		m_signals[2][m_index1] = (int)((double)(m_scopeHeight)* (value - m_minValue) / (m_maxValue - m_minValue));
		//if (!m_signalActive[2][m_index1])
			m_signalActive[2][m_index1] = a_sample;
	}
	else
	{
		m_signals[2][m_index1] = 0;
	}

	// set signal 3
	if (m_signalEnabled[3])
	{
		double value = cClamp(a_signalValue3, m_minValue, m_maxValue);
		m_signals[3][m_index1] = (int)((double)(m_scopeHeight)* (value - m_minValue) / (m_maxValue - m_minValue));
		//if (!m_signalActive[3][m_index1])
			m_signalActive[3][m_index1] = a_sample;
	}
	else
	{
		m_signals[3][m_index1] = 0;
	}

	// initialization case
	if ((m_index0 == 0) && (m_index1 == 1))
	{
		for (int i = 0; i<4; i++)
			m_signals[i][0] = m_signals[i][1];
	}

}

//==============================================================================
/*!
    This method sets the range of values that can be displayed by the scope.

    \param  a_minValue  Minimum value.
    \param  a_maxValue  Maximum value.
*/
//==============================================================================
void rtsScope::setRange(const double a_minValue, 
                      const double a_maxValue)
{
    // sanity check
    if (a_minValue == a_maxValue)
    {
        return;
    }

    // store values
    m_minValue = cMin(a_minValue, a_maxValue);
    m_maxValue = cMax(a_minValue, a_maxValue);
}


//==============================================================================
/*!
    This method sets the size of the scope by defining its width and height.

    \param  a_width   Width of scope.
    \param  a_height  Height of scope.
*/
//==============================================================================
void rtsScope::setSize(const double& a_width, const double& a_height)
{
    // minimum margin between scope data and edge of panel
    double SCOPE_MARGIN = m_scopeMargin;

	// store previous values for width and height
	m_prevScopeWidth = m_scopeWidth;
	m_prevScopeHeight = m_scopeHeight;

    // set new values for width and height.
    double w = cMax(m_panelRadiusTopLeft, m_panelRadiusBottomLeft) + 
               cMax(m_panelRadiusTopRight, m_panelRadiusBottomRight);
    w = cMax(w, SCOPE_MARGIN);
    m_width = cMax(a_width, w);

    double h = cMax(m_panelRadiusTopLeft, m_panelRadiusTopRight) + 
               cMax(m_panelRadiusBottomLeft, m_panelRadiusBottomRight);
    h = cMax(h, SCOPE_MARGIN);
    m_height = cMax(a_height, h);

    // update model of panel
    updatePanelMesh();

    // set dimension of scope.
    m_scopeWidth = m_width - w;
    m_scopeHeight = m_height - h;

    // set position of scope within panel
    m_scopePosition.set(0.5 * (m_width - m_scopeWidth), 0.5 * (m_height - m_scopeHeight), 0.0);

	
	// if grid doesn't exist yet, create it
	if (m_showGrid)
	{
		if (!m_gridCreated)
		{
			createGrid();
			m_gridCreated = true;
		}
		else resizeGrid();
	}

	// resize the signals due to change in size
	resizeSignals();
}

void rtsScope::resizeSignals()
{
	for (int i = 0; i < C_SCOPE_MAX_SAMPLES; i++)
	{
		m_signals[0][i] = (int)((double)(m_scopeHeight) / (double)m_prevScopeHeight * m_signals[0][i]);
	}
}

void rtsScope::setShowGrid(bool a_show)
{
	m_showGrid = a_show;

	int numHGridLines = m_gridLinesH.size();
	for (int i = 0; i < numHGridLines; i++)
	{
		m_gridLinesH[i]->setShowEnabled(a_show);
	}

	int numVGridLines = m_gridLinesV.size();
	for (int i = 0; i < numVGridLines; i++)
	{
		m_gridLinesV[i]->setShowEnabled(a_show);
	}
}

void rtsScope::fadeToColor(bool a_startFade, cColorf a_color, double a_timeout)
{
	if (a_startFade)
		m_nextFadeColor = a_color;

	if (!m_fadeTimer.on())
	{
		m_fadeTimer.setTimeoutPeriodSeconds(a_timeout);
		m_fadeTimer.start();
		m_fadeColor = m_nextFadeColor;
		cout << m_fadeColor.getR() << " " << m_fadeColor.getG() << " " << m_fadeColor.getB() << endl;

		m_peakFadeTimer.setTimeoutPeriodSeconds(0.50);
		m_peakFadeTimer.stop();
		m_peakFadeTimer.reset();
		m_peakFadeTimer.start();
	}
	else
	{

		if (m_peakFadeTimer.on())
		{
			double half = m_peakFadeTimer.getTimeoutPeriodSeconds() / 2;
			double time = m_peakFadeTimer.getCurrentTimeSeconds();

			// interpolate between baseline and supplied color to get current color
			float r, g, b;

			if (time < half)
			{
				// fade up to color
				r = m_baselineFadeColor.getR() + time / half * (m_fadeColor.getR() - m_baselineFadeColor.getR());
				g = m_baselineFadeColor.getG() + time / half * (m_fadeColor.getG() - m_baselineFadeColor.getG());
				b = m_baselineFadeColor.getB() + time / half * (m_fadeColor.getB() - m_baselineFadeColor.getB());
			}
			else
			{
				// fade back to base color
				time = time - half;
				r = m_fadeColor.getR() + time / half * (m_baselineFadeColor.getR() - m_fadeColor.getR());
				g = m_fadeColor.getG() + time / half * (m_baselineFadeColor.getG() - m_fadeColor.getG());
				b = m_fadeColor.getB() + time / half * (m_baselineFadeColor.getB() - m_fadeColor.getB());

			}


			cColorf current = cColorf(r, g, b);
			//current = cColorf(r, 0, 0);
			
			setCornerColors(current, current, m_baselineFadeColor, m_baselineFadeColor);

			if (m_peakFadeTimer.timeoutOccurred())
			{
				// reset peak timer (assuming peak timeout is smaller than fade timeout)
				m_peakFadeTimer.stop();
				m_peakFadeTimer.reset();

				//cout << current.getR() << " " << current.getG() << " " << current.getB() << endl;
				
				// hard reset on scope color
				setCornerColors(m_baselineFadeColor, m_baselineFadeColor, m_baselineFadeColor, m_baselineFadeColor);
			}
		}

		if (m_fadeTimer.timeoutOccurred())
		{
			m_fadeTimer.stop();
			m_fadeTimer.reset();

			// reset peak timer (assuming peak timeout is smaller than fade timeout)
			//m_peakFadeTimer.stop();
			//m_peakFadeTimer.reset();
			//cout << "reset fade timers" << endl;

			//cout << current.getR() << " " << current.getG() << " " << current.getB() << endl;
			


		}
	}

}

void rtsScope::updateColor(bool a_sampleSignal, double a_signalVal, cColorf a_color)
{

	if (a_sampleSignal)
	{
		if (a_signalVal > 0.01 && a_signalVal > m_fadeSignalVal)
		{
			setCornerColors(a_color, a_color, m_baselineFadeColor, m_baselineFadeColor);

			// reset fade to baseline timer (even before timeout)
			m_fadeToBaselineTimer.stop();
			m_fadeToBaselineTimer.reset();

			// start fade to baseline
			m_fadeToBaselineTimer.setTimeoutPeriodSeconds(3.0);
			m_fadeToBaselineTimer.start();

			// set start fade color
			m_fadeToBaselineStartColor = getColorTopLeft();
			m_fadeStartSignalVal = a_signalVal;
		}
	}
	else if (m_fadeToBaselineTimer.on())
	{

		// only compute faded color when timer is active
		double timeout = m_fadeToBaselineTimer.getTimeoutPeriodSeconds();
		double time = m_fadeToBaselineTimer.getCurrentTimeSeconds();

		float r, g, b;
		// fade back to base color
		r = m_fadeToBaselineStartColor.getR() + time / timeout * (m_baselineFadeColor.getR() - m_fadeToBaselineStartColor.getR());
		g = m_fadeToBaselineStartColor.getG() + time / timeout * (m_baselineFadeColor.getG() - m_fadeToBaselineStartColor.getG());
		b = m_fadeToBaselineStartColor.getB() + time / timeout * (m_baselineFadeColor.getB() - m_fadeToBaselineStartColor.getB());
		m_fadeSignalVal = m_fadeStartSignalVal + time / timeout * (0 - m_fadeStartSignalVal);

		cColorf current = cColorf(r, g, b);
		setCornerColors(current, current, m_baselineFadeColor, m_baselineFadeColor);

		if (m_fadeToBaselineTimer.timeoutOccurred())
		{
			m_fadeToBaselineTimer.stop();
			m_fadeToBaselineTimer.reset();
			m_fadeSignalVal = 0.0;
		}

	}

}

void rtsScope::createGrid()
{
	// compute vertical line spacing
	double gridLineDistV = m_scopeHeight;
	int numDivV = 1;
	int numVGridDiv = m_gridDivisionsV.size();
	for (int i = 0; i < numVGridDiv; i++)
	{
		double distCheck = m_scopeHeight / m_gridDivisionsV[i];
		if (distCheck > m_minDistBetweenGridLines)
		{
			numDivV = m_gridDivisionsV[i];
			gridLineDistV = distCheck;
			break;
		}
	}

	for (int i = 0; i < numDivV - 1; i++)
	{
		cVector3d start(m_scopeMargin, m_scopeMargin + (i + 1) * gridLineDistV, 0);
		cVector3d end(m_scopeMargin + m_scopeWidth, m_scopeMargin + (i + 1) * gridLineDistV, 0);
		cShapeLine* vLine = new cShapeLine(start, end);
		vLine->m_colorPointA = m_gridLineColor;
		vLine->m_colorPointB = m_gridLineColor;
		addChild(vLine);

		m_gridLinesV.push_back(vLine);
	}

	// compute horizontal line spacing
	double gridLineDistH = m_scopeWidth;
	int numDivH = 1;
	int numHGridDiv = m_gridDivisionsH.size();
	for (int i = 0; i < numHGridDiv; i++)
	{
		double distCheck = m_scopeWidth / m_gridDivisionsH[i];
		if (distCheck > m_minDistBetweenGridLines)
		{
			numDivH = m_gridDivisionsH[i];
			gridLineDistH = distCheck;
			break;
		}
	}

	for (int i = 0; i < numDivH - 1; i++)
	{
		cVector3d start(m_scopeMargin + (i + 1) * gridLineDistH, m_scopeMargin, 0);
		cVector3d end(m_scopeMargin + (i + 1) * gridLineDistH, m_scopeMargin + m_scopeHeight, 0);
		cShapeLine* hLine = new cShapeLine(start, end);
		hLine->m_colorPointA = m_gridLineColor;
		hLine->m_colorPointB = m_gridLineColor;
		addChild(hLine);

		m_gridLinesH.push_back(hLine);
	}
}

void rtsScope::resizeGrid()
{
	// check grid spacing
	double gridLineDistV = m_scopeHeight;
	int numDivV = 1;
	int numVGridDiv = m_gridDivisionsV.size();
	for (int i = 0; i < numVGridDiv; i++)
	{
		double distCheck = m_scopeHeight / m_gridDivisionsV[i];
		if (distCheck > m_minDistBetweenGridLines)
		{
			numDivV = m_gridDivisionsV[i];
			gridLineDistV = distCheck;
			break;
		}
	}
	

	// update vertical grid lines
	if (numDivV > (int)(m_gridLinesV.size() + 1))
	{
		// update existing lines
		int i;
		int numVGridLines = m_gridLinesV.size();
		for (i = 0; i < numVGridLines; i++)
		{
			// update existing lines
			cVector3d start(m_scopeMargin, m_scopeMargin + (i + 1) * gridLineDistV, 0);
			cVector3d end(m_scopeMargin + m_scopeWidth, m_scopeMargin + (i + 1) * gridLineDistV, 0);
			m_gridLinesV[i]->m_pointA = start;
			m_gridLinesV[i]->m_pointB = end;
			m_gridLinesV[i]->m_colorPointA = m_gridLineColor;
			m_gridLinesV[i]->m_colorPointB = m_gridLineColor;
		}

		// create additional lines
		for (int k = i; k < numDivV - 1; k++)
		{
			cVector3d start(m_scopeMargin, m_scopeMargin + (k + 1) * gridLineDistV, 0);
			cVector3d end(m_scopeMargin + m_scopeWidth, m_scopeMargin + (k + 1) * gridLineDistV, 0);
			cShapeLine* vLine = new cShapeLine(start, end);
			vLine->m_colorPointA = m_gridLineColor;
			vLine->m_colorPointB = m_gridLineColor;
			addChild(vLine);

			m_gridLinesV.push_back(vLine);
		}
	}
	else
	{
		int numVGridLines = m_gridLinesV.size();
		for (int i = 0; i < numVGridLines; i++)
		{
			if (i < numDivV-1)
			{
				// update existing lines
				cVector3d start(m_scopeMargin, m_scopeMargin + (i + 1) * gridLineDistV, 0);
				cVector3d end(m_scopeMargin + m_scopeWidth, m_scopeMargin + (i + 1) * gridLineDistV, 0);
				m_gridLinesV[i]->m_pointA = start;
				m_gridLinesV[i]->m_pointB = end;
				m_gridLinesV[i]->m_colorPointA = m_gridLineColor;
				m_gridLinesV[i]->m_colorPointB = m_gridLineColor;
				m_gridLinesV[i]->setShowEnabled(true);
			}
			else
			{
				m_gridLinesV[i]->setShowEnabled(false);
			}
		}

	}

	// check grid spacing
	double gridLineDistH = m_scopeWidth;
	int numDivH = 1;
	int numHGridDiv = m_gridDivisionsH.size();
	for (int i = 0; i < numHGridDiv; i++)
	{
		double distCheck = m_scopeWidth / m_gridDivisionsH[i];
		if (distCheck > m_minDistBetweenGridLines)
		{
			numDivH = m_gridDivisionsH[i];
			gridLineDistH = distCheck;
			break;
		}
	}

	// update horizontal grid lines
	if (numDivH > (int)(m_gridLinesH.size() + 1))
	{
		// update existing lines
		int i;
		int numHGridLines = m_gridLinesH.size();
		for (i = 0; i < numHGridLines; i++)
		{
			// update existing lines
			cVector3d start(m_scopeMargin + (i + 1) * gridLineDistH, m_scopeMargin, 0);
			cVector3d end(m_scopeMargin + (i + 1) * gridLineDistH, m_scopeMargin + m_scopeHeight, 0);
			m_gridLinesH[i]->m_pointA = start;
			m_gridLinesH[i]->m_pointB = end;
			m_gridLinesH[i]->m_colorPointA = m_gridLineColor;
			m_gridLinesH[i]->m_colorPointB = m_gridLineColor;
		}

		// create additional lines
		for (int k = i; k < numDivH - 1; k++)
		{
			cVector3d start(m_scopeMargin + (k + 1) * gridLineDistH, m_scopeMargin, 0);
			cVector3d end(m_scopeMargin + (k + 1) * gridLineDistH, m_scopeMargin + m_scopeHeight, 0);
			cShapeLine* hLine = new cShapeLine(start, end);
			hLine->m_colorPointA = m_gridLineColor;
			hLine->m_colorPointB = m_gridLineColor;
			addChild(hLine);

			m_gridLinesH.push_back(hLine);
		}
	}
	else
	{
		int numHGridLines = m_gridLinesH.size();
		for (int i = 0; i < numHGridLines; i++)
		{
			if (i < numDivH - 1)
			{
				// update existing lines
				cVector3d start(m_scopeMargin + (i + 1) * gridLineDistH, m_scopeMargin, 0);
				cVector3d end(m_scopeMargin + (i + 1) * gridLineDistH, m_scopeMargin+m_scopeHeight, 0);
				m_gridLinesH[i]->m_pointA = start;
				m_gridLinesH[i]->m_pointB = end;
				m_gridLinesH[i]->m_colorPointA = m_gridLineColor;
				m_gridLinesH[i]->m_colorPointB = m_gridLineColor;
				m_gridLinesH[i]->setShowEnabled(true);
			}
			else
			{
				m_gridLinesH[i]->setShowEnabled(false);
			}
		}

	}
}


//==============================================================================
/*!
    This method renders the scope using OpenGL.

    \param  a_options  Rendering options.
*/
//==============================================================================
void rtsScope::render(cRenderOptions& a_options)
{
#ifdef C_USE_OPENGL

    // render background panel
    if (m_panelEnabled)
    {
        cMesh::render(a_options);
    }

    /////////////////////////////////////////////////////////////////////////
    // Render parts that are always opaque
    /////////////////////////////////////////////////////////////////////////
    if (SECTION_RENDER_OPAQUE_PARTS_ONLY(a_options))
    {
        if (m_index1 == m_index0) { return; }

        // disable lighting
        glDisable(GL_LIGHTING);

        // set line width
        glLineWidth((GLfloat)m_lineWidth);

        // position scope within panel
        glPushMatrix();
        glTranslated(m_scopePosition(0), m_scopePosition(1), 0.0);

        // render signal 0
        for (int i=0; i<4; i++)
        {
            if (m_signalEnabled[i])
            {
                switch (i)
                {
                    case 0: m_colorSignal0.render(); break;
                    case 1: m_colorSignal1.render(); break;
                    case 2: m_colorSignal2.render(); break;
                    case 3: m_colorSignal3.render(); break;
                }

                int x = (int)m_scopeWidth;
                unsigned int i0 = m_index1;
                int i1 = i0-1;
                if (i1 < 0)
                {
                    i1 = C_SCOPE_MAX_SAMPLES-1;
                }

                glBegin(GL_LINES);
				bool first = false;
                while ((i0 != m_index0) && (x>0))
                {
					if (m_signalActive[i][i0] && !first)
					{
						glVertex3d(x, m_signals[i][i0], 0.0);
						first = true;
					}

					if (m_signalActive[i][i1] && first)
					{
						glVertex3d(x - 1, m_signals[i][i1], 0.0);
						first = false;
					}

                    i0 = i1;
                    i1--;
                    if (i1 < 0)
                    {
                        i1 = C_SCOPE_MAX_SAMPLES-1;
                    }
                    x--;
                }
                glEnd();
            }
        }

        // restore OpenGL settings
        glPopMatrix();
        glEnable(GL_LIGHTING);
    }

#endif
}