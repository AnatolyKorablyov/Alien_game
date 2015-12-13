#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Equip : public Entity {
public:
	Equip(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		x = X; y = Y;
		w = W; h = H;
		if (Name == "machineGun") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		else if (Name == "shotgun") {
			sprite.setTextureRect(IntRect(0, 40, w, h));
		}
		else if (Name == "automatical") {
			sprite.setTextureRect(IntRect(0, 20, w, h));
		}
		else if (Name == "rifle") {
			sprite.setTextureRect(IntRect(0, 55, w, h));
		}
	}
	void update(float time, float coordX, float coordY) {
		sprite.setPosition(x, y);
	}
};

class Bonus : public Entity {
public:
	Bonus(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		x = X; y = Y;
		w = W; h = H;
		if (Name == "Health") {
			sprite.setTextureRect(IntRect(100, 0, w, h));
		}
		else if (Name == "Armor") {
			sprite.setTextureRect(IntRect(50, 0, w, h));
		}
		else if (Name == "Ammo") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}

	}
	void update(float time, float coordX, float coordY) {
		sprite.setPosition(x, y);
	}
};

class Exit : public Entity {
public:
	Exit(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		x = X; y = Y;
		w = W; h = H;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setOrigin(0, 0);
	}
	void update(float time, float coordX, float coordY) {
		sprite.setPosition(x, y);
	}
};