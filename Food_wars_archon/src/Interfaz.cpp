#include "Interfaz.h"
#include "freeglut.h"
#include <string.h>
#include "ETSIDI.h"
#include "Mundo.h"
#include <stdlib.h>
#include <iostream>

// Constructor: cargamos la imagen.//texto,x,y,ancho,alto 
Interfaz::Interfaz() :

    fondo("imagenes/fondo_menu_principal.png", 0, 0, 800, 600),
    logo("imagenes/titulo_menu_principal.png", 150, 420, 500, 180),
    fondoSeleccion("imagenes/fondo_menu_seleccion.png", 0, 0, 800, 600), 

    iconoPausa("imagenes/pausa.png"),
    iconoAjustes("imagenes/ajustes.png"),
    iconoInfo("imagenes/info.png"),
    iconoVolver("imagenes/volver.png"),
    fondo_menu_dificultad("imagenes/fondo_menu_dificultad.png",0,0,800,600)
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
    //fondo_menu_dificultad
    fondo_menu_dificultad.setCenter(0, 0);
    fondo_menu_dificultad.setPos(0, 0);
    fondo_menu_dificultad.setSize(800, 600);

}


void Interfaz::dibujaMenu() {
    // 1. FONDO y LOGO (Ocupando toda la pantalla)
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Luz total para la imagen
    fondo.draw(); //dibujamos el fondo
    logo.draw();
    glDisable(GL_TEXTURE_2D);//desact texturas

    // 2. BOTONES
    dibujaBoton(300, 300, 200, 60, "1 JUGADOR", 0.5f, 0.9f, 0.2f, 0.1f, 0.5f, 0.0f);
    dibujaBoton(300, 200, 200, 60, "2 JUGADORES", 1.0f, 0.5f, 0.1f, 0.7f, 0.1f, 0.0f);
    dibujaBoton(300, 100, 200, 50, "INSTRUCCIONES", 0.5f, 0.9f, 0.2f, 0.1f, 0.5f, 0.0f);

    // 1. Dibujamos una banda oscura semitransparente en la base
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // Negro con 60% de opacidad
    glBegin(GL_QUADS);
    glVertex2f(0, 30); // Esquina inferior izquierda
    glVertex2f(800, 30); // Esquina inferior derecha
    glVertex2f(800, 75); // Esquina superior derecha
    glVertex2f(0, 75); // Esquina superior izquierda
    glEnd();
    glDisable(GL_BLEND);
    // 2. Texto encima en Blanco o Dorado suave
    dibujaTexto("ETSIDI - Informatica Industrial", 265, 45, 1.0f, 1.0f, 1.0f);
    
    dibujaBoton(670, 30, 110, 45, "SALIR", 0.8f, 0.1f, 0.1f, 0.4f, 0.0f, 0.0f);

}

void Interfaz::dibujaMenuDificultad() {
    glDisable(GL_LIGHTING);   
    glDisable(GL_TEXTURE_2D); 

    // 2. DIBUJO DEL FONDO (Con textura)
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco puro para que la imagen no se vea gris
    fondo_menu_dificultad.draw();
    glDisable(GL_TEXTURE_2D); // La apagamos justo después

    // 4. TÍTULO
    dibujaTexto("SELECCIONA DIFICULTAD", 280, 540, 0.0f, 0.0f, 0.0f);

    // 5. BOTONES (Asegúrate de que r, g, b no sean todos iguales o saldrán grises)
    dibujaBoton(300, 350, 200, 60, "PRINCIPIANTE", 0.1f, 0.8f, 0.1f, 0.0f, 0.4f, 0.0f);
    dibujaBoton(300, 250, 200, 60, "GUERRERO", 0.9f, 0.6f, 0.0f, 0.6f, 0.3f, 0.0f);
    dibujaBoton(300, 150, 200, 60, "PESADILLA", 0.9f, 0.1f, 0.1f, 0.5f, 0.0f, 0.0f);

    // 6. BOTÓN VOLVER
    dibujaBotonCircular(60, 540, 25, iconoVolver, 0.5f, 0.5f, 0.5f);
}

