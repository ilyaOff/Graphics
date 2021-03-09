#pragma once

#include "Models.h"
 void MakeVertex_obj(float* vertex, unsigned int vertex_count,
	float* normal, unsigned int normal_count,
	float* text, unsigned int text_count,
	unsigned int* index, unsigned int polygon_count,
	VertexText** res, unsigned int** ind,
	unsigned int* size, unsigned int* size_index)
{
	
	//(*size) = vertex_count *3;
	 (*size) = polygon_count * 4*3;// по 4 вершины в полигоне
	 (*size_index) = polygon_count * 4*3;
	(*res) = new VertexText[(*size)];
	(*ind) = new unsigned int[(*size_index)];


	unsigned int indexx;
	unsigned int indexInVer;
	float scale = 0.25f;
	for (unsigned int i = 0; i < (*size); i++)
	{
		(*res)[i].position = glm::vec3(0, 0, 0);
		(*res)[i].texcoords = glm::vec2(0, 0);
		(*res)[i].normal = glm::vec3(0, 0, 0);
	}
	for (unsigned int i = 0; i < (*size_index); i++)
	{
		
		(*ind)[i] = 0;
	}

	//цикл по полигонам
	for (unsigned int i = 0; i < polygon_count; i++)
	{
		//цикл по вершинам полигона 		
		for (unsigned int j = 0; j < 4; j++)
		{
			// по 4 вершины в полигоне
			// по 3 элемента в вершине - позиция, нормаль, координаты текстуры
			indexx = 12 * i + j * 3;
			indexInVer = 4 * i + j;
			/*
			std::cout <<  "Point:" << indexInVer << endl;
			std::cout << "	position " << vertex[3 * (index[indexx] - 1)]
						<< " " << vertex[3 * (index[indexx] - 1) + 1]
						<< " " << vertex[3 * (index[indexx] - 1) + 2] << endl;

			std::cout << "	text " << text[3 * (index[indexx + 1] - 1)]
				<< " " << text[3 * (index[indexx + 1] - 1)+1]<< endl;

			std::cout << "	normal " << normal[3 * (index[indexx + 2] - 1)]
				<< " " << normal[3 * (index[indexx + 2] - 1) + 1]
				<< " " << normal[3 * (index[indexx + 2] - 1) + 2] << endl;
			*/
			(*res)[indexInVer].position = glm::vec3(scale*vertex[3 * (index[indexx] - 1)],
				scale*vertex[3 * (index[indexx] - 1) + 1],
				scale*vertex[3 * (index[indexx] - 1) + 2]);

			(*res)[indexInVer].texcoords = glm::vec2(text[3 * (index[indexx + 1] - 1)],
				text[3 * (index[indexx + 1] - 1) + 1]);


			(*res)[indexInVer].normal = glm::vec3(normal[3 * (index[indexx + 2] - 1)],
				vertex[3 * (index[indexx + 2] - 1) + 1],
				vertex[3 * (index[indexx + 2] - 1) + 2]);

			(*ind)[indexInVer] = (index[indexx] - 1);
		}

	}
//	for (unsigned int i = 0; i < (*size_index)*3; i+=3)
	{		
//		if(i%3 == 0)
//			(*ind)[i/3] = (index[i] - 1);
	}
	std::cout << "END Model MakePoints" << endl;
	std::cout << "Point:" << (*ind)[0] << endl;
	std::cout << "Point:" << (*ind)[1] << endl;
	std::cout << "Point:" << (*ind)[2] << endl;
	std::cout << "Point:" << (*ind)[3] << endl;

	std::cout << "Point:" << (*ind)[4] << endl;
	std::cout << "Point:" << (*ind)[5] << endl;
	std::cout << "Point:" << (*ind)[6] << endl;
	std::cout << "Point:" << (*ind)[7] << endl;
}