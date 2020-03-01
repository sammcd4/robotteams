//------------------------------------------------------------------------------
#ifndef RTSSIGNALRINGH
#define RTSSIGNALRINGH
//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

class rtsSignalRing: public cGenericWidget
{    
    //--------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //--------------------------------------------------------------------------

public:

    //! Constructor of cScope.
	rtsSignalRing(double a_innerRadius, double a_outerRadius);

    //! Destructor of cScope.
	virtual ~rtsSignalRing() {};

    //--------------------------------------------------------------------------
    // PUBLIC METHODS:
    //--------------------------------------------------------------------------

public:

	void setColors(cColorf a_color0, cColorf a_color1, cColorf a_color2, cColorf a_color3);

	void setColor(int a_sectionNum, cColorf a_color);

	cColorf getColor(int a_sectionNum);

	void updateColorsFromNSEW(bool a_sampleSignal, vector<double> a_nsewVals, vector<cColorf> a_colors);

	void setBaselineFadeColor(cColorf a_color) { m_baselineFadeColor = a_color; }

	cColorf getBaselineFadeColor() { return m_baselineFadeColor; }

    //--------------------------------------------------------------------------
    // PROTECTED MEMBERS:
    //--------------------------------------------------------------------------
protected:

	vector<cMesh*> m_sections;

	vector<cColorf> m_fadeToBaselineStartColors;
	cColorf m_baselineFadeColor;
	vector<double> m_fadeSignalVals;
	vector<double> m_fadeStartSignalVals;

	// fade timer
	vector<cPrecisionClock> m_fadeToBaselineTimers;

    //--------------------------------------------------------------------------
    // PROTECTED METHODS:
    //--------------------------------------------------------------------------

	void updateSectionColor(int a_sectionNum, bool a_sampleSignal, double a_signalVal, cColorf a_color);

};


//------------------------------------------------------------------------------
#endif // RTSSIGNALRINGH
//------------------------------------------------------------------------------
