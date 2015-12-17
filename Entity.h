#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"

using namespace sf;

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy;
	float x, y;
	float speed;
	int w, h, health, armor, wGun, hGun;
	bool life, isMove, isSelect;
	int damage;
	float enemyShot, timeDeath;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name;
		speed = 0; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time, float coordX, float coordY) = 0;
};