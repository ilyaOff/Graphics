#pragma once


#pragma pack(1)
struct Vertex
{
	//float x, y, z;
	glm::vec3 position;
	glm::vec3 normal;
};
#pragma pack()

#include "Cube1.h"
#include "Pyramid.h"
#include "Floor.h"
#include "Sphereh.h"
#include "Shader.h"
#define PI 3.14159265359






class Model
{
	//Vertex* points;
public:
	GLuint vertexBuffer;//VBO
	GLuint vertexArray;//VAO
	GLuint indexArray;//EBO or IBO
	//void *indexArrays;//EBO or IBO
	//GLuint PointLoc;

	GLuint sizeIndex;

	glm::vec3 Position;	
	glm::vec3 Rotation;

	GLuint program;
	GLuint mvpLoc;	
	GLuint mLoc;
	GLuint nmLoc;
	GLuint VectorLoc;
	GLuint LightLoc;
	GLuint cameraPosLoc;
	GLuint cameraRotLoc;
	GLenum modeDraw;

	Model();
	/*Model(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices,
		const char* vertexPath, const char* fragmentPath);*/
	~Model();
	Model(const Model& a);//А есть ли смысл в копировании?
	void glDrawModel(glm::mat4* proj, glm::vec3* Light, glm::vec3* CameraPos = NULL, glm::vec3* CameraRot = NULL);
	
	
	void Init(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices, GLenum modeDraw,
		Shader sh, GLfloat* normals = NULL);

	void Use() { glUseProgram(program); }
	void UseMaterial(GLfloat* DiffuseMaterial);

private:

};

