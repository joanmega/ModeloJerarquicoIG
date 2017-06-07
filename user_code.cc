#include "user_code.h"
#include <GL/glut.h>

float PI = 3.14159265359;

void draw_ajedrez(MallaTVT miMalla);
void draw_alambre(MallaTVT miMalla);
void draw_solido(MallaTVT miMalla);

//**************************************************************************
// 				DEFINICION DE LOS METODOS DE CLASE
//**************************************************************************


//**************************************************************************
// 			Métodos de la clase malla
//***************************************************************************

MallaTVT::MallaTVT(){
	numCaras = 0;
	numVertices = 0;
}

void MallaTVT::clear(){
	vertices.clear();
	caras.clear();
	numCaras = 0;
	numVertices = 0;
}

void MallaTVT::pasarEstructura(vector<float> v){
	for (unsigned i = 0; i<v.size(); i+=3){
		_vertex3<float> punto(v[i], v[i+1], v[i+2]);
		vertices.push_back(punto);
	}
}

MallaTVT::MallaTVT(const char *cadena){
	read(cadena);
}

void MallaTVT::read(const char *cadena){
	vector<float> v;
	ply::read(cadena, v, caras);
	pasarEstructura(v);
	numCaras = caras.size();
	numVertices = vertices.size();
}

unsigned int MallaTVT::getNumVertices(){
	return numVertices;
}
int MallaTVT::getNumCaras(){
	return numCaras;
}
vector <_vertex3<float> > MallaTVT::getVertices(){
	return vertices;
}
vector<int> MallaTVT::getCaras(){
	return caras;
}

void MallaTVT::crearCaras(int numVerticesPerfil, int traslaciones){
	for (int j = 0; j < numVerticesPerfil*traslaciones; j+=numVerticesPerfil){
		for (int i = 0; i < numVerticesPerfil-1; i++){
		
		// Creamos las caras
			// Triángulo Inferior
			caras.push_back(i+j);
			caras.push_back(i+1+j);
			caras.push_back(i+numVerticesPerfil+j);
			
			// Triángulo Superior
			caras.push_back(i+numVerticesPerfil+j);
			caras.push_back(i+1+j);
			caras.push_back(i+numVerticesPerfil+1+j);
			
		}
	}
		
	//Creamos las caras de la tapa superior
	for (int j = 0; j < numVerticesPerfil*traslaciones; j+=numVerticesPerfil){
		caras.push_back(numVertices-1);
		caras.push_back(j+numVerticesPerfil-1);
		caras.push_back(j+(2*numVerticesPerfil)-1);
	}
	
	//Creamos las caras de la tapa inferior
	for (int j = 0; j < numVerticesPerfil*traslaciones; j+=numVerticesPerfil){
		caras.push_back(numVertices-2);
		caras.push_back(j);
		caras.push_back(j+numVerticesPerfil);
	}	
	
	// Actualizamos el número de caras
	
	numCaras = caras.size();
	
}

void MallaTVT::crearObjeto(int traslaciones){
	
	int numVerticesPerfil = numVertices;
	
	// Para crear los vértices de arriba a abajo.
	
	_vertex3<float> abajo(0, vertices[0].y, 0);
	_vertex3<float> arriba(0, vertices[vertices.size()-1].y, 0);
	
	// Realizamos las traslaciones del perfil obtenido del PLY
	
	for (int i = 0; i<numVertices*traslaciones; i++){
		_vertex3<float> p;
		p.x = cos(2*PI/traslaciones)*vertices[i].x+sin(2*PI/traslaciones)*vertices[i].z;
		p.y = vertices[i].y;
		p.z = -sin(2*PI/traslaciones)*vertices[i].x+cos(2*PI/traslaciones)*vertices[i].z;
		vertices.push_back(p);
	}
	
	// 	Introducimos el punto central de abajo y arriba.
	
	vertices.push_back(abajo);
	vertices.push_back(arriba);
	
	// Actualizamos el numero de vértices.
		
	numVertices = vertices.size();
		
	//Creamos las caras del objeto.
	
	caras.clear();
	
	crearCaras(numVerticesPerfil, traslaciones);
	
}


