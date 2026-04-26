#include "Interfaz.h"
#include "freeglut.h"
#include <string.h>

// Constructor: cargamos la imagen.//texto,x,y,ancho,alto 
Interfaz::Interfaz() :
    fondo("fondo_menu_principal.png", 0, 0, 800, 600),
    logo("titulo_menu_principal.png", 150, 420, 500, 180),
    fondoSeleccion("fondo_menu_seleccion.png", 0, 0, 800, 600) //
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
    correccionX = 1.0f;
}


void Interfaz::dibujaMenu() {
    calcularCorreccion();

    // 1. FONDO
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    fondo.draw();
    glDisable(GL_TEXTURE_2D);

    // 2. LOGO - FORZANDO POSICIÓN CON MATRICES
    float anchoLogoOriginal = 450.0f;
    float xOriginalLogo = 400.0f - (anchoLogoOriginal / 2.0f); // 175

    float centroReal = 400.0f * correccionX;
    float desplazaCentro = 400.0f - centroReal;

    float xCorrLogo = (xOriginalLogo * correccionX) + desplazaCentro;
    float anchoCorrLogo = anchoLogoOriginal * correccionX;

    // --- AJUSTE DEL EJE Y ---
    // Antes tenías 420. Prueba con 350 para bajarlo significativamente.
    float nuevaY = 350.0f;

    glPushMatrix(); // Guardamos la posición actual de la "cámara"

    // Movemos el origen de dibujo a las coordenadas que queremos
    glTranslatef(xCorrLogo, nuevaY, 0.0f);

    // Reseteamos la posición interna del objeto a 0 para que no se sumen
    logo.setPos(0, 0);
    logo.setCenter(0, 0);
    logo.setSize(anchoCorrLogo, 250.0f);

    glEnable(GL_TEXTURE_2D);
    logo.draw();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix(); // Restauramos la posición original para no afectar a los botones
   
    
    // 2. BOTONES (Se quedan igual porque la lógica de X irá dentro de la función)
    dibujaBoton(300, 300, 200, 60, "1 JUGADOR", true);
    dibujaBoton(300, 200, 200, 60, "2 JUGADORES", false);
    dibujaBoton(300, 100, 200, 50, "INSTRUCCIONES", true);

    // 3. BANDA (Se queda igual para que siempre llegue a los bordes)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(0, 30);
    glVertex2f(800, 30);
    glVertex2f(800, 75);
    glVertex2f(0, 75);
    glEnd();
    glDisable(GL_BLEND);

    // 4. TEXTO (Se queda igual porque dibujaTexto usará correccionX por dentro)
    dibujaTexto("ETSIDI - Informatica Industrial", 265, 45, 1.0f, 1.0f, 1.0f);
}

void Interfaz::dibujaSeleccion() {
    calcularCorreccion(); // 1. Siempre lo primero para obtener el factor actual

    // Coordenadas originales (para un diseño de 800x600)
    float xH = 100, yH = 500;
    float xJ = 500, yJ = 500;

    // 1. RESET DE CÁMARA
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 2. DIBUJO DEL FONDO DE SELECCIÓN
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    // El fondo se dibuja de 0 a 800, permitiendo que se estire con la ventana
    fondoSeleccion.draw();
    glDisable(GL_TEXTURE_2D);

    // 3. BOTONES DE BANDO
    // Pasamos las coordenadas originales. 
    // La corrección x * correccionX se hará automáticamente DENTRO de dibujaBoton.
    dibujaBoton(xH, yH, 180, 60, "HEALTHY", true);
    dibujaBoton(xJ, yJ, 180, 60, "JUNK", false);

    // 4. CÍRCULOS DE INFORMACIÓN
    // Pasamos las coordenadas y el tamaño original.
    // La corrección de posición y la forma circular se hará DENTRO de dibujaBotonInfo.
    dibujaBotonInfo(xH, yH, 180, 60, true);
    dibujaBotonInfo(xJ, yJ, 180, 60, false);
}

