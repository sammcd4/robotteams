#include "comm.h"

#include <stdint.h>
#include <string>

#include "agent.pb.h"
#include "command.pb.h"
#include "gravity.pb.h"

communicator::communicator(std::string hostname) {
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	// Initialize WinSock
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (err == 0) {
		startup_success = true;
	} else {
		startup_success = false;
	}

	this->command = new connection(hostname, COMMAND_PORT);
	this->query = new connection(hostname, QUERY_PORT);
	this->sim = new connection(hostname, SIM_PORT);
}

communicator::~communicator() {
	google::protobuf::ShutdownProtobufLibrary();
	WSACleanup();
}

bool communicator::is_valid() {
	return startup_success && command->is_valid() && query->is_valid() && sim->is_valid();
}

void communicator::pause() {
	// Build the pause command.
	comm::SimCommand command;
	command.set_type(comm::SimCommand_Type_PAUSE);

	// Send the pause command.
	this->sim->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->sim->read_frame(len);
}

void communicator::unpause() {
	// Build the pause command.
	comm::SimCommand command;
	command.set_type(comm::SimCommand_Type_UNPAUSE);

	// Send the pause command.
	this->sim->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->sim->read_frame(len);
}

double communicator::fps() {
	// Build the FPS query.
	comm::SimCommand command;
	command.set_type(comm::SimCommand_Type_FPS);

	// Send the FPS query.
	this->sim->write_message(&command);

	// Get (and return) the response.
	uint64_t len;
	char* response = this->sim->read_frame(len);

	comm::FPS* message = new comm::FPS();
	bool success = message->ParseFromArray(response, (int) len);

	if (success) {
		return message->frames();
	} else {
		return 0;
	}
}

void communicator::quit() {
	// Build the quit command.
	comm::SimCommand command;
	command.set_type(comm::SimCommand_Type_QUIT);

	// Send the quit command.
	this->sim->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->sim->read_frame(len);
}

comm::Swarm* communicator::swarm() {
	// Build the query.
	comm::Query query;
	query.set_type(comm::Query_Type_SWARM);

	// Send the query.
	this->query->write_message(&query);

	// Get the response.
	uint64_t len;
	char* response = this->query->read_frame(len);

	comm::Swarm* swarm = new comm::Swarm();
	bool success = swarm->ParseFromArray(response, (int) len);

	if (success) {
		return swarm;
	} else {
		return NULL;
	}
}

comm::PolygonList* communicator::polygons() {
	// Build the query.
	comm::Query query;
	query.set_type(comm::Query_Type_POLYGONS);

	// Send the query.
	this->query->write_message(&query);

	// Get the response.
	uint64_t len;
	char* response = this->query->read_frame(len);

	comm::PolygonList* polygonList = new comm::PolygonList();
	bool success = polygonList->ParseFromArray(response, (int) len);

	if (success) {
		return polygonList;
	} else {
		return NULL;
	}
}

comm::GravityList* communicator::get_gravity() {
	// Build the query.
	comm::Query query;
	query.set_type(comm::Query_Type_GRAVITY);

	// Send the query.
	this->query->write_message(&query);

	// Get the response.
	uint64_t len;
	char* response = this->query->read_frame(len);

	comm::GravityList* grav_list = new comm::GravityList();
	bool success = grav_list->ParseFromArray(response, (int) len);

	if (success) {
		return grav_list;
	} else {
		return NULL;
	}
}

void communicator::patrol(int priority, int id, double x, double y, double z) {
	// Build the vector.
	comm::Command_Vector* vector = new comm::Command_Vector();
	vector->set_x(x);
	vector->set_y(y);
	vector->set_z(z);

	// Build the travel command.
	comm::Command command;
		
	command.set_priority(priority);
	command.set_id(id);

	command.set_travel(true);
	command.set_allocated_heading(vector);

	// Send the travel command.
	this->command->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->command->read_frame(len);
}

void communicator::radius(int priority, int id, double radius)
{
	// Build the radius command.
	comm::Command command;

	command.set_priority(priority);
	command.set_id(id);

	command.set_minradius(true);
	command.set_minradiusvalue(radius);

	// Send the radius command.
	this->command->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->command->read_frame(len);
}

void communicator::add_gravity(double x, double y, double z, double sigma, double strength) {
	// Build the reset command.
	comm::Command command;

	// TODO: eventually this command should be given to a single agent.
	command.set_priority(0);
	command.set_id(0);

	command.set_gravity(true);

	comm::Command_Vector* location = new comm::Command_Vector();

	location->set_x(x);
	location->set_y(y);
	location->set_z(z);
	
	command.set_allocated_gravitylocation(location);
	command.set_gravitystrength(strength);
	command.set_gravitysigma(sigma);
	
	// Send the radius command.
	this->command->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->command->read_frame(len);
}

void communicator::reset_gravity() {
	// Build the reset command.
	comm::Command command;

	// TODO: eventually this command should be given to a single agent.
	command.set_priority(0);
	command.set_id(0);

	command.set_resetgravity(true);

	// Send the radius command.
	this->command->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->command->read_frame(len);
}

void communicator::set_force(int priority, int id, double x, double y, double z) {
	// Build the force-setting command.
	comm::Command command;

	command.set_priority(priority);
	command.set_id(id);

	command.set_userforce(true);

	// Build the vector.
	comm::Command_Vector* vector = new comm::Command_Vector();
	vector->set_x(x);
	vector->set_y(y);
	vector->set_z(z);

	command.set_allocated_forcevector(vector);

	// Send the commmand.
	this->command->write_message(&command);

	// Get (and ignore) the response.
	uint64_t len;
	this->command->read_frame(len);
}