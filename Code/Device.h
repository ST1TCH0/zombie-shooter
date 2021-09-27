#pragma once
#include "stdafx.h"

#include	<Windows.h>
#include	<winsock.h>
#include	<vector>
#include	<mutex>


#include "Player.h"
#include "Enemy.h"

struct gameData {
	std::vector<Enemy> enemies;
	std::vector<Player> players;
	int score;
};

struct senddata {
	int dir, id;
	float rot, x, y, health;
	bool is_dead, click;
	char n[16];
};

struct vect {
	float x;
	float y;
};

class Device {
protected:
	bool ready, loaded;
	int role;
	std::mutex mtx;
	struct gameData* data;
	Player me;
public:
	Device(struct gameData* data);

	virtual bool getReady() { return ready; };
	virtual bool getLoaded() { return loaded; };
	virtual int getRole() { return role; };
	virtual std::mutex* getMtx() { return &mtx; };
	virtual struct gameData* getData() { return data; }
	virtual Player getMe() { return me; }

	virtual void setReady(bool ready) { this->ready = ready; }
	virtual void setLoaded(bool loaded) { this->loaded = loaded; }
	virtual void setRole(int role) { this->role = role; }
	virtual void setMe(Player me) { this->me = me; }
};