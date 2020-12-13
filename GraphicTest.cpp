//#pragma comment (lib, "opengl32.lib")//DOES NOT WORK WITHOUT THIS
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

#include <gtc/quaternion.hpp> 
#include <gtx/quaternion.hpp>

#include<FreeImage.h>


#include<iostream>
#include<fstream>
#include<string>
//#define PI 3.14159265359// в Models.h

using namespace std;


#include "Models.h"
#include "Shader.h"
const int N = 7;//количество моделей
Model MyModel[N];

float Fov = 45;
int W, H;

//glm::vec4 LightDirection(0.0f, 2.0f, 2.0f, 0.0f);//солнце
glm::vec3 LightDirection(0.0f, 2.0f, 2.0f);//солнце

glm::vec3 CameraPosition(1.0f, 0.0f, 0.0f);
glm::vec3 CameraRotation(0.0f, 0.0f, 0.0f);
const float step = 0.1f;

bool startMouseMove = false;
bool MouseCursor = false;
bool ModPolygon = true;
bool SwapCamers = false;

float Time;
float DeltaTime;
bool startTime = false;

void init()
{
	glEnable(GL_DEPTH_TEST);//Включение теста по Z-буфферу
	glEnable(GL_CULL_FACE);//Отрисовка только лицевых граней
	glFrontFace(GL_CW);

	

	glClearColor(0.0, 0.1, 0.1, 0.0);//задает цвет для заполнения буфера кадра
	//только ребра  - GL_LINE
	//полностью - GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		
	
	//куб - зеркало камеры
	MyModel[0].Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		Shader("PyramidVertex.glsl", "PyramidFrag.glsl"),
		cube_normal);
	MyModel[0].Position = glm::vec3(0.0f, 2.0f, 2.0f);
	MyModel[0].Rotation = glm::vec3(0.0f, 60.0f, 0.0f);

	//пирамидка
	MyModel[1].Init(pyramid_vertices, sizeof(pyramid_vertices),
		pyramid_indices, sizeof(pyramid_indices), GL_TRIANGLES,
		Shader("PyramidVertex.glsl", "PyramidFrag.glsl"));
	MyModel[1].Position = glm::vec3(1.75f, -0.5f, 0.0f);
	MyModel[1].Rotation = glm::vec3(0.0f, -10.0f, 0.0f);

	//куб1
	MyModel[2].Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		Shader("PyramidVertex.glsl", "PyramidFrag.glsl"));
	MyModel[2].Position = glm::vec3(0.0f, 0.5f, -3.5f);
	MyModel[2].Rotation = glm::vec3(10.0f, 0.0f, 0.0f);

	//пол
	MyModel[3].Init(floor_vertices, sizeof(floor_vertices),
		floor_indices, sizeof(floor_indices), GL_QUADS,
		Shader("PyramidVertex.glsl", "CubeFrag.glsl"),
		floor_normals);
	MyModel[3].Position = glm::vec3(0.0f, -0.5f, 0.0f);
	MyModel[3].Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	MyModel[3].UseMaterial(&(glm::vec4(0,0,0.5f,0.5f))[0]);
	
	//золотая сфера
	Make_sphere(1);
	MyModel[4].Init(sphere_vertices, sizeof(sphere_vertices),
		sphere_indexes, sizeof(sphere_indexes), GL_TRIANGLES,
		Shader("CookTorranceVertex.glsl", "CookTorranceFrag.glsl"),
		sphere_normals);

	MyModel[4].Position = glm::vec3(0.0f, 1.5f, -1.0f);

	
	//Куб с текстурой
	MyModel[5].InitText(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices), GL_QUADS,
		Shader("TextureFongVertex.glsl", "TextureFongFrag.glsl"),
		cube_normal,
		"Metall1.jpg",cube_text);

	MyModel[5].Position = glm::vec3(-1.0f, 0.5f, -2.5f);
	MyModel[5].Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	//плоскость
	MyModel[6].InitText(floor_vertices, sizeof(floor_vertices),
		floor_indices, sizeof(floor_indices), GL_QUADS,
		Shader("TextureFongVertex.glsl", "TextureFongFrag.glsl"),
		floor_normals,
		"NormalMap.png", floor_text_normal);
	
	MyModel[6].Position = glm::vec3(0.0f, -0.5f, -5.0f);
	MyModel[6].Rotation = glm::vec3(PI/2, 0.0f, 0.0f);
	
	
}

glm::mat4x4 proj;
void reshape(int w, int h)
{
	startMouseMove = false;
	W = w; H = h;
	glViewport(0, 0, w, h); // изменить размера буфера до размера окна
	proj = glm::perspectiveFovRH(Fov, float(w), float(h), 0.1f, 25.0f);
	glutPostRedisplay();
	startTime = false;
}

