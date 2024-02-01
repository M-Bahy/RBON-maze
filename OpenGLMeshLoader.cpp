#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <iostream>
#include <string> 
#include <Windows.h>
#include <MMSystem.h>
#include <mciapi.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <playsoundapi.h>
#include <cstdlib>
#include <ctime>   
#include <vector>
#include <stdio.h>
#include <string.h>
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;
bool up = true;
float x = 1;
char* p0s[20];
double posHeroX = 15;
double posHeroY = 1.35;
double posHeroZ = -15;
void update(int value);
int rotateHero = 0;
bool coinOne = true;
bool coinTwo = true;
bool coinThree = true;
int timeRemaining = 33;
int score = 0;
int state = 0;
int cameraPerspective = 3;
float rotationAngle = 0.0f;

float eyeX = posHeroX; float eyeY = posHeroY + 2; float eyeZ = posHeroZ - 3;float centerX = posHeroX;float centerY = posHeroY;float centerZ = posHeroZ; float upX = 0.0f; float upY = 1.0f; float upZ = 0.0f;

bool isWin = false;












void CheckGLError(const char* checkpoint);

void drawMaze();
void drawCoins();

void InitLightSourcePlayer();


void InitLightSourceWholeGame();



//identifiers 



void drawWalls();
void setupCamera();