void MallaTVT::crearNormales(){
	_vertex3<float> ab, bc, normal;
	_vertex3<float> vNormal (0,0,0);
	
	
	for (int i=0; i<numVertices; i++){
		normalesVertices.push_back(vNormal);
	}
	
	for (int i=0; i<numCaras; i+=3){
		
		// Normalización de las caras
		
		ab.x = (vertices[caras[i+1]].x - vertices[caras[i]].x);
		ab.y = (vertices[caras[i+1]].y - vertices[caras[i]].y);
		ab.z = (vertices[caras[i+1]].z - vertices[caras[i]].z);
		
		bc.x = (vertices[caras[i+2]].x - vertices[caras[i]].x);
		bc.y = (vertices[caras[i+2]].y - vertices[caras[i]].y);
		bc.z = (vertices[caras[i+2]].z - vertices[caras[i]].z);
		
		//Calculamos la normal de la cara.
		normal.x = (ab.y * bc.z) - (ab.z * bc.y);
		normal.y = (ab.z * bc.x) - (ab.x * bc.z);
		normal.z = (ab.x * bc.y) - (ab.y * bc.x);
		
		normalesCaras.push_back(normal.normalize());
		
		// Normalización de los vértices de cada cara.
		
		normalesVertices[caras[i]] += normal.normalize();
		normalesVertices[caras[i+1]] += normal.normalize();
		normalesVertices[caras[i+2]] += normal.normalize();
		
	}
	
	
// 	cout << "Normalización de las caras" << endl;
// 	int j = 0;
// 	for (unsigned int i=0; i<caras.size(); i+=3){
// 		cout << "Cara: " << j << " -> [" << caras[i] << "," << caras[i+1] << "," << caras[i+2] << "]" << endl;
// 		normalesCaras[j].show_values();
// 		j++;
// 	}
	
	
// 	cout << "Normalización de los vértices" << endl;
	// Normalizamos los vértices del vector de vertices normalizados
	
	for (unsigned i = 0; i< normalesVertices.size(); i++){
		normalesVertices[i] = normalesVertices[i].normalize();
// 		cout << "Vertice: " << i << " -> ";
// 		normalesVertices[i].show_values();
	}
}

void MallaTVT::crearPerfil(_vertex3<float> punto){
	vertices.push_back(punto);
	numVertices = vertices.size();
}

void MallaTVT::crearSemiesfera(_vertex3<float> pInicial){
	_vertex3<float> punto = pInicial;
	this->crearPerfil(pInicial);
	for (int i = 0; i<10; i++){
		punto.x = cos(PI/20)*punto.x - sin(PI/20)*punto.y;
		punto.y = sin(PI/20)*punto.x + cos(PI/20)*punto.y;
		punto.z = 0;
		this->crearPerfil(punto);
	}
	this->crearObjeto(24);
}

void MallaTVT::crearCilindro(_vertex3<float> puntoInferior, _vertex3<float> puntoSuperior){
	this->crearPerfil(puntoSuperior);
	this->crearPerfil(puntoInferior);
 	this->crearObjeto(24);
}


//**************************************************************************
// 			Métodos de la clase muñeco
//***************************************************************************
		
