#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "view.h"

using namespace sf;

class Player :public Entity {
public:
	float CurrentFrame;
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//��������� ��� ������������ - ��������� �������
	int weapon, ammo;
	float rotation;
	bool gameover = false;
	int playerScore;
	Player(Image &image, Image &gunImage, Level &lev, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		health = 100; armor = 100; weapon = 1;
		playerScore = 0; state = stay; isSelect = false; obj = lev.GetAllObjects();
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		select_weapon(weapon);
	}

	void control() {
		bool pressBut = false;
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = left; speed = 0.2;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = right; speed = 0.2;
			pressBut = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (pressBut) {
				if (state == right) {
					state = rightUp; speed = 0.2;
				}
				if (state == left) {
					state = leftUp; speed = 0.2;
				}
			}
			else {
				state = up; speed = 0.2;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			if (pressBut) {
				if (state == right) {
					state = rightDown; speed = 0.2;
				}
				if (state == left) {
					state = leftDown; speed = 0.2;
				}
			}
			else {
				state = down; speed = 0.2;
			}
		}
	}
	void rotation_GG(Vector2f pos) {
		float dX = pos.x - x;//������ , ����������� ������, ������� ���������� ������ � ������
		float dY = pos.y - y;//�� ��, ���������� y
		rotation = (atan2(dY, dX)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
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
	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = 0; i < obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}

	}

	void update(float time, float coordX, float coordY) {
		CurrentFrame += 0.005f*time;
		if (CurrentFrame > 4) CurrentFrame -= 4;
		sprite.setTextureRect(IntRect(0, h * int(CurrentFrame), w, h));
		sprite.setRotation(rotation);//������������ ������ �� ��� �������
		gunSprite.setRotation(rotation);
		control();//������� ���������� ����������
		switch (state){
		case right:dx = speed; dy = 0; break;
		case rightUp: dx = speed; dy = -speed; break;
		case rightDown: dx = speed; dy = speed; break;
		case left:dx = -speed; dy = 0; break;
		case leftUp: dx = -speed; dy = -speed; break;
		case leftDown: dx = -speed; dy = speed; break;
		case up: dx = 0; dy = -speed; break;
		case down: dx = 0; dy = speed; break;
		case stay: break;
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);//������������ ������������ �� �
		y += dy*time;
		checkCollisionWithMap(0, dy);//������������ ������������ �� Y
		sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
		gunSprite.setPosition(x + w / 2, y + h / 2);

		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		if (life) { getPlayerCoordinateForView(x, y); }
	}
};