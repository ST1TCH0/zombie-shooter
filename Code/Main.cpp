#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0500
#define W_WIDTH 1600
#define W_HEIGHT 900
#pragma once
#include	"stdafx.h"

#include	<vector>
#include	<Windows.h>
#include	<winsock.h>
#include	<mutex>


#pragma comment(lib, "WSOCK32.lib")
RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "Shooter: Main Menu", Style::Default);

//
#include "Render.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Particle.h"
#include "TextField.h"

#include "Client.h"
#include "Host.h"
//

static DWORD WINAPI hosting(LPVOID p)
{
	return ((Host*)p)->host(p);
}

static DWORD WINAPI client_Run(LPVOID p)
{
	return ((Client*)((clientTaskData*)p)->client)->clientTask(p);
}

Device* OpenMenu(RenderWindow& window, gameData* data) {
	bool isMenu = 1;
	int menu;
	char nickname[16] = "";
	float scale = 0.7;

	Device* device = NULL;
	Texture host1tx, host2tx, cl1tx, cl2tx, menuBackground, frametx;
	host1tx.loadFromFile("Textures/HOST1.png");
	host2tx.loadFromFile("Textures/HOST2.png");
	cl1tx.loadFromFile("Textures/CLIENT1.png");
	cl2tx.loadFromFile("Textures/CLIENT2.png");
	menuBackground.loadFromFile("Textures/backgroundnew.jpg");
	frametx.loadFromFile("Textures/Frame.png");

	Sprite hostsp(host1tx), clsp(cl1tx), menuBg(menuBackground), framesp(frametx);

	Font font;
	font.loadFromFile("Fonts/PixellettersFull.ttf");

	SoundBuffer buffer;
	buffer.loadFromFile("Sounds/menu.ogg");
	Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(50);
	sound.setLoop(1);
	sound.play();

	framesp.setScale(scale, scale);
	hostsp.setScale(scale, scale);
	clsp.setScale(scale, scale);
	framesp.setPosition(window.getSize().x / 2 - frametx.getSize().x * scale / 2, window.getSize().y / 2 - frametx.getSize().y * scale / 2);
	hostsp.setPosition(framesp.getPosition().x + 30, framesp.getPosition().y + 450);
	clsp.setPosition(framesp.getPosition().x + 30, framesp.getPosition().y + 380);
	menuBg.setPosition(0, 0);

	Event event;

	std::string IP = "";

	Text ipt;
	ipt.setPosition(framesp.getPosition().x + 70, framesp.getPosition().y + 300);
	ipt.setFillColor(Color::Red);
	ipt.setCharacterSize(45);
	ipt.setString("Enter host IP:");
	ipt.setFont(font);

	Text ink;
	ink.setPosition(framesp.getPosition().x + 70, framesp.getPosition().y + 200);
	ink.setFillColor(Color::Red);
	ink.setCharacterSize(45);
	ink.setString("Nickname");
	ink.setFont(font);

	TextField tf = TextField();
	tf.setFont(font);
	tf.setPosition(Vector2f(ipt.getPosition().x + 250, ipt.getPosition().y + 12));
	//tf.open();

	TextField tfn = TextField();
	tfn.setFont(font);
	tfn.setPosition(Vector2f(ink.getPosition().x + 250, ink.getPosition().y + 12));
	//tfn.open();

	struct clientTaskData cd;

	while (isMenu)
	{
		menu = 0;

		hostsp.setTexture(host1tx);
		clsp.setTexture(cl1tx);
		if (IntRect(hostsp.getPosition().x, hostsp.getPosition().y, host1tx.getSize().x * scale, host1tx.getSize().y * scale).contains(Mouse::getPosition(window))) { hostsp.setTexture(host2tx); menu = 1; }
		if (IntRect(clsp.getPosition().x, clsp.getPosition().y, cl1tx.getSize().x * scale, cl1tx.getSize().y * scale).contains(Mouse::getPosition(window))) { clsp.setTexture(cl2tx); menu = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			tf.setActive(false);
			tfn.setActive(false);
			if (IntRect(ipt.getPosition().x, ipt.getPosition().y, 600, 50).contains(Mouse::getPosition(window))) tf.open();
			if (IntRect(ink.getPosition().x, ink.getPosition().y, 600, 50).contains(Mouse::getPosition(window))) tfn.open();

			if (menu == 1) {


				std::string temp = tfn.getText();
				if (temp.size() == 0 || temp.size() == 1) temp = "Player";
				std::copy(temp.begin(), temp.end(), nickname);
				nickname[temp.end() - temp.begin()] = '\0';

				data->players.push_back(Player(25.0f, Vector2f(800, 450), nickname));
				device = new Host(data);
				Host* a = dynamic_cast<Host*>(device);
				a->getData()->score = 0;
				a->setRole(0);
				CreateThread(NULL, 0, hosting, a, 0, NULL);

				window.setTitle("Shooter Host");
				sound.stop();
				isMenu = false;

			}
			else if (menu == 2) {
				std::string temp = tf.getText();

				char addr[16];
				std::copy(temp.begin(), temp.end(), cd.addr);
				cd.addr[temp.end() - temp.begin()] = '\0';

				temp = tfn.getText();
				std::copy(temp.begin(), temp.end(), cd.nickname);
				cd.nickname[temp.end() - temp.begin()] = '\0';

				device = new Client(data);
				printf("1 %p", device);
				cd.client = (Client*)device;
				device->getData()->score = 0;
				CreateThread(NULL, 0, client_Run, &cd, 0, NULL);

				window.setTitle("Shooter Client");
				sound.stop();
				isMenu = false;
			}
		}

		window.pollEvent(event);
		if (event.type == sf::Event::KeyPressed) {
			window.pollEvent(event);
			tf.input(event);
			tfn.input(event);
			if (event.type == Event::Closed)
				window.close();
		}

		window.draw(menuBg);
		window.draw(framesp);
		window.draw(hostsp);
		window.draw(clsp);
		window.draw(ipt);
		window.draw(ink);

		tf.render(window);
		tfn.render(window);

		window.display();
	}
	return device;
}

