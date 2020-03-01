#pragma once

#include "chai3d.h"
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace chai3d;
using namespace std;

struct rtsSignalDataPair
{
	double time;
	double signalVal;
};

struct rtsSignalData
{
	vector<rtsSignalDataPair> data;
};

struct rtsMessagePair
{
	double time;
	string message;
};

class rtsMessageFeed
{
public:

	// Constructor
	rtsMessageFeed(string a_messageInputFile);

	// Destructor
	~rtsMessageFeed();

	// get next message pair (time and message) if time is reached
	rtsMessagePair* getNextMessagePair(double a_simTime);

	// check if there has been a change in message data
	bool nextMessageReached() { return m_nextMessageReached; }

private:

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// input file stream object for message data
	ifstream* m_finM;

	// hq message data
	vector<rtsMessagePair*> m_messages;

	// message index
	int m_indexM;

	// next message reached flag
	bool m_nextMessageReached;
};

