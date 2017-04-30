#include "stdafx.h"
#include"GL/glew.h"
#include"GL/glut.h"

#include"MathAPIKernel/Vector3D.h"
#include"MathAPIKernel/Position3D.h"
#include"MathAPIKernel/Matrix4D.h"

#include "RoamingScenceManager.h"
#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692
RoamingScenceManager::RoamingScenceManager()
{
    OldMouse=new Position3D();
    Mouse=new Position3D();

    NewEye=new Vector3D(100,0,0);
    NewUp=new Vector3D(0,0,1);
    NewView=new Vector3D(0,0,0);

    AuxY =new Vector3D(0,1,0);
    AuxZ=new Vector3D();
	*AuxZ = (Vector3D)*NewEye - (Vector3D)*NewView;
    AuxX=new Vector3D();
    *AuxX=Vector3D::crossProduct(*AuxY,*AuxZ);
    AuxX->normalize();

    TempTranslateVec=new Vector3D(0,0,0);
    TempscaleFactor=1;
}

RoamingScenceManager::~RoamingScenceManager()
{
    delete OldMouse;
    delete Mouse;
    delete NewEye;
    delete NewUp;
    delete NewView;
    delete AuxX;
    delete AuxY;
    delete AuxZ;
	delete TempTranslateVec;
}



void RoamingScenceManager::init()
{
   init_CoordinaryDisplayList();
   glShadeModel(GL_SMOOTH);
   glClearColor(0.156,0.156,0.168,0.0);
   glClearDepth(1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   // glMatrixMode用来声明下一步要做什么，
   //参数有3种模式: GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理.
   //
   //1、如果参数是GL_PROJECTION，这个是投影的意思
   //就是要对投影相关进行操作，也就是把物体投影到一个平面上
   //就像我们照相一样，把3维物体投到2维的平面上。
   //这样，接下来的语句可以是跟透视相关的函数，比如glFrustum()或gluPerspective()；
   //
   //2、如果参数是GL_MODELVIEW，这个是对模型视景的操作
   //接下来的语句描绘一个以模型为基础的适应，这样来设置参数
   //接下来用到的就是像gluLookAt()这样的函数；
   //
   //3、若是GL_TEXTURE，就是对纹理相关进行操作
   glMatrixMode(GL_PROJECTION);//将当前矩阵指定为投影矩阵
   glLoadIdentity();//把矩阵设为单位矩阵
   glOrtho(-1000/2,1000/2,-1000/2,1000/2,-1000/2,1000/2);//类似一个包围盒的东西，我们画的东西被他包在里面，参数设置包围盒的大小
 
}


void RoamingScenceManager::render()
{
	//DrawCube();
	DrawCylinder();
	//DrawSphere();

}

void RoamingScenceManager::executeRotateOperation(int x, int y)
{
    Mouse->setX(x);
    Mouse->setY(y);

    Vector3D MouseTrace=*AuxY*(OldMouse->Y()-Mouse->Y())+*AuxX*(Mouse->X()-OldMouse->X());
    Vector3D RotateAsix=Vector3D::crossProduct(MouseTrace,*AuxZ);
    RotateAsix.normalize();

    float angle=MouseTrace.length();
    Matrix4D rotatMatrix=Matrix4D::getRotateMatrix(angle,RotateAsix);

    *NewEye=rotatMatrix*(*NewEye);
    *NewUp=rotatMatrix*(*NewUp);

    NewUp->normalize();
    *AuxY=*NewUp;
	*AuxZ = (Vector3D)*NewEye - (Vector3D)*NewView;
    *AuxX=Vector3D::crossProduct(*AuxY,*AuxZ);
    AuxX->normalize();
    *OldMouse=*Mouse;

}

void RoamingScenceManager::executeScaleOperation(float factor)
{
	if (TempscaleFactor <= 0)
	{
		TempscaleFactor = 0.1;
	}
	else
	{
		TempscaleFactor += factor;
	}

}

void RoamingScenceManager::executeTranslateOperation(int x, int y)
{
    Mouse->setX(x);
    Mouse->setY(y);
    Vector3D vec(-OldMouse->X()+Mouse->X(),-Mouse->Y()+OldMouse->Y(),0);
    *TempTranslateVec=*TempTranslateVec+vec;
    *OldMouse=*Mouse;


}

void RoamingScenceManager::getInitPos(int x, int y)
{
    Mouse->setX(x);
    Mouse->setY(y);
    *OldMouse=*Mouse;
}

void RoamingScenceManager::init_CoordinaryDisplayList()
{
    ID_COORDINATY=glGenLists(1);
    glNewList(ID_COORDINATY,GL_COMPILE);

    int length=50;
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,-100);
    glVertex3f(length,0.0,-100);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,-100);
    glVertex3f(0.0,length,-100);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,-100);
    glVertex3f(0.0,0.0,length);
    glEnd();

    glLineWidth(1);
    glEnd();
    glEndList();
}


