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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//设置图形显示模式,使用双缓存、建立RGB模式的窗口、深度缓存、
	glutInitWindowSize(800, 600);//初始化窗口大小
	glutInitWindowPosition(100, 100);//初始化窗口位置
	glutCreateWindow(argv[0]); //产生一个顶层窗口
	init();//初始化
	glutDisplayFunc(display);//为当前窗口设置显示回调函数
	glutReshapeFunc(reshape);//指定当窗口的大小改变时调用的函数
	glutKeyboardFunc(keyboard);//注册当前窗口的键盘回调函数
	glutSpecialFunc(specialkey);//设置当前窗口的特定键的回调函数
	glutMouseFunc(mouse);
	glutMainLoop();//让glut程序进入事件循环。

	delete p;
}
GLvoid init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);//渐变模式。GL_FLAT:单色模式  
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