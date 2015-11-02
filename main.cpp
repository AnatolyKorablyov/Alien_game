#include <SFML/Graphics.hpp>
#include <iostream> 
#include "map.h"
#include "view.h"//���������� ��� � ����� ������

using namespace sf;

///////////////////////////////////////////////////����� ������////////////////////////
class Player { // ����� ������
public:
	float x, y, w, h, dx, dy, speed = 0;//������ ������, ��������� (�� � � �� �), ���� ��������
	int dir = 0, dir2 = 0; //����������� (direction) �������� ������
	String File; //���� � �����������
	Image image;//���� �����������
	Texture texture;//���� ��������
	Sprite sprite;//���� ������
	Player(String F, float X, float Y, float W, float H) {  //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
		File = F;//��� �����+����������
		w = W; h = H;//������ � ������
		image.loadFromFile("images/" + File);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������. � ����� ������ "hero.png" � ��������� ������ ���������� 	image.loadFromFile("images/hero/png");
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(0, 0, w, h));  //������ ������� ���� ������������� ��� ������ ������ ����, � �� ���� ����� �����. IntRect - ���������� �����
	}
	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
		switch (dir)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed; dy = 0;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dx = 0; dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dx = 0; dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}
		switch (dir2)//��������� ��������� � ����������� �� �����������. (������ ����� ������������� �����������)
		{
		case 0: dx = speed;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ��������, ��� �������� ���� ������ ������
		case 1: dx = -speed;   break;//�� ���� ������ ������������� ��������, �� ������ ��������. ����������, ��� �������� ���� ������ �����
		case 2: dy = speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ ����
		case 3: dy = -speed;   break;//�� ���� ������ ������� ��������, �� ������ �������������. ����������, ��� �������� ���� ������ �����
		}
		x += dx*time;//�� �������� �� �������� �����. ���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		y += dy*time;//���������� �� ������
		speed = 0;//�������� ��������, ����� �������� �����������.
		sprite.setPosition(x, y); //������� ������ � ������� x y , ����������. ���������� ������� � ���� �������, ����� �� ��� ������ ����� �� �����.
		interactionWithMap();//�������� �������, ���������� �� �������������� � ������

	}
	float getplayercoordinateX() {	//���� ������� ����� �������� ���������� �	
		return x;
	}
	float getplayercoordinateY() {	//���� ������� ����� �������� ���������� Y 	
		return y;
	}
	void interactionWithMap()//�-��� �������������� � ������
	{

		for (int i = y / 54; i < (y + h) / 54; i++)//���������� �� ���� �����, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 54; j < (x + w) / 54; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '1')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					std::cout << i << " " << j << "\n";
					if (dy > 0)//���� �� ��� ����,
					{
						y = i * 50 - 1;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
					}
					if (dy < 0)
					{
						y = i * 50 + 100;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
					}
					if (dx > 0)
					{
						x = j * 50 - 1;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
					}
					if (dx < 0)
					{
						x = j * 50 + 100;//���������� ���� �����
					}
				}

				if (TileMap[i][j] == 's') { //���� ������ ����� 's' (������)
					x = 300; y = 300;//����� �� ��������... �������� ������������ �����
					TileMap[i][j] = ' ';//������� ������, ���� ����� �����. ����� � �� �������, ������.
				}
			}
	}
};

void move_player(float rotation, float time, Player &p, float &CurrentFrame) {
	bool press = false;
	p.sprite.setRotation(rotation);//������������ ������ �� ��� �������
	if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
		press = true;
		p.dir = 1; p.speed = 0.2; p.dir2 = -1;//dir =1 - ����������� �����, speed =0.1 - �������� ��������. �������� - ����� �� ��� ����� �� �� ��� �� �������� � ����� �� ���������� ������ ���
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 4) CurrentFrame -= 4;
		p.sprite.setTextureRect(IntRect(0, 54 * int(CurrentFrame), 91, 54)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
		getplayercoordinateforview(p.x, p.y);//�������� ���������� ������ � ������� ���������� �������
	}

	if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
		if (press == true)
			p.dir2 = 0;
		else {
			press = true;
			p.dir = 0; p.speed = 0.2;p.dir2 = -1;//����������� ������, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			p.sprite.setTextureRect(IntRect(0, 54 * int(CurrentFrame), 91, 54)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			getplayercoordinateforview(p.x, p.y);//�������� ���������� ������ � ������� ���������� ������
		}
	}

	if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
		if (press == true)
			p.dir2 = 3;
		else {
			press = true;
			p.dir = 3; p.speed = 0.2; p.dir2 = -1;//����������� ������, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			p.sprite.setTextureRect(IntRect(0, 54 * int(CurrentFrame), 91, 54)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			getplayercoordinateforview(p.x, p.y);//�������� ���������� ������ � ������� ���������� ������
		}
	}

	if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //���� ������ ������� ������� ����� ��� ���� ����� �
		if (press == true)
			p.dir2 = 2;
		else {
			press = true;
			p.dir = 2; p.speed = 0.2; p.dir2 = -1;//����������� ������, �� ����
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 4) CurrentFrame -= 4;
			p.sprite.setTextureRect(IntRect(0, 54 * int(CurrentFrame), 91, 54)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			getplayercoordinateforview(p.x, p.y);//�������� ���������� ������ � ������� ���������� ������
		}
	}
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(1370, 768), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, 1370, 768));//������ "����" ������ ��� �������� ������� ���� ������. (����� ����� ������ ��� �����) ��� �� ���� �������������.
	Player p("players_anim.png", 800, 800, 91, 54);//������� ������ p ������ player,������ "hero.png" ��� ��� �����+����������, ����� ���������� �,�, ������, ������.
	Clock clock;
	float dX = 0;
	float dY = 0;
	bool ret = false;
	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/map.png");//��������� ���� ��� �����
	Texture map;//�������� �����
	map.loadFromImage(map_image);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������
	float CurrentFrame = 0;//������ ������� ����
	while (window.isOpen())
	{
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
		}///////////////////////////////////////////���������� ���������� � ���������////////////////////////////////////////////////////////////////////////
		float dX = pos.x - p.x;//������ , ����������� ������, ������� ���������� ������ � ������
		float dY = pos.y - p.y;//�� ��, ���������� y
		float rotation = (atan2(dY, dX)) * 180 / 3.14159265;//�������� ���� � �������� � ��������� ��� � �������
		//std::cout << rotation << "\n";//������� �� ������� � ���������
		move_player(rotation, time, p, CurrentFrame);
		p.update(time);//�������� ������ p ������ Player � ������� ������� sfml, ��������� ����� � �������� ��������� ������� update. ��������� ����� �������� ����� ���������
		window.setView(view);//"��������" ������ � ���� sfml
		window.clear();
		/////////////////////////////������ �����/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == '0')  s_map.setTextureRect(IntRect(0, 0, 50, 50)); //���� ��������� ������ ������, �� ������ 1� ���������
				if (TileMap[i][j] == '1') s_map.setTextureRect(IntRect(100, 0, 50, 50));//���� ��������� ������ 0, �� ������ 3� ���������


				s_map.setPosition(j * 50,  i * 50);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

				window.draw(s_map);//������ ���������� �� �����
			}
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}