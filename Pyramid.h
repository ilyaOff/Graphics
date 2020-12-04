#pragma once
#pragma once
#include "Models.h"
/*Vertex pyramid_vertices[] = {
	glm::vec3 (-0.5f, 0.0f,  0.5f),//A
	glm::vec3(-0.5f, 0.0f, -0.5f ),//B
	glm::vec3(0.5f, 0.0f, -0.5f ),//C
	glm::vec3(0.5f, 0.0f, 0.5f ),//D
	glm::vec3(0.0f, 1.5f, 0.0f ),//O
};*/
/*float pyramid_vertices[] = {
	 -0.5f, 0.0f,  0.5f,//A
	 -0.5f, 0.0f, -0.5f ,//B
	 0.5f, 0.0f, -0.5f,//C
	 0.5f, 0.0f, 0.5f ,//D
	 0.0f, 1.5f, 0.0f ,//O
};*/

Vertex pyramid_vertices[] = {
	{ -0.5f, 0.0f,  0.5f},//A
	{ -0.5f, 0.0f, -0.5f },//B
	{ 0.5f, 0.0f, -0.5f },//C
	{ 0.5f, 0.0f, 0.5f },//D
	{ 0.0f, 1.5f, 0.0f },//O
};

GLuint pyramid_indices[] = {
	0,2,1,
	0,3,2,
	3,4,2,
	0,4,3,
	0,1,4,
	1,2,4
};
float pyramid_normal[] = {
	 -0.5f, 0.0f,  0.5f,//A
	 -0.5f, 0.0f, -0.5f ,//B
	 0.5f, 0.0f, -0.9f,//C
	 0.5f, 0.0f, 0.5f ,//D
	 0.0f, 1.5f, 0.0f ,//O
};