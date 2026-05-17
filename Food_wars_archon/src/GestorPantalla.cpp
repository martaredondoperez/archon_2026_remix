#include "GestorPantalla.h"
#include "freeglut.h"

GestorPantalla::GestorPantalla(float ancho, float alto)
    : anchoLogico(ancho), altoLogico(alto), viewportX(0), viewportY(0), viewportAncho(ancho), viewportAlto(alto)
{
    aspectJuego = anchoLogico / altoLogico;
}

void GestorPantalla::configurarCamara() {
    float anchoV = (float)glutGet(GLUT_WINDOW_WIDTH);
    float altoV = (float)glutGet(GLUT_WINDOW_HEIGHT);

    if (anchoV == 0 || altoV == 0) return;

    float aspectVentana = anchoV / altoV;

    // Calcular el viewport para que quepan exactamente 800x600 manteniendo aspecto
    if (aspectVentana >= aspectJuego) {
        // Ventana ancha (franjas a los lados)
        float altoJuegoPixeles = altoV;
        float anchoJuegoPixeles = altoJuegoPixeles * aspectJuego;
        float franjaAncho = (anchoV - anchoJuegoPixeles) / 2.0f;

        viewportX = franjaAncho;
        viewportY = 0;
        viewportAncho = anchoJuegoPixeles;
        viewportAlto = altoJuegoPixeles;
    }
    else {
        // Ventana alta (franjas arriba y abajo)
        float anchoJuegoPixeles = anchoV;
        float altoJuegoPixeles = anchoJuegoPixeles / aspectJuego;
        float franjaAlto = (altoV - altoJuegoPixeles) / 2.0f;

        viewportX = 0;
        viewportY = franjaAlto;
        viewportAncho = anchoJuegoPixeles;
        viewportAlto = altoJuegoPixeles;
    }

    // Aplicar el viewport
    glViewport((int)viewportX, (int)viewportY, (int)viewportAncho, (int)viewportAlto);

    // Configurar proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, anchoLogico, 0, altoLogico);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GestorPantalla::dibujarFranjas() {
    float anchoV = (float)glutGet(GLUT_WINDOW_WIDTH);
    float altoV = (float)glutGet(GLUT_WINDOW_HEIGHT);

    if (anchoV == 0 || altoV == 0) return;

    // Restaurar viewport a pantalla completa para dibujar franjas
    glViewport(0, 0, (int)anchoV, (int)altoV);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 0.0f); // Negro

    // Configurar proyección para pantalla
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, anchoV, 0, altoV, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float aspectVentana = anchoV / altoV;

    if (aspectVentana >= aspectJuego) {
        // Ventana ancha (franjas verticales a los lados)
        float altoPixeles = altoV;
        float anchoJuegoPixeles = altoPixeles * aspectJuego;
        float franjaAncho = (anchoV - anchoJuegoPixeles) / 2.0f;

        // Franja izquierda
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(franjaAncho, 0);
        glVertex2f(franjaAncho, altoV);
        glVertex2f(0, altoV);
        glEnd();

        // Franja derecha
        glBegin(GL_QUADS);
        glVertex2f(anchoV - franjaAncho, 0);
        glVertex2f(anchoV, 0);
        glVertex2f(anchoV, altoV);
        glVertex2f(anchoV - franjaAncho, altoV);
        glEnd();
    }
    else if (aspectVentana < aspectJuego) {
        // Ventana alta (franjas horizontales arriba y abajo)
        float anchoPixeles = anchoV;
        float altoJuegoPixeles = anchoPixeles / aspectJuego;
        float franjaAlto = (altoV - altoJuegoPixeles) / 2.0f;

        // Franja inferior
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(anchoV, 0);
        glVertex2f(anchoV, franjaAlto);
        glVertex2f(0, franjaAlto);
        glEnd();

        // Franja superior
        glBegin(GL_QUADS);
        glVertex2f(0, altoV - franjaAlto);
        glVertex2f(anchoV, altoV - franjaAlto);
        glVertex2f(anchoV, altoV);
        glVertex2f(0, altoV);
        glEnd();
    }

    // Restaurar matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Restaurar viewport al área de juego
    glViewport((int)viewportX, (int)viewportY, (int)viewportAncho, (int)viewportAlto);
}

void GestorPantalla::convertirCoordenadasMouse(float pixelX, float pixelY, float& juegoX, float& juegoY) {
    float anchoV = (float)glutGet(GLUT_WINDOW_WIDTH);
    float altoV = (float)glutGet(GLUT_WINDOW_HEIGHT);

    if (anchoV == 0 || altoV == 0) return;

    float aspectVentana = anchoV / altoV;

    if (aspectVentana >= aspectJuego) {
        // Ventana ancha (franjas laterales)
        float altoJuegoPixeles = altoV;
        float anchoJuegoPixeles = altoJuegoPixeles * aspectJuego;
        float franjaAncho = (anchoV - anchoJuegoPixeles) / 2.0f;

        // Restar las franjas del pixel para obtener la posición dentro del área de juego
        float pixelDentro = pixelX - franjaAncho;

        // Si está fuera del área de juego, devolver valores fuera de rango (0-800, 0-600)
        if (pixelDentro < 0 || pixelDentro > anchoJuegoPixeles) {
            juegoX = -1.0f;
            juegoY = -1.0f;
            return;
        }

        // Mapear a coordenadas de juego (0-800)
        juegoX = (pixelDentro / anchoJuegoPixeles) * anchoLogico;
        juegoY = (1.0f - (pixelY / altoV)) * altoLogico;
    }
    else {
        // Ventana alta (franjas arriba/abajo)
        float anchoJuegoPixeles = anchoV;
        float altoJuegoPixeles = anchoJuegoPixeles / aspectJuego;
        float franjaAlto = (altoV - altoJuegoPixeles) / 2.0f;

        // Restar las franjas del pixel
        float pixelDentro = pixelY - franjaAlto;

        // Si está fuera del área de juego, devolver valores fuera de rango
        if (pixelDentro < 0 || pixelDentro > altoJuegoPixeles) {
            juegoX = -1.0f;
            juegoY = -1.0f;
            return;
        }

        // Mapear a coordenadas de juego (0-600)
        juegoX = (pixelX / anchoJuegoPixeles) * anchoLogico;
        juegoY = (1.0f - (pixelDentro / altoJuegoPixeles)) * altoLogico;
    }
}


