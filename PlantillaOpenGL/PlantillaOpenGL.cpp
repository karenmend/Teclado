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

float velocidad = 0.5f;

double tiempoAnterior = 0.0;
double tiempoActual = 0.0;
double tiempoDiferencial = 0.0;

void dibujarTriangulo() {
	glPushMatrix();

	//Transformaciones
	glTranslatef(xTriangulo, yTriangulo, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(-0.2f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.2f, 0.0f);
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
		xTriangulo += velocidad * tiempoDiferencial;
	}
	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	if (estadoTeclaIzquierda == GLFW_PRESS) {
		xTriangulo -= velocidad * tiempoDiferencial;
	}
	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	if (estadoTeclaArriba == GLFW_PRESS) {
		yTriangulo += velocidad * tiempoDiferencial;
	}
	int estadoTeclaAbajo = glfwGetKey(window, GLFW_KEY_DOWN);
	if (estadoTeclaAbajo == GLFW_PRESS) {
		yTriangulo -= velocidad * tiempoDiferencial;
	}
}

void dibujar() {
	dibujarTriangulo();
}

void actualizar() {
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;
	moverTriangulo();
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

