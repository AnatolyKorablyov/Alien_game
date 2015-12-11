#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include "TinyXML\tinyxml.h"
#include "level.h"
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "Enemys.h"
#include "Bullets.h"
#include "Equip_bonuses_exit.h"

using namespace sf;

static const sf::Vector2f WINDOW_SIZE = { 1370, 768 };


void init_objects_in_map(Level lvl, std::vector<Entity*>  &entities, Image easyEnemyImage, Image mediumEnemyImage, Image banditImage, Image equipImage, Image gunImage, Image exitImage, Image solidImage) {
	std::vector<Object> easyE = lvl.GetObjects("easyEnemy");
	std::vector<Object> medE = lvl.GetObjects("mediumEnemy");
	std::vector<Object> bandE = lvl.GetObjects("bandit");
	std::vector<Object> shotgun = lvl.GetObjects("shotgun");
	std::vector<Object> machineGun = lvl.GetObjects("machineGun");
	std::vector<Object> rifle = lvl.GetObjects("rifle");
	std::vector<Object> automatical = lvl.GetObjects("automatical");
	std::vector<Object> exit = lvl.GetObjects("exit");
	std::vector<Object> solidExit = lvl.GetObjects("solidExit");

	for (int i = 0; i < int(easyE.size()); i++) //���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(easyEnemyImage, gunImage, lvl, easyE[i].rect.left, easyE[i].rect.top, 43, 75, 0, 0, "easyEnemy"));//� ���������� � ������ ���� ����� ������ � �����
	for (int i = 0; i < int(medE.size()); i++)
		entities.push_back(new Enemy(mediumEnemyImage, gunImage, lvl, medE[i].rect.left, medE[i].rect.top, 76, 44, 0, 0, "mediumEnemy"));
	for (int i = 0; i < int(bandE.size()); i++) //���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(banditImage, gunImage, lvl, easyE[i].rect.left, easyE[i].rect.top, 104, 56, 72, 15, "bandit"));//� ���������� � ������ ���� ����� ������ � �����
	for (int i = 0; i < int(machineGun.size()); i++)
		entities.push_back(new Equip(equipImage, machineGun[i].rect.left, machineGun[i].rect.top, 80, 20, "machineGun"));
	for (int i = 0; i < int(shotgun.size()); i++)
		entities.push_back(new Equip(equipImage, shotgun[i].rect.left, shotgun[i].rect.top, 80, 15, "shotgun"));
	for (int i = 0; i < int(automatical.size()); i++)
		entities.push_back(new Equip(equipImage, automatical[i].rect.left, automatical[i].rect.top, 80, 20, "automatical"));
	for (int i = 0; i < int(rifle.size()); i++)
		entities.push_back(new Equip(equipImage, rifle[i].rect.left, rifle[i].rect.top, 80, 13, "rifle"));
	for (int i = 0; i < int(exit.size()); i++)
		entities.push_back(new Exit(exitImage, exit[i].rect.left, exit[i].rect.top, 147, 138, "exit"));
	for (int i = 0; i < int(solidExit.size()); i++)
		entities.push_back(new Exit(solidImage, solidExit[i].rect.left, solidExit[i].rect.top, 147, 138, "solidExit"));
}

void skip_to_list(std::vector<Entity*>::iterator it, std::vector<Entity*>  &entities, Player p, int &randBonus, Image bonusImage, Sprite meetSprite, float time, float timeGame) {
	for (it = entities.begin(); it != entities.end();) {//������� ��� ���������� �� ������ �� �����
		Entity *b = (*it);//��� ��������, ����� �� ������ (*it)->
		if (b->life == false) {
			if (b->name == "bandit" || b->name == "easyEnemy" || b->name == "mediumEnemy") { //TODO cut on function
				randBonus = rand() % 15;
				if (randBonus == 1)
					entities.push_back(new Bonus(bonusImage, b->x, b->y, 50, 50, "Ammo"));
				if (randBonus == 2)
					entities.push_back(new Bonus(bonusImage, b->x, b->y, 50, 50, "Armor"));
				if (randBonus == 3)
					entities.push_back(new Bonus(bonusImage, b->x, b->y, 50, 50, "Health"));
				if (b->timeDeath == 0) {
					b->name = "meet";
					b->sprite = meetSprite;
					b->sprite.setTextureRect(IntRect(0, 0, 80, 80));
					b->sprite.setPosition(b->x, b->y);
					b->timeDeath = timeGame + 1;
					it++;
				}
				else if (b->timeDeath < timeGame) {
					it = entities.erase(it); delete b;// ���� ���� ������ �����, �� ������� ���
				}
				else it++;
			}
			else {
				it = entities.erase(it); delete b;
			}
		}
		else {
			b->update(time, p.x, p.y);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}
	}
}