void Interfaz::dibujaSeleccion() {

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

    // 4. BOTONES DE BANDO
    // Los posicionamos sobre los personajes del fondo (ajusta las coordenadas según tu imagen)
    dibujaBoton(xH, yH, 180, 60, "HEALTHY", 0.5f, 0.9f, 0.2f, 0.1f, 0.5f, 0.0f);
    dibujaBoton(xJ, yJ, 180, 60, "JUNK", 1.0f, 0.5f, 0.1f, 0.7f, 0.1f, 0.0f);

    // 4. CÍRCULOS DE INFORMACIÓN
    dibujaBotonCircular(xH+160, yH+30, 20.0f, iconoInfo, 0.2f, 0.5f, 0.9f);
    dibujaBotonCircular(xJ+160, yJ+30, 20.0f, iconoInfo, 0.2f, 0.5f, 0.9f);
    dibujaBotonCircular(40, 560, 25, iconoVolver, 0.3f, 0.3f, 0.3f);
}

void Interfaz::dibujaInstrucciones() {

    // 1. Fondo (se estira para cubrir todo, así que no necesita correccionX)
    glEnable(GL_TEXTURE_2D);
    fondo.draw();
    glDisable(GL_TEXTURE_2D);

    // 2. Caja negra semitransparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Asegura el modo de mezcla
    glColor4f(0, 0, 0, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(100, 100);
    glVertex2f(700, 100);
    glVertex2f(700, 500);
    glVertex2f(100, 500);
    glEnd();
    glDisable(GL_BLEND);

    dibujaBotonCircular(150, 450, 25, iconoVolver, 0.3f, 0.3f, 0.3f);

    // 3. Texto de las normas (Coordenadas Absolutas)
    dibujaTexto("NORMAS DEL JUEGO", 300, 450, 1.0f, 0.8f, 0.0f);
    dibujaTexto("- Muevete con las flechas.", 150, 380, 1.0f, 1.0f, 1.0f);
    dibujaTexto("- Come comida sana para ganar vida.", 150, 330, 1.0f, 1.0f, 1.0f);
    dibujaTexto("- Evita la comida basura enemiga.", 150, 280, 1.0f, 1.0f, 1.0f);
}

void Interfaz::dibujaHUDJuego(InfoFicha info) {
    if (!info.activa) return;

    // 1. Agrandamos el ALTO de la caja (de 120 a 140 o 150)
    float x = 620, y = 150, ancho = 170, alto = 145;
    dibujaBoton(x, y, ancho, alto, "", 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f);

    // 2. Nombre de la unidad (Subimos un poco la Y)
    char n[50];
    sprintf_s(n, "%s", info.nombre.c_str());
    dibujaTexto(n, x + 10, y + 120, 1.0f, 1.0f, 0.0f); // y + 120 en lugar de 90

    // 3. Barra de vida (La mantenemos en una zona intermedia)
    float porcentaje = (info.vidaMax > 0) ? (float)info.vidaActual / info.vidaMax : 0.0f;
    dibujaBoton(x + 10, y + 70, 150, 20, "", 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); // Fondo
    dibujaBoton(x + 10, y + 70, 150 * porcentaje, 20, "", 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f); // Vida

    // 4. Texto de HP (Justo debajo de la barra)
    char hp[30];
    sprintf_s(hp, "HP: %d / %d", info.vidaActual, info.vidaMax);
    dibujaTexto(hp, x + 10, y + 45, 1.0f, 1.0f, 1.0f);

    // 5. Texto de ATAQUE (Ahora tiene espacio de sobra abajo)
    char atk[30];
    sprintf_s(atk, "ATAQUE: %d", info.ataque);
    dibujaTexto(atk, x + 10, y + 15, 1.0f, 0.6f, 0.0f); // Color naranja
}


void Interfaz::dibujaMenuConfig(bool musicaActiva) {
    // Usamos tu base de PopUp
// Llamada para el popup de Ajustes en color Rosa
    dibujaPopUp("AJUSTES DE SONIDO",
        {
            "Aqui puedes configurar el volumen del juego",
            "Musica: 80%",
            "Efectos: 100%"
        },
        1.0f, 0.4f, 0.7f);    // Botón de Música ON/OFF
    if (musicaActiva)
        dibujaBoton(300, 320, 200, 50, "MUSICA: ON", 0.2f, 0.8f, 0.2f, 0.1f, 0.4f, 0.0f);
    else
        dibujaBoton(300, 320, 200, 50, "MUSICA: OFF", 0.8f, 0.2f, 0.2f, 0.4f, 0.0f, 0.0f);

    // Botón para volver al juego
    dibujaBoton(325, 200, 150, 40, "CERRAR", 0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f);
}

void Interfaz::dibujaPausa() {
    // 1. EL VELO OSCURO
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);  
    glDisable(GL_TEXTURE_2D);
    glColor4ub(10, 20, 40, 100); // Negro transparente

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();
    glDisable(GL_BLEND);
    glPopAttrib();
    dibujaTexto("PAUSA", 360, 320, 1.0f, 1.0f, 1.0f);
    dibujaTexto("Haz clic en el boton de pausa para volver", 250, 280, 0.8f, 0.8f, 0.8f);
    
}
void Interfaz::dibujaFinal(int ganador) {
    // 1. Fondo
    glEnable(GL_TEXTURE_2D);
    fondo.draw();
    glDisable(GL_TEXTURE_2D);

    // 2. Color cuadrado de victoria (Verde para Healthy, Rojo para Junk)
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    if (ganador == 1) { // Gana SALUDABLE -> Velo VERDE
        glColor3f(0.0f, 0.6f, 0.0f);
    }
    else { // Gana JUNK/BASURA -> Velo NARANJA
        glColor3f(1.0f, 0.5f, 0.0f);
    }

    // Dibujamos el cuadrado central
    glBegin(GL_QUADS);
    glVertex2f(100, 100); glVertex2f(700, 100);
    glVertex2f(700, 500); glVertex2f(100, 500);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    // 3. Títulos
    if (ganador == 1) dibujaTexto("¡VICTORIA SALUDABLE!", 280, 400, 1.0f, 1.0f, 1.0f);
    else dibujaTexto("¡VICTORIA BASURA!", 310, 400, 1.0f, 1.0f, 1.0f);

    // 4. BOTONES (Coordenadas sugeridas para tu sistema de click)
    // Botón Ranking
    dibujaBoton(300, 320, 200, 50, "RANKING", 1.0f, 0.5f, 0.7f, 0.7f, 0.2f, 0.4f);
    // Botón Reiniciar
    dibujaBoton(300, 240, 200, 50, "REINTENTAR", 0.3f, 0.7f, 1.0f, 0.1f, 0.4f, 0.8f);
    // Botón Menú/Cerrar
    dibujaBoton(300, 160, 200, 50, "SALIR DEL JUEGO", 1.0f, 0.2f, 0.2f, 0.6f, 0.0f, 0.0f);
}
void Interfaz::dibujaBoton(float x, float y, float ancho, float alto, const char* texto, float r1, float g1, float b1, float r2, float g2, float b2) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    // COMPROBACIÓN DE SI EL RATON ESTA ENCIMA
    bool encima = (Mundo::mouseX > x && Mundo::mouseX < x + ancho &&
        Mundo::mouseY > y && Mundo::mouseY < y + alto);
    if (encima) {
        // Solo si estamos encima, cambiamos el cursor.
        glutSetCursor(GLUT_CURSOR_TOP_SIDE);
    }
    glPushMatrix();
    
    //AUMENTO DEL BOTON CUANDO ENCIMA
    if (encima) {
        // 1. Calculamos el centro del botón
        float centroX = x + (ancho / 2.0f);
        float centroY = y + (alto / 2.0f);

        // 2. Trasladamos al centro, escalamos y volvemos
        glTranslatef(centroX, centroY, 0.0f);
        glScalef(1.05f, 1.05f, 1.0f); // Aumento del 5%
        glTranslatef(-centroX, -centroY, 0.0f);
    }
    // 1. BORDE NEGRO (Estilo Cartoon que ya tenías)
    float offset = 4.0f;
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - offset, y - offset);
    glVertex2f(x + ancho + offset, y - offset);
    glVertex2f(x + ancho + offset, y + alto + offset);
    glVertex2f(x - offset, y + alto + offset);
    glEnd();

    // 2. CUERPO CON DEGRADADO E ILUMINACION
    glBegin(GL_QUADS);
    if (encima) {
        // Si está encima, aclaramos ambos colores del degradado (+0.2f)
        glColor3f(fmin(r1 + 0.2f, 1.0f), fmin(g1 + 0.2f, 1.0f), fmin(b1 + 0.2f, 1.0f));
        glVertex2f(x, y + alto);
        glVertex2f(x + ancho, y + alto);

        glColor3f(fmin(r2 + 0.2f, 1.0f), fmin(g2 + 0.2f, 1.0f), fmin(b2 + 0.2f, 1.0f));
        glVertex2f(x + ancho, y);
        glVertex2f(x, y);

    }
    else {
        // Colores normales si no está encima
        glColor3f(r1, g1, b1);
        glVertex2f(x, y + alto);
        glVertex2f(x + ancho, y + alto);

        glColor3f(r2, g2, b2);
        glVertex2f(x + ancho, y);
        glVertex2f(x, y);

    }
    glEnd();

    // 3. TEXTO CENTRADO
    float anchoTexto = strlen(texto) * 11.0f; // Ajuste para Helvetica_18
    float posX = x + (ancho - anchoTexto) / 2.0f;
    float posY = y + (alto / 2.0f) - 6.0f;
    dibujaTexto(texto, posX, posY, 1.0f, 1.0f, 1.0f);
}

