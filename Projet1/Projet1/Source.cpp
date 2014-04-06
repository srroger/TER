#include "cmath"
#include "cstdlib"
#include "ctime"
#include "vector"
#include "windows.h"
#include "glut.h"

int resx=1000/*1600*/,resy=800;

class Cube;
class RubixCube;
class Viewer;

void reshape(int w,int h);
void init();
void key(unsigned char key,int x ,int y);
void specialkey(int key,int x ,int y);
void display();
void change_pos(int dir,int & a0,int & a1,int & a2,int & b0,int & b1,int & b2,int & c0,int & c1,int & c2);
void idle();
bool conformationbox(wchar_t winname[],wchar_t msg[]);
void msgbox(wchar_t winname[],wchar_t msg[],int icon=1);
void create_menu();
void view_menu(int m);
void plane_menu(int m);
void axis_menu(int m);
void main_menu(int m);

class Viewer
{
	double pos[3];
	double v_alpha,v_beta,v_radius,v_min_radius;
public:
	Viewer(double alpha,double beta, double radius, double min_radius);
	void angle_range_check();
	void set_position();
	void special_keyboard(int key);
	void reset_view(double alpha,double beta, double radius, double min_radius);
	void LookAtCentre();
};
Viewer::Viewer(double alpha,double beta, double radius, double min_radius)
{
	v_alpha=alpha; v_beta=beta; v_radius=radius; v_min_radius=min_radius;
	set_position();
}
void Viewer::angle_range_check()
{
	if(v_alpha>=360)
		v_alpha-=360;
	else if(v_alpha<0)
		v_alpha+=360;

	if(v_beta>=360)
		v_beta-=360;
	else if(v_beta<0)
		v_beta+=360;
}
void Viewer::set_position()
{
	double degtorad=3.1415926535897932384626433832795/180.0;
	angle_range_check();
	pos[0]=(v_radius * cos(degtorad * v_beta)) * cos(degtorad * v_alpha);
	pos[1]= v_radius * sin(degtorad * v_beta);
	pos[2]=(v_radius * cos(degtorad * v_beta)) * sin(degtorad * v_alpha);
}
void Viewer::special_keyboard(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		v_alpha+=2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT:
		v_alpha-=2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_DOWN:
		v_beta-=2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_UP:
		v_beta+=2; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_UP:
		v_radius+=1; set_position(); glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_DOWN:
		v_radius-=1; if(v_radius<v_min_radius) v_radius=v_min_radius;
		set_position(); glutPostRedisplay(); break;
	}
}
void Viewer::reset_view(double alpha,double beta, double radius, double min_radius)
{
	v_alpha=alpha; v_beta=beta; v_radius=radius; v_min_radius=min_radius;
	set_position(); glutPostRedisplay();
}
void Viewer::LookAtCentre()
{
	if(v_beta>90 && v_beta<270)
		gluLookAt(pos[0],pos[1],pos[2],0,0,0,0,-1,0);
	else
		gluLookAt(pos[0],pos[1],pos[2],0,0,0,0,1,0);
}
Viewer v(45,45,20,10);

