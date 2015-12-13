#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Bullet :public Entity {//����� ����
public:
	int direction, numBull;//����������� ����
	float tempy, tempx, rotation, Dx, Dy;
	Bullet(Image &image, Level &lvl, float X, float Y, int W, int H, float tempX, float tempY, String Name, int damag, int numBullet) :Entity(image, X, Y, W, H, Name) {//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		numBull = numBullet;
		obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
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
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265f;//�������� ���� � �������� � ��������� ��� � �������
	}
	void update(float time, float coordX, float coordY) {
		x += speed * (tempx - dx) * time;
		y += speed * (tempy - dy) * time;

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		for (int i = 0; i < int(obj.size()); i++) {//������ �� �������� solid
			if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2 + numBull * 10, y + h / 2 + numBull * 10);//�������� ������� ����
	}
};