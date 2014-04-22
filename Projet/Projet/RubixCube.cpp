#include "stdafx.h"
#include "RubixCube.h"
/**
		    ^y
		    |
		z<--x
*/

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406 
#endif
#define DEG_TO_RAD (M_PI/180.0) //multiplier cette valeur par le degree
#define RAD_TO_DEG (180.0/M_PI)

RubixCube::RubixCube(double length)
{
	x= 20; y=0 ; z=0;
	rx = 0; ry = 45; rz = -30;	
	//x= 0; y=0 ; z=0;
	//rx = 0; ry = 0; rz = 0;
	
	AxisX[0] = 1; AxisX[1] = 0; AxisX[2] = 0;
	AxisY[0] = 0; AxisY[1] = 1; AxisY[2] = 0;
	AxisZ[0] = 0; AxisZ[1] = 0; AxisZ[2] = 1;


	len=length;
	plane.push_back(2);
	axis.push_back(2);
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
			//Rotation d'un coté (celui que nous utilisons tout le temps)
			dir=1;
			rotating=false;
			rotate_cube_plane();
			//Fin rotation coté
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

	case 3:
		glRotated(angle,1,0,0);
		for(i = 0 ; i< 3 ; i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[i][j][k]].display();
		break;
		
	case 4:
		glRotated(angle,0,1,0);
		for(i = 0 ; i< 3 ; i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[i][j][k]].display();
		break;

	case 5:
		glRotated(angle,0,0,1);
		for(i = 0 ; i< 3 ; i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[i][j][k]].display();
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
	else if(axis.back()==2)
	{
		if(plane.back()==0) glTranslated(0,0,disp);
		else if(plane.back()==2) glTranslated(0,0,-disp);
	}
	else if(axis.back()==3 || axis.back()==4  || axis.back()==5 )
	{
		glTranslated(0,0,disp);
		glTranslated(0,0,-disp);
	}
	glColor4f(1,1,1,0.3);
	glRectd(-(len*2.25),-(len*2.25),(len*2.25),(len*2.25));
	glPopMatrix();
}
void idle();
void RubixCube::rotation_idle_func()
{
	if(rotating)
	{
		if(theta>=(double)90)
		{
			//glutIdleFunc(NULL);
			rotating=false; 
			theta=0.0;
			rotate_cube_plane();
		}
		else
			theta+=+10.0;
		glutPostRedisplay();
	}
	else
	{
		glutIdleFunc(idle);
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
	glTranslatef(x,y,z);glRotatef(rx,AxisX[0],AxisX[1],AxisX[2]);glRotatef(ry,AxisY[0],AxisY[1],AxisY[2]);glRotatef(rz,AxisZ[0],AxisZ[1],AxisZ[2]);
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
		else if(axis.back()==2)
		{
			if(plane.back()==0) glTranslated(0,0,len);
			else if(plane.back()==2) glTranslated(0,0,-len);
		}		
		else if(axis.back()==3)
		{
			glTranslated(-disp,0,0);
			glTranslated(disp,0,0);
		}		
		else if(axis.back()==4)
		{
			glTranslated(0,-disp,0);
			glTranslated(0,disp,0);
		}		
		else if(axis.back()==5)
		{
			glTranslated(0,0,len);
			glTranslated(0,0,-len);
		}
		glColor4f(1,1,1,0.3);
		glRectd(-(len*2.25),-(len*2.25),(len*2.25),(len*2.25));
		glPopMatrix();
		
		//Dessin du sol :
		//glPushMatrix();
		//glTranslated(-len * 20,0,0);			glRotatef(90,0,1,0);
		//glColor4f(0.5,0.5,0.7,0.5);
		//glRectd(-(len*32.5),-(len*32.5),(len*32.5),(len*32.5));
		//glPopMatrix();
	}
	else
		display_rotation();
}

