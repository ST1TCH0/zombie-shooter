#include "Device.h"

Device::Device(struct gameData *data) {
	ready = 1;
	loaded = 0;
	role = 5;
	this->data = data;
}