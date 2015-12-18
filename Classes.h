#pragma once

#include <SFML/Graphics.hpp>
#include "level.h"
#include "view.h"


using namespace sf;


struct characteristicts {
	Vector2f dist;// скорость по координатам
	Vector2f pos;
	float speed;
	Vector2f sizeHero;
	Vector2f sizeGun;
	bool life, isMove = false;
};

struct life_obj {
	int health;
	int armor;
	int damage;
	int num_weapon;
	int main_weapon;
	int ammo;
	int sec_weapon;
	int third_weapon;
	int main_damage;
	int sec_damage;
	float rotation;
	bool change_weapon;
	bool gameover = false;
	float CurrentFrame;
};

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	characteristicts properties;
	life_obj liv_pr;
	float enemyShot, timeDeath;
	float timeBurn;
	int numPortal;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;
	Entity();
	Entity(Image &image, Vector2f coord, Vector2f size, String Name) {
		properties.pos = coord;
		properties.sizeHero = size;
		name = Name;
		properties.dist.x = 0; properties.dist.y = 0;
		properties.speed = 0;
		properties.life = true; properties.isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(properties.sizeHero.y / 2, properties.sizeHero.x / 2);
	}
	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(properties.pos.x, properties.pos.y, properties.sizeHero.y, properties.sizeHero.x);//эта ф-ция нужна для проверки столкновений 
	}
	bool Intersects(Entity* two) {
		if (getRect().intersects(two->getRect()))
			return true;
	}
	virtual void update(float time, Vector2f posGG) = 0;
};

class Player :public Entity {
public:
	bool m_isShoot;
	float m_queueShot = 0;
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//добавляем тип перечисления - состояние объекта
	Player();
	Player(Image &image, Image &gunImage, Level &lvl, Vector2f coord, Vector2f sizeH, Vector2f sizeG, String Name) :Entity(image, coord, sizeH, Name) {
		properties.sizeHero = sizeH;
		properties.pos = coord;
		properties.sizeGun = sizeG;
		name = Name;
		obj = lvl.GetAllObjects();
		liv_pr.num_weapon = 1;
		liv_pr.main_damage = 30;
		liv_pr.ammo = 6;
		liv_pr.health = 100; liv_pr.armor = 100; liv_pr.main_weapon = 1;
		state = stay;
		liv_pr.sec_damage = 20;
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		select_weapon(liv_pr);
	}

