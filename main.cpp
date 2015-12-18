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


void skip_to_list(std::vector<Entity*>  &entities, Player p, game_indicators &GI, float time) {
	int randBonus;
	Image bonusImage = GI.pict.bonusImage;
	Sprite meetSprite = GI.pict.meetSprite;
	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end();) {//говорим что проходимс€ от начала до конца
		Entity *b = (*it);//дл€ удобства, чтобы не писать (*it)->
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
					b->timeDeath = GI.timeGame + 1;
					it++;
				}
				else if (b->timeDeath < GI.timeGame) {
					it = entities.erase(it); delete b;// если этот объект мертв, то удал€ем его
				}
				else it++;
			}
			else {
				it = entities.erase(it); delete b;
			}
		}
		else {
			b->update(time, p.properties.pos);//вызываем ф-цию update дл€ всех объектов (по сути дл€ тех, кто жив)
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
	if (one->getRect().intersects(two->getRect()) && (one->name == "easyEnemy" || one->name == "mediumEnemy" || one->name == "bandit") && 
		((two->name == "easyEnemy" || two->name == "mediumEnemy" || two->name == "bandit"))) {
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
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 0;
		p.liv_pr.ammo = 100;
		p.liv_pr.main_damage = 15;
		p.select_weapon(p.liv_pr);
		one->properties.life = false;
		p.queueShot = 0;
	}
	else if (one->name == "shotgun") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 1;
		p.liv_pr.ammo = 4;
		p.liv_pr.main_damage = 30;
		p.select_weapon(p.liv_pr);
		p.queueShot = 0;
		one->properties.life = false;
	}
	else if (one->name == "automatical") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 2;
		p.liv_pr.ammo = 30;
		p.liv_pr.main_damage = 20;
		p.select_weapon(p.liv_pr);
		one->properties.life = false;
		p.queueShot = 0;
	}
	else if (one->name == "rifle") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 3;
		p.liv_pr.ammo = 10;
		p.liv_pr.main_damage = 150;
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
		p.properties.pos.y -= p.properties.sizeHero.y / 2;
	}
}
void check_clashes(std::vector<Entity*>  &entities, Player &p, RenderWindow &window, game_indicators &GI) {
	for (auto i : entities) {//проходимс€ по эл-там списка TODO cut on function
		for (auto j : entities) {
			damage_player_to_enemys(i, j);
			clashes_enemys(i, j);
		}
		if (i->getRect().intersects(p.getRect())) {//если пр€моугольник спрайта объекта пересекаетс€ с игроком TODO cut on function
			damage_enemys_to_player(i, p, GI.sounds);
			pick_up_weapon(i, p);
			pick_up_bonuses_and_exit(i, p);
			if (i->name == "exit" && GI.areaClean)
				window.close();
		}
	}
}

void shooting_enemy(std::vector<Entity*>  &entities, Player p, game_indicators &GI) {
	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->name == "bandit" && ((*it)->properties.dist.x <= 500 || (*it)->properties.dist.x <= -500) && ((*it)->properties.dist.y < 300 || (*it)->properties.dist.y < 300) && (*it)->enemyShot < GI.timeGame) {//проверка находитс€ ли бандит на рассто€ннии выстрела и может ли он сделать выстрел
			Vector2f sizeHero = { 23, 7 };
			Vector2f posPl = { (*it)->properties.pos.x + (*it)->properties.sizeHero.y / 2, (*it)->properties.pos.y + (*it)->properties.sizeHero.x / 2 };
			
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, p.properties.pos, "enemyBullet", (*it)->liv_pr.damage, 0));
			(*it)->enemyShot = GI.timeGame + 1;
		}
	}
}

void player_shotting(Event event, std::vector<Entity*>  &entities, Player &p, game_indicators &GI) {
	if (p.isShoot && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
		p.isShoot = false;
	Vector2f posPl = { p.properties.pos.x + p.properties.sizeHero.y / 2, p.properties.pos.y + p.properties.sizeHero.x / 2 };
	Vector2f sizeHero = { 23, 7 };
	if (p.liv_pr.num_weapon == 1) {
		if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || p.isShoot) && p.liv_pr.main_weapon == 0 && p.liv_pr.ammo > 0 && GI.timeGame > p.queueShot) { //TODO cut on function
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.liv_pr.ammo -= 1;
			p.isShoot = true;
			p.queueShot = GI.timeGame + 0.1;
			GI.sounds.shootMachine.play();
		}
		else if (event.key.code == Mouse::Left && p.liv_pr.main_weapon == 1 && p.liv_pr.ammo > 0 && GI.timeGame > p.queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 1));
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 2));
			p.liv_pr.ammo -= 1;
			p.queueShot = GI.timeGame + 1;
			GI.sounds.shootShootGun.play();
		}
		else if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || p.isShoot) && p.liv_pr.main_weapon == 2 && p.liv_pr.ammo > 0 && GI.timeGame > p.queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.liv_pr.ammo -= 1;
			p.isShoot = true;
			p.queueShot = GI.timeGame + 0.1;
			GI.sounds.shootMachine.play();
		}
		else if (event.key.code == Mouse::Left && p.liv_pr.main_weapon == 3 && p.liv_pr.ammo > 0 && GI.timeGame > p.queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.liv_pr.ammo -= 1;
			p.queueShot = GI.timeGame + 2;
			GI.sounds.shootRifle.play();
		}
		else if (event.key.code == Mouse::Left)
			GI.sounds.shootReload.play();
	}
	else if (p.liv_pr.num_weapon == 2) {
		if (event.key.code == Mouse::Left && GI.timeGame > p.queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.queueShot = GI.timeGame + 0.2;
			GI.sounds.shootRifle.play();
		}
	}
}

