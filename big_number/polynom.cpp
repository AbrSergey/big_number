#include "polynom.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include "assert.h"

using namespace std;

polynom::polynom() : m_data (NULL), m_len(0), m_capacity(0){}

polynom::polynom(string str)
{
    int len = str.length();
    int lenBase = len / (sizeof(Base) * 8);
    int remainder = len % (sizeof(Base) * 8);

    for (int i = 0; i < len; i++) assert(str[i] == '0' || str[i] == '1');

    if (remainder == 0){

        m_capacity = lenBase;
        m_len = len;
        m_data = new Base[m_capacity];

        for (int i = 0; i < lenBase; i++)
            for (unsigned int j = 0; j < sizeof(Base)*8; j++){

                int b;

                int tmp = len - i*sizeof(Base)*8 - j - 1;

                if (str[tmp] == '1') b = 1;
                else b = 0;

                m_data[i] |= (b << j);
            }
    }
    else{
        lenBase++;
        m_capacity = lenBase;
        m_len = len;
        m_data = new Base[m_capacity];

        for (int i = 0; i < lenBase - 1; i++)
            for (unsigned int j = 0; j < sizeof(Base)*8; j++){

                int b;

                int tmp = len - i*sizeof(Base)*8 - j - 1;

                if (str[tmp] == '1') b = 1;
                else b = 0;

                m_data[i] |= (b << j);
            }

        for (int j = 0; j < remainder; j++){

            int b;
            if (str[remainder - j - 1] == '1') b = 1;
            else b = 0;

            m_data[m_capacity - 1] |= (b << j);
        }
    }
}

polynom::polynom(int quantityPower, FillTypePolynom filltype)
{
    assert (quantityPower >= 0);

    quantityPower++; // like len

    int lenBase = quantityPower / (sizeof(Base) * 8);
    int remainder = quantityPower % (sizeof(Base) * 8);

    switch (filltype){

        case FillTypePolynomZero:{

            if (remainder != 0) lenBase++;

            m_capacity = lenBase;
            m_len = 0;
            m_data = new Base[lenBase];

            for (int i = 0; i < lenBase; i++) m_data[i] = 0;

            break;
        }

        case FillTypeRandomPolynomPower:{

            if (remainder == 0){

                m_capacity = lenBase;
                m_len = quantityPower;
                m_data = new Base[m_capacity];

                for (int i = 0; i < m_capacity; i++) m_data[i] = rand() - rand();

                m_data[m_capacity - 1] |= (1 << (sizeof(Base) * 8 - 1));
            }
            else{

                lenBase++;

                m_capacity = lenBase;

                m_len = quantityPower;

                m_data = new Base[m_capacity];

                for (int i = 0; i < m_capacity; i++) m_data[i] = rand() - rand();

                int diff = sizeof(Base) * 8 - remainder;

                m_data[m_capacity - 1] = m_data[m_capacity - 1] << diff;

                m_data[m_capacity - 1] = m_data[m_capacity - 1] >> diff;

                m_data[m_capacity - 1] |= (1 << (remainder - 1));
            }

            break;
        }

        default: assert(!"Invalid FillType");
    }
}

polynom polynom::operator +(const polynom &inputPolynom) const
{

    int minLen = min(m_capacity, inputPolynom.m_capacity);
    int maxLen = max(m_capacity, inputPolynom.m_capacity);

    polynom result;
    result.m_capacity = maxLen;
    result.m_data = new Base[m_capacity];

    for (int i = 0; i < minLen; i++)
        result.m_data[i] = m_data[i] ^ inputPolynom.m_data[i];

    if (m_len > result.m_len)
        for (int i = minLen; i < maxLen; i++) result.m_data[i] = m_data[i];
    else
        for (int i = minLen; i < maxLen; i++) result.m_data[i] = inputPolynom.m_data[i];

    result.checkLength();

    return result;
}

polynom polynom::operator *(const polynom &inputPolynom) const
{
    int lenRes = m_len + inputPolynom.m_len;
    int capRes = lenRes / (sizeof(Base) * 8);
    if (m_len % (sizeof(Base) * 8) != 0) capRes++;

    polynom result;
    result.m_capacity = capRes;
    result.m_data = new Base[capRes];
    result.m_len = lenRes;

    for (int i = 0; i < m_len; i++){
        if (this->coefficient(i) == 1)
            for (int j = 0; j < inputPolynom.m_len; j++)
                if (inputPolynom.coefficient(j) == 1){

                    int whole = (i + j) / (sizeof(Base) * 8);
                    int remainder = (i + j) % (sizeof(Base) * 8);
                    int b = 1 << remainder;

                    result.m_data[whole] ^= b;
                }
    }

    result.checkLength();

    return result;
}

polynom &polynom::operator =(const polynom &inputPolynom)
{
    m_len = inputPolynom.m_len;

    int capacity = m_len / (sizeof(Base) * 8);

    if ((m_len % (sizeof(Base) * 8)) != 0) capacity++;

    if (m_capacity < capacity){

        delete [] m_data;

        m_data = new Base [ capacity ];

        m_capacity = capacity;
    }

    for (int i = 0; i < capacity; i++)

        m_data[i] = inputPolynom.m_data[i];

    return *this;
}

void polynom::print()
{
    int lenBase = m_len / (sizeof(Base) * 8); // in bytes
    int lenRemainder = m_len % (sizeof(Base) * 8); // in bits

    for (int i = lenRemainder - 1; i >=0; i--){

        int b = 1 << i;
        bool res = b & m_data[m_capacity - 1];

        if (res == 1) cout << "1 ";
        else cout << "0 ";
    }

    for (int i = lenBase - 1; i >= 0; i--){
        for (int j = sizeof(Base) * 8 - 1; j >= 0; j--){

            int b = 1 << j;
            bool res = b & m_data[i];

            if (res == 1) cout << "1 ";
            else cout << "0 ";
        }
    }
    cout << endl;
}

int polynom::coefficient(int number) const
{
    assert(number >= 0 && number < m_len);

    int whole = number / (sizeof(Base) * 8);
    int remainder = number % (sizeof(Base) * 8);

    int b;
    if (remainder == 0) b = 1;
    else b = 1 << remainder;

    b &= m_data[whole];
    b >>= remainder;

    if (b == 1) return 1;
    else return 0;
}

void polynom::checkLength()
{
    int i = m_capacity - 1;

    while ((m_data[i] == 0) && i > 0) {
        i--;
    }

    m_len = (i + 1) * sizeof(Base) * 8;

    if ((m_data[0] == 0) && (m_len == 1)){
        m_len = 0;
        return;
    }
    else{
        unsigned int k = 1 << (sizeof(Base) * 8 - 1);

        while ((m_data[i] & k) == 0 && k >= 1){
            m_len--;
            k >>= 1;
        }
    }

    return;
}
