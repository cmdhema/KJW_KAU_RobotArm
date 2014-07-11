#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <gl\glut.h>	
#include <gl\gl.h>		
#include <gl\glu.h>
#include <gl\GLAUX.H>
#include <math.h>
#include <time.h>

#define COUNT 36
#define PI 3.141592
void draw(GLenum);

using namespace std;

//View	
float ViewX = 0.0;
float ViewY = 0.0;
float ViewZ = 50.0;

float View1X = 0.0;
float View1Y = 2.0;
float View1Z = 12.0;

int View4SelectCheck = 0;

float ViewTemp1 = 0.0;
float ViewTemp2 = 0.0;
float ViewTemp3 = 0.0;

GLint ViewMinus1 = 0;
GLint ViewMinus2 = 0;
GLint ViewMinus3 = 0;

int Select_View = 1;



GLuint tex[6]; //  texture image 저장할 변수

GLfloat bottomTheta[COUNT];
GLint Width, Height;

GLUquadricObj *cylinder;
GLUquadricObj *box;

GLfloat delta = 0;

GLfloat upperFold = 0.0f; // 윗팔 접는 각도
GLfloat lowerFold = 20.0f;// 아랫팔 접는 각도
GLfloat zoom = 70.0f;
GLfloat m = 10, k = 20, i = 10;
GLfloat eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz = 0.0f;
GLboolean flag = false;
GLboolean xParallelFlag, yParallelFlag, zParallelFlag = false;

int boxIndex = 0;

GLint boxFinished[3] = { 0, 0, 0 };
GLint boxVisible[3] = { 1, 1, 1 };
GLfloat boxTheta[3] = { 0 };

GLint isSelected = 0;


void RotateY1(int value)
{

	//float temp1=ViewX, temp2=ViewZ;
	//printf("%f %f \n" ,temp1, temp2);
	ViewY -= 0.01;
	//ViewX=temp1*cos(0.05)-temp2*sin(0.05);
	//ViewZ=temp1*sin(0.05)+temp2*cos(0.05);
	//printf("%f %f \n" ,temp1, temp2);
	if (ViewMinus3 == 1)
	{
		glutTimerFunc(40, RotateY1, 1);
	}

	glutPostRedisplay();
}
void RotateY2(int value)
{

	//float temp1=ViewX, temp2=ViewZ;
	//printf("%f %f \n" ,temp1, temp2);
	ViewY += 0.01;
	//ViewX=temp1*cos(0.05)-temp2*sin(0.05);
	//ViewZ=temp1*sin(0.05)+temp2*cos(0.05);
	//printf("%f %f \n" ,temp1, temp2);
	if (ViewMinus3 == 1)
	{
		glutTimerFunc(40, RotateY2, 1);
	}

	glutPostRedisplay();
}

void Rotate3(int value) //회전 반시계방향
{
	float temp1 = ViewX, temp2 = ViewZ;
	printf("%f %f \n", temp1, temp2);
	ViewX = temp1*cos(0.012) - temp2*sin(0.012);
	ViewZ = temp1*sin(0.012) + temp2*cos(0.012);
	//printf("%f %f \n" ,temp1, temp2);
	if (ViewMinus1 == 1)
	{
		glutTimerFunc(40, Rotate3, 1);
	}

	glutPostRedisplay();


}

void Rotate4(int value) //회전 시계방향 움지길때
{
	float temp1 = ViewX, temp2 = ViewZ;
	printf("%f %f \n", temp1, temp2);
	ViewX = temp1*cos(0.013) + temp2*sin(0.013);
	ViewZ = -temp1*sin(0.013) + temp2*cos(0.013);
	//printf("%f %f \n" ,temp1, temp2);
	if (ViewMinus2 == 1)
	{
		glutTimerFunc(40, Rotate4, 1);
	}

	glutPostRedisplay();


}

void Rotate1(int value) //회전 반시계방향
{
	float temp1 = ViewX, temp2 = ViewZ;
	printf("%f %f \n", temp1, temp2);
	//sin 0 1 cos 0 0
	ViewX = temp1*cos(0.05) - temp2*sin(0.05);
	ViewZ = temp1*sin(0.05) + temp2*cos(0.05);
	//printf("%f %f \n" ,temp1, temp2);
	if (ViewMinus1 == 1)
	{
		glutTimerFunc(40, Rotate1, 1);
	}

	glutPostRedisplay();


}

