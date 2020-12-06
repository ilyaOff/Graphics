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
const int N = 2;//количество моделей

Model a;



GLuint vertexBuffer[N];//VBO
GLuint vertexArray[N];//VAO
GLuint EBO;
GLuint EBO2;
GLuint programs[N];
GLuint mvpLoc[N];
GLuint nmLoc[N];
GLuint mvLoc[N];
GLuint Light[N];
glm::vec3 Dir( 0.0f,1.0f,0.1f );
float Fov = 45;
int W, H;
float xAngle = 0;
float yAngle = 60;

float xAngle2 = 0;
float yAngle2 = 0;

void initShaider()
{
	
	Shader sh1("CubeVertex.glsl", "CubeFrag.glsl");
	Shader sh2("PyramidVertex.glsl", "PyramidFrag.glsl");
	char log[10000];
	int log_len;
	glGetProgramInfoLog(sh1.ID, sizeof(log) / sizeof(log[0]) - 1, &log_len, log);
	log[log_len] = 0;
	std::cout << "Shader" << sh1.ID << " compile result: " << log << std::endl;

	glGetProgramInfoLog(sh2.ID, sizeof(log) / sizeof(log[0]) - 1, &log_len, log);
	log[log_len] = 0;
	std::cout << "Shader" << sh2.ID << " compile result: " << log << std::endl;

	programs[0] = sh1.ID;
	
	mvpLoc[0] = glGetUniformLocation(programs[0], "mvp");
	mvLoc[0] = glGetUniformLocation(programs[0], "mv");
	nmLoc[0] = glGetUniformLocation(programs[0], "nm");
	Light[0] = glGetUniformLocation(programs[0], "dir");

	programs[1] = sh2.ID;
	
	mvpLoc[1] = glGetUniformLocation(programs[1], "mvp");
	mvLoc[1] = glGetUniformLocation(programs[1], "mv");
	nmLoc[1] = glGetUniformLocation(programs[1], "nm");
	Light[1] = glGetUniformLocation(programs[1], "LightDir");
	//sh2.use();
	
	/*
	string vsh_src[N];
	string fsh_src[N];
	//переделать
	
	fstream a("CubeVertex.glsl");// у меня не хочет без переменной работать
	fstream b("CubeFrag.glsl");
	vsh_src[0] = string((istreambuf_iterator<char>(a)), istreambuf_iterator<char>());
	fsh_src[0] = string((istreambuf_iterator<char>(b)), istreambuf_iterator<char>());
	a.close();
	b.close();

	fstream c("PyramidVertex.glsl");
	fstream d("PyramidFrag.glsl");
	vsh_src[1] = string((istreambuf_iterator<char>(c)), istreambuf_iterator<char>());
	fsh_src[1] = string((istreambuf_iterator<char>(d)), istreambuf_iterator<char>());
	c.close();
	d.close();
	const char* src;
	GLenum vertex_shader;
	GLenum fragment_shader;
	char log[10000];
	int log_len;
	for (int i = 0; i < N; i++)
	{		
		//Создание Объектов Shader And Program
		programs[i] = glCreateProgram();
		vertex_shader = glCreateShader(GL_VERTEX_SHADER_ARB);
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
		
		// Загрузка шейдеров
		src = vsh_src[i].c_str();
		glShaderSource(vertex_shader, 1, &src, NULL);

		src = fsh_src[i].c_str();
		glShaderSource(fragment_shader, 1, &src, NULL);

		// Компиляция шейдеров
		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);
		// Attach The Shader Objects To The Program Object
		glAttachShader(programs[i], vertex_shader);
		glAttachShader(programs[i], fragment_shader);
		
		// Link The Program Object
		glLinkProgram(programs[i]);
		
		// Удаление шейдеров
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	

		glGetProgramInfoLog(programs[i], sizeof(log) / sizeof(log[0]) - 1 , &log_len, log);
		log[log_len] = 0;
		std::cout << "Shader" << i << " compile result: " << log << std::endl;

		mvpLoc[i] = glGetUniformLocation(programs[0], "mvp");
		mvLoc[i] = glGetUniformLocation(programs[0], "mv");
		nmLoc[i] = glGetUniformLocation(programs[0], "nm");
	}
	*/
	//glGetProgramInfoLog(programs[0], /*sizeof(log) / sizeof(log[0]) - 1*/ 9999, &log_len, log);
	
	
	glUseProgramObjectARB(programs[0]);
	//glUseProgramObjectARB(programs[1]);
}
void init()
{
	glEnable(GL_DEPTH_TEST);//Включение теста по Z-буфферу
	glEnable(GL_CULL_FACE);//Отрисовка только лицевых граней
	glFrontFace(GL_CW);
	
	initShaider();
	Dir = glm::normalize(Dir);
	a.Init(cube_vertices, sizeof(cube_vertices),
		cube_indices, sizeof(cube_indices),
		"CubeVertex.glsl", "CubeFrag.glsl");
	a.Position = glm::vec3(0.0f, 0.5f, -2.5f);
	glGenBuffers(2, vertexBuffer);//генерирует идентификатор буффера
	glGenVertexArrays(2, vertexArray);//создаем вершинный массив
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &EBO2);
	//указываем в буффере данных, что байты образуют вершины
	glBindVertexArray(vertexArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);//указываем активный буфефр
	//загружаем данные в буффер
	glBufferData(GL_ARRAY_BUFFER,  sizeof(cube_vertices) , cube_vertices, GL_STATIC_DRAW);
		
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	
	//смещение = 0, размер 3 float, не нужно нормализовывать, 
	//лежат один за другим, лежат в текущем буфере(последний 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);//номер атрибута 

	//glBindVertexArray(0);
	
	glBindVertexArray(vertexArray[1]);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);//указываем активный буфефр
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);



	//смещение = 0, размер 3 float, не нужно нормализовывать, 
	//лежат один за другим, лежат в текущем буфере(последний 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);//номер атрибута 
	
	//int attribLoc = glGetAttribLocation(program, "modelPos");
	//glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(attribLoc);

	
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
	
	//КУб
	
	glUseProgram(programs[0]);
	glBindVertexArray(vertexArray[0]);
	glUniform3fv(Light[0],1, &Dir[0]);
	glm::mat4x4 mv = glm::translate(glm::vec3(-0.75f, 0.0f, -3.0f)) 
		//glm::rotate(xAngle, glm::vec3(1.0f, 0.0f, 0.0f)) *
		* glm::rotate(yAngle, glm::vec3(0.0f, 1.0f, 0.0f)) 
		* glm::rotate(xAngle, glm::vec3(1 , 0.0f, 0));
	glm::mat4x4 mvp = proj * mv;
	glm::mat3x3 nm = glm::transpose(glm::inverse(glm::mat3x3(mv)));

	glUniformMatrix4fv(mvpLoc[0], 1, GL_FALSE, &mvp[0][0]);//определяем матрицу для шейдера
	glUniformMatrix4fv(mvLoc[0], 1, GL_FALSE, &mv[0][0]);//определяем матрицу 	?
	glUniformMatrix4fv(nmLoc[0], 1, GL_FALSE, &nm[0][0]);//определяем матрицу 	?
	
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);//указываем активный буффер
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, 0/*cube_indices*/);
	
	//Пирамидка
	glUseProgram(programs[1]);
	glBindVertexArray(vertexArray[1]);
	glUniform3fv(Light[1], 1, &Dir[0]);
	mv = glm::translate(glm::vec3(0.25f, -0.5f, -3.0f))
		* glm::rotate(yAngle2, glm::vec3(0.0f, 1.0f, 0.0f)) 
		* glm::rotate(xAngle2, glm::vec3(1.0f, 0.0f, 0.0f));
		
	mvp = proj * mv;
	nm = glm::transpose(glm::inverse(glm::mat3x3(mv)));
	glUniformMatrix4fv(mvpLoc[1], 1, GL_FALSE, &mvp[0][0]);//определяем матрицу для шейдера2
	glUniformMatrix4fv(mvLoc[1], 1, GL_FALSE, &mv[0][0]);//определяем матрицу 	?
	glUniformMatrix4fv(nmLoc[1], 1, GL_FALSE, &nm[0][0]);//определяем матрицу 	?
	
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);//указываем активный буффер
	glDrawElements(GL_TRIANGLES, sizeof(pyramid_indices) / sizeof(pyramid_indices[0]), GL_UNSIGNED_INT, 0/*pyramid_indices*/);
	
	a.glDrawModel(proj,&Dir[0]);
	/*
	glUseProgram(a.program);
	glBindVertexArray(a.vertexArray);
	glUniform3fv(a.LightLoc, 1, &Dir[0]);
	mv = glm::translate(a.Position)
		//glm::rotate(xAngle, glm::vec3(1.0f, 0.0f, 0.0f)) *
		* glm::rotate(a.Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(a.Rotation.x, glm::vec3(1, 0.0f, 0));
	mvp = proj * mv;
	nm = glm::transpose(glm::inverse(glm::mat3x3(mv)));

	glUniformMatrix4fv(a.mvpLoc, 1, GL_FALSE, &mvp[0][0]);//определяем матрицу для шейдера
	glUniformMatrix4fv(a.mvLoc, 1, GL_FALSE, &mv[0][0]);//определяем матрицу 	?
	glUniformMatrix4fv(a.nmLoc, 1, GL_FALSE, &nm[0][0]);//определяем матрицу 	?
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, 0);
	*/
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
	a.Rotation.y +=  (direction / 5.0f);
	yAngle += (direction/5.0f);
	xAngle += (direction / 5.0f);
	yAngle2 += (direction/ 10.0f);

	glutPostRedisplay();
	//display();
	//cout << yAngle << ' ' << yAngle2 << endl;
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
	if (key == 27)//ESC
	{
		glutDestroyWindow(glutGetWindow());
		exit(1);
	}
	else if (key == ' ')
	{
		Fov = 45;
		//glutPostRedisplay();
		//display();
	}
	//std::cout << key << endl;
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
	//glutMouseFunc(MouseFunc);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keypress);
	glutMainLoop();
    return 0;
}


