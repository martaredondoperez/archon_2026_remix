#pragma once
#include "Comida.h"

class pesada : public Comida {
protected:

    pesada(Bando b, int f, int c);

public:
    
    bool intentarMover(int nuevaFila, int nuevaColumna) override;
};