#include "PopUp.h"
#include "freeglut.h"

PopUp::PopUp(std::string _titulo, float _x, float _y, float _ancho, float _alto)
    : titulo(_titulo), x(_x), y(_y), ancho(_ancho), alto(_alto)
{
    // 1. Creamos el botón dinámicamente con 'new'
    // El orden según tu Boton.h: x, y, r, g, b, texto
    botonCerrar = new Boton((_x + _ancho - 35.0f), (_y + _alto - 35.0f), 1.0f, 0.0f, 0.0f, "X");

    visible = false;
    r = 0.2f; g = 0.2f; b = 0.2f;
}

// 2. ¡MUY IMPORTANTE! Añade el destructor para evitar fugas de memoria
PopUp::~PopUp() {
    delete botonCerrar;
}

void PopUp::anadirLinea(const std::string& texto) {
    lineas.push_back(texto);
}

bool PopUp::gestionarClick(float mouseX, float mouseY) {
    if (!visible) return false;

    // 3. CAMBIO: Usamos flecha -> porque es un puntero
    if (botonCerrar != nullptr && botonCerrar->isMouseOver(mouseX, mouseY)) {
        visible = false;
        return true;
    }
    return false;
}

void PopUp::dibuja() {
    if (!visible) return;

    // 1. Fondo semitransparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(800, 0);
    glVertex2f(800, 600); glVertex2f(0, 600);
    glEnd();

    // 2. Caja del PopUp
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto); glVertex2f(x, y + alto);
    glEnd();

    // 4. CAMBIO: Usamos flecha -> para dibujar el botón
    if (botonCerrar != nullptr) {
        botonCerrar->dibuja();
    }

    // Texto del título
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x + 20, y + alto - 30);
    for (char c : titulo) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Texto de las líneas
    for (size_t i = 0; i < lineas.size(); i++) {
        glRasterPos2f(x + 20, (y + alto - 80) - (i * 30));
        for (char c : lineas[i]) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}