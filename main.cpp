#include <iostream>

//библиотеки для рендеринга OpenGl
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
//библиотека, чтобы подцепить пикчу
#include "image.h"
#include "SpaceObject.h"
#include"MyError.h"

#include <exception>

#pragma comment(lib, "glew32.lib")

using namespace std;

//Солнце и все небесные тела
SpaceObject sun(5.5, 0, 0, 0, 0, 0, 1.98847);			//Солнце
SpaceObject mer(1, 7, 0, 4.74, 2.11, 0, 3.3011);		//Меркурий
SpaceObject ven(1, 11, 0, 3.50, 177.0, 0, 4.886);		//Венера
SpaceObject ear(2, 16, 0, 2.98, 23.44, 0, 5.97);		//Земля
SpaceObject mar(1, 21, 0, 2.41, 25.00, 0, 6.4171);		//Марс
SpaceObject jup(3, 28, 0, 1.31, 3.13, 0, 1.89);		//Юпитер
SpaceObject sat(3, 37, 0, 0.97, 26.70, 0, 5.66);		//Сатурн
SpaceObject ura(2, 45, 0, 0.68, 97.77, 0, 86.05);		//Уран
SpaceObject nep(2, 53, 0, 0.54, 28.32, 0, 1.02);		//Нептун

SpaceObject lun(0.40, 3, 0, 5.40, 0, 0, 7.35);			//Луна

//спутники марса
SpaceObject pho(0.20, 1.8, 0, 2.3, 0, 0, 1.07);		//Фобос
SpaceObject dei(0.24, 2.4, 0, 3.6, 0, 0, 1.48);		//Деймос

//спутники юпитера
SpaceObject Io(0.60, 4.5, 0, 2.3, 0, 0, 8.93);		//Ио
SpaceObject eur(0.50, 5, 0, 4.4, 0, 0, 4.8);		//Европа
SpaceObject gan(0.42, 5.5, 0, 5, 0, 0, 1.48);		//Гaнимeд
SpaceObject cal(0.37, 6, 0, 2.3, 0, 0, 0.9);		//Kaллиcтo

//спутники урана
SpaceObject titania(0.26, 2.9, 0, 7, 0, 0, 3.527);		//Титания

//спутники нептуна
SpaceObject tri(0.36, 3.2, 0, 3.4, 0, 0, 2.14);		//Тритон

SpaceObject comet(1, 60, 0, 0.2, 0, 0, 0);		//comet

//допы
int isAnimate = 0;
int bigOrbitActive = 1;
int smallOrbitActive = 1;

int moonsActive = 1;


int labelsActive = 0;

int changeCamera = 0;
int CameraPosition = 50;

int cat = 0;

int newSpeed = 0;

int dir = 0;
int newSituation = 0;
int countComet = 0;


//взято с http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html 
//подцепляем текстуры к сферам
GLuint loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId); //Функция glGenTextures генерирует имена текстур.
								  //Функция glGenTextures возвращает n имен текстур в параметре textures.

	glBindTexture(GL_TEXTURE_2D, textureId); //Функция glBindTexture позволяет создавать текстуру, 
											 //привязанную к объекту текстуры.

	// Сопоставляем изображение с текстурой

	glTexImage2D(GL_TEXTURE_2D, //Всегда GL_TEXTURE_2D

		0, //Уровень 0 - это базовый уровень изображения. 
			//Уровень n - это N-е уменьшенное изображение, а нам такое не надо

		GL_RGB, //Формат OpenGL для изображений

		image->width, image->height, //длина и высота

		0, //Ширина границы. Должно быть либо 0, либо 1.

		GL_RGB, //GL_RGB, пиксели в ргб формате. 
				//Каждый элемент-это RGB-тройка.

		GL_UNSIGNED_BYTE, //Тип данных пиксельных данных.


		image->pixels); //Указатель на данные изображения в памяти.

	return textureId; //Функция возвращает id текстуры

}

//id текстур
GLuint sunTexture,
merTexture,
venTexture,
earTexture,
marTexture,
jupTexture,
satTexture,
uraTexture,
nepTexture,
pluTexture,

