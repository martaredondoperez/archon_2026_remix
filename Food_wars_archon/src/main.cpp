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
void redimensionar(int w, int h) {
    // 1. Ajustamos el Viewport a los nuevos píxeles reales
    glViewport(0, 0, w, h);

    // 2. Reconfiguramos la proyección para que 800x600 sigan siendo nuestras unidades
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Esto obliga a que, aunque la ventana sea gigante, 
    // nuestro mundo siga midiendo de 0 a 800 y de 0 a 600
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Food Wars: Archon ETSIDI");
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(keyboard);
    mundo.inicializa();

    // Registro de funciones - HEMOS QUITADO EL IDLE
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}