//---------------------------------------------------------------------------
#include "rtsDemoInstruction.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
 Constructor of rtsDemoInstruction.
 
 \fn     rtsDemoInstruction::rtsDemoInstruction()
 \param
 */
//===========================================================================

//rtsDemoInstruction::rtsDemoInstruction(double a_startTime, cVector3d a_localPos, string a_responseType, double a_fadeDuration)
//{
//	initDemoInstruction(a_startTime, a_fadeDuration);
//}

rtsDemoInstruction::rtsDemoInstruction(double a_startTime, double a_minDuration, double a_maxDuration, 
										cVector3d a_localPos, string a_responseType, double a_fadeDuration)
{
	initDemoInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);
}

rtsDemoInstruction::rtsDemoInstruction(double a_startTime, double a_minDuration, double a_maxDuration, string a_messageText, int a_windowW, int a_windowH, 
										string a_responseType, int a_negVertOffset, double a_fadeDuration)
{
	m_negVertOffset = a_negVertOffset;
	initDemoInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);

	// create message label
	cFont* font = NEW_CFONTCALIBRI40();
	cLabel* message = new cLabel(font);
	message->setText(a_messageText);
	message->m_fontColor.setGrayLevel(0.2f);
	message->setLocalPos((int)(0.5 * (a_windowW - message->getWidth())), a_windowH - a_negVertOffset);	
	m_messages.push_back(message);

	addChild(message);
	m_isMessage = true;
}

rtsDemoInstruction::rtsDemoInstruction(double a_startTime, double a_minDuration, double a_maxDuration, vector<string> a_messages, int a_windowW, int a_windowH, 
										string a_responseType, int a_negVertOffset, double a_fadeDuration)
{
	m_negVertOffset = a_negVertOffset;
	initDemoInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);

	for (int i = 0; i < a_messages.size(); i++)
	{
		// create message label
		cFont* font = NEW_CFONTCALIBRI40();
		cLabel* message = new cLabel(font);
		message->setText(a_messages[i]);
		message->m_fontColor.setGrayLevel(0.2f);
		message->setLocalPos((int)(0.5 * (a_windowW - message->getWidth())), a_windowH - a_negVertOffset - i * message->getHeight());
		m_messages.push_back(message);

		addChild(message);
	}
	m_isMessage = true;
}

void rtsDemoInstruction::initDemoInstruction(double a_startTime, double a_minDuration, double a_maxDuration,
											string a_responseType, double a_fadeDuration)
{
	setStartTime(a_startTime);
	setMinDuration(a_minDuration);
	setMaxDuration(a_maxDuration);
	setResponseType(a_responseType);
	setFadeDuration(a_fadeDuration);
	m_isMessage = false;
	m_isResizeByCamera = false;
	m_startCondition = false;
}

rtsDemoInstruction::~rtsDemoInstruction()
{
    // deallocate memory
    deallocate();
}

void rtsDemoInstruction::setMinDuration(double a_minDuration)
{
    m_minDuration = a_minDuration;
}

void rtsDemoInstruction::setMaxDuration(double a_maxDuration)
{
	m_maxDuration = a_maxDuration;
}

void rtsDemoInstruction::appendToMessage(string a_messageAddition, int a_windowW, int a_windowH)
{
	if (m_isMessage)
	{
		// create message label
		cFont* font = NEW_CFONTCALIBRI40();
		cLabel* message = new cLabel(font);
		message->setText(a_messageAddition);
		message->m_fontColor.setGrayLevel(0.2f);
		message->setLocalPos((int)(0.5 * (a_windowW - message->getWidth())), a_windowH - m_negVertOffset - (m_messages.size()) * message->getHeight());
		m_messages.push_back(message);

		addChild(message);
	}
}


void rtsDemoInstruction::resizeWithHandedness(int a_windowW, int a_windowH, bool a_rightHanded)
{
	if (m_isMessage)
	{
		for (int i = 0; i < m_messages.size(); i++)
		{
			if (a_rightHanded)
			{
				m_messages[i]->setLocalPos((int)(0.5 * (a_windowW - m_messages[i]->getWidth())), a_windowH - m_negVertOffset - i * m_messages[i]->getHeight() - 350);
			}
			else
			{
				m_messages[i]->setLocalPos(672 + (int)(0.5 * (a_windowW - m_messages[i]->getWidth())), a_windowH - m_negVertOffset - i * m_messages[i]->getHeight() - 350);
			} // hcval
		}
	}
}

void rtsDemoInstruction::resize(int a_windowW, int a_windowH)
{
	if (m_isMessage)
	{
		for (int i = 0; i < m_messages.size(); i++)
		{
			m_messages[i]->setLocalPos((int)(0.5 * (a_windowW - m_messages[i]->getWidth())), a_windowH - m_negVertOffset - i * m_messages[i]->getHeight());
		}
	}
}

void rtsDemoInstruction::deallocate()
{
	if (m_isMessage)
	{
		int numMessages = m_messages.size();
		for (int i = 0; i < numMessages; i++)
		{
			removeChild(m_messages[i]);

			delete m_messages[i];
			m_messages[i] = NULL;
		}
		m_messages.clear();
	}
}