struct init_sounds {
	SoundBuffer machineBuffer;//������ ����� ��� �����
	SoundBuffer damageBuffer;//������ ����� ��� �����
	SoundBuffer shotgunBuffer;//������ ����� ��� �����
	SoundBuffer rifleBuffer;//������ ����� ��� �����
	SoundBuffer reloadBuffer;//������ ����� ��� �����
	Sound shootMachine;
	Sound damage;
	Sound shootShootGun;
	Sound shootRifle;
	Sound shootReload;

	init_sounds() {
		machineBuffer.loadFromFile("sounds/MachineGun.wav");//��������� � ���� ����
		shootMachine.setBuffer(machineBuffer);

		damageBuffer.loadFromFile("sounds/ZombieAttack1.wav");//��������� � ���� ����
		damage.setBuffer(damageBuffer);//������� ���� � ��������� � ���� ���� �� ������

		shotgunBuffer.loadFromFile("sounds/PumpShotgun.wav");//��������� � ���� ����
		shootShootGun.setBuffer(shotgunBuffer);//������� ���� � ��������� � ���� ���� �� ������

		rifleBuffer.loadFromFile("sounds/Rifle.wav");//��������� � ���� ����
		shootRifle.setBuffer(rifleBuffer);//������� ���� � ��������� � ���� ���� �� ������

		reloadBuffer.loadFromFile("sounds/Reload.wav");//��������� � ���� ����
		shootReload.setBuffer(reloadBuffer);//������� ���� � ��������� � ���� ���� �� ������	
	}
};

