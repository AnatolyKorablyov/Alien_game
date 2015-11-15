#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой
void getPlayerCoordinateForView(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер€ем их, чтобы убрать кра€
 
	if (x < 685) tempX = 685;//убираем из вида левую сторону
	if (x > 1325) tempX = 1325;//убираем из вида правую сторону
	if (y < 380) tempY = 380;//верхнюю сторону
	if (y > 1650) tempY = 1650;//нижнюю сторону	
 
	view.setCenter(tempX, tempY); //следим за игроком, передава€ его координаты. 
	
}