#include "Interfaz.h"
#include "freeglut.h"
#include <string.h>

// Constructor: cargamos la imagen.//texto,x,y,ancho,alto 
Interfaz::Interfaz() :
    fondo("bin/imagenes/fondo_menu_principal.png", 0, 0, 800, 600),
    logo("bin/imagenes/titulo_menu_principal.png", 150, 420, 500, 180),
    fondoSeleccion("bin/imagenes/fondo_menu_seleccion.png", 0, 0, 800, 600) //
{
    // Forzamos posiciones por si acaso
    fondo.setPos(0, 0);
    fondo.setSize(800, 600);
    fondo.setCenter(0, 0);
    // El logo 
    logo.setPos(400, 420);
    logo.setSize(450, 250);
    fondo.setCenter(0, 0);
    //Fondo_seleccion
    fondoSeleccion.setCenter(0, 0);
    fondoSeleccion.setPos(0, 0);
    fondoSeleccion.setSize(800, 600);
}


void Interfaz::dibujaMenu() {
   
    // 1. FONDO y LOGO (Ocupando toda la pantalla)
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Luz total para la imagen
    fondo.draw(); //dibujamos el fondo
    logo.draw();
    glDisable(GL_TEXTURE_2D);//desact texturas

    // 2. BOTONES
    dibujaBoton(300, 300, 200, 60, "1 JUGADOR",true);
    dibujaBoton(300, 200, 200, 60, "2 JUGADORES",false);
    dibujaBoton(300, 100, 200, 50, "INSTRUCCIONES", true);

    // 1. Dibujamos una banda oscura semitransparente en la base
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // Negro con 60% de opacidad
    glBegin(GL_QUADS);
    glVertex2f(0, 30);   // Esquina inferior izquierda
    glVertex2f(800, 30); // Esquina inferior derecha
    glVertex2f(800, 75); // Esquina superior derecha
    glVertex2f(0, 75);   // Esquina superior izquierda
    glEnd();

    // 2. Texto encima en Blanco o Dorado suave
    dibujaTexto("ETSIDI - Informatica Industrial", 265, 45, 1.0f, 1.0f, 1.0f);
}

void Interfaz::dibujaSeleccion() {
    float xH = 100, yH = 500; // Posición bando Healthy
    float xJ = 500, yJ = 500; // Posición bando Junk
    // 1. RESET DE CÁMARA
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 2. DIBUJO DEL FONDO DE SELECCIÓN
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    fondoSeleccion.draw();
    glDisable(GL_TEXTURE_2D);

    // 4. BOTONES DE BANDO
    // Los posicionamos sobre los personajes del fondo (ajusta las coordenadas según tu imagen)
    dibujaBoton(100, 500, 180, 60, "HEALTHY", true);
    dibujaBoton(500, 500, 180, 60, "JUNK", false);

    // Dibujamos los círculos de info justo al lado
    dibujaBotonInfo(xH, yH, 150, 55, true);
    dibujaBotonInfo(xJ, yJ, 150, 55, false);
}

