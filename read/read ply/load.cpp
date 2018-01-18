#include"loader.h"
plyfile::plyfile(string p)
{
	path = p;
	indices = NULL;
	max[0] = 0; max[1] = 0; max[2] = 0;
	min[0] = 0; min[1] = 0; min[2] = 0;
	xrotangle = 0;
	yrotangle = 0;
	load();
	size = sqrt(pow(max[0] - min[0], 2) + pow(max[1] - min[1], 2) + pow(max[2] - min[2], 2));
	scale[0] = 1;
	scale[1] = 1;
	scale[2] = 1;
}

plyfile::~plyfile() 
{
	if (indices != 0)
		delete[]indices;
	for (int i = 0; i < array.size(); i++)
		delete[]array[i];
}
//读取文件名为p的文件，成功返回1，否则返回0
GLboolean plyfile::load(string p)
{
	fstream  file;
	if (!p.empty())
		file.open(p, ios::in);//文件以输入方式打开(文件数据输入到内存)
	else if (!path.empty())
		file.open(path, ios::in);
	else return 0;
	if (file.is_open()==0)//打开失败
	{
		cout << "read file fail" << endl;
		return 0;
	}
	string filein;
	while (!file.eof())//没有到达文件结尾就往内存读
	{
		file >> filein;
		if (filein == "ply" || filein == "PLY")
		{
			getline(file, filein, '\n');
		}
		else if (filein == "comment")
			getline(file, filein, '\n');
		else if (filein == "format")
			getline(file, filein, '\n');
		else if (filein == "element")
		{
			file >> filein;
			if (filein == "vertex")
			{
				file >> vertexnum;//读取顶点数目
			}
			else if (filein == "face")
			{
				file >> facenum;//读取面数目
				getline(file, filein, '\n');
				getline(file, filein, '\n');
			}
			else {
				cout << "ignore user-defined element" << endl;
				getline(file, filein, '\n');
			}
		}
		else if (filein == "property")
		{
			file >> filein;
			if (filein == "char" || filein == "int8")
				propertytype.push_back(GL_BYTE);
			else if (filein == "uchar" || filein == "uint8")
				propertytype.push_back(GL_UNSIGNED_BYTE);
			else if (filein == "short" || filein == "int16")
				propertytype.push_back(GL_SHORT);
			else if (filein == "ushort" || filein == "uint16")
				propertytype.push_back(GL_UNSIGNED_SHORT);
			else if (filein == "int" || filein == "int32")
				propertytype.push_back(GL_INT);
			else if (filein == "uint" || filein == "uint32")
				propertytype.push_back(GL_UNSIGNED_INT);
			else if (filein == "float" || filein == "float32")
				propertytype.push_back(GL_FLOAT);
			else if (filein == "double" || filein == "float64")
				propertytype.push_back(GL_DOUBLE);
			else
			{
				cout<< "Undefined property type!" << endl;
				propertytype.push_back(GL_DOUBLE);
			}
			file >> filein;
			if (filein == "x")
			{
				elementtype.push_back(GL_VERTEX_ARRAY);
				getline(file, filein, '\n');//x
				getline(file, filein, '\n');//y
				getline(file, filein, '\n');//z
				getline(file, filein, '\n');//confidence
				getline(file, filein, '\n');//intensity
				count.push_back(3);
			}
			else
			{
				cout << "Unknown property!" << endl;
			}
		}			
		else if (filein == "end_header")
		{
			if (count.size() != propertytype.size() || count.size() != elementtype.size())
			{
				cout << "the number of element collision" << endl;
				return 0;
			}
			GLvoid *subarray = NULL;
			for (int i = 0; i < propertytype.size(); i++)
			{
				switch (propertytype[i])
				{
				case GL_BYTE:subarray = new GLbyte[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_UNSIGNED_BYTE:subarray = new GLubyte[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_SHORT:subarray = new GLshort[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_UNSIGNED_SHORT:subarray = new GLushort[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_INT:subarray = new GLint[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_UNSIGNED_INT:subarray = new GLuint[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_FLOAT:subarray = new GLfloat[vertexnum*count[i]];
					array.push_back(subarray); break;
				case GL_DOUBLE:subarray = new GLdouble[vertexnum*count[i]];
					array.push_back(subarray); break;
				}
			}	
			int num=-1;
			vertex=new GLfloat[vertexnum][3];
			for (int i = 0; i < vertexnum; i++)//接下来的vertexnum行都是顶点信息  
			{
				for (int j = 0; j < count.size(); j++)//每行包括顶点的count.size()种信息  
					for (int k = 0; k < count[j]; k++)//每种信息包括count[j]个property  
					{
						num++;
						switch (propertytype[j])
						{
				/*		case GL_BYTE: {
							GLdouble tem = 0;//(GLdouble)(((GLbyte*)(array[j]))[i*count[j]+k]);  
							file >> tem;
							((GLbyte*)(array[j]))[i*count[j] + k] = GLbyte(tem);
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break; }
						case GL_UNSIGNED_BYTE: {
							GLint tem = 0;//(GLdouble)(((GLubyte*)(array[j]))[i*count[j]+k]);  
							file >> tem;
							((GLubyte*)(array[j]))[i*count[j] + k] = GLubyte(tem);
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break; }
						case GL_SHORT:
							file >> ((GLshort*)(array[j]))[i*count[j] + k];
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								GLdouble tem = (GLdouble)(((GLshort*)(array[j]))[i*count[j] + k]);
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break;
						case GL_UNSIGNED_SHORT:
							file >> ((GLushort*)(array[j]))[i*count[j] + k];
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								GLdouble tem = (GLdouble)(((GLushort*)(array[j]))[i*count[j] + k]);
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break;
						case GL_INT:
							file >> ((GLint*)(array[j]))[i*count[j] + k];
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								GLdouble tem = (GLdouble)(((GLint*)(array[j]))[i*count[j] + k]);
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break;
						case GL_UNSIGNED_INT:
							file >> ((GLuint*)(array[j]))[i*count[j] + k];
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								GLdouble tem = (GLdouble)(((GLuint*)(array[j]))[i*count[j] + k]);
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break;*/
						case GL_FLOAT: {
							GLdouble tem;
							if (num % 3 == 0)
							{
								getline(file, filein, '\n');
								file >> tem;
							}
							else
								file >> tem;
							((GLfloat*)(array[j]))[i*count[j] + k]=GLfloat(tem);
							vertex[i][k] = tem;
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								GLdouble tem = (GLdouble)(((GLfloat*)(array[j]))[i*count[j] + k]);
								if (tem > max[k])max[k] = GLdouble(tem);
								if (tem < min[k])min[k] = GLdouble(tem);
							}
							break; }
					/*	case GL_DOUBLE:
							file >> ((GLdouble*)(array[j]))[i*count[j] + k];
							if (elementtype[j] == GL_VERTEX_ARRAY && k < 3)
							{
								GLdouble tem = ((GLdouble*)(array[j]))[i*count[j] + k];
								if (tem > max[k])max[k] = tem;
								if (tem < min[k])min[k] = tem;
							}
							break;*/
						}
					}
			}
			if (facenum != 0)
				indices = new GLushort[facenum * 3];
			int i = 0;
			getline(file, filein, '\n');
			file >> filein;
			while (filein == "3" && i<facenum)
			{
				file >> indices[i * 3] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
				file >> filein;
				i++;
			}
		}
		else
		{
			continue;
		}
	}
	file.close();
	return 1;//finished
}
//输入=放大，输入-缩小
GLboolean plyfile::key(GLubyte key)
{
	switch (key)
   {
	case '=':
		scale[0] *= 1.1;
		scale[1] *= 1.1;
		scale[2] *= 1.1;
		break;
	case '-':
		scale[0] *= 0.9;
		scale[1] *= 0.9;
		scale[2] *= 0.9;
		break;
	}
	glutPostRedisplay();//再显示
	return 1;
}
//按上下左右箭头进行旋转
GLboolean plyfile::skey(GLint key)
{
	switch (key)
	{
	case GLUT_KEY_UP:xrotangle -= 5.0; break;
	case GLUT_KEY_DOWN:xrotangle += 5.0; break;
	case GLUT_KEY_RIGHT:yrotangle += 5.0; break;
	case GLUT_KEY_LEFT:yrotangle -= 5.0; break;
	}
	glutPostRedisplay();
	return 1;
}
//缩放窗口时进行重画
GLboolean plyfile::reshape(GLint w, GLint h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);//根据窗口大小改变调整图形大小
	glMatrixMode(GL_PROJECTION);//下一步对投影矩阵进行操作
	glLoadIdentity();//将当前的用户坐标系的原点移到了屏幕中心
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, size * 0.05, size * 5);
	glMatrixMode(GL_MODELVIEW);//切换到模型视景
	glLoadIdentity();//将当前的用户坐标系的原点移到了屏幕中心
	gluLookAt(max[0] * 3, max[1] * 3, max[2] * 3,0.0, 0.0, 0.0,-max[0] * 3, -max[1] * 3, (pow(max[0], 2) + max[1], 2) * 3 / max[2]);
	return 1;
}
GLboolean plyfile::display()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();//矩阵压入堆栈

	glScalef(scale[0], scale[1], scale[2]);//模型缩放函数
	glRotatef(xrotangle, 1.0, 0.0, 0.0);//绕x轴旋转
	glRotatef(yrotangle, 0.0, 1.0, 0.0);//绕y轴旋转
	//进行平滑处理
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	//画坐标系
	glColor3f(1.0, 1.0, 1.0);//设置为白色
	glBegin(GL_LINES);//画线，glVertex*设置顶点坐标
	glVertex3f((min[0]<0) ? (1.4*(min[0])) : (-0.8*min[0]), 0.0, 0.0);//x轴下限
	glVertex3f(1.8*(max[0]), 0.0, 0.0);//x轴上限
	glVertex3f(0.0, (min[1]<0) ? (1.4*(min[1])) : (-0.8*min[1]), 0.0);
	glVertex3f(0.0, 1.8*(max[1]), 0.0);
	glVertex3f(0.0, 0.0, (min[2]<0) ? (1.4*(min[2])) : (-0.8*min[2]));
	glVertex3f(0.0, 0.0, 1.8*(max[2]));
	/*glVertex3f(min[0], 0.0, 0.0);//x轴下限
	glVertex3f(max[0], 0.0, 0.0);
	glVertex3f(0.0, min[1], 0.0);
	glVertex3f(0.0,max[1], 0.0);
	glVertex3f(0.0, 0.0, min[2]);
	glVertex3f(0.0, 0.0, max[2]);*/
	glEnd();

	glPushMatrix();
	glTranslatef(1.8*(max[0]), 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.02*size, 0.04*size, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.8*(max[1]), 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.02*size, 0.04*size, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.8*(max[2]));
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glutSolidCone(0.02*size, 0.04*size, 10, 10);
	glPopMatrix();
	cout << max[2] << endl << min[2] << endl;
	//画物体 
	//glColor3f(1.0, 1.0, 1.0);  
	for (int i = 0; i<(elementtype.size()); i++)
	{
		switch (elementtype[i])
		{
		case GL_VERTEX_ARRAY:
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(count[i], propertytype[i], 0, array[i]);
			break;
		case GL_NORMAL_ARRAY:
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(propertytype[i], 0, array[i]);
			break;
		case GL_COLOR_ARRAY:
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(count[i], propertytype[i], 0, array[i]);
			break;
		}
	}

	for (int i = 0; i<facenum; i++)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &indices[i*3]);
	}
	glPopMatrix();

	glutSwapBuffers();

	return 1;

}
//显示读的信息
/*GLvoid plyfile::print()
{
	cout << "PLY file" << endl;
	cout << "path:" << path << endl;
	cout << "vertexnum:" << vertexnum << endl;
	cout << "property number:" << array.size() << endl <<
		count.size() << endl <<
		propertytype.size() << endl <<
		elementtype.size() << endl;
	cout << "count:" << endl;
	for (int i = 0; i<count.size(); i++)
		cout << i << " : " << count[i] << endl;
	cout << "propertytype" << endl;
	for (int i = 0; i<propertytype.size(); i++)
		cout << i << " : " << propertytype[i] << endl;
	cout << "elementtype" << endl;
	for (int i = 0; i<elementtype.size(); i++)
		cout << i << " : " << elementtype[i] << endl;
	cout << "max:" << endl;
	cout << max[0] << '\t' << max[1] << '\t' << max[2] << '\t' << endl;
	cout << "min:" << endl;
	cout << min[0] << '\t' << min[1] << '\t' << min[2] << '\t' << endl;
	cout << "facenum:" << facenum << endl;


}*/
GLvoid plyfile::mouse(GLint button, GLint state, GLint x, GLint y)
{
	
	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
	{
		isclick = true;
		getselect(x, y);
	}
	
		
}
GLvoid plyfile::getselect(GLint xmouse, GLint ymouse)
{
	GLdouble    modelview[16];
	GLdouble    projection[16];
	GLint       viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble world_x, world_y, world_z;
	// 获取近裁剪面上的交点
	gluUnProject((GLdouble)xmouse, (GLdouble)ymouse, 0, modelview, projection, viewport, &world_x, &world_y, &world_z);

	near_point[0] = world_x;
	near_point[1] = world_y;
	near_point[2] = world_z;

	// 获取远裁剪面上的交点
	gluUnProject((GLdouble)xmouse, (GLdouble)ymouse, 1, modelview, projection, viewport, &world_x, &world_y, &world_z);

	far_point[0] = world_x;
	far_point[1] = world_y;
	far_point[2] = world_z;
	GLfloat vVector[3];//求线段的向量
	vVector[0] = near_point[0] - far_point[0];
	vVector[1] = near_point[1] - far_point[1];
	vVector[2] = near_point[2] - far_point[2];
	if (intersect(vVector[0], vVector[1], vVector[2])==1)
	{
		cout << "x=" << crosspoint[0] << endl;
		cout << "y=" << crosspoint[1] << endl;
		cout << "z=" << crosspoint[2] << endl;

	}
}
/*获取近裁剪面上的交点和远裁剪面上的交点，这里涉及到了世界坐标系与交互平面上点的转化。
近裁剪面上的交点和远裁剪面上的交点会产生一条直线，求直线与平面的交点，
判断该交点是否在这个平面内，若在读出该点即可。
（1）求解直线与平面的交点坐标
思路：
   首先将直线方程转换为参数方程形式，然后代入平面方程，求得参数t；
   将t代入直线的参数方程即可求出交点坐标。

（2）判断交点是否在三角形内部
思路：
   面积法*/
