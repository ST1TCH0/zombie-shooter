#include "Host.h"

static DWORD WINAPI host_Run(LPVOID p)
{
	return ((Host*)((hostTaskData*) p)->host)->hostTask(p);
}


DWORD WINAPI Host::hostTask(LPVOID args) {
	struct hostTaskData* st = (struct hostTaskData*) args;
	struct senddata sd;
	std::string nn;
	int score, size, id=0;
	struct vect v;
	char n[16];
	clock_t start = clock();

	std::vector<Enemy> enemies;
	std::vector<Player> players;

	while (1) {
		
		start = clock();
		send(st->s, (char*)&score, sizeof(int), 0);
		recv(st->s, (char*)&sd, sizeof(int), 0);
		printf("testend: %d\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

		setReady(1);
		//while (st->host->getReady());

		printf("mtxstart: %d\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

		getMtx()->lock();
		enemies = data->enemies;
		players = data->players;
		score = data->score;
		getMtx()->unlock();

		printf("mtxend: %d\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		
		send(st->s, (char*)&score, sizeof(float), 0);

		size = enemies.size();
		send(st->s, (char*)&size, sizeof(int), 0);
		printf("fisrtsend: %d\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		for (int i = 0; i < size; i++) {
			v.x = enemies[i].getPosition().x;
			v.y = enemies[i].getPosition().y;
			send(st->s, (char*)&v, sizeof(vect), 0);
		}

		size = players.size();
		send(st->s, (char*)&size, sizeof(int), 0);
		send(st->s, (char*)&id, sizeof(int), 0);
		for (int i = 0; i < size; i++) {
			sd.x = players[i].getPosition().x;
			sd.y = players[i].getPosition().y;
			sd.dir = players[i].getDir();
			sd.rot = players[i].getRotation();
			nn = players[i].getNickname();
			std::copy(nn.begin(), nn.end(), sd.n);
			sd.n[nn.end() - nn.begin()] = '\0';
			sd.is_dead = players[i].isDead();
			sd.click = players[i].getClick();
			sd.health = players[i].getHealth();

			send(st->s, (char*)&sd, sizeof(senddata), 0);

			v.x = players[i].getAim().x;
			v.y = players[i].getAim().y;
			send(st->s, (char*)&v, sizeof(vect), 0);
		}
		printf("allsend: %d\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		setReady(1);

		recv(st->s, (char*)&sd, sizeof(senddata), 0);

		printf("recv: %d\n\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

		setReady(1);
		getMtx()->lock();
		data->players[st->role].setDir(sd.dir);
		data->players[st->role].setAim(Vector2f(sd.x, sd.y));
		data->players[st->role].setRotation(sd.rot);
		data->players[st->role].setClick(sd.click);
		printf("id: %i %i\n", sd.id, id);
		getMtx()->unlock();
		
		setLoaded(1);
		id++;
		printf("time: %d\n\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
	}
	return 0;
}

DWORD WINAPI Host::host(void* args) {
	int result, soc = 0;
	vect v;
	char n[16];

	SOCKET  s, so[3];
	WSADATA wsas;
	WORD wersja;
	wersja = MAKEWORD(1, 1);
	result = WSAStartup(wersja, &wsas);
	s = socket(AF_INET, SOCK_STREAM, 0);
	struct  sockaddr_in  sa, sc;
	memset((void*)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10101);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	result = bind(s, (struct sockaddr FAR*) & sa, sizeof(sa));
	result = listen(s, 5);
	int  lenc = sizeof(sc);

	struct hostTaskData hd[3];

	for (soc = 0; soc < 3; soc++) {
		so[soc] = accept(s, (struct  sockaddr  FAR*) & sc, &lenc);

		send(so[soc], (char*)&soc, sizeof(int), 0);
		v.x = 800 + (soc + 1) * 100;
		v.y = 450;
		send(so[soc], (char*)&v, sizeof(vect), 0);
		recv(so[soc], n, 16, 0);

		getMtx()->lock();
		data->players.push_back(Player(25.0f, Vector2f(v.x, v.y), n));
		getMtx()->unlock();

		hd[soc].s = so[soc];
		hd[soc].role = soc + 1;
		hd[soc].host = this;
		CreateThread(NULL, 0, host_Run, &hd[soc], 0, NULL);
	}
	return 0;
}