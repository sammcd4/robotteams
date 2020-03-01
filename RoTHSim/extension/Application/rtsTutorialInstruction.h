
//---------------------------------------------------------------------------
#ifndef RTSTUTORIALINSTRUCTIONH
#define RTSTUTORIALINSTRUCTIONH
//---------------------------------------------------------------------------
#include "chai3d.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;



//===========================================================================
/*!
    \file       rtsTutorialInstruction.h

    \brief 
    <b> Application </b> \n 
    Robot Team Simulator Application
*/
//===========================================================================

//===========================================================================
/*!
    \class      rtsTutorialInstruction
    \ingroup    Application

    \brief      
    Framework for robot team simulator demo for learning
*/
//===========================================================================
class rtsTutorialInstruction: public cGenericObject
{
  public:

	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

	//! Constructor of rtsTutorialInstruction.
	  rtsTutorialInstruction() {};
	  
	  //rtsTutorialInstruction(double a_startTime, cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);
	  
	  rtsTutorialInstruction(double a_startTime, double a_minDuration, double a_maxDuration, 
							cVector3d a_localPos = cVector3d(0, 0, 0), string a_responseType = "", double a_fadeDuration = 1.0);
	  
	  rtsTutorialInstruction(double a_startTime, double a_minDuration, double a_maxDuration, string a_messageText, int a_windowW, int a_windowH, 
							string a_responseType = "", int a_negVertOffset = 100, double a_fadeDuration = 1.0);

	  rtsTutorialInstruction(double a_startTime, double a_minDuration, double a_maxDuration, vector<string> a_messages, int a_windowW, int a_windowH, 
		  					string a_responseType = "", int a_negVertOffset = 100, double a_fadeDuration = 1.0);
    
	//! Destructor of rtsTutorialInstruction.
	  virtual ~rtsTutorialInstruction();

	//-----------------------------------------------------------------------
	// METHODS:
	//-----------------------------------------------------------------------
    
    // get minimum duration
    double getMinDuration() { return m_minDuration;}
    
	// set start time
	void setStartTime(double a_startTime) { m_startTime = a_startTime; }

	// get start time
	double getStartTime() { return m_startTime; }

	// set fade duration
	void setFadeDuration(double a_fadeDuration) { m_fadeDuration = a_fadeDuration; }

    // set minimum duration
    void setMinDuration(double a_minDuration);

	// set maximum duration
	void setMaxDuration(double a_maxDuration);

	// add to maximum duration
	void addToMaxDuration(double a_maxAddition) { m_maxDuration += a_maxAddition; }

	// get minimum end time
	double getMinEndTime() { return (m_startTime + m_minDuration); }

	// get maximum end time
	double getMaxEndTime() { return (m_startTime + m_maxDuration); }

	// resize instruction
	void resizeWithHandedness(int a_windowW, int a_windowH, bool a_rightHanded);

	// resize instruction
	void resize(int a_windowW, int a_windowH);

	// resize to panel
	virtual void resizeToPanel(int a_width, int a_height) {}

	// get response type
	string getResponseType() { return m_responseType; }

	// set response type
	void setResponseType(string a_responseType) { m_responseType = a_responseType; }

	// get negative vertical offset
	int getNegVertOffset() { return m_negVertOffset; }

	// set resize by camera
	void setResizeByCamera(bool a_isResizeByCamera) { m_isResizeByCamera = a_isResizeByCamera; }

	// get resize by camera
	bool getResizeByCamera() { return m_isResizeByCamera; }

	// append to message
	void appendToMessage(string a_messageAddition, int a_windowW, int a_windowH);

	// set start condition
	void setStartCondition(bool a_startCondition) { m_startCondition = a_startCondition; }

	// get start condition
	bool getStartCondition() { return m_startCondition; }
    
protected:

	void initTutorialInstruction(double a_startTime, double a_minDuration, double a_maxDuration,
							string a_responseType = "", double a_fadeDuration = 1.0);


    //-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
    
	// message labels
	vector<cLabel*> m_messages;

	// is a message
	bool m_isMessage;

	// is resizable by camera frame
	bool m_isResizeByCamera;
    
	// start time (seconds)
	double m_startTime;

	// fade duration (seconds)
	double m_fadeDuration;

    // minimum duration
    double m_minDuration;

	// maximum duration
	double m_maxDuration;

	// world identifier (true for haptics, false for hq)
	bool m_hapticsWorld;

	// response type identifier
	string m_responseType;

	// negative vertical offset
	int m_negVertOffset;

	// start condition
	bool m_startCondition;
    
private:
    
    //-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
    
    void deallocate();
};

//---------------------------------------------------------------------------
#endif //RTSTUTORIALINSTRUCTIONH
//---------------------------------------------------------------------------