Muneco::Muneco(){
	// Creamos el muñeco creando su cuerpo por revolución a partir de los pefiles.

	gradosCabeza = 0;
	gradosPiernas = 0;
	gradosBrazos = 0;
	finPaso = false;
	
// 	Creamos la cabeza del muñeco.
	
	_vertex3<float> puntoCabeza (3,0,0);
	Cabeza.crearSemiesfera(puntoCabeza);
	
// 	Creamos la cabeza del muñeco.
	
	_vertex3<float> puntoTroncoInferior(3, 0, 0);
	_vertex3<float> puntoTroncoSuperior(3, 5, 0);
	Tronco.crearCilindro(puntoTroncoInferior, puntoTroncoSuperior);
	
// 	Creamos el hombro del muñeco.
	
	_vertex3<float> puntoHombroInferior(0.25, 0, 0);
	_vertex3<float> puntoHombroSuperior(0.25, 0.25, 0);
	Hombro.crearCilindro(puntoHombroInferior, puntoHombroSuperior);
	
// 	Creamos el brazo del muñeco
	
	_vertex3<float> puntoBrazoInferior(0.5, 0, 0);
	_vertex3<float> puntoBrazoSuperior(0.5, 2.5, 0);
	Brazo.crearCilindro(puntoBrazoInferior, puntoBrazoSuperior);
	
// 	Creamos la pierna del muñeco.
	
	_vertex3<float> puntoPiernaInferior(0.5, 0, 0);
	_vertex3<float> puntoPiernaSuperior(0.5, 2.5, 0);
	Pierna.crearCilindro(puntoPiernaInferior, puntoPiernaSuperior);
	
// 	Creamos los extremos semiesféricos de las extremidades
	
	_vertex3<float> puntoTerminalExtremidad (0.5,0,0);
	TerminalExtremidad.crearSemiesfera(puntoTerminalExtremidad);
	
// 	Creamos la antena del muñeco.
	
	_vertex3<float> puntoAntenaInferior(0.05, 0, 0);
	_vertex3<float> puntoAntenaSuperior(0.05, 0.5, 0);
	Antena.crearCilindro(puntoAntenaInferior, puntoAntenaSuperior);
	
// 	Creamos los ojos del muñeco
	
	_vertex3<float> puntoOjoInferior(0.25, 0, 0);
	_vertex3<float> puntoOjoSuperior(0.25, 0.25, 0);
	Ojo.crearCilindro(puntoOjoInferior, puntoOjoSuperior);
	
}

/*****************************************************************************/
/**						Función que dibuja el muñeco						**/
/*****************************************************************************/

void Muneco::draw_muneco(int modo){
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.128, 255, 0);
	
		glTranslatef(0, 0, desplazamiento);
	
		// Dibujamos el tronco.
		
		glPushMatrix();
		
			MallaTVT tronco = getTronco();
			glTranslatef(0, 4, 0);
			if (modo == 4){
				draw_ajedrez(tronco);
			}else{
				draw_muneco_modos(tronco, modo);
			}
			
			// Creamos las uniones de los brazos al tronco
			
			MallaTVT hombroIzquierdo = getHombro();
			glRotatef(90, 0, 0, 1);
			glTranslatef(4, 3, 0);
			if (modo == 4){
				draw_ajedrez(hombroIzquierdo);
			}else{
				draw_muneco_modos(hombroIzquierdo, modo);
			}
			
			MallaTVT hombroDerecho = getHombro();
			glTranslatef(0, -6.25, 0);
			if (modo == 4){
				draw_ajedrez(hombroDerecho);
			}else{
				draw_muneco_modos(hombroDerecho, modo);
			}
			
			glRotatef(-90, 0, 0, 1);
		
		glPopMatrix();
			
// 		Dibujamos la cabeza.
	
		
		glPushMatrix();
		
			glRotatef(gradosCabeza, 0, 1, 0);
			
			MallaTVT cabeza = getCabeza();
			glTranslatef(0, 9.5, 0);
			if (modo == 4){
				draw_ajedrez(cabeza);
			}else{
				draw_muneco_modos(cabeza, modo);
			}
			
