#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Rendering.h"

static float  Normal[][4] = { { 0.0f, 0.0f, -1.0f },{ 0.0f, -1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f},{ -1.0f, 0.0f, 0.0f } };

static float points[][3] = { { -0.5, -0.5, -0.5 },{ 0.5, -0.5, -0.5 },
{ 0.5, 0.5, -0.5 },{ -0.5, 0.5, -0.5 },
{ -0.5, -0.5, 0.5 },{ 0.5, -0.5, 0.5 },
{ 0.5, 0.5, 0.5 },{ -0.5, 0.5, 0.5 } };

static int face[][4] = { { 0, 3, 2, 1 },{ 0, 1, 5, 4 },{ 1, 2, 6, 5 },
{ 4, 5, 6, 7 },{ 2, 3, 7, 6 },{ 0, 4, 7, 3 } };

static float  flr_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
static float  flr_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static float  flr_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
static float  flr_shininess = 16.0;
static float  lit1_position[] = { 100.0f, 20.0f, 100.0f, 1.00f }; /*point light*/
static float  lit1_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
static float  lit1_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f};
static float  global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
static GLUquadricObj* sphere = NULL, * cylinder = NULL, * Circle;
static float ort_ang = 150.0f, per_ang = 45.0f;
static float eye[3] = { 100.0f, 25.0f, 100.0f }, focus[3] = { 100.0f, 0.0f, 100.0f }, V_up[3] = { 0.0f, 0.0f, -1.0f };
static float fresh_ball_diffuse[4] = { 0.4f, 0.6f, 0.05f, 1.0f };//light green
static float fresh_ball_ambient[4] = { 0.01f, 0.4f, 0.01f,1.0f };
static float fresh_ball_specular[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
static float fresh_ball_shiness = 64.0f;
static float red_ball_diffuse[4] = { 0.5f, 0.01f, 0.01f, 1.0f };
static float red_ball_ambient[4] = { 0.4f, 0.01f, 0.01f,1.0f };
static float red_ball_specular[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
static float red_ball_shiness = 64.0f;
static float blue_ball_diffuse[4] = { 0.1f, 0.15f, 0.6f, 1.0f };
static float blue_ball_ambient[4] = { 0.01f, 0.01f, 0.4f,1.0f };
static float blue_ball_specular[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
static float blue_ball_shiness = 64.0f;
static float green_ball_diffuse[4] = { 0.1f, 0.6f, 0.05f, 1.0f };
static float green_ball_ambient[4] = { 0.01f, 0.4f, 0.01f,1.0f };
static float green_ball_specular[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
static float green_ball_shiness = 64.0f;
static float pink_ball_diffuse[4] = { 0.6f, 0.4f, 0.1f, 1.0f };
static float pink_ball_ambient[4] = { 0.4f, 0.01f, 0.01f,1.0f };
static float pink_ball_specular[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
static float pink_ball_shiness = 64.0f;
static float skyblue_ball_diffuse[4] = { 0.1f, 0.4f, 0.6f, 1.0f };//light blue
static float skyblue_ball_ambient[4] = { 0.01f, 0.01f, 0.4f,1.0f };
static float skyblue_ball_specular[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
static float skyblue_ball_shiness = 64.0f;
static float metal2_ambient[] = { 0.247250, 0.224500, 0.064500, 1.000000 };/*Gold*/
static float metal2_diffuse[] = { 0.346150, 0.314300, 0.090300, 1.000000 };
static float metal2_specular[] = { 0.797357, 0.723991, 0.208006, 1.000000 };
static float metal2_shininess = 40.199997;
static float mat1_ambient[] = { 0.174500, 0.011750, 0.011750, 1 };/*紅寶石*/
static float mat1_diffuse[] = { 0.614240, 0.041360, 0.041360, 1 };
static float mat1_specular[] = { 0.727811, 0.626959, 0.626959, 1 };
static float mat1_emission[] = { 0.4, 0.4, 0.8, 1.0 };
static float mat1_shininess = 5;
static unsigned int Canves[2];
static unsigned char Cindex;
static unsigned char Crate;

unsigned char  checkboard[TSIZE2][TSIZE2][4];   /* checkboard textures **/
unsigned char  tree[TSIZE][TSIZE][4];   /* tree textures */
unsigned char  texture[TSIZE][TSIZE][4];   /*  textures */
unsigned char  brick[TSIZE][TSIZE][4];        /* brick wall textures */
unsigned char  circle[TSIZE][TSIZE][4];
unsigned char  waterwave[TSIZE][TSIZE][4];
unsigned char  star[TSIZE][TSIZE][4];
unsigned char  land[TSIZE][TSIZE][4];
unsigned char Maze[201][201];
unsigned int textName[3];                   /* declare two texture maps*/
unsigned int TextureID[4];
ball Ball[12];
int NumofBalls = 12;

void InitSpeed(ball* object, float x, float y, float z)
{
	object->speed[0] = x;
	object->speed[1] = y;
	object->speed[2] = z;
}
void Location(ball* object, float x, float y, float z)
{
	object->xyz[0] = x;
	object->xyz[1] = y;
	object->xyz[2] = z;
}
void LUT()//初始化obstacle的位置
{
	for (int i = 0; i <= 200; i++)//一開始全部為100
		for (int j = 0; j <= 200; j++)
			Maze[i][j] = 100;
	for(int i=6;i<=10;i++)
		for (int j = 192; j <= 200; j++)
			Maze[j][i] = 255;
	for (int i = 0; i <= 4; i++)
		for (int j = 182; j <= 186; j++)
			Maze[j][i] = 255;
	for (int i = 48; i <= 52; i++)
		for (int j = 73; j <= 127; j++)
			Maze[j][i] = 255;
	for (int i = 73; i <= 127; i++)
		for (int j = 46; j <= 50; j++)
			Maze[j][i] = 255;
	for (int i = 148; i <= 152; i++)
		for (int j = 73; j <= 127; j++)
			Maze[j][i] = 255;
	for (int i = 73; i <= 127; i++)
		for (int j = 148; j <= 152; j++)
			Maze[j][i] = 255;
	for (int i = 148; i <= 152; i++)
		for (int j = 73; j <= 127; j++)
			Maze[j][i] = 255;
	for (int i = 64; i <= 72; i++)
		for (int j = 60; j <= 64; j++)
			Maze[j][i] = 255;
	for (int i = 128; i <= 136; i++)
		for (int j = 62; j <= 66; j++)
			Maze[j][i] = 255;
	for (int i = 132; i <= 136; i++)
		for (int j = 66; j <= 74; j++)
			Maze[j][i] = 255;
	for (int i = 64; i <= 68; i++)
		for (int j = 126; j <= 134; j++)
			Maze[j][i] = 255;
	for (int i = 64; i <= 72; i++)
		for (int j = 136; j <=140 ; j++)
			Maze[j][i] = 255;
	for (int i = 132; i <= 136; i++)
		for (int j = 132; j <= 140; j++)
			Maze[j][i] = 255;
	for (int i = 128; i <= 136; i++)
		for (int j = 136; j <= 140; j++)
			Maze[j][i] = 255;
	for (int i = 190; i <= 194; i++)
		for (int j = 0; j <= 8; j++)
			Maze[j][i] = 255;
	for (int i = 196; i <= 200; i++)
		for (int j = 14; j <= 18; j++)
			Maze[j][i] = 255;
	for (int i = 180; i <= 188; i++)
		for (int j = 14; j <= 18; j++)
			Maze[j][i] = 255;
	for (int i = 180; i <= 184; i++)
		for (int j = 6; j <= 14; j++)
			Maze[j][i] = 255;
	for (int i = 190; i <= 194; i++)
		for (int j = 25; j <= 35; j++)
			Maze[j][i] = 255;
	for (int i = 192; i <= 200; i++)
		for (int j = 33; j <= 37; j++)
			Maze[j][i] = 255;
}
void SetUpObject()
{
	LUT();
			
	for (int i = 0; i < NumofBalls; i++)
	{
		if (i < 2)		// 0 、 1 are predators
		{
			Ball[i].role = 0;
			Ball[i].R = 2.0f;
			if (i == 0)
			{
				Location(&Ball[i], 2.0f, 2.0f, 198.0f);
				InitSpeed(&Ball[i], 0.0f, 0.0f, -1.0f);
				Maze[2][198] = 0;
				Ball[i].Area = 0;
				Ball[i].Direction = 0;
				Ball[i].BottleNeck = 0;
				Ball[i].right = true;
			}	
			else
			{
				Maze[198][2] = 1;
				Location(&Ball[i], 198.0f, 2.0f, 2.0f);
				InitSpeed(&Ball[i], 0.0f, 0.0f, 1.0f);
				Ball[i].Area = 1;
				Ball[i].Direction = 2;
				Ball[i].BottleNeck = 0;
				Ball[i].right = true;
			}
		}
		else if (i < 4) // 2、3 are hovers
		{
			Ball[i].role = 1; 
			Ball[i].R = 2.0f;
			if (i == 2)
			{
				Maze[100][198] = 2;
				Location(&Ball[i], 100.0f, 2.0f, 198.0f);
				InitSpeed(&Ball[i], -1.0f, 0.0f, 0.0f);
				Ball[i].Area = 7;
				Ball[i].Direction = 3;
				Ball[i].BottleNeck = 0;
				Ball[i].right = false;
			}
			else
			{
				Maze[100][136] = 3;
				Location(&Ball[i], 100.0f, 2.0f, 136.0f);
				InitSpeed(&Ball[i], 1.0f, 0.0f, 0.0f);
				Ball[i].Area = 2;
				Ball[i].Direction = 1;
				Ball[i].BottleNeck = 0;
				Ball[i].right = true;
			}
				
		}
		else            // 4~ 11 are food
		{
			Ball[i].role = 2;
			Ball[i].R = 1.0f;
			InitSpeed(&Ball[i], 0.0f, 0.0f, 0.0f);
			if (i == 4)
			{
				Maze[100][100] = 4;
				Location(&Ball[i], 100.0f, 1.0f, 100.0f);
				Ball[i].Area = 2;
			}
		}			
	}
}
void draw_cube()
{
	int    i;
	glMaterialfv(GL_FRONT, GL_AMBIENT, metal2_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, metal2_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, metal2_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, metal2_shininess);
	for (i = 0; i < 6; i++) {
		glNormal3fv(Normal[i]);
		glBegin(GL_POLYGON);  /* Draw the face */
		glVertex3fv(points[face[i][0]]);
		glVertex3fv(points[face[i][1]]);
		glVertex3fv(points[face[i][2]]);
		glVertex3fv(points[face[i][3]]);
		glEnd();
	}
}
void ApplyTexture(float left, float right, float bottom, float top, float height, unsigned int TextureUnit)
{
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, TextureUnit);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse);  /*diffuse color */
	glMaterialfv(GL_FRONT, GL_AMBIENT, flr_ambient);
	glNormal3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(left, height, bottom);
	glTexCoord2f(1, 0);
	glVertex3f(left, height, top);
	glTexCoord2f(1, 1);
	glVertex3f(right, height, top);
	glTexCoord2f(0, 1);
	glVertex3f(right, height, bottom);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void draw_floor()
{
	ApplyTexture(0.0f, 200.0f, 0.0f, 200.0f, 0.0f, textName[0]);
	ApplyTexture(175.0f, 188.0f, 162.0f, 175.0f, 0.01f, Canves[Cindex]);
	ApplyTexture(62.5f, 52.0f, 113.0f, 87.0f, 0.01f, Canves[Cindex]);
	ApplyTexture(150.0f, 137.0f, 13.0f, 0.0f, 0.01f, Canves[Cindex]);
}
void CreateTextures()
{	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(3, textName);

	make_check();
	glBindTexture(GL_TEXTURE_2D, textName[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE2, TSIZE2, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, checkboard);

	brick_pattern();
	glBindTexture(GL_TEXTURE_2D, textName[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, brick);

	make_tree();
	glBindTexture(GL_TEXTURE_2D, textName[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, tree);

	int Width, Height, nrChannels;

	unsigned char* data = stbi_load("hole.png", &Width, &Height, &nrChannels, 0);
	glGenTextures(2, &Canves[0]);
	glBindTexture(GL_TEXTURE_2D, Canves[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	data = stbi_load("hole2.png", &Width, &Height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, Canves[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}
void Init()
{
	CreateTextures();
	SetUpObject();

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
	glEnable(GL_NORMALIZE);   /*Enable mornalization  */

	glLightfv(GL_LIGHT2, GL_DIFFUSE, lit1_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lit1_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, lit1_position);  /*fixed position in eye space---*/

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);
	glDisable(GL_LIGHTING);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */


	sphere = gluNewQuadric();
	cylinder = gluNewQuadric();
	Circle = gluNewQuadric();

	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricDrawStyle(Circle, GLU_FILL);
	gluQuadricNormals(Circle, GLU_SMOOTH);

	
}

void eye_location(int mode)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	switch (mode)
	{
	case 0:
		gluLookAt(eye[0], eye[1], eye[2], focus[0], focus[1], focus[2], V_up[0], V_up[1], V_up[2]);
		break;
	case 1:
	{
		float unit_vector[3] = { Ball[0].speed[0] ,Ball[0].speed[1] ,Ball[0].speed[2] };
		normalize(unit_vector);
		gluLookAt(Ball[0].xyz[0] - unit_vector[0] * 50.0f, 20.0f, Ball[0].xyz[2] - unit_vector[2] * 50.0f, Ball[0].xyz[0], Ball[0].xyz[1], Ball[0].xyz[2], 0.0f, 1.0f, 0.0f);
		break;
	}
	case 2:
	{
		float unit_vector[3] = { Ball[1].speed[0] ,Ball[1].speed[1] ,Ball[1].speed[2] };
		normalize(unit_vector);
		gluLookAt(Ball[1].xyz[0] - unit_vector[0] * 50.0f, 20.0f, Ball[1].xyz[2] - unit_vector[2] * 50.0f, Ball[1].xyz[0], Ball[1].xyz[1], Ball[1].xyz[2], 0.0f, 1.0f, 0.0f);
		break;
	}
	}
}

void projection(int mode)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix();
	switch (mode)
	{
	case 0:
		if (width < height)
			glOrtho(-1 * ort_ang, ort_ang, -1 * ort_ang * (float)height / (float)width,
				ort_ang * (float)height / (float)width,
				-0.0, 100.0);
		else
			glOrtho(-1 * ort_ang * (float)width / (float)height,
				ort_ang * (float)width / (float)height, -1 * ort_ang, ort_ang,
				-0.0, 100.0);
		break;
	case 1:
		gluPerspective(per_ang, (double)width / (double)height, 0.5, 100.0);
		break;
	}
}

void viewport(int mode)
{
	switch (mode)
	{
	case 0:
		glViewport(0, height / 2, width / 2, height / 2);
		break;
	case 1:
		glViewport(width / 2, height / 2, width / 2, height / 2);
		break;
	case 2:
		glViewport(0, 0, width / 2, height / 2);
		break;
	case 3:
		glViewport(width / 2, 0, width / 2, height / 2);
		break;
	case 4:
		glViewport(0, 0, width, height);
		break;
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (Display_Mode)
	{
	case FULL_SCREEN:
		projection(0);
		eye_location(0);
		viewport(4);
		draw_scene();
		break;
	case FOLLOW_BALL_0:
		projection(1);
		eye_location(1);
		viewport(4);
		draw_scene();
		break;
	case FOLLOW_BALL_1:
		projection(1);
		eye_location(2);
		viewport(4);
		draw_scene();
		break;
	}
	glutSwapBuffers();
	glFlush();
}
void JudgeMaterial(const ball* object)
{
	switch (object->role)
	{
	case 0:
		glMaterialfv(GL_FRONT, GL_AMBIENT, fresh_ball_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, fresh_ball_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, fresh_ball_diffuse);
		glMaterialf(GL_FRONT, GL_SHININESS, fresh_ball_shiness);
		break;
	case 1:
		glMaterialfv(GL_FRONT, GL_AMBIENT, red_ball_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, red_ball_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_ball_diffuse);
		glMaterialf(GL_FRONT, GL_SHININESS, red_ball_shiness);
		break;
	case 2:
		glMaterialfv(GL_FRONT, GL_AMBIENT, skyblue_ball_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, skyblue_ball_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, skyblue_ball_diffuse);
		glMaterialf(GL_FRONT, GL_SHININESS, skyblue_ball_shiness);
		break;
	}
}
void draw_balls()
{
	for (int i = 0; i < 5; i++)
	{
		if (Ball[i].role != 255)
		{
			glPushMatrix();
			glEnable(GL_LIGHTING);
			glTranslatef(Ball[i].xyz[0], Ball[i].xyz[1], Ball[i].xyz[2]);
			JudgeMaterial(&Ball[i]);
			gluSphere(sphere, Ball[i].R, 12, 12);
			glDisable(GL_LIGHTING);
			glPopMatrix();
		}
	}
}
void draw_wall(float x, float y, float z, float  Zx, float  Zy, float  Zz)
{
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTranslatef(x, y, z);
	glScalef(Zx, Zy, Zz);
	draw_cube();
	glDisable(GL_LIGHTING);
	glPopMatrix();
}
void draw_maze()
{
	draw_wall(8.0f, 4.0f, 196.0f, 4.0f, 8.0f, 8.0f);/*left bottom corner*/
	draw_wall(2.0f, 4.0f, 184.0f, 4.0f, 8.0f, 4.0f);

	draw_wall(192.0f, 4.0f, 4.0f, 4.0f, 8.0f, 8.0f);/*right top corner*/
	draw_wall(198.0f, 4.0f, 16.0f, 4.0f, 8.0f, 4.0f);

	draw_wall(184.0f, 4.0f, 16.0f, 8.0f, 8.0f, 4.0f);/*右上出發點*/
	draw_wall(182.0f, 4.0f, 10.0f, 4.0f, 8.0f, 8.0f);

	draw_wall(66.0f, 4.0f, 70.0f, 4.0f, 8.0f, 8.0f);/*open space*/
	draw_wall(68.0f, 4.0f, 64.0f, 8.0f, 8.0f, 4.0f);
	draw_wall(134.0f, 4.0f, 70.0f, 4.0f, 8.0f, 8.0f);
	draw_wall(132.0f, 4.0f, 64.0f, 8.0f, 8.0f, 4.0f);
	draw_wall(66.0f, 4.0f, 130.0f, 4.0f, 8.0f, 8.0f);
	draw_wall(68.0f, 4.0f, 136.0f, 8.0f, 8.0f, 4.0f);
	draw_wall(134.0f, 4.0f, 130.0f, 4.0f, 8.0f, 8.0f);
	draw_wall(132.0f, 4.0f, 136.0f, 8.0f, 8.0f, 4.0f);

	draw_wall(192.0f, 4.0f, 35.0f, 4.0f, 8.0f, 20.0f);/*右上出發點*/
	draw_wall(196.0f, 4.0f, 35.0f, 8.0f, 8.0f, 4.0f);

	draw_wall(100.0f, 4.0f, 48.0f, 54.0f, 8.0f, 4.0f); /*open space 外圍牆(上)*/
	draw_wall(150.0f, 4.0f, 100.0f, 4.0f, 8.0f, 54.0f);/*open space 外圍牆(右)*/
	draw_wall(100.0f, 4.0f, 150.0f, 54.0f, 8.0f, 4.0f);/*open space 外圍牆(下)*/
	draw_wall(50.0f, 4.0f, 100.0f, 4.0f, 8.0f, 54.0f);/*open space 外圍牆(左)*/


}
void draw_scene()
{
	glEnable(GL_LIGHTING);
	draw_floor();

	draw_balls();

	draw_maze();	
	glDisable(GL_LIGHTING);
}
void Forward_Proceeding(ball* object, int WhichOne)
{
	if (object->role == 255)
		return;
	int StartPos[3] = { object->xyz[0] ,object->xyz[1] ,object->xyz[2] };
	float Unit_Vector[3] = { object->speed[0] ,object->speed[1] ,object->speed[2] };
	float before = object->Direction;
	object->xyz[0] += object->speed[0];
	object->xyz[1] += object->speed[1];
	object->xyz[2] += object->speed[2];
	Detect_Boundary(object, WhichOne);
	if (!(StartPos[0] == object->xyz[0] && StartPos[1] == object->xyz[1] && StartPos[2] == object->xyz[2]))
	{
		int len = Len(object->speed);
		normalize(Unit_Vector);
		if (before == 0 || before == 2)
		{
			for (int i = StartPos[2], length = 0; length <= len; i += Unit_Vector[2], length++) {
				if (Maze[StartPos[0]][i] >= 0 && Maze[StartPos[0]][i] <= NumofBalls)
				{
					int another = Maze[StartPos[0]][i];
					if ((another == 0 || another == 1) && (WhichOne == 0 || WhichOne == 1))
					{
						if (length == len)
						{
							Maze[StartPos[0]][i] = WhichOne;
							
							break;;
						}
						else
						{
							Maze[StartPos[0]][i] = 100;
							continue;
						}	
					}
					if ((another == 2 || another == 3) && (WhichOne == 2 || WhichOne == 3))
					{
						if (length == len)
						{
							Maze[StartPos[0]][i] = WhichOne;
							continue;
						}
						else
						{
							Maze[StartPos[0]][i] = 100;
							continue;
						}
					}
					if (object->R < Ball[another].R)//be ate
					{
						object->role = 255;
						break;
					}
					else if (object->R == Ball[another].R)
					{
						object->role = 255;
						Ball[another].role = 255;
						Maze[StartPos[0]][i] = 100;
					}
					else if (object->R > Ball[another].R)//eat another
					{
						object->R += Ball[another].R;
						Ball[another].R = 0.0f;
						if (another < 4)
							Ball[another].role = 255;
						Maze[StartPos[0]][i] = 100;
						Maze[(int)object->xyz[0]][(int)object->xyz[2]] = i;
						break;
					}
				}
			}
		}
		else
		{
			for (int i = StartPos[0], length = 0; length <= len; i += Unit_Vector[0], length++) {
				if (Maze[i][StartPos[2]] >= 0 && Maze[i][StartPos[2]] <= NumofBalls)
				{
					int another = Maze[i][StartPos[2]];
					if ((another == 0 || another == 1) && (WhichOne == 0 || WhichOne == 1))
					{
						if (length == len)
						{
							Maze[i][StartPos[2]] = WhichOne;
							break;
						}
						else
						{
							Maze[i][StartPos[2]] = 100;
							continue;
						}
					}
					if ((another == 2 || another == 3) && (WhichOne == 2 || WhichOne == 3))
					{
						if (length == len)
						{
							Maze[i][StartPos[2]] = WhichOne;
							continue;
						}
						else
						{
							Maze[i][StartPos[2]] = 100;
							continue;
						}
					}
					if (object->R < Ball[another].R)//be ate
					{
						fprintf(stderr, "#3Which = %d another = %d\n", WhichOne, another);
						object->role = 255;
						break;
					}
					else if (object->R == Ball[another].R)
					{
						object->role = 255;
						Ball[another].role = 255;
						Maze[i][StartPos[2]] = 100;
					}
					else if (object->R > Ball[another].R)//eat another
					{
						fprintf(stderr, "#4Which = %d another = %d\n", WhichOne, another);
						object->R += Ball[another].R;
						Ball[another].R = 0.0f;
						if (another < 4)
							Ball[another].role = 255;
						Maze[i][StartPos[2]] = 100;
						Maze[(int)object->xyz[0]][(int)object->xyz[2]] = i;
						break;
					}
				}
			}
		}
	}
	else
		Maze[StartPos[0]][StartPos[2]] = WhichOne;
}
void myIdle()
{
	Crate++;/*The hole is shrinking*/
	if (Crate == 10)
	{
		Cindex = Cindex == 0 ? 1 : 0;
		Crate = 0;
	}
	for (int i = 4; i < 5/*NumofBalls*/; i++)
		if (Ball[i].R == 0.0f)
		{
			Ball[i].BottleNeck++;
			if (Ball[i].BottleNeck == 500)
			{
				Ball[i].BottleNeck = 0;
				Ball[i].R = 1.0f;
				Maze[(int)Ball[i].xyz[0]][(int)Ball[i].xyz[2]] = i;
			}
		}
	Forward_Proceeding(&Ball[0], 0);
	Forward_Proceeding(&Ball[1], 1);
	Forward_Proceeding(&Ball[2], 2);
	Forward_Proceeding(&Ball[3], 3);
	Investigation(Ball);
	display();
}
void keyin(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		ort_ang -= 5;
		if (ort_ang <= 10)
			ort_ang = 10;
		break;
	case '-':
		ort_ang += 5;
		break;
	case '8':
		eye[2] -= 5.0f;
		focus[2] -= 5.0f;
		break;
	case '2':
		eye[2] += 5.0f;
		focus[2] += 5.0f;
		break;
	case '4':
		eye[0] -= 5.0f;
		focus[0] -= 5.0f;
		break;
	case '6':
		eye[0] += 5.0f;
		focus[0] += 5.0f;
		break;
	}
	display();
}
void reshape(int w, int h)
{
	width = w, height = h;
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-ort_ang, ort_ang, -ort_ang*(float)h / (float)w, ort_ang * (float)h / (float)w,
			0, 100.0);
	else
		glOrtho(-ort_ang * (float)w / (float)h, ort_ang * (float)w / (float)h, -ort_ang, ort_ang,
			0, 100.0);
	glutPostRedisplay();
}