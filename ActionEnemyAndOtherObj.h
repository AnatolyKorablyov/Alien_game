#pragma once
#include "InitObjOnMap.h" 

void skip_to_list(std::vector<Entity*>  &entities, Player p, game_indicators &GI, float time) {
	int randBonus;
	Image bonusImage = GI.pict.bonusImage;
	Sprite meetSprite = GI.pict.meetSprite;
	std::vector<Entity*>::iterator it;
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
					b->timeDeath = GI.timeGame + 1;
					it++;
				}
				else if (b->timeDeath < GI.timeGame) {
					it = entities.erase(it); delete b;// если этот объект мертв, то удаляем его
				}
				else it++;
			}
			else {
				it = entities.erase(it); delete b;
			}
		}
		else {
			b->update(time, p.properties.pos);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
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

void create_enemy(std::vector<Entity*>  &entities, Player p, game_indicators &GI) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->name == "portal" && entities[i]->timeBurn < GI.timeGame) {
			entities[i]->timeBurn = GI.timeGame + 5;
			init_enemys_in_map(entities, GI, entities[i]->numPortal);
		}
	}
}
void shooting_enemy(std::vector<Entity*>  &entities, Player p, game_indicators &GI) {
	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->name == "bandit" && ((*it)->properties.dist.x <= 500 || (*it)->properties.dist.x <= -500) && ((*it)->properties.dist.y < 300 || (*it)->properties.dist.y < 300) && (*it)->enemyShot < GI.timeGame) {//проверка находится ли бандит на расстояннии выстрела и может ли он сделать выстрел
			Vector2f sizeHero = { 23, 7 };
			Vector2f posPl = { (*it)->properties.pos.x + (*it)->properties.sizeHero.y / 2, (*it)->properties.pos.y + (*it)->properties.sizeHero.x / 2 };
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, p.properties.pos, "enemyBullet", (*it)->liv_pr.damage, 0));
			(*it)->enemyShot = GI.timeGame + 1;
		}
	}
}