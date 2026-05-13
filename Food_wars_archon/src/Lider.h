#pragma once
#include "Comida.h"
class Lider :
    public Comida
{
protected:

    Lider(Bando b, int f, int c);

public:

    bool intentarMover(int nuevaFila, int nuevaColumna) override;
};

