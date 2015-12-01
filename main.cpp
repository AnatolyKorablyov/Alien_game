#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include "view.h"
#include "TinyXML\tinyxml.h"
#include "level.h"
#include <vector>
#include <list>

using namespace sf;
///////////////////////////////////////////////////����� ������////////////////////////
/*struct player {
	float x, y, w=91, h=54, wgun = 72, hgun = 20, dx = 0, dy = 0, speed = 0;
	int dir = -1, dir2 = -1, health = 100;
	bool life = true;
};*/
class Entity {
public:
	std::vector<Object> obj;//������ �������� �����
	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h, health, armor, wGun, hGun;
	bool life, isMove, isSelect;
	int damage;
	float enemyShot;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
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

class Player :public Entity {
public:
	float CurrentFrame;
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//��������� ��� ������������ - ��������� �������
	int weapon, ammo;
	float rotation;
	bool gameover = false;
	int playerScore;//��� ���������� ����� ���� ������ � ������
	Player(Image &image, Image &gunImage, Level &lev, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		health = 100; armor = 150; weapon = 1; weapon = 1;
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
		if (Keyboard::isKeyPressed(Keyboard::Left)) {//� ������ �����
			state = left; speed = 0.2;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = right; speed = 0.2;
			pressBut = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
			if (pressBut) {
				if (state == right) {
					state = rightUp; speed = 0.2;
				}
				if (state == left) {
					state = leftUp; speed = 0.2;
				}
			}
			else {
				state = up; speed = 0.2;//�������� ������ ������
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
				state = down; speed = 0.2;//�������� ������ ������
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
			damage = 5;
		}
		else if (wp == 1) {
			ammo = 4;
			damage = 30;
			gunSprite.setTextureRect(IntRect(0, 15, 72, 15));
		}
		else if (wp == 2) {
			ammo = 30;
			damage = 15;
			gunSprite.setTextureRect(IntRect(0, 30, 72, 15));
		}
		else if (wp == 3) {
			ammo = 10;
			damage = 50;
			gunSprite.setTextureRect(IntRect(0, 45, 72, 15));
		}
	}
	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
				if (obj[i].name == "exit"){//���� ��������� �����������
					gameover = true;
				}
			}
		
	}

