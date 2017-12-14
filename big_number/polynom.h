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

    polynom( std::string str );

    explicit polynom( int quantityPower, FillTypePolynom filltype = FillTypePolynomZero );

    polynom operator + ( const polynom & inputPolynom ) const;

    polynom operator * ( const polynom & inputPolynom ) const;

    polynom &operator = ( const polynom & inputPolynom );

    void print();

    int coefficient(int number) const;

    void checkLength();
};

#endif // POLYNOM_H
