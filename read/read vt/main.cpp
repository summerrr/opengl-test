#include <gl\glut.h>
#include "iostream"
#include "VTModel.h"
#include<time.h>
#define random(x) (rand()%x)

using namespace std;
float window_width = 600;
float window_height = 600;

//旋转角度
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
//平移距离
GLfloat xTrans = 0;
GLfloat yTrans = 0;
GLfloat zTrans = -32.0;

int ox;
int oy;
int buttonState;
GLfloat xRotLength = 0.0f;
GLfloat yRotLength = 0.0f;

VTModel *vt_model;
//设置光源参数数据
GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[] = { 0.1f, 0.1f, 0.8f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//画一个立方体（位置、长宽高）
void draw_box(float ox, float oy, float oz, float width, float height, float length)
{
	glLineWidth(1.0f);//线宽
	//开始画线12条边
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	//1
	glVertex3f(ox, oy, oz);//设置坐标
	glVertex3f(ox + width, oy, oz);

	//2
	glVertex3f(ox, oy, oz);
	glVertex3f(ox, oy + height, oz);

	//3
	glVertex3f(ox, oy, oz);
	glVertex3f(ox, oy, oz + length);

	//4
	glVertex3f(ox + width, oy, oz);
	glVertex3f(ox + width, oy + height, oz);

	//5
	glVertex3f(ox + width, oy + height, oz);
	glVertex3f(ox, oy + height, oz);

	//6
	glVertex3f(ox, oy + height, oz + length);
	glVertex3f(ox, oy, oz + length);

	//7
	glVertex3f(ox, oy + height, oz + length);
	glVertex3f(ox, oy + height, oz);

	//8
	glVertex3f(ox + width, oy, oz);
	glVertex3f(ox + width, oy, oz + length);

	//9
	glVertex3f(ox, oy, oz + length);
	glVertex3f(ox + width, oy, oz + length);

	//10
	glVertex3f(ox + width, oy + height, oz);
	glVertex3f(ox + width, oy + height, oz + length);

	//11
	glVertex3f(ox + width, oy + height, oz + length);
	glVertex3f(ox + width, oy, oz + length);

	//12
	glVertex3f(ox, oy + height, oz + length);
	glVertex3f(ox + width, oy + height, oz + length);
	glEnd();
}
//初始化
void init()
{
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0, (float)window_width/window_height, 10.0f, 100.0);
	glOrtho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
}

void draw_bunny()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//正面和背面填充模式
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	for (uint count = 0; count<vt_model->num_face; count++)
	{
		glNormal3f(vt_model->mem_point[vt_model->mem_face[count].p1].nx, vt_model->mem_point[vt_model->mem_face[count].p1].ny, vt_model->mem_point[vt_model->mem_face[count].p1].nz);
		glVertex3f(vt_model->mem_point[vt_model->mem_face[count].p1].x, vt_model->mem_point[vt_model->mem_face[count].p1].y, vt_model->mem_point[vt_model->mem_face[count].p1].z);
		/*if (vt_model->mem_point[vt_model->mem_face[count].p1].x> vt_model->b[0][0] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][1])
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][1] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][2])
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][2] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][3])
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][3] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][4])
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][4] && vt_model->mem_point[vt_model->mem_face[count].p1].x <vt_model->b[0][5])
			glColor3f(1.0f, 0.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][5] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][6])
			glColor3f(0.0f, 1.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][6] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][7])
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p1].x>vt_model->b[0][7] && vt_model->mem_point[vt_model->mem_face[count].p1].x < vt_model->b[0][8])
			glColor3f(1.0f, 0.5f, 1.0f);*/

		glNormal3f(vt_model->mem_point[vt_model->mem_face[count].p2].nx, vt_model->mem_point[vt_model->mem_face[count].p2].ny, vt_model->mem_point[vt_model->mem_face[count].p2].nz);
		glVertex3f(vt_model->mem_point[vt_model->mem_face[count].p2].x, vt_model->mem_point[vt_model->mem_face[count].p2].y, vt_model->mem_point[vt_model->mem_face[count].p2].z);
	/*	if (vt_model->mem_point[vt_model->mem_face[count].p2].x> vt_model->b[0][0] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][1])
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][1] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][2])
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][2] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][3])
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][3] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][4])
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][4] && vt_model->mem_point[vt_model->mem_face[count].p2].x <vt_model->b[0][5])
			glColor3f(1.0f, 0.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][5] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][6])
			glColor3f(0.0f, 1.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][6] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][7])
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p2].x>vt_model->b[0][7] && vt_model->mem_point[vt_model->mem_face[count].p2].x < vt_model->b[0][8])
			glColor3f(1.0f, 0.5f, 1.0f);*/

		glNormal3f(vt_model->mem_point[vt_model->mem_face[count].p3].nx, vt_model->mem_point[vt_model->mem_face[count].p3].ny, vt_model->mem_point[vt_model->mem_face[count].p3].nz);
		glVertex3f(vt_model->mem_point[vt_model->mem_face[count].p3].x, vt_model->mem_point[vt_model->mem_face[count].p3].y, vt_model->mem_point[vt_model->mem_face[count].p3].z);
		/*if (vt_model->mem_point[vt_model->mem_face[count].p3].x> vt_model->b[0][0] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][1])
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][1] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][2])
			glColor3f(0.0f, 1.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][2] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][3])
			glColor3f(0.0f, 0.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][3] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][4])
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][4] && vt_model->mem_point[vt_model->mem_face[count].p3].x <vt_model->b[0][5])
			glColor3f(1.0f, 0.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][5] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][6])
			glColor3f(0.0f, 1.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][6] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][7])
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (vt_model->mem_point[vt_model->mem_face[count].p3].x>vt_model->b[0][7] && vt_model->mem_point[vt_model->mem_face[count].p3].x < vt_model->b[0][8])
			glColor3f(1.0f, 0.5f, 1.0f);*/
	}
	glEnd();
}
/*void test_bunny()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//正面和背面填充模式
	glColor3f(1.0f, 0.5f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (uint count = 0; count < vt_model->num_face; count++)
	{
		srand((int)time(0));
		glNormal3f(vt_model->mem_point[vt_model->mem_face[count].p1].nx, vt_model->mem_point[vt_model->mem_face[count].p1].ny, vt_model->mem_point[vt_model->mem_face[count].p1].nz);
		float len = sqrt(vt_model->mem_point[vt_model->mem_face[count].p1].nx*vt_model->mem_point[vt_model->mem_face[count].p1].nx + vt_model->mem_point[vt_model->mem_face[count].p1].ny*vt_model->mem_point[vt_model->mem_face[count].p1].ny + vt_model->mem_point[vt_model->mem_face[count].p1].nz*vt_model->mem_point[vt_model->mem_face[count].p1].nz);
		float rx = vt_model->mem_point[vt_model->mem_face[count].p1].x + vt_model->mem_point[vt_model->mem_face[count].p1].nx / len*random(3);
		float ry = vt_model->mem_point[vt_model->mem_face[count].p1].y + vt_model->mem_point[vt_model->mem_face[count].p1].ny / len*random(3);
		float rz = vt_model->mem_point[vt_model->mem_face[count].p1].z + vt_model->mem_point[vt_model->mem_face[count].p1].nz / len*random(3);
		glVertex3f(rx, ry, rz);
		if (rx < 1&&ry<1&&rz<1)
			glColor3f(1.0, 0, 0);
		else if (rx < 2&&ry<2&&rz<2)
			glColor3f(0, 1.0, 0);
		else if (rx < 3&&ry<3&&rz<3)
			glColor3f(0, 0, 1.0);
		else
			glColor3f(1.0, 1.0, 1.0);
		glNormal3f(vt_model->mem_point[vt_model->mem_face[count].p2].nx, vt_model->mem_point[vt_model->mem_face[count].p2].ny, vt_model->mem_point[vt_model->mem_face[count].p2].nz);
		glVertex3f(vt_model->mem_point[vt_model->mem_face[count].p2].x, vt_model->mem_point[vt_model->mem_face[count].p2].y, vt_model->mem_point[vt_model->mem_face[count].p2].z);
		glNormal3f(vt_model->mem_point[vt_model->mem_face[count].p3].nx, vt_model->mem_point[vt_model->mem_face[count].p3].ny, vt_model->mem_point[vt_model->mem_face[count].p3].nz);
		glVertex3f(vt_model->mem_point[vt_model->mem_face[count].p3].x, vt_model->mem_point[vt_model->mem_face[count].p3].y, vt_model->mem_point[vt_model->mem_face[count].p3].z);
	}
	glEnd();
}*/