	void update(float time, float coordX, float coordY){
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 4) CurrentFrame -= 4;
		sprite.setTextureRect(IntRect(0, h * int(CurrentFrame), w, h));
		sprite.setRotation(rotation);//������������ ������ �� ��� �������
		gunSprite.setRotation(rotation);
		control();//������� ���������� ����������
		switch (state)//��� �������� ��������� �������� � ����������� �� ���������
		{
		case right:dx = speed; dy = 0;break;//��������� ���� ������
		case rightUp: dx = speed; dy = -speed; break;
		case rightDown: dx = speed; dy = speed; break;
		case left:dx = -speed; dy = 0; break;//��������� ���� �����
		case leftUp: dx = -speed; dy = -speed; break;
		case leftDown: dx = -speed; dy = speed; break;
		case up: dx = 0; dy = -speed; break;//����� ��������� �������� ������ (�������� �� ��������)
		case down: dx = 0; dy = speed; break;//����� ��������� �� ����� ������ ��������� (�������� �� ��������)
		case stay: break;//� ����� ����		
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

class Enemy :public Entity {
public:
	float CurrentFrame;
	float rotation;
	Enemy(Image &image, Image &gunImage, Level &lvl, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		obj = lvl.GetObjects("solid");
		w = W; h = H; wGun = Wgun; hGun = Hgun;
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		enemyShot = 0;
		if (name == "easyEnemy") {
			speed = 1; health = 50; armor = 0; damage = 1;
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		if (name == "mediumEnemy") {
			speed = 2; health = 100; armor = 0; damage = 2;
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		if (name == "bandit") {
			speed = 3; health = 100; armor = 100; damage = 15;
			sprite.setTextureRect(IntRect(0, 0, w, h));
			gunSprite.setTextureRect(IntRect(0, 30, wGun, hGun));
		}
	}

	void checkCollisionWithMap(float Dx, float Dy){//� ��� �������� ������������ � ������
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy>0) { y -= 10; }
					if (Dy<0) { y += 10;}
					if (Dx > 0) { x -= 10;}
					if (Dx < 0) { x += 10;}
				}
			}
	}

	void update(float time, float posPlayerX, float posPlayerY){
		if (name == "easyEnemy") {//��� ��������� � ����� ������ ������ ����� �����
			dx = posPlayerX - x;
			dy = posPlayerY - y;
			if (dx > 0) {
				x += speed;
			}
			else if (dx < 0)
				x -= speed;
			if (dy > 0) {
				y += speed;
			}
			else if (dy < 0)
				y -= speed;
			rotation = (atan2(dx, dy)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
			checkCollisionWithMap(dx, dy);//������������ ������������ �� �
			//x += speed * (dx);
			//y += speed * (dy);
			sprite.setRotation(-rotation);
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
		if (name == "mediumEnemy") {
			dx = posPlayerX - x;
			dy = posPlayerY - y;
			if (dx > 0) {
				x += speed;
			}
			else if (dx < 0)
				x -= speed;
			if (dy > 0) {
				y += speed;
			}
			else if (dy < 0)
				y -= speed;
			rotation = (atan2(dx, dy)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
			checkCollisionWithMap(dx, dy);//������������ ������������ �� �
			//x += speed * (posPlayerX - x);
			//y += speed * (posPlayerY - y);
			sprite.setRotation(-rotation);
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
		if (name == "bandit") {
			dx = posPlayerX - x;
			dy = posPlayerY - y;
			if (dx > 0 && dx > 500) {
				x += speed;
			}
			else if (dx < 0 && dx < -500)
				x -= speed;
			if (dy > 0 && dy > 300) {
				y += speed;
			}
			else if (dy < 0 && dy < -300)
				y -= speed;
			rotation = (atan2(dx, dy)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
			checkCollisionWithMap(dx, dy);//������������ ������������ �� �
										  //x += speed * (posPlayerX - x);
										  //y += speed * (posPlayerY - y);
			sprite.setRotation(-rotation + 90);
			gunSprite.setRotation(-rotation + 90);
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			gunSprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};
class Equip : public Entity {
public:
	Equip(Image &image, float X, float Y, int W, int H, String Name):Entity(image, X, Y, W, H, Name) {
		x = X; y = Y;
		w = W; h = H;
		if (Name == "machineGun") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
		else if (Name == "shotgun") {
			sprite.setTextureRect(IntRect(0, 40, w, h));
		}
		else if(Name == "automatical") {
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
		speed = 0.1;
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
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
	}
	void update(float time, float coordX, float coordY){
		x += speed * (tempx - dx);
		y += speed * (tempy - dy);

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//������ �� �������� solid
			if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2 + numBull * 10, y + h / 2 + numBull * 10);//�������� ������� ����
	}
};


int main() {
	float queueShot = 0, enemyShot = 0;
	Music music;
	//music.openFromFile("sounds/mus01.ogg");// ��� ��������� � ����� ��� ��
	music.play();

	sf::Vector2f posView;

	SoundBuffer machineBuffer;//������ ����� ��� �����
	machineBuffer.loadFromFile("sounds/MachineGun.wav");//��������� � ���� ����
	Sound shootMachine(machineBuffer);//������� ���� � ��������� � ���� ���� �� ������
	
	SoundBuffer damageBuffer;//������ ����� ��� �����
	damageBuffer.loadFromFile("sounds/ZombieAttack1.wav");//��������� � ���� ����
	Sound damage(damageBuffer);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer shotgunBuffer;//������ ����� ��� �����
	shotgunBuffer.loadFromFile("sounds/PumpShotgun.wav");//��������� � ���� ����
	Sound shootShootGun(shotgunBuffer);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer rifleBuffer;//������ ����� ��� �����
	rifleBuffer.loadFromFile("sounds/Rifle.wav");//��������� � ���� ����
	Sound shootRifle(rifleBuffer);//������� ���� � ��������� � ���� ���� �� ������

	SoundBuffer reloadBuffer;//������ ����� ��� �����
	reloadBuffer.loadFromFile("sounds/Reload.wav");//��������� � ���� ����
	Sound shootReload(reloadBuffer);//������� ���� � ��������� � ���� ���� �� ������

	int HP;
	String strHP;
	Font font;//����� 
	font.loadFromFile("images/Albionic.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 40);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setColor(Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����

	int tempX = 0;//��������� ����� �.������� �� ����� ������� ���� ���� ����
	int tempY = 0;//����� Y
	float distance = 0;//��� ���������� �� ������� �� ���� �������
	sf::RenderWindow window(sf::VideoMode(1370, 768), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, 1370, 768));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������.

	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("map.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.

	Object player = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����

	std::list<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
	std::list<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������

	std::vector<Object> easyE = lvl.GetObjects("easyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������
	std::vector<Object> medE = lvl.GetObjects("mediumEnemy");
	std::vector<Object> bandE = lvl.GetObjects("bandit");
	std::vector<Object> shotgun = lvl.GetObjects("shotgun");
	std::vector<Object> machineGun = lvl.GetObjects("machineGun");
	std::vector<Object> rifle = lvl.GetObjects("rifle");
	std::vector<Object> automatical = lvl.GetObjects("automatical");

	Image heroImage, gunImage, easyEnemyImage, bulletImage, mediumEnemyImage, equipImage, banditImage;
	bulletImage.loadFromFile("images/shot.png");
	heroImage.loadFromFile("images/players_anim.png");
	gunImage.loadFromFile("images/weapons.png");
	equipImage.loadFromFile("images/equipment2.png");
	easyEnemyImage.loadFromFile("images/easy_enemy.png");
	banditImage.loadFromFile("images/bandit.png");
	mediumEnemyImage.loadFromFile("images/MediumEnemy.png");

	Player p(heroImage, gunImage, lvl, player.rect.left, player.rect.top, 91, 54, 72, 15, "Player");//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
	Clock clock;
	Clock gameTime;
	for (int i = 0; i < easyE.size(); i++) //���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(easyEnemyImage, gunImage, lvl, easyE[i].rect.left, easyE[i].rect.top, 43, 75, 0, 0, "easyEnemy"));//� ���������� � ������ ���� ����� ������ � �����
	for (int i = 0; i < medE.size(); i++)
		entities.push_back(new Enemy(mediumEnemyImage, gunImage, lvl, medE[i].rect.left, medE[i].rect.top, 76, 44, 0, 0, "mediumEnemy"));
	for (int i = 0; i < bandE.size(); i++) //���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(banditImage, gunImage, lvl, easyE[i].rect.left, easyE[i].rect.top, 104, 56, 72, 15, "bandit"));//� ���������� � ������ ���� ����� ������ � �����
	for (int i = 0; i < machineGun.size(); i++)
		entities.push_back(new Equip(equipImage, machineGun[i].rect.left, machineGun[i].rect.top, 80, 20, "machineGun"));
	for (int i = 0; i < shotgun.size(); i++)
		entities.push_back(new Equip(equipImage, shotgun[i].rect.left, shotgun[i].rect.top, 80, 15, "shotgun"));
	for (int i = 0; i < automatical.size(); i++)
		entities.push_back(new Equip(equipImage, automatical[i].rect.left, automatical[i].rect.top, 80, 20, "automatical"));
	for (int i = 0; i < rifle.size(); i++)
		entities.push_back(new Equip(equipImage, rifle[i].rect.left, rifle[i].rect.top, 80, 13, "rifle"));
	bool ret = false;

	float CurrentFrame = 0;//������ ������� ����
	while (window.isOpen()){
		std::list<Entity*>::iterator at;//������ ��������.��� �������������� ����� ��������� ������
		Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)
		float time = clock.getElapsedTime().asMicroseconds();
		float timeGame = gameTime.getElapsedTime().asSeconds();
		clock.restart();
		time = time / 700;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == Mouse::Left && p.weapon == 0 && p.ammo > 0) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 0));
				p.ammo -= 1;
				shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.weapon == 1 && p.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 0));
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 1));
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 2));
				p.ammo -= 1;
				queueShot = timeGame + 2;
				shootShootGun.play();
			}
			else if (event.key.code == Mouse::Left && p.weapon == 2 && p.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 0));
				p.ammo -= 1;
				queueShot = timeGame + 0.25;
				shootMachine.play();
			}
			else if (event.key.code == Mouse::Left && p.weapon == 3 && p.ammo > 0 && timeGame > queueShot) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x + p.w / 2, p.y + p.h / 2, 23, 7, pos.x, pos.y, "Bullet", p.damage, 0));
				p.ammo -= 1;
				queueShot = timeGame + 5;
				shootRifle.play();
			}
			else if (event.key.code == Mouse::Left)
				shootReload.play();
		}
		p.rotation_GG(pos);
		if (p.life)
			p.update(time, p.x, p.y);// Player update function
		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			Entity *b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time, p.x, p.y);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false) { it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}
		for (it = entities.begin(); it != entities.end(); it++)//���������� �� ��-��� ������
		{
			if ((*it)->name == "bandit" && ((*it)->dx <= 500 || (*it)->dx <= -500) && ((*it)->dy < 300 || (*it)->dy < 300) && (*it)->enemyShot < timeGame) { //�������� ��������� �� ������ �� ����������� �������� � ����� �� �� ������� �������
				entities.push_back(new Bullet(bulletImage, lvl, (*it)->x + (*it)->w / 2, (*it)->y + (*it)->h / 2, 23, 7, p.x, p.y, "enemyBullet", (*it)->damage, 0));
				(*it)->enemyShot = timeGame + 1;
			}
			for (at = entities.begin(); at != entities.end(); at++) {
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "easyEnemy"))) {
					(*it)->health -= (*at)->damage;
					(*at)->life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "mediumEnemy"))) {
					(*it)->health -= (*at)->damage;
					(*at)->life = false;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "easyEnemy") && ((*it)->name == "easyEnemy"))) {
					(*it)->x -= 5;
					(*it)->y -= 5;
					(*at)->x += 5;
					(*at)->y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "easyEnemy") && ((*it)->name == "mediumEnemy"))) {
					(*it)->x -= 5;
					(*it)->y -= 5;
					(*at)->x += 5;
					(*at)->y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "bandit") && ((*it)->name == "bandit"))) {
					(*it)->x -= 5;
					(*it)->y -= 5;
					(*at)->x += 5;
					(*at)->y += 5;
				}
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "mediumEnemy") && ((*it)->name == "mediumEnemy"))) {
					if ((*it)->x > (*at)->x) {
						(*it)->x -= 5;
						(*at)->x += 5;
					}
					else {
						(*it)->x += 5;
						(*at)->x -= 5;
					}
					if ((*it)->y > (*at)->x) {
						(*it)->y += 5;
						(*at)->y -= 5;
					}
					else {
						(*it)->y -= 5;
						(*at)->y += 5;
					}

				}
			}
			if ((*it)->getRect().intersects(p.getRect())){//���� ������������� ������� ������� ������������ � �������
				if ((*it)->name == "easyEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					if ((*it)->x > p.x)
						(*it)->x -= 15;
					else
						(*it)->x += 15;
					if ((*it)->y > p.y)
						(*it)->y += 15;
					else
						(*it)->y -= 15;
					if (p.armor > 0)
						p.armor -= (*it)->damage;
					else
						p.health -= (*it)->damage;
					damage.play();
				}
				if ((*it)->name == "mediumEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					if ((*it)->x > p.x)
						(*it)->x += 15;
					else
						(*it)->x -= 15;
					if ((*it)->y > p.y)
						(*it)->y += 15;
					else
						(*it)->y -= 15;
					if (p.armor > 0)
						p.armor -= (*it)->damage;
					else
						p.health -= (*it)->damage;
					damage.play();
				}
				if ((*it)->name == "enemyBullet") {
					if (p.armor > 0)
						p.armor -= (*it)->damage;
					else
						p.health -= (*it)->damage;
					(*it)->life = false;
				}
				if ((*it)->name == "machineGun") {
					p.weapon = 0;
					p.select_weapon(p.weapon);
					(*it)->life = false;
				}
				if ((*it)->name == "shotgun") {
					p.weapon = 1;
					p.select_weapon(p.weapon);
					(*it)->life = false;
				}
				if ((*it)->name == "automatical") {
					p.weapon = 2;
					p.select_weapon(p.weapon);
					(*it)->life = false;
				}
				if ((*it)->name == "rifle") {
					p.weapon = 3;
					p.select_weapon(p.weapon);
					(*it)->life = false;
				}
			}
		}
		window.setView(view);
		window.clear();
		posView = view.getCenter();
		lvl.Draw(window);//������ ����� �����
		for (it = entities.begin(); it != entities.end(); it++) {
			if ((*it)->x > posView.x - 632 && (*it)->x < posView.x + 632 && (*it)->y > posView.y - 384 && (*it)->y < posView.y + 384) // ���� ������ ������� �� �����, �� ������
				window.draw((*it)->gunSprite);
				window.draw((*it)->sprite); //������ entities ������� (������ ��� ������ �����)
		}
		window.draw(p.gunSprite);
		window.draw(p.sprite);
		if (!p.life) {
			text.setPosition(posView.x - 100, posView.y - 50);//������ ������� ������, ����� ������
			text.setColor(Color::Red);
			text.setString("GAME OVER");//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		else {
			text.setPosition(posView.x - 610, posView.y + 290);//������ ������� ������, ����� �����
			text.setColor(Color::Red);
			HP = p.health;
			strHP = std::to_string(HP);
			text.setString(strHP);//������ ������ ������
			window.draw(text);//����� ���� �����
			
			if (p.armor > 0) {
				text.setPosition(posView.x - 530, posView.y + 290);//������ ������� ������, ����� �����
				text.setColor(Color::Blue); // �����
				HP = p.armor;
				strHP = std::to_string(HP);
				text.setString(strHP);//������ ������ ������
				window.draw(text);//����� ���� �����]
			}

			text.setPosition(posView.x + 500, posView.y + 290);//������ ������� ������, ����� �����
			text.setColor(Color::Cyan); // �����
			HP = p.ammo;
			strHP = std::to_string(HP);
			text.setString(strHP);//������ ������ ������
			window.draw(text);//����� ���� �����
		}
		if (p.gameover) {
			window.close();
		}
		window.display(); 
	}
	return 0;
}