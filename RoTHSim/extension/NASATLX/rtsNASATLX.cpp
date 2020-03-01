#include "rtsNASATLX.h"

rtsNASATLX::rtsNASATLX(cCamera* a_camera, int a_windowW, int a_windowH,
						string a_weightsOutputFilename, string a_ratingsOutputFilename, string a_scoreOutputFilename)
{
	// add tlx object as a child to camera front layer
	a_camera->m_frontLayer->addChild(this);

	m_weightsOutputFilename = a_weightsOutputFilename;
	m_ratingsOutputFilename = a_ratingsOutputFilename;
	m_scoreOutputFilename = a_scoreOutputFilename;

	// create list of factors
	m_factors.push_back("Mental Demand");
	m_factors.push_back("Physical Demand");
	m_factors.push_back("Temporal Demand");
	m_factors.push_back("Own Performance");
	m_factors.push_back("Effort");
	m_factors.push_back("Frustration");

	// create combinations
	srand(time(NULL));
	for (int i = 0; i < 6; i++)
	{
		for (int j = i+1; j < 6; j++)
		{
			int r = rand() % 2 + 1;
			if (r < 2)
			{
				vector<int> combo;
				combo.push_back(i);
				combo.push_back(j);
				m_weightCombinations.push_back(combo);
			}
			else
			{
				vector<int> combo;
				combo.push_back(j);
				combo.push_back(i);
				m_weightCombinations.push_back(combo);
			}
		}
	}

	//// print out combination in order
	//cout << "weight combinations in order\n";
	//for (int i = 0; i < m_weightCombinations.size(); i++)
	//{
	//	for (int j = 0; j < m_weightCombinations[i].size(); j++)
	//	{
	//		cout << m_weightCombinations[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	// randomize combinations
	random_shuffle(m_weightCombinations.begin(), m_weightCombinations.end());

	//// print out combination randomized
	//cout << "weight combinations randomized\n";
	//for (int i = 0; i < m_weightCombinations.size(); i++)
	//{
	//	for (int j = 0; j < m_weightCombinations[i].size(); j++)
	//	{
	//		cout << m_weightCombinations[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	// initialize weights to zero
	for (int i = 0; i < 6; i++)
		m_weights.push_back(0);

	// initialize ratings to zero
	for (int i = 0; i < 6; i++)
		m_ratings.push_back(0);

	// set current weight index
	m_weightIndex = 0;
	m_running = false;
	m_finished = false;
	m_weightsPortion = false;
	m_rightClicked = false;
	m_leftClicked = false;
	m_ratingPortion = false;


	// graphics
	m_panel = new cPanel();
	m_panel->setLocalPos(0, 0);
	m_panel->setSize(a_windowW, a_windowH);
	cColorf panelColor;
	panelColor.setGray();
	m_panel->setCornerColors(panelColor, panelColor, panelColor, panelColor);
	m_panel->setCornerRadius(3, 3, 3, 3);
	addChild(m_panel);


	// WEIGHTS

	// button label
	cFont *font = NEW_CFONTCALIBRI32();
	m_buttonLabel = new cLabel(font);
	m_buttonLabel->setText("Submit");
	m_buttonLabel->setLocalPos((a_windowW - m_buttonLabel->getTextWidth()) / 2, (a_windowH - m_buttonLabel->getTextHeight()) / 2);
	m_panel->addChild(m_buttonLabel);

	// left button label
	m_leftButtonLabel = new cLabel(font);
	m_leftButtonLabel->setText(m_factors[m_weightCombinations[m_weightIndex][0]]);
	m_leftButtonLabel->setLocalPos((a_windowW - m_leftButtonLabel->getTextWidth()) / 2 - 300, (a_windowH - m_leftButtonLabel->getTextHeight()) / 2 + 100);
	m_panel->addChild(m_leftButtonLabel);

	// right button label
	m_rightButtonLabel = new cLabel(font);
	m_rightButtonLabel->setText(m_factors[m_weightCombinations[m_weightIndex][1]]);
	m_rightButtonLabel->setLocalPos((a_windowW - m_rightButtonLabel->getTextWidth()) / 2 + 300, (a_windowH - m_rightButtonLabel->getTextHeight()) / 2 + 100);
	m_panel->addChild(m_rightButtonLabel);

	// OR label
	m_orLabel = new cLabel(font);
	m_orLabel->setText("OR");
	m_orLabel->m_fontColor.setBlack();
	m_orLabel->setLocalPos((a_windowW - m_orLabel->getTextWidth()) / 2, (a_windowH - m_orLabel->getTextHeight()) / 2 + 100);
	m_panel->addChild(m_orLabel);

	// instruction label
	m_instructionLabel = new cLabel(font);
	m_instructionLabel->setText("Please evaluate the previous task by selecting a rating for each scale below:");
	m_instructionLabel->m_fontColor.setBlack();
	m_instructionLabel->setLocalPos((a_windowW - m_instructionLabel->getTextWidth()) / 2, a_windowH - 2 * m_instructionLabel->getTextHeight());
	m_panel->addChild(m_instructionLabel);

	// weight count label
	m_weightCountLabel = new cLabel(font);
	m_weightCountLabel->setText("(1/15)");
	m_weightCountLabel->m_fontColor.setBlack();
	m_weightCountLabel->setLocalPos((a_windowW - m_weightCountLabel->getTextWidth()) / 2, a_windowH - 4 * m_weightCountLabel->getTextHeight());
	m_panel->addChild(m_weightCountLabel);

	// warning label
	m_warningLabel = new cLabel(font);
	m_warningLabel->setText("You must select an option to continue");
	m_warningLabel->m_fontColor.setRed();
	m_warningLabel->setLocalPos((a_windowW - m_warningLabel->getTextWidth()) / 2, (a_windowH - m_warningLabel->getTextHeight()) / 2 - 400);
	m_panel->addChild(m_warningLabel);

	cColorf buttonColor;
	buttonColor.setBlack();
	m_buttonDefaultColor = buttonColor;
	m_buttonLabel->m_fontColor = m_buttonDefaultColor;
	m_leftButtonLabel->m_fontColor = m_buttonDefaultColor;
	m_rightButtonLabel->m_fontColor = m_buttonDefaultColor;

	cColorf buttonClickColor;
	buttonClickColor.setBlue();
	m_buttonClickedColor = buttonClickColor;


	// RATING
	int numFactors = m_factors.size();
	for (int i = 0; i < numFactors; i++)
	{
		locale loc;
		string str = m_factors[i];
		string str2 = "";
		for (std::string::size_type i = 0; i<str.length(); ++i)
			str2 += toupper(str[i], loc);


		rtsSlider* slider;
		if (m_factors[i].compare("Own Performance"))
			slider = new rtsSlider(a_windowW*0.75, 100, str2, "", "Low", "High");
		else slider = new rtsSlider(a_windowW*0.75, 100, str2, "", "Good", "Bad");
		m_panel->addChild(slider);
		m_sliders.push_back(slider);
	}

	m_sliderIsContacted = false;

	// hide all widgets until tlx starts
	m_panel->setShowEnabled(false, true);


}