class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}


	Vector operator+(Vector& v) {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector operator-(Vector& v) {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector operator*(GLdouble n) {
		return Vector(x * n, y * n, z * n);
	}

	Vector operator/(GLdouble n) {
		return Vector(x / n, y / n, z / n);
	}

	Vector unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector cross(Vector v) {
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};



class Camera {
public:
	Vector Eye, At, Up;

	Camera() {
		Eye = Vector(eyeX, eyeY, eyeZ);
		At = Vector(centerX, centerY, centerZ);
		Up = Vector(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector right = Up.cross(At - Eye).unit();
		Eye = Eye + right * d;
		At = At + right * d;
	}

	void moveY(float d) {
		Eye = Eye + Up.unit() * d;
		At = At + Up.unit() * d;
	}

	void moveZ(float d) {
		Vector view = (At - Eye).unit();
		Eye = Eye + view * d;
		At = At + view * d;
	}

	void rotateX(float a) {
		Vector view = (At - Eye).unit();
		Vector right = Up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + Up * sin(DEG2RAD(a));
		Up = view.cross(right);
		At = Eye + view;
	}

	void rotateY(float a) {
		Vector view = (At - Eye).unit();
		Vector right = Up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(Up);
		At = Eye + view;
	}

	/* Vector minionPosition(posHeroX, posHeroY, posHeroZ);
	Vector minionDirection = Vector(0, 1, 0);  // Assuming the minion is facing upward

	// Set the camera position
	Vector cameraPosition = minionPosition - (minionDirection * 5) + Vector(0, 2, 0);
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			  minionPosition.x, minionPosition.y, minionPosition.z,
			  0, 1, 0);*/


	void look() {



		gluLookAt(
			Eye.x, Eye.y, Eye.z,
			At.x, At.y, At.z,
			Up.x, Up.y, Up.z
		);
	}
};
Camera camera;




















void Timer(int value) {



	if (isWin || timeRemaining <= 0) {
		x = 1;
		InitLightSourceWholeGame();
		InitLightSourcePlayer();
		return;
	}
	if (x <= -3)
		x = 1;
	else
		x -= 0.3;
	if (timeRemaining > 0) {
		timeRemaining--;
	}
	else {
		x = 0;
	}



	glutTimerFunc(1000, Timer, 0);

}


void print(int x, int y, int z, char* string)
{
	int len, i;
	glDisable(GL_LIGHTING);
	//set the position of the text in the window using the x and y coordinates
	glColor3f(1, 1, 1);

	glRasterPos3f(x, y, z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
	glEnable(GL_LIGHTING);
}

void drawTime() {

	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(0.5, 0.5,0.5);

	//char text[] = "Time Remaining: "+ std::to_string(minutes); // Change this to your desired text
	std::string timeText;
	if(timeRemaining > 0 && !isWin)
	   timeText = "Time Remaining: " + std::to_string(timeRemaining) + " Score: " + std::to_string(score);
	else if(isWin) {
		eyeX = 200;  eyeY = 200;  eyeZ = 200; centerX = 205; centerY = 205; centerZ = 205;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;
		camera = Camera();
		timeText = "You Win :) Score: : "+ std::to_string(score);
	}
	else {
		eyeX = 200;  eyeY = 200;  eyeZ = 200; centerX = 205; centerY = 205; centerZ = 205;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;
		camera = Camera();
		timeText = "You Lose :(  " ;
	}
	// Convert the C++ string to a C-style string
	const char* charTimeText = timeText.c_str();
	print(centerX, centerY + 2, centerZ, const_cast<char*>(charTimeText));

	glPopMatrix();
	glEnable(GL_LIGHTING);
}
























void myKeyboard(unsigned char button, int x, int y)
{


	if (isWin || timeRemaining <= 0) {
		x = 1;
		return;
	}
	float a = 1.8;

	switch (button)
	{
	case 'n':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 't':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'x':
		state = 1 - state;
		break;
	case 27:
		exit(0);
		break;
	case 'c':
		cameraPerspective = 3;
		switch (rotateHero) {
		case 0: eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ - 5; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break; //up
		case -180:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ + 5; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//down
		case 90:eyeX = posHeroX - 3;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//left
		case -90:eyeX = posHeroX + 3;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//right

		}
		camera = Camera();

		break;
	case 'v':

		switch (rotateHero) {
		case 0:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ + 5;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break; //up
		case -180:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ - 5;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//down
		case 90:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX + 5; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//left
		case -90:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX - 5; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//right

		}
		camera = Camera();
		cameraPerspective = 1;break;
	default:
		break;
	}
	float d = 0.9;

	switch (button) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;


	case 'i':
		camera.rotateX(a);
		break;
	case'k':
		camera.rotateX(-a);
		break;
	case 'j':
		camera.rotateY(a);
		break;
	case 'l':
		camera.rotateY(-a);
		break;
	case 'b':isWin = true;break;
	case 'o':timeRemaining = 0;break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();














}







void Special(int key, int x, int y) {

	if (isWin || timeRemaining <= 0) {
		x = 1;
		return;
	}

	double a = 0.5;









	if (!up && key == GLUT_KEY_LEFT)
		key = GLUT_KEY_RIGHT;
	else if (!up && key == GLUT_KEY_RIGHT)
		key = GLUT_KEY_LEFT;
	switch (key) {

	case GLUT_KEY_UP:
		up = true;
		rotateHero = 0;

		posHeroZ += a;
		if (posHeroZ > 18.5) {
			posHeroZ -= a;
			sndPlaySound(TEXT("crash.wav"), SND_SYNC);

		}

		if (cameraPerspective == 3) {
			eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ - 5; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;
		}
		else {
			eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ + 5;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;

		}
		break;
	case GLUT_KEY_DOWN:
		up = false;
		rotateHero = -180;

		posHeroZ -= a;
		if (posHeroZ < -19) {
			posHeroZ += a;
			sndPlaySound(TEXT("crash.wav"), SND_SYNC);

		}
		if (cameraPerspective == 3) {
			eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ + 5; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;
		}
		else {
			eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ - 5;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;

		}

		break;
	case GLUT_KEY_LEFT:
		rotateHero = 90;
		posHeroX += a;


		if (posHeroZ != 18.5) {
			if (posHeroX == 11.5) {
				posHeroX -= a;
				sndPlaySound(TEXT("crash.wav"), SND_SYNC);


			}
		}


		if (posHeroZ != -19) {
			if (posHeroX == 3.5 && state == 0) {
				posHeroX -= a;
				sndPlaySound(TEXT("crash.wav"), SND_SYNC);

			}
		}

		if (posHeroZ != 18.5) {
			if (posHeroX == -6.5) {
				posHeroX -= a;
				sndPlaySound(TEXT("crash.wav"), SND_SYNC);


			}
		}


		if (posHeroX > 18.5) {
			posHeroX -= a;
			sndPlaySound(TEXT("crash.wav"), SND_SYNC);

		}
		if (cameraPerspective == 3) {
			eyeX = posHeroX - 3;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;
		}
		else {
			eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX + 5; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;

		}

		break;


	case GLUT_KEY_RIGHT:
		rotateHero = -90;
		posHeroX -= a;


		if (posHeroZ != 18.5) {
			if (posHeroX == 14.5) {
				posHeroX += a;
				sndPlaySound(TEXT("crash.wav"), SND_SYNC);
			}
		}
		if (posHeroZ != -19) {
			if (posHeroX == 6.5 && state == 0) {
				posHeroX += a;
				sndPlaySound(TEXT("crash.wav"), SND_SYNC);

			}
		}

		if (posHeroZ != 18.5) {
			if (posHeroX == -3.5) {
				posHeroX += a;
				sndPlaySound(TEXT("crash.wav"), SND_SYNC);

			}
		}





		if (posHeroX < -18.5)
		{
			posHeroX += a;
			sndPlaySound(TEXT("crash.wav"), SND_SYNC);


		}

		if (cameraPerspective == 3) {
			eyeX = posHeroX + 3;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;
		}
		else {
			eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX - 5; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;

		}

		break;

	}

	camera = Camera();


	if (posHeroX == 9 && posHeroZ == 0)
	{
		if (coinOne) {
			sndPlaySound(TEXT("collect.wav"), SND_SYNC);
			score++;
		}

		coinOne = false;
	}

	if (posHeroX == -9 && posHeroZ == 0)
	{
		if (coinThree) {
			sndPlaySound(TEXT("collect.wav"), SND_SYNC);
			score++;
		}
		coinThree = false;
	}
	if (posHeroX == 0 && posHeroZ == 0)
	{
		if (coinTwo) {
			sndPlaySound(TEXT("collect.wav"), SND_SYNC);
			score++;
		}
		coinTwo = false;
	}
	if (posHeroX == -18 && posHeroZ == -17.5 && state == 0)
	{
		state = 1;
		rotateHero = 0;
		coinOne = true;
		coinThree = true;
		coinTwo = true;
		posHeroX = 15;
		posHeroY = 1.35;
		posHeroZ = -15;

		sndPlaySound(TEXT("goal.wav"), SND_SYNC);
		 eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ - 3; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;

		//float eyeX = 20.0f, float eyeY = 5.0f, float eyeZ = 25.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f;

		camera = Camera();
	}



	if (posHeroX == -17.5 && posHeroZ == -18 && state == 1)
	{
		state = 1;
		rotateHero = 0;
		coinOne = true;
		coinThree = true;
		coinTwo = true;
		posHeroX = 15;
		posHeroY = 1.35;
		posHeroZ = -15;

		sndPlaySound(TEXT("goal.wav"), SND_SYNC);
		//float eyeX = 20.0f, float eyeY = 5.0f, float eyeZ = 25.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f

		//camera = Camera();
		isWin = true;
	}

	std::cout << "posHeroX: " << posHeroX << "\n";
	std::cout << "posHeroZ: " << posHeroZ << "\n";



	glutPostRedisplay();
}










































Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_wall;
Model_3DS model_hero;
Model_3DS model_dollar;
Model_3DS model_hole;
Model_3DS model_dollarS;
Model_3DS model_wall_two;
Model_3DS model_door;
GLTexture model_groundTwo;
// Textures
GLTexture tex_ground;
GLTexture tex_bear;


//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	//// Enable Lighting for this OpenGL Program
	//glEnable(GL_LIGHTING);

	//// Enable Light Source number 0
	//// OpengL has 8 light sources
	//glEnable(GL_LIGHT0);

	//// Define Light source 0 ambient light
	//GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	//// Define Light source 0 diffuse light
	//GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	//// Define Light source 0 Specular light
	//GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//// Finally, define light source 0 position in World Space
	//GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void InitLightSourcePlayer()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT1);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.5f, 0.5f,0.5f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	if (!(isWin || timeRemaining <= 0)) {
		GLfloat diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	}
	else {
		GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	}

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

	// Set the light position to be above the player
	GLfloat light_position[] = { posHeroX, posHeroY + 3.0f, posHeroZ, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);

	// Set the light direction (pointing downwards in this case)
	GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	// Set the spotlight cutoff angle (e.g., 45 degrees)
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80);

	// Set the spotlight exponent (e.g., 10.0 for a smooth falloff)
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f);
}

