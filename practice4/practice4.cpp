#include <sys/types.h>
#include <stdio.h>
#include <cstdint>
//кодировка чисел 
int pack(int64_t* input, int nin, void* output, int n)
{
    int64_t inmask = 0;
    unsigned char* pout = (unsigned char*)output;
    int obit = 0;
    int nout = 0;
    *pout = 0;

    for (int i = 0; i < nin; i++)
    {
        inmask = (int64_t)1 << (n - 1);
        for (int k = 0; k < n; k++)
        {
            if (obit > 7)
            {
                obit = 0;
                pout++;
                *pout = 0;
            }
            //применение к указателю побитового ИЛИ
            *pout |= (((input[i] & inmask) >> (n - k - 1)) << (7 - obit));
            //умножение на 2
            inmask >>= 1;
            obit++;
            nout++;
        }
    }
    return nout;
}
//возвращение из закодированного состояния
int unpack(void* input, int nbitsin, int64_t* output, int n)
{
    unsigned char* pin = (unsigned char*)input;
    int64_t* pout = output;
    int nbits = nbitsin;
    unsigned char inmask = 0x80;
    int inbit = 0;
    int nout = 0;
    while (nbits > 0)
    {
        *pout = 0;
        for (int i = 0; i < n; i++)
        {
            if (inbit > 7)
            {
                pin++;
                inbit = 0;
            } 
            *pout |= ((int64_t)((*pin & (inmask >> inbit)) >> (7 - inbit))) << (n - i - 1);
            inbit++;
        }
        pout++;
        nbits -= n;
        nout++;
    }
    return nout;
}

int main()
{
    //int64_t - чтобы данные имели фиксированный размер
    int64_t input[] = { 0,1,2,3,4,5,6,7,8,9,10 };
    int64_t output[11];
    unsigned char compressed[11 * 8];
    int n = 4; //количество памяти 2**n

    int nbits = pack(input, 11, compressed, n);
    int nout = unpack(compressed, nbits, output, n);

    for (int i = 0; i <= 10; i++)
        printf("input: %lld   output: %lld\n", input[i], output[i]);
}