#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <ctype.h>
#include <math.h>
#include<GL/glut.h>
#include <iostream>
#include "quadTree.h"
#define KEY_ESC 27

#include <string>

#include <fstream>



//Crear quad tree


//dibuja un simple

quadTree QT (4,300,300);
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
	drawqt(&QT.root);///displayGizmo();

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
	ifstream input;
	string num,num2;

	input.open("C:/Users/Rodrigo/Documents/GitHub/EDA/QuadTree/cc.txt",std::ifstream::in);
	if(!input.is_open())
        cout<<"no se pudo abrir archivo";
	char c;
	while (input.get(c))
    {

        if (isdigit(c))
            num+=c;
        else if(c=='.' || c=='-')
        {
                char temp=c;
                input.get(c);
                if (isdigit(c)){
                    num+=temp;
                    num+=c;
                }
        }
        else if (c==',')
        {
            if(num.size()>0 && num2.size()==0)
            {
                num2=num;
                num="";
            }
            else if (num.size()>0 && num2.size()>0)
            {
                QT.addpoint(make_pair(stod(num2),stod(num)));
                cout<<"Adding point: ( "<<num2<<" , "<<num<<" )"<<endl;
                num=num2="";
            }

        }
    }

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
