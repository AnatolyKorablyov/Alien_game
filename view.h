#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������
void getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����
 
	if (x < 700) tempX = 700;//������� �� ���� ����� �������
	if (x > 800) tempX = 800;//������� �� ���� ����� �������
	if (y < 400) tempY = 400;//������� �������
	if (y > 1000) tempY = 1000;//������ �������	
 
	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 
	
}