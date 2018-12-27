//
// Created by zjp on 18-12-27.
//
#include <stdio.h>
#include "stdlib.h"
#include <iostream>
#include <string.h>
#include <time.h>

using namespace std;

#define ARRAY_SIZE 256 /*we get the 256 chars of each line*/
#define SIZE 48000000 /* size should be 1/8 of max*/
#define MAX  384000000/*the max bit space*/

#define SETBIT(ch,n) ch[n/8]|=1<<(7-n%8)
#define GETBIT(ch,n) (ch[n/8]&1<<(7-n%8))>>(7-n%8)

unsigned int len(char *ch);/* functions to calculate the length of the url*/

unsigned int RSHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int JSHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int PJWHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int ELFHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int BKDRHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int SDBMHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int DJBHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int DEKHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int BPHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int FNVHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int APHash(char* str, unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int HFLPHash(char* str,unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int HFHash(char* str,unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int StrHash( char* str,unsigned int len);/* functions to calculate the hash value of the url*/
unsigned int TianlHash(char* str,unsigned int len);/* functions to calculate the hash value of the url*/

int main()
{
    int i,num,num2=0; /* the number to record the repeated urls and the total of it*/
    unsigned int tt=0;
    int flag;         /*it helps to check weather the url has already existed */
    char buf[257];    /*it helps to print the start time of the program */
    time_t tmp = time(NULL);

    char *file1 = "/home/zjp/Documents/Parallel/ParallelProgram/BigData/input.txt";
    char *file2 = "/home/zjp/Documents/Parallel/ParallelProgram/BigData/output.txt";
    FILE *fp1,*fp2;/*pointer to the file */
    char ch[ARRAY_SIZE];
    char *vector ;/* the bit space*/
    vector = (char *)calloc(SIZE,sizeof(char));

    //printf("Please enter the file with repeated urls:\n");
    //scanf("%s",&file1);
    if( (fp1 = fopen(file1,"rb")) == NULL) {  /* open the goal file*/
        printf("Connot open the file %s!\n",file1);
    }

    //printf("Please enter the file you want to save to:\n");
    //scanf("%s",&file2);
    if( (fp2 = fopen(file2,"w")) == NULL) {
        printf("Connot open the file %s\n",file2);
    }
    strftime(buf,32,"%Y-%m-%d %H:%M:%S",localtime(&tmp));
    printf("%s\n",buf); /*print the system time*/

    for(i=0;i<SIZE;i++) {
        vector[i]=0;  /*set 0*/
    }

    while(!feof(fp1)) { /* the check process*/

        fgets(ch,ARRAY_SIZE,fp1);
        flag=0;
        tt++;
        if( GETBIT(vector, HFLPHash(ch,len(ch))%MAX) ) {
            flag++;
        } else {
            SETBIT(vector,HFLPHash(ch,len(ch))%MAX );
        }

        if( GETBIT(vector, StrHash(ch,len(ch))%MAX) ) {
            flag++;
        } else {
            SETBIT(vector,StrHash(ch,len(ch))%MAX );
        }

        if( GETBIT(vector, HFHash(ch,len(ch))%MAX) )   {
            flag++;
        } else {
            SETBIT(vector,HFHash(ch,len(ch))%MAX );
        }

        if( GETBIT(vector, DEKHash(ch,len(ch))%MAX) ) {
            flag++;
        } else {
            SETBIT(vector,DEKHash(ch,len(ch))%MAX );
        }

        if( GETBIT(vector, TianlHash(ch,len(ch))%MAX) ) {
            flag++;
        } else {
            SETBIT(vector,TianlHash(ch,len(ch))%MAX );
        }

        if( GETBIT(vector, SDBMHash(ch,len(ch))%MAX) )  {
            flag++;
        } else {
            SETBIT(vector,SDBMHash(ch,len(ch))%MAX );
        }

        if(flag<6)
            num2++;
        else
            fputs(ch,fp2);

        /*	printf(" %d",flag); */
    }
    /* the result*/
    printf("\nThere are %d urls!\n",tt);
    printf("\nThere are %d not repeated urls!\n",num2);
    printf("There are %d repeated urls!\n",tt-num2);
    fclose(fp1);
    fclose(fp2);
    return 0;
}


/*functions may be used in the main */
unsigned int len(char *ch)
{
    int m=0;
    while(ch[m]!='\0') {
        m++;
    }
    return m;
}

unsigned int RSHash(char* str, unsigned int len) {
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    unsigned int i = 0;

    for(i=0; i<len; str++, i++) {
        hash = hash*a + (*str);
        a = a*b;
    }
    return hash;
}
/* End Of RS Hash Function */


unsigned int JSHash(char* str, unsigned int len)
{
    unsigned int hash = 1315423911;
    unsigned int i    = 0;

    for(i=0; i<len; str++, i++) {
        hash ^= ((hash<<5) + (*str) + (hash>>2));
    }
    return hash;
}
/* End Of JS Hash Function */


unsigned int PJWHash(char* str, unsigned int len)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i = 0;

    for(i=0;i<len; str++, i++) {
        hash = (hash<<OneEighth) + (*str);
        if((test = hash & HighBits)  != 0) {
            hash = ((hash ^(test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}
/* End Of  P. J. Weinberger Hash Function */


unsigned int ELFHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;

    for(i = 0; i < len; str++, i++) {
        hash = (hash << 4) + (*str);
        if((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}
/* End Of ELF Hash Function */


unsigned int BKDRHash(char* str, unsigned int len)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;

    for(i = 0; i < len; str++, i++)
    {
        hash = (hash * seed) + (*str);
    }

    return hash;
}
/* End Of BKDR Hash Function */


unsigned int SDBMHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i    = 0;

    for(i = 0; i < len; str++, i++) {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
/* End Of SDBM Hash Function */


unsigned int DJBHash(char* str, unsigned int len)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;

    for(i = 0; i < len; str++, i++) {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}
/* End Of DJB Hash Function */


unsigned int DEKHash(char* str, unsigned int len)
{
    unsigned int hash = len;
    unsigned int i    = 0;

    for(i = 0; i < len; str++, i++) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }
    return hash;
}
/* End Of DEK Hash Function */


unsigned int BPHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++) {
        hash = hash << 7 ^ (*str);
    }

    return hash;
}
/* End Of BP Hash Function */


unsigned int FNVHash(char* str, unsigned int len)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash      = 0;
    unsigned int i         = 0;

    for(i = 0; i < len; str++, i++) {
        hash *= fnv_prime;
        hash ^= (*str);
    }

    return hash;
}
/* End Of FNV Hash Function */


unsigned int APHash(char* str, unsigned int len)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;

    for(i = 0; i < len; str++, i++) {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                (~((hash << 11) + (*str) ^ (hash >> 5)));
    }

    return hash;
}
/* End Of AP Hash Function */
unsigned int HFLPHash(char *str,unsigned int len)
{
    unsigned int n=0;
    int i;
    char* b=(char *)&n;
    for(i=0;i<strlen(str);++i) {
        b[i%4]^=str[i];
    }
    return n%len;
}
/* End Of HFLP Hash Function*/
unsigned int HFHash(char* str,unsigned int len)
{
    int result=0;
    char* ptr=str;
    int c;
    int i=0;
    for (i=1;c=*ptr++;i++)
        result += c*3*i;
    if (result<0)
        result = -result;
    return result%len;
}
/*End Of HKHash Function */

unsigned int StrHash( char *str,unsigned int len)
{
    register unsigned int   h;
    register unsigned char *p;
    for(h=0,p=(unsigned char *)str;*p;p++) {
        h=31*h+*p;
    }

    return h;

}
/*End Of StrHash Function*/

unsigned int TianlHash(char *str,unsigned int len)
{
    unsigned long urlHashValue=0;
    int ilength=strlen(str);
    int i;
    unsigned char ucChar;
    if(!ilength)  {
        return 0;
    }
    if(ilength<=256)  {
        urlHashValue=16777216*(ilength-1);
    } else {
        urlHashValue = 42781900080;
    }
    if(ilength<=96) {
        for(i=1;i<=ilength;i++) {
            ucChar=str[i-1];
            if(ucChar<='Z'&&ucChar>='A')  {
                ucChar=ucChar+32;
            }
            urlHashValue+=(3*i*ucChar*ucChar+5*i*ucChar+7*i+11*ucChar)%1677216;
        }
    } else  {
        for(i=1;i<=96;i++)
        {
            ucChar=str[i+ilength-96-1];
            if(ucChar<='Z'&&ucChar>='A')
            {
                ucChar=ucChar+32;
            }
            urlHashValue+=(3*i*ucChar*ucChar+5*i*ucChar+7*i+11*ucChar)%1677216;
        }
    }
    return urlHashValue;

}