rtsNASATLX::~rtsNASATLX()
{
	deallocate();
}

void rtsNASATLX::deallocate()
{
	// button
	m_panel->removeChild(m_buttonLabel);
	delete m_buttonLabel;
	m_buttonLabel = NULL;

	// left button
	m_panel->removeChild(m_leftButtonLabel);
	delete m_leftButtonLabel;
	m_leftButtonLabel = NULL;

	// right button
	m_panel->removeChild(m_rightButtonLabel);
	delete m_rightButtonLabel;
	m_rightButtonLabel = NULL;

	// or label
	m_panel->removeChild(m_orLabel);
	delete m_orLabel;
	m_orLabel = NULL;

	// instruction label
	m_panel->removeChild(m_instructionLabel);
	delete m_instructionLabel;
	m_instructionLabel = NULL;

	// weight label
	m_panel->removeChild(m_weightCountLabel);
	delete m_weightCountLabel;
	m_weightCountLabel = NULL;

	// warning label
	m_panel->removeChild(m_warningLabel);
	delete m_warningLabel;
	m_warningLabel = NULL;

	// sliders
	int numSliders = m_sliders.size();
	for (int i = 0; i < numSliders; i++)
	{
		m_panel->removeChild(m_sliders[i]);
		delete m_sliders[i];
		m_sliders[i] = NULL;
	}
	m_sliders.clear();

	// panel
	removeChild(m_panel);
	delete m_panel;
	m_panel = NULL;
}

void rtsNASATLX::start()
{
	// hide all children in camera front layer other than nasa tlx
	this->getParent()->setShowEnabled(false, true);
	this->getParent()->setShowEnabled(true, false);
	m_panel->setShowEnabled(true, false);


	m_running = true;

	m_ratingPortion = true;
	m_weightsPortion = false;
	setShowEnabledWeights(false);
	setShowEnabledRatings(true);


	//m_ratingPortion = true;
	//if (m_ratingPortion)
	//{
	//	m_instructionLabel->setText("Please evaluate the previous task by selecting a rating for each scale below:");
	//	m_buttonLabel->setLocalPos((m_windowW - m_buttonLabel->getTextWidth()) / 2, 50);
	//}
	//setShowEnabledWeights(false);
	//setShowEnabledRatings(true);
}