void Interfaz::dibujaInstrucciones() {
    // 1. Fondo (puedes usar el mismo del menú)
    glEnable(GL_TEXTURE_2D);
    fondo.draw();
    glDisable(GL_TEXTURE_2D);

    // 2. Caja negra semitransparente para leer bien
    glEnable(GL_BLEND);
    glColor4f(0, 0, 0, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(100, 100);
    glVertex2f(700, 100);
    glVertex2f(700, 500);
    glVertex2f(100, 500);
    glEnd();

    // 3. Texto de las normas
    dibujaTexto("NORMAS DEL JUEGO", 300, 450, 1.0f, 0.8f, 0.0f);
    dibujaTexto("- Muevete con las flechas.", 150, 380, 1.0f, 1.0f, 1.0f);
    dibujaTexto("- Come comida sana para ganar vida.", 150, 330, 1.0f, 1.0f, 1.0f);
    dibujaTexto("- Evita la comida basura enemiga.", 150, 280, 1.0f, 1.0f, 1.0f);

    dibujaTexto("Pulsa 'M' para volver al menu", 250, 150, 0.5f, 1.0f, 0.5f);
}

void Interfaz::dibujaPausa() {}
void Interfaz::dibujaFinal() {}

void Interfaz::dibujaBoton(float x, float y, float ancho, float alto, const char* texto, bool esVerde) {
    glDisable(GL_LIGHTING); // <-- OBLIGATORIO: Si no, el botón sale negro
    glDisable(GL_TEXTURE_2D); // Aseguramos que no hay texturas activas
    // 1. DIBUJAR EL BORDE (Grosor de 5 píxeles para estilo cartoon)
    float offset = 5.0f;
    glColor3f(0.0f, 0.0f, 0.0f); // Negro puro para el borde
    glBegin(GL_QUADS);
    glVertex2f(x - offset, y - offset);
    glVertex2f(x + ancho + offset, y - offset);
    glVertex2f(x + ancho + offset, y + alto + offset);
    glVertex2f(x - offset, y + alto + offset);
    glEnd();

    // 2. CUERPO CON DEGRADADO
    glBegin(GL_QUADS);
    if (esVerde) {
        glColor3f(0.5f, 0.9f, 0.2f); // Verde brillante arriba
        glVertex2f(x, y + alto);
        glVertex2f(x + ancho, y + alto);
        glColor3f(0.1f, 0.5f, 0.0f); // Verde bosque abajo
        glVertex2f(x + ancho, y);
        glVertex2f(x, y);
    }
    else {
        glColor3f(1.0f, 0.5f, 0.1f); // Naranja fuego arriba
        glVertex2f(x, y + alto);
        glVertex2f(x + ancho, y + alto);
        glColor3f(0.7f, 0.1f, 0.0f); // Rojo/Marrón abajo
        glVertex2f(x + ancho, y);
        glVertex2f(x, y);
    }
    glEnd();

    // 3. TEXTO CENTRADO
    // Calculamos el centro horizontal: x + (ancho / 2) - (desplazamiento por letras)
    // Cada letra de ROMAN_24 mide aprox 12-15 píxeles de ancho.
    float anchoTexto = strlen(texto) * 14.0f; // Ajuste aproximado para centrar
    float posX = x + (ancho - anchoTexto) / 2.0f;
    float posY = y + (alto / 2.0f) - 8.0f; // Bajamos un poco el texto para centrarlo verticalmente
    // Texto principal
    dibujaTexto(texto, posX, posY, 1.0f, 1.0f, 1.0f);
}

void Interfaz::dibujaTexto(const char* texto, float x, float y, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D); // 1. Apagar texturas [cite: 159]

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();           // Guardamos la matriz actual
    glLoadIdentity();         // 2. Resetear coordenadas para que no haya "desplazamientos ghost"

    glColor3f(r, g, b);       // 3. Forzar el color deseado
    glRasterPos2f(x, y);      // Posicionamos el cursor de texto

    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }

    glPopMatrix();            // Restauramos la matriz
}

void Interfaz::dibujaPopUp(const char* titulo, const char* descripcion, bool esVerde) {
    // 1. Oscurecer un poco el fondo para dar foco
    glEnable(GL_BLEND);
    glColor4f(0, 0, 0, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(800, 0);
    glVertex2f(800, 600); glVertex2f(0, 600);
    glEnd();

    // 2. Caja del Pop-Up
    float x = 200, y = 150, w = 400, h = 300;
    glColor3f(0.1f, 0.1f, 0.1f); // Fondo oscuro
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();

    // Borde de color según el bando
    if (esVerde) glColor3f(0, 1, 0); else glColor3f(1, 0.5f, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();

    // 3. Contenido
    dibujaTexto(titulo, x + 100, y + 250, 1, 1, 1);
    dibujaTexto(descripcion, x + 30, y + 150, 0.8f, 0.8f, 0.8f);

    // 4. LA CRUZ (Botón de cerrar arriba a la derecha)
    dibujaBoton(x + w - 40, y + h - 40, 30, 30, "X", false);
}

void Interfaz::dibujaBotonInfo(float x_boton, float y_boton, float ancho_boton, float alto_boton, bool esVerde) {
    // Calculamos el centro del círculo para que esté a la derecha del botón
    // x_boton + ancho_boton + 25 (un pequeño margen)
    float radio = 20.0f;
    float centroX = x_boton + ancho_boton + 30.0f;
    float centroY = y_boton + (alto_boton / 2.0f);

    // 1. DIBUJAR EL CÍRCULO (Fondo)
    if (esVerde) glColor3f(0.0f, 0.6f, 0.0f); // Verde oscuro
    else glColor3f(0.7f, 0.2f, 0.0f);        // Naranja oscuro

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(centroX + radio * cos(theta), centroY + radio * sin(theta));
    }
    glEnd();

    // 2. BORDE DEL CÍRCULO (Blanco o Negro)
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(centroX + radio * cos(theta), centroY + radio * sin(theta));
    }
    glEnd();

    // 3. LA "i" EN EL CENTRO
    dibujaTexto("i", centroX - 4, centroY - 8, 1.0f, 1.0f, 1.0f);
}