class Cube
{
	double cx,cy,cz,len;
	int color[6];
public:
	Cube();
	void set_color(int color);
	void display();
	void rotate(int axis,int dir);
	void set_length(double length);
	friend class RubixCube;
};
Cube::Cube()
{
	cx=cy=cz=0;
	len=1;
	for(int i=0;i<6;i++)
		color[i]=0;
}
inline void Cube::set_length(double length)
{
	len=length;
}
void Cube::set_color(int color)
{
	switch(color)
	{
	case 0: glColor3f(0,0,0); break;
	case 1: glColor3f(1,1,0); break;
	case 2: /*glColor3f(0.3,0.5,1);*/ glColor3f(1.0f, 0.5f, 0.0f);break;
	case 3: glColor3f(1,0,0); break;
	case 4: glColor3f(0,0.8,0); break;
	case 5: glColor3f(0,0,1); break;
	case 6: glColor3f(1,1,1); break;
	default: glColor3f(0,0,0);
	}
}
void Cube::display()
{
	glPushMatrix();
	glTranslatef(cx,cy,cz);
	/*Face 1 XY-Plane (Behind)*/
	set_color(color[0]);
	glBegin(GL_POLYGON);
	glVertex3d(-len/2,-len/2,-len/2); glVertex3d(-len/2,len/2,-len/2); glVertex3d(len/2,len/2,-len/2); glVertex3d(len/2,-len/2,-len/2);
	glEnd();
	/*Face 2 XZ-Plane (Bottom)*/
	set_color(color[1]);
	glBegin(GL_POLYGON);
	glVertex3d(-len/2,-len/2,-len/2); glVertex3d(len/2,-len/2,-len/2); glVertex3d(len/2,-len/2,len/2); glVertex3d(-len/2,-len/2,len/2);
	glEnd();
	/*Face 3 YZ-Plane (Left)*/
	set_color(color[2]);
	glBegin(GL_POLYGON);
	glVertex3d(-len/2,-len/2,len/2); glVertex3d(-len/2,len/2,len/2); glVertex3d(-len/2,len/2,-len/2); glVertex3d(-len/2,-len/2,-len/2);
	glEnd();
	/*Face 4 XY-Plane (Front)*/
	set_color(color[3]);
	glBegin(GL_POLYGON);
	glVertex3d(len/2,-len/2,len/2); glVertex3d(len/2,len/2,len/2); glVertex3d(-len/2,len/2,len/2); glVertex3d(-len/2,-len/2,len/2);
	glEnd();
	/*Face 5 XZ-Plane (Top)*/
	set_color(color[4]);
	glBegin(GL_POLYGON);
	glVertex3d(-len/2,len/2,len/2); glVertex3d(len/2,len/2,len/2); glVertex3d(len/2,len/2,-len/2); glVertex3d(-len/2,len/2,-len/2);
	glEnd();
	/*Face 6 YZ-Plane (Right)*/
	set_color(color[5]);
	glBegin(GL_POLYGON);
	glVertex3d(len/2,-len/2,len/2); glVertex3d(len/2,len/2,len/2); glVertex3d(len/2,len/2,-len/2); glVertex3d(len/2,-len/2,-len/2);
	glEnd();
	glPopMatrix();
}
void Cube::rotate(int axis,int dir)
{
	int temp;
	switch(axis)
	{
	case 0:/*X-axis*/
		if(dir)/*Clock-Wise*/
		{ temp=color[0]; color[0]=color[4]; color[4]=color[3]; color[3]=color[1]; color[1]=temp; }
		else/*Anti-Clock-Wise*/
		{ temp=color[0]; color[0]=color[1]; color[1]=color[3]; color[3]=color[4]; color[4]=temp; }
		break;
	case 1:/*Y-axis*/
		if(dir)/*Clock-Wise*/
		{ temp=color[0]; color[0]=color[2]; color[2]=color[3]; color[3]=color[5]; color[5]=temp; }
		else/*Anti-Clock-Wise*/
		{ temp=color[0]; color[0]=color[5]; color[5]=color[3]; color[3]=color[2]; color[2]=temp; }
		break;
	case 2:/*Z-axis*/
		if(dir)/*Clock-Wise*/
		{ temp=color[1]; color[1]=color[5]; color[5]=color[4]; color[4]=color[2]; color[2]=temp; }
		else/*Anti-Clock-Wise*/
		{ temp=color[1]; color[1]=color[2]; color[2]=color[4]; color[4]=color[5]; color[5]=temp; }
		break;
	}
}
class RubixCube
{
	Cube c[27];
	double len,theta;
	int dir, cubepos[3][3][3];
	bool rotating;
	std::vector<int> axis,plane,
		keypressed/*0=axis change, 1=plane change, 2=clockwise rot, 3=anticlockwise rot*/;
public:
	bool solving;
	RubixCube(double length);
	void set_centre();
	void display();
	void rotate_cube_plane();
	void display_rotation();
	void rotation_idle_func();
	void keyboard(unsigned char key);
	friend void change_pos(int dir,int & a0,int & a1,int & a2,int & b0,int & b1,int & b2,int & c0,int & c1,int & c2);
	void randomize();
	void solve();
	bool check_solved();
};
RubixCube::RubixCube(double length)
{
	len=length;
	plane.push_back(2);
	axis.push_back(0);
	solving,rotating=false;
	theta=0;
	dir=0;
	int p=0,i,j,k;
	for(i=0;i<27;i++)
		c[i].set_length(len);

	for(i=0;i<9;i++)
	{ c[i].color[2]=3; c[i+18].color[5]=6; }
	for(i=0;i<27;i+=9)
	{
		c[i+5].color[0]=1; c[i+8].color[0]=1; c[i+2].color[0]=1;
		c[i+2].color[1]=2; c[i+1].color[1]=2; c[i].color[1]=2;
		c[i].color[3]=4; c[i+3].color[3]=4; c[i+6].color[3]=4;
		c[i+6].color[4]=5; c[i+7].color[4]=5; c[i+8].color[4]=5;
	}
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				cubepos[i][j][k]=p++;
	set_centre();
}
bool RubixCube::check_solved()
{
	int v1, v2, i, j;
	v1=c[cubepos[0][0][0]].color[2]; v2=c[cubepos[2][0][0]].color[5];

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if( c[cubepos[0][i][j]].color[2]!=v1 || c[cubepos[2][i][j]].color[5]!=v2)
				return false;

	v1=c[cubepos[0][0][0]].color[0]; v2=c[cubepos[0][0][2]].color[3];

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if( c[cubepos[i][j][0]].color[0]!=v1 || c[cubepos[i][j][2]].color[3]!=v2)
				return false;

	v1=c[cubepos[0][0][0]].color[1]; v2=c[cubepos[0][2][0]].color[4];

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if( c[cubepos[i][0][j]].color[1]!=v1 || c[cubepos[i][2][j]].color[4]!=v2)
				return false;

	return true;
}
void RubixCube::randomize()
{
	time_t t; time(&t);
	tm * a = new tm();
	localtime_s(a,&t);

	int i,j,k,till;
	till=20+(a->tm_sec + a->tm_min + a->tm_hour + a->tm_mday + a->tm_mon + a->tm_year)%30;
	j=(a->tm_sec + a->tm_min + a->tm_hour + a->tm_mday + a->tm_mon + a->tm_year)%4;
	for(i=0;i<till;i++)
	{
		k=j;
		while(j==k  || (j==2 && k==3) || (j==3 && k==2))
			j=rand()%4;
		keypressed.push_back(k);
		switch(k)
		{
		case 0:
			axis.push_back(rand()%3);
			break;
		case 1:
			plane.push_back(rand()%3);
			break;
		case 2:
			dir=0;
			rotating=false;
			rotate_cube_plane();
			break;
		case 3:
			dir=1;
			rotating=false;
			rotate_cube_plane();
			break;
		}
	}
}
void RubixCube::solve()
{
	if(keypressed.empty())
	{solving=false; return;}
	switch(keypressed.back())
	{
	case 0:
		axis.pop_back();
		break;
	case 1:
		plane.pop_back();
		break;
	case 2:
		dir=1;
		rotating=true;
		rotate_cube_plane();
		break;
	case 3:
		dir=0;
		rotating=true;
		rotate_cube_plane();
		break;
	}
	keypressed.pop_back();
}
void RubixCube::display_rotation()
{
	int i,j,k;
	double angle, disp=len+0.2;

	if(dir) angle=-theta;
	else angle=theta;

	glPushMatrix();
	switch(axis.back())
	{
	case 0:
		for(i=0;i<3;i++)
		{
			if(i!=plane.back())
			{
				for(j=0;j<3;j++)
					for(k=0;k<3;k++)
						c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle,1,0,0);
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				c[cubepos[plane.back()][j][k]].display();
		break;
	case 1:
		for(j=0;j<3;j++)
		{
			if(j!=plane.back())
			{
				for(i=0;i<3;i++)
					for(k=0;k<3;k++)
						c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle,0,1,0);
		for(i=0;i<3;i++)
			for(k=0;k<3;k++)
				c[cubepos[i][plane.back()][k]].display();
		break;
	case 2:
		for(k=0;k<3;k++)
		{
			if(k!=plane.back())
			{
				for(i=0;i<3;i++)
					for(j=0;j<3;j++)
						c[cubepos[i][j][k]].display();
			}
		}
		glRotated(angle,0,0,1);
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				c[cubepos[i][j][plane.back()]].display();
		break;
	}
	if(axis.back()==0)
	{
		if(plane.back()==0) glTranslated(-disp,0,0);
		else if(plane.back()==2) glTranslated(disp,0,0);
		glRotatef(90,0,1,0);
	}
	else if(axis.back()==1)
	{
		if(plane.back()==0) glTranslated(0,-disp,0);
		else if(plane.back()==2) glTranslated(0,disp,0);
		glRotatef(90,1,0,0);
	}
	else
	{
		if(plane.back()==0) glTranslated(0,0,disp);
		else if(plane.back()==2) glTranslated(0,0,-disp);
	}
	glColor4f(1,1,1,0.3);
	glRectd(-(len*2.25),-(len*2.25),(len*2.25),(len*2.25));
	glPopMatrix();
}
void RubixCube::rotation_idle_func()
{
	if(rotating)
	{
		if(theta>=(double)90)
		{
			glutIdleFunc(NULL);
			rotating=false;
			theta=0.0;
			rotate_cube_plane();
		}
		else
			theta+=+0.5;
		glutPostRedisplay();
	}
}
void RubixCube::set_centre()
{
	int i,j,k;
	double dist=len+0.2;
	for(j=0;j<3;j++)
	{
		for(k=0;k<3;k++)
		{
			c[cubepos[0][j][k]].cx=-dist;
			c[cubepos[1][j][k]].cx=0;
			c[cubepos[2][j][k]].cx=dist;
		}
	}
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			c[cubepos[i][j][0]].cz=dist;
			c[cubepos[i][j][1]].cz=0;
			c[cubepos[i][j][2]].cz=-dist;
		}
	}
	for(i=0;i<3;i++)
	{
		for(k=0;k<3;k++)
		{
			c[cubepos[i][0][k]].cy=-dist;
			c[cubepos[i][1][k]].cy=0;
			c[cubepos[i][2][k]].cy=dist;
		}
	}
}
void RubixCube::display()
{
	if(!rotating)
	{
		double disp=len+0.2;
		glPushMatrix();
		for(int i=0;i<27;i++)
			c[i].display();

		if(axis.back()==0)
		{
			if(plane.back()==0) glTranslated(-disp,0,0);
			else if(plane.back()==2) glTranslated(disp,0,0);
			glRotatef(90,0,1,0);
		}
		else if(axis.back()==1)
		{
			if(plane.back()==0) glTranslated(0,-disp,0);
			else if(plane.back()==2) glTranslated(0,disp,0);
			glRotatef(90,1,0,0);
		}
		else
		{
			if(plane.back()==0) glTranslated(0,0,len);
			else if(plane.back()==2) glTranslated(0,0,-len);
		}
		glColor4f(1,1,1,0.3);
		glRectd(-(len*2.25),-(len*2.25),(len*2.25),(len*2.25));
		glPopMatrix();
	}
	else
		display_rotation();
}
void change_pos(int dir,int & a0,int & a1,int & a2,int & b0,int & b1,int & b2,int & c0,int & c1,int & c2)
{
	int temp;
	if(dir)
	{
		temp=a0; a0=a2; a2=c2; c2=c0; c0=temp;
		temp=b0; b0=a1; a1=b2; b2=c1; c1=temp;
	}
	else
	{
		temp=a0; a0=c0; c0=c2; c2=a2; a2=temp;
		temp=b0; b0=c1; c1=b2; b2=a1; a1=temp;
	}
}
void RubixCube::rotate_cube_plane()
{
	if(!rotating)
	{
		int i,j,k;
		if(plane.back()!=0 && plane.back()!=1 && plane.back()!=2)
			exit(2);
		switch(axis.back())
		{
		case 0:
			change_pos(dir,cubepos[plane.back()][0][0],cubepos[plane.back()][0][1],cubepos[plane.back()][0][2],cubepos[plane.back()][1][0],cubepos[plane.back()][1][1],cubepos[plane.back()][1][2],cubepos[plane.back()][2][0],cubepos[plane.back()][2][1],cubepos[plane.back()][2][2]);
			break;
		case 1:
			change_pos(dir,cubepos[2][plane.back()][0],cubepos[2][plane.back()][1],cubepos[2][plane.back()][2],cubepos[1][plane.back()][0],cubepos[1][plane.back()][1],cubepos[1][plane.back()][2],cubepos[0][plane.back()][0],cubepos[0][plane.back()][1],cubepos[0][plane.back()][2]);
			break;
		case 2:
			change_pos(dir,cubepos[0][0][plane.back()],cubepos[1][0][plane.back()],cubepos[2][0][plane.back()],cubepos[0][1][plane.back()],cubepos[1][1][plane.back()],cubepos[2][1][plane.back()],cubepos[0][2][plane.back()],cubepos[1][2][plane.back()],cubepos[2][2][plane.back()]);
			break;
		default:
			exit(2);
		}
		set_centre();

		switch(axis.back())
		{
		case 0:
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[plane.back()][j][k]].rotate(0,dir);
			break;
		case 1:
			for(i=0;i<3;i++)
				for(k=0;k<3;k++)
					c[cubepos[i][plane.back()][k]].rotate(1,dir);
			break;
		case 2:
			for(i=0;i<3;i++)
				for(j=0;j<3;j++)
					c[cubepos[i][j][plane.back()]].rotate(2,dir);
			break;
		}
		if(check_solved())
			msgbox(L"Cube Solved",L"The Cube Has Been Solved",2);
	}
	else
		glutIdleFunc(idle);
}
void RubixCube::keyboard(unsigned char key)
{
	if(!rotating)
	{
		switch (key)
		{
		case '.':
		case '>':
			dir=1;
			keypressed.push_back(3);
			rotating=true;
			rotate_cube_plane();
			break;
		case ',':
		case '<':
			dir=0;
			keypressed.push_back(2);
			rotating=true;
			rotate_cube_plane();
			break;
		case 'x':
		case 'X':
			axis.push_back(0);
			keypressed.push_back(0);
			break;
		case 'y':
		case 'Y':
			axis.push_back(1);
			keypressed.push_back(0);
			break;
		case 'z':
		case 'Z':
			axis.push_back(2);
			keypressed.push_back(0);
			break;
		case '1':
			plane.push_back(0);
			keypressed.push_back(1);
			break;
		case '2':
			plane.push_back(1);
			keypressed.push_back(1);
			break;
		case '3':
			plane.push_back(2);
			keypressed.push_back(1);
			break;
		case 's':
		case 'S':
			solving=true;
			solve();
			break;
		}
	}
}
RubixCube r(2);