void Interfaz::dibujaInstrucciones() {
    calcularCorreccion(); // Siempre actualizamos el factor primero

    // 1. Fondo (se estira para cubrir todo, así que no necesita correccionX)
    glEnable(GL_TEXTURE_2D);
    fondo.draw();
    glDisable(GL_TEXTURE_2D);

    // 2. Caja negra semitransparente
    // Calculamos los bordes X multiplicando por correccionX para que no se "ensanche"
    float centroReal = 400.0f * correccionX;
    float desplazaCentro = 400.0f - centroReal;
    float x_izq = (100 * correccionX) + desplazaCentro;
    float x_der = (700 * correccionX) + desplazaCentro;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(x_izq, 100);
    glVertex2f(x_der, 100);
    glVertex2f(x_der, 500);
    glVertex2f(x_izq, 500);
    glEnd();
    glDisable(GL_BLEND);

    // 3. Texto de las normas
    // Solo pasamos las coordenadas originales. 
    // Como dibujaTexto tendrá el "x * correccionX" dentro, se alinearán perfecto con la caja.
    dibujaTexto("NORMAS DEL JUEGO", 300, 450, 1.0f, 0.8f, 0.0f);
    dibujaTexto("- Muevete con las flechas.", 150, 380, 1.0f, 1.0f, 1.0f);
    dibujaTexto("- Come comida sana para ganar vida.", 150, 330, 1.0f, 1.0f, 1.0f);
    dibujaTexto("- Evita la comida basura enemiga.", 150, 280, 1.0f, 1.0f, 1.0f);

    dibujaTexto("Pulsa 'M' para volver al menu", 250, 150, 0.5f, 1.0f, 0.5f);
}

void Interfaz::dibujaPausa() {}
void Interfaz::dibujaFinal() {}

void Interfaz::dibujaBoton(float x, float y, float ancho, float alto, const char* texto, bool esVerde) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    float centroReal = 400.0f * correccionX;
    float desplazaCentro = 400.0f - centroReal;
    // 1. PREPARAR COORDENADAS CORREGIDAS
    // Usamos el correccionX que ya calculó la función que llamó a esta
    float xCorr = (x * correccionX) + desplazaCentro;
    float anchoCorr = ancho * correccionX;
    float offset = 5.0f * correccionX;

    // 2. DIBUJAR EL BORDE (Negro)
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(xCorr - offset, y - offset);
    glVertex2f(xCorr + anchoCorr + offset, y - offset);
    glVertex2f(xCorr + anchoCorr + offset, y + alto + offset);
    glVertex2f(xCorr - offset, y + alto + offset);
    glEnd();

    // 3. CUERPO CON DEGRADADO
    glBegin(GL_QUADS);
    if (esVerde) {
        glColor3f(0.5f, 0.9f, 0.2f); glVertex2f(xCorr, y + alto);
        glVertex2f(xCorr + anchoCorr, y + alto);
        glColor3f(0.1f, 0.5f, 0.0f); glVertex2f(xCorr + anchoCorr, y);
        glVertex2f(xCorr, y);
    }
    else {
        glColor3f(1.0f, 0.5f, 0.1f); glVertex2f(xCorr, y + alto);
        glVertex2f(xCorr + anchoCorr, y + alto);
        glColor3f(0.7f, 0.1f, 0.0f); glVertex2f(xCorr + anchoCorr, y);
        glVertex2f(xCorr, y);
    }
    glEnd();

    // 4. TEXTO CENTRADO
    // Calculamos la posición relativa al botón "sin corregir" 
    // porque dibujaTexto() aplicará la corrección por nosotros.
    float anchoTexto = strlen(texto) * 13.0f; // Ajuste para fuente ROMAN_24
    float posX = x + (ancho - anchoTexto) / 2.0f;
    float posY = y + (alto / 2.0f) - 8.0f;

    dibujaTexto(texto, posX, posY, 1.0f, 1.0f, 1.0f);
}
void Interfaz::dibujaTexto(const char* texto, float x, float y, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float centroReal = 400.0f * correccionX;
    float desplazaCentro = 400.0f - centroReal;
    float xCorr = (x * correccionX) + desplazaCentro;

    glColor3f(r, g, b);

    // EL CAMBIO CLAVE: Multiplicamos la coordenada x por correccionX
    // Esto desplaza el "cursor" de texto para que coincida con el botón o caja
    glRasterPos2f(xCorr, y);

    for (int i = 0; i < (int)strlen(texto); i++) {
        // Usamos la fuente que prefieras, ROMAN_24 es bastante legible
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, texto[i]);
    }

    glPopMatrix();
}

