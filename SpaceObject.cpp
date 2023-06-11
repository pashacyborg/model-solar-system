#include <iostream>
#include "SpaceObject.h"
#include <GL\glew.h>
#include <GL\freeglut_std.h>
#include <string>
#include "MyError.h"

using namespace std;

SpaceObject::SpaceObject(double _radius, double _distance, double _orbit, double _speedOrbit, double _axisTilt, double _axisAni, double _mass)
{
	setlocale(LC_ALL, "Russian");

	if (_radius <=0)
	{
		cout << "Неправильно указан радиус! \n";
		cout << "Объект сгененирован с ошибкой! \n";
	}
	else
	{
		radius = _radius;
	}

	distance = _distance;
	orbit = _orbit;

	if (_speedOrbit < 0)
	{
		cout << "Неправильно указана скорость! \n";
		cout << "Объект сгененирован с ошибкой! \n";
	}
	else
	{
		speedOrbit = _speedOrbit;
	}

	if (_axisTilt < 0)
	{
		cout << "Неправильно указан наклон! \n";
		cout << "Объект сгененирован с ошибкой! \n";
	}
	else
	{
		axisTilt = _axisTilt;
	}

	if (_axisAni < 0)
	{
		cout << "Неправильно указано вращение! \n";
		cout << "Объект сгененирован с ошибкой! \n";
	}
	else
	{
		axisAni = _axisAni;
	}

	if (_mass < 0)
	{
		cout << "Неправильно указана масса! \n";
		cout << "Объект сгененирован с ошибкой! \n";
	}
	else
	{
		mass = _mass;
	}

	/*radius = _radius;*/
	//speedOrbit = _speedOrbit;
}

double SpaceObject::getRadius()
{
	return radius;
}

void SpaceObject::setOrbit(double o, int index)
{
	if (index == 0)
	{
		orbit -= o;
	}
	if (index == 1)
	{
		orbit += o;
	}
}

double SpaceObject::getOrbit()
{
	return orbit;
}

double SpaceObject::getSpeedOrbit()
{
	return speedOrbit;
}

void SpaceObject::setDistance(double d)
{
	distance -= d;
}

double SpaceObject::getDistance()
{
	return distance;
}

double SpaceObject::getAxisTilt()
{
	return axisTilt;
}

void SpaceObject::setAxisAni(double aa, int index)
{
	if (index == 0)
	{
		axisAni -= aa;
	}
	if (index == 1)
	{
		axisAni += aa;
	}
}

double SpaceObject::getAxisAni()
{
	return axisAni;
}

double SpaceObject::getMass()
{
	return mass;
}

void SpaceObject::resetForComet()
{
	distance = 60;
	orbit = 0;
	speedOrbit = 0.2;
	axisTilt = 0;
	axisAni = 0;
}


void SpaceObject::glutWireTorusAll()
{
	glutWireTorus(0.001, distance, 100, 100);
}

void SpaceObject::oneLoop()
{
	if (getOrbit() > 360)
	{
		setOrbit(360, 0);
	}
}

void SpaceObject::oneLoopOurAxis()
{
	if (getAxisAni() > 360)
	{
		setAxisAni(360, 0);
	}
}

void SpaceObject::drawSmallOrbit(void)
{
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, distance, 100.0, 100.0);
	glPopMatrix();
}

void SpaceObject::string_sonvertMoons(void* font, char* string)
{
	char *c;
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void SpaceObject::drawMoon(char* str, int activeLabel, GLint texture)
{
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glRotatef(orbit, 0, 1, 0);
	glTranslatef(distance, 0, 0);

	if (activeLabel == 1)
	{
		glRasterPos3d(-1.2, 7, 0);
		string_sonvertMoons(GLUT_BITMAP_TIMES_ROMAN_10, str);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	//glDisable(GL_TEXTURE_2D);

	gluSphere(quadric, getRadius(), 20, 20);
	glPopMatrix();
}
