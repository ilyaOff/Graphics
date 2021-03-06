#pragma once


#pragma pack(1)
struct Vertex
{
	//float x, y, z;
	glm::vec3 position;
	glm::vec3 normal;
};
#pragma pack()

#pragma pack(1)
struct VertexText
{
	//float x, y, z;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};
#pragma pack()

#include "Cube1.h"
#include "Pyramid.h"
#include "Floor.h"
#include "Sphereh.h"
#include "Shader.h"
#include "Cat.h"
//#include "MakePoints.h"

#define PI 3.14159265359

const int MaxText = 2;

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
	GLenum modeDraw;

	GLuint mvpLoc;	
	GLuint mLoc;
	GLuint nmLoc;	
	
	GLuint CameraVLoc;
	GLuint cameraRotLoc;
	
	GLuint LightLoc;

	GLuint textureID[MaxText];
	GLuint textLoc[MaxText];
	GLuint textureCount;
	Model();
	~Model();
	/*Model(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices,
		const char* vertexPath, const char* fragmentPath);
	~Model();
	Model(const Model& a);
	*/
	void glDrawModel(glm::mat4* proj, glm::vec3* Light,
		glm::mat4* CameraV, glm::mat4* CameraRot = NULL);
	
	void Init(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices, GLenum modeDraw,
		Shader sh, GLfloat* normals = NULL);

	void Init(VertexText* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices,
		GLenum modeDraw, Shader sh);

	void InitText(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices,
		GLenum modeDraw, Shader sh,
		GLfloat* normals, GLfloat* text_coord);

	/*void InitTextObj(GLfloat* vertices, GLuint size_vertices,
		GLuint* ver_indices, GLuint size_indices,
		GLenum modeDraw, Shader sh, const char* texture,
		GLfloat* normals, GLuint normal_size,		
		GLfloat* text_coord, GLuint text_size );
		*/
	void Use() { glUseProgram(program); }
	void UseMaterial(GLfloat* DiffuseMaterial);
	void loatText(const char* texture);
private:

};

void Model::loatText(const char* texture)
{
	if (textureCount >= MaxText)
	{
		std::cout << "ERROR - MAX TEXTURE LOAD" << std::endl;
		return;
	}
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(texture, 0);
	FIBITMAP* image = FreeImage_Load(format, texture);
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);

	GLubyte* bits = (GLubyte*)FreeImage_GetBits(image);

	//��������� the OpenGL ����������� ������� 
	glGenTextures(1, &(textureID[textureCount]));
	//std::cout << program << 'S' << textureID[textureCount] << endl;
	glBindTexture(GL_TEXTURE_2D, textureID[textureCount]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(image);

	textureCount++;
	//std::cout << program << ' ' << textureCount << endl;
}
Model::Model()
{
	//points = NULL; 	
	vertexBuffer = 0;
	vertexArray = 0;;
	indexArray = 0;
	modeDraw = GL_TRIANGLES;
	sizeIndex = 0;

	Position = glm::vec3(0, 0, -1);
	Rotation = glm::vec3(0, 0, 0);

	program = 0;
	mvpLoc = 0;
	nmLoc = 0;
	mLoc = 0;
	LightLoc = 0;	
	cameraRotLoc = 0;

	textureCount = 0;
	textLoc[0] = textLoc[1] = 0;
	//textureID = 0;
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
	glDrawElements(GL_QUADS, sizeof(cube_indices) / sizeof(cube_indices[0]), GL_UNSIGNED_INT, 0);

}*/
void Model::glDrawModel(glm::mat4* proj, glm::vec3* Light,
			glm::mat4* CameraV, glm::mat4* CameraRot)
			//glm::vec3* CameraPos, glm::vec3* CameraRot, glm::vec3* CameraScale)
{
	glUseProgram(program);
	glBindVertexArray(vertexArray);
	
	//��������� � ������������
	
	glm::quat a = glm::quat(Rotation);
	
	glm::mat4x4 m = glm::translate(Position)
		* glm::mat4x4(a);
	//	* glm::rotate(Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
	//	* glm::rotate(Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
	//	* glm::rotate(Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	/*
	glm::mat4x4 cameraPos = glm::mat4x4(1.0);
	
	
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

		if (CameraScale != NULL)
		{
			cameraRot = glm::scale((*CameraScale)) * cameraRot;
		}
					
	}*/
	
	glm::mat4x4 mvp = (*proj) * *CameraV * m;
	glm::mat3x3 nm = glm::transpose(glm::inverse(glm::mat3x3(*CameraV * m)));
	
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
	//��� ��������
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);//���������� �������  ��������-�������-���������������
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, &m[0][0]);//���������� ������� 	���������
	glUniformMatrix3fv(nmLoc, 1, GL_FALSE, &nm[0][0]);//���������� ������� �������������� ��������

	glUniformMatrix4fv(CameraVLoc, 1, GL_FALSE, &(*CameraV)[0][0]);
	if (CameraRot != NULL)
	{
		glUniformMatrix4fv(cameraRotLoc, 1, GL_FALSE, &(*CameraRot)[0][0]);
	}	

	//����	
	glUniform3fv(LightLoc, 1,&(*Light)[0]);

	for (int i = 0; i < textureCount; i++)
	{
		//std::cout << program << ' ' << textLoc[i] << ' '<<(GLuint)(-1) << std::endl;
		glActiveTexture(GL_TEXTURE0+i);		
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		glUniform1i(textLoc[i], i);
	}
	

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
	CameraVLoc = glGetUniformLocation(program, "CameraV");
	cameraRotLoc = glGetUniformLocation(program, "CameraRotation");	
	LightLoc = glGetUniformLocation(program, "LightPos");
	//std::cout << "Model ID=" << program << std::endl;
	//-----------------------------------------------//
	Position = glm::vec3(0.0f, 0.0f, -4.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->modeDraw = modeDraw;
	//�������� ������� ������
	
	Vertex* points = NULL;
	points = new Vertex[size_vertices];//�� �������� �� ������, ������-��
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
		//�� ������ ��������
		for (GLuint i = 0; i < size_vertices / 3; i++)
		{
			points[i].normal = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		}
	}
	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//���������� ������������� �������
	glGenVertexArrays(1, &vertexArray);//������� ��������� ������
	glGenBuffers(1, &indexArray);

	//��������� � ������� ������, ��� ����� �������� �������
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//��������� �������� ������
	//��������� ������ � ������
	glBufferData(GL_ARRAY_BUFFER, size_vertices*sizeof(*points), points, GL_STATIC_DRAW);


	sizeIndex = size_indices/*/sizeof(ver_indices[0])*/;
	//indexArrays = ver_indices;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &ver_indices[0], GL_STATIC_DRAW);


	//�������� = 0, ������ 3 float, �� ����� ���������������, 
	//����� ���� �� ������, // ������� ��� ������ 1 ��������
	//����� � ������� ������(��������� 0)
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)+0, /*(GLvoid*)*/0);
	//glEnableVertexAttribArray(0);
	int tmpLoc = glGetAttribLocation(program, "modelPos");
	//PointLoc = tmpLoc;
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(tmpLoc);//����� �������� 
	tmpLoc = glGetAttribLocation(program, "modelNormal");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);
	
	glBindVertexArray(0);
	delete[] points;
}

