#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Bullet :public Entity {//класс пули
public:
	int direction, numBull;//направление пули
	float tempy, tempx, rotation, Dx, Dy;
	Bullet(Image &image, Level &lvl, float X, float Y, int W, int H, float tempX, float tempY, String Name, int damag, int numBullet) :Entity(image, X, Y, W, H, Name) {//всё так же, только взяли в конце состояние игрока (int dir)
		numBull = numBullet;
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
		x = X;
		y = Y;
		damage = damag;
		speed = 0.001f;
		tempx = tempX;
		tempy = tempY;
		w = W;
		h = H;
		life = true;
		dx = x;
		dy = y;
		life = true;
		Dx = tempx - x;
		Dy = tempy - y;
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
	}
	void update(float time, float coordX, float coordY) {
		x += speed * (tempx - dx) * time;
		y += speed * (tempy - dy) * time;

		if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) y = 1;

		for (int i = 0; i < int(obj.size()); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2 + numBull * 10, y + h / 2 + numBull * 10);//задается позицию пуле
	}
};