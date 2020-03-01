#include "rtsNASATLXDefinitions.h"

rtsNASATLXDefinitions::rtsNASATLXDefinitions(cCamera* a_camera, int a_windowW, int a_windowH)
{
	// add tlx object as a child to camera front layer
	a_camera->m_frontLayer->addChild(this);



	m_running = false;
	m_finished = false;

	// RATING DEFINITIONS

	cColorf panelColor;
	panelColor.setGray();
	m_panel2 = new cPanel();
	m_panel2->setLocalPos(0, 0);
	m_panel2->setSize(a_windowW, a_windowH);
	m_panel2->setCornerColors(panelColor, panelColor, panelColor, panelColor);
	m_panel2->setCornerRadius(3, 3, 3, 3);
	a_camera->m_frontLayer->addChild(m_panel2);

	// mental demand
	vector<cLabel*> mdLabels;
	cFont *font = NEW_CFONTCALIBRI32();
	cLabel* md0 = new cLabel(font);
	md0->setText("MENTAL DEMAND");
	m_panel2->addChild(md0);
	mdLabels.push_back(md0);
	cLabel* md1 = new cLabel(font);
	md1->setText("How much mental and perceptual activity was required (e.g. thinking, deciding, calculating, remembering,");
	m_panel2->addChild(md1);
	mdLabels.push_back(md1);
	cLabel* md2 = new cLabel(font);
	md2->setText("looking, searching, etc.)? Was the task easy or demanding, simple or complex, exacting or forgiving?");
	m_panel2->addChild(md2);
	mdLabels.push_back(md2);
	m_scaleDefs.push_back(mdLabels);

	// physical demand
	vector<cLabel*> pdLabels;
	cLabel* pd0 = new cLabel(font);
	pd0->setText("PHYSICAL DEMAND");
	pdLabels.push_back(pd0);
	m_panel2->addChild(pd0);
	cLabel* pd1 = new cLabel(font);
	pd1->setText("How much physical activity was required (e.g. pushing, pulling, turning, controlling,");
	m_panel2->addChild(pd1);
	pdLabels.push_back(pd1);
	cLabel* pd2 = new cLabel(font);
	pd2->setText("activating, etc.)? Was the task easy or demanding, slow or brisk, slack or strenuous?");
	m_panel2->addChild(pd2);
	pdLabels.push_back(pd2);
	m_scaleDefs.push_back(pdLabels);

	// temporal demand
	vector<cLabel*> tdLabels;
	cLabel* td0 = new cLabel(font);
	td0->setText("TEMPORAL DEMAND");
	m_panel2->addChild(td0);
	tdLabels.push_back(td0);
	cLabel* td1 = new cLabel(font);
	td1->setText("How much time pressure did you feel due to the rate or pace at which the tasks occurred?");
	m_panel2->addChild(td1);
	tdLabels.push_back(td1);
	cLabel* td2 = new cLabel(font);
	td2->setText("Was the pace slow and leisurely or rapid and frantic?");
	m_panel2->addChild(td2);
	tdLabels.push_back(td2);
	m_scaleDefs.push_back(tdLabels);

	// own performance
	vector<cLabel*> pLabels;
	cLabel* p0 = new cLabel(font);
	p0->setText("OWN PERFORMANCE");
	m_panel2->addChild(p0);
	pLabels.push_back(p0);
	cLabel* p1 = new cLabel(font);
	p1->setText("How successful do you think you were in accomplishing the goals of the task set?");
	m_panel2->addChild(p1);
	pLabels.push_back(p1);
	cLabel* p2 = new cLabel(font);
	p2->setText("How satisfied were you with your performance in accomplishing these goals?");
	m_panel2->addChild(p2);
	pLabels.push_back(p2);
	m_scaleDefs.push_back(pLabels);

	// effort
	vector<cLabel*> eLabels;
	cLabel* e0 = new cLabel(font);
	e0->setText("EFFORT");
	m_panel2->addChild(e0);
	eLabels.push_back(e0);
	cLabel* e1 = new cLabel(font);
	e1->setText("How hard did you have to work (mentally and physically) to accomplish your level of performance?");
	m_panel2->addChild(e1);
	eLabels.push_back(e1);
	//cLabel* e2 = new cLabel(font);
	//e2->setText("activating, etc.)? Was the task easy or demanding, slow or brisk, slack or strenuous?");
	//eLabels.push_back(e2);
	m_scaleDefs.push_back(eLabels);

	// frustration level
	vector<cLabel*> flLabels;
	cLabel* fl0 = new cLabel(font);
	fl0->setText("FRUSTRATION");
	m_panel2->addChild(fl0);
	flLabels.push_back(fl0);
	cLabel* fl1 = new cLabel(font);
	fl1->setText("How insecure, discouraged, irritated, stressed and annoyed versus secure, gratified, content,");
	m_panel2->addChild(fl1);
	flLabels.push_back(fl1);
	cLabel* fl2 = new cLabel(font);
	fl2->setText("relaxed and complacent did you feel during the task?");
	m_panel2->addChild(fl2);
	flLabels.push_back(fl2);
	m_scaleDefs.push_back(flLabels);

	// hide all widgets until tlx starts
	m_panel2->setShowEnabled(false, true);
}

rtsNASATLXDefinitions::~rtsNASATLXDefinitions()
{
	deallocate();
}

void rtsNASATLXDefinitions::deallocate()
{
	// definitions
	int numDefs = m_scaleDefs.size();
	for (int i = 0; i < numDefs; i++)
	{
		int numLabels = m_scaleDefs[i].size();
		for (int j = 0; j < numLabels; j++)
		{
			m_panel2->removeChild(m_scaleDefs[i][j]);
			delete m_scaleDefs[i][j];
			m_scaleDefs[i][j] = NULL;
		}
		m_scaleDefs[i].clear();
	}
	m_scaleDefs.clear();

	// panel
	removeChild(m_panel2);
	delete m_panel2;
	m_panel2 = NULL;
}

void rtsNASATLXDefinitions::start()
{
	// hide all children in camera front layer other than nasa tlx
	this->getParent()->setShowEnabled(false, true);
	this->getParent()->setShowEnabled(true, false);
	m_panel2->setShowEnabled(true, true);

	m_running = true;
}

void rtsNASATLXDefinitions::resize(int a_w, int a_h)
{
	m_windowW = a_w;
	m_windowH = a_h;

	m_panel2->setLocalPos(0, 0);
	m_panel2->setSize(a_w, a_h);

	// definitions on other screen
	int numDefs = m_scaleDefs.size();
	for (int i = 0; i < numDefs; i++)
	{
		int numLabels = m_scaleDefs[i].size();
		for (int j = 0; j < numLabels; j++)
		{
			m_scaleDefs[i][j]->m_fontColor.setBlack();
			if (j == 0)
			{
				m_scaleDefs[i][j]->setLocalPos(40, a_h - 100*(i + 1) - 30);
			}
			else
			{
				m_scaleDefs[i][j]->setLocalPos(a_w / 4, a_h - 100*(i + 1) - m_scaleDefs[i][j]->getHeight()*(j - 1) - 30);
			}
		}
	}
}