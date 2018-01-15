#ifndef POLYNOM_H
#define POLYNOM_H
#include <string>
#include "big_number.h"

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
    polynom ();

    polynom ( std::string str );

    polynom (int arr[] , int len);

    polynom (const polynom & numberPolynom);

    explicit polynom( int quantityPower, FillTypePolynom filltype = FillTypePolynomZero );

    polynom operator + ( const polynom & inputPolynom ) const;

    polynom operator * ( const polynom & inputPolynom ) const;

    polynom operator / ( const polynom & inputPolynom ) const;

    polynom operator % ( const polynom & inputPolynom ) const;

//    polynom operator ^ ( const polynom & inputPolynom ) const;

    polynom &operator = ( const polynom & inputPolynom );

    polynom operator << ( const int q ) const;

    bool operator ==( const int inputNumber ) const;

    bool operator != (const int numberOne) const;

    bool operator != (polynom input) const;

    polynom gcd (const polynom inputPolynom) const;

    bool reducability () const;

    bool primitive () const;

    void division (const polynom & inputPolynom, polynom & whole, polynom & remainder) const;

    void print();

    int coefficient(int number) const;

    void checkLength();

    int lenBase();

    int power () const;

    polynom pow (const big_number & y, const polynom &mod );
};

#endif // POLYNOM_H