// 			Dibujamos las antenas
			
				glColor3f(0.212, 0.212, 0.212);
				
				glRotatef(-30, 0, 0, 0);
							
				MallaTVT antenaIzquierda = getAntena();
				glTranslatef(-1, 2.25, 1.5);
				if (modo == 4){
					draw_ajedrez(antenaIzquierda);
				}else{
					draw_muneco_modos(antenaIzquierda, modo);
				}
				
				glRotatef(30, 0, 0, 0);
				
				MallaTVT antenaDerecha = getAntena();
				glTranslatef(2, 0, 0);
				if (modo == 4){
					draw_ajedrez(antenaDerecha);
				}else{
					draw_muneco_modos(antenaDerecha, modo);
				}
				
				
			// Dibujamos los ojos
				
				glRotatef(45, 1, 0, 0);
				
				MallaTVT ojoIzquierdo = getOjo();
				glTranslatef(0, 0, 1);
				if (modo == 4){
					draw_ajedrez(ojoIzquierdo);
				}else{
					draw_muneco_modos(ojoIzquierdo, modo);
				}
				
				MallaTVT ojoDerecho = getOjo();
				glTranslatef(-2, 0, 0);
				if (modo == 4){
					draw_ajedrez(ojoDerecho);
				}else{
					draw_muneco_modos(ojoDerecho, modo);
				}
				
				glRotatef(-45, 1, 0, 0);
			
			glRotatef(-gradosCabeza, 0, 1, 0);
			
		glPopMatrix();
		
	
// 		Dibujamos el brazo izquierdo
		
		
		glRotatef(180, 1, 0, 0);
		
		glPushMatrix();
		
			MallaTVT brazoIzquierdo = getBrazo();
			
			glTranslatef(-3.75, -8.25, 0);
			
			glRotatef(-gradosBrazos, 1, 0, 0);
			
			if (modo == 4){
				draw_ajedrez(brazoIzquierdo);
			}else{
				draw_muneco_modos(brazoIzquierdo, modo);
			}
			
// 				Dibujamos los terminales del brazo derecho
				
				MallaTVT terminalSuperiorBrazoIzquierdo = getTerminalExtremidad();
				glTranslatef(0, 2.5, 0);
				if (modo == 4){
					draw_ajedrez(terminalSuperiorBrazoIzquierdo);
				}else{
					draw_muneco_modos(terminalSuperiorBrazoIzquierdo, modo);
				}
				
				MallaTVT terminalInferiorBrazoIzquierdo = getTerminalExtremidad();
				glRotatef(180, 1,0,0);
				glTranslatef(0, 2.5, 0);
				if (modo == 4){
					draw_ajedrez(terminalInferiorBrazoIzquierdo);
				}else{
					draw_muneco_modos(terminalInferiorBrazoIzquierdo, modo);
				}
			
			glRotatef(gradosBrazos, 1, 0, 0);
		
		glPopMatrix();
				
		// Dibujamos el brazo derecho
		
		glPushMatrix();
			
			// Dibujamos el cilindro del brazo
		
			MallaTVT brazoDerecho = getBrazo();
			glTranslatef(3.75, -8.25, 0);
			glRotatef(-gradosBrazos, 1, 0, 0);
			if (modo == 4){
				draw_ajedrez(brazoDerecho);
			}else{
				draw_muneco_modos(brazoDerecho, modo);
			}
		
			// Dibujamos el terminal del brazo derecho
		
			MallaTVT terminalSuperiorBrazoDerecho = getTerminalExtremidad();
			glTranslatef(0, 2.5, 0);
			if (modo == 4){
				draw_ajedrez(terminalSuperiorBrazoDerecho);
			}else{
				draw_muneco_modos(terminalSuperiorBrazoDerecho, modo);
			}
		
			MallaTVT terminalInferiorBrazoDerecho = getTerminalExtremidad();
			glRotatef(180, 1,0,0);
			glTranslatef(0, 2.5, 0);
			if (modo == 4){
				draw_ajedrez(terminalInferiorBrazoDerecho);
			}else{
				draw_muneco_modos(terminalInferiorBrazoDerecho, modo);
			}
			
			glRotatef(gradosBrazos, 1, 0, 0);
			
		glPopMatrix();
		
		// Dibujamos la pierna izquierda
				
		glPushMatrix();
		
			
			MallaTVT piernaIzquierda = getPierna();
			glTranslatef(-1.25, -4, 0);
			glRotatef(-gradosPiernas, 1, 0, 0);
			if (modo == 4){
				draw_ajedrez(piernaIzquierda);
			}else{
				draw_muneco_modos(piernaIzquierda, modo);
			}
			
			// Dibujamos los terminales del pierna izquierdo
			
			MallaTVT terminalInferiorPiernaIzquierda = getTerminalExtremidad();
			glTranslatef(0, 2.5, 0);
			if (modo == 4){
				draw_ajedrez(terminalInferiorPiernaIzquierda);
			}else{
				draw_muneco_modos(terminalInferiorPiernaIzquierda, modo);
			}
			
			glRotatef(gradosPiernas, 1, 0, 0);
		
		glPopMatrix();
		
			
		// Dibujamos la pierna derecha
		
		glPushMatrix();
		
		
			MallaTVT piernaDerecha = getPierna();
			glTranslatef(1.25, -4, 0);
			glRotatef(gradosPiernas, 1, 0, 0);
			if (modo == 4){
				draw_ajedrez(piernaDerecha);
			}else{
				draw_muneco_modos(piernaDerecha, modo);
			}
		
			// Dibujamos los terminales del pierna derecha
		
			MallaTVT terminalInferiorPiernaDerecha = getTerminalExtremidad();
			glTranslatef(0, 2.5, 0);
			if (modo == 4){
				draw_ajedrez(terminalInferiorPiernaDerecha);
			}else{
				draw_muneco_modos(terminalInferiorPiernaDerecha, modo);
			}
			
			glRotatef(-gradosPiernas, 1, 0, 0);
			
		glPopMatrix();
		
	glPopMatrix();
}

