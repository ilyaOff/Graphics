//#pragma comment (lib, "opengl32.lib")//DOES NOT WORK WITHOUT THIS
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

#include<iostream>
#include<fstream>
#include<string>
//#define PI 3.14159265359// в Models.h

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
const int N = 5;//количество моделей
Model MyModel[N];

float Fov = 45;
int W, H;


glm::vec3 CameraPosition(1.0f, 0.0f, 0.0f);
glm::vec3 CameraRotation(0.0f, 0.0f, 0.0f);
const float step = 0.1f;

bool startMouseMove = false;
bool MouseCursor = false;
bool ModPolygon = true;


void init()
{
	glEnable(GL_DEPTH_TEST);//Включение теста по Z-буфферу
	glEnable(GL_CULL_FACE);//Отрисовка только лицевых граней
	glFrontFace(GL_CW);
	glClearColor(0.0, 0.1, 0.1, 0.0);//задает цвет для заполнения буфера кадра
	//только ребра  - GL_LINE
	//полностью - GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	


	MyModel[0].Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		Shader("CubeVertex.glsl", "CubeFrag.glsl"),
		cube_normal);

	MyModel[1].Init(pyramid_vertices, sizeof(pyramid_vertices),
		pyramid_indices, sizeof(pyramid_indices), GL_TRIANGLES,
		Shader("CubeVertex.glsl", "CubeFrag.glsl"));

	MyModel[2].Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		Shader("CubeVertex.glsl", "CubeFrag.glsl"));

	MyModel[3].Init(floor_vertices, sizeof(floor_vertices),
		floor_indices, sizeof(floor_indices), GL_QUADS,
		Shader("CubeVertex.glsl", "CubeFrag.glsl"),
		floor_normals);

	Make_sphere(1);
	MyModel[4].Init(sphere_vertices, sizeof(sphere_vertices),
		sphere_indexes, sizeof(sphere_indexes), GL_TRIANGLES,
		Shader("CubeVertex.glsl", "CubeFrag.glsl"),
		sphere_normals);

	MyModel[0].Position = glm::vec3(-1.75f, 0.0f, -0.0f);
	MyModel[1].Position = glm::vec3(1.75f, -0.5f, -0.0f);
	MyModel[2].Position = glm::vec3(0.0f, 0.5f, -3.5f);
	MyModel[3].Position = glm::vec3(0.0f, -540.5f, 0.0f);
	MyModel[4].Position = glm::vec3(0.0f, 1.5f, -1.0f);

	MyModel[0].Rotation = glm::vec3(0.0f, 60.0f, 0.0f);
	MyModel[1].Rotation = glm::vec3(0.0f, -10.0f, 0.0f);
	MyModel[2].Rotation = glm::vec3(10.0f, 0.0f, 45.0f);
	MyModel[3].Rotation = glm::vec3(PI/2, 0.0f, 0.0f);
}

glm::mat4x4 proj;
void reshape(int w, int h)
{
	startMouseMove = false;
	W = w; H = h;
	glViewport(0, 0, w, h); // изменить размера буфера до размера окна
	proj = glm::perspectiveFovRH(Fov, float(w), float(h), 0.1f, 25.0f);
	glutPostRedisplay();
}

