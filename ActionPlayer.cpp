
#include "ActionPlayer.h"

void damage_player_to_enemys(Entity &one, Entity &two) {
	if (one.Intersects(&two) && ((two.name == "Bullet") && (one.name == "easyEnemy"))) {
		one.liv_pr.health -= two.liv_pr.damage;
		two.properties.life = false;
	}
	if (one.Intersects(&two) && ((two.name == "Bullet") && (one.name == "mediumEnemy"))) {
		one.liv_pr.health -= two.liv_pr.damage;
		two.properties.life = false;
	}
	if (one.Intersects(&two) && ((two.name == "Bullet") && (one.name == "portal"))) {
		one.liv_pr.health -= two.liv_pr.damage;
		two.properties.life = false;
	}
	if (one.Intersects(&two) && ((two.name == "Bullet") && (one.name == "bandit"))) {
		if (one.liv_pr.armor > 0)
			one.liv_pr.armor -= two.liv_pr.damage;
		else
			one.liv_pr.health -= two.liv_pr.damage;
		two.properties.life = false;
	}
}

void pick_up_weapon(Entity &one, Player &p) {
	if (one.name == "machineGun") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 0;
		p.liv_pr.ammo = 100;
		p.liv_pr.main_damage = 15;
		p.select_weapon(p.liv_pr);
		one.properties.life = false;
		p.m_queueShot = 0;
	}
	else if (one.name == "shotgun") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 1;
		p.liv_pr.ammo = 4;
		p.liv_pr.main_damage = 30;
		p.select_weapon(p.liv_pr);
		p.m_queueShot = 0;
		one.properties.life = false;
	}
	else if (one.name == "automatical") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 2;
		p.liv_pr.ammo = 30;
		p.liv_pr.main_damage = 20;
		p.select_weapon(p.liv_pr);
		one.properties.life = false;
		p.m_queueShot = 0;
	}
	else if (one.name == "rifle") {
		p.liv_pr.num_weapon = 1;
		p.liv_pr.main_weapon = 3;
		p.liv_pr.ammo = 10;
		p.liv_pr.main_damage = 150;
		p.select_weapon(p.liv_pr);
		one.properties.life = false;
		p.m_queueShot = 0;
	}
}

void pick_up_bonuses_and_exit(Entity &one, Player &p) {
	if (one.name == "Ammo") {
		p.liv_pr.ammo += 2;
		one.properties.life = false;
	}
	if (one.name == "Armor") {
		one.properties.life = false;
		if (p.liv_pr.armor + 20 < 100)
			p.liv_pr.armor += 20;
		else if (p.liv_pr.armor == 100)
			one.properties.life = true;
		else
			p.liv_pr.armor = 100;
	}
	if (one.name == "Health") {
		one.properties.life = false;
		if (p.liv_pr.health + 20 < 100)
			p.liv_pr.health += 20;
		else if (p.liv_pr.health == 100)
			one.properties.life = true;
		else
			p.liv_pr.health = 100;
	}
	if (one.name == "solidExit") {
		p.properties.pos.y -= p.properties.sizeHero.y / 2;
	}
}

void player_shotting(Event event, std::vector<Entity*>  &entities, Player &p, game_indicators &GI) {
	if (p.m_isShoot && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
		p.m_isShoot = false;
	Vector2f posPl = { p.properties.pos.x + p.properties.sizeHero.y / 2, p.properties.pos.y + p.properties.sizeHero.x / 2 };
	Vector2f sizeHero = { 23, 7 };
	if (p.liv_pr.num_weapon == 1) {
		if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || p.m_isShoot) && p.liv_pr.main_weapon == 0 && p.liv_pr.ammo > 0 && GI.timeGame > p.m_queueShot) { //TODO cut on function
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.liv_pr.ammo -= 1;
			p.m_isShoot = true;
			p.m_queueShot = GI.timeGame + 0.1f;
			GI.sounds.shootMachine.play();
		}
		else if (event.key.code == Mouse::Left && p.liv_pr.main_weapon == 1 && p.liv_pr.ammo > 0 && GI.timeGame > p.m_queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 1));
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 2));
			p.liv_pr.ammo -= 1;
			p.m_queueShot = GI.timeGame + 1.0f;
			GI.sounds.shootShootGun.play();
		}
		else if (((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) || p.m_isShoot) && p.liv_pr.main_weapon == 2 && p.liv_pr.ammo > 0 && GI.timeGame > p.m_queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.liv_pr.ammo -= 1;
			p.m_isShoot = true;
			p.m_queueShot = GI.timeGame + 0.1f;
			GI.sounds.shootMachine.play();
		}
		else if (event.key.code == Mouse::Left && p.liv_pr.main_weapon == 3 && p.liv_pr.ammo > 0 && GI.timeGame > p.m_queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.liv_pr.ammo -= 1;
			p.m_queueShot = GI.timeGame + 2.0f;
			GI.sounds.shootRifle.play();
		}
		else if (event.key.code == Mouse::Left)
			GI.sounds.shootReload.play();
	}
	else if (p.liv_pr.num_weapon == 2) {
		if (event.key.code == Mouse::Left && GI.timeGame > p.m_queueShot) {
			entities.push_back(new Bullet(GI.pict.bulletImage, GI.lvl, posPl, sizeHero, GI.pos, "Bullet", p.liv_pr.damage, 0));
			p.m_queueShot = GI.timeGame + 0.2f;
			GI.sounds.shootRifle.play();
		}
	}
}