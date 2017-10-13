﻿#include "big_number.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include "assert.h"
#include <limits>

using namespace std;

typedef unsigned int Base;

typedef unsigned short int HalfBase;

typedef unsigned long long DoubleBase;

int charToHex( char x );

int rev(int i, int k);

Base mulBase( Base a, Base b, Base * majorRes);

big_number::big_number() : m_data (NULL), m_len(0), m_capacity(0){}

big_number::big_number(std::string str)
{

    if (str.empty()){

        m_len = 1;
        m_capacity = 1;
        m_data = new Base[1];

        return;
    }

    if ( str.length() > 1 && str[1] == 'x'){

        m_capacity = 0;

        int numberElements = str.length();

        int full = (numberElements - 2) / NUM_HEX_DIGITS_IN_BASE;

        int residue = (numberElements - 2) % NUM_HEX_DIGITS_IN_BASE;

        if (residue > 0)

            m_capacity = full + 1;

        else
            m_capacity = full;

        m_data = new Base[m_capacity];

        m_len = m_capacity;

        for (int i = 0; i < m_capacity; i++) m_data[i] = 0;

        for (int i = numberElements, base_num = 0; base_num <= full; ++base_num, i -= NUM_HEX_DIGITS_IN_BASE){

            for (int j = 1; j <= NUM_HEX_DIGITS_IN_BASE; ++j){

                if ( i - NUM_HEX_DIGITS_IN_BASE - 1 + j >= 2){

                    unsigned char x = charToHex( str[i - NUM_HEX_DIGITS_IN_BASE - 1 + j] );

                    m_data[base_num] |= x;

                    if (j < NUM_HEX_DIGITS_IN_BASE)

                        m_data[base_num] <<= 4;
                }
            }
        }
    }
    else {

        big_number result(1);

        Base ten_9 = 1;

        Base buf = 0;

        for (int i = 0; i < str.length(); i++){

            if (i % 9 == 0){

                big_number tmp(1);

                tmp.m_data[0] = buf;
                tmp.m_len = 1;

                result = result * ten_9 + tmp;

                ten_9 = 1;

                buf = 0;
            }

            ten_9 *= 10;

            int digit = str[i] - '0';

            buf *= 10;

            buf += digit;
        }

        big_number tmp(1);

        tmp.m_data[0] = buf;
        tmp.m_len = 1;

        big_number BN(1);

        BN = result * Base(ten_9);

        result = BN + tmp;

        m_capacity = result.m_capacity;

        m_data = new Base[m_capacity];

        result.checkLength();

        *this = tmp;
    }
}

big_number::big_number(const big_number &number)
{
    m_capacity = number.m_capacity;

    m_len = number.m_len;

    m_data = new Base[m_capacity];

    for (int i = 0; i < m_len; i++ )

        m_data[i] = number.m_data[i];
}

big_number::big_number(unsigned int quantity, FillType filltype) : m_len (quantity), m_capacity (quantity)
{ 
    if (quantity <= 0){

        m_len = m_capacity = 1;

        m_data = new Base[m_capacity];

        m_data[0] = 0;

        return  ;
    }
    switch (filltype){

    case FillTypeRandomBits:

        int len = quantity / (sizeof(Base) * 8);

        m_data = new Base[len];

        for (int i = len - 1; i >= 0; --i) m_data[i] = rand() - rand();

        if (quantity > (len * sizeof(Base) * 8)){

            m_capacity = m_len = len + 1;

            int diff = quantity - len * sizeof(Base) * 8;

            m_data[len] = rand() - rand();

            m_data[len] = m_data[len] << (sizeof(Base) * 8 - diff);

            m_data[len] = m_data[len] | 2147483648;

            m_data[len] = m_data[len] >> (sizeof(Base) * 8 - diff);
        }
        else{

            m_capacity = m_len = len;

            int diff = len * sizeof(Base) * 8 - quantity;

            m_data[len - 1] = m_data[len - 1] << diff;

            m_data[len - 1] = m_data[len - 1] | 2147483648;

            m_data[len - 1] = m_data[len - 1] >> diff;
        }
        m_data[0] = m_data[0] | 1;

        break;
    }

    if (filltype != FillTypeRandomBits) m_data = new Base[quantity];

    for ( int i = 0; i < m_capacity; ++i){

        switch(filltype){

        case FillTypeZero:

//            m_data[m_len - 1] = 56;

            m_data[i] = 0;

            break;

        case FillTypeRandom:

              m_data[i] = rand();

//            m_data[0] = 0;
//            m_data[1] = 0;

//            m_data[2] = 384;

            break;

        case FillTypeRandomBits:

            break;

        default:

            assert(!"Invalid FillType");
        }
    }

    this->checkLength();
}