void idle();
void RubixCube::rotate_rubix()
{// Case axix.back == 0
	axis.push_back(3);
	//int i,k,j;
	//rotating = true;
	//plane.push_back(0);
	//change_pos(dir,cubepos[0][0][0],cubepos[plane.back()][0][1],cubepos[plane.back()][0][2],cubepos[plane.back()][1][0],cubepos[plane.back()][1][1],cubepos[plane.back()][1][2],cubepos[plane.back()][2][0],cubepos[plane.back()][2][1],cubepos[plane.back()][2][2]);
	//	set_centre();
	//
	//plane.push_back(0);
	//for(j=0;j<3;j++)
	//	for(k=0;k<3;k++)
	//		c[cubepos[plane.back()][j][k]].rotate(0,dir);
	



}
void RubixCube::rotate_cube_plane()
{
	if(!rotating)
	{
		int i,j,k;int p = 0;
		if(plane.back()!=0 && plane.back()!=1 && plane.back()!=2)
			exit(2);
		//cout<<"eeeeeeeeeeeeeeeeeeeee"<<endl;
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
		case 3:
			for(p = 0 ; p < 3 ; p++)
			change_pos(dir,cubepos[p][0][0],cubepos[p][0][1],cubepos[p][0][2],cubepos[p][1][0],cubepos[p][1][1],cubepos[p][1][2],cubepos[p][2][0],cubepos[p][2][1],cubepos[p][2][2]);
			break;
		case 4:
			for(p = 0 ; p < 3 ; p++)
			change_pos(dir,cubepos[2][p][0],cubepos[2][p][1],cubepos[2][p][2],cubepos[1][p][0],cubepos[1][p][1],cubepos[1][p][2],cubepos[0][p][0],cubepos[0][p][1],cubepos[0][p][2]);
			break;
		case 5:
			for(p = 0 ; p < 3 ; p++)
			change_pos(dir,cubepos[0][0][p],cubepos[1][0][p],cubepos[2][0][p],cubepos[0][1][p],cubepos[1][1][p],cubepos[2][1][p],cubepos[0][2][p],cubepos[1][2][p],cubepos[2][2][p]);
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
		case 3:
			for(i = 0 ; i < 3; i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[i][j][k]].rotate(0,dir);
			break;		
		case 4:
			for(i = 0 ; i < 3; i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[i][j][k]].rotate(1,dir);
			break;
		case 5:
			for(i = 0 ; i < 3; i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					c[cubepos[i][j][k]].rotate(2,dir);
			break;
		}
		if(check_solved())
		{
			//msgbox(L"Cube Solved",L"The Cube Has Been Solved",2);
		}
	}
	else
	{
		glutIdleFunc(idle);
	}
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
		case 'c':
		case 'C':
			axis.push_back(3);
			keypressed.push_back(0);
			break;
		case 'u':
		case 'U':
			axis.push_back(4);
			keypressed.push_back(0);
			break;
		case 'e':
		case 'E':
			axis.push_back(5);
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
			//solving=true;
			//solve();
			break;
		}
	}
}


void RubixCube::moveZ(GLfloat val)
{
	z = z+val;
}

void RubixCube::moveY(GLfloat val)
{
	y = y + val; 
}
void RubixCube::moveX(GLfloat val)
{
	x = x + val; 
}

//Rouge
void RubixCube::moveRX()
{
	axis.push_back(3);
	//keypressed.push_back(0);
	dir=1;
	//keypressed.push_back(3);
	rotating=true;
	rotate_cube_plane();
	
	
	//rx = rx + val; if(rx > 360) rx= rx-360; if(rx < -360) rx= rx+360;
}

//Vert
void RubixCube::moveRY()
{
		axis.push_back(4);
		//keypressed.push_back(0);
		dir=1;
		//keypressed.push_back(3);
		rotating=true;
		rotate_cube_plane();

	//ry = ry + val; if(ry > 360) ry= ry-360; if(ry < -360) ry= ry+360;

}
//Bleu
void RubixCube::moveRZ()
{

		axis.push_back(5);
		//keypressed.push_back(0);
		dir=1;
		//keypressed.push_back(3);
		rotating=true;
		rotate_cube_plane();
	
	//rz = rz + val; if(rz > 360) rz= rz-360; if(rz < -360) rz= rz+360;
	
}