void InitLightSourceWholeGame()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT2);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.5f, 0.5f,0.5f, 0.0f };
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { x, x, x, 0.0f };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);

	// Set the light position to be above the player
	GLfloat light_position[] = { 0,3,0, 1.0f };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);

	// Set the light direction (pointing downwards in this case)
	GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

	// Set the spotlight cutoff angle (e.g., 45 degrees)
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 460);

	// Set the spotlight exponent (e.g., 10.0 for a smooth falloff)
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0f);
}


























/*
void InitLightSourcePlayer() {
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT7);

	// Define Light source 0 ambient light (white color)
	GLfloat ambient[] = { 0.1f,  0.1f,  0.1f,  0.1f };
	glLightfv(GL_LIGHT7, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light (white color)
	GLfloat diffuse[] = { 0.1f,  0.1f,  0.1f, 0.1f };
	glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light (white color)
	GLfloat specular[] = { 0.1f,  0.1f,  0.1f,  0.1f };
	glLightfv(GL_LIGHT7, GL_SPECULAR, specular);

	// Set the light position to be above the player
	GLfloat light_position[] = { 15,7, -15, 0.0 };
	glLightfv(GL_LIGHT7, GL_POSITION, light_position);

	// Set the light direction (pointing downwards in this case)
	GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, spot_direction);

	// Set the spotlight cutoff angle (e.g., 45 degrees)
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 2.0f);

	// Set the spotlight exponent (e.g., 10.0 for a smooth falloff)
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0f);
}


*//*
void setupLights(float currentTime) {
	// Calculate ambient, diffuse, and specular components based on time
	GLfloat ambientIntensity = 0.5f + 0.5f * sin(currentTime); // Varies between 0 and 1
	GLfloat diffuseIntensity = 0.5f + 0.5f * sin(currentTime); // Varies between 0 and 1
	GLfloat specularIntensity = 0.2f; // Constant specular intensity

	// Calculate sun color based on time
	GLfloat sunColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Start with white
	for (int i = 0; i < 3; ++i) {
		sunColor[i] *= ambientIntensity; // Adjust each color component based on ambient intensity
	}

	// Set material properties for the sun
	GLfloat shininess[] = { 50.0f }; // Specular shininess factor
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sunColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sunColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sunColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	// Set up directional light (representing the sun)
	GLfloat sunDirection[] = { 0.0f, 0.0f, 1.0f, 0.0f }; // Direction towards the sun
	GLfloat sunIntensity[] = { diffuseIntensity, diffuseIntensity, diffuseIntensity, 1.0f }; // Intensity of the sun
	glLightfv(GL_LIGHT0, GL_POSITION, sunDirection);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunIntensity);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunIntensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunColor);

	// Additional light source (moving light)
	GLfloat movingLightPosition[] = { 5.0f * sin(currentTime), 5.0f * cos(currentTime), 2.0f, 1.0f }; // Translate in a circular motion
	GLfloat movingLightColor[] = { 0.7f, 0.5f, 0.2f, 1.0f }; // Example color
	glLightfv(GL_LIGHT1, GL_POSITION, movingLightPosition);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, movingLightColor);

	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0 (sun)
	glEnable(GL_LIGHT0);

	// Enable Light Source number 1 (moving light)
	glEnable(GL_LIGHT1);
}

*/

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	//InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(1, 1, 1);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	if (state == 0)
		glBindTexture(GL_TEXTURE_2D, tex_bear.texture[0]);
	else if (state == 1)
		glBindTexture(GL_TEXTURE_2D, model_groundTwo.texture[0]);
	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}



