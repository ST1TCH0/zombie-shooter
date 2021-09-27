#pragma once

#include "Device.h"

class Client : public Device{
public:
	Client(struct gameData *data) : Device(data) {};
	DWORD WINAPI clientTask(void* args);
}; 

struct clientTaskData {
	char addr[16];
	char nickname[16];
	Client* client;
};