void big_number::printHex() const
{
    bool fl = false;

    std::cout << "0x";

    for (int i = m_len - 1; i >= 0 ; --i)

        for (int j = 0; j <= NUM_HEX_DIGITS_IN_BASE - 1; ++j){

            unsigned int x = m_data[i];

            x <<= (j*4);

            x >>= (NUM_HEX_DIGITS_IN_BASE - 1) * 4;

            if (x <= 9){

                x += '0';

                if (!((i == m_len - 1) && x == '0')){

                    std::cout << char(x);

                    fl = true;
                }
                else

                    if (fl) std::cout << char(x);
            }
            else{
                x += 'A' - 10;

                std::cout << char(x);

                fl = true;
            }
        }

    std::cout << std:: endl;
}

void big_number::printDbg() const
{
    printf("\n m_len = %d \n m_capacity = %d\n ", m_len, m_capacity);

    for (int i = 0; i < m_len; ++i)

        std::cout << "m_data[" << i << "] = " << m_data[i] << std::endl << " ";

    std::cout << std::endl;
}

big_number big_number::operator +(const big_number & input_number) const
{
    int max, min, m_len_or_input_number;

    if (m_len > input_number.m_len){

        m_len_or_input_number = 0;
        max = m_len;
        min = input_number.m_len;
    }

    else{

        m_len_or_input_number = 1;
        max = input_number.m_len;
        min = m_len;
        }

    big_number rezult;

    rezult.allocate_and_fill_zeroes(max + 1);

    int regist = 0;

    for (int i = 0; i < min; ++i){

        rezult.m_data[i] = m_data[i] + input_number.m_data[i] + regist;

        regist = 0;

        if (rezult.m_data[i] < m_data[i]) regist = 1;
    }

    for (int i = min; i < max; ++i){

        if (m_len_or_input_number == 0){

            rezult.m_data[i] = m_data[i] + regist;

            regist = 0;

            if (rezult.m_data[i] < m_data[i]) regist = 1;
        }
        else{

            rezult.m_data[i] = input_number.m_data[i] + regist;

            regist = 0;

            if (rezult.m_data[i] < input_number.m_data[i]) regist = 1;
        }
    }

    if (regist == 1)
        rezult.m_data[max] = 1;

    rezult.checkLength();

    return rezult;
}

big_number big_number::operator -(const big_number &input_number) const
{
    int max, min;

    if (m_len > input_number.m_len){

        max = m_len;
        min = input_number.m_len;
    }

    else{

        max = input_number.m_len;
        min = m_len;
        }

    big_number rezult(max);

    rezult.m_len = rezult.m_capacity;

    int regist = 0;

    for (int i = 0; i < min; ++i){

        rezult.m_data[i] = m_data[i] - input_number.m_data[i] - regist;

        regist = 0;

        if (rezult.m_data[i] > m_data[i]) regist = 1;
    }
    for (int i = min; i < max; ++i){

            rezult.m_data[i] = m_data[i] - regist;

            regist = 0;

            if (rezult.m_data[i] > m_data[i]) regist = 1;
    }

    rezult.checkLength();

    return rezult;
}

big_number big_number::operator *(const big_number &input_number) const
{

    if (*this == ZERO || input_number == ZERO){

        return ZERO;
    }

    int length = m_len + input_number.m_len;

    big_number result(length);

    result.m_capacity = length;

    result.m_len = length;

    Base residual;

    for (int i = 0; i < input_number.m_len; ++i)

        for (int j = 0; j < m_len; ++j){

            Base tmpResult;

            residual = mulBase(m_data[j], input_number.m_data[i], &tmpResult);

            result.m_data[ i + j ] += tmpResult;

            char overflow = 0;

            if (result.m_data[i + j] < tmpResult) overflow = 1;

            result.m_data[i + j + 1] += residual + overflow;

            overflow = result.m_data[i + j + 1] < residual ? 1 : 0;


            for (int k = 2; i + j + k < length && overflow == 1; ++k){

                result.m_data[i + j + k] += overflow;

                overflow = result.m_data[i + j + k ] ? 0 : 1;
            }
        }

    result.checkLength();

    return result;
}