void Interfaz::dibujaTexto(const char* texto, float x, float y, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D); // 1. Apagar texturas [cite: 159]
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();           // Guardamos la matriz actual

    // EL CAMBIO CLAVE: Multiplicamos la coordenada x por correccionX
    // Esto desplaza el "cursor" de texto para que coincida con el botón o caja
    glColor3f(r, g, b);
    glRasterPos2f(x, y);

    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }

    glPopMatrix();
}

void Interfaz::dibujaPopUp(const char* titulo, const std::vector<std::string>& lineas, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // 1. Fondo oscurecido (igual que antes)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(800, 0);
    glVertex2f(800, 600); glVertex2f(0, 600);
    glEnd();

    // 2. Caja de color
    float x = 150, y = 150, w = 500, h = 300; // La hacemos un pelín más ancha
    glColor3f(r * 0.6f, g * 0.6f, b * 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();

    // Borde brillante
    glColor3f(r, g, b);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();
    glDisable(GL_BLEND);

    // 3. Dibujar Título (en la parte superior de la caja)
    dibujaTexto(titulo, x + 30, y + h - 50, 1.0f, 1.0f, 1.0f);

    // 4. Dibujar múltiples líneas de descripción
    float espaciado = 35.0f; // Espacio entre cada línea
    for (int i = 0; i < lineas.size(); i++) {
        // Calculamos la posición Y para que cada línea baje un poco más
        float posY = (y + h - 100) - (i * espaciado);
        dibujaTexto(lineas[i].c_str(), x + 30, posY, 0.9f, 0.9f, 0.9f);
    }

    // 5. Botón de cerrar (X)
    dibujaBoton(x + w - 40, y + h - 40, 30, 30, "X", 1.0f, 0.0f, 0.0f, 1, 1, 1);
}

void Interfaz::dibujaBotonCircular(float cx, float cy, float radio, ETSIDI::Sprite& imagen, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // COMPROBACIÓN DE SI EL RATÓN ESTÁ DENTRO DEL CÍRCULO
    float d = sqrt(pow(Mundo::mouseX - cx, 2) + pow(Mundo::mouseY - cy, 2));
    bool encima = (d < radio);

    glPushMatrix(); 

    if (encima) {
        glutSetCursor(GLUT_CURSOR_TOP_SIDE); 

        // Efecto de aumento
        glTranslatef(cx, cy, 0.0f);     
        glScalef(1.1f, 1.1f, 1.0f);      
        glTranslatef(-cx, -cy, 0.0f);    

        // ILUMINACION 
        glColor3f(fmin(r + 0.2f, 1.0f), fmin(g + 0.2f, 1.0f), fmin(b + 0.2f, 1.0f));
    }
    else {
        glColor3f(r, g, b);
    }

    // 1. DIBUJAR EL CÍRCULO (FONDO)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(cx + radio * cos(theta), cy + radio * sin(theta));
    }
    glEnd();

    // 2. BORDE BLANCO (ESTILO CARTOON)
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(cx + radio * cos(theta), cy + radio * sin(theta));
    }
    glEnd();

    // 3. DIBUJAR LA IMAGEN (ICONO)
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    float lado = radio * 1.4f;

    // Si al restar se fue lejos, es que el Sprite se centra solo.
    // Usamos directamente las coordenadas del centro del círculo.
    imagen.setSize(lado, lado);
    imagen.setCenter(lado / 2.0f, lado / 2.0f);
    imagen.setPos(cx, cy);
    imagen.draw();
    glDisable(GL_TEXTURE_2D);
}


