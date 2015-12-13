#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"

using namespace sf;

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	Vector2f dSP;
	Vector2f coord;
	float speed;
	int health, armor;
	FloatRect sizeHero;
	Vector2f sizeGun;
	bool life, isMove, isSelect;
	int damage;
	float enemyShot, timeDeath;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image &image, Vector2f coords, FloatRect size, String Name) {
		coord = coords;
		sizeHero = size;
		name = Name;
		dSP.x = 0;
		dSP.y = 0;
		speed = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(float(sizeHero.width / 2), float(sizeHero.height / 2));
	}
	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(coord.x, coord.y, sizeHero.height, sizeHero.width);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time, Vector2f coords) = 0;
};