big_number big_number::operator *(DoubleBase input_number) const
{
    big_number input_big_number(2);

    Base * pInput_number = reinterpret_cast < Base * > (&input_number);

    input_big_number.m_data[0] = pInput_number[0];

    input_big_number.m_data[1] = pInput_number[1];

    input_big_number.m_len = 2;

    //result

    big_number result(this->m_len + 2);

    result = *this * input_big_number;

    result.checkLength();

    return result;
}

big_number big_number::operator *(Base input_number) const
{
    big_number input_big_number(1);

    input_big_number.m_data[0] = input_number;

    input_big_number.m_len = 1;

    //result

    big_number result(this->m_len + 1);

    result = *this * input_big_number;

    result.checkLength();

    return result;

}

big_number big_number::operator ^(const int power) const
{
    big_number result = *this;

    for (int i = 1; i < power; i++) result = result * (*this);

    return result;

}

big_number big_number::operator /(const big_number & input_number) const
{
    //make q and r

    big_number q, r;

    division (input_number, q, r);

    q.checkLength();

    return q;
}

big_number big_number::operator %(const big_number &input_number) const
{
    //make q and r

    big_number q, r;

    division(input_number, q, r);

    r.checkLength();

    return r;
}

big_number &big_number::operator =(const big_number &input_number)
{
    m_len = input_number.m_len;

    if ( m_capacity < m_len){

        delete [] m_data;

        m_data = new Base[ m_len ];

        m_capacity = m_len;
    }

    for (int i = 0; i < m_len; ++i)

        m_data[i] = input_number.m_data[i];

    return *this;
}

big_number &big_number::operator <<=(const int m)
{
    if (this->m_capacity - this->m_len < m){

        big_number help(this->m_len + m);

        help = *this;

        for (int i = help.m_len + m - 1; i >= m; i--)

            help.m_data[i] = help.m_data[i - m];

        for (int i = 0; i < m; i++ )

            help.m_data[i] = 0;

        help.m_len = this->m_len + m;

        *this = help;

        return *this;
    }

    for (int i = m_len + m - 1; i >= m; i--)

        m_data[i] = m_data[i - m];

    for (int i = 0; i < m; i++ )

        m_data[i] = 0;

    m_len += m;

    return *this;
}

big_number big_number::operator <<(const int m) const
{
    big_number result = *this;

    if (result.m_capacity - result.m_len < m){

        big_number help(result.m_len + m);

        help = result;

        for (int i = help.m_len + m - 1; i >= m; i--)

            help.m_data[i] = help.m_data[i - m];

        for (int i = 0; i < m; i++ )

            help.m_data[i] = 0;

        help.m_len = result.m_len + m;

        result = help;

        return result;
    }

    for (int i = result.m_len + m - 1; i >= m; i--)

        result.m_data[i] = result.m_data[i - m];

    for (int i = 0; i < m; i++ )

        result.m_data[i] = 0;

    result.m_len += m;

    return result;
}

big_number big_number::operator >>(const int m) const
{
    if (m >= m_len){
        big_number result(1);
        result.m_len = 1;
        result.m_data[0] = 0;
        return result;
    }

    big_number result = *this;

    for (int i = 0; i < result.m_len - m; i++)
        result.m_data[i] = result.m_data[i+m];

    result.m_len -= m;

    return result;
}

bool big_number::operator >(const big_number &input_number) const
{
    if (this->m_len > input_number.m_len) return true;

    if (this->m_len < input_number.m_len) return false;

    if (this->m_len == input_number.m_len)

        for (int i = this->m_len - 1; i >= 0; i--){

            if (this->m_data[i] > input_number.m_data[i]) return true;

            if (this->m_data[i] < input_number.m_data[i]) return false;
        }

    return true;
}

bool big_number::operator <(const unsigned int a) const
{
    if ((m_len == 1) && m_data[0] < a) return true;
    return false;
}

bool big_number::operator ==(const big_number &input_number) const
{
    if (this->m_len != input_number.m_len) return false;

    for (int i = this->m_len - 1; i >= 0; i--)

        if (this->m_data[i] != input_number.m_data[i]) return false;

    return true;
}

bool big_number::operator >=(const big_number &input_number) const
{
    if (this->m_len > input_number.m_len) return true;

    if (this->m_len < input_number.m_len) return false;

    if (this->m_len == input_number.m_len)

        for (int i = this->m_len - 1; i >= 0; i--){

            if (this->m_data[i] > input_number.m_data[i]) return true;

            if (this->m_data[i] < input_number.m_data[i]) return false;
        }

    return true;
}

