#include "Client.h"



DWORD WINAPI Client::clientTask(void* args) {
	SOCKET sckt;
	struct sockaddr_in sa;
	struct clientTaskData* cd = (struct clientTaskData*)args;
	WSADATA wsas;
	WORD wersja;
	wersja = MAKEWORD(2, 0);
	WSAStartup(wersja, &wsas);
	sckt = socket(AF_INET, SOCK_STREAM, 0);
	memset((void*)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10101);
	sa.sin_addr.s_addr = inet_addr(cd->addr);

	int size, score, id=0, id2=0;
	struct senddata sd;
	struct vect v;
	char n[16];
	clock_t start = clock();
	Enemy enemy;
	Player player(25.0f, Vector2f(0, 0), "P");

	while (connect(sckt, (struct sockaddr FAR*) & sa, sizeof(sa)) == SOCKET_ERROR);

	recv(sckt, (char*)&size, sizeof(int), 0);
	role = size + 1;

	recv(sckt, (char*)&v, sizeof(vect), 0);
	player.setPosition(Vector2f(v.x, v.y));

	for (int i = 0; i < strlen(cd->nickname); i++)
		n[i] = cd->nickname[i];
	n[strlen(cd->nickname)] = '\0';
	player.setNickname((String)n);

	me = player;

	send(sckt, n, 16, 0);

	std::vector<Enemy> enemies;
	std::vector<Player> players;
	std::string nn;

	while (1) {
		printf("time: %i\n\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		start = clock();
		recv(sckt, (char*)&sd, sizeof(int), 0);
		send(sckt, (char*)&score, sizeof(int), 0);
		printf("test: %i\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

		recv(sckt, (char*)&score, sizeof(int), 0);

		recv(sckt, (char*)&size, sizeof(int), 0);
		if (enemies.size() > size)
			for (int i = enemies.size() - size; i > 0; i--)
				enemies.erase(enemies.begin() + i - 1);
		else if (size > enemies.size())
			for (int i = size - enemies.size(); i > 0; i--)
				enemies.push_back(enemy);
		printf("enemfit: %i\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		for (int i = 0; i < size; i++) {
			recv(sckt, (char*)&v, sizeof(vect), 0);
			enemies[i].setPosition(sf::Vector2f(v.x, v.y));
		}
		printf("enem: %i\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		recv(sckt, (char*)&size, sizeof(int), 0);
		recv(sckt, (char*)&id2, sizeof(int), 0);

		if (players.size() > size)
			for (int i = players.size() - size; i > 0; i--)
				players.erase(players.begin() + i - 1);
		else if (size > players.size())
			for (int i = size - players.size(); i > 0; i--)
				players.push_back(player);
		printf("playersfit: %i\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		for (int i = 0; i < size; i++) {
			recv(sckt, (char*)&sd, sizeof(senddata), 0);

			players[i].setPosition(Vector2f(sd.x, sd.y));
			players[i].setDir(sd.dir);
			players[i].setRotation(sd.rot);
			players[i].setNickname((String)sd.n);
			players[i].setClick(sd.click);
			players[i].setDead(sd.is_dead);
			players[i].setHealth(sd.health);

			recv(sckt, (char*)&v, sizeof(vect), 0);
			players[i].setAim(Vector2f(v.x, v.y));
		}
		printf("load: %i\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		getMtx()->lock();
		data->players = players;
		data->enemies = enemies;
		data->score = score;
		getMtx()->unlock();

		setLoaded(1);
		setReady(1);
		//while (getReady());

		getMtx()->lock();
		sd.dir = getMe().getDir();
		sd.rot = getMe().getRotation();
		sd.is_dead = getMe().isDead();
		sd.click = getMe().getClick();
		sd.health = getMe().getHealth();
		sd.x = getMe().getAim().x;
		sd.y = getMe().getAim().y;
		sd.id = id;
		getMtx()->unlock();
		printf("send: %i\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
		printf("id: %i %i\n\n", id, id2);
		send(sckt, (char*)&sd, sizeof(senddata), 0);
		id++;
	}
	return 0;
}