	void control() {
		if (Keyboard::isKeyPressed(Keyboard::Num1)) {
			liv_pr.num_weapon = 1;
			liv_pr.change_weapon = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2)) {
			liv_pr.num_weapon = 2;
			liv_pr.change_weapon = true;
			m_queueShot = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num3)) {
			liv_pr.num_weapon = 3;
			liv_pr.change_weapon = true;
			m_queueShot = 0;
		}
		bool pressBut = false;
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			state = left; properties.speed = 0.2f;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			state = right; properties.speed = 0.2f;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			if (pressBut) {
				if (state == right) {
					state = rightUp; properties.speed = 0.2f;
				}
				if (state == left) {
					state = leftUp; properties.speed = 0.2f;
				}
			}
			else {
				state = up; properties.speed = 0.2f;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			if (pressBut) {
				if (state == right) {
					state = rightDown; properties.speed = 0.2f;
				}
				if (state == left) {
					state = leftDown; properties.speed = 0.2f;
				}
			}
			else {
				state = down; properties.speed = 0.2f;
			}
		}
	}
	void rotation_GG(Vector2f pos) {
		float dX = pos.x - properties.pos.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор
		float dY = pos.y - properties.pos.y;//он же, координата y
		liv_pr.rotation = (atan2(dY, dX)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
	}
	void select_weapon(life_obj &PR) {
		if (PR.num_weapon == 1) {
			PR.damage = PR.main_damage;
			if (PR.main_weapon == 0) {
				gunSprite.setTextureRect(IntRect(0, 0, 72, 15));
			}
			else if (PR.main_weapon == 1) {
				gunSprite.setTextureRect(IntRect(0, 15, 72, 15));
			}
			else if (PR.main_weapon == 2) {
				gunSprite.setTextureRect(IntRect(0, 30, 72, 15));
			}
			else if (PR.main_weapon == 3) {
				gunSprite.setTextureRect(IntRect(0, 45, 72, 15));
			}
		}
		else {
			PR.damage = PR.sec_damage;
			gunSprite.setTextureRect(IntRect(0, 60, 72, 15));
		}

	}
	void checkCollisionWithMap(Vector2f coordSpeed)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (coordSpeed.y>0) { properties.pos.y -= properties.sizeHero.y / 2; }
					else if (coordSpeed.y<0) { properties.pos.y += properties.sizeHero.y / 2; }
					if (coordSpeed.x>0) { properties.pos.x -= properties.sizeHero.y / 2; }
					else if (coordSpeed.x<0) { properties.pos.x += properties.sizeHero.y / 2; }
				}
			}

	}

	void update(float time, Vector2f posGG)  override {
		liv_pr.CurrentFrame += 0.005f*time;
		control();//функция управления персонажем
		if (liv_pr.change_weapon){
			select_weapon(liv_pr);
			liv_pr.change_weapon = false;
		}
		if ((liv_pr.CurrentFrame > 4 && liv_pr.num_weapon == 1) || (liv_pr.CurrentFrame > 8 && liv_pr.num_weapon == 2)) 
			liv_pr.CurrentFrame -= 4;
		sprite.setTextureRect(IntRect(0, int(properties.sizeHero.x) * int(liv_pr.CurrentFrame), int(properties.sizeHero.y), int(properties.sizeHero.x)));
		sprite.setRotation(liv_pr.rotation);//поворачиваем спрайт на эти градусы
		gunSprite.setRotation(liv_pr.rotation);
		switch (state) {
		case right: properties.dist.x = properties.speed; properties.dist.y = 0; break;
		case rightUp: properties.dist.x = properties.speed; properties.dist.y = -properties.speed; break;
		case rightDown: properties.dist.x = properties.speed; properties.dist.y = properties.speed; break;
		case left: properties.dist.x = -properties.speed; properties.dist.y = 0; break;
		case leftUp: properties.dist.x = -properties.speed; properties.dist.y = -properties.speed; break;
		case leftDown: properties.dist.x = -properties.speed; properties.dist.y = properties.speed; break;
		case up: properties.dist.x = 0; properties.dist.y = -properties.speed; break;
		case down: properties.dist.x = 0; properties.dist.y = properties.speed; break;
		case stay: break;
		}
		properties.pos.x += properties.dist.x*time;
		properties.pos.y += properties.dist.y*time;
		checkCollisionWithMap(properties.dist);//обрабатываем столкновение по Y
		sprite.setPosition(properties.pos.x + properties.sizeHero.x / 2, properties.pos.y + properties.sizeHero.y / 2); //задаем позицию спрайта в место его центра
		gunSprite.setPosition(properties.pos.x + properties.sizeHero.x / 2, properties.pos.y + properties.sizeHero.y / 2);

		if (liv_pr.health <= 0) { properties.life = false; }
		properties.speed = 0;
		if (properties.life) { getPlayerCoordinateForView(properties.pos.x, properties.pos.y); }
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, Image &gunImage, Level &lvl, Vector2f coord, Vector2f sizeH, Vector2f sizeG, String Name) :Entity(image, coord, sizeH, Name) {
		timeDeath = 0;
		properties.sizeHero = sizeH;
		properties.pos = coord;
		properties.sizeGun = sizeG;
		name = Name;
		obj = lvl.GetObjects("solid");
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		enemyShot = 0;
		if (name == "easyEnemy") {
			properties.speed = 0.03f; liv_pr.health = 50; liv_pr.armor = 0; liv_pr.damage = 1;
			sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		if (name == "mediumEnemy") {
			properties.speed = 0.06f; liv_pr.health = 100; liv_pr.armor = 0; liv_pr.damage = 2;
			sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		if (name == "bandit") {
			properties.speed = 0.1f; liv_pr.health = 100; liv_pr.armor = 100; liv_pr.damage = 15;
			sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
			gunSprite.setTextureRect(IntRect(0, 30, int(properties.sizeGun.y), int(properties.sizeGun.x)));
		}
	}

	void checkCollisionWithMap() {//ф ция проверки столкновений с картой
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (obj[i].rect.top + obj[i].rect.width > properties.pos.y) { properties.pos.y += properties.sizeHero.y  / 2; }
					else if (obj[i].rect.top < properties.pos.y) { properties.pos.y -= properties.sizeHero.y / 2; }
					if (obj[i].rect.left + obj[i].rect.height > properties.pos.x) { properties.pos.x += properties.sizeHero.y / 2; }
					else if (obj[i].rect.left < properties.pos.x) { properties.pos.x -= properties.sizeHero.y / 2; }
				}
			}
	}

	void same_action_enemys(float time, Vector2f posGG, int numFrame) {
		liv_pr.CurrentFrame += 0.001f*time;
		if (liv_pr.CurrentFrame > numFrame) liv_pr.CurrentFrame -= numFrame;
		sprite.setTextureRect(IntRect(0, int(properties.sizeHero.x) * int(liv_pr.CurrentFrame), int(properties.sizeHero.y), int(properties.sizeHero.x)));
		properties.dist.x = posGG.x - properties.pos.x;
		properties.dist.y = posGG.y - properties.pos.y;
	}

	void update(float time, Vector2f posGG) override {
		if (name == "easyEnemy"|| name == "mediumEnemy")  {//для персонажа с таким именем логика будет такой
			same_action_enemys(time, posGG, 2);
			if (properties.dist.x > 0) {
				properties.pos.x += properties.speed * time;
			}
			else if (properties.dist.x < 0)
				properties.pos.x -= properties.speed * time;
			if (properties.dist.y > 0) {
				properties.pos.y += properties.speed * time;
			}
			else if (properties.dist.y < 0)
				properties.pos.y -= properties.speed * time;
			liv_pr.rotation = (atan2(properties.dist.x, properties.dist.y)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap();//обрабатываем столкновение по Х
			sprite.setRotation(-liv_pr.rotation);
			sprite.setPosition(properties.pos.x + properties.sizeHero.y / 2, properties.pos.y + properties.sizeHero.x / 2); //задаем позицию спрайта в место его центра
			if (liv_pr.health <= 0) { properties.life = false; }
		}
		if (name == "bandit") {
			same_action_enemys(time, posGG, 4);
			if (properties.dist.x > 0 && properties.dist.x > 500) {
				properties.pos.x += properties.speed * time;
			}
			else if (properties.dist.x < 0 && properties.dist.x < -500)
				properties.pos.x -= properties.speed* time;
			if (properties.dist.y > 0 && properties.dist.y > 300) {
				properties.pos.y += properties.speed* time;
			}
			else if (properties.dist.y < 0 && properties.dist.y < -300)
				properties.pos.y -= properties.speed* time;
			liv_pr.rotation = (atan2(properties.dist.x, properties.dist.y)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap();//обрабатываем столкновение по Х
			sprite.setRotation(-liv_pr.rotation + 90);
			gunSprite.setRotation(-liv_pr.rotation + 90);
			sprite.setPosition(properties.pos.x + properties.sizeHero.y / 2, properties.pos.y + properties.sizeHero.x / 2); //задаем позицию спрайта в место его центра
			gunSprite.setPosition(properties.pos.x + properties.sizeHero.y / 2, properties.pos.y + properties.sizeHero.x / 2);
			if (liv_pr.health <= 0) { properties.life = false; }
		}
	}
};

class Equip : public Entity {
public:
	Equip(Image &image, Vector2f coord, Vector2f sizeH, String Name) :Entity(image, coord, sizeH, Name) {
		properties.sizeHero = sizeH;
		properties.pos = coord;
		name = Name;
		if (Name == "machineGun") {
			sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		else if (Name == "shotgun") {
			sprite.setTextureRect(IntRect(0, 40, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		else if (Name == "automatical") {
			sprite.setTextureRect(IntRect(0, 20, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		else if (Name == "rifle") {
			sprite.setTextureRect(IntRect(0, 55, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
	}
	void update(float time, Vector2f posGG)  override {
		sprite.setPosition(properties.pos.x, properties.pos.y);
	}
};

class Bonus : public Entity {
public:
	Bonus(Image &image, Vector2f coord, Vector2f sizeH, String Name) :Entity(image, coord, sizeH, Name) {
		properties.sizeHero = sizeH;
		properties.pos = coord;
		name = Name;
		if (name == "Health") {
			sprite.setTextureRect(IntRect(100, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		else if (name == "Armor") {
			sprite.setTextureRect(IntRect(50, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}
		else if (name == "Ammo") {
			sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		}

	}
	void update(float time, Vector2f posGG) override {
		sprite.setPosition(properties.pos.x, properties.pos.y);
	}
};

class Exit : public Entity {
public:
	Exit(Image &image, Vector2f coord, Vector2f sizeH, String Name) :Entity(image, coord, sizeH, Name) {
		properties.sizeHero = sizeH;
		properties.pos = coord;
		sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
		sprite.setOrigin(0, 0);
	}
	void update(float time, Vector2f posGG)  override {
		sprite.setPosition(properties.pos.x, properties.pos.y);
	}
};

class Portal : public Entity {
public:
	float m_rotation = 0;
	Portal(Image &image, Vector2f coord, Vector2f sizeH, String Name, int numPort) :Entity(image, coord, sizeH, Name) {
		properties.sizeHero = sizeH;
		properties.pos = coord;
		numPortal = numPort;
		liv_pr.health = 1000;
		sprite.setTextureRect(IntRect(0, 0, int(properties.sizeHero.y), int(properties.sizeHero.x)));
	}
	void update(float time, Vector2f posGG)  override {
		sprite.setRotation(m_rotation);
		sprite.setPosition(properties.pos.x, properties.pos.y);
		m_rotation += 0.5f;
		if (liv_pr.health <= 0) { properties.life = false; }
	}
};


class Bullet :public Entity {//класс пули
public:
	int direction, numBull;//направление пули
	Vector2f posTarget;
	Vector2f posStart;
	Bullet(Image &image, Level &lvl, Vector2f coord, Vector2f sizeH, Vector2f posT, String Name, int damag, int numBullet) :Entity(image, coord, sizeH, Name) {//всё так же, только взяли в конце состояние игрока (int dir)
		properties.pos = coord;
		properties.sizeHero = sizeH;
		posTarget = posT;
		numBull = numBullet;
		name = Name;
		if (name == "enemyBullet") {
			sprite.setColor(Color::Red);
		}
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
		liv_pr.damage = damag;
		properties.speed = 0.001f;
		properties.life = true;
		posStart = properties.pos;
		liv_pr.rotation = (atan2(posTarget.y - properties.pos.x, posTarget.y - properties.pos.x)) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
	}
	void update(float time, Vector2f posGG) override {
		properties.pos.x += properties.speed * (posTarget.x - posStart.x) * time;
		properties.pos.y += properties.speed * (posTarget.y - posStart.y) * time;

		if (properties.pos.x <= 0) properties.pos.x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (properties.pos.y <= 0) properties.pos.y = 1;

		for (int i = 0; i < int(obj.size()); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				properties.life = false;// то пуля умирает
			}
		}
		sprite.setRotation(liv_pr.rotation);
		sprite.setPosition(properties.pos.x + properties.sizeHero.y / 2 + numBull * 10, properties.pos.y + properties.sizeHero.x / 2 + numBull * 10);//задается позицию пуле
	}
};