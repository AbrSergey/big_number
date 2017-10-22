#include <iostream>
#include <stdio.h>
#include <string>
#include "big_number.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>


typedef unsigned int Base;

typedef unsigned short int HalfBase;

typedef unsigned long long DoubleBase;

using namespace std;

void test_pow();
void test_Kar_1();
void test_Kar_2();
void test_Bar_1();
void test_Bar_2();
big_number primeNumberGenerator(unsigned int lenBits);

int main(){

    big_number x("0x3");

    outTDM *r;

    int d[2];

    d[0] = 2; d[1] = 3;

    r = x.testDivisorMethod(x, *d);

    r->prime_number->printDbg();
    cout << endl << r->power << endl;


//    for (int n = 0; n < 1; n++){

//        big_number a = primeNumberGenerator(32);

//        cout << "prime number " << n << " = ";

//        a.printHex();

//        cout << endl;
//    }

    return 0;
}

big_number primeNumberGenerator(unsigned int lenBits)
{
    bool result;

    do{
//        srand(time(0));

        big_number a(lenBits, FillTypeRandomBits);

        result = a.testMillerRabin(10);

        if (result) return a;
    }
    while (!result);


}

void test_Bar_1(){

    for (int i = 0; i < 1; i++){

        big_number m(1, FillTypeRandom);
        big_number x(m.len()*2-1, FillTypeRandom);

        big_number z, C, D;

        z = m.zComp();

        C = x.Bar(m, z);
        D = x%m;

        if (C == D) std::cout << "True";

        else std::cout << "False";

        std::cout << endl;
    }
}

void test_Bar_2(){

    int mod_m = 2000, T = 100;

    big_number z;
    big_number m(mod_m, FillTypeRandom);

    z = m.zComp();

    clock_t start;
    double duration;

    start = std::clock();
    for (int i = 0; i < T; i++){
        big_number x(m.len()*2, FillTypeRandom);
        x.Bar(m, z);
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << "Bar = " << duration << std::endl;

    start = std::clock();
    for (int i = 0; i < T; i++){
        big_number x(m.len()*2, FillTypeRandom);
        x%m;
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << "% = " << duration << std::endl;


}

void test_Kar_1(){

    int T = 500, M = 1000, m, n;

    for (int i = 0; i < T; i++){

        m = 1 + rand()%M;
        n = 1 + rand()%M;

        big_number A(m, FillTypeRandom);
        big_number B(n, FillTypeRandom);
        big_number C, D;

        C = A.Kar(B);
        D = A*B;

        if (!(C == D))    std::cout << "False";
    }
}

void test_Kar_2(){

    int T = 50, M = 1500;

    big_number A(M, FillTypeRandom);
    big_number B(M, FillTypeRandom);
    big_number C;

    clock_t start;
    double duration;

    start = std::clock();
    for (int i = 0; i < T; i++) A.Kar(B);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << "Kar = " << duration << std::endl;

    start = std::clock();
    for (int i = 0; i < T; i++) A*B;
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << "* = " << duration << std::endl;

}

void test_pow(){

    std::string str = "11111111111111111111111111111111411111111111111111111111111111111";

    big_number mod(str);

    big_number x(5, FillTypeRandom), z(1,FillTypeZero);


    z = x.pow(str, mod);


    if ((x%mod) == (z)){

        cout << "OK" << endl;
    }
    else{
        x.printHex();

        mod.printHex();
        z.printHex();
    }
    return;
}
