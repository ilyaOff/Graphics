//#pragma comment (lib, "opengl32.lib")//DOES NOT WORK WITHOUT THIS
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

#include<iostream>
#include<fstream>
#include<string>
#define PI 3.14159265359

//#include<vector>
//#include<chrono>
//#include<numeric>

using namespace std;


//#include <fre>
//#include <windows.h>        // Заголовочный файл для Windows// ВАЖНО!!!!
//#include <gl\gl.h>          // Заголовочный файл для OpenGL32 библиотеки
//#include <glut.h>           // Заголовочный файл для GLu32 библиотеки
//#include <gl\glut.h>// у меня так не работает

//#include <gl\glaux.h>                     // Заголовочный файл для GLaux библиотеки

#include "Models.h"
#include "Shader.h"
const int N = 3;//количество моделей


Model MyModel[N];
glm::mat4x4 CameraV ;
glm::vec3 CameraPosition(1.0f, 0.0f, 0.0f);
glm::vec3 CameraRotation(0.0f, 0.0f, 0.0f);
const float step = 0.1f;
int X = 0, Y = 0;
bool start = false;

glm::vec3 Dir( 0.0f,1.0f,0.1f );
float Fov = 45;
int W, H;
float xAngle = 0;
float yAngle = 60;

float xAngle2 = 0;
float yAngle2 = 0;


void init()
{
	glEnable(GL_DEPTH_TEST);//Включение теста по Z-буфферу
	glEnable(GL_CULL_FACE);//Отрисовка только лицевых граней
	glFrontFace(GL_CW);
	
	
	Dir = glm::normalize(Dir);




	MyModel[0].Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		"CubeVertex.glsl", "CubeFrag.glsl");
	MyModel[1].Init(pyramid_vertices, sizeof(pyramid_vertices),
		pyramid_indices, sizeof(pyramid_indices), GL_TRIANGLES,
		"PyramidVertex.glsl", "PyramidFrag.glsl");

	MyModel[2].Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		"CubeVertex.glsl", "CubeFrag.glsl");

	MyModel[0].Position = glm::vec3(-0.75f, 0.0f, -3.0f);
	MyModel[1].Position = glm::vec3(0.25f, -0.5f, -3.0f);	
	MyModel[2].Position = glm::vec3(0.0f, 0.5f, -3.5f);

	MyModel[0].Rotation = glm::vec3(0.0f, 60.0f, 0.0f);
	MyModel[1].Rotation = glm::vec3(0.0f, -10.0f, 0.0f);
	MyModel[2].Rotation = glm::vec3(10.0f, 0.0f, 45.0f);
	
}

glm::mat4x4 proj;
void reshape(int w, int h)
{
	W = w; H = h;
	glViewport(0, 0, w, h); // изменить размера буфера до размера окна
	proj = glm::perspectiveFovRH(Fov, float(w), float(h), 0.1f, 5.0f);
	glutPostRedisplay();
}

void display(void)
{
	/*
		GL_COLOR_BUFFER_BIT-заполняет буфер кадра заранее настроенным цветом
		GL_DEPTH_BUFFER_BIT - очистка Z-буфера
	*/	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CameraV = glm::rotate(CameraRotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(CameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(CameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::translate(CameraPosition);

	for (int i = 0; i < N; i++)
	{
		MyModel[i].glDrawModel(&proj, &Dir[0], &CameraV);		
	}


	glFlush();//передает команды на исполнение
	glutSwapBuffers();
}
void idle(void) 
{
    //для анимации
}

void MouseWheelFunc(int wheel, int direction, int x, int y)
{
	//std::cout << wheel << ' ' << direction << ' '  << x <<' '  << y << endl;
	/*
	Fov += (direction )/10.0f;
	std::cout << Fov << endl;
	if (Fov < 10) Fov = 10;
	if (Fov >80) Fov = 80;
	proj = glm::perspectiveFovRH(Fov, float(W), float(H), 0.1f, 5.0f);
	
	*/
	MyModel[0].Rotation.y +=  (direction / 5.0f);
	MyModel[1].Rotation.y += (direction / 5.0f);
	MyModel[2].Rotation.y += (direction / 5.0f);
	
	glutPostRedisplay();
	//display();
	//cout << yAngle << ' ' << yAngle2 << endl;
}
void MouseMove(int x, int y)
{
	if (start)
	{
		//CameraRotation.x += (X - x)/100.0f;
		//X = x;
		CameraRotation.y += (X - x) / 100.0f;
		X = x;
		glutPostRedisplay();
	}
	else
	{
		start = true;
		X = x;
		Y = y;
	}
	cout << "MOVE" << endl;
	
}
/*
void MouseFunc(int wheel, int direction, int x, int y)
{
	glViewport(0, 0, x, y); // изменить размер окна
	std::cout << wheel << ' ' << direction << ' ' << x << ' ' << y << std::endl;
	display();
}*/
void keypress(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			glutDestroyWindow(glutGetWindow());
			exit(1); break;
		//case ' ': Fov = 45;break;
		case 'D':case 'd':CameraPosition.x -= step; glutPostRedisplay(); break;
		case'A':case 'a':CameraPosition.x += step; glutPostRedisplay(); break;

		case'W':case 'w':CameraPosition.z += step; glutPostRedisplay(); break;
		case'S':case 's':CameraPosition.z -= step; glutPostRedisplay(); break;
	default:
		break;
	}
	
	std::cout << key << ' ' <<(int)key << endl;
}
int main(int argc, char** argv)
{    
    glutInit(&argc, argv);
   
    glutCreateWindow("SIMPLE PROGRAM");
  
	glewInit();
	init();
	glClearColor(0.0, 0.0, 0.0, 0.0);//задает цвет для заполнения буфера кадра
	//только ребра  - GL_LINE
	//полностью - GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);


	//ссылка на мои функции
	glutMouseWheelFunc(MouseWheelFunc);
	glutPassiveMotionFunc(MouseMove);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keypress);
	glutMainLoop();
    return 0;
}