void displayEnvOne() {


	setupCamera();






	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTime();
	InitLightSourcePlayer();
	InitLightSourceWholeGame();
	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	/*GLfloat lightIntensity2[] = { 1.0,1.0, 1.0, 0.0f };
	GLfloat lightPosition2[] = { posHeroX, posHeroY+2, posHeroZ, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightIntensity2);*/



	/*glPushMatrix();
	glColor3d(1, 1,1);
	glTranslated(posHeroX, posHeroY - 2, posHeroZ-2);
	GLUquadricObj* quadric = gluNewQuadric();
	gluCylinder(quadric, 2, 2, 4.0, 20, 20);
	gluDeleteQuadric(quadric);
	glPopMatrix();*/
	// Draw Ground

	RenderGround();

	// Draw Tree Model
	glPushMatrix();
	glTranslatef(10, 2, 0);
	glScalef(0.7, 0.7, 0.7);
	//model_tree.Draw();
	glPopMatrix();

	// Draw house Model
	glPushMatrix();
	glRotatef(90.f, 1, 0, 0);
	//model_house.Draw();
	glPopMatrix();

	//RenderBear();
	glPushMatrix();
	glScaled(1, 2, 1);
	drawWalls();
	glPopMatrix();
	//sky box
	glPushMatrix();




	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();


	glPushMatrix();
	glTranslated(posHeroX, posHeroY, posHeroZ);
	glRotated(rotateHero, 0, 1, 0);
	glScaled(0.05, 0.05, 0.05);
	model_hero.Draw();

	glPopMatrix();













	drawMaze();
	//modelDollar.Draw();
	drawCoins();
	if (state == 0) {
		glPushMatrix();
		glTranslated(-18, 0, -17.5);
		glScaled(0.007, 0.007, 0.007);
		model_hole.Draw();
		glPopMatrix();
	}
	else if (state == 1) {
		glPushMatrix();
		glTranslated(-18, 0, -17.5);
		//glScaled(0.007, 0.007, 0.007);
		glRotated(90, 0, 1, 0);
		model_door.Draw();
		glPopMatrix();
	}

	CheckGLError("After rendering");

	glutSwapBuffers();
}

