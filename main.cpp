#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include "TinyXML\tinyxml.h"
#include "level.h"
#include <vector>
#include "Classes.h"
#include "LoadFiles.h"
#include "InitObjOnMap.h" 
#include "ActionEnemyAndOtherObj.h"
#include "ActionPlayer.h"

using namespace sf;

static const sf::Vector2f WINDOW_SIZE = { 1370, 768 };

void check_clashes_all(std::vector<Entity*>  &entities, Player &p, RenderWindow &window, game_indicators &GI) {
	for (auto i : entities) {//проходимся по эл-там списка TODO cut on function
		for (auto j : entities) {
			damage_player_to_enemys(i, j);
			clashes_enemys(i, j);
		}
		if (i->getRect().intersects(p.getRect())) {//если прямоугольник спрайта объекта пересекается с игроком TODO cut on function
			damage_enemys_to_player(i, p, GI.sounds);
			pick_up_weapon(i, p);
			pick_up_bonuses_and_exit(i, p);
			if (i->name == "exit" && GI.areaClean)
				window.close();
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

void set_param_text(game_indicators &GI, Vector2f pos_text, String strHP, Color col){
	GI.font.text_albion.setPosition(pos_text.x, pos_text.y);
	GI.font.text_albion.setColor(col);
	GI.font.text_albion.setString(strHP);
}

void draw_text(RenderWindow &window, game_indicators &GI, Player p) {
	Vector2f posit = { GI.posView.x + 600, GI.posView.y - 360 };
	int HP = GI.countEnemy;
	String strHP = std::to_string(HP);
	set_param_text(GI, posit, strHP, Color::Blue);
	window.draw(GI.font.text_albion);
	posit = { GI.posView.x - 100, GI.posView.y - 50 };
	if (GI.countEnemy == 0) { //TODO cut on function
		GI.areaClean = true;
		set_param_text(GI, posit, "AREA CLEAN", Color::Blue);
		window.draw(GI.font.text_albion);//рисую этот текст
	}
	if (!p.properties.life) {
		set_param_text(GI, posit, "GAME OVER", Color::Red);
		window.draw(GI.font.text_albion);//рисую этот текст
	}
	else {
		posit = { GI.posView.x - 610, GI.posView.y + 290 };//задаем позицию текста, слева внизу
		HP = p.liv_pr.health;
		strHP = std::to_string(HP);
		set_param_text(GI, posit, strHP, Color::Red);
		window.draw(GI.font.text_albion);//рисую этот текст

		if (p.liv_pr.armor > 0) {
			posit = { GI.posView.x - 530, GI.posView.y + 290 };//задаем позицию текста, слева внизу
			HP = p.liv_pr.armor;
			strHP = std::to_string(HP);
			set_param_text(GI, posit, strHP, Color::Blue);
			window.draw(GI.font.text_albion);//рисую этот текст]
		}
		posit = { GI.posView.x + 500, GI.posView.y + 290 };//задаем позицию текста, слева внизу
		if (p.liv_pr.num_weapon != 1) {
			strHP = "unlimit";
		}
		else {
			HP = p.liv_pr.ammo;
			strHP = std::to_string(HP);
		}
		set_param_text(GI, posit, strHP, Color::Cyan);
		window.draw(GI.font.text_albion);//рисую этот текст
	}
}


void ALL_draw(RenderWindow &window, game_indicators &GI, Player &p, std::vector<Entity*>  entities) {
	GI.countEnemy = 0;
	window.setView(view);
	window.clear();
	GI.posView = view.getCenter();
	GI.lvl.Draw(window);
	draw_objects(entities, GI, window);
	window.draw(p.gunSprite);
	window.draw(p.sprite);
	draw_text(window, GI, p);
	if (p.liv_pr.gameover) {
		window.close();
	}
	window.display();
}

void start_game() {
	game_indicators GI;
	std::vector<Entity*>  entities;
	GI.sounds.music.play();

	sf::RenderWindow window(sf::VideoMode(int(WINDOW_SIZE.x), int(WINDOW_SIZE.y)), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.
	init_objects_in_map(entities, GI);

	Vector2f posPl = { GI.player.rect.left, GI.player.rect.top };
	Vector2f sizeHero = { GI.player.rect.height, GI.player.rect.width };
	Vector2f sizeGun = { 72, 15 };
	Player p(GI.pict.heroImage, GI.pict.gunImage, GI.lvl, posPl, sizeHero, sizeGun, "Player");//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	Clock clock;
	Clock gameTime;

	while (window.isOpen()) {
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
			player_shotting(event, entities, p, GI);
		}

		p.rotation_GG(GI.pos);
		if (p.properties.life)
			p.update(time, p.properties.pos);

		skip_to_list(entities, p, GI, time);
		shooting_enemy(entities, p, GI);
		check_clashes_all(entities, p, window, GI);

		ALL_draw(window, GI, p, entities);
	}
}

int main() {
	start_game();
	return 0;
}