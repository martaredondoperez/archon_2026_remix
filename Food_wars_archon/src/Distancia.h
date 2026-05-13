#pragma once
#include "Comida.h"
class Distancia :
    public Comida
{
protected:

    Distancia(Bando b, int f, int c);

public:

    bool intentarMover(int nuevaFila, int nuevaColumna) override;
};