int main(int argc,char * argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(resx,resy);
	//glutInitWindowPosition(900,0);
	glutCreateWindow("Rubix's Cube");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutSpecialFunc(specialkey);
	init();
	glutMainLoop();
}
void idle()
{
	r.rotation_idle_func();
}
void reshape(int w,int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70,(GLfloat)w/(GLfloat)h,1,200);
	glMatrixMode(GL_MODELVIEW);
}
void key(unsigned char key,int x ,int y)
{
	switch(key)
	{
	case 'R':
	case 'r':
		v.reset_view(45,45,20,10);
		break;
	case 27:
		if(conformationbox(L"Exit?",L"Do You Want To Exit?"))
			exit(0);
		break;
	case 'n':
	case 'N':
		if(conformationbox(L"New Game?",L"Do You Want To Start A New Game?"))
		{
			r.randomize();
			glutPostRedisplay();
		}
		break;
	case '.': case '>':
	case ',': case '<':
	case 'x': case 'X':
	case 'y': case 'Y':
	case 'z': case 'Z':
	case '1': case '2': case '3':
		r.keyboard(key);
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		if(!r.solving)
			if(!conformationbox(L"Solve Cube?",L"Do You Want The Computer To Solve The Cube?"))
				break;
		r.keyboard(key);
		glutPostRedisplay();
		break;
	}
}
void specialkey(int key,int x ,int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
	case GLUT_KEY_DOWN:	case GLUT_KEY_UP:
	case GLUT_KEY_PAGE_UP:case GLUT_KEY_PAGE_DOWN:
		v.special_keyboard(key); break;
	case GLUT_KEY_F1:
		msgbox(L"Help",L"Keyboard Controls\n---------------------\nR:		Reset View\nN:		New Game\nS:		Solve Cube\n>:		Rotate ClockWise\n<:		Rotate AntiClockwise\n1:		Select Plane-1\n2:		Select Plane-2\n3:		Select Plane-3\nX:		Select Axis-X\nY:		Select Axis-Y\nZ:		Select Axis-Z\nUP:		Move Camera Up\nDOWN:		Move Camera Down\nLEFT:		Move Camera Left\nRIGHT:		Move Camera Right\nPAGE UP:		Zoom Out Camera\nPAGE DOWN:	Zoom In Camera\nF1:		Show Help\nF2:		Show Info\nESC:		Exit");
		break;
	case GLUT_KEY_F2:
		msgbox(L"Info",L"This Program Was Written By:\nAnshu Krishna\n\nContact: anshu.krishna5@gmail.com",2);
		break;
	}
}
void init()
{
	r.randomize();
	create_menu();
	glClearColor(0.2,0.2,0.2,1.0);
	glEnable(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	v.LookAtCentre();

	glTranslatef(-5,0,0);
	r.display();
	glTranslatef(10,0,0);
	glScalef(-0.5,-0.5,-0.5);
	r.display();

	glFlush();
	glutSwapBuffers();
	if(r.solving)
		key('s',0,0);
}
bool conformationbox(wchar_t winname[],wchar_t msg[])
{
	int ans = MessageBox(NULL,(LPCSTR) msg,(LPCSTR) winname,MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
	switch (ans)
	{
	case IDYES:
		return true;
	case IDNO:
		return false;
	default:
		return false;
	}
}
void msgbox(wchar_t winname[],wchar_t msg[],int icon)
{
	if(icon==1)
		MessageBox(NULL,(LPCSTR) msg,(LPCSTR) winname,MB_ICONQUESTION | MB_OK);
	else
		MessageBox(NULL,(LPCSTR) msg,(LPCSTR) winname,MB_ICONINFORMATION | MB_OK);
}
void create_menu()
{
	int viewmenu=glutCreateMenu(view_menu);
	glutAddMenuEntry("Reset Camera View (R)",0);
	glutAddMenuEntry("Move Camera Up (UP)",1);
	glutAddMenuEntry("Move Camera Down (DOWN)",2);
	glutAddMenuEntry("Move Camera Left (LEFT)",3);
	glutAddMenuEntry("Move Camera Right (RIGHT)",4);
	glutAddMenuEntry("Zoom In Camera (PAGE DOWN)",5);
	glutAddMenuEntry("Zoom Out Camera (PAGE UP)",6);

	int planemenu=glutCreateMenu(plane_menu);
	glutAddMenuEntry("Plane-1 (1)",1);
	glutAddMenuEntry("Plane-2 (2)",2);
	glutAddMenuEntry("Plane-3 (3)",3);

	int axismenu=glutCreateMenu(axis_menu);
	glutAddMenuEntry("Axis-X (X)",1);
	glutAddMenuEntry("Axis-Y (Y)",2);
	glutAddMenuEntry("Axis-Z (Z)",3);

	glutCreateMenu(main_menu);
	glutAddSubMenu("Camera",viewmenu);
	glutAddSubMenu("Select Axis",axismenu);
	glutAddSubMenu("Select Plane",planemenu);
	glutAddMenuEntry("New Game (N)",1);
	glutAddMenuEntry("Solve Cube (S)",2);
	glutAddMenuEntry("Info (F2)",3);
	glutAddMenuEntry("Help (F1)",4);
	glutAddMenuEntry("Exit (ESC)",5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void view_menu(int m)
{
	switch(m)
	{
	case 0: key('r',0,0); break;
	case 1: specialkey(GLUT_KEY_UP,0,0); break;
	case 2: specialkey(GLUT_KEY_DOWN,0,0); break;
	case 3: specialkey(GLUT_KEY_LEFT,0,0); break;
	case 4: specialkey(GLUT_KEY_RIGHT,0,0); break;
	case 5: specialkey(GLUT_KEY_PAGE_DOWN,0,0); break;
	case 6: specialkey(GLUT_KEY_PAGE_UP,0,0); break;
	}
}
void plane_menu(int m)
{
	switch(m)
	{
	case 1: key('1',0,0); break;
	case 2: key('2',0,0); break;
	case 3: key('3',0,0); break;
	}
}
void axis_menu(int m)
{
	switch(m)
	{
	case 1: key('x',0,0); break;
	case 2: key('y',0,0); break;
	case 3: key('z',0,0); break;
	}
}
void main_menu(int m)
{
	switch(m)
	{
	case 1: key('n',0,0); break;
	case 2: key('s',0,0); break;
	case 3: specialkey(GLUT_KEY_F2,0,0); break;
	case 4: specialkey(GLUT_KEY_F1,0,0); break;
	case 5: key(27,0,0); break;
	}
}