void Rotate2(int value) //회전 시계방향
{
	float temp1 = ViewX, temp2 = ViewZ;
	printf("%f %f \n", temp1, temp2);
	ViewX = temp1*cos(0.05) + temp2*sin(0.05);
	ViewZ = -temp1*sin(0.05) + temp2*cos(0.05);
	//printf("%f %f \n" ,temp1, temp2);
	if (ViewMinus2 == 1)
	{
		glutTimerFunc(40, Rotate2, 1);
	}

	glutPostRedisplay();


}
void CreateRandomizedBox() {

	int i = 0;
	int valid = 0;
	int value = 0;
	int temp[3] = { 0 };

	srand(time(NULL));

	do {

		valid = 1;
		value = rand() % 180;

		for (i = 0; i < 3; i++)
		{
			if (-20 < temp[i] - value && temp[i] - value < 20){

				valid = 0;
			}
		}

		if (valid == 0)
			continue;

		temp[boxIndex++] = value;

	} while (boxIndex < 3);

	for (i = 0; i < 3; i++) {

		boxTheta[i] = (GLfloat)temp[i] - 180;
	}

	//	cout << "delta: " << delta << endl;
	//	cout << "delta + 180: " << delta + 180 << endl;

	boxIndex = 0;
}

GLint level = 0;
GLint isGrabbing = 0;
GLint isAnimating = 0;
GLint target;


void drawBottom(int id, double degree)
{
	glLoadName(id);
	glPushMatrix();
	// glColor3f(id / (float)200, id / (float)200, id / (float)200);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 1, 0.3);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glRotatef(degree, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 3.0f, 0.0f);
	// gluCylinder(cylinder, 0.6f, 0.6f, 0.2, 4, 1);
	gluDisk(cylinder, 0.0f, 0.6f, 4, 1);
	glPopMatrix();
}



GLfloat targetTheta = -10000;

