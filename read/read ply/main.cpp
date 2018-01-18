#include"loader.h"
GLvoid init();
GLvoid display();
GLvoid reshape(GLint, GLint);
GLvoid keyboard(GLubyte, GLint, GLint);
GLvoid specialkey(GLint, GLint, GLint);
GLvoid mouse(GLint, GLint, GLint, GLint);
GLvoid getselect(GLint, GLint);
GLint intersect(GLfloat, GLfloat, GLfloat);
plyfile *p;
void main(int argc, char**argv)
{
	p = new plyfile("bunny.ply");
//	p->print();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//����ͼ����ʾģʽ,ʹ��˫���桢����RGBģʽ�Ĵ��ڡ���Ȼ��桢
	glutInitWindowSize(800, 600);//��ʼ�����ڴ�С
	glutInitWindowPosition(100, 100);//��ʼ������λ��
	glutCreateWindow(argv[0]); //����һ�����㴰��
	init();//��ʼ��
	glutDisplayFunc(display);//Ϊ��ǰ����������ʾ�ص�����
	glutReshapeFunc(reshape);//ָ�������ڵĴ�С�ı�ʱ���õĺ���
	glutKeyboardFunc(keyboard);//ע�ᵱǰ���ڵļ��̻ص�����
	glutSpecialFunc(specialkey);//���õ�ǰ���ڵ��ض����Ļص�����
	glutMouseFunc(mouse);
	glutMainLoop();//��glut��������¼�ѭ����

	delete p;
}
GLvoid init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);//����ģʽ��GL_FLAT:��ɫģʽ  
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
GLvoid display()
{
	p->display();
}

GLvoid reshape(GLint w, GLint h)
{
	p->reshape(w, h);
}

GLvoid keyboard(GLubyte key, GLint x, GLint y)
{
	p->key(key);
}

GLvoid specialkey(GLint key, GLint x, GLint y)
{
	p->skey(key);
}
GLvoid mouse(GLint button, GLint state, GLint x, GLint y)
{
	p->mouse(button,state,x,y);
}