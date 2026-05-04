#include "Comida.h"
#include "freeglut.h"
#include "Definiciones.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Constructor
Comida::Comida(Bando b, TipoFicha t, int f, int c) {
    bando = b;
    tipo = t;
    fila = f;
    columna = c;

    // Asignación de estadísticas según el tipo de tropa
    switch (tipo) {
    case LIDER:
        // El Rey de la Hamburguesa / Chef Michelin
        vidaMax = 100;
        ataque = 25;
        rangoMovimiento = 3;
        break;

    case PESADA:
        // Hamburguesa Triple / Aguacate Escudo (Tanques)
        vidaMax = 150;       // Muchísima vida
        ataque = 20;         // Pega duro cuerpo a cuerpo
        rangoMovimiento = 2; // Muy lentos
        break;

    case VOLADORA:
        // Pizza Pepperoni / Rodaja de Limón
        vidaMax = 50;        // Poca vida 
        ataque = 15;
        rangoMovimiento = 2; // Pueden cruzar medio tablero rápido
        break;

    case DISTANCIA:
        // Disparo de Ketchup / Chorro de Vitaminas (Los peones)
        vidaMax = 40;        // Muy poca vida, mueren rápido
        ataque = 10;         // Daño bajo en tablero
        rangoMovimiento = 3; // Movimiento estándar
        break;

    case ESPECIAL:
        // Helado Shapeshifter / Jengibre Teleport
        vidaMax = 80;        // Vida media
        ataque = 30;         // Tienen el ataque más fuerte 
        rangoMovimiento = 4; // Muy ágiles
        break;

    default:
        // Por si acaso hay un error, creamos una ficha genérica
        vidaMax = 50;
        ataque = 10;
        rangoMovimiento = 3;
        break;
    }

    // Al nacer, la ficha siempre tiene la vida a tope
    vidaActual = vidaMax;
}

// Función de dibujo temporal (cuadrados de colores para hacer pruebas)
void Comida::dibuja(float xMin, float yMin, float lado) {
    float centroX = xMin + (lado / 2.0f);
    float centroY = yMin + (lado / 2.0f);
    float radioFicha = lado * 0.3f;

    // Colores base por bando
    if (bando == SALUDABLE) glColor3ub(0, 150, 0); // Verde para la comida sana
    else glColor3ub(150, 0, 0);                    // Rojo para la comida rápida

    // Detalles visuales por tipo para distinguirlos en tu pantalla
    if (tipo == LIDER) glColor3ub(255, 215, 0);         // Dorado (Líder)
    else if (tipo == PESADA) glColor3ub(100, 100, 100); // Gris oscuro (Tanque)
    else if (tipo == DISTANCIA) glColor3ub(100, 200, 255); // Azul claro (Tirador)

    glBegin(GL_QUADS);
    glVertex2f(centroX - radioFicha, centroY - radioFicha);
    glVertex2f(centroX + radioFicha, centroY - radioFicha);
    glVertex2f(centroX + radioFicha, centroY + radioFicha);
    glVertex2f(centroX - radioFicha, centroY + radioFicha);
    glEnd();
}
//  FUNCIONES PENDIENTES 
bool Comida::intentarMover(int nuevaFila, int nuevaColumna) {
    int distFilas = std::abs(nuevaFila - fila);
    int distColumnas = std::abs(nuevaColumna - columna);

    // Regla de oro 1: No puedes "moverte" a la casilla donde ya estás
    if (distFilas == 0 && distColumnas == 0) return false;

    // --- EL CÁLCULO DEL COSTE ---
    int costeMovimiento;
    if (tipo == VOLADORA || tipo == LIDER) {
        // Los que vuelan usan el coste máximo entre los ejes.
        // Moverse 3 casillas en diagonal (3, 3) les cuesta solo 3 de rango.
        costeMovimiento = std::max(distFilas, distColumnas);
    }
    else {
        // Los terrestres suman sus pasos.
        costeMovimiento = distFilas + distColumnas;
    }

    if (costeMovimiento > rangoMovimiento) return false;
 

    switch (tipo) {

    case PESADA:
        // Solo puede moverse en LÍNEA RECTA 
        if (distFilas == 0 || distColumnas == 0) {
            return true;
        }
        else {
            return false; 
        }
        break;

    case VOLADORA:
    
        // Mientras esté en su rango, puede ir donde quiera.
        return true;
        break;

    case LIDER:
        // rango bajo, donde quiera
        return true;
        break;

    case DISTANCIA:
        if (distColumnas == 0) {
            return true;
        }
        else {
            return false;
        }
        break;

    case ESPECIAL:
        // como el caballo.
       
        if ((distFilas == 3 && distColumnas == 1) || (distFilas == 1 && distColumnas == 3)) {
            return true;
        }
        else {
            return false;
        }
        break;
    }
}
void Comida::recibirDano(int cantidad) {
    // Aquí se programará cómo baja la vida.
    // De momento, restamos la cantidad directamente.
    vidaActual -= cantidad;

    if (vidaActual < 0) {
        vidaActual = 0;
    }
}