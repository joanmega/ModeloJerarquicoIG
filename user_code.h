#include <GL/gl.h>
#include <vector>
#include "vertex.h"
#include "file_ply_stl.h"
#include "cmath"

class MallaTVT{
	private:
		int numVertices;
		int numCaras;
		vector<_vertex3<float> > vertices;	// salida:  vector de coords. de vert.
		vector<int> caras;					// salida:  vector de triángulos (índices)
		vector<_vertex3<float> > normalesCaras;
		vector<_vertex3<float> > normalesVertices;
		
		void pasarEstructura(vector<float> v);
		void crearCaras(int numVerticesPerfil, int traslaciones);
		
		
	public:
		
		MallaTVT();
		MallaTVT(const char *cadena);
		void read(const char *cadena);
		unsigned int getNumVertices();
		void crearObjeto(int traslaciones);
		int getNumCaras();
		vector <_vertex3<float> > getVertices();
		vector<int> getCaras();
		void crearPerfil(_vertex3<float> punto);
		void crearNormales();
		void crearSemiesfera(_vertex3<float> pInicial);
		void crearCilindro(_vertex3<float> puntoInferior, _vertex3<float> puntoSuperior);
		void clear();
};
/******************************************************************************
 ***************************** Clase Muñeco ***********************************
 *****************************************************************************/


class Muneco{
	private:
		MallaTVT Cabeza;
		MallaTVT Tronco;
		MallaTVT Brazo;
		MallaTVT Pierna;
		MallaTVT Ojo;
		MallaTVT TerminalExtremidad;
		MallaTVT Hombro;
		MallaTVT Antena;
		
		float gradosCabeza;
		float gradosPiernas;
		float gradosBrazos;
		float desplazamiento;
		
		
		bool finPaso;
		
	public:
		
		Muneco();
		
		void draw_muneco(int modo);
		void draw_muneco_modos(MallaTVT &miMalla, int modo);
		
		MallaTVT & getCabeza(){
			return Cabeza;
		};
		
		MallaTVT &getTronco(){
			return Tronco;
		};
		
		MallaTVT &getBrazo(){
			return Brazo;
		};
		
		MallaTVT &getPierna(){
			return Pierna;
		};
		
		MallaTVT &getTerminalExtremidad(){
			return TerminalExtremidad;
		};
		
		MallaTVT & getHombro(){
			return Hombro;
		};
		
		MallaTVT & getAntena(){
			return Antena;
		};
		
		MallaTVT & getOjo(){
			return Ojo;
		};
		
		void girarCabeza(int i);
		void girarPiernas(int i);
		void girarBrazos(int i);
		
};

void draw_plyPuntos(MallaTVT &miMalla);

void draw_solido(MallaTVT miMalla);

void draw_alambre(MallaTVT miMalla);

void draw_ajedrez(MallaTVT miMalla);

void draw_muneco(Muneco &muneco);