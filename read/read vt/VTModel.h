#pragma once
#ifndef __VTMODEL_H__
#define __VTMODEL_H__
typedef unsigned int uint;
struct Point
{
	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	uint num;
};
struct Face
{
	uint p1;
	uint p2;
	uint p3;

	float nx;
	float ny;
	float nz;
};
class VTModel
{
public:
	Point *mem_point;
	Face *mem_face;

	uint num_point;//点的数量
	uint num_face;//面的数量
	float b[3][9];//区域划分边界
public:
	VTModel();
	~VTModel();
	void parse(const char *filename);
};
#endif