#pragma once

#include "chai3d.h"
#include <algorithm>
#include <vector>
#include <time.h>
#include <fstream>
#include <locale>
#include "NASATLX\rtsSlider.h"

using namespace chai3d;
using namespace std;

// target type is specific type of sphere object
class rtsNASATLX: public cGenericObject
{
public:

	// Constructor
	rtsNASATLX(cCamera* a_camera, int a_windowW, int a_windowH, string a_weightsOutputFilename = "weights.txt",
				string a_ratingsOutputFilename = "ratings.txt", string a_scoreOutputFilename = "score.txt");

	virtual ~rtsNASATLX();

	// start nasa tlx
	void start();

	// resize
	void resize(int a_w, int a_h);

	// update weights
	void updateWeights(bool a_right);

	// get running state
	bool getRunning() { return m_running; }

	// reset button color
	void resetButtonColor();

	// set button color to clicked color
	void setButtonClickColor();

	// get button label
	cLabel* getButtonLabel() { return m_buttonLabel; }

	// get left button label
	cLabel* getLeftButtonLabel() { return m_leftButtonLabel; }

	// get right button label
	cLabel* getRightButtonLabel() { return m_rightButtonLabel; }

	void updateButtons(cCamera* a_camera, int windowW, int windowH, int x, int y, bool downClick);

	void updateSliders(int x, int y);

	void setShowEnabledWeights(bool a_show);
	void setShowEnabledRatings(bool a_show);

	void recordWeights();
	void recordRatings();
	void recordWeightedScore();

	bool finished() { return m_finished; }

private:

	// delete and point NULL all children
	void deallocate();

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------
	
	// tlx running
	bool m_running;

	// tlx finished
	bool m_finished;

	// weight portion
	bool m_weightsPortion;

	// rating portion
	bool m_ratingPortion;

	// weight combinations
	vector<vector<int>> m_weightCombinations;

	// current weight index
	int m_weightIndex;

	// vector of weights for each factor
	vector<int> m_weights;

	// vector of ratings for each factor
	vector<int> m_ratings;

	// list of factors
	vector<string> m_factors;

	// Graphics

	// main panel
	cPanel* m_panel;

	// submit button
	cLabel* m_buttonLabel;

	// left factor button
	cLabel* m_leftButtonLabel;

	// right factor
	cLabel* m_rightButtonLabel;

	cLabel* m_warningLabel;
	cLabel* m_instructionLabel;
	cLabel* m_weightCountLabel;
	cLabel* m_orLabel;
	bool m_rightClicked;
	bool m_leftClicked;
	cColorf m_buttonClickedColor;
	cColorf m_buttonDefaultColor;

	vector<rtsSlider*> m_sliders;
	int m_contactedSlider;
	bool m_sliderIsContacted;

	int m_windowW;
	int m_windowH;

	string m_weightsOutputFilename;
	string m_ratingsOutputFilename;
	string m_scoreOutputFilename;
};

