#include "freeglut.h"
#include "Mundo.h"
#include <iostream>

Mundo mundo;

void display() {
    // La limpieza se hace DENTRO de mundo.dibuja() para controlar los dos buffers
    mundo.dibuja();
}

void timer(int v) {

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    mundo.mouse(button, state, x, y);
}

void redimensionar(int ancho, int alto) {
    if (alto == 0) alto = 1;

    // 1. Calculamos la relación de aspecto
    float aspect_ventana = (float)ancho / (float)alto;
    float aspect_juego = 800.0f / 600.0f; // = 1.33

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (aspect_ventana >= aspect_juego) {
        // La ventana es más ancha que el juego (Panorámica)
        // Ajustamos el ancho para que el juego se quede centrado
        float ancho_logico = 600.0f * aspect_ventana;
        float extra = (ancho_logico - 800.0f) / 2.0f;
        glOrtho(-extra, 800.0f + extra, 0.0, 600.0, -100.0, 100.0);
    }
    else {
        // La ventana es más alta que el juego
        // Ajustamos el alto
        float alto_logico = 800.0f / aspect_ventana;
        float extra = (alto_logico - 600.0f) / 2.0f;
        glOrtho(0.0, 800.0f, -extra, 600.0f + extra, -100.0, 100.0);
    }

    // 2. El Viewport siempre ocupa toda la ventana real
    glViewport(0, 0, ancho, alto);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Food Wars: Archon ETSIDI");
    glutReshapeFunc(redimensionar);
    mundo.inicializa();

    // Registro de funciones - HEMOS QUITADO EL IDLE
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    glutPassiveMotionFunc(Mundo::mousePasivo);

    glutMainLoop();
    return 0;
}