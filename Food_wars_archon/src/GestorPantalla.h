#pragma once

class GestorPantalla {
private:
    float anchoLogico;    // 800
    float altoLogico;     // 600
    float aspectJuego;
    float viewportX, viewportY, viewportAncho, viewportAlto;

public:
    GestorPantalla(float ancho = 800.0f, float alto = 600.0f);
    ~GestorPantalla() = default;

    // Configurar la cámara ortográfica (llamar al inicio de cada draw)
    void configurarCamara();

    // Dibujar franjas negras en los lados si la pantalla tiene otro aspecto
    void dibujarFranjas();

    // Convertir coordenadas de píxeles de ventana a coordenadas de juego
    void convertirCoordenadasMouse(float pixelX, float pixelY, float& juegoX, float& juegoY);

    // Getters para obtener dimensiones
    float getAnchoLogico() const { return anchoLogico; }
    float getAltoLogico() const { return altoLogico; }
};