void displayEnvTwo() {
	setupCamera();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTime();

	InitLightSourcePlayer();
	InitLightSourceWholeGame();

	/*GLfloat lightIntensity[] = {0.7, 0.7, 0.7, 1.0f};
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	*/
	// Draw Ground

	RenderGround();



	glPushMatrix();
	glScaled(1, 2, 1);
	drawWalls();
	glPopMatrix();



	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();


	glPushMatrix();
	glTranslated(posHeroX, posHeroY, posHeroZ);
	glRotated(rotateHero, 0, 1, 0);
	glScaled(0.05, 0.05, 0.05);
	model_hero.Draw();
	glPopMatrix();


	drawMaze();
	//modelDollar.Draw();
	drawCoins();

	glPushMatrix();
	glTranslated(-18, 0, -17.5);
	//glScaled(0.007, 0.007, 0.007);
	glRotated(90, 0, 1, 0);
	model_door.Draw();
	glPopMatrix();

	CheckGLError("After rendering");

	glutSwapBuffers();
}

void print2(int x, int y, int z, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y, z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)


{
	
	//if (timeRemaining == 1 ||  ) {
	//	

	//	 glPushMatrix();
	//	 glColor3f(1, 1, 1);

	//	 //char text[] = "Time Remaining: "+ std::to_string(minutes); // Change this to your desired text
	//	 std::string timeText = "Time Remaining: ";

	//	 // Convert the C++ string to a C-style string
	//	 const char* charTimeText = timeText.c_str();
	//	 print2(205, 205, 205, const_cast<char*>(charTimeText));

	//	 glPopMatrix();
	//}
	
		drawTime();

		if (!isWin || timeRemaining > 0)
			displayEnvOne();
	

}

//=======================================================================
// Keyboard Function
//=======================================================================