// 인덱스가 가리키는 상자를 특정 각도로 이동시킵니다.
// index: 상자 인덱스.
// theta: 이동하고자 하는 각도.
void AnimateArm(int index) {

	if (boxVisible[index] == 0 && level == 0) {

		isAnimating = 0;
		return;
	}

	//	cout << "delta: " << delta << endl;

	target = index;

	if (isAnimating == 0)
		return;

	// 특정 위치로 이동하며 팔 각도 초기화.
	// lowerFold: 20도 유지.
	// upperFold: 0도 유지.
	if (level == 0 && (upperFold != 20 || lowerFold != 0)) {

		if (upperFold > 20)
			upperFold -= 1;
		else if (upperFold < 20)
			upperFold += 1;

		if (lowerFold > 0)
			lowerFold -= 1;
		else if (lowerFold < 0)
			lowerFold += 1;
	}
	else {

		if (level < 1)
			level = 1;

		// 전체 각도 조절.
		if (level == 1 && delta != boxTheta[target] + 180) {

			if (delta > boxTheta[target] + 180)
			{
				delta -= 1.0;
				if (Select_View == 4)
				{
					
					Rotate3(1);
					ViewMinus1 = 0;
					ViewMinus2 = 0;
					ViewMinus3 = 0;
				}
			}

			if (delta < boxTheta[target] + 180)
			{
				delta += 1.0;
				if (Select_View == 4)
				{

					Rotate4(1);
					ViewMinus1 = 0;
					ViewMinus2 = 0;
					ViewMinus3 = 0;
				}
			}
		}
		else {

			if (level < 2)
				level = 2;

			if (level == 2 && lowerFold < 60) {

				lowerFold += 1;
				if (Select_View == 4)
				{
					RotateY2(1);
					ViewMinus3 = 0;
				}
			}
			else {

				if (level < 3)
					level = 3;

				// 상부 팔 각도 조절 - 내리기.
				if (level == 3 && upperFold < 60) {

					upperFold += 1;
					if (Select_View == 4)
					{
						RotateY1(1);
						ViewMinus3 = 0;
					}
				}
				else {

					if (level < 4)
						level = 4;

					// 박스를 잡았다고 표시하며 주운 박스를 보이지 않도록 설정.
					// 보이지 않은 상태에서는 해당 상자를 목적 위치로 이동하여 다음 모션 준비.
					isGrabbing = 1;
					boxVisible[target] = 0;

					// 하부 팔 각도 조절 - 올리기.
					if (level == 4 && lowerFold > 0) {

						lowerFold -= 1;
						if (Select_View == 4)
						{
							RotateY1(1);
							ViewMinus3 = 0;
						}
					}
					else {

						if (level < 5)
							level = 5;

						// 이동할 지점을 선택하지 않은 경우에는 진행하지 못하도록 막는다.
						if (level == 5 && isSelected == 0) {


							glutPostRedisplay();
							glutTimerFunc(10, AnimateArm, index);

							return;
						}
						else {

							if (level < 6)
								level = 6;

							// 목적 위치 지정.
							// 목적 위치로 이동.

							boxTheta[target] = targetTheta - 180;

							if (level == 6 && delta != targetTheta) {

								if (delta > targetTheta){
									delta -= 1.0;
									if (Select_View == 4)
									{
										Rotate3(1);
										ViewMinus1 = 0;
										ViewMinus2 = 0;
										ViewMinus3 = 0;
									}
								}

								if (delta < targetTheta)
								{
									delta += 1.0;
									if (Select_View == 4)
									{
										Rotate4(1);
										ViewMinus1 = 0;
										ViewMinus2 = 0;
										ViewMinus3 = 0;
									}
								}

								if (delta > 180)
									delta -= 360;

								//							cout << "TargetTheta: " << targetTheta << endl;
							}
							else {

								if (level < 7)
									level = 7;

								// 내려놓기 위해서 하부 팔 각도 조절
								if (level == 7 && lowerFold < 60) {

									lowerFold += 1;
									if (Select_View == 4)
									{
										RotateY2(1);
										ViewMinus3 = 0;
									}
								}
								else {

									if (level < 8)
										level = 8;

									// 박스를 내려놓은 상태이므로 해당 위치의 박스를 보이도록 설정.
									isGrabbing = 0;
									boxVisible[target] = 1;

									// 상부 팔 각도 조절 - 올리기.
									if (level == 8 && lowerFold > 0) {

										lowerFold -= 1.0;
										if (Select_View == 4)
										{
											RotateY1(1);
											ViewMinus3 = 0;
										}
									}
									else {

										if (level < 9)
											level = 9;

										if (level == 9 && upperFold > 20) {

											upperFold -= 1;
										}
										else {

											isAnimating = 0;
											isSelected = 0;
											level = 0;

											glutPostRedisplay();

											return;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(10, AnimateArm, index);
}

void AnimateArmWrapper(int index, GLfloat theta) {

	isAnimating = 1;
	level = 0;

	// targetTheta = theta;
	AnimateArm(index);
}

#define BUFSIZE 4096
void MyMouse(int button, int state, int x, int y) {

	if (state != GLUT_DOWN)
		return;

	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(BUFSIZE, selectBuf);

	glRenderMode(GL_SELECT); // Enter the SELECT render mode
	glInitNames();
	glPushName(-1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 5.0, 5.0, viewport);
	gluPerspective(zoom, 1.0, 0.0, 40.0);
	glMatrixMode(GL_MODELVIEW);

	draw(GL_SELECT);
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);

	cout << "Hit!!!!!!! : " << hits << "\n";

	for (int i = 0; i < 10; i++)
		cout << i << ". " << selectBuf[i] << "\n";

	if (button == GLUT_RIGHT_BUTTON) {

		if (level == 5) {

			if (100 <= selectBuf[7] && selectBuf[7] <= 100 + COUNT || 100 <= selectBuf[3] && selectBuf[3] <= 100 + COUNT) {
				if (100 <= selectBuf[7] && selectBuf[7] <= 100 + COUNT)
					targetTheta = bottomTheta[selectBuf[7] - 100];
				else
					targetTheta = bottomTheta[selectBuf[3] - 100];
				isSelected = 1;
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON) {

		if ((0 <= selectBuf[7] && selectBuf[7] <= 2) || ((0 <= selectBuf[3] && selectBuf[3] <= 2))  && level < 5)
			if (0 <= selectBuf[7] && selectBuf[7] <= 2)
				AnimateArmWrapper(selectBuf[7], targetTheta);
			else
				AnimateArmWrapper(selectBuf[3], targetTheta);
	}


}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {

	if (KeyPressed == '4')
		isAnimating = 0;

	if (KeyPressed == '5') {

		isSelected = 1;
		targetTheta = -50;
	}

	if (isAnimating == 1)
		return;

	// shift 눌렀을 때
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT){

		switch (KeyPressed){

		case 'S': case 's': if (-90 <= upperFold && upperFold < 90) upperFold += 1; break; // 윗팔 펴기
		case 'W': case 'w': if (-90 < upperFold && upperFold <= 90) upperFold -= 1; break; // 윗팔 접기

		default: break;
		}
	}
	else {

		switch (KeyPressed){

			/* Special key */
		case 'Q': case 'q': case 27: exit(0); break; // 종료(27 = 'esc' ASCII code)

			/* Rotate key */
		case 'A': case 'a': delta += 1; break; // 화면중심으로 왼쪽으로 회전
		case 'D': case 'd': delta -= 1; break; // 화면중심으로 오른쪽으로 회전

			/* Arm fold key */
		case 'S': case 's': if (-90 <= lowerFold && lowerFold < 90) lowerFold += 1; break; // 아랫팔 펴기
		case 'W': case 'w': if (-90 < lowerFold && lowerFold <= 90) lowerFold -= 1; break; // 아랫팔 접기

		case '1': AnimateArmWrapper(0, -30); break;
		case '2': AnimateArmWrapper(1, -60); break;
		case '3': AnimateArmWrapper(2, -90); break;

			/* Perspective Projection (원근투상) */
		case 'T': case 't': zoom -= 1; break;
		case 'G': case 'g': zoom += 1; break;

			/* Parallel Projection (평행투상) */

			// x축에서 봤을 때
		case 'X': case 'x':
			xParallelFlag = true;
			eyex = 1.0f, eyey = 0.0f, eyez = 0.0f, upx = 0.0f, upy = 1.0f, upz = 0.0f;
			break;

			// y축에서 봤을 때
		case 'Y': case 'y':
			yParallelFlag = true;
			eyex = 0.0f, eyey = 1.0f, eyez = 0.0f, upx = 0.0f, upy = 0.0f, upz = -1.0f;
			break;

			// z축에서 봤을 때 
		case 'Z': case 'z':
			zParallelFlag = true;
			eyex = 0.0f, eyey = 0.0f, eyez = 1.0f, upx = 0.0f, upy = 1.0f, upz = 0.0f;
			break;

		case 'V': case 'v':

			xParallelFlag = false;
			yParallelFlag = false;
			zParallelFlag = false;
			break;

		case 'M': case 'm':
			m += 1;
			break;

		case 'N': case 'n':
			m -= 1;
			break;

		case 'K': case 'k':
			k += 1;
			break;

		case 'J': case 'j':
			k -= 1;
			break;

		case 'I': case 'i':
			i += 1;
			break;

		case 'U': case 'u':
			i -= 1;
			break;

		case '-':

			//Rotate();
			ViewMinus2 = 0;
			if (ViewMinus1 == 1)
				ViewMinus1 = 0;
			else ViewMinus1 = 1;
			glutTimerFunc(10, Rotate1, 1);
			break;

		case '=':

			//Rotate();
			ViewMinus1 = 0;
			if (ViewMinus2 == 1)
				ViewMinus2 = 0;
			else ViewMinus2 = 1;
			glutTimerFunc(10, Rotate2, 1);
			break;
		case '7':
			ViewY += 1.0;
			break;
		case '8':
			ViewY -= 1.0;
			break;

		case '0':
			//0.0, -10.0, -50.0
			if (View4SelectCheck == 0)
			{
				ViewX = 0.0;
				ViewY = -10.0;
				ViewZ = 12.0;
				ViewZ = ViewZ;
				Select_View = 4;
			}
			else
			{
				ViewX = ViewTemp1;
				ViewY = ViewTemp2;
				ViewZ = ViewTemp3;
				Select_View = 4;
			}
			break;
		case '9':
			Select_View = 1;
			ViewTemp1 = ViewX;
			ViewTemp2 = ViewY;
			ViewTemp3 = ViewZ;
			// ViewX = 0.0;
			//float ViewY = 0.0;
			//float ViewZ = 50.0;
			ViewX = 0.0;
			ViewY = 0.0;
			ViewZ = 50.0;
			break;

		default: break;
		}
	}

	glutPostRedisplay();
}

AUX_RGBImageRec *LoadBMPFile(char *filename)
{
	FILE *hFile = NULL;
	if (!filename) return NULL;

	hFile = fopen(filename, "r");
	if (hFile) {
		fclose(hFile);

		wchar_t wtext[20];
		mbstowcs(wtext, filename, strlen(filename) + 1);//Plus null
		LPWSTR ptr = wtext;
		return auxDIBImageLoad(ptr);
	}

	return NULL;
}
void DrawWall(){

	glColor3f(1, 1, 1);


	/* Front Door */
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(50.0, 20.0, 0);
	glTexCoord2f(0, 1); glVertex3f(-50.0, 20.0, 0);
	glTexCoord2f(0, 0); glVertex3f(-50.0, -20.0, 0);
	glTexCoord2f(1, 0); glVertex3f(50.0, -20.0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>

	/* Right Wall */
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);	glVertex3f(50.0, 20.0, 0);
	glTexCoord2f(1, 1);	glVertex3f(50.0, -20.0, 0);
	glTexCoord2f(0, 1);	glVertex3f(50.0, -20.0, -100.0);
	glTexCoord2f(0, 0);	glVertex3f(50.0, 20.0, -100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>

	/* Left Wall */
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 0); glVertex3f(-50.0, 20.0, 0);
	glTexCoord2f(0, 0); glVertex3f(-50.0, -20.0, 0);
	glTexCoord2f(0, 1); glVertex3f(-50.0, -20.0, -100.0);
	glTexCoord2f(1, 1); glVertex3f(-50.0, 20.0, -100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>

	/* Bottom Wall */
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(50.0, -20.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-50.0, -20.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(-50.0, -20.0, -100.0);
	glTexCoord2f(1, 0); glVertex3f(50.0, -20.0, -100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>

	/* Top Wall */
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(50.0, 20.0, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-50.0, 20.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(-50.0, 20.0, -100.0);
	glTexCoord2f(1, 0); glVertex3f(50.0, 20.0, -100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>


	/* End Door */
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(50.0, 20.0, -100.0);
	glTexCoord2f(0, 1); glVertex3f(-50.0, 20.0, -100.0);
	glTexCoord2f(0, 0); glVertex3f(-50.0, -20.0, -100.0);
	glTexCoord2f(1, 0); glVertex3f(50.0, -20.0, -100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>
}

void Base(){

	glColor3f(0.73f, 0.56f, 0.56f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[4]);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, GL_TRUE);
	gluCylinder(cylinder, 1.0f, 1.0f, 0.3f, 4, 1);
	glTranslatef(0.0f, 0.0f, 0.3f);
	glColor3f(0.80f, 0.36f, 0.36f);
	gluCylinder(cylinder, 1.0f, 0.7f, 0.2f, 4, 1);
	gluDisk(cylinder, 0.0f, 0.7f, 4, 1);
	glTranslatef(0.0f, 0.0f, 0.2f);

	glColor3f(0.73f, 0.56f, 0.56f);
	glRotatef(delta, 0.0f, 0.0f, 1.0f);
	gluCylinder(cylinder, 0.5f, 0.5f, 0.3f, 30, 1);
	glTranslatef(0.0f, 0.0f, 0.3f);
	glColor3f(0.80f, 0.36f, 0.36f);
	gluDisk(cylinder, 0.0f, 0.5f, 30, 1);
	glTranslatef(0.0f, 0.0f, -0.3f);

	glDisable(GL_TEXTURE_2D);
}


void Box() {

	int i = 0;

	glColor3f(1.0f, 1.0f, 1.0f);

	for (i = 0; i < 3; i++) {
		glLoadName(i);
		glPushMatrix();
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		glRotatef(boxTheta[i], 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 3.0f, 0.0f);

		if (boxVisible[i] == 1){

			glScalef(0.7, 0.7, 0.7);

			//Multi-colored side - FRONT
			glEnable(GL_TEXTURE_2D); //<11>
			glBindTexture(GL_TEXTURE_2D, tex[3]);
			glBegin(GL_POLYGON);

			//glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, -0.5);      // P1 is red
			glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);      // P2 is green
			glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);      // P3 is blue
			glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);      // P4 is purple

			glEnd();

			// Purple side - RIGHT
			glBegin(GL_POLYGON);
			//glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, -0.5);
			glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);
			glTexCoord2f(1, 1); glVertex3f(0.5, 0.5, 0.5);
			glTexCoord2f(1, 0); glVertex3f(0.5, -0.5, 0.5);
			glEnd();

			// Green side - LEFT
			glBegin(GL_POLYGON);
			//glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(0, 1); glVertex3f(-0.5, 0.5, 0.5);
			glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);
			glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);
			glEnd();

			// Blue side - TOP
			glBegin(GL_POLYGON);
			//glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(0, 0); glVertex3f(0.5, 0.5, 0.5);
			glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);
			glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);
			glTexCoord2f(1, 0); glVertex3f(-0.5, 0.5, 0.5);
			glEnd();


			// Red side - BOTTOM

			glEnable(GL_TEXTURE_2D); //<11>
			//glColor3f(1.0, 1.0, 1.0);
			glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, -0.5);
			glTexCoord2f(0, 1); glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);
			glEnd();

			// White side - BACK
			glBegin(GL_POLYGON);
			//glColor3f(1.0, 1.0, 0.0);
			glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, 0.5);
			glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, 0.5);
			glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, 0.5);
			glEnd();
			glDisable(GL_TEXTURE_2D); //<11>
		}

		glPopMatrix();

	}
}

void LowerArm(){
	glColor3f(0.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, 0.3);
	glRotatef(lowerFold, 1.0f, 0.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, GL_TRUE);
	gluCylinder(cylinder, 0.1, 0.1, 1.5, 30, 1);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 0.0, 1.0);
	//gluDisk(cylinder, 0.0f, 0.1f, 30, 1);
}


void UpperArm(){
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 1.5);
	glRotatef(upperFold, 1.0f, 0.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, GL_TRUE);
	gluCylinder(cylinder, 0.1, 0.1, 1.5, 30, 1);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 0.0, 1.0);
	//gluDisk(cylinder, 0.0f, 0.1f, 30, 1);
}

