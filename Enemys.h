#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Enemy :public Entity {
public:
	float CurrentFrame;
	float rotation;
	Enemy(Image &image, Image &gunImage, Level &lvl, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		timeDeath = 0;
		obj = lvl.GetObjects("solid");
		w = W; h = H; wGun = Wgun; hGun = Hgun;
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		enemyShot = 0;
		if (name == "easyEnemy") {
			speed = 0.03f; health = 50; armor = 0; damage = 1;
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		if (name == "mediumEnemy") {
			speed = 0.06f; health = 100; armor = 0; damage = 2;
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		if (name == "bandit") {
			speed = 0.1f; health = 100; armor = 100; damage = 15;
			sprite.setTextureRect(IntRect(0, 0, w, h));
			gunSprite.setTextureRect(IntRect(0, 30, wGun, hGun));
		}
	}

	void checkCollisionWithMap(float Dx, float Dy) {//ф ция проверки столкновений с картой
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { y -= 10; }
					if (Dy<0) { y += 10; }
					if (Dx > 0) { x -= 10; }
					if (Dx < 0) { x += 10; }
				}
			}
	}

	void update(float time, float posPlayerX, float posPlayerY) {
		if (name == "easyEnemy") {//для персонажа с таким именем логика будет такой
			CurrentFrame += 0.0005f*time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			sprite.setTextureRect(IntRect(0, h * int(CurrentFrame), w, h));
			dx = posPlayerX - x;
			dy = posPlayerY - y;
			if (dx > 0) {
				x += speed* time;
			}
			else if (dx < 0)
				x -= speed* time;
			if (dy > 0) {
				y += speed* time;
			}
			else if (dy < 0)
				y -= speed* time;
			rotation = (atan2(dx, dy)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
			sprite.setRotation(-rotation);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
		if (name == "mediumEnemy") {
			CurrentFrame += 0.001f*time;
			if (CurrentFrame > 2) CurrentFrame -= 2;
			sprite.setTextureRect(IntRect(0, h * int(CurrentFrame), w, h));
			dx = posPlayerX - x;
			dy = posPlayerY - y;
			if (dx > 0) {
				x += speed * time;
			}
			else if (dx < 0)
				x -= speed * time;
			if (dy > 0) {
				y += speed * time;
			}
			else if (dy < 0)
				y -= speed * time;
			rotation = (atan2(dx, dy)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
										  //x += speed * (posPlayerX - x);
										  //y += speed * (posPlayerY - y);
			sprite.setRotation(-rotation);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
		if (name == "bandit") {
			CurrentFrame += 0.005f*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			sprite.setTextureRect(IntRect(0, h * int(CurrentFrame), w, h));
			dx = posPlayerX - x;
			dy = posPlayerY - y;
			if (dx > 0 && dx > 500) {
				x += speed * time;
			}
			else if (dx < 0 && dx < -500)
				x -= speed* time;
			if (dy > 0 && dy > 300) {
				y += speed* time;
			}
			else if (dy < 0 && dy < -300)
				y -= speed* time;
			rotation = (atan2(dx, dy)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
			sprite.setRotation(-rotation + 90);
			gunSprite.setRotation(-rotation + 90);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			gunSprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};