#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"

using namespace sf;

class Entity {
public:
	std::vector<Object> obj;//������ �������� �����
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
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
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
	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(coord.x, coord.y, sizeHero.height, sizeHero.width);//��� �-��� ����� ��� �������� ������������ 
	}
	virtual void update(float time, Vector2f coords) = 0;
};