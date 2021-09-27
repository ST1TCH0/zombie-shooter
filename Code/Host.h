#pragma once

#include "Device.h"
#include <time.h>
static DWORD WINAPI host_Run(LPVOID p);

class Host : public Device{
public:
	Host(struct gameData *data) : Device(data) {};
	DWORD WINAPI host(void* args);
	DWORD WINAPI hostTask(LPVOID args);
};

struct hostTaskData {
	SOCKET s;
	int role;
	Host* host;
};
