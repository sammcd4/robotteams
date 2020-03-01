#include "rtsSlider.h"

//! Constructor of cScope.
rtsSlider::rtsSlider(int a_width, int a_height, string a_topLeftCornerMessage, string a_topRightCornerMessage, string a_bottomLeftCornerMessage,
													string a_bottomRightCornerMessage)
{
	// enclosing panel
	m_panel = new cPanel();
	m_panel->setLocalPos(0, 0);
	m_panel->setSize(a_width, a_height);
	cColorf panelColor;
	panelColor.setGray();
	m_panel->setCornerColors(panelColor, panelColor, panelColor, panelColor);
	m_panel->setCornerRadius(3, 3, 3, 3);
	addChild(m_panel);

	// line
	m_line = new cShapeLine(cVector3d(0, a_height / 2, 0), cVector3d(a_width, a_height / 2, 0));
	m_line->setLocalPos(0, 0, 0);
	m_line->setLineWidth(2.5);
	m_line->m_colorPointA.setBlack();
	m_line->m_colorPointB.setBlack();
	m_panel->addChild(m_line);

	// knob
	m_knob = new cPanel();
	m_knob->setSize(a_width / 50, a_height / 3);
	m_knob->setLocalPos(0, a_height / 2 - m_knob->getHeight() / 2, 0);
	cColorf knobColor;
	knobColor.setBlack();
	m_knob->setCornerColors(knobColor, knobColor, knobColor, knobColor);
	m_knob->setCornerRadius(10, 10, 10, 10);
	m_panel->addChild(m_knob);

	m_val = 0.0;

	// labels on each side
	cFont *font = NEW_CFONTCALIBRI32();

	cColorf labelColor;
	labelColor.setBlack();

	m_topLeftCornerLabel = new cLabel(font);
	m_topLeftCornerLabel->setText(a_topLeftCornerMessage);
	m_topLeftCornerLabel->m_fontColor = labelColor;
	m_panel->addChild(m_topLeftCornerLabel);

	m_topRightCornerLabel = new cLabel(font);
	m_topRightCornerLabel->setText(to_string(0));
	m_topRightCornerLabel->m_fontColor = labelColor;
	m_panel->addChild(m_topRightCornerLabel);

	m_bottomLeftCornerLabel = new cLabel(font);
	m_bottomLeftCornerLabel->setText(a_bottomLeftCornerMessage);
	m_bottomLeftCornerLabel->m_fontColor = labelColor;
	m_panel->addChild(m_bottomLeftCornerLabel);

	m_bottomRightCornerLabel = new cLabel(font);
	m_bottomRightCornerLabel->setText(a_bottomRightCornerMessage);
	m_bottomRightCornerLabel->m_fontColor = labelColor;
	m_panel->addChild(m_bottomRightCornerLabel);

	resize();
}

void rtsSlider::resize()
{
	m_line->m_pointA = cVector3d(0, m_panel->getHeight() / 2, 0);
	m_line->m_pointB = cVector3d(m_panel->getWidth(), m_panel->getHeight() / 2, 0);
	m_knob->setLocalPos(m_val - m_knob->getWidth() / 2, m_panel->getHeight() / 2 - m_knob->getHeight() / 2, 0);
	m_topLeftCornerLabel->setLocalPos(0, m_panel->getHeight() - m_topLeftCornerLabel->getHeight());
	m_topRightCornerLabel->setLocalPos(m_panel->getWidth() - m_topRightCornerLabel->getWidth(), m_panel->getHeight() - m_topRightCornerLabel->getHeight());
	m_bottomLeftCornerLabel->setLocalPos(0, 0);
	m_bottomRightCornerLabel->setLocalPos(m_panel->getWidth() - m_bottomRightCornerLabel->getWidth(), 0);
}

void rtsSlider::setValue(double a_val)
{
	m_val = cClamp(a_val*m_panel->getWidth(), 0.0, m_panel->getWidth());
	m_knob->setLocalPos(m_val - m_knob->getWidth() / 2, m_panel->getHeight() / 2 - m_knob->getHeight() / 2, 0);
	m_topRightCornerLabel->setText(to_string(int(getValue())));
	m_topRightCornerLabel->setLocalPos(m_panel->getWidth() - m_topRightCornerLabel->getWidth(), m_panel->getHeight() - m_topRightCornerLabel->getHeight());
}

double rtsSlider::getValue()
{
	return m_val / getWidth() * 100.0;
}

cShapeLine* rtsSlider::getLine()
{
	return m_line;
}

//! Destructor of cScope.
rtsSlider::~rtsSlider() 
{
	deallocate();
}

void rtsSlider::deallocate()
{
	// line
	m_panel->removeChild(m_line);
	delete m_line;
	m_line = NULL;

	// knob
	m_panel->removeChild(m_knob);
	delete m_knob;
	m_knob = NULL;

	// top left label
	m_panel->removeChild(m_topLeftCornerLabel);
	delete m_topLeftCornerLabel;
	m_topLeftCornerLabel = NULL;

	// top right label
	m_panel->removeChild(m_topRightCornerLabel);
	delete m_topRightCornerLabel;
	m_topRightCornerLabel = NULL;

	// bottom left label
	m_panel->removeChild(m_bottomLeftCornerLabel);
	delete m_bottomLeftCornerLabel;
	m_bottomLeftCornerLabel = NULL;

	// bottom right label
	m_panel->removeChild(m_bottomRightCornerLabel);
	delete m_bottomRightCornerLabel;
	m_bottomRightCornerLabel = NULL;

	// panel
	removeChild(m_panel);
	delete m_panel;
	m_panel = NULL;
}