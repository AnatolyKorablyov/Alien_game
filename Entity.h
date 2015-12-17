#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"

using namespace sf;

class Entity {
public:
	std::vector<Object> obj;//������ �������� �����
	float dx, dy;
	float x, y;
	float speed;
	int w, h, health, armor, wGun, hGun;
	bool life, isMove, isSelect;
	int damage;
	float enemyShot, timeDeath;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name;
		speed = 0; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}
	virtual void update(float time, float coordX, float coordY) = 0;
};