spaceTexture,

cometTexture,

moonTexture,

phoTexture,
deiTexture,

IoTexture,
eurTexture,
ganTexture,
calTexture,

titTexture,

triTexture;

void preparation(void){
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	//свечение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	Image* space = loadBMP("stars.bmp");	spaceTexture = loadTexture(space);	delete space;
	Image* sun = loadBMP("sun.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercury.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("earth.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("mars.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturn.bmp");		satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("uranus.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("neptune.bmp");	nepTexture = loadTexture(nep);		delete nep;
	Image* comet = loadBMP("comet.bmp");	cometTexture = loadTexture(comet);	delete comet;
	
	Image* moon = loadBMP("moon.bmp");		moonTexture = loadTexture(moon);	delete moon;

	Image* pho = loadBMP("phobos.bmp");		phoTexture = loadTexture(pho);		delete pho;
	Image* dei = loadBMP("deimos.bmp");		deiTexture = loadTexture(dei);		delete dei;

	Image* Io = loadBMP("Io.bmp");			IoTexture = loadTexture(Io);		delete Io;
	Image* eur = loadBMP("europe.bmp");		eurTexture = loadTexture(eur);		delete eur;
	Image* gan = loadBMP("ganimed.bmp");	ganTexture = loadTexture(gan);		delete gan;
	Image* cal = loadBMP("callista.bmp");	calTexture = loadTexture(cal);		delete cal;

	Image* tit = loadBMP("titania.bmp");	titTexture = loadTexture(tit);		delete tit;

	Image* tri = loadBMP("triton.bmp");		triTexture = loadTexture(tri);		delete tri;

	if (cat == 1)
	{
		Image* space = loadBMP("cat.bmp");		spaceTexture = loadTexture(space);		delete space;
	}


	//переменные для света
	float lightPos[] = { 0, 0, -50, 1 }; // Положение источника света
	static float spotAngle = 50; // Конус прожектора полуугловой
	float spotDirection[] = { 1, 0, 0 }; //Направление источника света
	static float spotExponent = 1.0; // показатель источника света

	//свет
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0, 0, 0, 1 };
	float lightDifAndSpec[] = { 1, 1, 1, 1 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}

void string_sonvert(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void orbitalTrails(void){
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0, 0, 0);
	glRotatef(90, 1, 0, 0);
	mer.glutWireTorusAll();
	ven.glutWireTorusAll();
	ear.glutWireTorusAll();
	mar.glutWireTorusAll();
	jup.glutWireTorusAll();
	sat.glutWireTorusAll();
	ura.glutWireTorusAll();
	nep.glutWireTorusAll();
	glPopMatrix();
}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 200);
	glMatrixMode(GL_MODELVIEW);
}
void rendering_planets(GLUquadric* quadric, SpaceObject planet, GLint texture, char* s)
{
	if (s == "Comet")
	{
		if (countComet > 1)
		{
			comet.resetForComet();
			countComet = 1;
		}

		glPushMatrix();

		glRotatef(planet.getOrbit(), 0, 1, 0);
		
		glTranslatef(planet.getDistance(), 20, -20);

		if (labelsActive == 1)
		{
			glRasterPos3d(0, 3, 0);
			glColor3ub(255, 255, 255);
			string_sonvert(GLUT_BITMAP_TIMES_ROMAN_10, s);
		}
		
		GLdouble equation[4] = { 0, 1, 0,0 };
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, equation);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);

		glColor3d(255, 255, 255);
		gluSphere(quadric, planet.getRadius(), 20, 20);
		glDisable(GL_CLIP_PLANE0);

		glPopMatrix();
	}
	else
	{

		glColor3ub(255, 255, 255); //белый 
		glPushMatrix();
		glRotatef(planet.getOrbit(), 0, 1, 0);
		glTranslatef(planet.getDistance(), 0, 0);

		if (labelsActive == 1)
		{
			if (s == "Sun")
			{
				glRasterPos3d(-1.2, 7, 0); //void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z);
			}
			else
			{
				if (s == "Saturn")
				{
					glRasterPos3d(0, 4.4, 0);
				}
				else
				{
					glRasterPos3d(0, 3, 0);
				}
			}
			glColor3ub(255, 255, 255);
			string_sonvert(GLUT_BITMAP_TIMES_ROMAN_10, s);
		}

		glPushMatrix();
		glRotatef(planet.getAxisTilt(), 1, 0, 0);
		glRotatef(planet.getAxisAni(), 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		gluQuadricTexture(quadric, 1);
		glColor3ub(255, 255, 255); // цвет объекта на модели (белый)
		gluSphere(quadric, planet.getRadius(), 20, 20);
		if (s == "Saturn")
		{
			glPopMatrix(); //чтобы кольца не вращались
		}
		glDisable(GL_TEXTURE_2D);
		if (s == "Saturn")
		{
			glPushMatrix();
			glColor3ub(158, 145, 137);
			glRotatef(-63, 1, 0, 0);
			glutWireTorus(0.2, 6, 30, 30);
			glutWireTorus(0.4, 5, 30, 30);
			glPopMatrix();
		}
		glPopMatrix();
		if (s == "Earth")
		{
			if (smallOrbitActive == 1)
			{
				lun.drawSmallOrbit();
			}
			if (moonsActive == 1)
			{
				lun.drawMoon("Moon", labelsActive, moonTexture);
			}
		}

		if (s == "Mars")
		{
			if (smallOrbitActive == 1)
			{
				pho.drawSmallOrbit();
				dei.drawSmallOrbit();
			}

			if (moonsActive == 1)
			{
				pho.drawMoon("Phobos", labelsActive, phoTexture);
				dei.drawMoon("Deimos", labelsActive, deiTexture);
			}
		}

		if (s == "Jupiter")
		{
			if (smallOrbitActive == 1)
			{
				eur.drawSmallOrbit();
				gan.drawSmallOrbit();
				cal.drawSmallOrbit();
				Io.drawSmallOrbit();
			}

			if (moonsActive == 1)
			{
				eur.drawMoon("Europe", labelsActive, eurTexture);
				gan.drawMoon("Ganimed", labelsActive, ganTexture);
				cal.drawMoon("Callista", labelsActive, calTexture);
				Io.drawMoon("Io", labelsActive, IoTexture);
			}
		}

		if (s == "Uranus")
		{
			if (smallOrbitActive == 1)
			{
				titania.drawSmallOrbit();
			}

			if (moonsActive == 1)
			{
				titania.drawMoon("Titania", labelsActive, titTexture);
			}
		}

if (s == "Neptune")
{
	if (smallOrbitActive == 1)
	{
		tri.drawSmallOrbit();
	}

	if (moonsActive == 1)
	{
		tri.drawMoon("Triton", labelsActive, triTexture);
	}
}

glPopMatrix();

if (s == "Saturn")
{
	glColor3ub(255, 255, 255);
}
	}
}