big_number big_number::pow(const big_number &y, const big_number &mod)
{
    // declaration
    big_number z(mod.m_len, FillTypeZero), q = *this;

    // step 1
    if ((y.m_data[0] & 1) == 0) z.m_data[0] = 1;

    else z = *this;

    // step 2
    for (int j = 1; j < 8*sizeof(Base); j++){

        q = (q * q);
        q = q % mod;
        if ((y.m_data[0] >> j) & 1 == 1) z = (z * q) % mod;
    }

    for (int i = 1; i < m_len; i++)
        for (int j = 0; j < 8*sizeof(Base); j++){

            q = q * q % mod;
            if ((y.m_data[i] >> j) & 1 == 1) z = (z * q) % mod;
        }
    return z;
}

big_number big_number::Kar(const big_number &v) const
{
    int l = min(m_len, v.m_len), i;

    if (l <= KAR_BASES){

        big_number A;
        A = (*this) * v;
        return A;
    }

    int baseNum = max(m_len, v.m_len);

    baseNum >>= 1;

    baseNum = max(m_len, v.m_len) - baseNum;

    int u_size = baseNum, v_size = baseNum;

    if (baseNum > m_len)  u_size = m_len;

    if (baseNum > v.m_len) v_size = v.m_len;

    big_number u0(u_size), v0(v_size);

    u0.m_len = u_size;
    for (l = 0; l < u_size; l++) u0.m_data[l] = m_data[l];

    big_number u1(m_len - l);
    for (i = l; l < m_len; l++) u1.m_data[l - i] = m_data[l];
    u1.m_len = l - i;

    v0.m_len = v_size;
    for (l = 0; l < v_size; l++) v0.m_data[l] = v.m_data[l];

    big_number v1(v.m_len - l);
    for (i = l; l < v.m_len; l++) v1.m_data[l - i] = v.m_data[l];
    v1.m_len = l - i;

    big_number A, B;

    A = u1.Kar(v1);

    B = u0.Kar(v0);

    return ((A << 2*baseNum) + (((u1 + u0).Kar(v1 + v0) - A - B) << baseNum) + B);
}

big_number big_number::Bar(const big_number & m, big_number &z) const
{
//    std::cout << "m.m_len = " << m.m_len << endl;
//    std::cout << "z.m_len = " << z.m_len << endl;

    big_number b(2);
    b.m_data[0] = 0;
    b.m_data[1] = 1;
    b.m_len = 2;

    big_number r1(m.m_len + 1);

    for (int i = 0; i < m.m_len + 1; i++) r1.m_data[i] = m_data[i];
    r1.m_len = m.m_len + 1;

    big_number r2 = (((*this >> (m.m_len - 1)) * z) >> (m.m_len + 1))*m;
    r2.m_len = m.m_len + 1;

    if (r1 >= r2) r1 = r1 - r2;
    else r1 = (b << (m.m_len + 1)) + r1 - r2;

    while (r1 >= m){
        r1 = r1 - m;
//        std::cout << "*";
    }
//    std::cout << std::endl;

    return r1;
}

int big_number::function2(big_number &r) const
{
    int s = 0, i = 0;

    while (m_data[i] == 0 && i < m_len){

        s += sizeof(Base) * 8;

        ++i;
    }

    int a = 1;

    while ((m_data[i] & a) == 0 && a < 2147483648){

        s++;
        a = a << 1;
    }

    r = *this;

    int ss = s;

    r.m_len -= i;

    for (int l = 0; l < i; ++l){

        ss -= (8*sizeof(Base));

        r.m_data[l] = r.m_data[l + i];
    }

    Base mask = 1, cover = 0;

    mask = (1 << (ss-1)) - 1;

    for (i = 0; i < r.m_len - 1; ++i){

        r.m_data[i] >>= ss;

        cover = r.m_data[i + 1] & mask;

        cover <<= (sizeof(Base) - ss);

        r.m_data[i] |= cover;
    }
    r.m_data[i] >>= ss;

    return s;
}

void big_number::checkLength()
{
    for (;m_len > 1 && m_data[m_len - 1] == 0; m_len --){}
}

