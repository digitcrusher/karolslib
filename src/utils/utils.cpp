/*
 * utils.cpp
 * karolslib Source Code
 * Available on Github
 *
 * Copyright (C) 2017 Karol "digitcrusher" Łacina
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <src/utils/utils.h>

#if defined(__linux__)
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
long KL_getNS() {
    timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC, &ts)) return 0;
    return ts.tv_sec*1000000000+ts.tv_nsec;
}
int getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}
#elif defined(_WIN32)
#include <windows.h>
long KL_getNS() { //Get nanoseconds
    LARGE_INTEGER f,c; //Union which will store frequency and ticks
    if(!QueryPerformanceFrequency(&f)) return 0; //Try to get processor clock frequency
    if(!QueryPerformanceCounter(&c)) return 0; //Try to get processor clock ticks
    return c.QuadPart/(f.QuadPart/1000000000); //Return nanoseconds (Ticks / (Frequency / MSperSECOND))
}
void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
#endif

int indexof(const char* string, char search) {
    for(int i=0; i<(int)strlen(string); i++) {
        if(*(string+i) == search) {
            return i;
        }
    }
    return 1;
}
size_t intlen(int x) {
    int i=x;
    size_t len=1;
    if(i!=0) {
        for(len = 0; i!=0; len++) {
            i = i/10;
        }
    }
    return len+(x<0);
}
size_t uintlen(unsigned int x) {
    int i=x;
    size_t len=1;
    if(i!=0) {
        for(len = 0; i!=0; len++) {
            i = i/10;
        }
    }
    return len;
}
size_t floatlen(float x) {
    float i=x;
    size_t len=1;
    if((int)i!=0) {
        for(len = 0; (int)i!=0; len++) {
            i = i/10;
        }
    }
    i = x;
    if(i!=0) {
        for(; (int)i!=i; len++) {
            i = i*10;
        }
    }
    return len+(x<0)+((int)x!=x);
}
bool stof(char* str, float* ret) {
    int len=strlen(str);
    bool neg=0;
    bool floatpoint=0;
    float num=0;
    char* whole=(char*)malloc(sizeof(char)*len);
    int wholelen=0;
    char* point=(char*)malloc(sizeof(char)*len);
    int floatlen=0;
    for(int i=0; i<len; i++) {
        if(i == 0 && *(str+i) == '-') {
            neg = 1;
        }else if(*(str+i) == '.' && !floatpoint) {
            floatpoint = 1;
        }else if(*(str+i)-'0' >= 0 && *(str+i)-'0' <= 9) {
            if(floatpoint) {
                *(point+floatlen++) = *(str+i)-'0';
            }else {
                *(whole+wholelen++) = *(str+i)-'0';
            }
        }else {
            return 1;
        }
    }
    for(int i=0; i<wholelen; i++) {
        if(i>=1) {
            num += pow(10,i)**(whole+wholelen-1-i);
        }else {
            num += *(whole+wholelen-1-i);
        }
    }
    for(int i=0; i<floatlen; i++) {
        num += *(point+i)/pow(10,i+1);
    }
    if(neg) {
        num = -(num);
    }
    free(whole);
    free(point);
    *ret = num;
    return 0;
}
bool stoi(char* str, int* ret) {
    int len=strlen(str);
    bool neg=0;
    int num=0;
    char* whole=(char*)malloc(sizeof(char)*len);
    int wholelen=0;
    for(int i=0; i<len; i++) {
        if(i == 0 && *(str+i) == '-') {
            neg = 1;
        }else if(*(str+i)-'0' >= 0 && *(str+i)-'0' <= 9) {
            *(whole+wholelen++) = *(str+i)-'0';
        }else {
            return 1;
        }
    }
    for(int i=0; i<wholelen; i++) {
        if(i>0) {
            num += pow(10,i)**(whole+wholelen-1-i);
        }else {
            num += *(whole+wholelen-1-i);
        }
    }
    if(neg) {
        num = -(num);
    }
    free(whole);
    *ret = num;
    return 0;
}
bool stoui(char* str, unsigned int* ret) {
    int len=strlen(str);
    unsigned int num=0;
    char* whole=(char*)malloc(sizeof(char)*len);
    int wholelen=0;
    for(int i=0; i<len; i++) {
        if(*(str+i)-'0' >= 0 && *(str+i)-'0' <= 9) {
            *(whole+wholelen++) = *(str+i)-'0';
        }else {
            return 1;
        }
    }
    for(int i=0; i<wholelen; i++) {
        if(i>0) {
            num += pow(10,i)**(whole+wholelen-1-i);
        }else {
            num += *(whole+wholelen-1-i);
        }
    }
    free(whole);
    *ret = num;
    return 0;
}
char* itos(int x) {
    int len=intlen(x);
    char* str=(char*)malloc(sizeof(char)*(len+1));
    int num=x;
    bool neg=0;
    int ptr=0;
    if(x<0) {
        neg = 1;
        num = 0-x;
        *(str+ptr++) = '-';
    }
    for(int i=0; i<len-neg; i++, ptr++) {
        *(str+len-1-i) = num%10+'0';
        num = num/10;
    }
    *(str+len) = '\0';
    return str;
}
char* uitos(unsigned int x) {
    int len=uintlen(x);
    char* str=(char*)malloc(sizeof(char)*(len+1));
    unsigned int num=x;
    int ptr=0;
    for(int i=0; i<len; i++, ptr++) {
        *(str+len-1-i) = num%10+'0';
        num = num/10;
    }
    *(str+len) = '\0';
    return str;
}
char* ftos(float x) {
    int len=floatlen(x);
    char* str=(char*)malloc(sizeof(char)*(len+1));
    float num=x;
    bool neg=0;
    bool exponent=0;
    int ptr=0;
    if(x<0) {
        neg = 1;
        num = 0-x;
        *(str+ptr++) = '-';
    }
    if((int)x!=x) {
        exponent = 1;
    }
    len = intlen((int)x);
    for(int i=0; i<len-neg; i++, ptr++) {
        *(str+len-1-i) = (int)num%10+'0';
        num = num/10;
    }
    num = neg ? 0-x : x;
    if(exponent) {
        *(str+ptr++) = '.';
        for(; (int)num!=num; ptr++) {
            num = num*10;
            *(str+ptr) = (int)num%10+'0';
        }
    }
    len = floatlen(x);
    *(str+len) = '\0';
    return str;
}
int htoi(char* hex) {
    int dec=0;
    for(int i=strlen(hex); i>0; i--) {
        int num=0;
        switch(hex[i-1]) {
            case'0':
                num=0;
            break;
            case'1':
                num=1;
            break;
            case'2':
                num=2;
            break;
            case'3':
                num=3;
            break;
            case'4':
                num=4;
            break;
            case'5':
                num=5;
            break;
            case'6':
                num=6;
            break;
            case'7':
                num=7;
            break;
            case'8':
                num=8;
            break;
            case'9':
                num=9;
            break;
            case'A':
                num=10;
            break;
            case'a':
                num=10;
            break;
            case'B':
                num=11;
            break;
            case'b':
                num=11;
            break;
            case'C':
                num=12;
            break;
            case'c':
                num=12;
            break;
            case'D':
                num=13;
            break;
            case'd':
                num=13;
            break;
            case'E':
                num=14;
            break;
            case'e':
                num=14;
            break;
            case'F':
                num=15;
            break;
            case'f':
                num=15;
            break;
            default:
                num=0;
            break;
        }
        if(!strlen(hex)-i==0) {
            dec+=pow(16,i)*num;
        }else {
            dec+=num;
        }
    }
    return dec;
}
/*char* itos(int input, char* msg) {
    int len = intlen(input);
    char* result = (char*)malloc(sizeof(char)*(len+strlen(msg)));
    int sub = 0;
    for(int i=0;i<len;i++) {
        int current = floor((input-sub)/pow(10,(len-i-1)));
        *(result+i) = current + '0';
        sub += pow(10,(len-i-1))*current;
    }
    strcpy(result+len, msg);
    return result;
}*/
