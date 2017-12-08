#include "polynom.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include "assert.h"

using namespace std;

polynom::polynom() : m_data (NULL), m_len(0), m_capacity(0){}

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

                unsigned int h = (1 << (sizeof(Base) * - 1));

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