Model::Model()
{
	//points = NULL; 
	
	vertexBuffer = 0;
	vertexArray = 0;;
	indexArray = 0;;
	modeDraw = GL_TRIANGLES;
	sizeIndex = 0;

	Position = glm::vec3(0, 0, -1);
	Rotation = glm::vec3(0, 0, 0);

	program = 0;
	mvpLoc = 0;
	nmLoc = 0;
	mLoc = 0;
	LightLoc = 0;
	cameraPosLoc = 0;
	cameraRotLoc = 0;
}
/*Model::Model(GLfloat* vertices,  GLuint size_vertices,
	GLuint* ver_indices, GLuint size_indices,
	const char* vertexPath, const char* fragmentPath)
{

	
	Shader sh(vertexPath, fragmentPath);
	program = sh.ID;
	mvpLoc = glGetUniformLocation(program, "mvp");
	mvLoc = glGetUniformLocation(program, "mv");
	nmLoc = glGetUniformLocation(program, "nm");
	LightLoc = glGetUniformLocation(program, "dir");
	//-----------------------------------------------//
	Position = glm::vec3(0.0f, 0.0f, -3.0f);
	Rotation = glm::vec3(0, 0, 0);
	//создание массива вершин
	points = new Vertex[size_vertices];
	for (int i = 0; i < size_vertices; i++)
	{
		points[i].x = vertices[i*3];
		points[i].y = vertices[i * 3 + 1];
		points[i].z = vertices[i * 3 + 2];
	}

	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//генерирует идентификатор буффера
	glGenVertexArrays(1, &vertexArray);//создаем вершинный массив
	glGenBuffers(1, &indexArray);
	
	//указываем в буффере данных, что байты образуют вершины
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//указываем активный буфефр
	//загружаем данные в буффер
	glBufferData(GL_ARRAY_BUFFER, size_vertices, &points[0], GL_STATIC_DRAW);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &ver_indices[0], GL_STATIC_DRAW);

	//смещение = 0, размер 3 float, не нужно нормализовывать, 
	//лежат один за другим, лежат в текущем буфере(последний 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);//номер атрибута 
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, 0);

}*/
void Model::glDrawModel(glm::mat4* proj, glm::vec3* Light, glm::vec3* CameraPos, glm::vec3* CameraRot)
{
	glUseProgram(program);
	glBindVertexArray(vertexArray);
	
	//положение в пространстве
	//glm::quat a = glm::quat(2);
	
	glm::quat a = glm::quat(Rotation);
	
	glm::mat4x4 m = glm::translate(Position)
		* glm::mat4x4(a);
	//	* glm::rotate(Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
	//	* glm::rotate(Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
	//	* glm::rotate(Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4x4 cameraPos = glm::mat4x4(1.0);
	glm::mat4x4 cameraRot = glm::mat4x4(1.0);
	if (CameraPos != NULL)
	{
		cameraPos = glm::translate(-(*CameraPos));
	//	cameraPos = (*CameraPos);
	}
	if (CameraRot != NULL)
	{
	///	glm::squad()
		cameraRot = //glm::mat4x4(glm::quat(*CameraRot));
			
			glm::rotate((*CameraRot).z, glm::vec3(0.0f, 0.0f, 1.0f)) *
			 glm::rotate((*CameraRot).x, glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate((*CameraRot).y, glm::vec3(0.0f, 1.0f, 0.0f));
					
	}
	glm::mat4x4 mvp = (*proj) * cameraRot * cameraPos * m;
	glm::mat3x3 nm = glm::transpose(glm::inverse(glm::mat3x3(cameraRot * cameraPos * m)));
	//glm::mat3x3 nm = glm::mat3x3(mv);
	/*
	cout << "nm " << program << endl;
	glm::vec3 rrr = (nm * glm::vec3(0.5f, 0.5f, 0.5f));
	cout << rrr.x;
	cout << rrr.y;
	cout << rrr.z;
	cout << endl;
	rrr = glm::mat3x3(mv) * glm::vec3(0.5f, 0.5f, 0.5f);
	cout << "mv " << program << endl;
	cout << rrr.x;
	cout << rrr.y;
	cout << rrr.z;
	cout << endl << endl;
	
	cout << nm[0][0] << ' ' << nm[0][1] << ' ' << nm[0][2] << endl;
	cout << nm[1][0] << ' ' << nm[1][1] << ' ' << nm[1][2] << endl;
	cout << nm[2][0] << ' ' << nm[2][1] << ' ' << nm[2][2] << endl;
	cout << endl;
	cout << mv[0][0] << ' ' << mv[0][1] << ' ' << mv[0][2] << ' ' << mv[0][3] << endl;
	cout << mv[1][0] << ' ' << mv[1][1] << ' ' << mv[1][2] << ' ' << mv[1][3] << endl;
	cout << mv[2][0] << ' ' << mv[2][1] << ' ' << mv[2][2] << ' ' << mv[2][3] << endl;
	cout << mv[3][0] << ' ' << mv[3][1] << ' ' << mv[3][2] << ' ' << mv[3][3] << endl;
	cout << glm::determinant(nm) << ' ' << glm::determinant(mv) << endl;
	*/
	//для шейдеров
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);//определяем матрицу для шейдера Модельно-Видовая-Спроецированная
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, &m[0][0]);//определяем матрицу 	Модельная
	glUniformMatrix4fv(cameraPosLoc, 1, GL_FALSE, &cameraPos[0][0]);
	glUniformMatrix4fv(cameraRotLoc, 1, GL_FALSE, &cameraRot[0][0]);
	glUniform3fv(VectorLoc, 1, &(*CameraPos)[0]);

	glUniformMatrix3fv(nmLoc, 1, GL_FALSE, &nm[0][0]);//определяем матрицу 	?
	//свет	
	glUniform3fv(LightLoc, 1,&(*Light)[0]);

	glDrawElements(modeDraw, sizeIndex, GL_UNSIGNED_INT, 0);

}


