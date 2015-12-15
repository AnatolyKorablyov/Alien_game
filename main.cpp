#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include "TinyXML\tinyxml.h"
#include "level.h"
#include <vector>
#include "Classes.h"
#include "LoadFiles.h"
#include "InitObjOnMap.h"

using namespace sf;

static const sf::Vector2f WINDOW_SIZE = { 1370, 768 };



void skip_to_list(std::vector<Entity*>::iterator it, std::vector<Entity*>  &entities, Player p, int &randBonus, Image bonusImage, Sprite meetSprite, float time, float timeGame) {
	for (it = entities.begin(); it != entities.end();) {//говорим что проходимся от начала до конца
		Entity *b = (*it);//для удобства, чтобы не писать (*it)->
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
					it = entities.erase(it); delete b;// если этот объект мертв, то удаляем его
				}
				else it++;
			}
			else {
				it = entities.erase(it); delete b;
			}
		}
		else {
			b->update(time, p.properties.pos.x, p.properties.pos.y);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
	}
}

void damage_player_to_enemys(Entity* &one, Entity* &two) {
	if (one->getRect().intersects(two->getRect()) && ((two->name == "Bullet") && (one->name == "easyEnemy"))) {
		one->liv_pr.health -= two->liv_pr.damage;
		two->properties.life = false;
	}
	if (one->getRect().intersects(two->getRect()) && ((two->name == "Bullet") && (one->name == "mediumEnemy"))) {
		one->liv_pr.health -= two->liv_pr.damage;
		two->properties.life = false;
	}
	if (one->getRect().intersects(two->getRect()) && ((two->name == "Bullet") && (one->name == "bandit"))) {
		if (one->liv_pr.armor > 0)
			one->liv_pr.armor -= two->liv_pr.damage;
		else
			one->liv_pr.health -= two->liv_pr.damage;
		two->properties.life = false;
	}
}

void clashes_enemys(Entity* &one, Entity* &two) {
	if (one->getRect().intersects(two->getRect())) {
		if (one->properties.pos.x > two->properties.pos.x) {
			one->properties.pos.x -= 5;
			two->properties.pos.x += 5;
		}
		else {
			one->properties.pos.x += 5;
			two->properties.pos.x -= 5;
		}
		if (one->properties.pos.y > two->properties.pos.x) {
			one->properties.pos.y += 5;
			two->properties.pos.y -= 5;
		}
		else {
			one->properties.pos.y -= 5;
			two->properties.pos.y += 5;
		}
	}
}

void damage_enemys_to_player(Entity* &one, Player &p, init_sounds &sounds) {
	if (one->name == "easyEnemy" || one->name == "mediumEnemy") {
		if (one->properties.pos.x > p.properties.pos.x)
			one->properties.pos.x -= 15;
		else
			one->properties.pos.x += 15;
		if (one->properties.pos.y > p.properties.pos.y)
			one->properties.pos.y += 15;
		else
			one->properties.pos.y -= 15;
		if (p.liv_pr.armor > 0)
			p.liv_pr.armor -= one->liv_pr.damage;
		else
			p.liv_pr.health -= one->liv_pr.damage;
		sounds.damage.play();
	}
	if (one->name == "enemyBullet") {
		if (p.liv_pr.armor > 0)
			p.liv_pr.armor -= one->liv_pr.damage;
		else
			p.liv_pr.health -= one->liv_pr.damage;
		one->properties.life = false;
	}
}

void pick_up_weapon(Entity* &one, Player &p) {
	if (one->name == "machineGun") {
		p.liv_pr.weapon = 0;
		p.select_weapon(p.liv_pr);
		one->properties.life = false;
		p.queueShot = 0;
	}
	if (one->name == "shotgun") {
		p.liv_pr.weapon = 1;
		p.select_weapon(p.liv_pr);
		p.queueShot = 0;
		one->properties.life = false;
	}
	if (one->name == "automatical") {
		p.liv_pr.weapon = 2;
		p.select_weapon(p.liv_pr);
		one->properties.life = false;
		p.queueShot = 0;
	}
	if (one->name == "rifle") {
		p.liv_pr.weapon = 3;
		p.select_weapon(p.liv_pr);
		one->properties.life = false;
		p.queueShot = 0;
	}
}

