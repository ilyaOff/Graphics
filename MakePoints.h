#pragma once

#include "Models.h"

void MakeVertex_obj(float* vertex, unsigned int vertex_size,
	float* normal, unsigned int normal_size,
	float* text, unsigned int text_size,
	unsigned int* index, unsigned int index_size,
	VertexText** res, int** index_res, 
	unsigned int* size)
{
	 (*size) = index_size * 4;
	(*res) = new VertexText[vertex_size];
	(*index_res) = new int[*size];
	unsigned int indexx;
	for (unsigned int i = 0; i < index_size; i++)
	{
		for(int j = 0; j < 4; j++)
		{ 
			indexx = 12 * i + j * 3;
			(*res)[indexx].position = glm::vec3(vertex[3*(index[indexx]-1)  ],
									vertex[ 3*(index[indexx]-1)  +1 ],
									vertex[ 3*(index[indexx]-1)  +2 ]);

			(*res)[indexx].texcoords = glm::vec2(text[ 3*(index[indexx + 1]-1) ],
										text[ 3*(index[indexx + 1]-1) + 1]);


			(*res)[indexx].normal = glm::vec3(normal[ 3*(index[indexx+2]-1)],
										vertex[3*(index[indexx + 2] - 1) + 1],
										vertex[3*(index[indexx + 2] - 1) + 2]);

			(*index_res)[indexx] = index[indexx] - 1;
		}
	}
}