void draw_objects(std::vector<Entity*>  entities, game_indicators &GI, RenderWindow &window) {
	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->name == "bandit" || (*it)->name == "easyEnemy" || (*it)->name == "mediumEnemy") {
			GI.countEnemy += 1;
		}
		if (GI.areaClean && (*it)->name == "solidExit")
			(*it)->properties.life = false;
		if ((*it)->properties.pos.x > GI.posView.x - 632 && (*it)->properties.pos.x < GI.posView.x + 632 && (*it)->properties.pos.y > GI.posView.y - 384 && (*it)->properties.pos.y < GI.posView.y + 384) { // если объект выходит за экран, не рисуем
			window.draw((*it)->gunSprite);
			if ((*it)->name == "exit") {
				if (GI.areaClean)
					window.draw((*it)->sprite);
			}
			else
				window.draw((*it)->sprite);
		}
	}
}
void start_game() {
	game_indicators GI;
	std::vector<Entity*>  entities;
	GI.sounds.music.play();

	sf::RenderWindow window(sf::VideoMode(int(WINDOW_SIZE.x), int(WINDOW_SIZE.y)), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));//размер "вида" камеры при создании объекта вида камеры. (потом можем мен€ть как хотим) „то то типа инициализации.

	

	init_objects_in_map(entities, GI);

	
	Vector2f posPl = { GI.player.rect.left, GI.player.rect.top };
	Vector2f sizeHero = { GI.player.rect.height, GI.player.rect.width };
	Vector2f sizeGun = { 72, 15 };
	Player p(GI.pict.heroImage, GI.pict.gunImage, GI.lvl, posPl, sizeHero, sizeGun, "Player");//создаем объект p класса player,задаем "hero.png" как им€ файла+расширение, далее координата ’,”, ширина, высота.
	Clock clock;
	Clock gameTime;

	while (window.isOpen()) {
		std::vector<Entity*>::iterator at;//второй итератор.дл€ взаимодействи€ между объектами списка
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		GI.pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		float time = float(clock.getElapsedTime().asMicroseconds());
		GI.timeGame = gameTime.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == Keyboard::M) {
				init_objects_in_map(entities, GI);
			}
			player_shotting(event, entities, p, GI);
			
		}
		p.rotation_GG(GI.pos);
		if (p.properties.life)
			p.update(time, p.properties.pos);// Player update function


		skip_to_list(entities, p, GI, time);
		shooting_enemy(entities, p, GI);
		check_clashes(entities, p, window, GI);

		GI.countEnemy = 0;
		window.setView(view);
		window.clear();
		GI.posView = view.getCenter();
		GI.lvl.Draw(window);//рисуем новую карту
		draw_objects(entities, GI, window);
		window.draw(p.gunSprite);
		window.draw(p.sprite);
		GI.font.text_albion.setPosition(GI.posView.x + 600, GI.posView.y - 360);
		GI.font.HP = GI.countEnemy;
		GI.font.strHP = std::to_string(GI.font.HP);
		GI.font.text_albion.setColor(Color::Blue);
		GI.font.text_albion.setString(GI.font.strHP);
		window.draw(GI.font.text_albion);
		if (GI.countEnemy == 0) { //TODO cut on function
			//sounds.music.stop();
			GI.areaClean = true;
			GI.font.text_albion.setPosition(GI.posView.x - 100, GI.posView.y - 50);//задаем позицию текста, центр камеры
			GI.font.text_albion.setColor(Color::Blue);
			GI.font.text_albion.setString("AREA CLEAN");//задает строку тексту
			window.draw(GI.font.text_albion);//рисую этот текст
		}
		if (!p.properties.life) {
			GI.font.text_albion.setPosition(GI.posView.x - 100, GI.posView.y - 50);//задаем позицию текста, центр камеры
			GI.font.text_albion.setColor(Color::Red);
			GI.font.text_albion.setString("GAME OVER");//задает строку тексту
			window.draw(GI.font.text_albion);//рисую этот текст
		}
		else {
			GI.font.text_albion.setPosition(GI.posView.x - 610, GI.posView.y + 290);//задаем позицию текста, слева внизу
			GI.font.text_albion.setColor(Color::Red);
			GI.font.HP = p.liv_pr.health;
			GI.font.strHP = std::to_string(GI.font.HP);
			GI.font.text_albion.setString(GI.font.strHP);//задает строку тексту
			window.draw(GI.font.text_albion);//рисую этот текст

			if (p.liv_pr.armor > 0) {
				GI.font.text_albion.setPosition(GI.posView.x - 530, GI.posView.y + 290);//задаем позицию текста, слева внизу
				GI.font.text_albion.setColor(Color::Blue); // брон€
				GI.font.HP = p.liv_pr.armor;
				GI.font.strHP = std::to_string(GI.font.HP);
				GI.font.text_albion.setString(GI.font.strHP);//задает строку тексту
				window.draw(GI.font.text_albion);//рисую этот текст]
			}

			GI.font.text_albion.setPosition(GI.posView.x + 500, GI.posView.y + 290);//задаем позицию текста, слева внизу
			GI.font.text_albion.setColor(Color::Cyan); // брон€
			
			if (p.liv_pr.num_weapon != 1) {
				GI.font.strHP = "unlimit";
			}
			else {
				GI.font.HP = p.liv_pr.ammo;
				GI.font.strHP = std::to_string(GI.font.HP);
			}

			GI.font.text_albion.setString(GI.font.strHP);//задает строку тексту
			window.draw(GI.font.text_albion);//рисую этот текст
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