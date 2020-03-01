#include "rtsSignalRing.h"
//------------------------------------------------------------------------------

//==============================================================================
/*!
    Constructor of rtsSignalRing.
*/
//==============================================================================
rtsSignalRing::rtsSignalRing(double a_innerRadius, double a_outerRadius)
{
	double sectionAngle = 90;
	cColorf color(1.0, 1.0, 1.0);

	for (int i = 0; i < 4; i++)
	{
		cMesh* section = new cMesh();
		cMatrix3d rot;
		rot.identity();
		rot.rotateAboutLocalAxisDeg(cVector3d(0, 0, 1.0), i*90);
		cCreateRingSection(section, a_innerRadius, a_innerRadius, a_outerRadius, sectionAngle, true, 32U, 32U, cVector3d(0,0,0), rot);
		section->setLocalPos(0, 0, 0);
		m_sections.push_back(section);
		addChild(section);

		m_fadeToBaselineStartColors.push_back(color);
		m_fadeSignalVals.push_back(0.0);
		m_fadeStartSignalVals.push_back(0.0);
		m_baselineFadeColor = color;
		cPrecisionClock clock;
		m_fadeToBaselineTimers.push_back(clock);

	}
	setColors(color, color, color, color);

	// don't want it to look 3d because it's a widget
	cColorf noRelfect(0, 0, 0);
	m_sections[0]->m_material->m_emission = noRelfect;
	m_sections[1]->m_material->m_emission = noRelfect;
	m_sections[2]->m_material->m_emission = noRelfect;
	m_sections[3]->m_material->m_emission = noRelfect;
	m_sections[0]->m_material->m_specular = noRelfect;
	m_sections[1]->m_material->m_specular = noRelfect;
	m_sections[2]->m_material->m_specular = noRelfect;
	m_sections[3]->m_material->m_specular = noRelfect;
	m_sections[0]->m_material->m_diffuse = noRelfect;
	m_sections[1]->m_material->m_diffuse = noRelfect;
	m_sections[2]->m_material->m_diffuse = noRelfect;
	m_sections[3]->m_material->m_diffuse = noRelfect;




	// fade timer
	vector<cPrecisionClock> m_fadeToBaselineTimers;

}

void rtsSignalRing::setColors(cColorf a_color0, cColorf a_color1, cColorf a_color2, cColorf a_color3)
{
	m_sections[0]->m_material->m_ambient = a_color0;
	m_sections[1]->m_material->m_ambient = a_color1;
	m_sections[2]->m_material->m_ambient = a_color2;
	m_sections[3]->m_material->m_ambient = a_color3;
}

void rtsSignalRing::setColor(int a_sectionNum, cColorf a_color)
{
	m_sections[a_sectionNum]->m_material->m_ambient = a_color;
}

cColorf rtsSignalRing::getColor(int a_sectionNum)
{
	return m_sections[a_sectionNum]->m_material->m_ambient;
}


void rtsSignalRing::updateColorsFromNSEW(bool a_sampleSignal, vector<double> a_nsewVals, vector<cColorf> a_colors)
{
	// north data
	updateSectionColor(0, a_sampleSignal, a_nsewVals[0], a_colors[0]);
	
	// south data
	updateSectionColor(2, a_sampleSignal, a_nsewVals[1], a_colors[1]);

	// east data
	updateSectionColor(3, a_sampleSignal, a_nsewVals[2], a_colors[2]);
	
	// west data
	updateSectionColor(1, a_sampleSignal, a_nsewVals[3], a_colors[3]);
}

void rtsSignalRing::updateSectionColor(int a_sectionNum, bool a_sampleSignal, double a_signalVal, cColorf a_color)
{

	if (a_sampleSignal)
	{
		if (a_signalVal > 0.01 && a_signalVal > m_fadeSignalVals[a_sectionNum])
		{
			setColor(a_sectionNum, a_color);

			// reset fade to baseline timer (even before timeout)
			m_fadeToBaselineTimers[a_sectionNum].stop();
			m_fadeToBaselineTimers[a_sectionNum].reset();

			// start fade to baseline
			m_fadeToBaselineTimers[a_sectionNum].setTimeoutPeriodSeconds(2.0);
			m_fadeToBaselineTimers[a_sectionNum].start();

			// set start fade color
			m_fadeToBaselineStartColors[a_sectionNum] = getColor(a_sectionNum);;
			m_fadeStartSignalVals[a_sectionNum] = a_signalVal;
		}
	}
	else if (m_fadeToBaselineTimers[a_sectionNum].on())
	{

		// only compute faded color when timer is active
		double timeout = m_fadeToBaselineTimers[a_sectionNum].getTimeoutPeriodSeconds();
		double time = m_fadeToBaselineTimers[a_sectionNum].getCurrentTimeSeconds();

		float r, g, b;
		// fade back to base color
		r = m_fadeToBaselineStartColors[a_sectionNum].getR() + time / timeout * (m_baselineFadeColor.getR() - m_fadeToBaselineStartColors[a_sectionNum].getR());
		g = m_fadeToBaselineStartColors[a_sectionNum].getG() + time / timeout * (m_baselineFadeColor.getG() - m_fadeToBaselineStartColors[a_sectionNum].getG());
		b = m_fadeToBaselineStartColors[a_sectionNum].getB() + time / timeout * (m_baselineFadeColor.getB() - m_fadeToBaselineStartColors[a_sectionNum].getB());
		m_fadeSignalVals[a_sectionNum] = m_fadeStartSignalVals[a_sectionNum] + time / timeout * (0 - m_fadeStartSignalVals[a_sectionNum]);

		cColorf current = cColorf(r, g, b);
		setColor(a_sectionNum, current);

		if (m_fadeToBaselineTimers[a_sectionNum].timeoutOccurred())
		{
			m_fadeToBaselineTimers[a_sectionNum].stop();
			m_fadeToBaselineTimers[a_sectionNum].reset();
			m_fadeSignalVals[a_sectionNum] = 0.0;
		}

	}

}