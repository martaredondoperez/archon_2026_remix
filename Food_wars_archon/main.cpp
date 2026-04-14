#include "freeglut.h"
#include "Mundo.h"

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
    mundo.gestionaRaton(button, state, x, y);
}

void keyboard(unsigned char key, int x, int y) {
    mundo.teclado(key, x, y);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Food Wars: Archon ETSIDI");

    glutKeyboardFunc(keyboard);
    mundo.inicializa();

    // Registro de funciones - HEMOS QUITADO EL IDLE
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}