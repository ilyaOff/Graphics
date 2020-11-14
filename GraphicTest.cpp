//#pragma comment (lib, "opengl32.lib")//DOES NOT WORK WITHOUT THIS

#include <iostream>

//#include <fre>
#include <windows.h>        // Заголовочный файл для Windows// ВАЖНО!!!!
#include <gl\gl.h>          // Заголовочный файл для OpenGL32 библиотеки
#include <glut.h>           // Заголовочный файл для GLu32 библиотеки
//#include <gl\glut.h>// у меня так не работает

//#include <gl\glaux.h>                     // Заголовочный файл для GLaux библиотеки
void reshape(int w, int h)
{
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
}
void display(void)
{
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
}
void idle(void) 
{
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
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

    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    glutMainLoop();
    return 0;
}