int main() {
	int randBonus;
	float queueShot = 0, enemyShot = 0;
	int countEnemy = 0;

	bool areaClean = false;
	Music music;
	music.openFromFile("sounds/mus01.ogg");
	music.play();

	sf::Vector2f posView;

	init_sounds sounds;

	int HP;
	String strHP;
	Font font;//����� 
	font.loadFromFile("images/Albionic.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 40);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setColor(Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����

	int tempX = 0;//��������� ����� �.������� �� ����� ������� ���� ���� ����
	int tempY = 0;//����� Y
	float distance = 0;//��� ���������� �� ������� �� ���� �������
	sf::RenderWindow window(sf::VideoMode(int(WINDOW_SIZE.x), int(WINDOW_SIZE.y)), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������.

	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("map.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.

	Object player = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����

	std::vector<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
	std::vector<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������


	Image heroImage, gunImage, easyEnemyImage, bulletImage, mediumEnemyImage, equipImage, banditImage, meetImage, bonusImage, exitImage, solidImage;
	bulletImage.loadFromFile("images/shot2.png");
	heroImage.loadFromFile("images/players_anim.png");
	gunImage.loadFromFile("images/weapons.png");
	equipImage.loadFromFile("images/equipment2.png");
	easyEnemyImage.loadFromFile("images/easy_enemy.png");
	banditImage.loadFromFile("images/bandit.png");
	mediumEnemyImage.loadFromFile("images/MediumEnemy.png");
	meetImage.loadFromFile("images/meet.png");
	bonusImage.loadFromFile("images/bonus.png");
	exitImage.loadFromFile("images/exitImage.png");
	solidImage.loadFromFile("images/solidImage.png");

	init_objects_in_map(lvl, entities, easyEnemyImage, mediumEnemyImage, banditImage, equipImage, gunImage, exitImage, solidImage);

	Texture meetTexture;
	meetTexture.loadFromImage(meetImage);
	Sprite meetSprite;
	meetSprite.setTexture(meetTexture);
	Player p(heroImage, gunImage, lvl, player.rect.left, player.rect.top, 91, 54, 72, 15, "Player");//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
	Clock clock;
	Clock gameTime;
	
	bool ret = false;
	bool isShoot = false;
	float CurrentFrame = 0;//������ ������� ����
	while (window.isOpen()){
		std::vector<Entity*>::iterator at;//������ ��������.��� �������������� ����� ��������� ������
		Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)
		float time = float(clock.getElapsedTime().asMicroseconds());
		float timeGame = gameTime.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
			if (isShoot && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				isShoot = false;
			if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || isShoot) && p.weapon == 0 && p.ammo > 0) { //TODO cut on function
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 0));
				p.ammo -= 1;
				isShoot = true;
				sounds.shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.weapon == 1 && p.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 21, 15, pos.x, pos.y, "Bullet", p.damage, 0));
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 21, 15, pos.x, pos.y, "Bullet", p.damage, 1));
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 21, 15, pos.x, pos.y, "Bullet", p.damage, 2));
				p.ammo -= 1;
				queueShot = timeGame + 1;
				sounds.shootShootGun.play();
			}
			else if (event.key.code == Mouse::Left && p.weapon == 2 && p.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 21, 15, pos.x, pos.y, "Bullet", p.damage, 0));
				p.ammo -= 1;
				queueShot = timeGame + 0.25f;
				sounds.shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.weapon == 3 && p.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 21, 15, pos.x, pos.y, "Bullet", p.damage, 0));
				p.ammo -= 1;
				queueShot = timeGame + 2;
				sounds.shootRifle.play();
			}
			else if (event.key.code == Mouse::Left)
				sounds.shootReload.play();
		}
		p.rotation_GG(pos);
		if (p.life)
			p.update(time, p.x, p.y);// Player update function
		

		skip_to_list(it, entities, p, randBonus, bonusImage, meetSprite, time, timeGame);






		for (it = entities.begin(); it != entities.end(); it++){//���������� �� ��-��� ������ TODO cut on function
			if ((*it)->name == "bandit" && ((*it)->dx <= 500 || (*it)->dx <= -500) && ((*it)->dy < 300 || (*it)->dy < 300) && (*it)->enemyShot < timeGame) { //�������� ��������� �� ������ �� ����������� �������� � ����� �� �� ������� �������
				entities.push_back(new Bullet(bulletImage, lvl, (*it)->x + (*it)->w / 2, (*it)->y + (*it)->h / 2, 21, 15, p.x, p.y, "enemyBullet", (*it)->damage, 0));
				(*it)->enemyShot = timeGame + 1;
			}
			for (at = entities.begin(); at != entities.end(); at++) {
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "easyEnemy"))) {
					(*it)->health -= (*at)->damage;
					(*at)->life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "mediumEnemy"))) {
					(*it)->health -= (*at)->damage;
					(*at)->life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "bandit"))) {
					if ((*it)->armor > 0)
						(*it)->armor -= (*at)->damage;
					else 
						(*it)->health -= (*at)->damage;
					(*at)->life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "easyEnemy") && ((*it)->name == "easyEnemy"))) {
					(*it)->x -= 5;
					(*it)->y -= 5;
					(*at)->x += 5;
					(*at)->y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "easyEnemy") && ((*it)->name == "mediumEnemy"))) {
					(*it)->x -= 5;
					(*it)->y -= 5;
					(*at)->x += 5;
					(*at)->y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "bandit") && ((*it)->name == "bandit"))) {
					(*it)->x -= 5;
					(*it)->y -= 5;
					(*at)->x += 5;
					(*at)->y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "mediumEnemy") && ((*it)->name == "mediumEnemy"))) {
					if ((*it)->x > (*at)->x) {
						(*it)->x -= 5;
						(*at)->x += 5;
					}
					else {
						(*it)->x += 5;
						(*at)->x -= 5;
					}
					if ((*it)->y > (*at)->x) {
						(*it)->y += 5;
						(*at)->y -= 5;
					}
					else {
						(*it)->y -= 5;
						(*at)->y += 5;
					}

				}
			}
			if ((*it)->getRect().intersects(p.getRect())){//���� ������������� ������� ������� ������������ � ������� TODO cut on function
				if ((*it)->name == "easyEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					if ((*it)->x > p.x)
						(*it)->x -= 15;
					else
						(*it)->x += 15;
					if ((*it)->y > p.y)
						(*it)->y += 15;
					else
						(*it)->y -= 15;
					if (p.armor > 0)
						p.armor -= (*it)->damage;
					else
						p.health -= (*it)->damage;
					sounds.damage.play();
				}
				if ((*it)->name == "mediumEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					if ((*it)->x > p.x)
						(*it)->x += 15;
					else
						(*it)->x -= 15;
					if ((*it)->y > p.y)
						(*it)->y += 15;
					else
						(*it)->y -= 15;
					if (p.armor > 0)
						p.armor -= (*it)->damage;
					else
						p.health -= (*it)->damage;
					sounds.damage.play();
				}
				if ((*it)->name == "enemyBullet") {
					if (p.armor > 0)
						p.armor -= (*it)->damage;
					else
						p.health -= (*it)->damage;
					(*it)->life = false;
				}
				if ((*it)->name == "Ammo") {
					p.ammo += 2;
					(*it)->life = false;
				}
				if ((*it)->name == "Armor") {
					(*it)->life = false;
					if (p.armor + 20 < 100)
						p.armor += 20;
					else if (p.armor == 100)
						(*it)->life = true;
					else
						p.armor = 100;
				}
				if ((*it)->name == "Health") {
					(*it)->life = false;
					if (p.health + 20 < 100)
						p.health += 20;
					else if (p.health == 100)
						(*it)->life = true;
					else
						p.health = 100;
				}
				if ((*it)->name == "machineGun") {
					p.weapon = 0;
					p.select_weapon(p.weapon);
					(*it)->life = false;
					queueShot = 0;
				}
				if ((*it)->name == "shotgun") {
					p.weapon = 1;
					p.select_weapon(p.weapon);
					queueShot = 0;
					(*it)->life = false;
				}
				if ((*it)->name == "automatical") {
					p.weapon = 2;
					p.select_weapon(p.weapon);
					(*it)->life = false;
					queueShot = 0;
				}
				if ((*it)->name == "rifle") {
					p.weapon = 3;
					p.select_weapon(p.weapon);
					(*it)->life = false;
					queueShot = 0;
				}
				if ((*it)->name == "solidExit") {
					p.y -= 13;
				}
				if ((*it)->name == "exit" && areaClean)
					window.close();
			}
		}
		countEnemy = 0;
		window.setView(view);
		window.clear();
		posView = view.getCenter();
		lvl.Draw(window);//������ ����� �����
		for (it = entities.begin(); it != entities.end(); it++) { // TODO cut on function
			if ((*it)->name == "bandit" || (*it)->name == "easyEnemy" || (*it)->name == "mediumEnemy") {
				countEnemy += 1;
			}
			if (areaClean && (*it)->name == "solidExit")
				(*it)->life = false;
			if ((*it)->x > posView.x - 632 && (*it)->x < posView.x + 632 && (*it)->y > posView.y - 384 && (*it)->y < posView.y + 384) { // ���� ������ ������� �� �����, �� ������
				window.draw((*it)->gunSprite);
				if ((*it)->name == "exit") {
					if (areaClean)
						window.draw((*it)->sprite);
				}
				else
					window.draw((*it)->sprite);
			}
		}
		window.draw(p.gunSprite);
		window.draw(p.sprite);
		text.setPosition(posView.x + 600, posView.y - 360);
		HP = countEnemy;
		strHP = std::to_string(HP);
		text.setColor(Color::Blue);
		text.setString(strHP);
		window.draw(text);
		if (countEnemy == 0) { //TODO cut on function
			music.stop();
			areaClean = true;
			text.setPosition(posView.x - 100, posView.y - 50);//������ ������� ������, ����� ������
			text.setColor(Color::Blue);
			text.setString("AREA CLEAN");//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		if (!p.life) {
			text.setPosition(posView.x - 100, posView.y - 50);//������ ������� ������, ����� ������
			text.setColor(Color::Red);
			text.setString("GAME OVER");//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		else {
			text.setPosition(posView.x - 610, posView.y + 290);//������ ������� ������, ����� �����
			text.setColor(Color::Red);
			HP = p.health;
			strHP = std::to_string(HP);
			text.setString(strHP);//������ ������ ������
			window.draw(text);//����� ���� �����
			
			if (p.armor > 0) {
				text.setPosition(posView.x - 530, posView.y + 290);//������ ������� ������, ����� �����
				text.setColor(Color::Blue); // �����
				HP = p.armor;
				strHP = std::to_string(HP);
				text.setString(strHP);//������ ������ ������
				window.draw(text);//����� ���� �����]
			}

			text.setPosition(posView.x + 500, posView.y + 290);//������ ������� ������, ����� �����
			text.setColor(Color::Cyan); // �����
			HP = p.ammo;
			strHP = std::to_string(HP);
			text.setString(strHP);//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		if (p.gameover) {
			window.close();
		}
		window.display(); 
	}
	return 0;
}