void display_func()
{
	//glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_DEPTH_TEST);
	//glClearColor(0.75f, 0.75f, 0.75f, 0.25f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	if (buttonState == 1)
	{
		xRot += (xRotLength - xRot)*0.1f;
		yRot += (yRotLength - yRot)*0.1f;
	}

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // 设置1号光源环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // 设置1号光源漫射光
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // 设置1号光源位置
	glEnable(GL_LIGHT1);//打开光源
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glTranslatef(xTrans, yTrans, zTrans);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	draw_box(-10.0f, -10.0f, -10.0f, 20.0f, 20.0f, 20.0f);
	draw_bunny();

	glPopMatrix();

	glutSwapBuffers();
}

void reshape_func(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (float)width / height, 0.001, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
}
//w、s沿z平移，a、d沿x平移，q、e沿y平移
void keyboard_func(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		zTrans += 2.0f;
	}

	if (key == 's')
	{
		zTrans -= 2.0f;
	}

	if (key == 'a')
	{
		xTrans -= 2.0f;
	}

	if (key == 'd')
	{
		xTrans += 2.0f;
	}

	if (key == 'q')
	{
		yTrans -= 2.0f;
	}

	if (key == 'e')
	{
		yTrans += 2.0f;
	}

	glutPostRedisplay();
}
//↑向上平移，↓向下平移，←向左平移，→向右平移
void specialkey_func(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		zTrans += 2.0f;
	}

	if (key == GLUT_KEY_DOWN)
	{
		zTrans -= 2.0f;
	}

	if (key == GLUT_KEY_LEFT)
	{
		xTrans -= 2.0f;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		xTrans += 2.0f;
	}

	glutPostRedisplay();
}
//鼠标按下状态置为1，松开状态置为0，传入鼠标位置给ox，oy
void mouse_func(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		buttonState = 1;
	}
	else if (state == GLUT_UP)
	{
		buttonState = 0;
	}

	ox = x; oy = y;

	glutPostRedisplay();
}
//状态为1进行旋转
void motion_func(int x, int y)
{
	float dx, dy;
	dx = (float)(x - ox);
	dy = (float)(y - oy);

	if (buttonState == 1)
	{
		xRotLength += dy / 5.0f;
		yRotLength += dx / 5.0f;
	}

	ox = x; oy = y;

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Bunny");

	init();

	vt_model = new VTModel();
	vt_model->parse("bunny.vt");

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glutDisplayFunc(display_func);
	glutReshapeFunc(reshape_func);
	glutSpecialFunc(specialkey_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 0;
}