void rtsNASATLX::setShowEnabledWeights(bool a_show)
{
	m_instructionLabel->setShowEnabled(a_show);

	m_buttonLabel->setShowEnabled(a_show);
	m_leftButtonLabel->setShowEnabled(a_show);
	m_rightButtonLabel->setShowEnabled(a_show);
	m_orLabel->setShowEnabled(a_show);
	m_weightCountLabel->setShowEnabled(a_show);

	// show all but warning label
	m_warningLabel->setShowEnabled(false);
}

void rtsNASATLX::setShowEnabledRatings(bool a_show)
{
	m_instructionLabel->setShowEnabled(a_show);
	m_buttonLabel->setShowEnabled(a_show);

	int numSliders = m_sliders.size();
	for (int i = 0; i < numSliders; i++)
	{
		m_sliders[i]->setShowEnabled(a_show);
	}
}

void rtsNASATLX::resize(int a_w, int a_h)
{
	m_windowW = a_w;
	m_windowH = a_h;

	m_panel->setLocalPos(0, 0);
	m_panel->setSize(a_w, a_h);

	if (m_weightsPortion)
	{
		m_buttonLabel->setLocalPos((a_w - m_buttonLabel->getTextWidth()) / 2, (a_h - m_buttonLabel->getTextHeight()) / 2);

		m_leftButtonLabel->setLocalPos((a_w - m_leftButtonLabel->getTextWidth()) / 2 - 200, (a_h - m_leftButtonLabel->getTextHeight()) / 2 + 50);
		m_rightButtonLabel->setLocalPos((a_w - m_rightButtonLabel->getTextWidth()) / 2 + 200, (a_h - m_rightButtonLabel->getTextHeight()) / 2 + 50);
		m_orLabel->setLocalPos((a_w - m_orLabel->getTextWidth()) / 2, (a_h - m_orLabel->getTextHeight()) / 2 + 50);
		m_instructionLabel->setLocalPos((a_w - m_instructionLabel->getTextWidth()) / 2, a_h - 2 * m_instructionLabel->getTextHeight());
		m_weightCountLabel->setLocalPos((a_w - m_weightCountLabel->getTextWidth()) / 2, a_h - 4 * m_weightCountLabel->getTextHeight());
	}
	else if (m_ratingPortion)
	{
		m_instructionLabel->setLocalPos((a_w - m_instructionLabel->getTextWidth()) / 2, a_h - 2 * m_instructionLabel->getTextHeight());
		m_buttonLabel->setLocalPos((m_windowW - m_buttonLabel->getTextWidth()) / 2, 50);	
		m_sliders[0]->setLocalPos((a_w - m_sliders[0]->getWidth()) / 2, 100);
		m_sliders[0]->resize();

		int numSliders = m_sliders.size();
		for (int i = 0; i < numSliders; i++)
		{
			m_sliders[i]->setLocalPos((a_w - m_sliders[i]->getWidth()) / 2, a_h - (m_sliders[i]->getHeight()+10)*(i + 1) - 100);
			m_sliders[i]->resize();
		}
	}

}

void rtsNASATLX::updateWeights(bool a_right)
{
	if (m_weightsPortion && m_weightIndex < m_weightCombinations.size())
	{
		// current factor choices
		int left = m_weightCombinations[m_weightIndex][0];
		int right = m_weightCombinations[m_weightIndex][1];

		if (a_right)
		{
			m_weights[right]++;
		}
		else
		{
			m_weights[left]++;
		}
		m_weightIndex++;
		bool end = false;
		if (m_weightIndex > 15 - 1)
		{
			m_weightIndex = 15 - 1;
			end = true;
		}
		string text = "(" + to_string(m_weightIndex+1) + "/15)";
		m_weightCountLabel->setText(text);

		m_leftButtonLabel->setText(m_factors[m_weightCombinations[m_weightIndex][0]]);
		m_rightButtonLabel->setText(m_factors[m_weightCombinations[m_weightIndex][1]]);

		if (end)
		{
			m_weightIndex++;
			m_weightsPortion = false;
			//m_ratingPortion = true;

			recordWeights();

			//m_instructionLabel->setText("Please evaluate the previous task by selecting a rating for each scale below:");
			resize(m_windowW, m_windowH);
			//m_buttonLabel->setLocalPos((m_windowW - m_buttonLabel->getTextWidth()) / 2, 50);
			setShowEnabledWeights(false);
			//setShowEnabledRatings(true);

			// weights last
			m_running = false;
			m_finished = true;
			recordWeightedScore();
		}
	}
}

void rtsNASATLX::resetButtonColor()
{
	m_buttonLabel->m_fontColor = m_buttonDefaultColor;
}

void rtsNASATLX::setButtonClickColor()
{
	m_buttonLabel->m_fontColor = m_buttonClickedColor;
}

