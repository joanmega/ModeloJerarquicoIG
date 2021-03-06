﻿# ModeloJerarquicoIG

José Antonio Medina García

Esta aplicación crea un modelo jerárquico de tres grados de libertad, programado en C++, usando la librería de OpenGL. 

Este modelo representa al muñeco de Android, el cual puede mover brazos, cabeza y piernas. También es capaz de desplazarse por la pantalla.

Además de poder usar el modo de modelo jerárquico, también se puede utilizar para visualizar archivos .ply.

Los distintos objetos, tanto el muñeco de Android como los modelos de los archivos .ply, se pueden visualizar en tres modos diferentes (Solido, ajedrez y alambre).

Para compilar esta aplicación se debe ejecutar el archivo makefile.

Una vez compilado ejecutaremos el programa con ./prac3 seguido del fichero ply que queremos cargar. Por ejemplo:

	./prac3 ./ant.ply

Al iniciar el programa no veremos ninguna figura cargada. Para ver alguna debemos de pulsar las siguientes teclas:

1 -> Para cargar el modelo ply.
2 -> Para ver el peón.
3 -> Para cargar nuestro modelo jerarquico.

Por defecto se cargará el modelo en modo ajedrez. Para cambiar el modo debemos de pulsar las siguientes teclas:

S -> Modo sólido.
L -> Modo lineas/alambre.
P -> Modo puntos.
A -> Para volver al modo ajedrez.

Para ver los diferentes grados de libertad de nuestro modelo jerarquico pulsaremos las siguientes teclas:

Z/z -> Para mover la cabeza de nuestro muñeco.
X/x -> Para mover las piernas del muñeco y se desplace para atrás (X) o para adelante (x).
C/c -> Para mover los brazos de nuestro muñeco.

Además también podemos:

- Acercarnos a cualquiera de nuestros modelos con las teclas RePag y AvPag.
- Girar hacia la izquierda y derecha, arriba y abajo con las teclas de direcciones.
- Salir del programa con la tecla Q.
