
//------------------------------------------------------------------------------
#ifndef RTSScopeH
#define RTSScopeH
//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//------------------------------------------------------------------------------
//const int C_SCOPE_MAX_SAMPLES = 3000;
//------------------------------------------------------------------------------

class rtsScope : public cScope
{    
    //--------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //--------------------------------------------------------------------------

public:

    //! Constructor of cScope.
	rtsScope();

    //! Destructor of cScope.
	virtual ~rtsScope() {};


    //--------------------------------------------------------------------------
    // PUBLIC METHODS:
    //--------------------------------------------------------------------------

public:

    //! This method sets the line width of the signal.
    inline void setLineWidth(const double a_lineWidth) { m_lineWidth = fabs(a_lineWidth); }

    //! This method returns the line width of the signal
    inline double getLineWidth() const { return (m_lineWidth); }

	//! This method sets values for signals 0, 1, 2, and 3.
	void setSampledSignalValues(bool a_sample,
								const double a_signalValue0 = 0,
								const double a_signalValue1 = 0,
								const double a_signalValue2 = 0,
								const double a_signalValue3 = 0);

    //! This method sets the range of input values which can be displayed on the scope.
    virtual void setRange(const double a_minValue, 
                          const double a_maxValue); 

    //! This method sets the size of this scope.
    virtual void setSize(const double& a_width, const double& a_height);

	// resize signals
	void resizeSignals();

	// set show grid
	void setShowGrid(bool a_show);

	// get scope margin
	double getScopeMargin() { return m_scopeMargin; }

	// fade to a color
	void fadeToColor(bool a_startFade, cColorf a_color, double a_timeout);

	void updateColor(bool a_sampleSignal, double a_signalVal, cColorf a_color);

	void setBaselineFadeColor(cColorf a_color) { m_baselineFadeColor = a_color; }

	cColorf getBaselineFadeColor() { return m_baselineFadeColor; }

    //--------------------------------------------------------------------------
    // PUBLIC MEMBERS:
    //--------------------------------------------------------------------------

public:




    //--------------------------------------------------------------------------
    // PROTECTED MEMBERS:
    //--------------------------------------------------------------------------
protected:

	//! active flags for all signals.
	bool m_signalActive[4][C_SCOPE_MAX_SAMPLES];

	// previous scope width (before size change)
	double m_prevScopeWidth;

	// previous scope height (before size change)
	double m_prevScopeHeight;

	// show grid
	bool m_showGrid;

	// scope margin
	double m_scopeMargin;

	//// number of horizontal grid lines
	//int m_numGridLinesH;

	//// number of vertical grid lines
	//int m_numGridLinesV;

	// minimum distance between grid lines
	int m_minDistBetweenGridLines;

	// grid line color
	cColorf m_gridLineColor;

	// horizontal grid lines
	vector<cShapeLine*> m_gridLinesH;

	// vertical grid lines
	vector<cShapeLine*> m_gridLinesV;

	// grid created flag
	bool m_gridCreated;

	// division options for grid lines
	vector<int> m_gridDivisionsV;
	vector<int> m_gridDivisionsH;

	// baseline fade color
	cColorf m_baselineFadeColor;
	cColorf m_fadeColor;
	cColorf m_nextFadeColor;
	cColorf m_fadeToBaselineStartColor;
	double m_fadeSignalVal;
	double m_fadeStartSignalVal;

	// fade timer
	cPrecisionClock m_fadeTimer;
	cPrecisionClock m_peakFadeTimer;
	cPrecisionClock m_fadeToBaselineTimer;


    //--------------------------------------------------------------------------
    // PROTECTED METHODS:
    //--------------------------------------------------------------------------

protected:

	// create grid lines
	void createGrid();

	// resize grid lines
	void resizeGrid();

protected:

    //! This method renders the object graphically using OpenGL.
    virtual void render(cRenderOptions& a_options);
};


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
