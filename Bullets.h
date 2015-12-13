#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Bullet :public Entity {//����� ����
public:
	int direction, numBull;//����������� ����
	float rotation, Dx, Dy;
	Vector2f posPl;
	Vector2f coordSt;
	Bullet(Image &image, Level &lvl, Vector2f coords, FloatRect sizeHeros, Vector2f posPlayer, String Name, int damag, int numBullet) :Entity(image, coords, sizeHeros, Name) {//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		numBull = numBullet;
		obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
		damage = damag;
		speed = 0.001f;
		coord = coords;
		sizeHero = sizeHeros;
		posPl = posPlayer;
		life = true;
		coordSt = coord;
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265f;//�������� ���� � �������� � ��������� ��� � �������
	}
	void update(float time, Vector2f coords) override {
		coord.x += speed * (posPl.x - coordSt.x) * time;
		coord.y += speed * (posPl.y - coordSt.y) * time;

		if (coord.x <= 0) coord.x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (coord.y <= 0) coord.y = 1;

		for (int i = 0; i < int(obj.size()); i++) {//������ �� �������� solid
			if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(coord.x + sizeHero.width / 2 + numBull * 10, coord.y + sizeHero.height / 2 + numBull * 10);//�������� ������� ����
	}
};