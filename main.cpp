#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include "TinyXML\tinyxml.h"
#include "level.h"
#include <vector>
#include "Classes.h"

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
	Vector2f sizeGu = { 0, 0 };
	for (int i = 0; i < int(easyE.size()); i++) {
		Vector2f coordin = { easyE[i].rect.left, easyE[i].rect.top };
		Vector2f sizeHer = { easyE[i].rect.height, easyE[i].rect.width };
		entities.push_back(new Enemy(easyEnemyImage, gunImage, lvl, coordin, sizeHer, sizeGu, "easyEnemy"));//� ���������� � ������ ���� ����� ������ � �����
	}
	for (int i = 0; i < int(medE.size()); i++) {
		Vector2f coordin = { medE[i].rect.left, medE[i].rect.top };
		Vector2f sizeHer = { medE[i].rect.height, medE[i].rect.width };
		entities.push_back(new Enemy(mediumEnemyImage, gunImage, lvl, coordin, sizeHer, sizeGu, "mediumEnemy"));
	}
	for (int i = 0; i < int(bandE.size()); i++) {
		Vector2f coordin = { bandE[i].rect.left, bandE[i].rect.top };
		Vector2f sizeHer = { bandE[i].rect.height, bandE[i].rect.width };
		Vector2f sizeGu = { 15, 72 };
		entities.push_back(new Enemy(banditImage, gunImage, lvl, coordin, sizeHer, sizeGu, "bandit"));//� ���������� � ������ ���� ����� ������ � �����
	}
	for (int i = 0; i < int(machineGun.size()); i++) {
		Vector2f coordin = { machineGun[i].rect.left, machineGun[i].rect.top };
		Vector2f sizeHer = { 20, 80};
		entities.push_back(new Equip(equipImage, coordin, sizeHer, "machineGun"));
	}
	for (int i = 0; i < int(shotgun.size()); i++) {
		Vector2f coordin = { shotgun[i].rect.left, shotgun[i].rect.top };
		Vector2f sizeHer = { 15, 80};
		entities.push_back(new Equip(equipImage, coordin, sizeHer, "shotgun"));
	}
	for (int i = 0; i < int(automatical.size()); i++) {
		Vector2f coordin = { automatical[i].rect.left, automatical[i].rect.top };
		Vector2f sizeHer = { 20, 80};
		entities.push_back(new Equip(equipImage, coordin, sizeHer, "automatical"));
	}
	for (int i = 0; i < int(rifle.size()); i++) {
		Vector2f coordin = { rifle[i].rect.left, rifle[i].rect.top };
		Vector2f sizeHer = { 13, 80};
		entities.push_back(new Equip(equipImage, coordin, sizeHer, "rifle"));
	}
	for (int i = 0; i < int(exit.size()); i++) {
		Vector2f coordin = { exit[i].rect.left, exit[i].rect.top };
		Vector2f sizeHer = { 147, 138 };
		entities.push_back(new Exit(exitImage, coordin, sizeHer, "exit"));
	}
	for (int i = 0; i < int(solidExit.size()); i++) {
		Vector2f coordin = { solidExit[i].rect.left, solidExit[i].rect.top };
		Vector2f sizeHer = { 147, 138 };
		entities.push_back(new Exit(solidImage, coordin, sizeHer, "solidExit"));
	}
}