bool Interfaz::botonPulsado(float mouseX, float mouseY, float btnX, float btnY, float btnAncho, float btnAlto) {
    return (mouseX >= btnX && mouseX <= (btnX + btnAncho) &&
        mouseY >= btnY && mouseY <= (btnY + btnAlto));
}
bool Interfaz::botonCircularPulsado(float clickX, float clickY, float cx, float cy, float radio) {
    // Distancia euclídea: d = sqrt((x2-x1)^2 + (y2-y1)^2)
    float d = sqrt(pow(clickX - cx, 2) + pow(clickY - cy, 2));
    return d < radio;
}

void Interfaz::mostrarInfoBando(int bando) {
    if (bando == 1) { // Healthy
        dibujaPopUp("HEALTHY TEAM",
            {
                "Vida: 120",
                "Ataque: Nutricion nutritiva"
            },
            0.0f, 1.0f, 0.0f);
    }
    else if (bando == 2) { // Junk
        dibujaPopUp("JUNK TEAM",
            {
                "Vida: 100",
                "Ataque: Grasas Saturadas",
                "Especial: Lluvia de Ketchup"
            },
            1.0f, 0.5f, 0.0f);
    }
}

void Interfaz::mostrarInfoTablero(int tipo) {
    if (tipo == 3) {
        // Usamos tu función molde con los textos de ayuda
        dibujaPopUp("GUIA DE JUEGO",
            {
                "- Selecciona una ficha con clic izquierdo.",
                "- Las casillas blancas son movimientos validos.",
                "- Captura al jefe enemigo para ganar."
            },
            0.0f, 0.0f, 1.0f);
    }
    else if (tipo == 4) {
        // Usamos el mismo molde pero con textos de configuración
        dibujaPopUp("AJUSTES",
            {
                "MUSICA: [ ON ]",
                "SONIDO: [ ON ]",
                "DIFICULTAD: NORMAL"
            },
            1.0f, 0.4f, 0.8f);
    }
}

