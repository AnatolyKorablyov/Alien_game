#include <SFML/Graphics.hpp>
#include <iostream> 
//#include "map.h"
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
	int w, h, health;
	bool life, isMove, isSelect;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}
	virtual void update(float time) = 0;
};

class Player :public Entity {
public:
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//��������� ��� ������������ - ��������� �������
	float rotation;
	int playerScore;//��� ���������� ����� ���� ������ � ������
	Player(Image &image, Image &gunImage, Level &lev, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay; isSelect = false; obj = lev.GetAllObjects();
		gunTexture.loadFromImage(gunImage);
		gunSprite.setTexture(gunTexture);
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			gunSprite.setTextureRect(IntRect(0, 0, Wgun, Hgun));
		}
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
			}
	}

	void update(float time)
	{
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
	Enemy(Image &image, Image &gunImage, Level &lvl, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		obj = lvl.GetObjects("solid");
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = -0.1;//���� ��������.���� ������ ������ ���������
			//dy = -0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy){//� ��� �������� ������������ � ������
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid")//���� ��������� �����������
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = -0.1; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0.1; }
					if (Dx > 0) { x = obj[i].rect.left - w; dx = -0.1; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; }
				}
			}
	}

	void update(float time){
		if (name == "easyEnemy") {//��� ��������� � ����� ������ ������ ����� �����
			checkCollisionWithMap(dx, dy);//������������ ������������ �� �
			x += dx*time;
			y += dy*time;
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
	}
};

class Bullet :public Entity {//����� ����
public:
	int direction;//����������� ����
	float tempy, tempx, rotation, Dx, Dy;
	Bullet(Image &image, Level &lvl, float X, float Y, int W, int H, float tempX, float tempY, String Name) :Entity(image, X, Y, W, H, Name) {//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
		obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
		x = X;
		y = Y;
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
		//���� ������������� � ������������
	}


	void update(float time)
	{
		x += speed * (tempx - dx);//???? ?? ???? ? ??????? ??????? ???????
		y += speed * (tempy - dy);//???? ?? ?????? ??? ??

		if (x <= 0) x = 1;// ???????? ???? ? ????? ?????, ????? ??? ?????????? ?????? ??? ???????? ?? ???????? ?? ?????? ????? ? ?? ???? ??????
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//?????? ?? ???????? solid
			if (getRect().intersects(obj[i].rect)) //???? ???? ?????? ?????????? ? ?????,
			{
				life = false;// ?? ???? ???????
			}
		}
		/*switch (direction)
		{
		case 0: dx = -speed; dy = 0;   break;//������� �������� state = left
		case 1: dx = speed; dy = 0;   break;//������� �������� state = right
		case 2: dx = 0; dy = -speed;   break;//������� �������� state = up
		case 3: dx = 0; dy = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 4: dx = 0; dy = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		case 5: dx = 0; dy = -speed;   break;//������� �������� �� ������� ��������� � �����������, ���� ������ ��������� �����, ��� ������ ��� �� �����
		}

		x += dx*time;//���� �������� ���� �� �
		y += dy*time;//�� �

		if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//������ �� �������� solid
			if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
			{
				life = false;// �� ���� �������
			}
		}*/
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
	}
};

/*void interactionWithMap(player &p)//�-��� �������������� � ������
{
	for (int i = p.y / 53; i < (p.y + p.h) / 53; i++)//���������� �� ���� �����, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
		for (int j = p.x / 54; j < (p.x + p.w) / 54; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
		{
			if (TileMap[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
			{
				if (p.dy > 0)//���� �� ��� ����,
				{
					p.y = i * 50 - 1;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
				}
				if (p.dy < 0)
				{
					p.y = i * 50 + 100;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
				}
				if (p.dx > 0)
				{
					p.x = j * 50 - 1;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
				}
				if (p.dx < 0)
				{
					p.x = j * 50 + 100;//���������� ���� �����
				}
			}
			if (TileMap[i][j] == 'e') {
				p.health -= 40;//���� ����� ����������� � ���� ������,�� ���������� health=health-40;
				TileMap[i][j] = '0';//������ ������
			}
		}
}*/