GLint plyfile::intersect(GLfloat x, GLfloat y, GLfloat z)//判断交点是否在三维模型的某个面里
{
	normal=new GLfloat[facenum][3];
	for (int i = 0; i < facenum; i++)
	{
		GLfloat temp1[3], temp2[3];
		GLfloat length;
		temp1[0] = vertex[indices[i * 3]][0] - vertex[indices[i * 3 + 1]][0];//两点x相减
		temp1[1] = vertex[indices[i * 3]][1] - vertex[indices[i * 3 + 1]][1];//两点y相减
		temp1[2] = vertex[indices[i * 3]][2] - vertex[indices[i * 3 + 1]][2];//两点z相减
		temp2[0] = vertex[indices[i * 3 + 1]][0] - vertex[indices[i * 3 + 2]][0];
		temp2[1] = vertex[indices[i * 3 + 1]][1] - vertex[indices[i * 3 + 2]][1];
		temp2[2] = vertex[indices[i * 3 + 1]][2] - vertex[indices[i * 3 + 2]][2];
		normal[i][0]= temp1[1] * temp2[2] - temp1[2] * temp2[1];
		normal[i][1] = temp1[0] * temp2[2] - temp1[2] * temp2[0];
		normal[i][2]= temp1[0] * temp2[1] - temp1[1] * temp2[0];
		length = sqrt(normal[i][0] * normal[i][0] + normal[i][1] * normal[i][1] + normal[i][2] * normal[i][2]);
		normal[i][0] /= length;
		normal[i][1] /= length;
		normal[i][2] /= length;
	}
	GLfloat A[3], B[3], C[3];//顶点A的xyz坐标，顶点B的xyz坐标，顶点C的xyz坐标
	GLfloat a, b, c, L1, L2, L3,Pa,Pb,Pc,P,Sa,Sb,Sc,S,S1;
	GLfloat TriD;//平面方程常数项
	for (int i = 0; i < facenum; i++)
	{
		GLfloat TriangleV[3];//三角形法向量
		for (int j = 0; j < 3; j++)
			TriangleV[j] = normal[i][j];
		A[0] = vertex[indices[i * 3]][0];
		A[1] = vertex[indices[i * 3]][1];
		A[2] = vertex[indices[i * 3]][2];
		B[0] = vertex[indices[i * 3 + 1]][0];
		B[1] = vertex[indices[i * 3 + 1]][1];
		B[2] = vertex[indices[i * 3 + 1]][2];
		C[0] = vertex[indices[i * 3 + 2]][0];
		C[1] = vertex[indices[i * 3 + 2]][1];
		C[2] = vertex[indices[i * 3 + 2]][2];
		//计算参数t
		TriD = -(TriangleV[0] * A[0] + TriangleV[1] * A[1] + TriangleV[2] * A[2]);//常数项
		float tempU, tempD;  //临时变量
		tempU = TriangleV[0] * near_point[0] + TriangleV[1] * near_point[1] + TriangleV[2] * near_point[2] + TriD;
		tempD = TriangleV[0] * x + TriangleV[1] * y + TriangleV[2] * z;
		float t = -tempU / tempD;
		//计算交点坐标
		crosspoint[0] = x*t + near_point[0];
		crosspoint[1] = y*t + near_point[1];
		crosspoint[2] = z*t + near_point[2];
		//计算交点到三个顶点的长度
		L1 = sqrt((crosspoint[0] - A[0])*(crosspoint[0] - A[0]) + (crosspoint[1] - A[1])*(crosspoint[1] - A[1]) + (crosspoint[2] - A[2])*(crosspoint[2] - A[2]));
		L2 = sqrt((crosspoint[0] - B[0])*(crosspoint[0] - B[0]) + (crosspoint[1] - B[1])*(crosspoint[1] - B[1]) + (crosspoint[2] - B[2])*(crosspoint[2] - B[2]));
		L3 = sqrt((crosspoint[0] - C[0])*(crosspoint[0] - C[0]) + (crosspoint[1] - C[1])*(crosspoint[1] - C[1]) + (crosspoint[2] - C[2])*(crosspoint[2] - C[2]));
		//计算三角形三条边的长度
		a = sqrt((C[0] - B[0])*(C[0] - B[0]) + (C[1] - B[1])*(C[1] - B[1]) + (C[2] - B[2])*(C[2] - B[2]));
		b = sqrt((C[0] - A[0])*(C[0] - A[0]) + (C[1] - A[1])*(C[1] - A[1]) + (C[2] - A[2])*(C[2] - A[2]));
		c = sqrt((B[0] - A[0])*(B[0] - A[0]) + (B[1] - A[1])*(B[1] - A[1]) + (B[2] - A[2])*(B[2] - A[2]));
		//判断是否在三角形内
		Pa = (L2 + L3 + a) / 2;
		Sa = sqrt(Pa*(Pa - a)*(Pa - L2)*(Pa - L3));

		Pb = (L1 + L3 + b) / 2;
		Sb = sqrt(Pb*(Pb - b)*(Pb - L1)*(Pa - L3));

		Pc = (L1 + L2 + c) / 2;
		Sc = sqrt(Pc*(Pc - c)*(Pc - L2)*(Pc - L1));
		S1 = Sa + Sb + Sc;

		P = (a + b + c) / 2;
		S = sqrt(P*(P - a)*(P - b)*(P - c));
		if (S1!=0&&S1<= S)
		{
			//cout << S << endl << S1 << endl;
			return 1;
		}
		if (i == facenum)
			return 0;
	}
}