void RoamingScenceManager::DrawCube()
{
	glClearColor(0.156, 0.156, 0.168, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*NewView = *NewEye*-1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(TempTranslateVec->X(), TempTranslateVec->Y(), TempTranslateVec->Z());
	glScalef(TempscaleFactor, TempscaleFactor, TempscaleFactor);
	gluLookAt(NewEye->X(), NewEye->Y(), NewEye->Z(),
		NewView->X(), NewView->Y(), NewView->Z(),
		NewUp->X(), NewUp->Y(), NewUp->Z());
	glColor3f(0, 0.8, 0.5);
	glutSolidCube(100);
	glCallList(ID_COORDINATY);
}


void RoamingScenceManager::DrawCylinder()
{
	//画柱面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double delta = 1.0;//增加的度数
	int radius = 50;//半径
	int height = 180;//高
	int degree = 360;//度数
	double theta = delta;

	*NewView = *NewEye*-1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(TempTranslateVec->X(), TempTranslateVec->Y(), TempTranslateVec->Z());
	glScalef(TempscaleFactor, TempscaleFactor, TempscaleFactor);
	gluLookAt(NewEye->X(), NewEye->Y(), NewEye->Z(),
		NewView->X(), NewView->Y(), NewView->Z(),
		NewUp->X(), NewUp->Y(), NewUp->Z());

	//设置光影效果
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//GLfloat lightambient[] = { 0.5, 0.5, 0.5, 1.0 };//环境光
	//GLfloat lightdiffuse[] = { 1.0, 1.0, 1.0, 1.0 };//漫反射
	//GLfloat lightspecular[] = { 0.0, 1.0, 0.0, 1.0 };//镜面反射光
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightspecular);
	//GLfloat specular[] = { 0.5, 0.5, 0.5, 0.0 };//镜面反射光色
	//GLfloat shininess[] = { 100.0 }; //镜面反射的光亮度
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	//glLightModelf(GL_LIGHT_MODEL_AMBIENT, (0.0, 0.0, 0.0));

	//GLfloat light_position[] = { 3.0, 3.0, 3.0, 0.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	glColor3f(0.5f, 0.5f, 1.0f);
	glBegin(GL_QUAD_STRIP);
	glVertex3d(radius, 0, height/2);
	glVertex3d(radius, 0, -height / 2);
	for (int i = 1; i< degree; ++i)
	{
		double x = radius * cos(theta);
		double y = radius * sin(theta);
		glVertex3d(x, y, height / 2);
		glVertex3d(x, y, -height / 2);
		theta += delta;
	}
	glVertex3d(radius, 0, height / 2);
	glVertex3d(radius, 0, -height / 2);
	glEnd();



	//画下底面
	glColor3f(0.8f, 0.0f, 0.0f);
	theta = 0.0;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, -height / 2);
	glVertex3d(radius, 0, -height / 2);
	for (int i = 1; i < degree; ++i)
	{
		double x = radius * cos(theta);
		double y = radius * sin(theta);
		glVertex3d(x, y, -height / 2);
		theta += delta;
	}
	glVertex3d(radius, 0, -height / 2);
	glEnd();



	//画上底面
	glColor3f(1.0f, 1.0f, 1.0f);
	theta = 0.0;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, height / 2);
	glVertex3d(radius, 0, height / 2);
	for (int i = 0; i < degree; ++i)
	{
		double x = radius * cos(theta);
		double y = radius * sin(theta);
		glVertex3d(x, y, height / 2);
		theta += delta;
	}
	glVertex3d(radius, 0, height / 2);
	glEnd();


	glCallList(ID_COORDINATY);
}

void RoamingScenceManager::DrawSphere()
{
	//球心坐标为（x，y，z），球的半径为radius，M，N分别表示球体的横纵向被分成多少份
	GLfloat xx=0;
	GLfloat yy=0;
	GLfloat zz=0;
	GLfloat radius=50;
	GLfloat M =50;
	GLfloat N=50;
	float step_z = PI / M;
	float step_xy = 2 * PI / N;
	float x[4], y[4], z[4];

	float angle_z = 0.0;
	float angle_xy = 0.0;
	glColor3f(1.0f, 0.0f, 0.5f);
	/*glColor4f(1.0f, 0.0f, 0.5f, 0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*NewView = *NewEye*-1;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(TempTranslateVec->X(), TempTranslateVec->Y(), TempTranslateVec->Z());
	glScalef(TempscaleFactor, TempscaleFactor, TempscaleFactor);
	gluLookAt(NewEye->X(), NewEye->Y(), NewEye->Z(),
		NewView->X(), NewView->Y(), NewView->Z(),
		NewUp->X(), NewUp->Y(), NewUp->Z());
	glBegin(GL_QUADS);
	for (int i = -M; i<M; i++)
	{
		angle_z = i * step_z;//计算Z轴方向上的角度θ

		for (int j = -N; j<N; j++)
		{
			angle_xy = j * step_xy;//计算XY面上的角度β

			//x=r*cos(θ)*cos(β)
			//y=r*cos(θ)*sin(β)
			//z=r*sin(θ)
			x[0] = radius * cos(angle_z) * cos(angle_xy);
			y[0] = radius * cos(angle_z) * sin(angle_xy);
			z[0] = radius * sin(angle_z);

			x[1] = radius * cos(angle_z + step_z) * cos(angle_xy);
			y[1] = radius * cos(angle_z + step_z) * sin(angle_xy);
			z[1] = radius * sin(angle_z + step_z);

			x[2] = radius*cos(angle_z + step_z)*cos(angle_xy + step_xy);
			y[2] = radius*cos(angle_z + step_z)*sin(angle_xy + step_xy);
			z[2] = radius*sin(angle_z + step_z);

			x[3] = radius * cos(angle_z) * cos(angle_xy + step_xy);
			y[3] = radius * cos(angle_z) * sin(angle_xy + step_xy);
			z[3] = radius * sin(angle_z);

			for (int k = 0; k<4; k++)
			{
				glVertex3f(xx + x[k], yy + y[k], zz + z[k]);//加上圆心的坐标偏移
			}
		}
	}
	glEnd();
	glCallList(ID_COORDINATY);
}

