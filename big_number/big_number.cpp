#include "big_number.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include "assert.h"
#include <limits>

using namespace std;

typedef unsigned int Base;

typedef unsigned short int HalfBase;

typedef unsigned long long DoubleBase;

int charToHex( char x );

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

bool big_number::operator >(const big_number &input_number) const
{
    if (this->m_len > input_number.m_len) return true;

    if (this->m_len < input_number.m_len) return false;

    if (this->m_len == input_number.m_len)

        for (int i = this->m_len - 1; i >= 0; i--){

            if (this->m_data[i] > input_number.m_data[i]) return true;

            if (this->m_data[i] < input_number.m_data[i]) return false;

            if (i == 0) return false;
        }
}

bool big_number::operator ==(const big_number &input_number) const
{
    if (this->m_len != input_number.m_len) return false;

    for (int i = this->m_len - 1; i >= 0; i--)

        if (this->m_data[i] != input_number.m_data[i]) return false;

    return true;
}

big_number &big_number::pow(const big_number &y, const big_number &mod)
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

big_number &big_number::Kar(const big_number &v)
{
    int lu = m_len/2;

    int lv = v.m_len/2;

    int l;

    if (lu > lv) l = lu;
    else l = lv;

    big_number u1(m_len - l), v1(v.m_len - l), u0(l), v0(l);

    for (int i = 0; i < l; i++){

        u0.m_data[i] = m_data[i];

//        u0.m_capacity++;
//        u0.m_len++;
    }

    for (int i = l; i < m_len; i++){

        u1.m_data[0] = m_data[i];

//        u1.m_capacity++;
//        u1.m_len++;
    }

    for (int i = 0; i < l; i++){

        v0.m_data[0] = v.m_data[i];

//        v0.m_capacity++;
//        v0.m_len++;
    }

    for (int i = l; i < v.m_len; i++){

        v1.m_data[0] = v.m_data[i];

//        v1.m_capacity++;
//        v1.m_len++;
    }

    big_number A, B, C;

    A = u1*v1;

    B = u0*v0;

    C = (u1 + u0)*(v1 + v0);

    C = C - A - B;

    A <<= sizeof(Base)/2;

    C <<= sizeof(Base)/4;

    big_number rezult = A + C + B;

    return rezult;
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

