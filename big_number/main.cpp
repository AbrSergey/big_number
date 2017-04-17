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

int main(){

   // big_number a(2, FillTypeRandom), b(2, FillTypeRandom), c(8);

    test_Kar_2();

    return 0;
}

void test_Kar_1(){

    int T = 100, M = 1000, m, n;

    for (int i = 0; i < T; i++){

        m = 1 + rand()%M;
        n = 1 + rand()%M;

        big_number A(m, FillTypeRandom);
        big_number B(n, FillTypeRandom);
        big_number C, D;

        C = A.Kar(B);
        D = A*B;

        if (C == D) std::cout << "True";

        else std::cout << "False";

        std::cout << endl;
    }
}

void test_Kar_2(){

    int T = 50, M = 1000, m, n;

    big_number A(M, FillTypeRandom);
    big_number B(M, FillTypeRandom);
    big_number C;

    time_t t1 = time(NULL);
    for (int i = 0; i < T; i++) A.Kar(B);
    time_t t2 = time(NULL);

    std::cout << "Kar = " << t2-t1 << std::endl;

    t1 = time(NULL), t2;
    for (int i = 0; i < T; i++) A*B;
    t2 = time(NULL);

    std::cout << "* = " << t2-t1 << std::endl;

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



/*
bool inspection (const big_number & a,const big_number & b,const big_number & c);

void print_error(const big_number & a,const big_number & b,const big_number & c,const big_number & d, const big_number  e = ZERO);

int main(){

    string test_root_dir = "/home/sergey/tests";

    string test_name = "bigint_div_tests";

    char test_num_str [4];

    for(int i = 1; i <= 70; i++){

        sprintf(test_num_str,"%03d",i);

        string datname =  test_root_dir + "/" + test_name + "/" + test_num_str + ".dat";

        ifstream in_dat_file(datname.c_str());

        if (!in_dat_file)
            cout << "cant open " << datname << endl;

        string num_1_str, num_2_str;

        in_dat_file >> num_1_str;

        in_dat_file >> num_2_str;

        big_number a (num_1_str);

        big_number b (num_2_str);

        string ansname =  test_root_dir + "/" + test_name + "/" + test_num_str + ".ans";

        ifstream in_ans_file(ansname);

        if (!in_ans_file)
            cout << "cant open " << datname << endl;


        string ans_str;

        in_ans_file >> ans_str;

        big_number c (ans_str);

        if(a / b == c)
            printf("TEST %d PASSED\n", i);
        else
        {
            printf("TEST %d FAILED\n", i);

            a.printHex();
            b.printHex();
            (a/b).printDbg();
            c.printDbg();
            break;
        }
    }
}

*/
/*
    srand(time(0));

    const int NUM_TESTS = 10;

    const int NUM_BASES = 8000;

    for (int i = 0; i < NUM_TESTS; i++){

        cout << endl << " Test № " << i << " ";

        big_number a(rand() % NUM_BASES + 1, FillTypeRandom);

        big_number b(rand() % NUM_BASES + 1, FillTypeRandom);

        big_number c(rand() % NUM_BASES + 1, FillTypeRandom );

        if (!inspection(a, b, c)) break;
    }
    return 0;

}

bool inspection (const big_number & a,const big_number & b,const big_number & c){

     big_number x, y, z, m, n, p, t;

     try
     {
         cout << endl << "Add: ";

         x = a + b;
         y = b + a;

         if ( x == y)
             cout << "OK";
         else {
             cout << "a + b = b + a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = a + b + c;
         y = c + b + a;

         if ( x == y)
             cout << "OK";
         else {
             cout << "a + b + c = c + b + a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = a + b + c;
         y = a + (b + c);

         if ( x == y)
             cout << "OK";
         else {
             cout << "a + b + c = a + (b + c)" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         cout << endl << "Sub: ";

         x = a + b - b;
         y = a;

         if ( x == y)
             cout << "OK";
         else {
             cout << "a + b - b =a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = a + b + c - b - c;
         y = a;

         if ( x == y)
             cout << "OK";
         else {
             cout << "a + b + c - b - c = a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = a + b + c - c;
         y = a + b;

         if ( x == y)
             cout << "OK";
         else {
             cout << "aa + b + c - c = a + b" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         cout << endl << "Mul: " ;

         x = a*b*c;
         y = c*b*a;

         if ( x == y)
             cout << "OK";
         else {
             cout << "a*b*c =c*b*a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = a*b*c;
         y = a*(b*c);

         if ( x == y)
             cout << "OK";
         else {
             cout << "a*b*c =a*(b*c)" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = ( a + b )*c;
         y = a*c + b*c;

         if ( x == y)
             cout << "OK";
         else {
             cout << "( a + b )*c =a*c + b*c" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = ( a + b )*( a + b );
         y = a*a + a*b + a*b + b*b;

         if ( x == y)
             cout << "OK";
         else {
             cout << "( a + b )*( a + b ) =a*a + a*b + a*b + b*b" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = ( a + b + c - b )*b;
         y = a*b + c*b;

         if ( x == y)
             cout << "OK";
         else {
             cout << "( a + b + c - b )*b =a*b + c*b" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         cout << endl << "div:";


         m = a / b;

         n  = m*b;

         p = a % b;

         t = n + p;

        // x = ( a / b )*b + a % b;
        // y = a;

         if ( t == a)
             cout << "OK";
         else {
             cout << "( a / b )*b + a % b =a" << endl;
             print_error(a,b,x,y,c);

             m.printHex();

             p.printHex();

             n.printHex();

             t.printHex();

             a.printHex();

             return false;
         }

         x = ( a / b )*b;
         y = a;

         if (y > x || y == x)
             cout << "OK";
         else {
              cout << "( a / b )*b=a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = ( a*b ) / b;

         y = a;

         if ( x == y)
             cout << "OK";
         else {
             cout << "( a*b ) / b = a" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = ( a*b*c ) / c;
         y = a*b;

         if ( x == y)
             cout << "OK";
         else {
             cout << "( a*b*c ) / c = a*b" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         x = ( a*b*c ) / ( a*b );

         y= c;

         if ( x == y)
             cout << "OK";
         else {
              cout << "( a*b*c ) / ( a*b ) = c" << endl;
             print_error(a,b,x,y,c);
             return false;
         }

         cout << endl;

     }
     catch(std::logic_error & e)
     {
         cout << e.what();
     }

     return true;
}

void print_error(const big_number & a,const big_number & b,const big_number & c,const big_number & d, const big_number  e){

    cout << "FAILED" << endl;

    a.printHex();

    b.printHex();

    e.printHex();

    c.printHex();

    d.printHex();

    c.printDbg();

    d.printDbg();
}
*/