void skip_to_list(std::vector<Entity*>::iterator it, std::vector<Entity*>  &entities, Player p, int &randBonus, Image bonusImage, Sprite meetSprite, float time, float timeGame) {
	for (it = entities.begin(); it != entities.end();) {//������� ��� ���������� �� ������ �� �����
		Entity *b = (*it);//��� ��������, ����� �� ������ (*it)->
		if (b->properties.life == false) {
			if (b->name == "bandit" || b->name == "easyEnemy" || b->name == "mediumEnemy") { //TODO cut on function
				randBonus = rand() % 15;
				Vector2f sizeBonus = { 50, 50 };
				if (randBonus == 1)
					entities.push_back(new Bonus(bonusImage, b->properties.pos, sizeBonus, "Ammo"));
				if (randBonus == 2)
					entities.push_back(new Bonus(bonusImage, b->properties.pos, sizeBonus, "Armor"));
				if (randBonus == 3)
					entities.push_back(new Bonus(bonusImage, b->properties.pos, sizeBonus, "Health"));
				if (b->timeDeath == 0) {
					b->name = "meet";
					b->sprite = meetSprite;
					b->sprite.setTextureRect(IntRect(0, 0, 80, 80));
					b->sprite.setPosition(b->properties.pos.x, b->properties.pos.y);
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
			b->update(time, p.properties.pos.x, p.properties.pos.y);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
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
	//music.play();

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
	Vector2f posPl = { player.rect.left, player.rect.top };
	Vector2f sizeHero = { player.rect.height, player.rect.width };
	Vector2f sizeGun = { 72, 15 };
	Player p(heroImage, gunImage, lvl, posPl, sizeHero, sizeGun, "Player");//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
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
			posPl = { p.properties.pos.x + p.properties.sizeHero.y / 2, p.properties.pos.y + p.properties.sizeHero.x / 2 };
			sizeHero = { 23, 7 };
			if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || isShoot) && p.liv_pr.weapon == 0 && p.liv_pr.ammo > 0) { //TODO cut on function
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				p.liv_pr.ammo -= 1;
				isShoot = true;
				sounds.shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.liv_pr.weapon == 1 && p.liv_pr.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 1));
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 2));
				p.liv_pr.ammo -= 1;
				queueShot = timeGame + 1;
				sounds.shootShootGun.play();
			}
			else if (event.key.code == Mouse::Left && p.liv_pr.weapon == 2 && p.liv_pr.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				p.liv_pr.ammo -= 1;
				queueShot = timeGame + 0.25f;
				sounds.shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.liv_pr.weapon == 3 && p.liv_pr.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				p.liv_pr.ammo -= 1;
				queueShot = timeGame + 2;
				sounds.shootRifle.play();
			}
			else if (event.key.code == Mouse::Left)
				sounds.shootReload.play();
		}
		p.rotation_GG(pos);
		if (p.properties.life)
			p.update(time, p.properties.pos.x, p.properties.pos.y);// Player update function
		

		skip_to_list(it, entities, p, randBonus, bonusImage, meetSprite, time, timeGame);

		for (it = entities.begin(); it != entities.end(); it++){//���������� �� ��-��� ������ TODO cut on function
			if ((*it)->name == "bandit" && ((*it)->properties.dx <= 500 || (*it)->properties.dx <= -500) && ((*it)->properties.dy < 300 || (*it)->properties.dy < 300) && (*it)->enemyShot < timeGame) { //�������� ��������� �� ������ �� ����������� �������� � ����� �� �� ������� �������
				sizeHero = { 23, 7 };
				posPl = { (*it)->properties.pos.x + (*it)->properties.sizeHero.y / 2, (*it)->properties.pos.y + (*it)->properties.sizeHero.x / 2 };
				entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, p.properties.pos, "enemyBullet", (*it)->liv_pr.damage, 0));
				(*it)->enemyShot = timeGame + 1;
			}
			for (at = entities.begin(); at != entities.end(); at++) {
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "easyEnemy"))) {
					(*it)->liv_pr.health -= (*at)->liv_pr.damage;
					(*at)->properties.life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "mediumEnemy"))) {
					(*it)->liv_pr.health -= (*at)->liv_pr.damage;
					(*at)->properties.life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "bandit"))) {
					if ((*it)->liv_pr.armor > 0)
						(*it)->liv_pr.armor -= (*at)->liv_pr.damage;
					else 
						(*it)->liv_pr.health -= (*at)->liv_pr.damage;
					(*at)->properties.life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "easyEnemy") && ((*it)->name == "easyEnemy"))) {
					(*it)->properties.pos.x -= 5;
					(*it)->properties.pos.y -= 5;
					(*at)->properties.pos.x += 5;
					(*at)->properties.pos.y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "easyEnemy") && ((*it)->name == "mediumEnemy"))) {
					(*it)->properties.pos.x -= 5;
					(*it)->properties.pos.y -= 5;
					(*at)->properties.pos.x += 5;
					(*at)->properties.pos.y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "bandit") && ((*it)->name == "bandit"))) {
					(*it)->properties.pos.x -= 5;
					(*it)->properties.pos.y -= 5;
					(*at)->properties.pos.x += 5;
					(*at)->properties.pos.y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "mediumEnemy") && ((*it)->name == "mediumEnemy"))) {
					if ((*it)->properties.pos.x > (*at)->properties.pos.x) {
						(*it)->properties.pos.x -= 5;
						(*at)->properties.pos.x += 5;
					}
					else {
						(*it)->properties.pos.x += 5;
						(*at)->properties.pos.x -= 5;
					}
					if ((*it)->properties.pos.y > (*at)->properties.pos.x) {
						(*it)->properties.pos.y += 5;
						(*at)->properties.pos.y -= 5;
					}
					else {
						(*it)->properties.pos.y -= 5;
						(*at)->properties.pos.y += 5;
					}

				}
			}
			if ((*it)->getRect().intersects(p.getRect())){//���� ������������� ������� ������� ������������ � ������� TODO cut on function
				if ((*it)->name == "easyEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					if ((*it)->properties.pos.x > p.properties.pos.x)
						(*it)->properties.pos.x -= 15;
					else
						(*it)->properties.pos.x += 15;
					if ((*it)->properties.pos.y > p.properties.pos.y)
						(*it)->properties.pos.y += 15;
					else
						(*it)->properties.pos.y -= 15;
					if (p.liv_pr.armor > 0)
						p.liv_pr.armor -= (*it)->liv_pr.damage;
					else
						p.liv_pr.health -= (*it)->liv_pr.damage;
					sounds.damage.play();
				}

				if ((*it)->name == "mediumEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					if ((*it)->properties.pos.x > p.properties.pos.x)
						(*it)->properties.pos.x += 15;
					else
						(*it)->properties.pos.x -= 15;
					if ((*it)->properties.pos.y > p.properties.pos.y)
						(*it)->properties.pos.y += 15;
					else
						(*it)->properties.pos.y -= 15;
					if (p.liv_pr.armor > 0)
						p.liv_pr.armor -= (*it)->liv_pr.damage;
					else
						p.liv_pr.health -= (*it)->liv_pr.damage;
					sounds.damage.play();
				}
				if ((*it)->name == "enemyBullet") {
					if (p.liv_pr.armor > 0)
						p.liv_pr.armor -= (*it)->liv_pr.damage;
					else
						p.liv_pr.health -= (*it)->liv_pr.damage;
					(*it)->properties.life = false;
				}
				if ((*it)->name == "Ammo") {
					p.liv_pr.ammo += 2;
					(*it)->properties.life = false;
				}
				if ((*it)->name == "Armor") {
					(*it)->properties.life = false;
					if (p.liv_pr.armor + 20 < 100)
						p.liv_pr.armor += 20;
					else if (p.liv_pr.armor == 100)
						(*it)->properties.life = true;
					else
						p.liv_pr.armor = 100;
				}
				if ((*it)->name == "Health") {
					(*it)->properties.life = false;
					if (p.liv_pr.health + 20 < 100)
						p.liv_pr.health += 20;
					else if (p.liv_pr.health == 100)
						(*it)->properties.life = true;
					else
						p.liv_pr.health = 100;
				}
				if ((*it)->name == "machineGun") {
					p.liv_pr.weapon = 0;
					p.select_weapon(p.liv_pr);
					(*it)->properties.life = false;
					queueShot = 0;
				}
				if ((*it)->name == "shotgun") {
					p.liv_pr.weapon = 1;
					p.select_weapon(p.liv_pr);
					queueShot = 0;
					(*it)->properties.life = false;
				}
				if ((*it)->name == "automatical") {
					p.liv_pr.weapon = 2;
					p.select_weapon(p.liv_pr);
					(*it)->properties.life = false;
					queueShot = 0;
				}
				if ((*it)->name == "rifle") {
					p.liv_pr.weapon = 3;
					p.select_weapon(p.liv_pr);
					(*it)->properties.life = false;
					queueShot = 0;
				}
				if ((*it)->name == "solidExit") {
					p.properties.pos.y -= 13;
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
				(*it)->properties.life = false;
			if ((*it)->properties.pos.x > posView.x - 632 && (*it)->properties.pos.x < posView.x + 632 && (*it)->properties.pos.y > posView.y - 384 && (*it)->properties.pos.y < posView.y + 384) { // ���� ������ ������� �� �����, �� ������
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
		if (!p.properties.life) {
			text.setPosition(posView.x - 100, posView.y - 50);//������ ������� ������, ����� ������
			text.setColor(Color::Red);
			text.setString("GAME OVER");//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		else {
			text.setPosition(posView.x - 610, posView.y + 290);//������ ������� ������, ����� �����
			text.setColor(Color::Red);
			HP = p.liv_pr.health;
			strHP = std::to_string(HP);
			text.setString(strHP);//������ ������ ������
			window.draw(text);//����� ���� �����
			
			if (p.liv_pr.armor > 0) {
				text.setPosition(posView.x - 530, posView.y + 290);//������ ������� ������, ����� �����
				text.setColor(Color::Blue); // �����
				HP = p.liv_pr.armor;
				strHP = std::to_string(HP);
				text.setString(strHP);//������ ������ ������
				window.draw(text);//����� ���� �����]
			}

			text.setPosition(posView.x + 500, posView.y+ 290);//������ ������� ������, ����� �����
			text.setColor(Color::Cyan); // �����
			HP = p.liv_pr.ammo;
			strHP = std::to_string(HP);
			text.setString(strHP);//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		if (p.liv_pr.gameover) {
			window.close();
		}
		window.display(); 
	}
	return 0;
}