void big_number::division(const big_number &input_number, big_number &q, big_number &r) const
{
    // copy *this to u_number and input_number to v_number

    if (input_number == ZERO)
        throw std::logic_error("Division by zero");

    if (*this == ZERO){

        r = ZERO;

        q = ZERO;

        return;
    }

    if (m_len < input_number.m_len){

        r = *this;

        q = ZERO;

        return;
    }

    big_number u(m_len + 1);

    u = *this;

    int l = u.m_len;

    if (input_number.m_len == 1){

        r = big_number(1);

        u.div_base(input_number.m_data[0], r.m_data[0]);

        r.checkLength();

        q = u;

        return;
    }


    big_number v = input_number;

    q.allocate_and_fill_zeroes(m_len - input_number.m_len + 2);

    // D1 normalization

    DoubleBase b = 1;

    b <<= (sizeof(Base) * 8);

    Base d = b / (v.m_data[v.m_len - 1] + 1);

    u = u * d;

    v = v * d;

    //preliminary calculation

    if (l == u.m_len) u.m_len ++;

    int m = u.m_len - v.m_len - 1;

    int n = v.m_len;

    //D2 initial setting

    for (int j = m; j >= 0; --j){

        //D3 compute q_cap and r_cap

        DoubleBase q_cap, r_cap;

        q_cap = (u.m_data[j + n] * b + u.m_data[j + n - 1]) / v.m_data[n - 1];

        r_cap = (u.m_data[j + n] * b + u.m_data[j + n - 1]) % v.m_data[n - 1];

        if (q_cap == b || (q_cap * v.m_data[n - 2]) > (b * r_cap + u.m_data[j + n - 2])){

            q_cap -= 1;

            r_cap += v.m_data[n - 1];

            if (r_cap < b){

                if (q_cap == b || (q_cap * v.m_data[n - 2]) > (b * r_cap + u.m_data[j + n - 2])){

                    q_cap -= 1;

                    r_cap += v.m_data[n - 1];
                }
            }
        }

        //D4 multiplyd and subtract

        big_number mediate = (v * q_cap);

        bool interim = u.shift_sub(mediate, j);

        //D5

        q.m_data[j] = q_cap;
        //q.m_len ++;

        //D6

        if(interim){

            q.m_data[j] -= 1;

            u.shift_add(v, j);
        }

        //D7
    }

    //D8

    Base remainder = 0;

    u.div_base(d, remainder);

    r = u;

    r.checkLength();

    q.checkLength();
}

void big_number::div_base(Base d, Base &remainder)
{
    if (d == 0)
        throw std::logic_error("Division by zero");

    DoubleBase dividing = 0;

    for(int i = m_len-1; i >= 0; i--)
    {
        dividing <<= (sizeof(Base) * 8 );

        dividing += m_data[i];

        Base q_r = (dividing / d);

        m_data[i] = q_r;

        dividing = dividing % d;
    }

    assert (dividing <= std::numeric_limits <Base>::max());

    remainder = Base(dividing);

    checkLength();
}

bool big_number::shift_sub(const big_number &input_number, int j)
{
    int check_negative = 0;

    Base interim = 0;

    //shift start
    for (int i = 0; i < input_number.m_len; i++){

        interim = m_data[i + j] - input_number.m_data[i] - check_negative;

        if (interim > m_data[i + j])

            check_negative = 1;

        else
            check_negative = 0;

        m_data[i + j] = interim;
    }

    for(int i = input_number.m_len + j; i < m_len && check_negative == 1; i++){

        interim = m_data[i] - check_negative;

        if (interim > m_data[i])

            check_negative = 1;
        else
            check_negative = 0;

        m_data[i] = interim;
    }

    checkLength();

    if (check_negative == 1) return true;

    else return false;
}

void big_number::shift_add(const big_number & input_number, int j)
{
    int overflow = 0;

    for(int i = 0 ; i < input_number.m_len; i++){

        m_data[i + j] += input_number.m_data[i] + overflow;

        if(m_data[i + j] < input_number.m_data[i]) overflow = 1;

        else
            overflow = 0;
    }


    for (int i = input_number.m_len + j; i < m_len && overflow; i++){

        m_data[i] += overflow;

        if (m_data[i] < overflow) overflow = 1;
        else
            overflow = 0;
    }
}

void big_number::allocate_and_fill_zeroes(int len)
{
    m_capacity = len;

    delete m_data;

    m_data = new Base[m_capacity];

    m_len = m_capacity;

    memset(m_data, 0 , m_len * sizeof(Base));
}

int big_number::len() const
{
    return m_len;
}

big_number big_number::zComp()
{
    big_number z(2*m_len + 1);

    z.m_data[2*m_len] = 1;

    z.m_len = 2*m_len + 1;

    return z/(*this);
}

