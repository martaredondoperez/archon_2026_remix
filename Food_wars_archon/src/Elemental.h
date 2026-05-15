#pragma once
#include "Comida.h"
class Elemental :
    public Comida
{
protected:

    Elemental(Bando b, int f, int c);

public:

    bool intentarMover(int nuevaFila, int nuevaColumna) override;
};

