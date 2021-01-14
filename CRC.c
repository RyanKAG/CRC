#include <stdio.h>
#include <stdlib.h> 

#define length 256
unsigned long long int crcTable[length]; //this usually would be a constant

/*
* CalcCRCTable Calculates all the possible crc for all possible 8 bit combination
* This function is supposed to run once and use the result statically, but for the sake of showing my understanding of CRC 
* - it will run every time this code is ran
* to avoid this overhead we use the defind CRCTable at the top.
*/

void CalcCRCTable()
{
    unsigned long long int polynomial = 0x000000000000001BULL;
    unsigned long long int mask = 0x8000000000000000ULL;
    
    /* iterate over all possible input byte values 0 - 255 */
    for (unsigned long long int divident = 0; divident < length; divident++)
    {
        unsigned long long int currentByte = (divident << 56);/* move divident byte into MSB of 64Bit CRC */
        for (unsigned char bit = 0; bit < 8; bit++)
        {
            if ((currentByte & mask) != 0)
            {
                currentByte <<= 1;
                currentByte ^= polynomial;
            }
            else
            {
                currentByte <<= 1;
            }
        }
        crcTable[divident] = currentByte;
    }
}

// //unfortuantly this is not working correctly now
// unsigned char* appendCrc(char *msg, int len, unsigned long long int crc)
// {
//     //create a space for the msg and for CRC
//     char *appendedArray = malloc((len + 8) * sizeof(unsigned char));  
//     for (int i = 0; i < len - 8; i++)
//         appendedArray[i] = msg[i];

//     //append each crc byte to the array
//     appendedArray[len + 1] = (char)((crc & 0xFF00000000000000ULL) >> 56);
//     appendedArray[len + 2] = (char)((crc & 0x00FF000000000000ULL) >> 48);
//     appendedArray[len + 3] = (char)((crc & 0x0000FF0000000000ULL) >> 40);
//     appendedArray[len + 4] = (char)((crc & 0x000000FF00000000ULL) >> 36);
//     appendedArray[len + 5] = (char)((crc & 0x00000000FF000000ULL) >> 24);
//     appendedArray[len + 6] = (char)((crc & 0x0000000000FF0000ULL) >> 16);
//     appendedArray[len + 7] = (char)((crc & 0x000000000000FF00ULL) >> 8);
//     appendedArray[len + 7] = (char)(crc & 0x00000000000000FFULL);

//     return appendedArray;
// }


unsigned long long int crc_64(char *msg, int size)
{
    unsigned long long int crc = 0ULL;
    for (int i = 0; i < size; i++)
    {
        /* XOR-in next input byte into MSB of crc and get this MSB, that's our new intermediate divident */
        unsigned char index = (unsigned char)((crc ^ ((unsigned long long int)*msg << 56)) >> 56);
        /* Shift out the MSB used for division per lookuptable and XOR with the remainder */
        crc = (unsigned long long int)((crc << 8) ^ (crcTable[index]));
        msg++;
    }

    return crc;
}

//this function depends on Append function, so currently this is not working.
//i will fix this in the future

// unsigned long long int check_crc(char *msg, int size)
// {
//     return crc_64(&msg, size) == 0;
// }


//here the crc values are correct 
//the only problem is that the appending function is not working becuase of malloc
//which means the checking function is not working also. This will be fixed in the future
int main(void)
{
    CalcCRCTable(); //init table

    char test0[22] = "this is test number 0";
    char test1[23] = "this is test number one";
    char test2[39] = "this is a random test which numberred 2";
    
    printf("%llu", crc_64(test0, 22));
    printf("\n%llu", crc_64(test1, 23));
    printf("\n%llu", crc_64(test2, 39));

    
}