void display(void)
{
	/*
		GL_COLOR_BUFFER_BIT-заполняет буфер кадра заранее настроенным цветом
		GL_DEPTH_BUFFER_BIT - очистка Z-буфера
	*/	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4x4 CameraRot= glm::rotate(CameraRotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(CameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(CameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//Свет-солнце поворочативается вместе с камерой
	//glm::vec3 a = glm::vec3(CameraV * Dir);
	
	glm::mat4x4 CameraPos =  glm::translate(CameraPosition);
	
	
	for (int i = 0; i < N; i++)
	{
		//MyModel[i].glDrawModel(&proj, &Dir[0], &CameraV);
		//MyModel[i].glDrawModel(&proj,  &CameraPos, &CameraRot);
		MyModel[i].glDrawModel(&proj, &CameraPosition, &CameraRotation);
	}


	glFlush();//передает команды на исполнение
	//glutSwapBuffers();
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
	MyModel[3].Rotation.x += (direction / 5.0f);
	MyModel[4].Rotation.y += (direction / 5.0f);
	glutPostRedisplay();
	//display();
	//cout << yAngle << ' ' << yAngle2 << endl;
}
void MouseMove(int x, int y)
{
	if (startMouseMove)
	{
		//CameraRotation.x += (X - x)/100.0f;
		//X = x;
		if (!MouseCursor)
		{
			float delta = ((float)x / W - 0.5f);
			if (delta > 0.00006 || delta < -0.0006)
				CameraRotation.y += 3.5f * delta;
			delta = (float)y / H - 0.5f;
			if(delta > 0.0007 || delta < -0.0007)
				CameraRotation.x += 3.5f * (delta);
			//if (CameraRotation.x > 15)
			//	CameraRotation.x = 15;
			//if (CameraRotation.x < -15)
			//	CameraRotation.x = -15;
			glutWarpPointer(W / 2, H / 2);
			glutPostRedisplay();
			//cout << ((float)y / H - 0.5f) << endl;
		}
	}
	else
	{
		startMouseMove = true;
	}

	
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
		case 'T': case't':ModPolygon = !ModPolygon;
			if (ModPolygon)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			break;		
		case 'Q': case 'q': MouseCursor = !MouseCursor;
			if (MouseCursor)
			{
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);//включить курсор
			}
			else
			{
				glutSetCursor(GLUT_CURSOR_NONE);//убрать курсор
				glutWarpPointer(W / 2, H / 2);
			}
			break;
		case ' ': CameraPosition = glm::vec3(0,0,0);
					CameraRotation = glm::vec3(0, 0, 0);
					startMouseMove = false; glutPostRedisplay(); break;
		case 'R':case 'r':
			CameraPosition -= step * glm::vec3(0, 1, 0);
			glutPostRedisplay(); break;
		case'F':case 'f':
			CameraPosition += step * glm::vec3(0, 1, 0);
			glutPostRedisplay(); break;


		case 'D':case 'd':
			CameraPosition -= step*glm::vec3(cos(CameraRotation.y),0, sin(CameraRotation.y));
			glutPostRedisplay(); break;
		case'A':case 'a':
			CameraPosition += step * glm::vec3(cos(CameraRotation.y), 0, sin(CameraRotation.y));
			glutPostRedisplay(); break;
		case'W':case 'w':
			CameraPosition += step * glm::vec3(sin(-CameraRotation.y), 0, cos(CameraRotation.y));
			glutPostRedisplay(); break;
		case'S':case 's':
			CameraPosition -= step * glm::vec3(sin(-CameraRotation.y), 0, cos(CameraRotation.y));
			glutPostRedisplay(); break;
	default:
		break;
	}
	/*
	if (right != 0 || forward != 0)
	{
		float siny = sin(CameraRotation.y);
		float cosy = cos(CameraRotation.y);
		CameraPosition += step * glm::normalize(
			forward * glm::vec3(siny, 0, cosy)
			- right * glm::vec3(cosy, 0, siny));
		glutPostRedisplay();
	}*/
	//std::cout << key << ' ' <<(int)key << endl;
}
int main(int argc, char** argv)
{    
    glutInit(&argc, argv);
   
    glutCreateWindow("SIMPLE PROGRAM");
  
	glewInit();
	init();

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);


	//ссылка на мои функции
	glutMouseWheelFunc(MouseWheelFunc);
	glutPassiveMotionFunc(MouseMove);
	glutSetCursor(GLUT_CURSOR_NONE);//убрать курсор
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keypress);
	glutMainLoop();
    return 0;
}


