#ifndef GRFGENERATOR_H
#define GRFGENERATOR_H

#include "dataio.h"

class GrfGenerator
{
public:
    static Zapis &GenerateZapis();

private:
    GrfGenerator();
    static Shapka &GenShapka();
    static ShapkaSector &GenShapkaSector();
    static Sector &GenSector();
};

#endif // GRFGENERATOR_H
