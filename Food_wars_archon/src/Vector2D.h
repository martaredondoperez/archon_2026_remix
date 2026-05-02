#pragma once

class Vector2D {
public:
    float x, y;

    // Constructor que inicializa a cero por defecto
    Vector2D(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

    // Un método útil para sumar vectores (posicion + velocidad)
    Vector2D operator + (const Vector2D& v) const {
        return Vector2D(x + v.x, y + v.y);
    }
};