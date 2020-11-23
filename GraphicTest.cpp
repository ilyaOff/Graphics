//#pragma comment (lib, "opengl32.lib")//DOES NOT WORK WITHOUT THIS
#include<GL/glew.h>
#include<GL/freeglut.h>

#include<iostream>
#include<fstream>
#include<string>

//#include <fre>
//#include <windows.h>        // Заголовочный файл для Windows// ВАЖНО!!!!
//#include <gl\gl.h>          // Заголовочный файл для OpenGL32 библиотеки
//#include <glut.h>           // Заголовочный файл для GLu32 библиотеки
//#include <gl\glut.h>// у меня так не работает

//#include <gl\glaux.h>                     // Заголовочный файл для GLaux библиотеки




#pragma pack(1)
struct Vertex
{
	float x, y, z;
};
#pragma pack()

Vertex cube_vertices[] = {
	{ -0.5f, -0.5f, -0.5f },
	{ -0.5f, 0.5f, -0.5f },
	{ 0.5f, 0.5f, -0.5f },
	{ 0.5f, -0.5f, -0.5f },
	{ -0.5f, -0.5f, 0.5f },
	{ -0.5f, 0.5f, 0.5f },
	{ 0.5f, 0.5f, 0.5f },
	{ 0.5f, -0.5f, 0.5f }
};

GLuint cube_indices[] = {
	0,1,2,3,
	4,5,6,7,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,4,7
};

GLuint vertexBuffer;
GLuint vertexArray;
void reshape(int w, int h)
{
    //изменение размеров окна
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
}
void display(void)
{
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
}
void idle(void) 
{
    //для анимации
}

int main(int argc, char** argv)
{    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("VERY SIMPLE PROGRAM");// Подпись окна
    //ссылка на мои функции
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

	glewInit();

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    glutMainLoop();
    return 0;
}


