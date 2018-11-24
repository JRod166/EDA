#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <ctype.h>
#include <math.h>
#include<GL/glut.h>
#include <iostream>
#include "quadTree.h"
#define KEY_ESC 27
#include <vector>

#include <string>

#include <fstream>

#define PI 3.1415926535897932384626433832795
int radio=50;
double px,py;

//Crear quad tree
//dibuja un simple
quadTree QT (350,300,300);
vector <vector <string> > characteristics;
vector <point> area;

void DrawCircle(double cx, double cy, double r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    double x,y,theta;
    glColor3d(255,0,0);
    //cout<<"RADIOO"<<radio<<endl;
    for (int ii = 0; ii < num_segments; ii++)   {
        theta = 2.0f * 3.1415926f * double(ii) / double(num_segments);//get the current angle
        x = r * cosf(theta);//calculate the x component
        y = r * sinf(theta);//calculate the y component
        //cout<<"["<<x+cx<<","<<y+cy<<"]\t";
        glVertex2d(x + cx, y + cy);//output vertex
    }

    //cout << endl;
    glEnd();
}

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
        QT.addpoint(make_pair(auxx,auxy));
    }
}

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
	glBegin(GL_POINTS);
	glColor3d(255,67,0);
	glVertex2d(150, 150);
	glColor3d(255,67,0);
	glVertex2d(150, 180.8798);
	glEnd();
}

void drawqt (node** p)
{
    if ((*p)->children[0]==0)
    {
        for(int i=0;i<(*p)->data.size();i++)
        {
            glBegin(GL_POINTS);
            glColor3d(0,255,0);
            glVertex2d((*p)->data[i].first, (*p)->data[i].second);
            glEnd();
        }
    return;
    }
    typecor dis=(*p)->cmax.first-(*p)->cmin.first;
    dis/=2;
    typecor miniX,miniY,maxiX,maxiY;
    miniX=(*p)->cmin.first;
    miniY=(*p)->cmin.second;
    maxiX=(*p)->cmax.first;
    maxiY=(*p)->cmax.second;
    glBegin(GL_LINES);
	glColor3d(0,0,255);
	glVertex2d(miniX,miniY+dis);
	glVertex2d(maxiX,miniY+dis);
	glColor3d(0,0, 255);
	glVertex2d(miniX+dis,miniY);
	glVertex2d(miniX+dis,maxiY);
	glEnd();
	for (int i=0;i<4;i++){
        drawqt(&((*p)->children[i]));
       // cout<<"I:"<<i<<endl;
	}
}

void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON  && state == GLUT_DOWN)
  {
    QT.addpoint(make_pair((x-300)*1.0,(300-y)*1.0));
    cout<<". Adding point( "<<x-300<<" , "<<300-y<<")"<<endl;
    //convertir x,y
	//insertar un nuevo punto en el quadtree
  }
}

void OnMouseMotion(int x, int y)
{
    //Cpunto p(x,tamanho-y);
    //Cpunto t = encuadrar_punto(p);
    //radio = (double)(qt->m_qtfin.m_x - qt->m_qtini.m_x)/10;
    px = x-300;
    py = 300-y;
    area.resize(0);
    QT.incircle(QT.root,&area,make_pair(px*1.0,py*1.0),radio);
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
    DrawCircle(px,py,radio,100);
	//dibujar quadTree (qt->draw())

	//dibuja el gizmo
	drawqt(&QT.root);///displayGizmo();

	glBegin(GL_POINTS);
	glPointSize(3);
	glColor3d(255,255,255);
	for(int i=0;i<area.size();i++)
    {
        glVertex2d(area[i].first,area[i].second);
        //cout<<"punto: "<<area[i].first<<" , "<<area[i].second<<endl;
    }
	glEnd();

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
	glutCreateWindow("QuadTree"); //titulo de la ventana
	extractcharacteristics();
	insertPoints(19,20);

	init_GL(); //funcion de inicializacion de OpenGL
	glPointSize(1.5);
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
