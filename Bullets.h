#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Bullet :public Entity {//класс пули
public:
	int direction, numBull;//направление пули
	float rotation, Dx, Dy;
	Vector2f posPl;
	Vector2f coordSt;
	Bullet(Image &image, Level &lvl, Vector2f coords, FloatRect sizeHeros, Vector2f posPlayer, String Name, int damag, int numBullet) :Entity(image, coords, sizeHeros, Name) {//всё так же, только взяли в конце состояние игрока (int dir)
		numBull = numBullet;
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
		damage = damag;
		speed = 0.001f;
		coord = coords;
		sizeHero = sizeHeros;
		posPl = posPlayer;
		life = true;
		coordSt = coord;
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
	}
	void update(float time, Vector2f coords) override {
		coord.x += speed * (posPl.x - coordSt.x) * time;
		coord.y += speed * (posPl.y - coordSt.y) * time;

		if (coord.x <= 0) coord.x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (coord.y <= 0) coord.y = 1;

		for (int i = 0; i < int(obj.size()); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(coord.x + sizeHero.width / 2 + numBull * 10, coord.y + sizeHero.height / 2 + numBull * 10);//задается позицию пуле
	}
};