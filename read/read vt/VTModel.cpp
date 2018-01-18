#include "VTModel.h"
#include"iostream"
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

VTModel::VTModel()//构造函数
{
}

VTModel::~VTModel()//析构函数
{

}

void VTModel::parse(const char *filename)//读文件
{
	FILE *f = fopen(filename, "r");

	if (f == NULL)
	{
		printf("File open error!");
	}

	fscanf(f, "%u %u", &num_point, &num_face);
	//开辟空间
	mem_point = (Point *)malloc(sizeof(Point)*num_point);
	mem_face = (Face *)malloc(sizeof(Face)*num_face);

	memset(mem_point, 0, sizeof(Point)*num_point);
	memset(mem_face, 0, sizeof(Face)*num_face);

	for (uint count = 0; count<num_point; count++)//读入所有的点
	{
		fscanf(f, "%f %f %f", &(mem_point[count].x), &(mem_point[count].y), &(mem_point[count].z));
		mem_point[count].nx = 0.0f;
		mem_point[count].ny = 0.0f;
		mem_point[count].nz = 0.0f;
		mem_point[count].num = 0;
	}
	//划分区域
	float max[3] = { 0 }, min[3] = { 0 };
	for (int i = 0; i < num_point; i++)
	{
		if (mem_point[i].x > max[0])
			max[0] = mem_point[i].x;
		if (mem_point[i].x < min[0])
			min[0] = mem_point[i].x;
		if (mem_point[i].y > max[1])
			max[1] = mem_point[i].y;
		if (mem_point[i].y < min[1])
			min[1] = mem_point[i].y;
		if (mem_point[i].z > max[2])
			max[2] = mem_point[i].z;
		if (mem_point[i].z < min[2])
			min[2] = mem_point[i].z;
	}
	cout << max[0] << endl << max[1] << endl << max[2] << endl;
	cout << min[0] << endl << min[1] << endl << min[2] << endl;
	/*float aver[3];
	aver[0] = (max[0] + min[0]) / 2;
	aver[1] = (max[1] + min[1]) / 2;
	aver[2] = (max[2] + min[2]) / 2;
	cout << aver[0] << endl << aver[1] << endl << aver[2] << endl;*/
	float bound[3];
	bound[0] = (max[0] - min[0]) / 8;
	bound[1] = (max[1] - min[1]) / 8;
	bound[2] = (max[2] - min[2]) / 8;
	//float b[3][9];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 9; j++)
		{
			b[i][j] = min[i] + bound[i] * j;
			cout << b[i][j] << endl;
		}

	for (uint count = 0; count<num_face; count++)//读入所有的面
	{
		fscanf(f, "%u %u %u", &(mem_face[count].p1), &(mem_face[count].p2), &(mem_face[count].p3));

		mem_face[count].nx = 0.0f;
		mem_face[count].ny = 0.0f;
		mem_face[count].nz = 0.0f;
	}

	fclose(f);

	for (uint count = 0; count<num_face; count++)//计算法向量
	{
		float a1, b1, c1, a2, b2, c2;

		a1 = mem_point[mem_face[count].p2].x - mem_point[mem_face[count].p1].x;
		b1 = mem_point[mem_face[count].p2].y - mem_point[mem_face[count].p1].y;
		c1 = mem_point[mem_face[count].p2].z - mem_point[mem_face[count].p1].z;
		a2 = mem_point[mem_face[count].p3].x - mem_point[mem_face[count].p1].x;
		b2 = mem_point[mem_face[count].p3].y - mem_point[mem_face[count].p1].y;
		c2 = mem_point[mem_face[count].p3].z - mem_point[mem_face[count].p1].z;
		//叉乘求得面法线
		mem_face[count].nx = b1*c2 - b2*c1;
		mem_face[count].ny = c1*a2 - a1*c2;
		mem_face[count].nz = a1*b2 - a2*b1;
		//求点法线
		mem_point[mem_face[count].p1].num++;
		mem_point[mem_face[count].p1].nx += mem_face[count].nx;
		mem_point[mem_face[count].p1].ny += mem_face[count].ny;
		mem_point[mem_face[count].p1].nz += mem_face[count].nz;

		mem_point[mem_face[count].p2].num++;
		mem_point[mem_face[count].p2].nx += mem_face[count].nx;
		mem_point[mem_face[count].p2].ny += mem_face[count].ny;
		mem_point[mem_face[count].p2].nz += mem_face[count].nz;

		mem_point[mem_face[count].p3].num++;
		mem_point[mem_face[count].p3].nx += mem_face[count].nx;
		mem_point[mem_face[count].p3].ny += mem_face[count].ny;
		mem_point[mem_face[count].p3].nz += mem_face[count].nz;
	}

	for (uint count = 0; count<num_point; count++)
	{
		mem_point[count].nx = mem_point[count].nx / mem_point[count].num;
		mem_point[count].ny = mem_point[count].ny / mem_point[count].num;
		mem_point[count].nz = mem_point[count].nz / mem_point[count].num;

		float d = sqrt(mem_point[count].nx*mem_point[count].nx + mem_point[count].ny*mem_point[count].ny + mem_point[count].nz*mem_point[count].nz);
		//单位化
		mem_point[count].nx = mem_point[count].nx / d;
		mem_point[count].ny = mem_point[count].ny / d;
		mem_point[count].nz = mem_point[count].nz / d;
	}
}