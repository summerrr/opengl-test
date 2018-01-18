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
		plyfile(string = "");//���캯��
		~plyfile();//��������
	//	GLvoid print();
		GLboolean display();//ģ����ʾ
		GLboolean reshape(GLint, GLint);
		GLboolean key(GLubyte);
		GLboolean skey(GLint);
		GLvoid mouse(GLint, GLint, GLint, GLint);
		GLvoid getselect(GLint, GLint);
		GLint intersect(GLfloat, GLfloat, GLfloat);
    private:
	    GLboolean load(string = "");//����ģ��

		string path;//·��
					 
		GLint vertexnum;//��������
		vector<GLvoid *>array;/*save information of vertex,different element in different array*/
		vector<GLsizei>count;//Ԫ�ذ�����������
		vector<GLenum>propertytype;//��������
		vector<GLenum>elementtype;/*Ԫ������must be GL_VERTEX_ARRAY/GL_NORMAL_ARRAY/GL_COLOR_ARRAY*/
								 
		GLint facenum;//������
		GLushort *indices;//����
						//detail for projection translation  
		GLdouble max[3];/*max of x,y,z*/
		GLdouble min[3];/*min of x,y,z*/
		GLdouble size;//�Խ��߳���
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