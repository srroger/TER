#include "cmath"
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"

int resx = 1000, resy = 800;

class Cube;
class RubixCube;
class Viewer;

void reshape(int w, int h);
void init();
void key(unsigned char key, int x, int y);
void specialkey(int key, int x, int y);
void display();
void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2);
void idle();
void processNormalKeys(unsigned char key, int x, int y);

class Viewer
{
	double pos[3];
	double v_alpha, v_beta, v_radius, v_min_radius;
public:
	Viewer(double alpha, double beta, double radius, double min_radius);
	void angle_range_check();
	void set_position();
	void special_keyboard(int key);
	void reset_view(double alpha, double beta, double radius, double min_radius);
	void LookAtCentre();
};
Viewer::Viewer(double alpha, double beta, double radius, double min_radius)
{
	v_alpha = alpha; v_beta = beta; v_radius = radius; v_min_radius = min_radius;
	set_position();
}
void Viewer::angle_range_check()
{
	if (v_alpha >= 360)
		v_alpha -= 360;
	else if (v_alpha<0)
		v_alpha += 360;

	if (v_beta >= 360)
		v_beta -= 360;
	else if (v_beta<0)
		v_beta += 360;
}
void Viewer::set_position()
{
	double degtorad = 3.1415926535897932384626433832795 / 180.0;
	angle_range_check();
	pos[0] = (v_radius * cos(degtorad * v_beta)) * cos(degtorad * v_alpha);
	pos[1] = v_radius * sin(degtorad * v_beta);
	pos[2] = (v_radius * cos(degtorad * v_beta)) * sin(degtorad * v_alpha);
}
void Viewer::special_keyboard(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		v_alpha += 2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT:
		v_alpha -= 2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_DOWN:
		v_beta -= 2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_UP:
		v_beta += 2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_UP:
		v_radius += 1; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_DOWN:
		v_radius -= 1; if (v_radius<v_min_radius) v_radius = v_min_radius;
		set_position(); glutPostRedisplay(); break;
	}
}

void Viewer::reset_view(double alpha, double beta, double radius, double min_radius)
{
	v_alpha = alpha; v_beta = beta; v_radius = radius; v_min_radius = min_radius;
	set_position(); glutPostRedisplay();
}
void Viewer::LookAtCentre()
{
	if (v_beta>90 && v_beta<270)
		gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, 0, -1, 0);
	else
		gluLookAt(pos[0], pos[1], pos[2], 0, 0, 0, 0, 1, 0);
}
Viewer v(45, 45, 20, 10);

