// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#include <math.h>
#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"

// L�gg till egna globaler h�r efter behov.
float maxDistance = 130.0f; 
float cohW = 0.004;
float avoidW = 0.21;
float alignW = 0.008;

float distBtwPts(FPoint j, FPoint i)
{
	float diffX = powf(j.v - i.v, 2);
	float diffY = powf(j.h - i.h,2);
	float res = sqrtf(diffX + diffY);
	return res;
}



FPoint calcAvoidance(FPoint j, FPoint i)
{
	FPoint res;
	float length = distBtwPts(j,i);
	res.h = (j.h - i.h) / (length);
	res.v = (j.v - i.v) / ( length);
	return res;
}

void SpriteBehavior() // Din kod!
{
	SpritePtr sprite;
	float count;
	sprite = gSpriteRoot;
	SpritePtr sprite2;
	sprite2 = gSpriteRoot;
	FPoint speedDiff, averagePos, avoidanceVector;
	do{
		speedDiff.h = 0;
		speedDiff.v = 0;
		averagePos.h = 0;
		averagePos.v = 0;
		avoidanceVector.h = 0;
		avoidanceVector.v = 0;
		sprite2 = gSpriteRoot;
		count = 0;
		do {
			if(sprite != sprite2 && distBtwPts(sprite2->position, sprite->position) < maxDistance) 
			{
				//alignment
				speedDiff.h += sprite2->speed.h - sprite->speed.h;
				speedDiff.v += sprite2->speed.v - sprite->speed.v; 
				//cohesion
				averagePos.h += sprite2->position.h;
				averagePos.v += sprite2->position.v;
				// seperation
				FPoint avoid = calcAvoidance(sprite->position, sprite2->position);
				avoidanceVector.h += avoid.h;
				avoidanceVector.v += avoid.v;
				count++;	
			}
			sprite2 = sprite2->next;
		} while(sprite2 != NULL);
		if(count > 0)
		{
			
			sprite->spdDiff.h = speedDiff.h / count;
			sprite->spdDiff.v = speedDiff.v / count;
			sprite->avPos.h = averagePos.h / count;
			sprite->avPos.v = averagePos.v / count;
			sprite->avoidVec.h = avoidanceVector.h / count;
			sprite->avoidVec.v = avoidanceVector.v / count;
		}
		sprite = sprite->next;
	} while(sprite != NULL);

	SpritePtr newSp;
	newSp = gSpriteRoot;
	do {
			
		newSp->speed.h += (newSp->avPos.h - newSp->position.h) * cohW;
		newSp->speed.v += (newSp->avPos.v - newSp->position.v) * cohW;

		newSp->speed.h += (newSp->avoidVec.h) * avoidW;
		newSp->speed.v += (newSp->avoidVec.v) * avoidW;

		newSp->speed.h += (newSp->spdDiff.h) * alignW;
		newSp->speed.v += (newSp->spdDiff.v) * alignW;

		newSp->position.h += newSp->speed.h;
		newSp->position.v += newSp->speed.v;
		
		newSp = newSp->next;
	} while(newSp != NULL);
}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 250, 200, -1, 1.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
