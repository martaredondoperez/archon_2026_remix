#include "PopUp.h"
#include "freeglut.h"
#include "BotonRectangular.h" // <--- ¡CRUCIAL! Para poder instanciar el hijo

PopUp::PopUp(std::string _titulo, float _x, float _y, float _ancho, float _alto)
    : titulo(_titulo), x(_x), y(_y), ancho(_ancho), alto(_alto)
{
    // Si tu esquina superior derecha es sumando ancho y alto:
    float bx = _x + _ancho - 40.0f;
    float by = _y + _alto - 40.0f;
    float tam = 25.0f;

    botonCerrar = new BotonRectangular(bx, by, tam, tam, "X",
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f);

    visible = false;
    r = 0.2f; g = 0.2f; b = 0.2f;
}

PopUp::~PopUp() {
    if (botonCerrar != nullptr) {
        delete botonCerrar; // Al ser virtual el destructor de Boton, liberará correctamente el hijo
    }
    limpiarBotones();
}

void PopUp::anadirLinea(const std::string& texto) {
    lineas.push_back(texto);
}

void PopUp::actualizarMouse(float mx, float my) {
    if (botonCerrar != nullptr) {
        botonCerrar->setMouseOver(botonCerrar->isMouseOver(mx, my));
    }
    for (auto b : botonesInteractivos) {
        if (b != nullptr) {
            b->setMouseOver(b->isMouseOver(mx, my));
        }
    }
}

bool PopUp::gestionarClick(float mouseX, float mouseY) {
    if (!visible) return false;

    if (botonCerrar != nullptr && botonCerrar->isMouseOver(mouseX, mouseY)) {
        visible = false;
        return true;
    }

    // Verificar clics en botones interactivos
    for (auto b : botonesInteractivos) {
        if (b != nullptr && b->isMouseOver(mouseX, mouseY)) {
            b->ejecutar();
            return false;
        }
    }
    return false;
}

void PopUp::dibuja() {
    if (!visible) return;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 0. FONDO OSCURECIDO Y TRANSPARENTE (VELO)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f); // Negro con 40% de transparencia
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();
    glDisable(GL_BLEND);

    // 1. FONDO DEL POPUP
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto);
    glVertex2f(x, y + alto);
    glEnd();

    // 2. BORDE DEL POPUP
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto);
    glVertex2f(x, y + alto);
    glEnd();
    glLineWidth(1.0f);

    // 3. DIBUJAR BOTÓN CERRAR (X)
    if (botonCerrar != nullptr) {
        botonCerrar->dibuja();
    }

    // Dibujar botones interactivos
    for (auto b : botonesInteractivos) {
        if (b != nullptr) {
            b->dibuja();
        }
    }

    // 4. TEXTOS
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x + 20.0f, y + alto - 30.0f);
    for (char c : titulo) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    for (size_t i = 0; i < lineas.size(); i++) {
        glRasterPos2f(x + 20.0f, (y + alto - 80.0f) - (i * 30.0f));
        for (char c : lineas[i]) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glPopAttrib();
}

void PopUp::anadirBoton(Boton* boton) {
    if (boton != nullptr) {
        botonesInteractivos.push_back(boton);
    }
}

void PopUp::limpiarBotones() {
    for (auto b : botonesInteractivos) {
        if (b != nullptr) {
            delete b;
        }
    }
    botonesInteractivos.clear();
}