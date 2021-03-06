// PlantillaOpenGL.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
/*#include <conio.h>*/

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

//Declaramos un apuntador de ventana.
GLFWwindow *window;

using namespace std;
float xTriangulo = 0.0f; //Cantidad de translacion en X
float yTriangulo = 0.0f; //Cantidad de translacion en Y

float anguloTriangulo = 0.0f; 
float velocidadAngular = 90;

float xCuadrado = 0.5f;
float yCuadrado = 0.3f;

float velocidad = 0.7f;
float aceleracion = 0.8f;
float velocidadActual = 0.0f;

double tiempoAnterior = 0.0;
double tiempoActual = 0.0;
double tiempoDiferencial = 0.0;

float rTriangulo = 1.0f;
float gTriangulo = 1.0f;
float bTriangulo = 1.0f;


void dibujarCuadrado() {
	glPushMatrix();
	//Transformaciones
	
	glTranslatef(xCuadrado, yCuadrado, 0.0f);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.4f, 0.5f);
	glVertex3f(-0.1f, 0.1f, 0.0f);
	glVertex3f(-0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, -0.1f, 0.0f);
	glVertex3f(0.1f, 0.1f, 0.0f);
	
	
	glEnd();
	glPopMatrix();
}
void dibujarTriangulo() {
	glPushMatrix();

	//Transformaciones (1.Translacion, 2.rotacion, 3.escala)
	glTranslatef(xTriangulo, yTriangulo, 0.0f);
	glRotatef(anguloTriangulo, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(rTriangulo, gTriangulo, bTriangulo);
	glVertex3f(0.0f, 0.08f, 0.0f);
	glVertex3f(-0.05f, -0.08f, 0.0f);
	glVertex3f(0.05f, -0.08f, 0.0f);
	glEnd();

	glPopMatrix();
}

void teclado_callback( GLFWwindow * window, int key, int scancode, int action, int mods){
	//Reconocer que tecla es
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		xTriangulo += velocidad;
	}
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		xTriangulo -= velocidad;
	}
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		yTriangulo += velocidad;
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		yTriangulo -= velocidad;
	}
}

void moverTriangulo() {
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (estadoTeclaDerecha == GLFW_PRESS) {
		anguloTriangulo -= velocidadAngular * tiempoDiferencial;
		if (anguloTriangulo < 0) {
			anguloTriangulo += 360;
		}
	}
	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	if (estadoTeclaIzquierda == GLFW_PRESS) {
		anguloTriangulo += velocidadAngular * tiempoDiferencial;
		
		if (anguloTriangulo > 360) {
			anguloTriangulo -= 360;

		}

	}
	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	if (estadoTeclaArriba == GLFW_PRESS) {

		if (velocidadActual <= velocidad) {
			velocidadActual += aceleracion * tiempoDiferencial;
		}
		else
		{
			if (velocidadActual >= 0) {
				velocidadActual -= aceleracion * tiempoDiferencial;
				if (velocidadActual < 0) {
					velocidadActual = 0.0f;
				}
			}
		}
		xTriangulo += cos((anguloTriangulo + 90.0f) * 3.14159f / 180.0f) * velocidad * tiempoDiferencial;
		yTriangulo += sin((anguloTriangulo + 90.0f) * 3.14159f / 180.0f) * velocidad * tiempoDiferencial;
	}
	
}

void dibujar() {
	dibujarTriangulo();
	dibujarCuadrado();
}
void checarColisiones() {

	//Si el triangulo colisiona con el cuadrado
	//Conocer el ancho y alto de la figura y que  tanto lo trasladamos.
	if (xTriangulo - 0.2f  < xCuadrado + 0.1f && xTriangulo + 0.2f > xCuadrado - 0.1f &&
		yTriangulo - 0.2f  < yCuadrado + 0.1f && yTriangulo + 0.2f > yCuadrado - 0.1f) {
		//cout << "Colision en X" << endl;

		gTriangulo = 0.0f;
		bTriangulo = 0.0f;

		xCuadrado = (rand() % 2) - 1;
		yCuadrado = (rand() % 2) - 1;
	}
	else {
		gTriangulo = 1.0f;
		bTriangulo = 1.0f;
	}

}
void actualizar() {
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;
	
	moverTriangulo();
	checarColisiones();

	tiempoAnterior = tiempoActual;

}


int main()
{
	/*Crear un contexto 

	°Un contexto es una ventana de un sistema operativo
	°OpenGL no gestiona el iso de ventanas*/

	
	// Si no se pudo iniciar glfw terminamos ejecución
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	window =
		glfwCreateWindow(1024, 728, "Ventana", NULL, NULL);
	if (!window) {
		glfwTerminate();
			exit(EXIT_FAILURE);
	}
	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido el contexto, activamos funciones modernas(trabajar con GPU/CPU)
	glewExperimental = true;
	GLenum  errorGlew = glewInit();

	if (errorGlew != GLEW_OK) {
		cout << glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL;
	
	//glfwSetKeyCallback(window, teclado_callback);
	
	tiempoAnterior = glfwGetTime();

	//Ciclo de dibujo(draw loop)
	while (!glfwWindowShouldClose(window)) {
		glViewport(0, 0, 1024, 768);
		//Establece el color de borrado
		glClearColor(1, 0.8, 0, 1);
		//Borramos
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();

		//Cambiar Buffers
		glfwSwapBuffers(window);
		//Reconocer si hay entradas
		glfwPollEvents();


		
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

