#include "Interfaz.h"
#include "freeglut.h"
#include <string.h>
#include "ETSIDI.h"
#include "Mundo.h"
#include <stdlib.h>
#include <iostream>
#include <algorithm>

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
    // INICIALIZACIÓN DE OBJETOS BOTÓN (Sustituye a las llamadas manuales)
}

void Interfaz::inicializa(Mundo* mundo) {
    limpiarBotones(); // Borra lo que hubiera antes para no duplicar

    // ==========================================
    // 1. BOTONES MENÚ PRINCIPAL
    // ==========================================
    Boton* b1j = new BotonRectangular(300, 300, 200, 60, "1 JUGADOR", 0.5f, 0.9f, 0.2f, 0.1f, 0.5f, 0.0f);
    b1j->setAccion([mundo]() { mundo->setNumJugadores(1); mundo->setEstado(PANTALLA_NOMBRE); });
    mapaBotones[MENU_PRINCIPAL].push_back(b1j);

    Boton* b2j = new BotonRectangular(300, 220, 200, 60, "2 JUGADORES", 1.0f, 0.5f, 0.1f, 0.7f, 0.1f, 0.0f);
    b2j->setAccion([mundo]() { mundo->setNumJugadores(2); mundo->setEstado(PANTALLA_NOMBRE); });
    mapaBotones[MENU_PRINCIPAL].push_back(b2j);

    Boton* b_inst = new BotonRectangular(300, 140, 200, 60, "INSTRUCCIONES", 0.8f, 0.2f, 0.2f, 0.4f, 0.0f, 0.0f);
    b_inst->setAccion([mundo]() { mundo->setEstado(INSTRUCCIONES); });
    mapaBotones[MENU_PRINCIPAL].push_back(b_inst);

    Boton* b_exit = new BotonRectangular(670, 30, 110, 45, "SALIR", 0.8f, 0.1f, 0.1f, 0.4f, 0.0f, 0.0f);
    b_exit->setAccion([]() { exit(0); });
    mapaBotones[MENU_PRINCIPAL].push_back(b_exit);

    // ==========================================
    // 2. BOTONES SELECCIÓN DE BANDO
    // ==========================================
    Boton* b_hlth = new BotonRectangular(100, 500, 180, 60, "HEALTHY", 0.5f, 0.9f, 0.2f, 0.1f, 0.5f, 0.0f);
    b_hlth->setAccion([mundo]() { mundo->seleccionarBando(1); });
    mapaBotones[SELECCION_BANDO].push_back(b_hlth);

    Boton* b_jnk = new BotonRectangular(500, 500, 180, 60, "JUNK", 1.0f, 0.5f, 0.1f, 0.7f, 0.1f, 0.0f);
    b_jnk->setAccion([mundo]() { mundo->seleccionarBando(2); });
    mapaBotones[SELECCION_BANDO].push_back(b_jnk);

    Boton* b_iH = new BotonCircular(260, 530, 20.0f, &iconoInfo, 0.2f, 0.5f, 0.9f);
    b_iH->setAccion([mundo]() { mundo->setInfoActual(INFO_HEALTHY); });
    mapaBotones[SELECCION_BANDO].push_back(b_iH);

    Boton* b_iJ = new BotonCircular(660, 530, 20.0f, &iconoInfo, 0.2f, 0.5f, 0.9f);
    b_iJ->setAccion([mundo]() { mundo->setInfoActual(INFO_JUNK); });
    mapaBotones[SELECCION_BANDO].push_back(b_iJ);

    Boton* b_vS = new BotonCircular(40, 560, 25, &iconoVolver, 0.3f, 0.3f, 0.3f);
    b_vS->setAccion([mundo]() { mundo->setEstado(MENU_PRINCIPAL); });
    mapaBotones[SELECCION_BANDO].push_back(b_vS);

    // ==========================================
    // 3. BOTONES INSTRUCCIONES
    // ==========================================
    Boton* b_vI = new BotonCircular(150, 450, 25, &iconoVolver, 0.3f, 0.3f, 0.3f);
    b_vI->setAccion([mundo]() { mundo->setEstado(MENU_PRINCIPAL); });
    mapaBotones[INSTRUCCIONES].push_back(b_vI);

    // ==========================================
    // 4. BOTONES CONFIGURACIÓN (O INFO ACTUAL)
    // ==========================================
     /*Boton* b_mus = new BotonRectangular(300, 320, 200, 50, "MUSICA: ON", 0.2f, 0.8f, 0.2f, 0.1f, 0.4f, 0.0f);
    // (Falta acción para música si la tienes)
    mapaBotones[CONFIGURACION].push_back(b_mus);

    Boton* b_cC = new BotonRectangular(325, 200, 150, 40, "CERRAR", 0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f);
    b_cC->setAccion([m]() { m->setInfoActual(NINGUNA); });
    mapaBotones[CONFIGURACION].push_back(b_cC);
    */
    // ==========================================
    // 5. BOTONES MENÚ FINAL (GAMEOVER)
    // ==========================================
    Boton* b_rk = new BotonRectangular(300, 320, 200, 50, "RANKING", 1.0f, 0.5f, 0.7f, 0.7f, 0.2f, 0.4f);
    b_rk->setAccion([mundo]() { mundo->setEstado(RANKING); });
    mapaBotones[GAMEOVER].push_back(b_rk);

    Boton* b_rt = new BotonRectangular(300, 240, 200, 50, "REINTENTAR", 0.3f, 0.7f, 1.0f, 0.1f, 0.4f, 0.8f);
    b_rt->setAccion([mundo]() { mundo->setEstado(MENU_PRINCIPAL); });
    mapaBotones[GAMEOVER].push_back(b_rt);

    Boton* b_sF = new BotonRectangular(300, 160, 200, 50, "SALIR DEL JUEGO", 1.0f, 0.2f, 0.2f, 0.6f, 0.0f, 0.0f);
    b_sF->setAccion([]() { exit(0); });
    mapaBotones[GAMEOVER].push_back(b_sF);

    // ==========================================
    // 6. BOTONES POPUP (INFO_ACTUAL)
    // ==========================================
    /*Boton* b_cX = new BotonRectangular(610, 410, 30, 30, "X", 1.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f);
    b_cX->setAccion([mundo]() { mundo->setInfoActual(NINGUNA); });
    mapaBotones[POPUP].push_back(b_cX); // Asegúrate que POPUP esté en el Enum
    */
    // ==========================================
    // 7. BOTONES PANTALLA NOMBRE
    // ==========================================
    Boton* b_vN = new BotonCircular(60, 540, 25, &iconoVolver, 0.5f, 0.5f, 0.5f);
    b_vN->setAccion([mundo]() { mundo->setEstado(MENU_PRINCIPAL); });
    mapaBotones[PANTALLA_NOMBRE].push_back(b_vN);

    // ==========================================
    // 8. BOTONES RANKING
    // ==========================================
    Boton* b_vR = new BotonCircular(60, 540, 25, &iconoVolver, 0.5f, 0.5f, 0.5f);
    b_vR->setAccion([mundo]() { mundo->setEstado(GAMEOVER); });
    mapaBotones[RANKING].push_back(b_vR);

}