void Nipper(){

	glColor3f(0.73f, 0.56f, 0.56f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, GL_TRUE);
	gluCylinder(cylinder, 0.1, 0.1, 1.0, 30, 1);
	glColor3f(1.0, 1.0, 0.2);
	gluDisk(cylinder, 0.0f, 0.05f, 30, 1);
	glTranslatef(0.0f, 0.0f, 1.0);
	gluDisk(cylinder, 0.0f, 0.05f, 30, 1);
	glDisable(GL_TEXTURE_2D);
}

void Fork(){
	glColor3f(0.73f, 0.56f, 0.56f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, GL_TRUE);
	gluCylinder(cylinder, 0.1, 0.1, 0.4, 30, 1);
	glDisable(GL_TEXTURE_2D);
}

void FakeBox() {
	glTranslatef(-0.3f, 0.2f, 0.4f);
	glRotatef(45, 0.0f, 0.0f, 1.0f);

	glScalef(0.7, 0.7, 0.7);

	glColor3f(1.0, 1.0, 1.0);
	//Multi-colored side - FRONT
	glEnable(GL_TEXTURE_2D); //<11>
	glBindTexture(GL_TEXTURE_2D, tex[3]);
	glBegin(GL_POLYGON);

	//glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, -0.5);      // P1 is red
	glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);      // P2 is green
	glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);      // P3 is blue
	glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);      // P4 is purple

	glEnd();


	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	//glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(1, 1); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(1, 0); glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	// Green side - LEFT
	glBegin(GL_POLYGON);
	//glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0, 1); glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	// Blue side - TOP
	glBegin(GL_POLYGON);
	//glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0, 0); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1, 0); glVertex3f(-0.5, 0.5, 0.5);
	glEnd();


	// Red side - BOTTOM

	glEnable(GL_TEXTURE_2D); //<11>
	//glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0, 1); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	// White side - BACK
	glBegin(GL_POLYGON);
	//glColor3f(1.0, 1.0, 0.0);
	glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D); //<11>
}