void Muneco::girarCabeza(int i){
	if (gradosCabeza <= 90 && gradosCabeza>=-90){
		if (gradosCabeza==90){
			gradosCabeza=89;
		}
		if(gradosCabeza==-90){
			gradosCabeza=-89;
		}
		gradosCabeza = gradosCabeza + i;
	}
}

void Muneco::girarPiernas(int i){
	int d = i;
	if (i == 1){
		// Si andamos para adelante
		if (gradosPiernas <= 35 && gradosPiernas>=-35){
			if (gradosPiernas==35){
				gradosPiernas=34;
				finPaso = true;
			}
			if(gradosPiernas==-35){
				gradosPiernas=-34;
				finPaso = false;
			}
			if (finPaso == false){
				gradosPiernas = gradosPiernas + i;
			}else{
				gradosPiernas = gradosPiernas - i;
			}
		}
	}else{
		// Si andamos para atrás
		if (gradosPiernas <= 35 && gradosPiernas>=-35){
			if (gradosPiernas==35){
				gradosPiernas=34;
				finPaso = true;
			}
			if(gradosPiernas==-35){
				gradosPiernas=-34;
				finPaso = false;
			}
			if (finPaso == true){
				gradosPiernas = gradosPiernas + i;
			}else{
				gradosPiernas = gradosPiernas - i;
			}
		}
	}
	desplazamiento += d*0.0125;
}

void Muneco::girarBrazos(int i){
	if (gradosBrazos <= 180 && gradosBrazos>=-1){
		if (gradosBrazos==180){
			gradosBrazos=179;
		}
		if(gradosBrazos==-1){
			gradosBrazos=0;
		}
		gradosBrazos = gradosBrazos + i;
	}
}

//**************************************************************************
// 			Funcion para dibujar el muñeco en distintos modos 
//***************************************************************************