//vert-rouge
void RubixCube::moveRUF()
{
	//plane.push_back(2);
	axis.push_back(2);
	//keypressed.push_back(1);

	dir=1;
	//keypressed.push_back(3);
	rotating=true;
	cout<<"la moveruf"<<endl;
	rotate_cube_plane();
}


void RubixCube::moveR(bool memesens)
{
	
	//Rouge
	axis.push_back(3);
	if(memesens)dir=1; else dir = 0;
	rotating=false;
	rotate_cube_plane();

}
void RubixCube::moveV(bool memesens)
{
	
	//Vert
	axis.push_back(4);
	if(memesens)dir=1; else dir = 0;
	rotating=false;
	rotate_cube_plane();

}
void RubixCube::moveB(bool memesens)
{
	//Bleu
	axis.push_back(5);
	if(memesens)dir=1; else dir = 0;
	rotating=false;
	rotate_cube_plane();


}
void RubixCube::moveRV(bool memesens)
{
	axis.push_back(2);
	//Rotation d'un coté (celui que nous utilisons tout le temps)
	if(memesens)dir=1; else dir = 0;
	rotating=false;
	rotate_cube_plane();
	//Fin rotation coté
}

//Melange pour le scenario
void RubixCube::melange()
{	//moveRV(false);moveR(false);	moveRV(false);
	//Scenarii 1 : 2xrouge, vert-rouge, bleu, vert-rouge
	//On le fait dans le sens inverse afin que le cube soit fait à la fin de la liste des mouvements
	//moveRV(false);
	//moveB(false);
	//moveRV(false);
	//moveR(false);
	//moveR(false);

	string filename = "Scenario.txt";
	ifstream fichier(filename, ios::in);  // on ouvre en lecture

	if(fichier)
	{
		string ligne;//string RV = "RV"; string R ="R" ; string V = "V"; string B = "B";
		while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
		{
			
			if(ligne.compare("RV"))
				moveRV(false);			
			else if(ligne.compare("R"))
				moveR(false);
			else if(ligne.compare("V"))
				moveV(false);						
			else if(ligne.compare("B"))
				moveB(false);
			
		}
	}	
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl;
 
	fichier.close();
}

void RubixCube::reinit()
{
	rx = 0;
	ry = 0;
	rz = 0;
	x = 0;
	y = 0;
	z = 0; 
}