void draw(GLenum mode) {
	DrawWall();
	glPushMatrix();
	glTranslatef(0.0, -10.0, -30.0);
	glScalef(3.0, 3.0, 3.0);
	glPushMatrix();

	glTranslatef(0.0, -5.0, -5.0);
	glScalef(3.0, 3.0, 3.0);

	if (level >= 5) {

		// 상자: -10 ~ -180
		for (int i = 0; i < COUNT / 2; i++)
			drawBottom(100 + i, i * 10 + 180);

		// 상자: 
		for (int i = COUNT / 2; i < COUNT; i++)
			drawBottom(100 + i, -i * 10);
	}

	glPushMatrix();
	glTranslatef(0.0, 0.4, 0.0);
	Box();

	glPopMatrix();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0, -5.0, -5.0);
	glScalef(3.0, 3.0, 3.0);

	glPushMatrix();

	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 3.0f, 0.0f);

	glPopMatrix();

	glPopMatrix();

	/* Mother robot */
	glPushMatrix();

	glTranslatef(0.0, -5.0, -5.0);
	glScalef(3.0, 3.0, 3.0);

	glPushMatrix();

	Base();

	glPushMatrix();

	LowerArm();

	glPushMatrix();

	UpperArm();

	glPushMatrix();

	glRotatef(90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -0.5, 1.5);

	glRotatef(90, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);

	Nipper();

	glPushMatrix();

	glRotatef(-90, 0.0f, 1.0f, 0.0f);
	glRotatef(-45, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.03, -0.03, 0.0);

	Fork();
	glTranslatef(0.0f, 0.0f, 0.2);

	glPushMatrix();

	glPushMatrix();

	if (isGrabbing == 1) {

		glColor3f(1, 1, 0);
		glTranslatef(0.0f, -0.6f, 0.0f);

		FakeBox();
	}

	glPopMatrix();

	glTranslatef(-0.65, -0.65, -0.2);
	Fork();
	glTranslatef(0.0f, 0.0f, 0.2);

	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}

