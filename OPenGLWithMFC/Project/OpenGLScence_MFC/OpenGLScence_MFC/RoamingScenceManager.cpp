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
   // glMatrixMode����������һ��Ҫ��ʲô��
   //������3��ģʽ: GL_PROJECTION ͶӰ, GL_MODELVIEW ģ����ͼ, GL_TEXTURE ����.
   //
   //1�����������GL_PROJECTION�������ͶӰ����˼
   //����Ҫ��ͶӰ��ؽ��в�����Ҳ���ǰ�����ͶӰ��һ��ƽ����
   //������������һ������3ά����Ͷ��2ά��ƽ���ϡ�
   //���������������������Ǹ�͸����صĺ���������glFrustum()��gluPerspective()��
   //
   //2�����������GL_MODELVIEW������Ƕ�ģ���Ӿ��Ĳ���
   //��������������һ����ģ��Ϊ��������Ӧ�����������ò���
   //�������õ��ľ�����gluLookAt()�����ĺ�����
   //
   //3������GL_TEXTURE�����Ƕ�������ؽ��в���
   glMatrixMode(GL_PROJECTION);//����ǰ����ָ��ΪͶӰ����
   glLoadIdentity();//�Ѿ�����Ϊ��λ����
   glOrtho(-1000/2,1000/2,-1000/2,1000/2,-1000/2,1000/2);//����һ����Χ�еĶ��������ǻ��Ķ��������������棬�������ð�Χ�еĴ�С
 
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
	//������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double delta = 1.0;//���ӵĶ���
	int radius = 50;//�뾶
	int height = 180;//��
	int degree = 360;//����
	double theta = delta;

	*NewView = *NewEye*-1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(TempTranslateVec->X(), TempTranslateVec->Y(), TempTranslateVec->Z());
	glScalef(TempscaleFactor, TempscaleFactor, TempscaleFactor);
	gluLookAt(NewEye->X(), NewEye->Y(), NewEye->Z(),
		NewView->X(), NewView->Y(), NewView->Z(),
		NewUp->X(), NewUp->Y(), NewUp->Z());

	//���ù�ӰЧ��
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//GLfloat lightambient[] = { 0.5, 0.5, 0.5, 1.0 };//������
	//GLfloat lightdiffuse[] = { 1.0, 1.0, 1.0, 1.0 };//������
	//GLfloat lightspecular[] = { 0.0, 1.0, 0.0, 1.0 };//���淴���
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightspecular);
	//GLfloat specular[] = { 0.5, 0.5, 0.5, 0.0 };//���淴���ɫ
	//GLfloat shininess[] = { 100.0 }; //���淴��Ĺ�����
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



	//���µ���
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



	//���ϵ���
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
	//��������Ϊ��x��y��z������İ뾶Ϊradius��M��N�ֱ��ʾ����ĺ����򱻷ֳɶ��ٷ�
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
		angle_z = i * step_z;//����Z�᷽���ϵĽǶȦ�

		for (int j = -N; j<N; j++)
		{
			angle_xy = j * step_xy;//����XY���ϵĽǶȦ�

			//x=r*cos(��)*cos(��)
			//y=r*cos(��)*sin(��)
			//z=r*sin(��)
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
				glVertex3f(xx + x[k], yy + y[k], zz + z[k]);//����Բ�ĵ�����ƫ��
			}
		}
	}
	glEnd();
	glCallList(ID_COORDINATY);
}