unsigned int big_number::zeroCount()
{
    unsigned int zero = 0, i;

    for (i = 0; m_data[i] == 0; zero += sizeof(Base)*8, i++){}

    unsigned int x = 1;

    while (m_data[i] &= x){

        zero++;
        x <<= 1;
    }
    return zero;
}

bool big_number::testMillerRabin(int t)
{
    big_number one("0x1"), r(m_len), two("0x2");

    big_number max = (*this) - one;

    unsigned int s = max.function2(r);

    for (int i = 0; i < t; i++){

        big_number b(m_len, FillTypeRandomBits);

        b = two + b%(max - two);

        big_number y = b.pow(r, (*this));

        if ((!(y == one)) && (!(y == max))){

            for (unsigned int j = 1; (j < s) && !(y == max); y = (y * y) % (*this), j++)
                if (y == one) return 0;

            if (!(y == max)) return 0;
        }
    }
    return 1;
}

big_number *big_number::fft(const int &n, const int &k, big_number &w, const big_number *a)
{
    big_number *R = new big_number[n];

    big_number *S = new big_number[n];

    for (int i = 0; i < n; i++) R[i] = a[i];

    for (int l = k - 1; l >= 0; l--){

        for (int i = 0; i < n; i++) S[i] = R[i];

        int mask = 1 << l;

        for (int i = 0; i < n; i++){

            int i_1 = i & (~mask);
            int i_2 = i | mask;

            big_number e(1);
            e.m_data[0] = rev(i >> l, k);

            big_number p = w.pow(e, (*this));

            R[i] = (S[i_1] + p * S[i_2]) % (*this);
        }
    }

    for (int i = 0; i < n; i++) S[i] = R[rev(i, k)];

    return S;
}

void big_number::testfft()
{
    int n = 8;

    big_number *a = new big_number[n];

    for (int i = 0; i < n; i++){
        a[i].m_data = new Base[1];
        a[i].m_capacity = a[i].m_len = 1;
    }

    a[0].m_data[0] = 8;
    a[1].m_data[0] = 5;
    a[2].m_data[0] = 9;
    a[3].m_data[0] = 0;
    a[4].m_data[0] = 3;
    a[5].m_data[0] = 3;
    a[6].m_data[0] = 9;
    a[7].m_data[0] = 5;

    big_number *b, w(1), m(1);
    w.m_data[0] = 2; w.m_len = 1;
    m.m_data[0] = 17; m.m_len = 1;

    b = m.fft(8, 3, w, a);

    for (int i = 0; i < n; i++) b[i].printHex();
}

outTDM *big_number::testDivisorMethod(const big_number &n)
{
    int k = 1, t = 0;

    big_number one ("1");

    if (n == one){

        outTDM result;

        result.prime_number = &one; //??

        result.power = 1;

        return &result; //??
    }

    big_number d(1);

    d.m_data[0] = 2;

    d.m_len = 1;

    big_number q, r;

    q = n / d;

    r = n % d;

//    if (r == 0) {

//        t++;


//    }
}

int charToHex( char x ){

    if (x >= '0' && x <= '9') return x - '0';
    
    else return x - 'A' + 10;
}

Base mulBase( Base a, Base b, Base * minorRes){

     HalfBase * pA = reinterpret_cast < HalfBase * > (&a);

     HalfBase * pB = reinterpret_cast < HalfBase * > (&b);

     Base result[2] = {0, 0};

     HalfBase * pResult = reinterpret_cast < HalfBase * > (result);

     for (int i = 0; i < 2; ++i)

         for (int j = 0; j < 2; ++j){

             Base tmpResult = pA[i] * pB[j];

             HalfBase * pTmpResult = reinterpret_cast < HalfBase * > (&tmpResult);

             pResult[ i + j ] += pTmpResult[0];

             char overflow = 0;

             if (pResult[i + j] < pTmpResult[0]) overflow = 1;

             pResult[i + j + 1] += pTmpResult[1] + overflow;

             overflow = pResult[i + j + 1] < pTmpResult[1] ? 1 : 0;


             for (int k = 2; i + j + k < 4 && overflow == 1; ++k){

                 pResult[i + j + k] += overflow;

                 overflow = pResult[i + j + k ] ? 0 : 1;
             }
         }

     *minorRes  = result[0];

     return result[1];
}

int rev(int i, int k)
{
    int n = i, j = k, res = 0;
    while(j){
        res <<= 1;
        res += n & 1;
        n >>= 1;
        j--;
    }
    return res;
}
