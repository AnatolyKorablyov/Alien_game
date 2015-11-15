#include <SFML/Graphics.hpp>
#include <iostream> 
//#include "map.h"
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
	int w, h, health;
	bool life, isMove, isSelect;
	Texture texture, gunTexture;
	Sprite sprite, gunSprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time) = 0;
};

class Player :public Entity {
public:
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;//добавляем тип перечисления - состояние объекта
	float rotation;
	int playerScore;//эта переменная может быть только у игрока
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
			}
	}

	void update(float time)
	{
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
	Enemy(Image &image, Image &gunImage, Level &lvl, float X, float Y, int W, int H, int Wgun, int Hgun, String Name) :Entity(image, X, Y, W, H, Name) {
		obj = lvl.GetObjects("solid");
		if (name == "easyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = -0.1;//даем скорость.этот объект всегда двигается
			//dy = -0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy){//ф ция проверки столкновений с картой
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = -0.1; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0.1; }
					if (Dx > 0) { x = obj[i].rect.left - w; dx = -0.1; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; }
				}
			}
	}

	void update(float time){
		if (name == "easyEnemy") {//для персонажа с таким именем логика будет такой
			checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
			x += dx*time;
			y += dy*time;
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
	}
};

class Bullet :public Entity {//класс пули
public:
	int direction;//направление пули
	float tempy, tempx, rotation, Dx, Dy;
	Bullet(Image &image, Level &lvl, float X, float Y, int W, int H, float tempX, float tempY, String Name) :Entity(image, X, Y, W, H, Name) {//всё так же, только взяли в конце состояние игрока (int dir)
		obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
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
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
		//выше инициализация в конструкторе
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
		case 0: dx = -speed; dy = 0;   break;//интовое значение state = left
		case 1: dx = speed; dy = 0;   break;//интовое значение state = right
		case 2: dx = 0; dy = -speed;   break;//интовое значение state = up
		case 3: dx = 0; dy = -speed;   break;//интовое значение не имеющее отношения к направлению, пока просто стрельнем вверх, нам сейчас это не важно
		case 4: dx = 0; dy = -speed;   break;//интовое значение не имеющее отношения к направлению, пока просто стрельнем вверх, нам сейчас это не важно
		case 5: dx = 0; dy = -speed;   break;//интовое значение не имеющее отношения к направлению, пока просто стрельнем вверх, нам сейчас это не важно
		}

		x += dx*time;//само движение пули по х
		y += dy*time;//по у

		if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}*/
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пуле
	}
};

/*void interactionWithMap(player &p)//ф-ция взаимодействия с картой
{
	for (int i = p.y / 53; i < (p.y + p.h) / 53; i++)//проходимся по всей карте, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
		for (int j = p.x / 54; j < (p.x + p.w) / 54; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
		{
			if (TileMap[i][j] == '1')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
			{
				if (p.dy > 0)//если мы шли вниз,
				{
					p.y = i * 50 - 1;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
				}
				if (p.dy < 0)
				{
					p.y = i * 50 + 100;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
				}
				if (p.dx > 0)
				{
					p.x = j * 50 - 1;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
				}
				if (p.dx < 0)
				{
					p.x = j * 50 + 100;//аналогично идем влево
				}
			}
			if (TileMap[i][j] == 'e') {
				p.health -= 40;//если взяли ядовитейший в мире цветок,то переменная health=health-40;
				TileMap[i][j] = '0';//убрали цветок
			}
		}
}*/

/*void player_control(player &p, float time, Sprite &hero, Sprite &gun) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
{
	if (p.dir == 0)//реализуем поведение в зависимости от направления. (каждая цифра соответствует направлению)
	{
		p.dx = p.speed; p.dy = 0;
	}//вправо
	if (p.dir == 1)
	{
		p.dx = -p.speed; p.dy = 0;
	}//влево
	if (p.dir == 2)
	{
		p.dx = 0; p.dy = p.speed;
	}//вниз
	if (p.dir == 3)
	{
		p.dx = 0; p.dy = -p.speed;
	}//вверх
	p.x += p.dx*time;//то движение из прошлого урока. наше ускорение на время получаем смещение координат и как следствие движение
	p.y += p.dy*time;//аналогично по игреку
	p.speed = 0;//зануляем скорость, чтобы персонаж остановился.
	interactionWithMap(p);//вызываем функцию, отвечающую за взаимодействие с картой
	hero.setPosition(p.x, p.y); //выводим спрайт в позицию x y , посередине. бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
	gun.setPosition(p.x, p.y);
	if (p.health <= 0) { p.life = false; p.speed = 0; }//если жизней меньше либо равно 0, то умираем и исключаем движение игрока после смерти
}*/

