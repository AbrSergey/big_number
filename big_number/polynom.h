#ifndef POLYNOM_H
#define POLYNOM_H
#include <string>

typedef unsigned int Base;

enum FillTypePolynom{

    FillTypePolynomZero,

    FillTypeRandomPolynomPower,
};

class polynom
{
private:

    Base * m_data;

    int m_len;  // power + 1

    int m_capacity; // quantity of bases

public:
    polynom();

    explicit polynom(int quantityPower, FillTypePolynom filltype = FillTypePolynomZero);

    void print();
};

#endif // POLYNOM_H
