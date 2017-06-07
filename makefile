## makefile prácticas IG (CUA, octubre 2013)
##
## compila todos los fuentes C y C++ (.c, .cc y .cpp)
## crea un ejecutable enlazando con glut y gl
## modificar "ejecutable" para crear un ejecutable con otro nombre
##
## usar:
##
##    make (o make x) - compilar y ejecutar
##    make comp       - compilar (también con make "ejecutable")
##    make clean      - borrar ejecutable y archivos .o



.PHONY: x, clean, comp
.SUFFIXES:
.SUFFIXES: .o .c .cc .cpp

ejecutable:=  prac3
params_comp:= -g -Wall
libs:=        -lglut -lGL -L/usr/lib/nvidia-331
unidades:=    $(wildcard *.cc) $(wildcard *.cpp) $(wildcard *.c)
fuentes:=     $(UNIDADES) $(wildcard *.h) $(wildcard *.hpp)
unidades_o:=  $(addsuffix .o, $(basename $(unidades))) 


x:
	@echo "Prácticas de Informática Gráfica 13-14 (makefile)"
	@echo "-------------------------------------------------"
	@make --no-print-directory comp

comp: $(ejecutable)

$(ejecutable): $(unidades_o)
	g++ -o $(ejecutable) $(unidades_o)  $(libs) 

$(unidades_o): $(fuentes) 

.cc.o:
	g++ $(params_comp) -c $<

.cpp.o:
	g++ $(params_comp) -c $<

.c.o:
	gcc $(params_comp) -c $<

clean:
	rm -rf *.o $(ejecutable)
