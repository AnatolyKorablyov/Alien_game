#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой
void getplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер€ем их, чтобы убрать кра€
 
	if (x < 700) tempX = 700;//убираем из вида левую сторону
	if (x > 800) tempX = 800;//убираем из вида левую сторону
	if (y < 400) tempY = 400;//верхнюю сторону
	if (y > 1000) tempY = 1000;//нижнюю сторону	
 
	view.setCenter(tempX, tempY); //следим за игроком, передава€ его координаты. 
	
}