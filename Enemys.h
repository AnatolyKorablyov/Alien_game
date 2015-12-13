#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Enemy :public Entity {
public:
	float CurrentFrame;
	float rotation;
	Enemy(Image &image, Image &gunImage, Level &lvl, Vector2f coords, FloatRect sizeHeros, Vector2f sizeGuns, String Name) :Entity(image, coord, sizeHero, Name) {
		coord = coords;
		sizeHero = sizeHeros;
		sizeGun = sizeGuns;
		timeDeath = 0;
		obj = lvl.GetObjects("solid");
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		enemyShot = 0;
		if (name == "easyEnemy") {
			speed = 0.03f; health = 50; armor = 0; damage = 1;
			sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
		}
		if (name == "mediumEnemy") {
			speed = 0.06f; health = 100; armor = 0; damage = 2;
			sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
		}
		if (name == "bandit") {
			speed = 0.1f; health = 100; armor = 100; damage = 15;
			sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
			gunSprite.setTextureRect(IntRect(0, 30, sizeGun.x, sizeGun.y));
		}
	}

	void checkCollisionWithMap(float Dx, float Dy) {//ф ция проверки столкновений с картой
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { coord.y -= 10; }
					if (Dy<0) { coord.y += 10; }
					if (Dx > 0) { coord.x -= 10; }
					if (Dx < 0) { coord.x += 10; }
				}
			}
	}

	void update(float time, Vector2f coords) override{
		if (name == "easyEnemy") {//для персонажа с таким именем логика будет такой
			CurrentFrame += 0.0005f*time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			sprite.setTextureRect(IntRect(0, sizeHero.height * int(CurrentFrame), sizeHero.width, sizeHero.height));
			dSP.x = coords.x - coord.x;
			dSP.y = coords.y - coord.y;
			if (dSP.x > 0) {
				coord.x += speed* time;
			}
			else if (dSP.x < 0)
				coord.x -= speed* time;
			if (dSP.y > 0) {
				coord.y += speed* time;
			}
			else if (dSP.y < 0)
				coord.y -= speed* time;
			rotation = (atan2(dSP.x, dSP.y)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dSP.x, dSP.y);//обрабатываем столкновение по Х
			sprite.setRotation(-rotation);
			sprite.setPosition(coord.x + sizeHero.width / 2, coord.y + sizeHero.height / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
		if (name == "mediumEnemy") {
			CurrentFrame += 0.001f*time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			sprite.setTextureRect(IntRect(0, sizeHero.height * int(CurrentFrame), sizeHero.width, sizeHero.height));
			dSP.x = coords.x - coord.x;
			dSP.y = coords.y - coord.y;
			if (dSP.x > 0) {
				coord.x += speed * time;
			}
			else if (dSP.x < 0)
				coord.x -= speed * time;
			if (dSP.y > 0) {
				coord.y += speed * time;
			}
			else if (dSP.y < 0)
				coord.y -= speed * time;
			rotation = (atan2(dSP.x, dSP.y)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dSP.x, dSP.y);//обрабатываем столкновение по Х
										  //coord.x += speed * (posPlayerX - coord.x);
										  //coord.y += speed * (posPlayerY - coord.y);
			sprite.setRotation(-rotation);
			sprite.setPosition(coord.x + sizeHero.width / 2, coord.y + sizeHero.height / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
		if (name == "bandit") {
			CurrentFrame += 0.005f*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(0, sizeHero.height * int(CurrentFrame), sizeHero.width, sizeHero.height));
			dSP.x = coords.x - coord.x;
			dSP.y = coords.y - coord.y;
			if (dSP.x > 0 && dSP.x > 500) {
				coord.x += speed * time;
			}
			else if (dSP.x < 0 && dSP.x < -500)
				coord.x -= speed* time;
			if (dSP.y > 0 && dSP.y > 300) {
				coord.y += speed* time;
			}
			else if (dSP.y < 0 && dSP.y < -300)
				coord.y -= speed* time;
			rotation = (atan2(dSP.x, dSP.y)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dSP.x, dSP.y);//обрабатываем столкновение по Х
			sprite.setRotation(-rotation + 90);
			gunSprite.setRotation(-rotation + 90);
			sprite.setPosition(coord.x + sizeHero.width / 2, coord.y + sizeHero.height / 2); //задаем позицию спрайта в место его центра
			gunSprite.setPosition(coord.x + sizeHero.width / 2, coord.y + sizeHero.height / 2);
			if (health <= 0) { life = false; }
		}
	}
};