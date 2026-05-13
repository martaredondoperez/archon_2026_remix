#pragma once
#include "Comida.h"
class Especial :
    public Comida
{
protected:

    Especial(Bando b, int f, int c);

public:

    bool intentarMover(int nuevaFila, int nuevaColumna) override;
};