void drawMaze() {


	for (float i = 16.7;i > -19.3;i -= 1.3) {
		glPushMatrix();

		glRotated(90, 0, 1, 0);
		glTranslatef(i + 2, 1.35, 13);
		glScalef(0.01, 0.02, 0.01);
		if (state == 0)
			model_wall.Draw();
		else if (state == 1)
			model_wall_two.Draw();
		glPopMatrix();
	}


	if (state == 0) {
		for (float i = 16.7;i > -19.3;i -= 1.3) {
			glPushMatrix();

			glRotated(90, 0, 1, 0);
			glTranslatef(i - 0.2, 1.35, 5);
			glScalef(0.01, 0.02, 0.01);
			if (state == 0)
				model_wall.Draw();
			else if (state == 1)
				model_wall_two.Draw();
			glPopMatrix();
		}
	}


	for (float i = 16.7;i > -19.3;i -= 1.3) {
		glPushMatrix();

		glRotated(90, 0, 1, 0);
		glTranslatef(i + 2, 1.35, -5);
		glScalef(0.01, 0.02, 0.01);
		if (state == 0)
			model_wall.Draw();
		else if (state == 1)
			model_wall_two.Draw();
		glPopMatrix();
	}

}
void drawWalls() {

	// Draw front wall
	glPushMatrix();

	glTranslatef(19.3, 1.2, 19.8);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(18, 1.2, 19.8);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();



	for (float i = 16.7;i > -19.3;i -= 1.3) {
		glPushMatrix();

		glTranslatef(i, 1.2, 19.8);
		glScalef(0.01, 0.01, 0.01);
		if (state == 0)
			model_wall.Draw();
		else if (state == 1)
			model_wall_two.Draw();
		glPopMatrix();
	}


	glPushMatrix();

	glTranslatef(-19.3, 1.2, 19.8);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();



	//right wall


	glPushMatrix();

	glTranslatef(20, 1.2, 19.3);
	glRotated(90, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(20, 1.2, 18);
	glRotated(90, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();



	for (float i = 16.7;i > -19.3;i -= 1.3) {
		glPushMatrix();
		glTranslatef(20, 1.2, i);
		glRotated(90, 0, 1, 0);

		glScalef(0.01, 0.01, 0.01);
		if (state == 0)
			model_wall.Draw();
		else if (state == 1)
			model_wall_two.Draw();
		glPopMatrix();
	}


	glPushMatrix();

	glTranslatef(20, 1.2, -19.3);
	glRotated(90, 0, 1, 0);

	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();




	// Draw Back wall 


	glPushMatrix();

	glTranslatef(19.3, 1.2, -19.8);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(18, 1.2, -19.8);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();



	for (float i = 16.7;i > -19.3;i -= 1.3) {
		glPushMatrix();

		glTranslatef(i, 1.2, -19.8);
		glScalef(0.01, 0.01, 0.01);
		if (state == 0)
			model_wall.Draw();
		else if (state == 1)
			model_wall_two.Draw();
		glPopMatrix();
	}


	glPushMatrix();

	glTranslatef(-19.3, 1.2, -19.8);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();


	// Draw left wall

	glPushMatrix();

	glTranslatef(-20, 1.2, 19.3);
	glRotated(90, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-20, 1.2, 18);
	glRotated(90, 0, 1, 0);
	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();



	for (float i = 16.7;i > -19.3;i -= 1.3) {
		glPushMatrix();
		glTranslatef(-20, 1.2, i);
		glRotated(90, 0, 1, 0);

		glScalef(0.01, 0.01, 0.01);
		if (state == 0)
			model_wall.Draw();
		else if (state == 1)
			model_wall_two.Draw();
		glPopMatrix();
	}


	glPushMatrix();

	glTranslatef(-20, 1.2, -19.3);
	glRotated(90, 0, 1, 0);

	glScalef(0.01, 0.01, 0.01);
	if (state == 0)
		model_wall.Draw();
	else if (state == 1)
		model_wall_two.Draw();
	glPopMatrix();
























}











void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}
/*
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Calculate the position of the camera behind and above the minion
	Vector minionPosition(posHeroX, posHeroY, posHeroZ);
	Vector minionDirection = Vector(0, 1, 0);  // Assuming the minion is facing upward

	// Set the camera position
	Vector cameraPosition = minionPosition - (minionDirection * 5) + Vector(0, 2, 0);
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		minionPosition.x, minionPosition.y, minionPosition.z,
		0, 1, 0);

	// Optionally, you can look at a point slightly in front of the minion
	// gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
	//           minionPosition.x + minionDirection.x, minionPosition.y + minionDirection.y, minionPosition.z + minionDirection.z,
	//           0, 1, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

*/

























//=======================================================================
// Motion Function
//=======================================================================



void CheckGLError(const char* checkpoint)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL Error at checkpoint '" << checkpoint << "': " << gluErrorString(error) << std::endl;
	}
}

