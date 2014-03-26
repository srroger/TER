#include "stdafx.h"
#include "Interpreteur.h"

Interpreteur::Interpreteur()
{
	mode = IDLE;
}
Interpreteur::Interpreteur(Viewer* vi , RubixCube* ru,int hr, int sr, int hg, int sg, int hb, int sb)
{
	mode = IDLE;
	this->v = vi;
	this->r = ru;
	hR = hr;
	sR = sr;
	hG = hg;
	sG = sg;
	hB = hb;
	sB = sb;
	faireRotation=true;

	PointGL vide; vide.couleurFacile = RIEN;
	PointPolaire videP; videP.couleurFacile = RIEN;
	for(int  i = 0 ; i <6 ; i++)
	{
		ptCar[i] = vide;
		ptPol[i] = videP;
	}

	cptRefreshFrame =0;
	RefreshEvery = 10;
	toleranceIdle = 20;

	sensiRot =0.1;
	cranRotation = 90;

	seuilCote = 15;
}


Interpreteur::~Interpreteur(void)
{
}


bool Interpreteur::reinitialisation(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
{

	bool good = true;
	bool couleur[3];bool couleurNew[3];
	for(int j = 0 ; j < 3 ; j++)
	{
		couleur[j]=false;
		couleurNew[j]=false;
	}

	for(int i = 0 ; i < tabCentre.size() ; i++)
	{
		switch (tabCentre[i].couleurFacile)
		{	
		case ROUGE : ptCar[0].couleurFacile = tabCentre[i].couleurFacile ; ptCar[0].x = tabCentre[i].point.x; ptCar[0].y = tabCentre[i].point.y; couleur[tabCentre[i].couleurFacile]=true; break;
		case VERT :  ptCar[1].couleurFacile = tabCentre[i].couleurFacile ; ptCar[1].x = tabCentre[i].point.x; ptCar[1].y = tabCentre[i].point.y; couleur[tabCentre[i].couleurFacile]=true;break;
		case BLEU :  ptCar[2].couleurFacile = tabCentre[i].couleurFacile ; ptCar[2].x = tabCentre[i].point.x; ptCar[2].y = tabCentre[i].point.y; couleur[tabCentre[i].couleurFacile]=true;break;
		}

		switch (tabNewCentre[i].couleurFacile)
		{	
		case ROUGE : ptCar[0 +3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[0 + 3].x = tabNewCentre[i].point.x; ptCar[0 + 3].y = tabNewCentre[i].point.y; couleurNew[tabNewCentre[i].couleurFacile]=true; break;
		case VERT :  ptCar[1+3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[1+3].x = tabNewCentre[i].point.x; ptCar[1+3].y = tabNewCentre[i].point.y; couleurNew[tabNewCentre[i].couleurFacile]=true;break;
		case BLEU :  ptCar[2+3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[2+3].x = tabNewCentre[i].point.x; ptCar[2+3].y = tabNewCentre[i].point.y; couleurNew[tabNewCentre[i].couleurFacile]=true;break;
		}

	}

	int i = 0;
	while(i < 3 && couleur[i]==couleurNew[i])
		i++;
	
	if(i < 3)
	{
		if(cptRefreshFrame <= RefreshEvery/3) //On verifie qu'il ne s'agit d'une perte momentanée d'un point
		{good = false; cptRefreshFrame = 0; return good;}
	}
	else
	{
		//Verifie que les points non pas trop bouge
		for(int j = 0 ; j < 3 ; j++)
		{
			if(!( abs(ptCar[j].x - ptCar[j].x) < toleranceIdle && abs(ptCar[j].y - ptCar[j].y) < toleranceIdle) )
			{
				if(cptRefreshFrame <= RefreshEvery/3) //On verifie qu'il ne s'agit d'une perte momentanée d'un point
				{good = false; cptRefreshFrame = 0; return good;}
			}
		}
	}

	if(good)
		cptRefreshFrame++;

	cout << "cptRefresh : " <<cptRefreshFrame <<endl;
	if(cptRefreshFrame >= RefreshEvery)
	{
		cout <<"Reinitialise la scene" <<endl;
		r->reinit();
		cptRefreshFrame = 0;
	}
	return good;
}

//Le point rouge est tout seul, et il se déplace
void Interpreteur::translation(void)
{
	//La size etant de 1 (normalement)

	//Changement de repère car dans opengl et openCV on a pas le meme repère x,y,z
	PointGL vect; 
	vect.x = 0;
	vect.y = -dir[0].y;
	vect.z = -dir[0].x;
	
	float seuille = 1;
	if(abs(vect.x) > seuille) r->moveX(vect.x*0.2);
	if(abs(vect.y) > seuille) r->moveY(vect.y*0.2);
	if(abs(vect.z) > seuille) r->moveZ(vect.z*0.2);

}

//Les rotations se font à l'aide du langage des signes sur 1 doigts
void Interpreteur::rotation(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
{
	//Cherche le centre entre les 3 points
	//Calcul du barycentre
	bool good = false;
	//for(int  i = 0 ; i < 4 ; i++) testeur[i] = false;
	bool couleur[3];bool couleurNew[3];
	for(int j = 0 ; j < 3 ; j++)
	{
		couleur[j]=false;
		couleurNew[j]=false;
	}

	for(int i = 0 ; i < tabCentre.size() ; i++)
	{
		if (cptRefreshFrame==0)
		switch (tabCentre[i].couleurFacile)
		{	
		case ROUGE : ptCar[0].couleurFacile = tabCentre[i].couleurFacile ; ptCar[0].x = tabCentre[i].point.x; ptCar[0].y = tabCentre[i].point.y; couleur[tabCentre[i].couleurFacile]=true; break;
		case VERT :  ptCar[1].couleurFacile = tabCentre[i].couleurFacile ; ptCar[1].x = tabCentre[i].point.x; ptCar[1].y = tabCentre[i].point.y; couleur[tabCentre[i].couleurFacile]=true;break;
		case BLEU :  ptCar[2].couleurFacile = tabCentre[i].couleurFacile ; ptCar[2].x = tabCentre[i].point.x; ptCar[2].y = tabCentre[i].point.y; couleur[tabCentre[i].couleurFacile]=true;break;
		}

		if(cptRefreshFrame <= RefreshEvery)
		switch (tabNewCentre[i].couleurFacile)
		{	
		case ROUGE : ptCar[0 +3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[0 + 3].x = tabNewCentre[i].point.x; ptCar[0 + 3].y = tabNewCentre[i].point.y; couleurNew[tabNewCentre[i].couleurFacile]=true; break;
		case VERT :  ptCar[1+3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[1+3].x = tabNewCentre[i].point.x; ptCar[1+3].y = tabNewCentre[i].point.y; couleurNew[tabNewCentre[i].couleurFacile]=true;break;
		case BLEU :  ptCar[2+3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[2+3].x = tabNewCentre[i].point.x; ptCar[2+3].y = tabNewCentre[i].point.y; couleurNew[tabNewCentre[i].couleurFacile]=true;break;
		}

	}

	cout << "Essaye la rotation" << endl;
	for(int j = 0 ; j < 3 ; j++)
	{
		if(couleur[j])
			cout << j <<",";
		if(couleurNew[j])
			cout << j;
		cout << "||" ;
	}
	

	//En fonction des couleurs vu, ont décide quelle type de rotation il faut faire
	// Bleu Rouge -> Rotation Face
	// Bleu Vert -> Rotation lateral
	// Vert Rouge -> Rotation bas
	/*if(couleur[BLEU] && couleurNew[BLEU] && couleur[ROUGE] && couleurNew[ROUGE])
		rotationFace(tabCentre,tabNewCentre);
	if(couleur[BLEU] && couleurNew[BLEU] && couleur[VERT] && couleurNew[VERT])
		rotationCote(tabCentre,tabNewCentre);
	if( couleur[VERT] && couleurNew[VERT] && couleur[ROUGE] && couleurNew[ROUGE])
		rotationBas(tabCentre,tabNewCentre);*/


	if(couleur[ROUGE] && couleurNew[ROUGE])
		rotationFace(tabCentre,tabNewCentre);
	if(couleur[BLEU] && couleurNew[BLEU])
		rotationCote(tabCentre,tabNewCentre);
	if( couleur[VERT] && couleurNew[VERT])
		rotationBas(tabCentre,tabNewCentre);

	faireRotation=false; // on a fait une rotation
}


//VERT
bool Interpreteur::rotationBas(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
{
	r->moveRY(cranRotation); 
}

//ROUGE
bool Interpreteur::rotationFace(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
{	
	r->moveRX(cranRotation); 
}

//BLEU
bool Interpreteur::rotationCote(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
{
	r->moveRZ(cranRotation); return true;
}

void Interpreteur::launch(vector<Centre> tabCentre, vector<Centre> tabNewCentre){

	if(tabNewCentre.size() != 0 && tabCentre.size() != 0 && tabNewCentre.size() == tabCentre.size())
	{
		
		dir = direction(tabCentre,tabNewCentre);
		mode = tabNewCentre.size();
		switch (mode)
		{
		case 0: {faireRotation=true;break;}
		case 1: {if(faireRotation) rotation(tabCentre,tabNewCentre); break;}
		case 2: {if(tabCentre[0].couleurFacile == ROUGE || tabCentre[1].couleurFacile == ROUGE ) translation(); break;}
		case 3:	{if(tabCentre[0].couleurFacile == ROUGE || tabCentre[1].couleurFacile == ROUGE || tabCentre[2].couleurFacile == ROUGE ) translation(); break;}
		//case 1 :{ if(tabCentre[0].couleurFacile == ROUGE ) translation(); break;} // N effectue la translation que si c'est un marqueur rouge
		//case 2 : {rotation(tabCentre,tabNewCentre); break;}
		//case 3 : {reinitialisation(tabCentre,tabNewCentre); break;}
			default :{ break;}
		}

	}
	

	
	//Repere le mode

}


vector<CvPoint> Interpreteur::direction(vector<Centre> tabCentre, vector<Centre> tabNewCentre){
	vector<CvPoint> DirDeplacement;
	for(int i=0;i<tabCentre.size();i++){
		CvPoint pt;
		pt.x= tabCentre.at(i).point.x - tabNewCentre.at(i).point.x;
		pt.y= tabCentre.at(i).point.y - tabNewCentre.at(i).point.y;
		DirDeplacement.push_back(pt);
	}
	
	return DirDeplacement;
}














//--------------------------------------------------------------------------------------------------///
//-------------------------Vielles versions de méthodes qui ne sont plus utilisés--------------------///
//--------------------------------------------------------------------------------------------------///

//void Interpreteur::rotation(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
//{
//	//Cherche le centre entre les 3 points
//	//Calcul du barycentre
//	bool good = false;
//	if (cptRefreshFrame==0) for(int  i = 0 ; i < 6 ; i++) testeur[i] = false;
//	for(int i = 0 ; i < 3 ; i++)
//	{
//		if (cptRefreshFrame==0)
//		switch (tabCentre[i].couleurFacile)
//		{	
//		case ROUGE : ptCar[0].couleurFacile = tabCentre[i].couleurFacile ; ptCar[0].x = tabCentre[i].point.x; ptCar[0].y = tabCentre[i].point.y; testeur[0]=true;break;
//		case VERT :  ptCar[1].couleurFacile = tabCentre[i].couleurFacile ; ptCar[1].x = tabCentre[i].point.x; ptCar[1].y = tabCentre[i].point.y; testeur[1]=true;break;
//		case BLEU :  ptCar[2].couleurFacile = tabCentre[i].couleurFacile ; ptCar[2].x = tabCentre[i].point.x; ptCar[2].y = tabCentre[i].point.y; testeur[2]=true;break;
//		}
//
//		if(cptRefreshFrame <= RefreshEvery)
//		switch (tabNewCentre[i].couleurFacile)
//		{	
//		case ROUGE : ptCar[0 +3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[0 + 3].x = tabNewCentre[i].point.x; ptCar[0 + 3].y = tabNewCentre[i].point.y; testeur[3]=true;break;
//		case VERT :  ptCar[1+3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[1+3].x = tabNewCentre[i].point.x; ptCar[1+3].y = tabNewCentre[i].point.y; testeur[4]=true;break;
//		case BLEU :  ptCar[2+3].couleurFacile = tabNewCentre[i].couleurFacile ; ptCar[2+3].x = tabNewCentre[i].point.x; ptCar[2+3].y = tabNewCentre[i].point.y; testeur[5]=true;break;
//		}
//
//	}
//	if (cptRefreshFrame==0)
//		if(testeur[0] && testeur[1] && testeur[2])
//			good = true;
//	if (cptRefreshFrame!=0)
//		if(testeur[3] && testeur[4] && testeur[5])
//			good = true;
//
//	if(testeur[0] && testeur[1] && testeur[2] && testeur[3] && testeur[4] && testeur[5])
//	{
//		//Fait le calcul uniquement au bon moment
//		if(cptRefreshFrame == RefreshEvery)
//		{
//			if(!rotationCote(tabCentre,tabNewCentre))
//				rotationFace(tabCentre,tabNewCentre);
//		}
//	}
//	else 
//	{
//		cout << cptRefreshFrame <<"Manque les couleurs : ";
//		for(int i = 0 ; i < 6 ; i++)
//		{
//			
//			if(!testeur[i])
//			{
//				cout << i <<",";
//			}
//		}
//			cout <<endl;
//	}
//
//
//	if(good) //Verifie que dans la frame il y a bien eu assez de couleur
//		cptRefreshFrame++;
//	if (cptRefreshFrame >RefreshEvery)
//	{
//		cptRefreshFrame=0;
//	}
//}
//
//void Interpreteur::rotationFace(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
//{
//				////Calcul du barycentre C
//			//C.x = (ptCar[0].x + ptCar[1].x + ptCar[2].x)/3;
//			//C.y = (ptCar[0].y + ptCar[1].y + ptCar[2].y)/3;
//			//C2.x = (ptCar[0+3].x + ptCar[1+3].x + ptCar[2+3].x)/3;
//			//C2.y = (ptCar[0+3].y + ptCar[1+3].y + ptCar[2+3].y)/3;
//
//			////Place les points en polaire avec pour repaire de centre C.
//
//			//ptPol[0].r = sqrt( pow((ptCar[0].x-C.x),2) + pow((ptCar[0].y - C.y),2) );
//			//ptPol[1].r = sqrt( pow((ptCar[1].x-C.x),2) + pow((ptCar[1].y - C.y),2) );
//			//ptPol[2].r = sqrt( pow((ptCar[2].x-C.x),2) + pow((ptCar[2].y - C.y),2) );
//			//ptPol[3].r = sqrt( pow(( ptCar[0+3].x-C2.x),2) + pow(( ptCar[0+3].y - C2.y),2) );
//			//ptPol[4].r = sqrt( pow(( ptCar[1+3].x-C2.x),2) + pow(( ptCar[1+3].y - C2.y),2) );
//			//ptPol[5].r = sqrt( pow(( ptCar[2+3].x-C2.x),2) + pow(( ptCar[2+3].y - C2.y),2) );
//
//			//ptPol[0].teta = acos( ( ptCar[0].x-C.x) / ptPol[0].r);
//			//ptPol[1].teta = acos( (ptCar[1].x-C.x) / ptPol[1].r);
//			//ptPol[2].teta = acos( (ptCar[2].x-C.x) / ptPol[2].r);
//			//ptPol[3].teta = acos( ( ptCar[0+3].x-C2.x) / ptPol[3].r);
//			//ptPol[4].teta = acos( ( ptCar[1+3].x-C2.x) / ptPol[4].r);
//			//ptPol[5].teta = acos( ( ptCar[2+3].x-C2.x) / ptPol[5].r);
//
//			//Calcul du barycentre C
//			C.x = (ptCar[0].x + ptCar[1].x + ptCar[2].x)/3;
//			C.y = (ptCar[0].y + ptCar[1].y + ptCar[2].y)/3;
//			C2.x = (ptCar[0+3].x + ptCar[1+3].x + ptCar[2+3].x)/3;
//			C2.y = (ptCar[0+3].y + ptCar[1+3].y + ptCar[2+3].y)/3;
//			PointGL Cmoy;
//			//Cmoy.x = C.x + (C2.x-C.x);
//			//Cmoy.y = C.y + (C2.y-C.y);
//			Cmoy = C;
//			//Place les points en polaire avec pour repaire de centre C.
//
//			ptPol[0].r = sqrt( pow((ptCar[0].x-Cmoy.x),2) + pow((ptCar[0].y - Cmoy.y),2) );
//			ptPol[1].r = sqrt( pow((ptCar[1].x-Cmoy.x),2) + pow((ptCar[1].y - Cmoy.y),2) );
//			ptPol[2].r = sqrt( pow((ptCar[2].x-Cmoy.x),2) + pow((ptCar[2].y - Cmoy.y),2) );
//			ptPol[3].r = sqrt( pow(( ptCar[0+3].x-Cmoy.x),2) + pow(( ptCar[0+3].y - Cmoy.y),2) );
//			ptPol[4].r = sqrt( pow(( ptCar[1+3].x-Cmoy.x),2) + pow(( ptCar[1+3].y - Cmoy.y),2) );
//			ptPol[5].r = sqrt( pow(( ptCar[2+3].x-Cmoy.x),2) + pow(( ptCar[2+3].y - Cmoy.y),2) );
//
//			ptPol[0].teta = acos(  ( ptCar[0].x-Cmoy.x) / ptPol[0].r);
//			ptPol[1].teta = acos( (ptCar[1].x-Cmoy.x) / ptPol[1].r);
//			ptPol[2].teta = acos( (ptCar[2].x-Cmoy.x) / ptPol[2].r);
//			ptPol[3].teta = acos( ( ptCar[0+3].x-Cmoy.x) / ptPol[3].r);
//			ptPol[4].teta = acos( ( ptCar[1+3].x-Cmoy.x) / ptPol[4].r);
//			ptPol[5].teta = acos( ( ptCar[2+3].x-Cmoy.x) / ptPol[5].r);
//			float delta[3];
//			for(int i = 0 ; i < 3 ; i++)
//			{
//				delta[i] = ptPol[i+3].teta - ptPol[i].teta;
//				//cout << endl << delta[i] << ":"<<i;
//			}
//
//			//Droite ou gauche Si delta > 0 : gauche sinon droite si delta trop proche de 0 rien du tout
//			//Face donc sur l'axe d'openGL
//			float sommeDelta = 0; 
//			float moyDelta =0; int nbDelta=0;
//			for(int i =0; i <3 ; i++)
//			{
//				sommeDelta += delta[i];  nbDelta++;
//			}
//			moyDelta =sommeDelta / nbDelta;
//			cout << "d :" << moyDelta << endl;
//		
//			if(abs(moyDelta) > sensiRot)
//			if(moyDelta < 0) 
//			{
//				r->moveRX(cranRotation);
//			}else
//				r->moveRX(-cranRotation);
//
//}
//
//
//bool Interpreteur::rotationCote(vector<Centre> tabCentre, vector<Centre> tabNewCentre)
//{
//	bool done = true;
//	float sommeD = 0;
//	float moyD = 0; int nbD = 0;
//	for(int  i = 0 ; i < 3 ; i++)
//	{
//		for(int j = i+1 ; j <3 ; j++)
//		{
//			sommeD = sommeD + abs(ptCar[i].x - ptCar[j].x);
//			//cout <<ptCar[i].x  << " - "<<   ptCar[j].x << "=" << (ptCar[i].x - ptCar[j].x) << "()" << sommeD <<endl;
//
//			nbD++;
//		}
//	}
//	moyD = sommeD / (float)nbD;
//	cout <<"Moyd "<< moyD <<endl;
//	if(abs(moyD) > seuilCote)
//		done = false;
//
//	if(done)
//	{
//		//Regade si il y a eu changement d'ordre et dans quel sens
//		//Creer deux tableau ranger par ordre des y croissant
//		PointGL tri1[3];
//		PointGL tri2[3];
//
//		for(int i = 0 ; i<3 ; i++)
//		{
//			tri1[i] = ptCar[i];
//			tri2[i] = ptCar[i+3];
//		}
//
//
//
//		int i, _i;
//		for(_i=0;_i<3;++_i)
//		{
//			for(i=0;i<3-1;i++)
//			{
//				if(tri1[i].y > tri1[i+1].y)
//				{
//					PointGL inter;
//
//					inter = tri1[i];
//					tri1[i] = tri1[i+1];
//					tri1[i+1] = inter;
//				}
//			}
//		}
//		
//		for(_i=0;_i<3;++_i)
//		{
//			for(i=0;i<3-1;i++)
//			{
//				if(tri2[i].y > tri2[i+1].y)
//				{
//					PointGL inter;
//
//					inter = tri2[i];
//					tri2[i] = tri2[i+1];
//					tri2[i+1] = inter;
//				}
//			}
//		}
//		
//		for(int j =0; j <3 ;j++) cout << tri1[j].couleurFacile <<" (y:"<< tri1[j].y <<")->"  <<tri2[j].couleurFacile<<"(y:"<<tri2[j].y<<")" <<endl;
//		
//		//Fin tri
//		int sens =1;//0 pour bas, 1 pour haut
//		i = 0;
//		while(tri1[i].couleurFacile == tri2[i].couleurFacile && i<3)
//		{
//			i++;
//		}
//		if(i<3)
//		{
//			if(i==2)
//			{
//				if(tri1[i].couleurFacile == tri2[0].couleurFacile)//Alors le point le plus en bas est passe en premier, donc bas
//					sens = 0;
//			}else
//				if((tri1[i].couleurFacile == tri2[i+1].couleurFacile))
//					sens = 0;
//
//			//Fait la rotation sur le cote droit par defaut
//			if(sens == 0) 
//			{
//				r->moveRZ(cranRotation*2);
//			}else
//				r->moveRZ(-cranRotation*2);
//		}
//		else
//		{
//			//Ne fait rien
//		}
//	}
//	return done;
//}