// ������� �������
void Model::Init(VertexText* points, GLuint size_vertices,
	GLuint* ver_indices, GLuint size_indices,
	GLenum modeDraw, Shader sh)
{
	Position = glm::vec3(0.0f, 0.0f, -4.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->modeDraw = modeDraw;

	program = sh.ID;

	mvpLoc = glGetUniformLocation(program, "mvp");
	mLoc = glGetUniformLocation(program, "m");
	nmLoc = glGetUniformLocation(program, "nm");
	CameraVLoc = glGetUniformLocation(program, "CameraV");
	cameraRotLoc = glGetUniformLocation(program, "CameraRotation");
	LightLoc = glGetUniformLocation(program, "LightPos");

	//std::cout << "MOdelText ID=" << program << std::endl;
	//-----------------------------------------------//
	// �������� ��������
	//� ������ �������
	//�������� ������
	// �� �� � ������ �������
	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//���������� ������������� �������
	glGenVertexArrays(1, &vertexArray);//������� ��������� ������
	glGenBuffers(1, &indexArray);

	//��������� � ������� ������, ��� ����� �������� �������
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//��������� �������� ������
	//��������� ������ � ������
	// *3, ��� ��� ������ ������??????
	glBufferData(GL_ARRAY_BUFFER,size_vertices * sizeof(VertexText), points, GL_STATIC_DRAW);


	sizeIndex = size_indices;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  size_indices, &ver_indices[0], GL_STATIC_DRAW);


	//�������� = 0, ������ 3 float, �� ����� ���������������, 
	//����� ���� �� ������, // ������� ��� ������ 1 ��������
	//����� � ������� ������(��������� 0)
	int tmpLoc = glGetAttribLocation(program, "modelPos");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), 0);
	glEnableVertexAttribArray(tmpLoc);//����� �������� 

	tmpLoc = glGetAttribLocation(program, "modelNormal");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);

	tmpLoc = glGetAttribLocation(program, "modelTexCoor");
	glVertexAttribPointer(tmpLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);

	glBindVertexArray(0);

	//delete[] points;

}