/*void move_player(float rotation, float time, player &p, float &CurrentFrame, Sprite &hero, Sprite &gun) {
	bool press = false;
	hero.setRotation(rotation);//поворачиваем спрайт на эти градусы
	gun.setRotation(rotation);
	if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
		press = true;
		p.dir = 1; p.speed = 0.2; p.dir2 = -1;//dir =1 - направление вверх, speed =0.1 - скорость движения. Заметьте - время мы уже здесь ни на что не умножаем и нигде не используем каждый раз
	}

	if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
		if (press == true)
			p.dir2 = 0;
		else {
			press = true;
			p.dir = 0; p.speed = 0.2;p.dir2 = -1;//направление вправо, см выше
		}
	}

	if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
		if (press == true)
			p.dir2 = 3;
		else {
			press = true;
			p.dir = 3; p.speed = 0.2; p.dir2 = -1;//направление вправо, см выше
		}
	}

	if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
		if (press == true)
			p.dir2 = 2;
		else {
			press = true;
			p.dir = 2; p.speed = 0.2; p.dir2 = -1;//направление вправо, см выше
		}
	}
	CurrentFrame += 0.005*time;
	if (CurrentFrame > 4) CurrentFrame -= 4;
	hero.setTextureRect(IntRect(0, p.h * int(CurrentFrame), p.w, p.h)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
	getplayercoordinateforview(p.x, p.y);//передаем координаты игрока в функцию управления камеро
}*/

int main(){
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

	std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хранятся в этом векторе


	Image heroImage, gunImage, easyEnemyImage, bulletImage;
	bulletImage.loadFromFile("images/shot.png");
	heroImage.loadFromFile("images/players_anim.png");
	gunImage.loadFromFile("images/weapon.png");
	easyEnemyImage.loadFromFile("images/easy_enemy.png");
	std::cout << player.rect.left << player.rect.top;
	Player p(heroImage, gunImage, lvl, player.rect.left, player.rect.top, 91, 54, 72, 20, "Player");//создаем объект p класса player,задаем "hero.png" как имя файла+расширение, далее координата Х,У, ширина, высота.
	Clock clock;

	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new Enemy(easyEnemyImage, gunImage, lvl, e[i].rect.left, e[i].rect.top, 43, 75, 0, 0, "easyEnemy"));//и закидываем в список всех наших врагов с карты

	//Enemy easyEnemy(easyEnemyImage, gunImage, lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 43, 75, 0, 0, "EasyEnemy");
	bool ret = false;

	float CurrentFrame = 0;//хранит текущий кадр
	while (window.isOpen())
	{
		std::list<Entity*>::iterator at;//второй итератор.для взаимодействия между объектами списка
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
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
		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity *b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false) { it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
		for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
		{
			for (at = entities.begin(); at != entities.end(); at++) {
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "easyEnemy"))) {
					(*it)->health -= 13;
					(*at)->life = false;
				}
			}
			if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
			{
				if ((*it)->name == "easyEnemy") {//и при этом имя объекта EasyEnemy,то..
					(*it)->dx = 0;
					p.health -= 1;
				}
			}
		}
		if (!p.life)
			window.close();
		//for (it = entities.begin(); it != entities.end(); it++) { (*it)->update(time); }//для всех элементов списка(пока это только враги,но могут быть и пули к примеру) активируем ф-цию update
		window.setView(view);
		window.clear();
		lvl.Draw(window);//рисуем новую карту
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		//window.draw(easyEnemy.sprite);
		window.draw(p.gunSprite);
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}