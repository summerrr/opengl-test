#pragma once
#include<string>
#include<iostream>
#include<gl/glut.h>
#include<vector>
#include<fstream>
using namespace std;
class plyfile
{
    public:
		plyfile(string = "");//构造函数
		~plyfile();//析构函数
	//	GLvoid print();
		GLboolean display();//模型显示
		GLboolean reshape(GLint, GLint);
		GLboolean key(GLubyte);
		GLboolean skey(GLint);
		GLvoid mouse(GLint, GLint, GLint, GLint);
		GLvoid getselect(GLint, GLint);
		GLint intersect(GLfloat, GLfloat, GLfloat);
    private:
	    GLboolean load(string = "");//载入模型

		string path;//路径
					 
		GLint vertexnum;//顶点数量
		vector<GLvoid *>array;/*save information of vertex,different element in different array*/
		vector<GLsizei>count;//元素包含的属性数
		vector<GLenum>propertytype;//属性类型
		vector<GLenum>elementtype;/*元素类型must be GL_VERTEX_ARRAY/GL_NORMAL_ARRAY/GL_COLOR_ARRAY*/
								 
		GLint facenum;//面数量
		GLushort *indices;//索引
						//detail for projection translation  
		GLdouble max[3];/*max of x,y,z*/
		GLdouble min[3];/*min of x,y,z*/
		GLdouble size;//对角线长度
					  //model translation  
		GLdouble xrotangle;
		GLdouble yrotangle;

		GLdouble scale[3];
		GLfloat near_point[3];
		GLfloat far_point[3];
		GLfloat crosspoint[3];
		GLboolean isclick;
		GLfloat(*vertex)[3];
		GLfloat(*normal)[3];
		




};