void frame_rendering(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (changeCamera == 0)gluLookAt(0, CameraPosition, 50, 0, 0, 0, 0, 1, 0);
	if (changeCamera == 1)gluLookAt(0, 0, CameraPosition, 0, 0, 0, 0, 1, 0);
	if (changeCamera == 2)gluLookAt(0, CameraPosition, 0.00001, 0, 0, 0, 0, 1, 0);

	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric* quadric;
	quadric = gluNewQuadric();

	//солнце
	rendering_planets(quadric, sun, sunTexture, "Sun");

	//меркурий
	rendering_planets(quadric, mer, merTexture, "Mercury");

	//венера
	rendering_planets(quadric, ven, venTexture, "Venus");

	//земля и луна
	rendering_planets(quadric, ear, earTexture, "Earth");

	//марс и спутники
	rendering_planets(quadric, mar, marTexture, "Mars");

	//юпитер и спутники
	rendering_planets(quadric, jup, jupTexture, "Jupiter");

	//сатурн, кольца и спутники
	rendering_planets(quadric, sat, satTexture, "Saturn");

	//уран и спутники
	rendering_planets(quadric, ura, uraTexture, "Uranus");

	//нептун и спутники
	rendering_planets(quadric, nep, nepTexture, "Neptune");

	if (newSituation == 1)
	{
		rendering_planets(quadric, comet, cometTexture, "Comet");
	}

	//настройки фона
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spaceTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);

	//задняя стенка
	glTexCoord2f(-1, 0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	//нижняя стенка
	glBindTexture(GL_TEXTURE_2D, spaceTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex3f(-200, -45, 200);
	glTexCoord2f(2.0, 1.0); glVertex3f(200, -45, 200);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, -45, -200);
	glTexCoord2f(1.0, 2.0); glVertex3f(-200, -45, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}

void animation(int n)
{
	if (isAnimate)
	{
		if (newSpeed == 2)
		{
			mer.setOrbit(mer.getSpeedOrbit() / 4, 1);
			ven.setOrbit(ven.getSpeedOrbit() / 4, 1);
			ear.setOrbit(ear.getSpeedOrbit() / 4, 1);
			mar.setOrbit(mar.getSpeedOrbit() / 4, 1);
			jup.setOrbit(jup.getSpeedOrbit() / 4, 1);
			sat.setOrbit(sat.getSpeedOrbit() / 4, 1);
			ura.setOrbit(ura.getSpeedOrbit() / 4, 1);
			nep.setOrbit(nep.getSpeedOrbit() / 4, 1);

			lun.setOrbit(lun.getSpeedOrbit() / 4, 1);
			pho.setOrbit(pho.getSpeedOrbit() / 4, 1);
			dei.setOrbit(dei.getSpeedOrbit() / 4, 1);
			eur.setOrbit(eur.getSpeedOrbit() / 4, 1);
			gan.setOrbit(gan.getSpeedOrbit() / 4, 1);
			cal.setOrbit(cal.getSpeedOrbit() / 4, 1);
			Io.setOrbit(Io.getSpeedOrbit() / 4, 1);

			titania.setOrbit(titania.getSpeedOrbit() / 4, 1);
			tri.setOrbit(tri.getSpeedOrbit() / 4, 1);

			//comet.setDistance(comet.getSpeedOrbit()/4);
			if (abs(comet.getDistance()) < 45)
			{
				comet.setDistance(comet.getSpeedOrbit()/4 / 2);
			}
			else
			{
				comet.setDistance(comet.getSpeedOrbit()/4 * 4);
			}
			if (abs(comet.getDistance()) < 45)
			{
				comet.setOrbit(comet.getSpeedOrbit()/ 4, 1);
			}
		}

		if (newSpeed == 1)
		{
			mer.setOrbit(mer.getSpeedOrbit() * 2, 1);
			ven.setOrbit(ven.getSpeedOrbit() * 2, 1);
			ear.setOrbit(ear.getSpeedOrbit() * 2, 1);
			mar.setOrbit(mar.getSpeedOrbit() * 2, 1);
			jup.setOrbit(jup.getSpeedOrbit() * 2, 1);
			sat.setOrbit(sat.getSpeedOrbit() * 2, 1);
			ura.setOrbit(ura.getSpeedOrbit() * 2, 1);
			nep.setOrbit(nep.getSpeedOrbit() * 2, 1);

			lun.setOrbit(lun.getSpeedOrbit() * 2, 1);
			pho.setOrbit(pho.getSpeedOrbit() * 2, 1);
			dei.setOrbit(dei.getSpeedOrbit() * 2, 1);
			eur.setOrbit(eur.getSpeedOrbit() * 2, 1);
			gan.setOrbit(gan.getSpeedOrbit() * 2, 1);
			cal.setOrbit(cal.getSpeedOrbit() * 2, 1);
			Io.setOrbit(Io.getSpeedOrbit() * 2, 1);

			titania.setOrbit(titania.getSpeedOrbit() *2, 1);
			tri.setOrbit(tri.getSpeedOrbit() *2, 1);

			//comet.setDistance(comet.getSpeedOrbit()*2);
			if (abs(comet.getDistance()) < 45)
			{
				comet.setDistance(comet.getSpeedOrbit()*2 / 2);
			}
			else
			{
				comet.setDistance(comet.getSpeedOrbit()*2 * 4);
			}
			if (abs(comet.getDistance()) < 45)
			{
				comet.setOrbit(comet.getSpeedOrbit() * 2, 1);
			}
		}

		if (newSpeed == 0)
		{
			mer.setOrbit(mer.getSpeedOrbit(), 1);
			ven.setOrbit(ven.getSpeedOrbit(), 1);
			ear.setOrbit(ear.getSpeedOrbit(), 1);
			mar.setOrbit(mar.getSpeedOrbit(), 1);
			jup.setOrbit(jup.getSpeedOrbit(), 1);
			sat.setOrbit(sat.getSpeedOrbit(), 1);
			ura.setOrbit(ura.getSpeedOrbit(), 1);
			nep.setOrbit(nep.getSpeedOrbit(), 1);

			lun.setOrbit(lun.getSpeedOrbit(), 1);
			pho.setOrbit(pho.getSpeedOrbit(), 1);
			dei.setOrbit(dei.getSpeedOrbit(), 1);
			eur.setOrbit(eur.getSpeedOrbit(), 1);
			gan.setOrbit(gan.getSpeedOrbit(), 1);
			cal.setOrbit(cal.getSpeedOrbit(), 1);
			Io.setOrbit(Io.getSpeedOrbit(), 1);

			titania.setOrbit(titania.getSpeedOrbit(), 1);
			tri.setOrbit(tri.getSpeedOrbit(), 1);

			if (abs(comet.getDistance()) < 45)
			{
				comet.setDistance(comet.getSpeedOrbit()/2);
			}
			else
			{
				comet.setDistance(comet.getSpeedOrbit()*4);
			}
			if (abs(comet.getDistance()) < 45)
			{
				comet.setOrbit(comet.getSpeedOrbit(), 1);
			}
		}

		mer.oneLoop();

		ven.oneLoop();

		ear.oneLoop();

		mar.oneLoop();

		jup.oneLoop();

		sat.oneLoop();

		ura.oneLoop();

		nep.oneLoop();

		lun.oneLoop();

		pho.oneLoop();

		dei.oneLoop();

		eur.oneLoop();

		gan.oneLoop();

		cal.oneLoop();

		Io.oneLoop();

		titania.oneLoop();

		tri.oneLoop();

		mer.setAxisAni(10, 1);
		ven.setAxisAni(10, 1);
		ear.setAxisAni(10, 1);
		mar.setAxisAni(10, 1);
		jup.setAxisAni(10, 1);
		sat.setAxisAni(10, 1);
		ura.setAxisAni(10, 1);
		nep.setAxisAni(10, 1);

		mer.oneLoopOurAxis();

		ven.oneLoopOurAxis();

		ear.oneLoopOurAxis();

		mar.oneLoopOurAxis();

		jup.oneLoopOurAxis();

		sat.oneLoopOurAxis();

		ura.oneLoopOurAxis();

		nep.oneLoopOurAxis();

		glutPostRedisplay();
		glutTimerFunc(30, animation, 1);
	}
}

void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		exit(0);
	}

	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && CameraPosition < 100)
	{
		CameraPosition++;
	}

	if (direction < 0 && CameraPosition > 0)
	{
		CameraPosition--;
	}

	glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		//escape
	case 27: 
		exit(0); 
		break; 

	case ' ': 
		if (isAnimate)
		{
			isAnimate = 0;
		}
		else 
		{ 
			isAnimate = 1; 
			animation(1); 
		} 
		break;

	case 'o': 
		if (smallOrbitActive) 
		{
			smallOrbitActive = 0; 
		}
		else
		{
			smallOrbitActive = 1; 
		}
		glutPostRedisplay(); 
		break;

	case 'O': 
		if (bigOrbitActive)
		{
			bigOrbitActive = 0;
		}
		else
		{
			bigOrbitActive = 1; 
		}
		glutPostRedisplay(); 
		break;

	case 'm': 
		if (moonsActive)
		{
			moonsActive = 0; 
		}
		else
		{
			moonsActive = 1;
		}
		glutPostRedisplay();
		break;

	case 'M': 
		if (moonsActive)
			moonsActive = 0;
		else 
			moonsActive = 1;
		glutPostRedisplay(); 
		break;


	case 'l': 
		if (labelsActive)
		{
			labelsActive = 0;
		}
		else
		{
			labelsActive = 1; 
		}
		glutPostRedisplay();
		break;

	case 'L': 
		if (labelsActive)
		{
			labelsActive = 0; 
		}
		else
		{
			labelsActive = 1;
		}
		glutPostRedisplay(); 
		break;

	case '1': 
		changeCamera = 0; 
		glutPostRedisplay();
		break;

	case '2': 
		changeCamera = 1;
		glutPostRedisplay();
		break;

	case '3':
		changeCamera = 2; 
		glutPostRedisplay();
		break;

	case 'C':
		if (cat)
		{
			cat = 0; 
		}
		else
		{
			cat = 1;
		}
		preparation(); 
		break;

	case 'c': 
		if (cat)
		{
			cat = 0;
		}
		else
		{
			cat = 1;
		}
		preparation(); 
		break;

	case '=':
		newSpeed = 1; 
		animation(1); 
		break;

	case '-': 
		newSpeed = 2; 
		animation(1); 
		break;

	case '0':
		newSpeed = 0; 
		animation(1); 
		break;

	case 'S':
		newSituation = 1;
		countComet++;
		glutPostRedisplay();
		break;

	case 's':
		newSituation = 1;
		countComet++;
		glutPostRedisplay();
		break;

	case 'I':

		std::cout << "Объект: Sun \n";
		std::cout << "Масса объекта: " << sun.getMass() << "*10^30кг \n";

		std::cout << "Объект: Mercury \n";
		std::cout << "Масса объекта: " << mer.getMass() << "*10^23кг \n";

		std::cout << "Объект: Mars \n";
		std::cout << "Масса объекта: " << mar.getMass() << "*10^23кг \n";

		std::cout << "Объект: Ganimed \n";
		std::cout << "Масса объекта: " << gan.getMass() << "*10^23кг \n";

		std::cout << "Объект: Callista \n";
		std::cout << "Масса объекта: " << cal.getMass() << "*10^23кг \n";

		std::cout << "Объект: Venus \n";
		std::cout << "Масса объекта: " << ven.getMass() << "*10^24кг \n";

		std::cout << "Объект: Earth \n";
		std::cout << "Масса объекта: " << ear.getMass() << "*10^24кг \n";

		std::cout << "Объект: Jupiter \n";
		std::cout << "Масса объекта: " << jup.getMass() << "*10^27кг \n";

		std::cout << "Объект: Saturn \n";
		std::cout << "Масса объекта: " << sat.getMass() << "*10^26кг \n";

		std::cout << "Объект: Neptune \n";
		std::cout << "Масса объекта: " << nep.getMass() << "*10^26кг \n";

		std::cout << "Объект: Uranus \n";
		std::cout << "Масса объекта: " << ura.getMass() << "*10^25кг \n";

		std::cout << "Объект: Moon \n";
		std::cout << "Масса объекта: " << lun.getMass() << "*10^22кг \n";

		std::cout << "Объект: Io \n";
		std::cout << "Масса объекта: " << Io.getMass() << "*10^22кг \n";

		std::cout << "Объект: Europe \n";
		std::cout << "Масса объекта: " << eur.getMass() << "*10^22кг \n";

		std::cout << "Объект: Triton \n";
		std::cout << "Масса объекта: " << tri.getMass() << "*10^22кг \n";

		std::cout << "Объект: Phobos \n";
		std::cout << "Масса объекта: " << pho.getMass() << "*10^16кг \n";

		std::cout << "Объект: Deimos \n";
		std::cout << "Масса объекта: " << dei.getMass() << "*10^15кг \n";

		std::cout << "Объект: Titania \n";
		std::cout << "Масса объекта: " << dei.getMass() << "*10^21кг \n";

		break;

	case 'i':
		
		std::cout << "Объект: Sun \n";
		std::cout << "Масса объекта: " << sun.getMass() << "10^30кг \n";

		std::cout << "Объект: Mercury \n";
		std::cout << "Масса объекта: " << mer.getMass() << "10^23кг \n";

		std::cout << "Объект: Mars \n";
		std::cout << "Масса объекта: " << mar.getMass() << "10^23кг \n";

		std::cout << "Объект: Ganimed \n";
		std::cout << "Масса объекта: " << gan.getMass() << "10^23кг \n";

		std::cout << "Объект: Callista \n";
		std::cout << "Масса объекта: " << cal.getMass() << "10^23кг \n";

		std::cout << "Объект: Venus \n";
		std::cout << "Масса объекта: " << ven.getMass() << "10^24кг \n";

		std::cout << "Объект: Earth \n";
		std::cout << "Масса объекта: " << ear.getMass() << "10^24кг \n";

		std::cout << "Объект: Jupiter \n";
		std::cout << "Масса объекта: " << jup.getMass() << "10^27кг \n";

		std::cout << "Объект: Saturn \n";
		std::cout << "Масса объекта: " << sat.getMass() << "10^26кг \n";

		std::cout << "Объект: Neptune \n";
		std::cout << "Масса объекта: " << nep.getMass() << "10^26кг \n";

		std::cout << "Объект: Uranus \n";
		std::cout << "Масса объекта: " << ura.getMass() << "10^25кг \n";

		std::cout << "Объект: Moon \n";
		std::cout << "Масса объекта: " << lun.getMass() << "10^22кг \n";

		std::cout << "Объект: Io \n";
		std::cout << "Масса объекта: " << Io.getMass() << "10^22кг \n";

		std::cout << "Объект: Europe \n";
		std::cout << "Масса объекта: " << eur.getMass() << "10^22кг \n";

		std::cout << "Объект: Triton \n";
		std::cout << "Масса объекта: " << tri.getMass() << "10^22кг \n";

		std::cout << "Объект: Phobos \n";
		std::cout << "Масса объекта: " << pho.getMass() << "10^16кг \n";

		std::cout << "Объект: Deimos \n";
		std::cout << "Масса объекта: " << dei.getMass() << "10^15кг \n";

		std::cout << "Объект: Titania \n";
		std::cout << "Масса объекта: " << dei.getMass() << "10^21кг \n";

		break;
	}
}