void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	/*y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}*/
	if (isWin || timeRemaining <= 0) {
		x = 1;
		return;
	}

	switch (button) {

	case GLUT_LEFT_BUTTON:
		cameraPerspective = 3;
		switch (rotateHero) {
		case 0: eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ - 5; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break; //up
		case -180:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ + 5; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//down
		case 90:eyeX = posHeroX - 3;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//left
		case -90:eyeX = posHeroX + 3;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//right

		}

		break;
	case GLUT_RIGHT_BUTTON:
		cameraPerspective = 1;
		switch (rotateHero) {
		case 0:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ + 5;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break; //up
		case -180:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX; centerY = posHeroY; centerZ = posHeroZ - 5;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//down
		case 90:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX + 5; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//left
		case -90:eyeX = posHeroX;  eyeY = posHeroY + 2;  eyeZ = posHeroZ; centerX = posHeroX - 5; centerY = posHeroY; centerZ = posHeroZ;  upX = 0.0f;  upY = 1.0f;  upZ = 0.0f;break;//right

		}
		break;




	}
	camera = Camera();


}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	//model_tree.Load("Models/tree/Tree1.3ds");

	model_wall.Load("Models/bahy/Bush Boxwood-Wall-R40427-00 formdecor N190218.3ds");

	model_wall_two.Load("Models/wallTwo/Bush Boxwood-Wall-R40427-00 formdecor N190218.3ds");

	model_hero.Load("Models/minion/Toy Minion N280319.3ds");
	// Loading texture files

	tex_ground.Load("Textures/ground.bmp");
	tex_bear.Load("Textures/ground.bmp");
	model_dollar.Load("Models/key/rc-coin.3DS");
	model_dollarS.Load("Models/boxe2/box.3ds");
	model_hole.Load("Models/hole/Manhole N290114.3DS");

	model_door.Load("Models/door/door.3ds");
	model_groundTwo.Load("Textures/sun.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);










}






void drawCoins() {





	if (coinTwo) {

		glPushMatrix();
		if (state == 0) {
			glTranslated(0, 1, 0);
			glScaled(0.02, 0.02, 0.02);
			glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

			model_dollar.Draw();
		}
		else if (state == 1) {
			glTranslated(0, 1, 0);
			glScaled(0.2, 0.2, 0.2);
			glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

			model_dollarS.Draw();
		}
		glPopMatrix();
	}

	if (coinOne)
	{

		glPushMatrix();
		if (state == 0) {
			glTranslated(9, 1, 0);
			glScaled(0.02, 0.02, 0.02);
			glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

			model_dollar.Draw();
		}
		else if (state == 1) {
			glTranslated(9, 1, 0);
			glScaled(0.2, 0.2, 0.2);
			glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

			model_dollarS.Draw();
		}
		glPopMatrix();
	}

	if (coinThree) {
		glPushMatrix();
		if (state == 0) {
			glTranslated(-9, 1, 0);
			glScaled(0.02, 0.02, 0.02);
			glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

			model_dollar.Draw();
		}
		else if (state == 1) {
			glTranslated(-9, 1, 0);
			glScaled(0.2, 0.2, 0.2);
			glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

			model_dollarS.Draw();
		}
		glPopMatrix();

	}
















}


void update(int value) {

	if (isWin || timeRemaining <= 0) {
		return;
	}
	// Update rotation angle
	rotationAngle += 2.0f;

	// Request a redraw
	glutPostRedisplay();

	// Set up the next update
	glutTimerFunc(16, update, 0);
}












//=======================================================================
// Main Function
//=======================================================================




void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutSpecialFunc(Special);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);
	glutTimerFunc(0, update, 0);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHT7);

	glShadeModel(GL_SMOOTH);
	glutTimerFunc(0, Timer, 0);
	glutMainLoop();
}