void pick_up_bonuses_and_exit(Entity* &one, Player &p) {
	if (one->name == "Ammo") {
		p.liv_pr.ammo += 2;
		one->properties.life = false;
	}
	if (one->name == "Armor") {
		one->properties.life = false;
		if (p.liv_pr.armor + 20 < 100)
			p.liv_pr.armor += 20;
		else if (p.liv_pr.armor == 100)
			one->properties.life = true;
		else
			p.liv_pr.armor = 100;
	}
	if (one->name == "Health") {
		one->properties.life = false;
		if (p.liv_pr.health + 20 < 100)
			p.liv_pr.health += 20;
		else if (p.liv_pr.health == 100)
			one->properties.life = true;
		else
			p.liv_pr.health = 100;
	}
	if (one->name == "solidExit") {
		p.properties.pos.y -= 20;
	}
}
void check_clashes(std::vector<Entity*>  &entities, Player &p, init_sounds &sounds, RenderWindow &window, bool areaClean) {
	for (auto i : entities) {//проходимся по эл-там списка TODO cut on function
		for (auto j : entities) {
			damage_player_to_enemys(i, j);
			clashes_enemys(i, j);
		}
		if (i->getRect().intersects(p.getRect())) {//если прямоугольник спрайта объекта пересекается с игроком TODO cut on function
			damage_enemys_to_player(i, p, sounds);
			pick_up_weapon(i, p);
			pick_up_bonuses_and_exit(i, p);
			if (i->name == "exit" && areaClean)
				window.close();
		}
	}
}

void shooting_enemy(std::vector<Entity*>::iterator it, std::vector<Entity*>  &entities, Player p, Image bulletImage, Level lvl, float timeGame) {
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->name == "bandit" && ((*it)->properties.dx <= 500 || (*it)->properties.dx <= -500) && ((*it)->properties.dy < 300 || (*it)->properties.dy < 300) && (*it)->enemyShot < timeGame) {//проверка находится ли бандит на расстояннии выстрела и может ли он сделать выстрел
			Vector2f sizeHero = { 23, 7 };
			Vector2f posPl = { (*it)->properties.pos.x + (*it)->properties.sizeHero.y / 2, (*it)->properties.pos.y + (*it)->properties.sizeHero.x / 2 };
			
			entities.push_back(new Bullet(bulletImage, lvl, posPl, sizeHero, p.properties.pos, "enemyBullet", (*it)->liv_pr.damage, 0));
			(*it)->enemyShot = timeGame + 1;
		}
	}
}