class Cube
{
	double cx, cy, cz, len;
	int color[6];
public:
	Cube();
	void set_color(int color);
	void display();
	void rotate(int axis, int dir);
	void set_length(double length);
	friend class RubixCube;
};
Cube::Cube()
{
	cx = cy = cz = 0;
	len = 1;
	for (int i = 0; i<6; i++)
		color[i] = 0;
}
inline void Cube::set_length(double length)
{
	len = length;
}
void Cube::set_color(int color)
{
	switch (color)
	{
	case 0: glColor3f(0, 0, 0); break;
	case 1: glColor3f(1, 1, 0); break;
	case 2: glColor3f(0.3, 0.5, 1); break;
	case 3: glColor3f(1, 0, 0); break;
	case 4: glColor3f(0, 0.8, 0); break;
	case 5: glColor3f(0, 0, 1); break;
	case 6: glColor3f(1, 1, 1); break;
	default: glColor3f(0, 0, 0);
	}
}
void Cube::display()
{
	glPushMatrix();
	glTranslatef(cx, cy, cz);
	/*Face 1 XY-Plane (Behind)*/
	set_color(color[0]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, -len / 2); glVertex3d(-len / 2, len / 2, -len / 2); glVertex3d(len / 2, len / 2, -len / 2); glVertex3d(len / 2, -len / 2, -len / 2);
	glEnd();
	/*Face 2 XZ-Plane (Bottom)*/
	set_color(color[1]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, -len / 2); glVertex3d(len / 2, -len / 2, -len / 2); glVertex3d(len / 2, -len / 2, len / 2); glVertex3d(-len / 2, -len / 2, len / 2);
	glEnd();
	/*Face 3 YZ-Plane (Left)*/
	set_color(color[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, -len / 2, len / 2); glVertex3d(-len / 2, len / 2, len / 2); glVertex3d(-len / 2, len / 2, -len / 2); glVertex3d(-len / 2, -len / 2, -len / 2);
	glEnd();
	/*Face 4 XY-Plane (Front)*/
	set_color(color[3]);
	glBegin(GL_POLYGON);
	glVertex3d(len / 2, -len / 2, len / 2); glVertex3d(len / 2, len / 2, len / 2); glVertex3d(-len / 2, len / 2, len / 2); glVertex3d(-len / 2, -len / 2, len / 2);
	glEnd();
	/*Face 5 XZ-Plane (Top)*/
	set_color(color[4]);
	glBegin(GL_POLYGON);
	glVertex3d(-len / 2, len / 2, len / 2); glVertex3d(len / 2, len / 2, len / 2); glVertex3d(len / 2, len / 2, -len / 2); glVertex3d(-len / 2, len / 2, -len / 2);
	glEnd();
	/*Face 6 YZ-Plane (Right)*/
	set_color(color[5]);
	glBegin(GL_POLYGON);
	glVertex3d(len / 2, -len / 2, len / 2); glVertex3d(len / 2, len / 2, len / 2); glVertex3d(len / 2, len / 2, -len / 2); glVertex3d(len / 2, -len / 2, -len / 2);
	glEnd();
	glPopMatrix();
}
void Cube::rotate(int axis, int dir)
{
	int temp;
	switch (axis)
	{
	case 0:
		if (dir)
		{
			temp = color[0]; color[0] = color[4]; color[4] = color[3]; color[3] = color[1]; color[1] = temp;
		}
		else
		{
			temp = color[0]; color[0] = color[1]; color[1] = color[3]; color[3] = color[4]; color[4] = temp;
		}
		break;
	case 1:
		if (dir)
		{
			temp = color[0]; color[0] = color[2]; color[2] = color[3]; color[3] = color[5]; color[5] = temp;
		}
		else
		{
			temp = color[0]; color[0] = color[5]; color[5] = color[3]; color[3] = color[2]; color[2] = temp;
		}
		break;
	case 2:
		if (dir)
		{
			temp = color[1]; color[1] = color[5]; color[5] = color[4]; color[4] = color[2]; color[2] = temp;
		}
		else
		{
			temp = color[1]; color[1] = color[2]; color[2] = color[4]; color[4] = color[5]; color[5] = temp;
		}
		break;
	}
}
class RubixCube
{
	Cube c[27];
	double len, theta;
	int dir, cubepos[3][3][3];
	bool rotating;
	std::vector<int> axis, plane,
		keypressed/*0=axis change, 1=plane change, 2=clockwise rot, 3=anticlockwise rot*/;
public:
	RubixCube(double length);
	void set_centre();
	void display();
	void display_rotation();
	void rotation_idle_func();
	void keyboard(unsigned char key);
	friend void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2);
	
};
RubixCube::RubixCube(double length)
{
	len = length;
	plane.push_back(2);
	axis.push_back(0);
	rotating = false;
	theta = 0;
	dir = 0;
	int p = 0, i, j, k;
	for (i = 0; i<27; i++)
		c[i].set_length(len);

	for (i = 0; i<9; i++)
	{
		c[i].color[2] = 3; c[i + 18].color[5] = 6;
	}
	for (i = 0; i<27; i += 9)
	{
		c[i + 5].color[0] = 1; c[i + 8].color[0] = 1; c[i + 2].color[0] = 1;
		c[i + 2].color[1] = 2; c[i + 1].color[1] = 2; c[i].color[1] = 2;
		c[i].color[3] = 4; c[i + 3].color[3] = 4; c[i + 6].color[3] = 4;
		c[i + 6].color[4] = 5; c[i + 7].color[4] = 5; c[i + 8].color[4] = 5;
	}
	for (i = 0; i<3; i++)
	for (j = 0; j<3; j++)
	for (k = 0; k<3; k++)
		cubepos[i][j][k] = p++;
	set_centre();
}


