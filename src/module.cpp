/*
 * module.cpp
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
#include <stdlib.h> //malloc, realloc
#include <string.h> //strlen, strcpy, strcmp
#include <src/module.h>

modboard* stdmodbrd;

routine* createRoutine(const char* name, void (*func)()) {
    routine* rtn = (routine*)malloc(sizeof(routine)); //Allocate memory for rtn.
    rtn->name = (char*)malloc(sizeof(char)*strlen(name)); //Allocate memory for name.
    strcpy(rtn->name, name); //Copy name to rtn->name.
    rtn->func = func; //Set the pointer.
    return rtn;
}
void deleteRoutine(routine& rtn) {
    free(rtn.name);
}
void deleteRoutine(routine* rtn) {
    return deleteRoutine(*rtn);
}
void addRtn(routine rtn, module& mod) {
    mod.size++; //Update the number of modules.
    mod.rtns = (routine*)realloc((void*)mod.rtns, sizeof(routine)*mod.size); //Reallocate routines.
    *(mod.rtns+mod.size-1) = rtn; //Copy rtn to the back of the array.
}
void addRtn(routine rtn, module* mod) {
    return addRtn(rtn, *mod);
}
routine* getRtn(int x, module& mod) {
    if(x>=0 && x<mod.size) { //Is x in the bounds of the array?
        return mod.rtns+x; //Yes, return routine x.
    }
    return NULL; //No, return NULL.
}
routine* getRtn(int x, module* mod) {
    return getRtn(x, *mod);
}
routine* getRtn(const char* x, module& mod) {
    for(int i=0; i>=0 && i<mod.size; i++) { //Iterate over the routines.
        if(!strcmp((mod.rtns+i)->name, x)) { //Do the names match?
            return mod.rtns+i; //Yes, return the matching routine.
        }
    }
    return NULL; //No, return NULL.
}
routine* getRtn(const char* x, module* mod) {
    return getRtn(x, *mod);
}
module* createModule(const char* name, void* data) {
    module* mod = (module*)malloc(sizeof(module));
    mod->name = (char*)malloc(sizeof(char)*strlen(name));
    strcpy(mod->name, name); //Copy name to mod->name.
    mod->data = data;
    mod->size = 0; //No routines at the time of the creation.
    mod->rtns = (routine*)malloc(sizeof(routine)*mod->size);
    return mod;
}
void deleteModule(module& mod) {
    free(mod.name);
    free(mod.data);
    free(mod.rtns);
}
void deleteModule(module* mod) {
    return deleteModule(*mod);
}
void addMod(module mod, modboard& brd) {
    brd.size++;
    brd.array = (module*)realloc((void*)brd.array, sizeof(module)*brd.size); //Reallocate.
    *(brd.array+brd.size-1) = mod; //Copy.
}
void addMod(module mod, modboard* brd) {
    return addMod(mod, *brd);
}
module* getMod(int x, modboard& brd) {
    if(x>=0 && x<brd.size)
        return brd.array+x;
    return NULL;
}
module* getMod(int x, modboard* brd) {
    return getMod(x, *brd);
}
module* getMod(const char* x, modboard& brd) {
    for(int i=0; i>=0 && i<brd.size; i++) {
        if(!strcmp((brd.array+i)->name, x)) {
            return brd.array+i;
        }
    }
    return NULL;
}
module* getMod(const char* x, modboard* brd) {
    return getMod(x, *brd);
}
modboard* createModBoard() {
    modboard* brd = (modboard*)malloc(sizeof(modboard));
    brd->size = 0;
    brd->array = (module*)malloc(sizeof(module)*brd->size);
    return brd;
}
void deleteModBoard(modboard& brd) {
    free(brd.array);
}
void deleteModBoard(modboard* brd) {
    return deleteModBoard(*brd);
}
void resetModBoard(modboard& brd) {
    brd.size = 0;
    free(brd.array);
    brd.array = (module*)malloc(sizeof(module)*brd.size);
}
void resetModBoard(modboard* brd) {
    return resetModBoard(*brd);
}