/*void player_control(player &p, float time, Sprite &hero, Sprite &gun) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
{
	if (p.dir == 0)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
	{
		p.dx = p.speed; p.dy = 0;
	}//������
	if (p.dir == 1)
	{
		p.dx = -p.speed; p.dy = 0;
	}//�����
	if (p.dir == 2)
	{
		p.dx = 0; p.dy = p.speed;
	}//����
	if (p.dir == 3)
	{
		p.dx = 0; p.dy = -p.speed;
	}//�����
	p.x += p.dx*time;//�� �������� �� �������� �����. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
	p.y += p.dy*time;//���������� �� ������
	p.speed = 0;//�������� ��������, ����� �������� �����������.
	interactionWithMap(p);//�������� �������, ���������� �� �������������� � ������
	hero.setPosition(p.x, p.y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
	gun.setPosition(p.x, p.y);
	if (p.health <= 0) { p.life = false; p.speed = 0; }//���� ������ ������ ���� ����� 0, �� ������� � ��������� �������� ������ ����� ������
}*/

/*void move_player(float rotation, float time, player &p, float &CurrentFrame, Sprite &hero, Sprite &gun) {
	bool press = false;
	hero.setRotation(rotation);//������������ ������ �� ��� �������
	gun.setRotation(rotation);
	if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
		press = true;
		p.dir = 1; p.speed = 0.2; p.dir2 = -1;//dir =1 - ����������� �����, speed =0.1 - �������� ��������. �������� - ����� �� ��� ����� �� �� ��� �� �������� � ����� �� ���������� ������ ���
	}

	if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
		if (press == true)
			p.dir2 = 0;
		else {
			press = true;
			p.dir = 0; p.speed = 0.2;p.dir2 = -1;//����������� ������, �� ����
		}
	}

	if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
		if (press == true)
			p.dir2 = 3;
		else {
			press = true;
			p.dir = 3; p.speed = 0.2; p.dir2 = -1;//����������� ������, �� ����
		}
	}

	if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //���� ������ ������� ������� ����� ��� ���� ����� �
		if (press == true)
			p.dir2 = 2;
		else {
			press = true;
			p.dir = 2; p.speed = 0.2; p.dir2 = -1;//����������� ������, �� ����
		}
	}
	CurrentFrame += 0.005*time;
	if (CurrentFrame > 4) CurrentFrame -= 4;
	hero.setTextureRect(IntRect(0, p.h * int(CurrentFrame), p.w, p.h)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
	getplayercoordinateforview(p.x, p.y);//�������� ���������� ������ � ������� ���������� ������
}*/

int main(){
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

	std::vector<Object> e = lvl.GetObjects("easyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������


	Image heroImage, gunImage, easyEnemyImage, bulletImage;
	bulletImage.loadFromFile("images/shot.png");
	heroImage.loadFromFile("images/players_anim.png");
	gunImage.loadFromFile("images/weapon.png");
	easyEnemyImage.loadFromFile("images/easy_enemy.png");
	std::cout << player.rect.left << player.rect.top;
	Player p(heroImage, gunImage, lvl, player.rect.left, player.rect.top, 91, 54, 72, 20, "Player");//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
	Clock clock;

	for (int i = 0; i < e.size(); i++)//���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(easyEnemyImage, gunImage, lvl, e[i].rect.left, e[i].rect.top, 43, 75, 0, 0, "easyEnemy"));//� ���������� � ������ ���� ����� ������ � �����

	//Enemy easyEnemy(easyEnemyImage, gunImage, lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 43, 75, 0, 0, "EasyEnemy");
	bool ret = false;

	float CurrentFrame = 0;//������ ������� ����
	while (window.isOpen())
	{
		std::list<Entity*>::iterator at;//������ ��������.��� �������������� ����� ��������� ������
		Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == Mouse::Left) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x, p.y, 23, 7, pos.x, pos.y, "Bullet"));
			}
		}
		p.rotation_GG(pos);
		p.update(time);// Player update function
		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			Entity *b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false) { it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}
		for (it = entities.begin(); it != entities.end(); it++)//���������� �� ��-��� ������
		{
			for (at = entities.begin(); at != entities.end(); at++) {
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "easyEnemy"))) {
					(*it)->health -= 13;
					(*at)->life = false;
				}
			}
			if ((*it)->getRect().intersects(p.getRect()))//���� ������������� ������� ������� ������������ � �������
			{
				if ((*it)->name == "easyEnemy") {//� ��� ���� ��� ������� EasyEnemy,��..
					(*it)->dx = 0;
					p.health -= 1;
				}
			}
		}
		if (!p.life)
			window.close();
		//for (it = entities.begin(); it != entities.end(); it++) { (*it)->update(time); }//��� ���� ��������� ������(���� ��� ������ �����,�� ����� ���� � ���� � �������) ���������� �-��� update
		window.setView(view);
		window.clear();
		lvl.Draw(window);//������ ����� �����
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite); //������ entities ������� (������ ��� ������ �����)
		}
		//window.draw(easyEnemy.sprite);
		window.draw(p.gunSprite);
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}