void Interfaz::dibujaPantallaNombre(int numJugador, std::string nombreActual) {
    // 1. Fondo de la pantalla
    fondo.draw();

    // 2. Título principal con sombra para que resalte
    glColor3f(1.0f, 1.0f, 1.0f); // Texto blanco
    dibujaTexto("CONFIGURACION DE JUGADORES", 230, 500, 1.2f, 1.2f, 1.2f);

    // 3. Subtítulo dinámico (Jugador 1 o Jugador 2)
    //char subTitulo[50];
    //printf(subTitulo, "INTRODUCE NOMBRE DEL JUGADOR %d", numJugador);
    //dibujaTexto(subTitulo, 240, 420, 0.9f, 0.9f, 0.9f);

    // 4. Dibujo de la "Caja de Texto"
    // Fondo oscuro de la caja
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(200, 350);
    glVertex2f(600, 350);
    glVertex2f(600, 280);
    glVertex2f(200, 280);
    glEnd();

    // Borde resaltado de la caja
    glColor3f(0.0f, 1.0f, 0.8f); // Color cian/tecnológico
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(200, 350);
    glVertex2f(600, 350);
    glVertex2f(600, 280);
    glVertex2f(200, 280);
    glEnd();
    glLineWidth(1.0f);

    // 5. El texto que el usuario está escribiendo
    // Añadimos un cursor parpadeante al final
    static int frameCounter = 0;
    frameCounter++;
    std::string textoVisible = nombreActual;
    if ((frameCounter / 30) % 2 == 0) { // Parpadeo cada 30 frames aprox.
        textoVisible += "|";
    }

    // Dibujamos el nombre dentro de la caja
    dibujaTexto(textoVisible.c_str(), 220, 305, 1.2f, 1.0f, 1.0f);

    // 6. Instrucciones en la parte inferior
    glColor3f(1.0f, 1.0f, 1.0f);
    dibujaTexto("Pulsa ENTER para confirmar nombre", 270, 230, 0.7f, 0.7f, 0.7f);

    if (nombreActual.empty()) {
        glColor3f(0.5f, 0.5f, 0.5f);
        dibujaTexto("Escribe aqui...", 220, 305, 0.8f, 0.8f, 0.8f);
    }

    glDisable(GL_BLEND);
    
    // 6. BOTÓN VOLVER
    dibujaBotonCircular(60, 540, 25, iconoVolver, 0.5f, 0.5f, 0.5f);
}