/*void comthread(Device *device, ) {
	while (!device->getReady());
	while (!device->getLoaded());
	evice->getMtx()->lock();
	data->enemies = data->data->enemies;
	data->players = data->data->players;
	score = data->score;
	device->getMtx()->unlock();
		
}
*/
int main() {
	srand(time(NULL));

	window.setFramerateLimit(60);
	Render render = Render(&window);
	struct gameData dat, * data = &dat;

	Bullet b1;

	Enemy enemy;

	int b_cntr = 0;
	int spawnCounter = 10;
	int pdir = 0, temp, spawn = 50;
	double dtemp, dtemp2;

	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	std::string nn;

	Device* device = OpenMenu(window, data);

	std::vector<SoundBuffer> buffer;

	for (int i = 0; i < 30; i++) {
		SoundBuffer t;
		buffer.push_back(t);
	}

	buffer[0].loadFromFile("Sounds/battle.ogg");
	buffer[1].loadFromFile("Sounds/hurt1.ogg");
	buffer[2].loadFromFile("Sounds/hurt2.ogg");
	buffer[3].loadFromFile("Sounds/hurt3.ogg");
	buffer[4].loadFromFile("Sounds/hurt4.ogg");
	buffer[5].loadFromFile("Sounds/hurt5.ogg");
	buffer[6].loadFromFile("Sounds/hurt6.ogg");
	buffer[7].loadFromFile("Sounds/step1.ogg");
	buffer[8].loadFromFile("Sounds/step2.ogg");
	buffer[9].loadFromFile("Sounds/step3.ogg");
	buffer[10].loadFromFile("Sounds/step4.ogg");
	buffer[11].loadFromFile("Sounds/shoot.ogg");
	buffer[12].loadFromFile("Sounds/death.ogg");
	buffer[13].loadFromFile("Sounds/z_death1.ogg");
	buffer[14].loadFromFile("Sounds/z_death2.ogg");
	buffer[15].loadFromFile("Sounds/z_death3.ogg");
	buffer[16].loadFromFile("Sounds/z_death4.ogg");

	bool play_dead = false;

	Sound shoot;
	shoot.setVolume(30);
	Sound death;
	death.setVolume(50);
	Sound hurt;
	hurt.setVolume(60);
	Sound step;
	step.setVolume(30);
	Sound z_death;
	z_death.setVolume(40);

	int step_cntr = 0;
	float player_speed = 2.0f;
	double zombie_speed = 0.5;
	bool is_pause = false;

	{
		SoundBuffer buff;
		buff.loadFromFile("Sounds/wait.ogg");
		Sound wait;
		wait.setBuffer(buff);
		wait.setVolume(50);
		wait.setLoop(1);
		wait.play();
		while (data->players.size() < 2) {

			render.UIWait();
		}
		wait.stop();
	}

	//std::vector<Enemy> data->enemies = data->data->enemies;
	//std::vector<Player> data->players = data->data->players;
	std::vector<std::vector<Bullet>> bullets;
	for (int i = 0; i < 4; i++)
		bullets.push_back(std::vector<Bullet>());
	std::vector<Particle> particles;

	Clock clock;
	float lastTime = 0;
	float fps = 60;

	int score = 0;
	int framecounter = 0;

	Sound battle_theme;
	battle_theme.setBuffer(buffer[0]);
	battle_theme.setLoop(1);
	battle_theme.setVolume(50);
	battle_theme.play();

	Clock mClock;
	int mFrame = 0;

	while (window.isOpen()) {
		//if (device->getLoaded()) {
			//device->getMtx()->lock();
			//data->enemies = data->data->enemies;
			//data->players = data->data->players;
			score = data->score;
			//device->getMtx()->unlock();
			device->setLoaded(0);
		//}

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (data->players[device->getRole()].isDead() && !play_dead) {
			play_dead = true;
			death.setBuffer(buffer[12]);
			death.play();
		}

		//Update
		//Vectors
		playerCenter = data->players[device->getRole()].getPosition();
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / (float)sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		data->players[device->getRole()].setAim(aimDirNorm);

		if (!data->players[device->getRole()].isDead()) {
			float PI = 3.14159265f;
			float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
			data->players[device->getRole()].setRotation(deg + 90);
		}

		pdir = 0;

		if (!data->players[device->getRole()].isDead()) {

			//Player
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				if (step_cntr % 20 == 0) {
					step.setBuffer(buffer[7 + (rand() % 3)]);
					step.play();
				}
				pdir += 1;
			}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				if (step_cntr % 20 == 0) {
					step.setBuffer(buffer[7 + (rand() % 3)]);
					step.play();
				}
				pdir += 2;
			}

			if (Keyboard::isKeyPressed(Keyboard::W)) {
				if (step_cntr % 20 == 0) {
					step.setBuffer(buffer[7 + (rand() % 3)]);
					step.play();
				}
				pdir += 4;
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				if (step_cntr % 20 == 0) {
					step.setBuffer(buffer[7 + (rand() % 3)]);
					step.play();
				}
				pdir += 8;
			}
			data->players[device->getRole()].setDir(pdir);
			if (step_cntr % 20 == 0) step_cntr = 0;
			step_cntr++;

		}

		if (Mouse::isButtonPressed(Mouse::Left) && !data->players[device->getRole()].isDead()) {
			data->players[device->getRole()].setClick(true);
		}
		else
			data->players[device->getRole()].setClick(false);

		for (int i = 0; i < data->players.size(); i++) {
			if (data->players[i].getClick() && b_cntr % 10 == 0) {
				b1.shape.setPosition(data->players[i].getPosition());
				aimDir = mousePosWindow - data->players[i].getPosition();
				aimDirNorm = aimDir / (float)sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
				b1.currVelocity = data->players[i].getAim() * b1.maxSpeed;
				b1.shape.setRotation(data->players[i].getRotation());
				bullets[i].push_back(b1);
				shoot.setBuffer(buffer[11]);
				shoot.play();

				b_cntr = 0;

				Particle smoke(&render.GetTexture("smoke"), Color(90, 90, 90, 255), 80, 30, 1);
				smoke.setPosition(data->players[i].getPosition() - Vector2f(12.5, 12.5));
				particles.push_back(smoke);

				Particle light(&render.GetTexture("light"), Color(255, 255, 0, 0), 5, 30, 2);
				light.setPosition(data->players[i].getPosition() - Vector2f(12.5, 12.5));
				particles.push_back(light);
			}

			temp = data->players[i].getDir();
			switch (temp) {
			case 1:
				if (data->players[i].getPosition().x > 13)
					data->players[i].move(-player_speed, 0.f);
				break;
			case 2:
				if (data->players[i].getPosition().x < window.getSize().x - 13)
					data->players[i].move(player_speed, 0.f);
				break;
			case 4:
				if (data->players[i].getPosition().y > 13)
					data->players[i].move(0.f, -player_speed);
				break;
			case 5:
				if (data->players[i].getPosition().x > 13 && data->players[i].getPosition().y > 13)
					data->players[i].move(-player_speed / 1.414, -player_speed / 1.414);
				else if (data->players[i].getPosition().x > 13)
					data->players[i].move(-player_speed, 0.f);
				else if (data->players[i].getPosition().y > 13)
					data->players[i].move(0.f, -player_speed);
				break;
			case 6:
				if (data->players[i].getPosition().x < window.getSize().x - 13 && data->players[i].getPosition().y > 13)
					data->players[i].move(player_speed / 1.414, -player_speed / 1.414);
				else if (data->players[i].getPosition().x < window.getSize().x - 13)
					data->players[i].move(player_speed, 0.f);
				else if (data->players[i].getPosition().y > 13)
					data->players[i].move(0.f, -player_speed);
				break;
			case 8:
				if (data->players[i].getPosition().y < window.getSize().y - 13)
					data->players[i].move(0.f, player_speed);
				break;
			case 9:
				if (data->players[i].getPosition().x > 13 && data->players[i].getPosition().y < window.getSize().y - 13)
					data->players[i].move(-player_speed / 1.414, player_speed / 1.414);
				else if (data->players[i].getPosition().x > 13)
					data->players[i].move(-player_speed, 0.f);
				else if (data->players[i].getPosition().y < window.getSize().y - 13)
					data->players[i].move(0.f, player_speed);
				break;
			case 10:
				if (data->players[i].getPosition().x < window.getSize().x - 13 && data->players[i].getPosition().y < window.getSize().y - 13)
					data->players[i].move(player_speed / 1.414, player_speed / 1.414);
				else if (data->players[i].getPosition().x < window.getSize().x - 13)
					data->players[i].move(player_speed, 0.f);
				else if (data->players[i].getPosition().y < window.getSize().y - 13)
					data->players[i].move(0.f, player_speed);
				break;
			}
		}
		//data->enemies
		if (device->getRole() == 0) {
			int p = rand() % 1000;

			if (p < spawn) {
				int i = rand() % 4;
				p = rand();
				switch (i) {
				case 0:
					enemy.setPosition(Vector2f(rand() % (window.getSize().x + 100), -50));
					break;
				case 1:
					enemy.setPosition(Vector2f(rand() % (window.getSize().x + 100), window.getSize().y + 50));
					break;
				case 2:
					enemy.setPosition(Vector2f(-50, rand() % (window.getSize().y + 100)));
					break;
				case 3:
					enemy.setPosition(Vector2f(window.getSize().x + 50, rand() % (window.getSize().y + 100)));
					break;
				}

				data->enemies.push_back(enemy);
			}

			spawnCounter = 0;
			if (framecounter % 30 == 0) {
				spawn++;
			}

		}
		b_cntr++;

		//Shooting
		for (size_t k = 0; k < data->enemies.size(); k++) {

			Vector2f epos;
			dtemp = 100000000;
			temp = 0;
			for (int i = 0; i < data->players.size(); i++) {
				if (!data->players[i].isDead()) {
					dtemp2 = pow(data->players[i].getPosition().x - data->enemies[k].getPosition().x, 2) + pow(data->players[i].getPosition().y - data->enemies[k].getPosition().y, 2);
					if (dtemp2 < dtemp) {
						temp = i;
						dtemp = dtemp2;
					}
				}
			}
			dtemp = sqrt(dtemp);
			epos.x = zombie_speed / dtemp * (data->players[temp].getPosition().x - data->enemies[k].getPosition().x);
			epos.y = zombie_speed / dtemp * (data->players[temp].getPosition().y - data->enemies[k].getPosition().y);

			data->enemies.at(k).setPosition(data->enemies[k].getPosition() + epos);

			Vector2f zaimDir = (data->enemies[k].getPosition() + epos - data->players[temp].getPosition());
			Vector2f zaimDirNorm = zaimDir / (float)sqrt(pow(zaimDir.x, 2) + pow(zaimDir.y, 2));

			float PI = 3.14159265f;
			float deg = atan2(zaimDirNorm.y, zaimDirNorm.x) * 180 / PI;
			data->enemies.at(k).setRotation(deg + 90);

			for (int i = 0; i < data->players.size(); i++) {
				if (data->players[i].getVisual()->getGlobalBounds().intersects(data->enemies[k].getGlobalBounds()) && b_cntr % 10 == 0 && !data->players[i].isDead()) {
					if (i == device->getRole()) {
						hurt.setBuffer(buffer[1 + (rand() % 6)]);
						hurt.play();
					}
					data->players[i].setHealth(data->players[i].getHealth() - 1);
				}
			}
		}
		for (int j = 0; j < bullets.size(); j++) {
			size_t bsize = bullets[j].size();
			for (size_t i = 0; i < bsize; i++)
			{
				bullets[j][i].shape.move(bullets[j][i].currVelocity);

				//Out of bounds
				if (bullets[j][i].shape.getPosition().x < 0 || bullets[j][i].shape.getPosition().x > window.getSize().x
					|| bullets[j][i].shape.getPosition().y < 0 || bullets[j][i].shape.getPosition().y > window.getSize().y)
				{
					bullets[j].erase(bullets[j].begin() + i);

				}
				else
				{
					//Enemy collision
					for (size_t k = 0; k < data->enemies.size(); k++)
					{
						if (bullets[j][i].shape.getGlobalBounds().intersects(data->enemies[k].getGlobalBounds()))
						{
							Particle blood(&render.GetTexture("blood"), Color::Red, 250, 50, 0);
							blood.setPosition(data->enemies.at(k).getPosition());
							blood.setRotation(data->enemies.at(k).getRotation());
							particles.push_back(blood);

							bullets[j].erase(bullets[j].begin() + i);
							data->enemies.erase(data->enemies.begin() + k);
							z_death.setBuffer(buffer[13 + (rand() % 3)]);
							z_death.play();
							//data->players[i].addScore();
							if (device->getRole() == 0) {
								score++;
								data->players[0].setScore(score);
							}

							break;
						}
					}
				}
				bsize = bullets[j].size();
			}
		}

		for (size_t i = 0; i < particles.size(); i++) {
			if (!particles.at(i).Update()) particles.erase(particles.begin() + i);
		}

		//if (device->getReady()) {
			//device->getMtx()->lock();
			if (device->getRole()) {
				device->setMe(data->players[device->getRole()]);
			}
			else {
				//data->enemies = data->enemies;
				//data->players = data->players;
				data->score = score;
			}
			//device->getMtx()->unlock();
			device->setReady(0);
		/*}
		//else {
			if (!device->getRole()) {
				data->data->enemies = data->enemies;
				data->data->players = data->players;
				data->score = score;
			}
		}*/

		//Draw
		render.RenderStatic();

		for (size_t i = 0; i < particles.size(); i++) {
			if (particles[i].getType() != 1) window.draw(particles[i].getVisual());
			else if (particles[i].getType() == 1 || particles[i].getType() == 2) window.draw(particles[i].getVisual(), particles[i].getTransf());
		}

		for (size_t i = 0; i < data->enemies.size(); i++) {
			data->enemies[i].setTexture(&render.GetTexture("zombie"));
			window.draw(data->enemies[i]);
		}

		for (int i = 0; i < data->players.size(); i++) {
			data->players[i].Update();
			Text nick;
			Font font = render.GetFont();
			Vector2f ppos = data->players[i].getPosition();
			ppos.y = ppos.y + 30;
			nick.setPosition(ppos);
			nick.setFillColor(Color::White);
			nick.setCharacterSize(24);
			nick.setString(data->players[i].getNickname());
			nick.setFont(font);

			Text health;
			ppos.x = ppos.x - 45;
			health.setPosition(ppos);
			health.setFillColor(Color::Red);
			health.setCharacterSize(24);

			std::string h = "";
			int n = (int)data->players[i].getHealth() / 10;

			for (int j = 0; j < n; j++) h.append("|");

			if (data->players[i].getHealth() < 1) h = "DEAD";

			health.setString(h);
			health.setFont(font);

			window.draw(nick);
			window.draw(health);
			data->players[i].getVisual()->setTexture(&render.GetTexture("player"));
			window.draw(*data->players[i].getVisual());

		}

		Font font = render.GetFont();

		Text tscore;
		tscore.setPosition(Vector2f(10, 10));
		tscore.setFillColor(Color::Green);
		tscore.setCharacterSize(32);
		tscore.setString("Score: " + std::to_string(score));
		tscore.setFont(font);

		Text fpst;
		fpst.setPosition(Vector2f(10, 30));
		fpst.setFillColor(Color::Green);
		fpst.setCharacterSize(32);
		if (mClock.getElapsedTime().asSeconds() >= 1.f)
		{
			fps = mFrame;
			mFrame = 0;
			mClock.restart();
		}

		++mFrame;
		fpst.setString(" ");
		fpst.setString("FPS: " + std::to_string((int)fps));
		fpst.setFont(font);

		window.draw(fpst);
		window.draw(tscore);

		for (size_t i = 0; i < bullets.size(); i++)
			for (size_t j = 0; j < bullets[i].size(); j++)
				window.draw(bullets[i][j].shape);

		window.display();
		framecounter++;
	}

	return 0;
}