#include "freeglut.h"
#include "Mundo.h"
#include <iostream>

Mundo* mundo = nullptr;

void display() {
    // La limpieza se hace DENTRO de mundo.dibuja() para controlar los dos buffers
    if (mundo) mundo->dibuja();
}

void timer(int v) {
    mundo.actualizaFisicas();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    if (mundo) mundo->mouse(button, state, x, y);
}

void redimensionar(int ancho, int alto) {
    if (alto == 0) alto = 1;
    glViewport(0, 0, ancho, alto);
}
void funcionTeclado(unsigned char key, int x, int y) {
    if (mundo) mundo->teclado(key, x, y); 
}

void funcionTeclasEspeciales(int key, int x, int y) {
    if (mundo) mundo->teclasEspeciales(key, x, y);
}

void funcionTecladoUp(unsigned char key, int x, int y) {
    mundo.tecladoUp(key, x, y);
}

void funcionTeclasEspecialesUp(int key, int x, int y) {
    mundo.teclasEspecialesUp(key, x, y);
void funcionMousePasivo(int x, int y) {
    if (mundo) mundo->mousePasivo(x, y);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Food Wars: Archon ETSIDI");
    glutReshapeFunc(redimensionar);

    // CREAR LA INSTANCIA DE MUNDO DESPUÉS DE QUE GLUT ESTÉ LISTO
    mundo = new Mundo();
    mundo->inicializa();

    // Registro de funciones - HEMOS QUITADO EL IDLE
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    glutPassiveMotionFunc(funcionMousePasivo);
    glutKeyboardFunc(funcionTeclado);
    glutSpecialFunc(funcionTeclasEspeciales);
    glutKeyboardUpFunc(funcionTecladoUp);
    glutSpecialUpFunc(funcionTeclasEspecialesUp);

    glutMainLoop();
    return 0;
}