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
void factorizationMTD();
void factorizationSiftingMethodFerma();
void discreteLogarithm (unsigned int len = 3);

int main(){

//    srand(time(0));

    discreteLogarithm(24);

    return 0;
}

void discreteLogarithm(unsigned int len){

    for (int i = 0; i < 1; i++)
    {
        big_number m = primeNumberGenerator(len);

//        big_number m("0x11");

        // factorization m

        bool isFactorized;

        outTDM *result = new outTDM[30];

        int k = (m-1).testDivisorMethod((m - 1), result, isFactorized);

        // initialization

        big_number g = m.primitiveRoot(k, result);
//        big_number g("0x3");

        big_number a(len, FillTypeRandomBits), res;
//        big_number a("0x2"), res;

        a = a % m;
        if (a == 0) break;


        // start algorithm

        res = m.polygHellman(g, a, k, result);

        cout << "m = "; m.printHex();
        cout << "g = "; g.printHex();
        cout << "a = "; a.printHex();
        cout << "res = "; res.printHex();


        // testing

        big_number test = g.pow(res, m);

        if (test == a) cout << "True!" << endl;
        else cout << "Failed!" << endl;

        cout << "test = "; test.printHex();
        cout << endl;
    }
}

void factorizationSiftingMethodFerma(){

    // цикл i=0 до k - длина множителей и перемножить числа и дать алгоритму

    for (int i = 28; i < 29; i++){

        big_number x(i, FillTypeRandomBits);
        big_number y(i, FillTypeRandomBits);
        big_number z = x * y;

        big_number n = z;
        big_number a, b;
        n.siftingMethodFerma(n, a, b); // z = 0x25BEF x = 0x1BB y = 0x15D проверить.


        if (n == z){

            cout << "z = "; z.printHex();
            cout << "x = "; x.printHex();
            cout << "y = "; y.printHex();

            cout << "n = "; n.printHex();
            cout << "a = "; a.printHex();
            cout << "b = "; b.printHex();

            cout << endl;
        }
        else{
            cout << "False! i = " << i;
            break;
        }

    }

//    big_number n("0x25BEF");
//    n.printHex();

//    big_number a, b;
//    n.siftingMethodFerma(n, a, b);

//    a.printHex();
//    b.printHex();
}

void factorizationMTD(){

    big_number x("100");

    outTDM *result = new outTDM[30];

    int lenResult;

    bool isFactorized;

    lenResult = x.testDivisorMethod(x, result, isFactorized);

    for (int i = 0; i <= lenResult; i++){

        if (isFactorized) cout << "Factorized" << endl;
        else cout << "Not factorized" << endl;

        cout << "prime number " << i + 1 << " = ";

        result[i].prime_number.printHex();

        cout << "power = " << result[i].power << endl << endl << endl;
    }
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
