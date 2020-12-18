#pragma once
#include "Models.h";
#ifndef PI
#define PI 3.14159265359
#endif // !PI


const int Ntetta = 30;
const int Nphi = 60;
const unsigned int size_sphere = 3 * ((Ntetta - 2) * Nphi + 2);
const unsigned int size_sphere_index = 6 * (Ntetta - 2) * Nphi;
float sphere_vertices[3 * ((Ntetta - 2) * Nphi + 2)];
float sphere_normals[3 * ((Ntetta - 2) * Nphi + 2)];
GLuint sphere_indexes[6*(Ntetta - 2 ) * Nphi ];
/*
6 * (Ntetta-4 ) * Nphi -- слои в середине
+ 2 * Nphi -- полюсы
+ 5* 2 * Nphi -- слои, ближайшие к полюсу
*/

void Make_sphere(float R)
{
	float tetta = 0;
	float phi = 0;
	float nx, ny, nz;
	unsigned int index = 0;
	int nver = -1;
	int	nextCircle, nextPoint;
	for (int i = 0; i < Ntetta; i++)
	{		
		tetta = (float)i * PI / (Ntetta-1);
		
		if(i > 0 && i < (Ntetta-1))
		{
			
			for (int j = 0; j < Nphi; j++)
			{
				nver++;
				phi = (float)j * 2 * PI / (Nphi-1);
				nx = sin(tetta) * cos(phi);
				ny = cos(tetta);
				nz = sin(tetta) * sin(phi);

				//std::cout << "VER " << i * Nphi + j;
				sphere_vertices[3 * nver ] = R * nx;
				sphere_vertices[3 * nver+1] = R * ny;
				sphere_vertices[3 * nver +2] = R * nz;

				sphere_normals[3 * nver] =  nx;
				sphere_normals[3 * nver + 1] = ny;
				sphere_normals[3 * nver + 2] =  nz;
				
				
				//if (j%2)
				{
					nextCircle = (i < Ntetta - 2) ? (nver + Nphi) : (nver - j + Nphi);
					nextPoint = (j > 0) ? (nver -1) : ((nver+Nphi - 1)) ;
					sphere_indexes[index++] = nver;//текущая
					sphere_indexes[index++] = nextPoint;//предыдущая
					sphere_indexes[index++] = nextCircle;//ниже на круг
				}
				//else
				{
					nextPoint = (j > 0) ? (nver - 1) : ((nver + Nphi - 1));
					nextCircle = (i > 1) ? (nextPoint - Nphi) : (0);
					
					//nextPoint = (j < Nphi - 1) ? (nver+1) : (nver+1-Nphi);
					sphere_indexes[index++] = nver;//текущая			
					sphere_indexes[index++] = nextCircle;//выше на круг
					sphere_indexes[index++] = nextPoint;//предыдущая --//следующая
				}		
			}
			
		}
		else
		{
			nver++;
			phi = 0;
			nx = 0 ;
			ny = (i ? -1 : 1); //cos(tetta);
			nz = 0;

			//std::cout << "VER " << i * Nphi + j;
			sphere_vertices[3 * nver] = R * nx;
			sphere_vertices[3 * nver + 1] = R * ny;
			sphere_vertices[3 * nver + 2] = R * nz;

			sphere_normals[3 * nver] = nx;
			sphere_normals[3 * nver + 1] = ny;
			sphere_normals[3 * nver + 2] = nz;
			
		}
	}
	//std::cout << endl << index << 'P' << 6 * (Ntetta)*Nphi << endl;
	//std::cout << endl<< nver+1 << 'S' << 3 * ((Ntetta - 2) * Nphi + 2) << endl;
}