void MyMainMenu(int entryID){


	if (entryID == 3) exit(0);
	exit(0); 		//프로그램 종료
	glutPostRedisplay();
}

void MySubMenu(int entryID){
	if (entryID == 1)
		//IsSmall = true;  	//작은 크기로 그리기
		//-
	{
		ViewMinus2 = 0;
		if (ViewMinus1 == 1)
			ViewMinus1 = 0;
		else ViewMinus1 = 1;
		glutTimerFunc(10, Rotate1, 1);
	}
	else if (entryID == 2)
		//IsSmall = false;	//큰 크기로 그리기
		//+
	{
		ViewMinus1 = 0;
		if (ViewMinus2 == 1)
			ViewMinus2 = 0;
		else ViewMinus2 = 1;
		glutTimerFunc(10, Rotate2, 1);

	}
	glutPostRedisplay();
}
void MySubMenu2(int entryID){
	if (entryID == 1){
		Select_View = 1;
		ViewTemp1 = ViewX;
		ViewTemp2 = ViewY;
		ViewTemp3 = ViewZ;
		// ViewX = 0.0;
		//float ViewY = 0.0;
		//float ViewZ = 50.0;
		ViewX = 0.0;
		ViewY = 0.0;
		ViewZ = 50.0;
	}
	else if (entryID == 2)
	{

		if (View4SelectCheck == 0)
		{
			ViewX = 0.0;
			ViewY = -10.0;
			ViewZ = 12.0;
			ViewZ = ViewZ;
			Select_View = 4;
		}
		else
		{
			ViewX = ViewTemp1;
			ViewY = ViewTemp2;
			ViewZ = ViewTemp3;
			Select_View = 4;
		}
	}
	glutPostRedisplay();
}