void Interfaz::dibujaMenu() {
     fondo.draw(); //dibujamos el fondo
    logo.draw();
    // 1. FONDO y LOGO (Ocupando toda la pantalla)
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // Luz total para la imagen
    glDisable(GL_TEXTURE_2D);//desact texturas
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

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

    //Botones
    dibujaBotones(MENU_PRINCIPAL);
    // 2. Texto encima en Blanco o Dorado suave
    dibujaTexto("ETSIDI - Informatica Industrial", 265, 45, 1.0f, 1.0f, 1.0f);
    
}


void Interfaz::dibujaSeleccion() {
    // 1. RESET DE CÁMARA
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 2. DIBUJO DEL FONDO DE SELECCIÓN
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    fondoSeleccion.draw();
    glDisable(GL_TEXTURE_2D);

    // 4. BOTONES
    dibujaBotones(SELECCION_BANDO);

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

    dibujaBotones(INSTRUCCIONES);

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
//    dibujaBoton(x, y, ancho, alto, "", 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f);

    // 2. Nombre de la unidad (Subimos un poco la Y)
    char n[50];
    sprintf_s(n, "%s", info.nombre.c_str());
    dibujaTexto(n, x + 10, y + 120, 1.0f, 1.0f, 0.0f); // y + 120 en lugar de 90

    // 3. Barra de vida (La mantenemos en una zona intermedia)
    float porcentaje = (info.vidaMax > 0) ? (float)info.vidaActual / info.vidaMax : 0.0f;
    //dibujaBoton(x + 10, y + 70, 150, 20, "", 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); // Fondo
    //dibujaBoton(x + 10, y + 70, 150 * porcentaje, 20, "", 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f); // Vida

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
    if (musicaActiva) {
      //  btnMusica->setTexto("MUSICA: ON");
        //btnMusica->setColores(0.2f, 0.8f, 0.2f, 0.1f, 0.4f, 0.0f); // Verde    
    }
    else {
       // btnMusica->setTexto("MUSICA: OFF");
        //btnMusica->setColores(0.8f, 0.2f, 0.2f, 0.4f, 0.0f, 0.0f); // Rojo
    }
    // Botones
    //dibujaBotones(CONFI);

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

    // Botones
    dibujaBotones(GAMEOVER);

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

    // Botones
     //  dibujaBotones(POPUP);

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
    
   //botones
    dibujaBotones(PANTALLA_NOMBRE);

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

    // botones
    dibujaBotones(RANKING);

}

