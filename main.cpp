#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include "view.h"
#include "TinyXML\tinyxml.h"
#include "level.h"
#include <vector>
#include <list>

using namespace sf;
///////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
/*struct player {
	float x, y, w=91, h=54, wgun = 72, hgun = 20, dx = 0, dy = 0, speed = 0;
	int dir = -1, dir2 = -1, health = 100;
	bool life = true;
};*/
class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, health, armor, wGun, hGun;
	bool life, isMove, isSelect;
	int damage;
	float enemyShot;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time, float coordX, float coordY) = 0;
};

class Player :public Entity {
public:
	float CurrentFrame;
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//добавляем тип перечисления - состояние объекта
	int weapon, ammo;
	float rotation;
	bool gameover = false;
	int playerScore;//эта переменная может быть только у игрока
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
		if (Keyboard::isKeyPressed(Keyboard::Left)) {//а именно левая
			state = left; speed = 0.2;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = right; speed = 0.2;
			pressBut = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {//если нажата клавиша вверх и мы на земле, то можем прыгать
			if (pressBut) {
				if (state == right) {
					state = rightUp; speed = 0.2;
				}
				if (state == left) {
					state = leftUp; speed = 0.2;
				}
			}
			else {
				state = up; speed = 0.2;//увеличил высоту прыжка
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
				state = down; speed = 0.2;//увеличил высоту прыжка
			}
		}
	}
	void rotation_GG(Vector2f pos) {
		float dX = pos.x - x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор
		float dY = pos.y - y;//он же, координата y
		rotation = (atan2(dY, dX)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
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
	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
				if (obj[i].name == "exit"){//если встретили препятствие
					gameover = true;
				}
			}
		
	}