void start_game() {
	int randBonus;
	float queueShot = 0, enemyShot = 0;
	int countEnemy = 0;

	bool areaClean = false;
	Music music;
	music.openFromFile("sounds/mus01.ogg");
	//music.play();

	sf::Vector2f posView;

	init_sounds sounds;
	init_picture pict;
	int HP;
	String strHP;
	Font font;//шрифт 
	font.loadFromFile("images/Albionic.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый

	sf::RenderWindow window(sf::VideoMode(int(WINDOW_SIZE.x), int(WINDOW_SIZE.y)), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него

	std::vector<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::vector<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка


	

	init_objects_in_map(lvl, entities, pict);

	Texture meetTexture;
	meetTexture.loadFromImage(pict.meetImage);
	Sprite meetSprite;
	meetSprite.setTexture(meetTexture);
	Vector2f posPl = { player.rect.left, player.rect.top };
	Vector2f sizeHero = { player.rect.height, player.rect.width };
	Vector2f sizeGun = { 72, 15 };
	Player p(pict.heroImage, pict.gunImage, lvl, posPl, sizeHero, sizeGun, "Player");//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	Clock clock;
	Clock gameTime;

	bool ret = false;
	bool isShoot = false;
	float CurrentFrame = 0;//хранит текущий кадр
	while (window.isOpen()) {
		std::vector<Entity*>::iterator at;//второй итератор.для взаимодействия между объектами списка
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		float time = float(clock.getElapsedTime().asMicroseconds());
		float timeGame = gameTime.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (isShoot && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				isShoot = false;
			posPl = { p.properties.pos.x + p.properties.sizeHero.y / 2, p.properties.pos.y + p.properties.sizeHero.x / 2 };
			sizeHero = { 23, 7 };
			if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || isShoot) && p.liv_pr.weapon == 0 && p.liv_pr.ammo > 0) { //TODO cut on function
				entities.push_back(new Bullet(pict.bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				p.liv_pr.ammo -= 1;
				isShoot = true;
				sounds.shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.liv_pr.weapon == 1 && p.liv_pr.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(pict.bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				entities.push_back(new Bullet(pict.bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 1));
				entities.push_back(new Bullet(pict.bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 2));
				p.liv_pr.ammo -= 1;
				queueShot = timeGame + 1;
				sounds.shootShootGun.play();
			}
			else if (event.key.code == Mouse::Left && p.liv_pr.weapon == 2 && p.liv_pr.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(pict.bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
				p.liv_pr.ammo -= 1;
				queueShot = timeGame + 0.25f;
				sounds.shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.liv_pr.weapon == 3 && p.liv_pr.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(pict.bulletImage, lvl, posPl, sizeHero, pos, "Bullet", p.liv_pr.damage, 0));
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


		skip_to_list(it, entities, p, randBonus, pict.bonusImage, meetSprite, time, timeGame);
		shooting_enemy(it, entities, p, pict.bulletImage, lvl, timeGame);
		check_clashes(entities, p, sounds, window, areaClean);

		countEnemy = 0;
		window.setView(view);
		window.clear();
		posView = view.getCenter();
		lvl.Draw(window);//рисуем новую карту
		for (it = entities.begin(); it != entities.end(); it++) { // TODO cut on function
			if ((*it)->name == "bandit" || (*it)->name == "easyEnemy" || (*it)->name == "mediumEnemy") {
				countEnemy += 1;
			}
			if (areaClean && (*it)->name == "solidExit")
				(*it)->properties.life = false;
			if ((*it)->properties.pos.x > posView.x - 632 && (*it)->properties.pos.x < posView.x + 632 && (*it)->properties.pos.y > posView.y - 384 && (*it)->properties.pos.y < posView.y + 384) { // если объект выходит за экран, не рисуем
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
			text.setPosition(posView.x - 100, posView.y - 50);//задаем позицию текста, центр камеры
			text.setColor(Color::Blue);
			text.setString("AREA CLEAN");//задает строку тексту
			window.draw(text);//рисую этот текст
		}
		if (!p.properties.life) {
			text.setPosition(posView.x - 100, posView.y - 50);//задаем позицию текста, центр камеры
			text.setColor(Color::Red);
			text.setString("GAME OVER");//задает строку тексту
			window.draw(text);//рисую этот текст
		}
		else {
			text.setPosition(posView.x - 610, posView.y + 290);//задаем позицию текста, слева внизу
			text.setColor(Color::Red);
			HP = p.liv_pr.health;
			strHP = std::to_string(HP);
			text.setString(strHP);//задает строку тексту
			window.draw(text);//рисую этот текст

			if (p.liv_pr.armor > 0) {
				text.setPosition(posView.x - 530, posView.y + 290);//задаем позицию текста, слева внизу
				text.setColor(Color::Blue); // броня
				HP = p.liv_pr.armor;
				strHP = std::to_string(HP);
				text.setString(strHP);//задает строку тексту
				window.draw(text);//рисую этот текст]
			}

			text.setPosition(posView.x + 500, posView.y + 290);//задаем позицию текста, слева внизу
			text.setColor(Color::Cyan); // броня
			HP = p.liv_pr.ammo;
			strHP = std::to_string(HP);
			text.setString(strHP);//задает строку тексту
			window.draw(text);//рисую этот текст
		}
		if (p.liv_pr.gameover) {
			window.close();
		}
		window.display();
	}
}

int main() {
	start_game();
	return 0;
}