void display(void)
{
	/*
		GL_COLOR_BUFFER_BIT-заполняет буфер кадра заранее настроенным цветом
		GL_DEPTH_BUFFER_BIT - очистка Z-буфера
	*/	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glm::mat4x4 CameraRot= glm::rotate(CameraRotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
//		* glm::rotate(CameraRotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
	//	* glm::rotate(CameraRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//Свет-солнце поворочативается вместе с камерой
	//glm::vec3 a = glm::vec3(CameraV * Dir);	
	//glm::mat4x4 CameraPos =  glm::translate(CameraPosition);
	glEnable(GL_LIGHTING);//????????????????
	//-------------------------------------------------//
	/*
	glEnable(GL_STENCIL_TEST);//Буфер трафарета
	glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE);//Настройки теста буфера трафарета
	
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);//рисуем  зеркало
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//без цвета

	MyModel[3].glDrawModel(&proj, &(LightDirection),&(CameraPosition), &CameraRotation);//зеркало
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


	//glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);//Не меняем трафарет
	glStencilFunc(GL_EQUAL, 1, 0xFF);//рисуем только там, где зеркало


	
	glm::mat4x4 ReflectMat = glm::mat4x4(glm::quat(MyModel[3].Rotation))*glm::translate(-MyModel[3].Position);
		
	
	glm::vec3 CamZerPos;
	glm::vec3 CamZerRot;
	
	CamZerPos = glm::vec3(ReflectMat * glm::vec4(CameraPosition, 1));
	CamZerPos = glm::vec3(CamZerPos.x, -CamZerPos.y, CamZerPos.z);

	//CamZerRot = glm::vec3(-PI / 2 +CameraRotation.x, CameraRotation.y, CameraRotation.z);//вроде правильно
	CamZerRot = CameraRotation - MyModel[3].Rotation;
	CamZerRot = glm::vec3(-CamZerRot.x, CamZerRot.y, CamZerRot.z);
	
	glm::vec3 lightRef = glm::vec3(ReflectMat * glm::vec4(LightDirection, 1));

	
	std::cout << "CAM " <<
		CameraPosition.x << ' ' << CameraPosition.y << ' ' << CameraPosition.z << endl 
		<< CameraRotation.x << ' ' << CameraRotation.y << ' ' << CameraRotation.z << endl;
	std::cout << "Reflect " <<
		MyModel[3].Position.x << ' ' << MyModel[3].Position.y << ' ' << MyModel[3].Position.z << endl;
	std::cout << "RCAM " <<
		CamZerPos.x << ' ' << CamZerPos.y << ' ' << CamZerPos.z << endl << endl
		<< CamZerRot.x << ' ' << CamZerRot.y << ' ' << CamZerRot.z << endl;;


	for (int i = 0; i < N; i++)
	{		
		if(i != 3)
		MyModel[i].glDrawModel(&proj, &(lightRef),&CamZerPos, &CamZerRot);
	}

	glDisable(GL_STENCIL_TEST);
	*/
	//glStencilFunc(GL_GEQUAL, 1, 0xFF);//рисуем только там, где зеркало
	//-------------------------------------------------//
	for (int i = 0; i < N; i++)
	{
		//if(i != 3)
		MyModel[i].glDrawModel(&proj,&(LightDirection), &CameraPosition, &CameraRotation);
	}
	//MyModel[3].Position += glm::vec3(0, -0.001f, 0);
	//MyModel[3].glDrawModel(&proj, &(LightDirection), &CameraPosition, &CameraRotation); //Цвет панели
	//MyModel[3].Position += glm::vec3(0, 0.001f, 0);
	/*glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	MyModel[3].glDrawModel(&proj, &(LightDirection), &CameraPosition, &CameraRotation); //Цвет панели
	glDisable(GL_BLEND);*/

	/*
	if (SwapCamers)
	{
		CameraPosition = CamZerPos;
		CameraRotation = CamZerRot;
		SwapCamers = false;
	}
	*/

	glutSwapBuffers();
	glFlush();//передает команды на исполнение
	
}
void idle(void) 
{
    //для анимации
	if (!startTime)
	{
		DeltaTime = 0;
		startTime = true;
		Time = glutGet(GLUT_ELAPSED_TIME);
		return;
	}
	DeltaTime = Time;
	Time = glutGet(GLUT_ELAPSED_TIME);
	DeltaTime = 0.0001*(Time - DeltaTime);	
	MyModel[2].Rotation.y += DeltaTime;//для себя
	
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
	MyModel[2].Rotation.x += (direction / 5.0f);
	
	//MyModel[3].Rotation.x += (direction / 5.0f);
	MyModel[4].Position.y += (direction / 5.0f);
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
			//влево-вправо
			float delta = (float)(x+x - W) /(W+W);
			if (delta > 0.0003 || delta < -0.003)
				CameraRotation.y += 3.5f * delta;

			//Вверх-вних
			delta = (float)(y + y -H) / (H + H);
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
	//русские буквы не работают(
	switch (key)
	{
		case 27:
			glutDestroyWindow(glutGetWindow());
			exit(1); break;
		case 'U': case'u':
			SwapCamers = true; break;
		case 'Е':case 'е':case 'T': case't':
			ModPolygon = !ModPolygon;
			if (ModPolygon)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			break;		
		case 'Й':case 'й':case 'Q': case 'q': 
			MouseCursor = !MouseCursor;
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


		case 'R':case 'r'://case 'К':case 'к':
			CameraPosition += step * glm::vec3(0, 1, 0);
			glutPostRedisplay(); break;
		case'F':case 'f'://case'А':case 'а':
			CameraPosition -= step * glm::vec3(0, 1, 0);
			glutPostRedisplay(); break;

		case 'D':case 'd'://case 'В':case 'в':
			CameraPosition += step*glm::vec3(cos(CameraRotation.y),0, sin(CameraRotation.y));
			glutPostRedisplay(); break;
		case'A':case 'a'://case 'Ф':case 'ф':
			CameraPosition -= step * glm::vec3(cos(CameraRotation.y), 0, sin(CameraRotation.y));
			glutPostRedisplay(); break;
		case'W':case 'w'://case 'Ц':case 'ц':
			CameraPosition -= step * glm::vec3(sin(-CameraRotation.y), 0, cos(CameraRotation.y));
			glutPostRedisplay(); break;
		case'S':case 's'://case 'Ы':case 'ы':
			CameraPosition += step * glm::vec3(sin(-CameraRotation.y), 0, cos(CameraRotation.y));
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

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH|GLUT_STENCIL);


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