void Interfaz::dibujaMenuRanking(std::string nombreJugadorActual) {
    fondo.draw(); // Primero el fondo
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 1. Fondo de la caja (Rosa oscuro/morado con transparencia)
    glColor4f(0.2f, 0.0f, 0.1f, 0.75f);
    glBegin(GL_QUADS);
    glVertex2f(150, 460); // Arriba Izquierda
    glVertex2f(650, 460); // Arriba Derecha
    glVertex2f(650, 80);  // Abajo Derecha
    glVertex2f(150, 80);  // Abajo Izquierda
    glEnd();

    // 2. Borde Brillante (Rosa Neón)
    glColor3f(1.0f, 0.0f, 0.8f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(150, 460);
    glVertex2f(650, 460);
    glVertex2f(650, 80);
    glVertex2f(150, 80);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    
    // 3. Dibujar
    dibujaTexto("RANKING - LOS 5 MAS RAPIDOS", 250, 200, 1.0f, 1.0f, 0.0f);
    
    std::vector<EntradaRanking> lista;

    // 1. Leer todos los datos del archivo
    FILE* f = nullptr;
    errno_t err = fopen_s(&f, "ranking.bin", "rb");

    if (err == 0 && f != nullptr) {
        EntradaRanking aux;
        // Leemos mientras haya datos
        while (fread(&aux, sizeof(EntradaRanking), 1, f)) {
            lista.push_back(aux);
        }
        fclose(f);
    }
    else {
        // Esto solo sale si el archivo NO existe
        std::cout << "DEBUG: No existe el archivo o esta vacio." << std::endl;
    }

    // 2. Ordenar por turnos
    std::sort(lista.begin(), lista.end(), [](const EntradaRanking& a, const EntradaRanking& b) {
        return a.turnos < b.turnos;
        });

    


    int miPuesto = -1;
    for (int i = 0; i < (int)lista.size(); i++) {
        // Guardamos el puesto del jugador actual
        // IMPORTANTE: strcmp si nombre es char[]
        if (nombreJugadorActual == lista[i].nombre) miPuesto = i + 1;

        if (i < 5) {
            char linea[100];
            // CAMBIO CLAVE: sprintf_s para llenar la cadena 'linea'
            sprintf_s(linea, "%d. %s - %d turnos (%s)", i + 1, lista[i].nombre, lista[i].turnos, lista[i].bando);
            dibujaTexto(linea, 200, 430 - (i * 40), 1.0f, 1.0f, 1.0f);
        }
    }

    // 4. Mostrar info del jugador actual
    if (miPuesto != -1) {
        char resumen[100];
        // CAMBIO CLAVE: sprintf_s aquí también
        sprintf_s(resumen, "Tu puesto actual: %d de %d jugadores", miPuesto, (int)lista.size());
        dibujaTexto(resumen, 250, 150, 0.8f, 1.0f, 0.8f);
    }
    else if (lista.empty()) {
        dibujaTexto("No hay partidas registradas todavia", 250, 300, 0.7f, 0.7f, 0.7f);
    }

    // 6. BOTÓN VOLVER
    dibujaBotonCircular(60, 540, 25, iconoVolver, 0.5f, 0.5f, 0.5f);
}