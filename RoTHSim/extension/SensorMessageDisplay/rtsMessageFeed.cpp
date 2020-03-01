#include "rtsMessageFeed.h"

rtsMessageFeed::rtsMessageFeed(string a_messageInputFile)
{
	// read in message data from file
	m_finM = new ifstream(a_messageInputFile.c_str());

	// read in message data
	string startDelimeter = "< ";
	string endDelimeter = ">";
	string delimeter = " ";
	while (!m_finM->eof())
	{
		rtsMessagePair* messagePair = new rtsMessagePair();
		bool readingMessage = false;
		double t;
		string s;
		string messageString;
		*(m_finM) >> t;
		//int peeked = m_finM->peek();
		while (startDelimeter.find(m_finM->peek()) != string::npos)
		{
			m_finM->get();
			readingMessage = true;
		}

		while (readingMessage)
		{
			if (delimeter.find(m_finM->peek()) != string::npos)
			{
				m_finM->get();
			}
			else if (endDelimeter.find(m_finM->peek()) != string::npos)
			{
				m_finM->get();
				readingMessage = false;
			}
			else
			{
				*(m_finM) >> s;
				messageString += s + " ";
			}
		}
		messagePair->time = t;
		messagePair->message = messageString;

		if (!m_finM->eof())
		{
			m_messages.push_back(messagePair);
		}
	}

	m_indexM = 0;
	m_nextMessageReached = false;
}

rtsMessageFeed::~rtsMessageFeed()
{

}

rtsMessagePair* rtsMessageFeed::getNextMessagePair(double a_simTime)
{
	rtsMessagePair* nextMessage;
	m_nextMessageReached = false;

	if (m_indexM < (int)(m_messages.size()))
	{
		rtsMessagePair* currentPair = m_messages[m_indexM];
		nextMessage = currentPair;

		if (currentPair->time < a_simTime)
		{
			m_indexM++;
			m_nextMessageReached = true;
		}
	}
	else
	{
		// last message in list
		nextMessage = m_messages.back();
	}

	return nextMessage;
}
