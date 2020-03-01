#pragma once

#include <string>

#include "connection.h"

#include "swarm.pb.h"
#include "polygon.pb.h"
#include "gravity.pb.h"

static const std::string COMMAND_PORT = "8080";
static const std::string QUERY_PORT = "8081";
static const std::string SIM_PORT = "8082";

class communicator {
public:
	communicator(std::string hostname);
	~communicator();

	bool is_valid();

	void pause();
	void unpause();
	
	double fps();

	void quit();

	comm::Swarm* swarm();
	comm::PolygonList* polygons();
	
	void patrol(int priority, int id, double x, double y, double z);
	void radius(int priority, int id, double radius);

	void add_gravity(double x, double y, double z, double sigma, double strength);

	comm::GravityList* get_gravity();

	void reset_gravity();

	void set_force(int priority, int id, double x, double y, double z);

private:
	bool startup_success;

	connection* command;
	connection* query;
	connection* sim;
};