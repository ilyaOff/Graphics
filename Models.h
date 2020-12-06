#pragma once


#pragma pack(1)
struct Vertex
{
	float x, y, z;
	//glm::vec3 position;
	//glm::vec3 normal;
};
#pragma pack()

#include "Cube1.h"
#include "Pyramid.h"


#include "Shader.h"


class Model
{
	Vertex* points;
public:
	GLuint vertexBuffer;//VBO
	GLuint vertexArray;//VAO
	GLuint indexArray;//EBO or IBO

	glm::vec3 Position;	
	glm::vec3 Rotation;

	GLuint program;
	GLuint mvpLoc;
	GLuint nmLoc;
	GLuint mvLoc;
	GLuint LightLoc;

	Model();
	Model(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices,
		const char* vertexPath, const char* fragmentPath);
	~Model();
	Model(const Model& a);//� ���� �� ����� � �����������?
	void glDrawModel(glm::mat4 proj, GLfloat* Dir);
	void Init(GLfloat* vertices, GLuint size_vertices,
				GLuint* ver_indices, GLuint size_indices,
				const char* vertexPath, const char* fragmentPath);

private:

};

Model::Model()
{
	points = NULL; 
	
	vertexBuffer = 0;
	vertexArray = 0;;
	indexArray = 0;;

	Position = glm::vec3(0, 0, -1);
	Rotation = glm::vec3(0, 0, 0);

	program = 0;
	mvpLoc = 0;
	nmLoc = 0;
	mvLoc = 0;
	LightLoc = 0;
}
Model::Model(GLfloat* vertices,  GLuint size_vertices,
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
	//�������� ������� ������
	points = new Vertex[size_vertices];
	for (int i = 0; i < size_vertices; i++)
	{
		points[i].x = vertices[i*3];
		points[i].y = vertices[i * 3 + 1];
		points[i].z = vertices[i * 3 + 2];
	}

	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//���������� ������������� �������
	glGenVertexArrays(1, &vertexArray);//������� ��������� ������
	glGenBuffers(1, &indexArray);
	
	//��������� � ������� ������, ��� ����� �������� �������
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//��������� �������� ������
	//��������� ������ � ������
	glBufferData(GL_ARRAY_BUFFER, size_vertices, &points[0], GL_STATIC_DRAW);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &ver_indices[0], GL_STATIC_DRAW);

	//�������� = 0, ������ 3 float, �� ����� ���������������, 
	//����� ���� �� ������, ����� � ������� ������(��������� 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);//����� �������� 
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, 0/*cube_indices*/);

}
void Model::glDrawModel(glm::mat4 proj,GLfloat* Dir)
{
	glUseProgram(program);
	glBindVertexArray(vertexArray);
	//std::cout << "DRAW" << endl;
	//��������� � ������������
	glm::mat4x4 mv = glm::translate(Position)
		* glm::rotate(Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4x4 mvp = proj * mv;
	glm::mat3x3 nm = glm::transpose(glm::inverse(glm::mat3x3(mv)));
	//��� ��������
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);//���������� ������� ��� ������� ��������-�������-���������������
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, &mv[0][0]);//���������� ������� 	��������-�������
	glUniformMatrix4fv(nmLoc, 1, GL_FALSE, &nm[0][0]);//���������� ������� 	?
	//����
	glUniform3fv(LightLoc, 1, &Dir[0]);

	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, 0);

}
void Model::Init(GLfloat* vertices, GLuint size_vertices,
	GLuint* ver_indices, GLuint size_indices,
	const char* vertexPath, const char* fragmentPath)
{
	Shader sh(vertexPath, fragmentPath);
	program = sh.ID;
	mvpLoc = glGetUniformLocation(program, "mvp");
	mvLoc = glGetUniformLocation(program, "mv");
	nmLoc = glGetUniformLocation(program, "nm");
	LightLoc = glGetUniformLocation(program, "dir");
	std::cout << "MOdel ID=" << program << std::endl;
	//-----------------------------------------------//
	Position = glm::vec3(0.0f, 0.0f, -4.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	//�������� ������� ������
	if (points != NULL)
		delete[] points;
	points = new Vertex[size_vertices];
	for (int i = 0; i < size_vertices; i++)
	{
		points[i].x = vertices[i * 3];
		points[i].y = vertices[i * 3 + 1];
		points[i].z = vertices[i * 3 + 2];
	}

	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//���������� ������������� �������
	glGenVertexArrays(1, &vertexArray);//������� ��������� ������
	glGenBuffers(1, &indexArray);

	//��������� � ������� ������, ��� ����� �������� �������
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//��������� �������� ������
	//��������� ������ � ������
	glBufferData(GL_ARRAY_BUFFER, size_vertices, &points[0], GL_STATIC_DRAW);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &ver_indices[0], GL_STATIC_DRAW);


	//�������� = 0, ������ 3 float, �� ����� ���������������, 
	//����� ���� �� ������, ����� � ������� ������(��������� 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);//����� �������� 

	//glBindVertexArray(0);

}
Model::~Model()
{
	if (points != NULL)
	{
		delete[] points;
	}
}

//� ���� �� ����� � �����������?
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
}