void Model::InitText(GLfloat* vertices, GLuint size_vertices,
	GLuint* ver_indices, GLuint size_indices, 
	GLenum modeDraw, Shader sh,
	GLfloat* normals,  GLfloat* text_coord)
{
	Position = glm::vec3(0.0f, 0.0f, -4.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->modeDraw = modeDraw;

	program = sh.ID;

	mvpLoc = glGetUniformLocation(program, "mvp");
	mLoc = glGetUniformLocation(program, "m");
	nmLoc = glGetUniformLocation(program, "nm");
	CameraVLoc = glGetUniformLocation(program, "CameraV");
	cameraRotLoc = glGetUniformLocation(program, "CameraRotation");
	LightLoc = glGetUniformLocation(program, "LightPos");

	//std::cout << "MOdelText ID=" << program << std::endl;
	//-----------------------------------------------//
	// �������� ��������
	//� ������ �������
	//-----------------------------------------------//
	
	//�������� ������� ������

	VertexText* points = new VertexText[size_vertices];
	for (GLuint i = 0; i < size_vertices / 3; i++)
	{
		points[i].position = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
	}
	//if (normals != NULL)
	{
		for (GLuint i = 0; 3*i < size_vertices ; i++)
		{
			points[i].normal = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		}
	}
	//if (text_coord != NULL)
	{
		for (GLuint i = 0; 3 * i < size_vertices; i++)
		{
			points[i].texcoords = glm::vec2(text_coord[i * 2], text_coord[i * 2 + 1]);
		}
	}
	
	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//���������� ������������� �������
	glGenVertexArrays(1, &vertexArray);//������� ��������� ������
	glGenBuffers(1, &indexArray);

	//��������� � ������� ������, ��� ����� �������� �������
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//��������� �������� ������
	//��������� ������ � ������
	glBufferData(GL_ARRAY_BUFFER, size_vertices * sizeof(VertexText), points, GL_STATIC_DRAW);


	sizeIndex = size_indices;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &ver_indices[0], GL_STATIC_DRAW);


	//�������� = 0, ������ 3 float, �� ����� ���������������, 
	//����� ���� �� ������, // ������� ��� ������ 1 ��������
	//����� � ������� ������(��������� 0)
	int tmpLoc = glGetAttribLocation(program, "modelPos");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), 0);
	glEnableVertexAttribArray(tmpLoc);//����� �������� 

	tmpLoc = glGetAttribLocation(program, "modelNormal");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);

	tmpLoc = glGetAttribLocation(program, "modelTexCoor");
	glVertexAttribPointer(tmpLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);

	glBindVertexArray(0);

	delete[] points;

}

void Model::UseMaterial(GLfloat* DiffuseMaterial)
{
	this->Use();
	GLuint Param = glGetUniformLocation(program, "MaterialDiffuse");
	glUniform4fv(Param, 1, DiffuseMaterial);
}


Model::~Model()
{	
	if (vertexArray != 0)
	{
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &indexArray);
	}
}



/*
void Model::InitTextObj(GLfloat* vertices, GLuint size_vertices,
	GLuint* ver_indices, GLuint size_indices,
	GLenum modeDraw, Shader sh, const char* texture,
	GLfloat* normals, GLuint normal_size,
	GLfloat* text_coord, GLuint text_size)
{
	Position = glm::vec3(0.0f, 0.0f, -4.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->modeDraw = modeDraw;

	program = sh.ID;

	mvpLoc = glGetUniformLocation(program, "mvp");
	mLoc = glGetUniformLocation(program, "m");
	nmLoc = glGetUniformLocation(program, "nm");
	cameraPosLoc = glGetUniformLocation(program, "CameraPosition");
	cameraRotLoc = glGetUniformLocation(program, "CameraRotation");
	VectorLoc = glGetUniformLocation(program, "Vec");
	LightLoc = glGetUniformLocation(program, "LightDir");

	std::cout << "MOdelText ID=" << program << std::endl;
	//-----------------------------------------------//
	// �������� ��������

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(texture, 0);
	FIBITMAP* image = FreeImage_Load(format, texture);
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);

	GLubyte* bits = (GLubyte*)FreeImage_GetBits(image);

	//��������� the OpenGL ����������� �������
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(image);
	//-----------------------------------------------//

	//�������� ������� ������


	GLuint size, size_index;
	int* index_res = NULL;
	VertexText* points = NULL;
	MakeVertex_obj(vertices, size_vertices,
		normals, normal_size,
		text_coord, text_size,
		ver_indices, size_indices,
		&points, &index_res,
		&size, &size_index);
	// size = ;
	//-----------------------------------------------//
	glGenBuffers(1, &vertexBuffer);//���������� ������������� �������
	glGenVertexArrays(1, &vertexArray);//������� ��������� ������
	glGenBuffers(1, &indexArray);

	//��������� � ������� ������, ��� ����� �������� �������
	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//��������� �������� ������
	//��������� ������ � ������
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(*points), points, GL_STATIC_DRAW);

	sizeIndex = size_indices;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_index, &index_res[0], GL_STATIC_DRAW);

	//�������� = 0, ������ 3 float, �� ����� ���������������,
	//����� ���� �� ������, // ������� ��� ������ 1 ��������
	//����� � ������� ������(��������� 0)
	int tmpLoc = glGetAttribLocation(program, "modelPos");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), 0);
	glEnableVertexAttribArray(tmpLoc);//����� ��������

	tmpLoc = glGetAttribLocation(program, "modelNormal");
	glVertexAttribPointer(tmpLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);

	tmpLoc = glGetAttribLocation(program, "modelTexCoor");
	glVertexAttribPointer(tmpLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexText), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tmpLoc);

	glBindVertexArray(0);
	if (index_res != NULL)
		delete[] index_res;
	if (points != NULL)
		delete[] points;

};
*/