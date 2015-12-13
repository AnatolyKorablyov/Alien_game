#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "view.h"

using namespace sf;

class Player :public Entity {
public:
	float CurrentFrame;
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//добавляем тип перечисления - состояние объекта
	int weapon, ammo;
	float rotation;
	bool gameover = false;
	int playerScore;
	Player(Image &image, Image &gunImage, Level &lev, Vector2f coords, FloatRect sizeHeros, Vector2f sizeGuns, String Name) :Entity(image, coords, sizeHeros, Name) {
		coord = coords;
		sizeHero = sizeHeros;
		sizeGun = sizeGuns;
		health = 100; armor = 100; weapon = 1;
		playerScore = 0; state = stay; isSelect = false; obj = lev.GetAllObjects();
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
		}
		select_weapon(weapon);
	}

	void control() {
		bool pressBut = false;
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = left; speed = 0.2f;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = right; speed = 0.2f;
			pressBut = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (pressBut) {
				if (state == right) {
					state = rightUp; speed = 0.2f;
				}
				if (state == left) {
					state = leftUp; speed = 0.2f;
				}
			}
			else {
				state = up; speed = 0.2f;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			if (pressBut) {
				if (state == right) {
					state = rightDown; speed = 0.2f;
				}
				if (state == left) {
					state = leftDown; speed = 0.2f;
				}
			}
			else {
				state = down; speed = 0.2f;
			}
		}
	}
	void rotation_GG(Vector2f pos) {
		float dX = pos.x - coord.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор
		float dY = pos.y - coord.y;//он же, координата y
		rotation = (atan2(dY, dX)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
	}
	void select_weapon(int wp) {
		if (wp == 0) {
			gunSprite.setTextureRect(IntRect(0, 0, 72, 15));
			ammo = 100;
			damage = 15;
		}
		else if (wp == 1) {
			ammo = 4;
			damage = 30;
			gunSprite.setTextureRect(IntRect(0, 15, 72, 15));
		}
		else if (wp == 2) {
			ammo = 30;
			damage = 20;
			gunSprite.setTextureRect(IntRect(0, 30, 72, 15));
		}
		else if (wp == 3) {
			ammo = 10;
			damage = 150;
			gunSprite.setTextureRect(IntRect(0, 45, 72, 15));
		}
	}
	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { coord.y -= 20; }
					if (Dy<0) { coord.y += 20; }
					if (Dx>0) { coord.x -= 20; }
					if (Dx<0) { coord.x += 20; }
				}
			}

	}

	void update(float time, Vector2f coords) override {
		CurrentFrame += 0.005f*time;
		if (CurrentFrame > 4) CurrentFrame -= 4;
		sprite.setTextureRect(IntRect(0, sizeHero.height * int(CurrentFrame), sizeHero.width, sizeHero.height));
		sprite.setRotation(rotation);//поворачиваем спрайт на эти градусы
		gunSprite.setRotation(rotation);
		control();//функция управления персонажем
		switch (state){
		case right:dSP.x = speed; dSP.y = 0; break;
		case rightUp: dSP.x = speed; dSP.y = -speed; break;
		case rightDown: dSP.x = speed; dSP.y = speed; break;
		case left:dSP.x = -speed; dSP.y = 0; break;
		case leftUp: dSP.x = -speed; dSP.y = -speed; break;
		case leftDown: dSP.x = -speed; dSP.y = speed; break;
		case up: dSP.x = 0; dSP.y = -speed; break;
		case down: dSP.x = 0; dSP.y = speed; break;
		case stay: break;
		}
		coord.x += dSP.x*time;
		coord.y += dSP.y*time;
		checkCollisionWithMap(dSP.x, dSP.y);//обрабатываем столкновение по Х
		//checkCollisionWithMap(0, dSP.y);//обрабатываем столкновение по coord.y
		sprite.setPosition(coord.x + sizeHero.width / 2, coord.y + sizeHero.height / 2); //задаем позицию спрайта в место его центра
		gunSprite.setPosition(coord.x + sizeHero.width / 2, coord.y + sizeHero.height / 2);

		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		if (life) { getPlayerCoordinateForView(coord.x, coord.y); }
	}
};