void rtsNASATLX::updateButtons(cCamera* a_camera, int windowW, int windowH, int x, int y, bool downClick)
{
	cCollisionRecorder recorder;
	cCollisionSettings settings;

	//flagMoveObject = false;

	// detect for any collision between mouse and front layer widgets
	bool hit = a_camera->selectFrontLayer(x, (windowH - y), windowW, windowH, recorder, settings);
	if (hit)
	{
		if (m_weightsPortion)
		{
			// reset label font color
			resetButtonColor();

			// check mouse selection
			if (recorder.m_nearestCollision.m_object == m_buttonLabel)
			{
				if (!downClick)
				{
					//m_buttonLabel->m_fontColor = m_buttonClickedColor;
					if (m_rightClicked)
					{
						updateWeights(m_rightClicked);
						m_rightButtonLabel->m_fontColor = m_buttonDefaultColor;
						m_leftButtonLabel->m_fontColor = m_buttonDefaultColor;
						m_leftClicked = false;
						m_rightClicked = false;
					}
					else if (m_leftClicked)
					{
						updateWeights(!m_leftClicked);
						m_rightButtonLabel->m_fontColor = m_buttonDefaultColor;
						m_leftButtonLabel->m_fontColor = m_buttonDefaultColor;
						m_leftClicked = false;
						m_rightClicked = false;
					}
					else
					{
						m_warningLabel->setShowEnabled(true, true);
					}
				}
			}
			else if (recorder.m_nearestCollision.m_object == m_leftButtonLabel)
			{
				if (!downClick)
				{
					m_warningLabel->setShowEnabled(false, true);

					m_leftButtonLabel->m_fontColor = m_buttonClickedColor;
					m_rightButtonLabel->m_fontColor = m_buttonDefaultColor;
					m_leftClicked = true;
					m_rightClicked = false;
				}
			}
			else if (recorder.m_nearestCollision.m_object == m_rightButtonLabel)
			{
				if (!downClick)
				{
					m_warningLabel->setShowEnabled(false, true);

					m_rightButtonLabel->m_fontColor = m_buttonClickedColor;
					m_leftButtonLabel->m_fontColor = m_buttonDefaultColor;
					m_rightClicked = true;
					m_leftClicked = false;
				}
			}
		}
		else if (m_ratingPortion)
		{
			if (recorder.m_nearestCollision.m_object == m_buttonLabel)
			{
				if (!downClick)
				{
					recordRatings();
					setShowEnabledRatings(false);
					//resize(10, 10); // get these items out of the way
					int numSliders = m_sliders.size();
					for (int i = 0; i < numSliders; i++)
					{
						m_sliders[i]->setLocalPos(1000,1000);
						m_sliders[i]->resize();
					}
					m_ratingPortion = false;

					m_instructionLabel->setText("Please select the item that in your opinion contributes the most to Workload.");
					m_weightsPortion = true;
					resize(m_windowW, m_windowH);
					setShowEnabledWeights(true);
					//recordWeightedScore();

					//m_running = false;
					//m_finished = true;
				}
			}
			else
			{
				int numSliders = m_sliders.size();
				for (int i = 0; i < numSliders; i++)
				{
					if (recorder.m_nearestCollision.m_object == m_sliders[i]->getLine() ||
						recorder.m_nearestCollision.m_object == m_sliders[i]->getKnob())
					{
						if (downClick)
						{
							m_contactedSlider = i;
							m_sliderIsContacted = true;
							updateSliders(x, y);
							break;
						}
					}
				}

				if (!downClick)
					m_sliderIsContacted = false;
			}
		}
	}
}

void rtsNASATLX::updateSliders(int x, int y)
{
	if (m_sliderIsContacted)
	{
		double val = (x - m_sliders[m_contactedSlider]->getLocalPos().x()) / m_sliders[m_contactedSlider]->getWidth();
		m_sliders[m_contactedSlider]->setValue(val);
	}
}

void rtsNASATLX::recordWeights()
{
	ofstream fout(m_weightsOutputFilename.c_str());
	int numWeights = m_weights.size();
	for (int i = 0; i < numWeights; i++)
	{
		fout << m_weights[i] << endl;
	}
	fout.close();
}

void rtsNASATLX::recordRatings()
{
	// retrieve ratings for sliders
	ofstream fout(m_ratingsOutputFilename.c_str());
	int numRatings = m_ratings.size();
	for (int i = 0; i < numRatings; i++)
	{
		m_ratings[i] = m_sliders[i]->getValue();
		fout << m_ratings[i] << endl;
	}
	fout.close();
}

void rtsNASATLX::recordWeightedScore()
{
	// compute overall workload score
	ofstream fout(m_scoreOutputFilename.c_str());
	double score = 0;
	int numRatings = m_ratings.size();
	for (int i = 0; i < numRatings; i++)
	{
		score += m_ratings[i] * m_weights[i];
	}
	score /= 15.0;
	fout << score;
	fout.close();
}
