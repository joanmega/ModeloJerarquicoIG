#include "stdlib.h"
#include "stdio.h"
#include <GL/glut.h>
#include <ctype.h>
#include "user_code.h"
#include "cstring"

// Variables Globales

int opcion = 5;

MallaTVT miMalla;
Muneco muneco;

const char *archivoPLY;
bool munecoActivo = false;

// tamaño de los ejes
const int AXIS_SIZE=5000;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,UI_window_width=500,UI_window_height=500;

//**************************************************************************
//
//***************************************************************************

void clear_window()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
	//  Front_plane>0  Back_plane>PlanoDelantero)
	glFrustum(-Window_width,Window_width,-Window_height,Window_height,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

	// posicion del observador
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-Observer_distance);
	glRotatef(Observer_angle_x,1,0,0);
	glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	glBegin(GL_LINES);
	// eje X, color rojo
	glColor3f(1,0,0);
	glVertex3f(-AXIS_SIZE,0,0);
	glVertex3f(AXIS_SIZE,0,0);
	// eje Y, color verde
	glColor3f(0,1,0);
	glVertex3f(0,-AXIS_SIZE,0);
	glVertex3f(0,AXIS_SIZE,0);
	// eje Z, color azul
	glColor3f(0,0,1);
	glVertex3f(0,0,-AXIS_SIZE);
	glVertex3f(0,0,AXIS_SIZE);
	glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//***************************************************************************

void draw_objects()
{
	switch(opcion){
		// Dibujamos en modo puntos.
		case 1:{
			if (munecoActivo == false){
				draw_plyPuntos(miMalla);
			}else{
				muneco.draw_muneco(1);
			}
			break;
		}
		// Dibujamos en modo alambre
		case 2:{
			if (munecoActivo == false){
				draw_alambre(miMalla);
			}else{
				muneco.draw_muneco(2);
			}
			break;
		}
		// Dibujamos en modo solido
		case 3:{
			if (munecoActivo == false){
				draw_solido(miMalla);
			}else{
				muneco.draw_muneco(3);
			}
			break;
		}
		// Dibujamos en modo ajedrez
		case 4:{
			if (munecoActivo == false){
				draw_ajedrez(miMalla);
			}else{
				muneco.draw_muneco(4);
			}
			break;
		}
		// Cargamos el archivo PLY
		case 5:{
			draw_ajedrez(miMalla);
			break;
		}
		// Cargamos el objeto de rotación
		case 6:{
			draw_ajedrez(miMalla);
			break;
		}
		// Cargamos el objeto jerárquico
		case 7:{
			muneco.draw_muneco(4);
			break;
		}
	}
}



//**************************************************************************
//
//***************************************************************************

void draw_scene(void)
{

	clear_window();
	change_observer();
	draw_axis();
	draw_objects();
	glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
	change_projection();
	glViewport(0,0,Ancho1,Alto1);
	glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1,int x,int y)
{
	// Salir de la aplicación
	if (toupper(Tecla1)=='Q') {
		exit(0);
	}
	// Visualización en modo puntos.
	if (toupper(Tecla1)=='P') {
		opcion = 1;
	}
	// Visualización en modo alambre
	if (toupper(Tecla1)=='L') {
		opcion = 2;
	}
	// Visualización en modo sólido
	if (toupper(Tecla1)=='S') {
		opcion = 3;
	}
	//Visualización en modo ajedrez
	if (toupper(Tecla1)=='A') {
		opcion = 4;
	}
	// Cargamos el PLY
	if (toupper(Tecla1)=='1') {
		munecoActivo = false;
		miMalla.clear();
		miMalla.read(archivoPLY);
		opcion = 5;
	}
	// Cargamos el modelo por revolución
	if (toupper(Tecla1)=='2') {
		munecoActivo = false;
		int numLados = 24;
		miMalla.clear();
		miMalla.read("./peon.ply");
		miMalla.crearObjeto(numLados);
		opcion = 6;
	}
	// Cargamos el modelos jerárquico
	if (toupper(Tecla1)=='3') {
		munecoActivo = true;
		opcion = 7;
	}
	
	//Giramos la cabeza del muñeco
	if (Tecla1 == 'z'){
		muneco.girarCabeza(1);
	}
	if (Tecla1 == 'Z'){
		muneco.girarCabeza(-1);
	}
	
	//Movemos las piernas del muñeco
	
	if (Tecla1 == 'x'){
		muneco.girarPiernas(1);
	}
	if (Tecla1 == 'X'){
		muneco.girarPiernas(-1);
	}
	
	//Movemos los Brazos del muñeco
	
	if (Tecla1 == 'c'){
		muneco.girarBrazos(1);
	}
	if (Tecla1 == 'C'){
		muneco.girarBrazos(-1);
	}
	glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_keys(int Tecla1,int x,int y)
{

	switch (Tecla1){
		case GLUT_KEY_LEFT:Observer_angle_y--;break;
		case GLUT_KEY_RIGHT:Observer_angle_y++;break;
		case GLUT_KEY_UP:Observer_angle_x--;break;
		case GLUT_KEY_DOWN:Observer_angle_x++;break;
		case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
		case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
		default: glutPostRedisplay(); break;
	}
	glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
	// se inicalizan la ventana y los planos de corte
	Window_width=.5;
	Window_height=.5;
	Front_plane=1;
	Back_plane=1000;

	// se inicia la posicion del observador, en el eje z
	Observer_distance=3*Front_plane;
	Observer_angle_x=0;
	Observer_angle_y=0;

	// se indica cual sera el color para limpiar la ventana	(r,v,a,al)
	// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
	glClearColor(1,1,1,1);

	// se habilita el z-bufer
	glEnable(GL_DEPTH_TEST);
	change_projection();
	glViewport(0,0,UI_window_width,UI_window_height);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv)
{
	if (argc < 2){
		exit(0);
	}
	
	// Si los parámetros introducidos no son los mínimos
		
	archivoPLY = argv[1];
		
	// Dibujamos el modelo jeraquico
	// se llama a la inicialización de glut
	glutInit(&argc, argv);

	// se indica las caracteristicas que se desean para la visualización con OpenGL
	// Las posibilidades son:
	// GLUT_SIMPLE -> memoria de imagen simple
	// GLUT_DOUBLE -> memoria de imagen doble
	// GLUT_INDEX -> memoria de imagen con color indizado
	// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
	// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
	// GLUT_DEPTH -> memoria de profundidad o z-bufer
	// GLUT_STENCIL -> memoria de estarcido
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// posicion de la esquina inferior izquierdad de la ventana
	glutInitWindowPosition(UI_window_pos_x,UI_window_pos_y);

	// tamaño de la ventana (ancho y alto)
	glutInitWindowSize(UI_window_width,UI_window_height);

	// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
	// al bucle de eventos)
	glutCreateWindow("Practica 3");

	// asignación de la funcion llamada "dibujar" al evento de dibujo
	glutDisplayFunc(draw_scene);
	// asignación de la funcion llamada "cambiar_tamanio_ventana" al evento correspondiente
	glutReshapeFunc(change_window_size);
	// asignación de la funcion llamada "tecla_normal" al evento correspondiente
	glutKeyboardFunc(normal_keys);
	// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
	glutSpecialFunc(special_keys);

	// funcion de inicialización
	initialize();

	// inicio del bucle de eventos
	glutMainLoop();
	return 0;
}