void RubixCube::recalcAxis()
{
	AxisX[0] = 1; AxisX[1] = 0; AxisX[2] = 0;
	AxisY[0] = 0; AxisY[1] = 1; AxisY[2] = 0;
	AxisZ[0] = 0; AxisZ[1] = 0; AxisZ[2] = 1;

	float NAxisX[3];
	float NAxisY[3];
	float NAxisZ[3];

	/**
	//Rotation Ox
	AxisX[0] = AxisX[0] - (AxisX[0] - AxisX[0]);
	AxisX[1] = AxisX[1] - (-AxisX[2]*sin(DEG_TO_RAD*rx) + AxisX[1]*cos(DEG_TO_RAD*rx) - AxisX[1]);
	AxisX[2] = AxisX[2] - (AxisX[2]*cos(DEG_TO_RAD*rx) +  AxisX[1]*sin(DEG_TO_RAD*rx) - AxisX[2]);
	
	//Rotation Oy
	AxisX[0] = AxisX[0] - (AxisX[0]*cos(DEG_TO_RAD*ry) + AxisX[2]*sin(DEG_TO_RAD*ry) - AxisX[0]);
	AxisX[1] = AxisX[1] - (AxisX[1] - AxisX[1]);
	AxisX[2] = AxisX[2] - (-AxisX[0]*sin(DEG_TO_RAD*ry) +  AxisX[2]*cos(DEG_TO_RAD*ry) - AxisX[2]);
	
	//Rotation Oz
	AxisX[0] = AxisX[0] - (AxisX[0]*cos(DEG_TO_RAD*rz) + AxisX[1]*sin(DEG_TO_RAD*rz) - AxisX[0]);
	AxisX[1] = AxisX[1] - (-AxisX[0]*sin(DEG_TO_RAD*rz) + AxisX[1]*cos(DEG_TO_RAD*rz) - AxisX[1]);
	AxisX[2] = AxisX[2] - (AxisX[2] - AxisX[2]);


	//Rotation Ox
	AxisY[0] = AxisY[0] - (AxisY[0] - AxisY[0]);
	AxisY[1] = AxisY[1] - (-AxisY[2]*sin(DEG_TO_RAD*rx) + AxisY[1]*cos(DEG_TO_RAD*rx) - AxisY[1]);
	AxisY[2] = AxisY[2] - (AxisY[2]*cos(DEG_TO_RAD*rx) +  AxisY[1]*sin(DEG_TO_RAD*rx) - AxisY[2]);
	
	//Rotation Oy
	AxisY[0] = AxisY[0] - (AxisY[0]*cos(DEG_TO_RAD*ry) + AxisY[2]*sin(DEG_TO_RAD*ry) - AxisY[0]);
	AxisY[1] = AxisY[1] - (AxisY[1] - AxisY[1]);
	AxisY[2] = AxisY[2] - (-AxisY[0]*sin(DEG_TO_RAD*ry) +  AxisY[2]*cos(DEG_TO_RAD*ry) - AxisY[2]);

	//Rotation Oz
	AxisY[0] = AxisY[0] - (AxisY[0]*cos(DEG_TO_RAD*rz) + AxisY[1]*sin(DEG_TO_RAD*rz) - AxisY[0]);
	AxisY[1] = AxisY[1] - (-AxisY[0]*sin(DEG_TO_RAD*rz) + AxisY[1]*cos(DEG_TO_RAD*rz) - AxisY[1]);
	AxisY[2] = AxisY[2] - (AxisY[2] - AxisY[2]);


	//Rotation Ox
	AxisZ[0] = AxisZ[0] - (AxisZ[0] - AxisZ[0]);
	AxisZ[1] = AxisZ[1] - (-AxisZ[2]*sin(DEG_TO_RAD*rx) + AxisZ[1]*cos(DEG_TO_RAD*rx) - AxisZ[1]);
	AxisZ[2] = AxisZ[2] - (AxisZ[2]*cos(DEG_TO_RAD*rx) +  AxisZ[1]*sin(DEG_TO_RAD*rx) - AxisZ[2]);

	//Rotation Oy
	AxisZ[0] = AxisZ[0] - (AxisZ[0]*cos(DEG_TO_RAD*ry) + AxisZ[2]*sin(DEG_TO_RAD*ry) - AxisZ[0]);
	AxisZ[1] = AxisZ[1] - (AxisZ[1] - AxisZ[1]);
	AxisZ[2] = AxisZ[2] - (-AxisZ[0]*sin(DEG_TO_RAD*ry) +  AxisZ[2]*cos(DEG_TO_RAD*ry) - AxisZ[2]);

	//Rotation Oz
	AxisZ[0] = AxisZ[0] - (AxisZ[0]*cos(DEG_TO_RAD*rz) + AxisZ[1]*sin(DEG_TO_RAD*rz) - AxisZ[0]);
	AxisZ[1] = AxisZ[1] - (-AxisZ[0]*sin(DEG_TO_RAD*rz) + AxisZ[1]*cos(DEG_TO_RAD*rz) - AxisZ[1]);
	AxisZ[2] = AxisZ[2] - (AxisZ[2] - AxisZ[2]);


	
	*/
}