void Interfaz::dibujaPopUp(const char* titulo, const char* descripcion, bool esVerde) {
    calcularCorreccion(); // Siempre actualizamos el factor

    // 1. Oscurecer el fondo (0 a 800 siempre cubre toda la pantalla)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    // 2. Coordenadas de la Caja corregidas
    // Definimos las dimensiones originales
    float x = 200, y = 150, w = 400, h = 300;

    // Aplicamos la corrección a la posición X y al ancho
    float centroReal = 400.0f * correccionX;
    float desplazaCentro = 400.0f - centroReal;
    float xCorr = (x * correccionX) + desplazaCentro;
    float wCorr = w * correccionX;

    // Fondo de la caja
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(xCorr, y);
    glVertex2f(xCorr + wCorr, y);
    glVertex2f(xCorr + wCorr, y + h);
    glVertex2f(xCorr, y + h);
    glEnd();

    // Borde de color
    if (esVerde) glColor3f(0, 1, 0); else glColor3f(1, 0.5f, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xCorr, y);
    glVertex2f(xCorr + wCorr, y);
    glVertex2f(xCorr + wCorr, y + h);
    glVertex2f(xCorr, y + h);
    glEnd();
    glDisable(GL_BLEND);

    // 3. Contenido
    // Pasamos coordenadas originales porque dibujaTexto ya aplica correccionX
    dibujaTexto(titulo, x + 100, y + 250, 1, 1, 1);
    dibujaTexto(descripcion, x + 30, y + 150, 0.8f, 0.8f, 0.8f);

    // 4. LA CRUZ (Botón de cerrar)
    // Pasamos coordenadas originales porque dibujaBoton ya aplica correccionX
    dibujaBoton(x + w - 40, y + h - 40, 30, 30, "X", false);
}

void Interfaz::dibujaBotonInfo(float x_boton, float y_boton, float ancho_boton, float alto_boton, bool esVerde) {
    // Ya no calculamos aquí width/height, usamos el correccionX de la clase

    float radio = 20.0f;

    // 1. CORRECCIÓN DE LA POSICIÓN DEL CENTRO
    // El círculo debe estar a la derecha del botón (x + ancho + margen).
    // Multiplicamos TODA la posición X por la corrección.
    float centroReal = 400.0f * correccionX;
    float desplazaCentro = 400.0f - centroReal;
    float centroX = ((x_boton + ancho_boton + 30.0f) * correccionX) + desplazaCentro;
    float centroY = y_boton + (alto_boton / 2.0f);

    // 2. DIBUJAR EL CÍRCULO (Fondo)
    if (esVerde) glColor3f(0.0f, 0.6f, 0.0f);
    else glColor3f(0.7f, 0.2f, 0.0f);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        // Corregimos el radio en X para que sea un círculo perfecto y no un óvalo
        glVertex2f(centroX + (radio * cos(theta) * correccionX), centroY + radio * sin(theta));
    }
    glEnd();

    // 3. BORDE DEL CÍRCULO
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(centroX + (radio * cos(theta) * correccionX), centroY + radio * sin(theta));
    }
    glEnd();

    // 4. LA "i" EN EL CENTRO
    // Importante: No uses centroX aquí directamente porque dibujaTexto ya multiplica por correccionX.
    // Le pasamos la coordenada "lógica" (sin corregir) para que dibujaTexto haga el trabajo.
    float posX_i = (x_boton + ancho_boton + 30.0f) - 4.0f;
    float posY_i = centroY - 8.0f;

    dibujaTexto("i", posX_i, posY_i, 1.0f, 1.0f, 1.0f);
}
void Interfaz::calcularCorreccion() {
    float width = (float)glutGet(GLUT_WINDOW_WIDTH);
    float height = (float)glutGet(GLUT_WINDOW_HEIGHT);
    // 1.33f es la relación 800/600
    if (height > 0)
    this->correccionX = 1.3333f / (width / height);
}