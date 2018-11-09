#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include<GL/glut.h>
#include "RTree.h"
#define KEY_ESC 27

#include <fstream>
#include <string>
using namespace std;

//Crear quad tree
RTree RT(500);
int r=0,g=0,b=255;
vector<vector<string>> characteristics;

void extractcharacteristics()
{
    string aux;
    fstream input;
    vector<string > vec_aux;
    input.open("C:/Users/Rodrigo/Documents/GitHub/EDA/QuadTree/crime50k.csv",std::ifstream::in);
	if(!input.is_open())
        cout<<"no se pudo abrir archivo";
	char c;
	while (input.get(c))
    {
        if(c=='\n')
        {
            characteristics.push_back(vec_aux);
            vec_aux.clear();
        }
        else if(c!=';')
        {
            aux+=c;
        }
        else
        {
            vec_aux.push_back(aux);
            aux="";
        }
    }
}

void insertPoints(int xposition, int yposition)
{
    double auxx,auxy;
    for (int i=1; i<characteristics.size();i++)
    {
        auxx=((stod(characteristics[i][xposition])/41.84307539)-1)*40000;
        auxy=((stod(characteristics[i][yposition])/-87.67370828)-1)*40000;
        cout<<i<<". Adding point: ( "<<auxx<<" , "<<auxy<<" )"<<endl;
        RT.Insert(make_pair(auxx,auxy));
    }
}



//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(-300, 0);
	glVertex2d(300, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, -300);
	glVertex2d(0, 300);
	glEnd();
}

void drawrt (Node** p,int jump)
{
    r=(0+jump*20)%255;
    g=(0+jump*20)%255;
    if(!RT.root){return;}
    typecor miniX,miniY,maxiX,maxiY;
    miniX=(*p)->cmin.first;
    miniY=(*p)->cmin.second;
    maxiX=(*p)->cmax.first;
    maxiY=(*p)->cmax.second;
    glBegin(GL_LINES);
    glColor3d(r,g,b);
    glVertex2d(miniX,miniY);
    glVertex2d(maxiX,miniY);
    glColor3d(r,g,b);
    glVertex2d(miniX,miniY);
    glVertex2d(miniX,maxiY);
    glColor3d(r,g,b);
    glVertex2d(miniX,maxiY);
    glVertex2d(maxiX,maxiY);
    glColor3d(r,g,b);
    glVertex2d(maxiX,miniY);
    glVertex2d(maxiX,maxiY);
    glEnd();
    for(int i=0;i<(*p)->data.size();i++)
    {
        glBegin(GL_POINTS);
        glColor3d(0,255,0);
        glVertex2d((*p)->data[i].first, (*p)->data[i].second);
        glEnd();
    }
    jump++;
	for (int i=0;i<(*p)->children.size();i++){
        drawrt(&((*p)->children[i]),jump);
       // cout<<"I:"<<i<<endl;
	}
}





void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    cout<<". Adding point( "<<x-300<<" , "<<300-y<<")"<<endl;
    RT.Insert(make_pair((x-300)*1.0,(300-y)*1.0));

    //convertir x,y
	//insertar un nuevo punto en el quadtree
  }
}

void OnMouseMotion(int x, int y)
{
     //opcional
	 //hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);

	//dibujar quadTree (qt->draw())

	//dibuja el gizmo
	drawrt(&RT.root,0);
	displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana

    extractcharacteristics();
	insertPoints(19,20);

	init_GL(); //funcion de inicializacion de OpenGL
    glPointSize(3);
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);

	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
