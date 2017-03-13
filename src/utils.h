/*
 * utils.h
 * Karolslib Source Code
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
#ifndef _KAROLSLIB_UTILS_H_
#define _KAROLSLIB_UTILS_H_

#if defined(WIN32) && defined(_WIN32) && defined(__WIN32)
long getMS();
void hidecursor();
#else
int getch();
#endif

int indexof(const char* string, char search); //Search string for search
size_t intlen(int x); //How many digits in x?
size_t uintlen(unsigned int x); //How many digits in x?
size_t floatlen(float x); //How many digits in x?
bool stof(char* str, float* ret); //Convert str to a float
bool stoi(char* str, int* ret); //Convert str to an int
bool stoui(char* str, unsigned int* ret); //Convert str to an unsigned int
char* itos(int x); //Convert int to string
char* uitos(unsigned int x); //Convert unsigned int to string
char* ftos(float x); //Convert float to string
int htoi(char* hex); //Convert hex string to int
//char* itos(int input, char* msg); //Convert int to string and concatenate msg to the end !!!!!NOT USED!!!!!

template<typename T> class Vector {
    private:
        int vsize;
        const int maxsize=2147483647;
        T* array;
    public:
        Vector();
        ~Vector();
        bool push_back(T t);
        bool pop_back(T* t);
        bool replace(int element, T t);
        bool resize(int newsize);
        bool getP(int element, T* t);
        bool clear();
        T* getArray();
        int size();
        T operator[](int n);
};

#endif
