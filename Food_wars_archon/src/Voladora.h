#pragma once
#include "Comida.h"
class Voladora :
    public Comida
{
protected:

    Voladora(Bando b, int f, int c);

public:

    bool intentarMover(int nuevaFila, int nuevaColumna) override;
};

