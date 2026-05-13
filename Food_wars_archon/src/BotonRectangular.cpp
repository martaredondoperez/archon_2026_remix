#include "BotonRectangular.h"
#include "ETSIDI.h"

BotonRectangular::BotonRectangular(float _x, float _y, float _w, float _h, std::string _t,
    float _r1, float _g1, float _b1, float _r2, float _g2, float _b2)
    : Boton(_x, _y, _r1, _g1, _b1), ancho(_w), alto(_h), texto(_t), r2(_r2), g2(_g2), b2(_b2) {
}

void BotonRectangular::dibuja() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // 1. OBTENER EFECTOS DE LA MADRE
    float s, b;
    gestionarEfectos(s, b);

    // 2. LOGICA DE TAMAÑO (Escalado centrado)
    float anchoE = ancho * s;
    float altoE = alto * s;
    float ox = (anchoE - ancho) / 2.0f;
    float oy = (altoE - alto) / 2.0f;

    // 3. DIBUJAR EL RECTÁNGULO (RELLENO)
    // Usamos el color base + el brillo 'b' que nos da la madre
    glColor3f(this->r + b, this->g + b, this->b + b);

    glBegin(GL_QUADS);
    glVertex2f(x - ox, y - oy);
    glVertex2f(x + ancho + ox, y - oy);
    glVertex2f(x + ancho + ox, y + alto + oy);
    glVertex2f(x - ox, y + alto + oy);
    glEnd();

    // 4. DIBUJAR EL BORDE
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(mouseOver ? 3.0f : 1.5f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x - ox, y - oy);
    glVertex2f(x + ancho + ox, y - oy);
    glVertex2f(x + ancho + ox, y + alto + oy);
    glVertex2f(x - ox, y + alto + oy);
    glEnd();

    // 5. EL TEXTO (Se queda en su sitio original para que no baile)
    float mitad_texto_pixeles = (texto.length() * 5.3f);
    float textX = (x + ancho / 2.0f) - mitad_texto_pixeles;
    float textY = y + (alto / 2.0f) - 5.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(textX, textY);
    for (int i = 0; i < (int)texto.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
}

bool BotonRectangular::isMouseOver(float mx, float my) {
    return (mx >= x && mx <= x + ancho && my >= y && my <= y + alto);
}

