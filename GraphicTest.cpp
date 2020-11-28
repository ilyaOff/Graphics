//#pragma comment (lib, "opengl32.lib")//DOES NOT WORK WITHOUT THIS
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>

#include<iostream>
#include<fstream>
#include<string>


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


GLuint vertexBuffer;
GLuint vertexArray;
GLuint program;
GLuint mvpLoc;
GLuint mvpLoc2;

float Fov = 45;
int W, H;
float xAngle = 0;
float yAngle = 60;

float xAngle2 = 0;
float yAngle2 = 0;

void init()
{
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	
	//переделать
	fstream a("CubeVertex.glsl");// у меня не хочет без переменной работать
	fstream b("CubeFrag.glsl");
	string vsh_src((istreambuf_iterator<char>(a)), istreambuf_iterator<char>());
	string fsh_src((istreambuf_iterator<char>(b)), istreambuf_iterator<char>());
	a.close();
	b.close();

	fstream c("PyramidVertex.glsl");
	fstream d("PyramidFrag.glsl");
	string vsh_src2((istreambuf_iterator<char>(c)), istreambuf_iterator<char>());
	string fsh_src2((istreambuf_iterator<char>(d)), istreambuf_iterator<char>());
	c.close();
	d.close();

	//Создание Объектов Shader And Program
	program = glCreateProgram();
	//GLenum vertex_shader = glCreateShader(GL_VERTEX_SHADER_ARB);
	//GLenum fragment_shader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
	GLenum vertex_shader2 = glCreateShader(GL_VERTEX_SHADER_ARB);
	GLenum fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER_ARB);
	// Загрузка шейдеров
	const char* src = vsh_src.c_str();
	/*
	glShaderSource(vertex_shader, 1, &src, NULL);
	src = fsh_src.c_str();
	glShaderSource(fragment_shader, 1, &src, NULL);
	*/
	src = vsh_src2.c_str();
	
	glShaderSource(vertex_shader2, 1, &src, NULL);
	src = fsh_src2.c_str();
	glShaderSource(fragment_shader2, 1, &src, NULL);
	// Компиляция шейдеров
	//glCompileShader(vertex_shader);
	//glCompileShader(fragment_shader);
	glCompileShader(vertex_shader2);
	glCompileShader(fragment_shader2);
	// Attach The Shader Objects To The Program Object
	//glAttachShader(program, vertex_shader);	
	glAttachShader(program, vertex_shader2);
	//glAttachShader(program, fragment_shader);
	glAttachShader(program, fragment_shader2);

	// Link The Program Object
	glLinkProgram(program);

	char log[10000];
	int log_len;
	
	glGetProgramInfoLog(program, /*sizeof(log) / sizeof(log[0]) - 1*/ 9999, &log_len, log);
	log[log_len] = 0;
	std::cout << "by "  <<sizeof(log) / sizeof(log[0]) - 1 << std::endl;
	std::cout << "Shader compile result: " << log << std::endl;

	mvpLoc = glGetUniformLocation(program, "mvp");
	mvpLoc2 = glGetUniformLocation(program, "mvp2");

	glUseProgramObjectARB(program);

	glGenBuffers(1, &vertexBuffer);//генерирует идентификатор буффера
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//указываем активный буфефр
	//загружаем данные в буффер
	//glBufferData(GL_ARRAY_BUFFER,  sizeof(cube_vertices) + sizeof(pyramid_vertices), cube_vertices, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_vertices) , sizeof(pyramid_vertices), pyramid_vertices);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

	//указываем в буффере данных, что байты образуют вершины
	glGenVertexArrays(1, &vertexArray);//создаем вершинный массив
	glBindVertexArray(vertexArray);
	//смещение = 0, размер 3 float, не нужно нормализовывать, 
	//лежат один за другим, лежат в текущем буфере(последний 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);//номер атрибута 

	
	//int attribLoc = glGetAttribLocation(program, "modelPos");
	//glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(attribLoc);

	glClearColor(0.0, 0.0, 0.0, 0.0);//задает цвет для заполнения буфера кадра
	//только ребра  - GL_LINE
	//полностью - GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

glm::mat4x4 proj;
void reshape(int w, int h)
{
	W = w; H = h;
	glViewport(0, 0, w, h); // изменить размера буфера до размера окна
	proj = glm::perspectiveFovRH(Fov, float(w), float(h), 0.1f, 5.0f);
}

void display(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);//заполняет буфер кадра заранее настроенным цветом
	
	glm::mat4x4 mvp = proj *
		glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) *
		glm::rotate(xAngle, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4x4 mvp2 = proj *
		glm::translate(glm::vec3(0.0f, -0.5f, -3.0f)) *
		glm::rotate(xAngle2, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(yAngle2, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);//определяем матрицу для шейдера
	glUniformMatrix4fv(mvpLoc2, 1, GL_FALSE, &mvp2[0][0]);//определяем матрицу для шейдера
	//glDrawElements(GL_TRIANGLES, sizeof(pyramid_indices) / sizeof(pyramid_indices[0]), GL_UNSIGNED_INT, pyramid_indices);
	//glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, cube_indices);

	glDrawElements(GL_TRIANGLES, 0 + sizeof(pyramid_indices) / sizeof(pyramid_indices[0]), GL_UNSIGNED_INT, pyramid_indices);
	
	
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
	yAngle += (direction*5);
	xAngle2 += (direction/10.0f );
	//xAngle += (direction * 5);
	glutPostRedisplay();
	//display();
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
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutCreateWindow("SIMPLE PROGRAM");
  
	glewInit();
	init();

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


