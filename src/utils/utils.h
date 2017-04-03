/*
 * utils.h
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
#ifndef KAROLSLIB_UTILS_H
#define KAROLSLIB_UTILS_H

#if defined(__linux__)
long KL_getNS();
int getch();
#elif defined(_WIN32)
long KL_getNS();
void hidecursor();
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

template<typename T> class KL_Vector {
    private:
        int vsize;
        const int maxsize=2147483647;
        T* array;
    public:
        KL_Vector();
        ~KL_Vector();
        bool pushBack(T t);
        bool popBack(T* t);
        bool replace(int element, T t);
        bool resize(int newsize);
        bool getP(int element, T* t);
        bool clear();
        T* getArray();
        int size();
        T operator[](int n);
};
template<typename T> KL_Vector<T>::KL_Vector() {
    vsize=0;
    array=(T*)malloc(sizeof(T)*vsize);
}
template<typename T> KL_Vector<T>::~KL_Vector() {
    free(array);
}
template<typename T> bool KL_Vector<T>::pushBack(T t) {
    if(resize(vsize+1))
        return 1;
    array[vsize-1] = t;
    return 0;
}
template<typename T> bool KL_Vector<T>::popBack(T* t) {
    *t = array[vsize-1];
    return resize(vsize-1);
}
template<typename T> bool KL_Vector<T>::replace(int element, T t) {
    if(element>vsize-1)
        return 1;
    array[element] = t;
    return 0;
}
template<typename T> bool KL_Vector<T>::resize(int newsize) {
    if(newsize>maxsize)
        return 1;
    array = (T*)realloc((void*)array, sizeof(T)*newsize);
    vsize = newsize;
    return 0;
}
template<typename T> bool KL_Vector<T>::getP(int element, T* t) {
    if(element>vsize-1)
        return 1;
    t = array+element;
    return 0;
}
template<typename T> bool KL_Vector<T>::clear() {
    vsize=0;
    resize(vsize);
    return 0;
}
template<typename T> T* KL_Vector<T>::getArray() {
    return array;
}
template<typename T> int KL_Vector<T>::size() {
    return vsize;
}
template<typename T> T KL_Vector<T>::operator[](int n) {
    return array[n];
}

#endif