void MyDisplay(){

	int i = 0;


	glViewport(0, 0, Width, Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, 1.0, 0.0, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	if (xParallelFlag || yParallelFlag || zParallelFlag)
		gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	else//정진
	{
		if (Select_View == 1) gluLookAt(ViewX, ViewY, ViewZ - 50.0, 0.0, -10.0, -50.0, 0.0, 1.0, 0.0); //기본 뷰
		
		else if (Select_View == 4){
			gluLookAt(0.0, 10.0, -50.0, ViewX, ViewY, ViewZ - 50.0, 0.0, 1.0, 0.0);
		}
		
	}
	draw(GL_RENDER);

	glutSwapBuffers();
	glFlush();
}

void MyReshape(int w, int h) {
	Width = w; Height = h;
}

int main(int argc, char** argv) {

	CreateRandomizedBox();
	Width = 500; Height = 500;


	// 각도초기화.
	for (int i = 0; i < COUNT / 2; i++) {

		bottomTheta[i] = i * 10;
	}

	for (int i = COUNT / 2; i < COUNT; i++) {

		bottomTheta[i] = 170 - i * 10;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Robot Arm");


	glClearColor(1.0, 1.0, 1, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	/* 전면제거 */
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glOrtho(-20.0, 20.0, -50.0, 50.0, -100.0, 100.0);

	cylinder = gluNewQuadric(); 		        /* allocate quadric object */
	gluQuadricDrawStyle(cylinder, GLU_FILL); 	/* render it as wireframe */

	box = gluNewQuadric();
	gluQuadricDrawStyle(box, GLU_FILL);

	AUX_RGBImageRec *texRec[6]; //<1>
	memset(texRec, 0, sizeof(void *)* 6); //<2>

	if ((texRec[0] = LoadBMPFile("texture1.bmp")) && //<3>
		(texRec[1] = LoadBMPFile("texture2.bmp")) &&
		(texRec[2] = LoadBMPFile("texture3.bmp")) &&
		(texRec[3] = LoadBMPFile("texture4.bmp")) &&
		(texRec[4] = LoadBMPFile("texture5.bmp")) &&
		(texRec[5] = LoadBMPFile("texture6.bmp"))) {
			glGenTextures(5, &tex[0]); //<4>
			for (int i = 0; i < 6; i++) { //<5>
				glBindTexture(GL_TEXTURE_2D, tex[i]); //<6>
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //<7>
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //<8>
				glTexImage2D(GL_TEXTURE_2D, 0, 4, texRec[i]->sizeX, texRec[i]->sizeY, 0,
					GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data); //<9>
			}
	}
	else return FALSE;

	for (int i = 0; i < 6; i++) { //<10>
		if (texRec[i]) {
			if (texRec[i]->data) free(texRec[i]->data);
			free(texRec[i]);
		}
		else return FALSE;
	}


	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //<12>


	// menu
	
	GLint MySubMenuID = glutCreateMenu(MySubMenu);
	glutAddMenuEntry("시계방향(시작/멈춤)", 1);
	glutAddMenuEntry("반시계방향(시작/멈춤)", 2);

	GLint MySubMenuID2 = glutCreateMenu(MySubMenu2);
	glutAddMenuEntry("기본", 1);
	glutAddMenuEntry("집는 방향", 2);
	


	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("시점 변환", MySubMenuID2);
	glutAddSubMenu("회전", MySubMenuID);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	
	glutDisplayFunc(MyDisplay);
	//	MyLightInit();
	glutReshapeFunc(MyReshape);
	glutKeyboardFunc(MyKeyboard);
	glutMouseFunc(MyMouse);
	glutMainLoop();

	return 0;
}