#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Equip : public Entity {
public:
	Equip(Image &image, Vector2f coords, FloatRect sizeHeros, String Name) :Entity(image, coords, sizeHeros, Name) {
		coord = coords;
		sizeHero = sizeHeros;
		if (Name == "machineGun") {
			sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
		}
		else if (Name == "shotgun") {
			sprite.setTextureRect(IntRect(0, 40, sizeHero.width, sizeHero.height));
		}
		else if (Name == "automatical") {
			sprite.setTextureRect(IntRect(0, 20, sizeHero.width, sizeHero.height));
		}
		else if (Name == "rifle") {
			sprite.setTextureRect(IntRect(0, 55, sizeHero.width, sizeHero.height));
		}
	}
	void update(float time, Vector2f coords) override {
		sprite.setPosition(coord.x, coord.y);
	}
};

class Bonus : public Entity {
public:
	Bonus(Image &image, Vector2f coords, FloatRect sizeHeros, String Name) :Entity(image, coords, sizeHeros, Name) {
		coord = coords;
		sizeHero = sizeHeros;
		if (Name == "Health") {
			sprite.setTextureRect(IntRect(100, 0, sizeHero.width, sizeHero.height));
		}
		else if (Name == "Armor") {
			sprite.setTextureRect(IntRect(50, 0, sizeHero.width, sizeHero.height));
		}
		else if (Name == "Ammo") {
			sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
		}

	}
	void update(float time, Vector2f coords) override {
		sprite.setPosition(coord.x, coord.y);
	}
};

class Exit : public Entity {
public:
	Exit(Image &image, Vector2f coords, FloatRect sizeHeros, String Name) :Entity(image, coords, sizeHeros, Name) {
		coord = coords;
		sizeHero = sizeHeros;
		sprite.setTextureRect(IntRect(0, 0, sizeHero.width, sizeHero.height));
		sprite.setOrigin(0, 0);
	}
	void update(float time, Vector2f coords) override {
		sprite.setPosition(coord.x, coord.y);
	}
};