Interfaz::~Interfaz() {
    for (auto& par : mapaBotones) {
        // 'par.first' es el Estado (la clave)
        // 'par.second' es el vector de botones (el valor)
        for (auto b : par.second) {
            delete b; // Aquí se aplica el polimorfismo: borra el hijo que sea
        }
        par.second.clear(); // Limpiamos el vector interno
    }
    mapaBotones.clear(); // Limpiamos el mapa completo
}

void Interfaz::gestionarClick(float mx, float my, Estado estadoActual) {
    if (mapaBotones.count(estadoActual)) {
        for (auto b : mapaBotones[estadoActual]) {
            if (b->isMouseOver(mx, my)) {
                b->ejecutar(); // El botón ya sabe qué tiene que hacer
                return; // Salimos para no clickear dos cosas a la vez
            }
        }
    }
}

void Interfaz::actualizaEstadoBotones(float mx, float my, Estado estadoActual) {
    
    if (mapaBotones.count(estadoActual)) {
        for (auto b : mapaBotones[estadoActual]) {
            b->setMouseOver(b->isMouseOver(mx, my));
        }
    }
}

void Interfaz::limpiarBotones() {
    // Recorremos el mapa: 'par.first' es el Estado, 'par.second' es el vector
    for (auto& par : mapaBotones) {
        for (auto b : par.second) {
            delete b;
        }
        par.second.clear();
    }
    mapaBotones.clear();
}

void Interfaz::dibujaBotones(Estado estadoActual) {
    // Buscamos si el estado actual tiene botones en el mapa
    if (mapaBotones.count(estadoActual)) {
        for (auto b : mapaBotones[estadoActual]) {
            // Aquí ocurre el POLIMORFISMO: 
            // 'b' es un Boton*, pero se llamará a BotonRectangular::dibuja() 
            // o BotonCircular::dibuja() según lo que sea el objeto realmente.
            b->dibuja();
        }
    }
}