#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H
#include <string>
#include <exception>
#include <stdexcept>

#define KAR_BASES 70

struct outTDM;

typedef unsigned int Base;

typedef unsigned short int HalfBase;

typedef unsigned long long DoubleBase;

enum FillType{

    FillTypeZero,

    FillTypeRandom,

    FillTypeRandomBits
};



class big_number
{
private:

    const int NUM_HEX_DIGITS_IN_BASE = 2 * sizeof(Base);

    Base* m_data;

    int m_len;

    int m_capacity;

    friend int checkLength( big_number& );

    friend int symbolLegendre(int, int);

    friend int symbolJacobi(int, int n);

public:

    big_number();

    big_number( std::string str );

    big_number (const big_number& number);

    explicit big_number(unsigned int quantity, FillType filltype = FillTypeZero);

    void printHex() const;

    void printDbg() const;
    
    big_number operator + ( const big_number & input_number ) const;

    big_number operator + (const int & n ) const;

    big_number operator - ( const big_number & input_number ) const;

    big_number operator - ( const int & n ) const;

    big_number operator * ( const big_number & input_number ) const;

    big_number operator * ( DoubleBase input_number ) const;

    big_number operator * ( Base input_number ) const;

    big_number operator ^ ( const int power ) const;

    big_number operator / ( const big_number & input_number ) const;

    big_number operator / ( const int & input_number ) const;

    big_number operator % ( const big_number & input_number ) const;

    big_number operator % ( const int & input_number ) const;

    big_number &operator = ( const big_number & input_number );

    big_number &operator = ( const int input_number );

    big_number &operator <<= ( const int m );

    big_number operator << ( const int m ) const;

    big_number operator >> (const int m) const;

    bool operator > ( const big_number & input_number ) const;

    bool operator < ( const big_number & input_number ) const;

    bool operator < ( const unsigned int a) const;

    bool operator == ( const big_number & input_number ) const;

    bool operator == ( const int input_number) const;

    bool operator != ( const int input_number) const;

    bool operator >= ( const big_number & input_number ) const;

    big_number pow (const big_number & y, const big_number & mod ) const;

    big_number pow (const int y, const big_number & mod ) const;

    big_number Kar (const big_number & v) const;

    big_number Bar (const big_number & m, big_number &z) const;

   // bool operator == ( Base & input_number ) const;

    int function2 (big_number&r ) const;

    void checkLength();

    int symbolLegendre(int a, int p);

    int symbolJacobi(int a, int p);

    void division (const big_number & input_number, big_number & q, big_number & r) const;

    void div_base (Base d, Base &remainder);

    bool shift_sub (const big_number & input_number, int j);

    void shift_add (const big_number & input_number, int j);

    void allocate_and_fill_zeroes(int len);

    int len() const;

    big_number zComp ();

    unsigned int zeroCount (); // need to test

    bool testMillerRabin (int t) const;

    big_number * fft(const int &n, const int &k, big_number &w, const big_number *a);

    void testfft();

    big_number sqrt () const;

    // factorization

    int testDivisorMethod (const big_number & input_number, outTDM * result, bool &isFactorized);

    void siftingMethodFerma (const big_number &n, big_number & a, big_number & b); // returns an array big_number of two numbers (if the number is prime then number and zero)

    // logarithm

    big_number primitiveRoot () const;

    big_number polygHellman (const big_number & g, const big_number &a) const;
};

struct outTDM {

    big_number prime_number;

    int power;
};

const big_number ZERO = big_number(1);

#endif // BIG_NUMBER_H