void Muneco::draw_muneco_modos(MallaTVT &miMalla, int modo){
	switch(modo){
		case 1:{	// Modo Puntos
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
			glPointSize(2);
			glBegin(GL_POINTS);
			break;
		}
		case 2:{	// Modo Alambre
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth( 1.0 );
			glBegin(GL_TRIANGLES);
			break;
		}
		case 3:{	//Modo solido
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glLineWidth( 1.0 );
			glBegin(GL_TRIANGLES);
			break;
		}
	}
	
	vector<_vertex3<float> > v = miMalla.getVertices();
	
	vector<int> c = miMalla.getCaras();
	
	for (int i=0; i<miMalla.getNumCaras(); i+=3){
		glVertex3fv((GLfloat *) &v[c[i]]);
		glVertex3fv((GLfloat *) &v[c[i+1]]);
		glVertex3fv((GLfloat *) &v[c[i+2]]);
	}
	glEnd();
}


//**************************************************************************
// 			Funcion para dibujar los vertices 
//***************************************************************************

void draw_plyPuntos(MallaTVT &miMalla){
	glPointSize(2);
	glBegin(GL_POINTS);
	
	vector<_vertex3<float> > v = miMalla.getVertices();
	
	for (unsigned i=0; i<miMalla.getNumVertices(); i++){
		glVertex3fv((GLfloat *) &v[i]);
	}
	glEnd();
}



//***************************************************************************
// 						Funcion para dibujar vertices
//***************************************************************************

void draw_vertices(vector<float> &Vertices)
{
	vector <_vertex3<float> > v;
	for (unsigned i = 0; i<Vertices.size(); i+=3){
		_vertex3<float> punto(Vertices[i], Vertices[i+1], Vertices[i+2]);
		v.push_back(punto);
	}
	glPointSize(2);
	glBegin(GL_POINTS);
	for (unsigned i=0; i<v.size(); i++){
			glVertex3fv((GLfloat *) &v[i]);
		}
	glEnd();
}


//**************************************************************************
//								Modo Sólido
//**************************************************************************

void draw_solido(MallaTVT miMalla){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth( 1.0 );
	glPointSize( 4.0 );
	glBegin(GL_TRIANGLES);
	
	vector<_vertex3<float> > v = miMalla.getVertices();
	vector<int> c = miMalla.getCaras();
	
	for (int i=0; i<miMalla.getNumCaras(); i+=3){
		glVertex3fv((GLfloat *) &v[c[i]]);
		glVertex3fv((GLfloat *) &v[c[i+1]]);
		glVertex3fv((GLfloat *) &v[c[i+2]]);
	}
	glEnd();
}

//**************************************************************************
//							Modo Alambre
//**************************************************************************

void draw_alambre(MallaTVT miMalla){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth( 1.0 );
	glPointSize(1.0);
	glBegin(GL_TRIANGLES);
	
	vector<_vertex3<float> > v = miMalla.getVertices();
	
	vector<int> c = miMalla.getCaras();
	for (int i=0; i<miMalla.getNumCaras(); i+=3){
		glVertex3fv((GLfloat *) &v[c[i]]);
		glVertex3fv((GLfloat *) &v[c[i+1]]);
		glVertex3fv((GLfloat *) &v[c[i+2]]);
	}
	glEnd();
}

/*****************************************************************************/
/**							Modo Ajedrez									**/
/*****************************************************************************/

void draw_ajedrez(MallaTVT miMalla){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	int j=0;
	glLineWidth( 1.0 );
	glPointSize( 4.0 );
	glBegin(GL_TRIANGLES);
	
	vector<_vertex3<float> > v = miMalla.getVertices();
	vector<int> c = miMalla.getCaras();
	
	for (int i=0; i<miMalla.getNumCaras(); i+=3){
		if (j%2){
			glColor3f( 0, 0, 1 ) ;
		}else{
			glColor3f(0.128, 255, 0);
		}
		glVertex3fv((GLfloat *) &v[c[i]]);
		glVertex3fv((GLfloat *) &v[c[i+1]]);
		glVertex3fv((GLfloat *) &v[c[i+2]]);
		j++;
	}
	glEnd();
}
