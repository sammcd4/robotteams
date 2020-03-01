
//------------------------------------------------------------------------------
#ifndef RTSSLIDERH
#define RTSSLIDERH
//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//------------------------------------------------------------------------------
//const int C_SCOPE_MAX_SAMPLES = 3000;
//------------------------------------------------------------------------------

class rtsSlider : public cGenericWidget
{    
    //--------------------------------------------------------------------------
    // CONSTRUCTOR & DESTRUCTOR:
    //--------------------------------------------------------------------------

public:

    //! Constructor of cScope.
	rtsSlider(int a_width, int a_height, string a_topLeftCornerMessage = "", string a_topRightCornerMessage = "", string a_bottomLeftCornerMessage = "",
					string a_bottomRightCornerMessage = "");

    //! Destructor of cScope.
	virtual ~rtsSlider();


    //--------------------------------------------------------------------------
    // PUBLIC METHODS:
    //--------------------------------------------------------------------------

public:
	virtual double getWidth(){ return m_panel->getWidth(); }
	virtual double getHeight(){ return m_panel->getHeight(); }

    //--------------------------------------------------------------------------
    // PUBLIC MEMBERS:
    //--------------------------------------------------------------------------

public:

	void resize();

	// set value between 0-1
	void setValue(double a_val);

	// get value of slider (in percentage)
	double getValue();

	cShapeLine* getLine();
	cPanel* getKnob() { return m_knob; }

	//--------------------------------------------------------------------------
	// PROTECTED METHODS:
	//--------------------------------------------------------------------------
protected:

	void deallocate();

    //--------------------------------------------------------------------------
    // PROTECTED MEMBERS:
    //--------------------------------------------------------------------------
protected:

	cPanel* m_panel;

	cShapeLine* m_line;

	cPanel* m_knob;
	double m_val;

	cLabel* m_topLeftCornerLabel;
	cLabel* m_topRightCornerLabel;
	cLabel* m_bottomLeftCornerLabel;
	cLabel* m_bottomRightCornerLabel;


};


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