void intructions(void)
{
	cout << "ПРОБЕЛ для запуска/паузы" << endl;
	cout << "ESC для закрытия" << endl;
	cout << "ВЫПОЛНЯЙТЕ НА АНГЛИЙСКОЙ РАСКЛАДКЕ" << endl;
	cout << "О для видимости орбит планет" << endl;
	cout << "o для видимости орбит спутников" << endl;
	cout << "M/m для видимости спутников" << endl;
	cout << "L/l для показа надписи" << endl;
	cout << "1, 2  или 3 для разнных углов обзора" << endl;
	cout << "Можно использовать колесико мыши для отладки обзора" << endl;
	cout << "C/c для смены фона" << endl;
	cout << "Лучше всего переключать режимы после установки паузы," << endl;
	cout << "иначе случится баг и планеты будут ускоряться и ускоряться.." << endl;
	cout << "Чинится баг тоже нажатием паузы :)" << endl;
	cout << "0, -, = для управления скоростью" << endl;
	cout << "0 для @естественной@ скорости" << endl;
	cout << "- замедление" << endl;
	cout << "= ускорение" << endl;
	cout << "S/s вызов ситуации (полет летающего объкта - кометы, хотя больше похоже на нло)" << endl;
	cout << "Иногда возникают баги и стоит нажимать S/s 2 раза" << endl;
	cout << "I/i для получения информации о массах объектов." << endl;
}

int main(int argc, char **argv){
	setlocale(LC_ALL, "Russian");

	intructions();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("task 4");

	glutDisplayFunc(frame_rendering);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	preparation();
	glutMainLoop();
}