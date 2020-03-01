#include "CDataManager.h"

cDataManager::cDataManager(string a_inputFile, string a_outputFile, string a_eventOutputFile)
{
	// create output file stream
	m_fin = new ifstream(a_inputFile.c_str());

	// create output file stream
	m_fout = new ofstream(a_outputFile.c_str());

	// create output file stream
	m_eventFout = new ofstream(a_eventOutputFile.c_str());

	// set a default sample rate
	m_sampleRate = 1.0;

	// reset timer
	m_timer.reset();
	m_timer.stop();
}

cDataManager::~cDataManager()
{
	m_fout->close();
	m_eventFout->close();
}

bool cDataManager::collectAndWriteData(cVector3d a_cursorPos,
							   cVector3d a_feedbackForce,
							   int a_teamMode,
							   vector<cVector3d> a_robotPositions)
{

	if (!m_timer.on())
	{
		m_timer.start();
	}

	if ( m_timer.getCurrentTimeSeconds() >= m_sampleRate)
	{
		//std::cout << "data sampled at " << m_sampleRate << "sec" << std::endl;

		// stop timer
		m_timer.stop();

		// set separation width
		double width = 15;

		// write timestamp to file
		*m_fout << getCurrentTimeSeconds() << std::setw(width);

		// write cursor position to file
		*m_fout << a_cursorPos.x() << std::setw(width)
			<< a_cursorPos.y() << std::setw(width)
			<< a_cursorPos.z() << std::setw(width);

		// write haptic feedback force to file
		*m_fout << a_feedbackForce.x() << setw(width)
			<< a_feedbackForce.y() << setw(width)
			<< a_feedbackForce.z() << setw(width);

		// write team mode to file
		*m_fout << a_teamMode << setw(width);

		// write agent positions to file
		int numRobotPositions = a_robotPositions.size();
		for (int i = 0; i < numRobotPositions; ++i)
		{
			*m_fout << a_robotPositions[i].x() << std::setw(width)
				<< a_robotPositions[i].y() << std::setw(width)
				<< a_robotPositions[i].z() << std::setw(width);
		}

		// next line
		*m_fout << std::endl;

		// start timer again
		m_timer.reset();
		m_timer.start();

		// check ofstream failbit
		if (m_fout->fail())
			return false;
		else return true;
	}
	else return false;
}

bool cDataManager::collectAndWriteEventData(bool a_manipButtonDown, bool a_manipButtonUp, bool a_manipButtonDoubleClick,
								bool a_manipSpacingButtonDown, bool a_manipSpacingButtonUp,
								bool a_commandPatrolButtonDown, bool a_commandPatrolButtonUp,
								bool a_resizeButtonDown, bool a_resizeButtonUp)
{
	// check if an event has occured
	if (a_manipButtonDown || a_manipButtonUp || a_manipButtonDoubleClick
		|| a_manipSpacingButtonDown || a_manipSpacingButtonUp
		|| a_commandPatrolButtonDown || a_commandPatrolButtonUp
		|| a_resizeButtonDown || a_resizeButtonUp)
	{
		// set separation width
		double width = 2;

		// write timestamp to file
		*m_eventFout << getCurrentTimeSeconds() << std::setw(width);

		// write manip button events
		*m_eventFout << int(a_manipButtonDown) << std::setw(width) << int(a_manipButtonUp) << std::setw(width) 
												<< int(a_manipButtonDoubleClick) << std::setw(width);

		// write manip spacing button events
		*m_eventFout << int(a_manipSpacingButtonDown) << std::setw(width) << int(a_manipSpacingButtonUp) << std::setw(width);

		// write command patrol button events
		*m_eventFout << int(a_commandPatrolButtonDown) << std::setw(width) << int(a_commandPatrolButtonUp) << std::setw(width);

		// write resize button events
		*m_eventFout << int(a_resizeButtonDown) << std::setw(width) << int(a_resizeButtonUp);

		// next line
		*m_eventFout << std::endl;

		// check ofstream failbit
		if (m_eventFout->fail())
			return false;
		else return true;
	}
	else return true;
}

cData cDataManager::collectData(cVector3d a_cursorPos,
								cVector3d a_feedbackForce,
								vector<cVector3d> a_robotPositions)
{
	cData data;

	if (!m_timer.on())
	{
		m_timer.start();
	}

	if (m_timer.getCurrentTimeSeconds() >= m_sampleRate)
	{
		//std::cout << "data sampled at " << m_sampleRate << "sec" << std::endl;

		// reset timer
		m_timer.reset();
		m_timer.stop();

		// store time data
		data.setTime(getCurrentTimeSeconds()); 

		// store cursor position
		data.setCursorPos(a_cursorPos);

		// store haptic feedback force
		data.setHapticForce(a_feedbackForce);

		// write agent positions to file
		data.setRobotPositions(a_robotPositions);

		// start timer again
		m_timer.start();

		// check ofstream failbit
		if (m_fout->fail())
			data.setValid(false);
		else data.setValid(true);
	}
	
	return data;
}

bool cDataManager::writeData(cData a_data)
{
	// set separation width
	double width = 15;

	// write timestamp to file
	*m_fout << a_data.getTime() << std::setw(width);

	// write cursor position to file
	*m_fout << a_data.getCursorPos().x() << std::setw(width)
		<< a_data.getCursorPos().y() << std::setw(width)
		<< a_data.getCursorPos().z() << std::setw(width);

	// write haptic feedback force to file
	*m_fout << a_data.getHapticForce().x() << setw(width)
		<< a_data.getHapticForce().y() << setw(width)
		<< a_data.getHapticForce().z() << setw(width);

	// write agent positions to file
	int numRobotPositions = a_data.getRobotPositions().size();
	for (int i = 0; i < numRobotPositions; ++i)
	{
		*m_fout << a_data.getRobotPositions()[i].x() << std::setw(width)
			<< a_data.getRobotPositions()[i].y() << std::setw(width)
			<< a_data.getRobotPositions()[i].z() << std::setw(width);
	}

	// next line
	*m_fout << std::endl;

	// check ofstream failbit
	if (m_fout->fail())
		return false;
	else return true;

}


string cDataManager::readUserInput(string a_message)
{
	std::cout << a_message;
	string input;
	std::cin >> input;
	return input;
}

std::ifstream* cDataManager::getFin()
{
	return m_fin;
}