void Model::Init(GLfloat* vertices, GLuint size_vertices,
	GLuint* ver_indices, GLuint size_indices, GLenum modeDraw,
	Shader sh, GLfloat* normals )
{
	
	program = sh.ID;

	mvpLoc = glGetUniformLocation(program, "mvp");
	mLoc = glGetUniformLocation(program, "m");
	nmLoc = glGetUniformLocation(program, "nm");
	cameraPosLoc = glGetUniformLocation(program, "CameraPosition");
	cameraRotLoc = glGetUniformLocation(program, "CameraRotation");
	VectorLoc = glGetUniformLocation(program, "Vec");


	//LightLoc = glGetUniformLocation(program, "LightPos");
	LightLoc = glGetUniformLocation(program, "LightDir");
	std::cout << "MOdel ID=" << program << std::endl;
	//-----------------------------------------------//
	Position = glm::vec3(0.0f, 0.0f, -4.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->modeDraw = modeDraw;
	//создание массива вершин
	
	Vertex* points = new Vertex[size_vertices];
	for (GLuint i = 0; i < size_vertices/3; i++)
	{
		/*
		points[i].x = vertices[i * 3];
		points[i].y = vertices[i * 3 + 1];
		points[i].z = vertices[i * 3 + 2];
		*/
		points[i].position = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
	}
	if (normals != NULL)
	{
		for (GLuint i = 0; i < size_vertices / 3; i++)
		{
			/*
			points[i].x = vertices[i * 3];
			points[i].y = vertices[i * 3 + 1];
			points[i].z = vertices[i * 3 + 2];
			*/
			points[i].normal  = glm::vec3(normals[i * 3], normals[i * 3+1], normals[i * 3+2]);


		}
	}
	else
	{
		//Не расчет нормалей
		for (GLuint i = 0; i < size_vertices / 3; i++)
		{
			points[i].normal = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		}
	}
	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//генерирует идентификатор буффера
	glGenVertexArrays(1, &vertexArray);//создаем вершинный массив
	glGenBuffers(1, &indexArray);

	//указываем в буффере данных, что байты образуют вершины
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//указываем активный буфефр
	//загружаем данные в буффер
	glBufferData(GL_ARRAY_BUFFER, size_vertices*sizeof(*points), points, GL_STATIC_DRAW);


	sizeIndex = size_indices/*/sizeof(ver_indices[0])*/;
	//indexArrays = ver_indices;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &ver_indices[0], GL_STATIC_DRAW);


	//смещение = 0, размер 3 float, не нужно нормализовывать, 
	//лежат один за другим, // неверно для больше 1 атрибута
	//лежат в текущем буфере(последний 0)
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)+0, /*(GLvoid*)*/0);
	//glEnableVertexAttribArray(0);
	int tmpLoc = glGetAttribLocation(program, "modelPos");
	//PointLoc = tmpLoc;
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(tmpLoc);//номер атрибута 
	tmpLoc = glGetAttribLocation(program, "modelNormal");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);
	
	glBindVertexArray(0);

}

void Model::UseMaterial(GLfloat* DiffuseMaterial)
{
	this->Use();
	GLuint Param = glGetUniformLocation(program, "MaterialDiffuse");
	glUniform4fv(Param, 1, DiffuseMaterial);
}


Model::~Model()
{
	//if (points != NULL)
	{
	//	delete[] points;
	}
}

//А есть ли смысл в копировании?
/*
Model::Model(const Model& a)
{
	
	vertexBuffer = a.vertexBuffer;
	vertexArray = a.vertexArray;
	indexArray = a.indexArray;

	Position = a.Position;
	Rotation = a.Rotation;
	program = a.program;
	mvpLoc = a.mvpLoc;
	nmLoc = a.nmLoc;
	mvLoc = a.mvLoc;
	LightLoc = a.LightLoc;

	if (points != NULL)
		delete[] points;
	int size = sizeof(a.points);
	points = new Vertex[size];
	for (int i = 0; i < size; i++)
	{
		points[i] = a.points[i];
	}
}*/