	void update(float time, float coordX, float coordY){
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 4) CurrentFrame -= 4;
		sprite.setTextureRect(IntRect(0, h * int(CurrentFrame), w, h));
		sprite.setRotation(rotation);//поворачиваем спрайт на эти градусы
		gunSprite.setRotation(rotation);
		control();//функция управления персонажем
		switch (state)//тут делаются различные действия в зависимости от состояния
		{
		case right:dx = speed; dy = 0;break;//состояние идти вправо
		case rightUp: dx = speed; dy = -speed; break;
		case rightDown: dx = speed; dy = speed; break;
		case left:dx = -speed; dy = 0; break;//состояние идти влево
		case leftUp: dx = -speed; dy = -speed; break;
		case leftDown: dx = -speed; dy = speed; break;
		case up: dx = 0; dy = -speed; break;//будет состояние поднятия наверх (например по лестнице)
		case down: dx = 0; dy = speed; break;//будет состояние во время спуска персонажа (например по лестнице)
		case stay: break;//и здесь тоже		
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
		y += dy*time;
		checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
		sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
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

	void checkCollisionWithMap(float Dx, float Dy){//ф ция проверки столкновений с картой
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { y -= 10; }
					if (Dy<0) { y += 10;}
					if (Dx > 0) { x -= 10;}
					if (Dx < 0) { x += 10;}
				}
			}
	}

	void update(float time, float posPlayerX, float posPlayerY){
		if (name == "easyEnemy") {//для персонажа с таким именем логика будет такой
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
			rotation = (atan2(dx, dy)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
			//x += speed * (dx);
			//y += speed * (dy);
			sprite.setRotation(-rotation);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
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
			rotation = (atan2(dx, dy)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
			//x += speed * (posPlayerX - x);
			//y += speed * (posPlayerY - y);
			sprite.setRotation(-rotation);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
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
			rotation = (atan2(dx, dy)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
										  //x += speed * (posPlayerX - x);
										  //y += speed * (posPlayerY - y);
			sprite.setRotation(-rotation + 90);
			gunSprite.setRotation(-rotation + 90);
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
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
class Bullet :public Entity {//класс пули
public:
	int direction, numBull;//направление пули
	float tempy, tempx, rotation, Dx, Dy;
	Bullet(Image &image, Level &lvl, float X, float Y, int W, int H, float tempX, float tempY, String Name, int damag, int numBullet) :Entity(image, X, Y, W, H, Name) {//всё так же, только взяли в конце состояние игрока (int dir)
		numBull = numBullet;
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
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
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
	}
	void update(float time, float coordX, float coordY){
		x += speed * (tempx - dx);
		y += speed * (tempy - dy);

		if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2 + numBull * 10, y + h / 2 + numBull * 10);//задается позицию пуле
	}
};


int main() {
	float queueShot = 0, enemyShot = 0;
	Music music;
	//music.openFromFile("sounds/mus01.ogg");// тут загружаем в буфер что то
	music.play();

	sf::Vector2f posView;

	SoundBuffer machineBuffer;//создаём буфер для звука
	machineBuffer.loadFromFile("sounds/MachineGun.wav");//загружаем в него звук
	Sound shootMachine(machineBuffer);//создаем звук и загружаем в него звук из буфера
	
	SoundBuffer damageBuffer;//создаём буфер для звука
	damageBuffer.loadFromFile("sounds/ZombieAttack1.wav");//загружаем в него звук
	Sound damage(damageBuffer);//создаем звук и загружаем в него звук из буфера

	SoundBuffer shotgunBuffer;//создаём буфер для звука
	shotgunBuffer.loadFromFile("sounds/PumpShotgun.wav");//загружаем в него звук
	Sound shootShootGun(shotgunBuffer);//создаем звук и загружаем в него звук из буфера

	SoundBuffer rifleBuffer;//создаём буфер для звука
	rifleBuffer.loadFromFile("sounds/Rifle.wav");//загружаем в него звук
	Sound shootRifle(rifleBuffer);//создаем звук и загружаем в него звук из буфера

	SoundBuffer reloadBuffer;//создаём буфер для звука
	reloadBuffer.loadFromFile("sounds/Reload.wav");//загружаем в него звук
	Sound shootReload(reloadBuffer);//создаем звук и загружаем в него звук из буфера

	int HP;
	String strHP;
	Font font;//шрифт 
	font.loadFromFile("images/Albionic.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый

	int tempX = 0;//временная коорд Х.Снимаем ее после нажатия прав клав мыши
	int tempY = 0;//коорд Y
	float distance = 0;//это расстояние от объекта до тыка курсора
	sf::RenderWindow window(sf::VideoMode(1370, 768), "Alien Overkill");
	view.reset(sf::FloatRect(0, 0, 1370, 768));//размер "вида" камеры при создании объекта вида камеры. (потом можем менять как хотим) Что то типа инициализации.

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него

	std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка

	std::vector<Object> easyE = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хранятся в этом векторе
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

	Player p(heroImage, gunImage, lvl, player.rect.left, player.rect.top, 91, 54, 72, 15, "Player");//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	Clock clock;
	Clock gameTime;
	for (int i = 0; i < easyE.size(); i++) //проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new Enemy(easyEnemyImage, gunImage, lvl, easyE[i].rect.left, easyE[i].rect.top, 43, 75, 0, 0, "easyEnemy"));//и закидываем в список всех наших врагов с карты
	for (int i = 0; i < medE.size(); i++)
		entities.push_back(new Enemy(mediumEnemyImage, gunImage, lvl, medE[i].rect.left, medE[i].rect.top, 76, 44, 0, 0, "mediumEnemy"));
	for (int i = 0; i < bandE.size(); i++) //проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new Enemy(banditImage, gunImage, lvl, easyE[i].rect.left, easyE[i].rect.top, 104, 56, 72, 15, "bandit"));//и закидываем в список всех наших врагов с карты
	for (int i = 0; i < machineGun.size(); i++)
		entities.push_back(new Equip(equipImage, machineGun[i].rect.left, machineGun[i].rect.top, 80, 20, "machineGun"));
	for (int i = 0; i < shotgun.size(); i++)
		entities.push_back(new Equip(equipImage, shotgun[i].rect.left, shotgun[i].rect.top, 80, 15, "shotgun"));
	for (int i = 0; i < automatical.size(); i++)
		entities.push_back(new Equip(equipImage, automatical[i].rect.left, automatical[i].rect.top, 80, 20, "automatical"));
	for (int i = 0; i < rifle.size(); i++)
		entities.push_back(new Equip(equipImage, rifle[i].rect.left, rifle[i].rect.top, 80, 13, "rifle"));
	bool ret = false;

	float CurrentFrame = 0;//хранит текущий кадр
	while (window.isOpen()){
		std::list<Entity*>::iterator at;//второй итератор.для взаимодействия между объектами списка
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
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
		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity *b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time, p.x, p.y);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false) { it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
		for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
		{
			if ((*it)->name == "bandit" && ((*it)->dx <= 500 || (*it)->dx <= -500) && ((*it)->dy < 300 || (*it)->dy < 300) && (*it)->enemyShot < timeGame) { //проверка находится ли бандит на расстояннии выстрела и может ли он сделать выстрел
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
			if ((*it)->getRect().intersects(p.getRect())){//если прямоугольник спрайта объекта пересекается с игроком
				if ((*it)->name == "easyEnemy") {//и при этом имя объекта EasyEnemy,то..
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
				if ((*it)->name == "mediumEnemy") {//и при этом имя объекта EasyEnemy,то..
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
		lvl.Draw(window);//рисуем новую карту
		for (it = entities.begin(); it != entities.end(); it++) {
			if ((*it)->x > posView.x - 632 && (*it)->x < posView.x + 632 && (*it)->y > posView.y - 384 && (*it)->y < posView.y + 384) // если объект выходит за экран, не рисуем
				window.draw((*it)->gunSprite);
				window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		window.draw(p.gunSprite);
		window.draw(p.sprite);
		if (!p.life) {
			text.setPosition(posView.x - 100, posView.y - 50);//задаем позицию текста, центр камеры
			text.setColor(Color::Red);
			text.setString("GAME OVER");//задает строку тексту
			window.draw(text);//рисую этот текст
		}
		else {
			text.setPosition(posView.x - 610, posView.y + 290);//задаем позицию текста, слева внизу
			text.setColor(Color::Red);
			HP = p.health;
			strHP = std::to_string(HP);
			text.setString(strHP);//задает строку тексту
			window.draw(text);//рисую этот текст
			
			if (p.armor > 0) {
				text.setPosition(posView.x - 530, posView.y + 290);//задаем позицию текста, слева внизу
				text.setColor(Color::Blue); // броня
				HP = p.armor;
				strHP = std::to_string(HP);
				text.setString(strHP);//задает строку тексту
				window.draw(text);//рисую этот текст]
			}

			text.setPosition(posView.x + 500, posView.y + 290);//задаем позицию текста, слева внизу
			text.setColor(Color::Cyan); // броня
			HP = p.ammo;
			strHP = std::to_string(HP);
			text.setString(strHP);//задает строку тексту
			window.draw(text);//рисую этот текст
		}
		if (p.gameover) {
			window.close();
		}
		window.display(); 
	}
	return 0;
}