void RubixCube::display_rotation()
{
	int i, j, k;
	double angle, disp = len + 0.2;

	if (dir) angle = -theta;
	else angle = theta;

	glPushMatrix();
	switch (axis.back())
	{
	case 0:
		for (i = 0; i<3; i++)
		{
			if (i != plane.back())
			{
				for (j = 0; j<3; j++)
				for (k = 0; k<3; k++)
					c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle, 1, 0, 0);
		for (j = 0; j<3; j++)
		for (k = 0; k<3; k++)
			c[cubepos[plane.back()][j][k]].display();
		break;
	case 1:
		for (j = 0; j<3; j++)
		{
			if (j != plane.back())
			{
				for (i = 0; i<3; i++)
				for (k = 0; k<3; k++)
					c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle, 0, 1, 0);
		for (i = 0; i<3; i++)
		for (k = 0; k<3; k++)
			c[cubepos[i][plane.back()][k]].display();
		break;
	case 2:
		for (k = 0; k<3; k++)
		{
			if (k != plane.back())
			{
				for (i = 0; i<3; i++)
				for (j = 0; j<3; j++)
					c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle, 0, 0, 1);
		for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			c[cubepos[i][j][plane.back()]].display();
		break;
	}
	if (axis.back() == 0)
	{
		if (plane.back() == 0) glTranslated(-disp, 0, 0);
		else if (plane.back() == 2) glTranslated(disp, 0, 0);
		glRotatef(90, 0, 1, 0);
	}
	else if (axis.back() == 1)
	{
		if (plane.back() == 0) glTranslated(0, -disp, 0);
		else if (plane.back() == 2) glTranslated(0, disp, 0);
		glRotatef(90, 1, 0, 0);
	}
	else
	{
		if (plane.back() == 0) glTranslated(0, 0, disp);
		else if (plane.back() == 2) glTranslated(0, 0, -disp);
	}
	glColor4f(1, 1, 1, 0.3);
	glRectd(-(len*2.25), -(len*2.25), (len*2.25), (len*2.25));
	glPopMatrix();
}
void RubixCube::rotation_idle_func()
{
	if (rotating)
	{
		if (theta >= (double)90)
		{
			glutIdleFunc(NULL);
			rotating = false;
			theta = 0.0;
		}
		else
			theta += +0.5;
		    glutPostRedisplay();
	}
}
void RubixCube::set_centre()
{
	int i, j, k;
	double dist = len + 0.2;
	for (j = 0; j<3; j++)
	{
		for (k = 0; k<3; k++)
		{
			c[cubepos[0][j][k]].cx = -dist;
			c[cubepos[1][j][k]].cx = 0;
			c[cubepos[2][j][k]].cx = dist;
		}
	}
	for (i = 0; i<3; i++)
	{
		for (j = 0; j<3; j++)
		{
			c[cubepos[i][j][0]].cz = dist;
			c[cubepos[i][j][1]].cz = 0;
			c[cubepos[i][j][2]].cz = -dist;
		}
	}
	for (i = 0; i<3; i++)
	{
		for (k = 0; k<3; k++)
		{
			c[cubepos[i][0][k]].cy = -dist;
			c[cubepos[i][1][k]].cy = 0;
			c[cubepos[i][2][k]].cy = dist;
		}
	}
}
void RubixCube::display()
{
	if (!rotating)
	{
		double disp = len + 0.2;
		glPushMatrix();
		for (int i = 0; i<27; i++)
			c[i].display();

		if (axis.back() == 0)
		{
			if (plane.back() == 0) glTranslated(-disp, 0, 0);
			else if (plane.back() == 2) glTranslated(disp, 0, 0);
			glRotatef(90, 0, 1, 0);
		}
		else if (axis.back() == 1)
		{
			if (plane.back() == 0) glTranslated(0, -disp, 0);
			else if (plane.back() == 2) glTranslated(0, disp, 0);
			glRotatef(90, 1, 0, 0);
		}
		else
		{
			if (plane.back() == 0) glTranslated(0, 0, len);
			else if (plane.back() == 2) glTranslated(0, 0, -len);
		}
		glColor4f(1, 1, 1, 0.3);
		glPopMatrix();
	}
	else
		display_rotation();
}
void change_pos(int dir, int & a0, int & a1, int & a2, int & b0, int & b1, int & b2, int & c0, int & c1, int & c2)
{
	int temp;
	if (dir)
	{
		temp = a0; a0 = a2; a2 = c2; c2 = c0; c0 = temp;
		temp = b0; b0 = a1; a1 = b2; b2 = c1; c1 = temp;
	}
	else
	{
		temp = a0; a0 = c0; c0 = c2; c2 = a2; a2 = temp;
		temp = b0; b0 = c1; c1 = b2; b2 = a1; a1 = temp;
	}
}


RubixCube r(2);

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(resx, resy);
	//glutInitWindowPosition(900,0);
	glutCreateWindow("Rubik's Cube");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(specialkey);
	init();
	glutMainLoop();
}
void idle()
{
	r.rotation_idle_func();
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (GLfloat)w / (GLfloat)h, 1, 200);
	glMatrixMode(GL_MODELVIEW);
}

void specialkey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
	case GLUT_KEY_DOWN:	case GLUT_KEY_UP:
	case GLUT_KEY_PAGE_UP:case GLUT_KEY_PAGE_DOWN:
	v.special_keyboard(key); break;
	}
}
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}
void init()
{
	
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glEnable(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	v.LookAtCentre();

	glTranslatef(0, 0, 0);
	r.display();
	

	glFlush();
	glutSwapBuffers();
	
}



