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
#include <src/utils/module.h>

KL_modboard* KL_stdmodbrd;

KL_routine* KL_createRoutine(const char* name, void (*func)()) {
    KL_routine* rtn = (KL_routine*)malloc(sizeof(KL_routine)); //Allocate memory for rtn.
    rtn->name = (char*)malloc(sizeof(char)*strlen(name)); //Allocate memory for name.
    strcpy(rtn->name, name); //Copy name to rtn->name.
    rtn->func = func; //Set the pointer.
    return rtn;
}
void KL_destroyRoutine(KL_routine& rtn) {
    free(rtn.name);
}
void KL_destroyRoutine(KL_routine* rtn) {
    return KL_destroyRoutine(*rtn);
}
void KL_addRtn(KL_routine rtn, KL_module& mod) {
    mod.size++; //Update the number of modules.
    mod.rtns = (KL_routine*)realloc((void*)mod.rtns, sizeof(KL_routine)*mod.size); //Reallocate routines.
    *(mod.rtns+mod.size-1) = rtn; //Copy rtn to the back of the array.
}
void KL_addRtn(KL_routine rtn, KL_module* mod) {
    return KL_addRtn(rtn, *mod);
}
KL_routine* KL_getRtn(int x, KL_module& mod) {
    if(x>=0 && x<mod.size) { //Is x in the bounds of the array?
        return mod.rtns+x; //Yes, return KL_routine x.
    }
    return NULL; //No, return NULL.
}
KL_routine* KL_getRtn(int x, KL_module* mod) {
    return KL_getRtn(x, *mod);
}
KL_routine* KL_getRtn(const char* x, KL_module& mod) {
    for(int i=0; i>=0 && i<mod.size; i++) { //Iterate over the routines.
        if(!strcmp((mod.rtns+i)->name, x)) { //Do the names match?
            return mod.rtns+i; //Yes, return the matching KL_routine.
        }
    }
    return NULL; //No, return NULL.
}
KL_routine* KL_getRtn(const char* x, KL_module* mod) {
    return KL_getRtn(x, *mod);
}
KL_module* KL_createModule(const char* name, void* data) {
    KL_module* mod = (KL_module*)malloc(sizeof(KL_module));
    mod->name = (char*)malloc(sizeof(char)*strlen(name));
    strcpy(mod->name, name); //Copy name to mod->name.
    mod->data = data;
    mod->size = 0; //No routines at the time of the creation.
    mod->rtns = (KL_routine*)malloc(sizeof(KL_routine)*mod->size);
    return mod;
}
void KL_destroyModule(KL_module& mod) {
    free(mod.name);
    free(mod.data);
    free(mod.rtns);
}
void KL_destroyModule(KL_module* mod) {
    return KL_destroyModule(*mod);
}
void KL_addMod(KL_module mod, KL_modboard& brd) {
    brd.size++;
    brd.array = (KL_module*)realloc((void*)brd.array, sizeof(KL_module)*brd.size); //Reallocate.
    *(brd.array+brd.size-1) = mod; //Copy.
}
void KL_addMod(KL_module mod, KL_modboard* brd) {
    return KL_addMod(mod, *brd);
}
KL_module* KL_getMod(int x, KL_modboard& brd) {
    if(x>=0 && x<brd.size) {
        return brd.array+x;
    }
    return NULL;
}
KL_module* KL_getMod(int x, KL_modboard* brd) {
    return KL_getMod(x, *brd);
}
KL_module* KL_getMod(const char* x, KL_modboard& brd) {
    for(int i=0; i>=0 && i<brd.size; i++) {
        if(!strcmp((brd.array+i)->name, x)) {
            return brd.array+i;
        }
    }
    return NULL;
}
KL_module* KL_getMod(const char* x, KL_modboard* brd) {
    return KL_getMod(x, *brd);
}
KL_modboard* KL_createModBoard() {
    KL_modboard* brd = (KL_modboard*)malloc(sizeof(KL_modboard));
    brd->size = 0;
    brd->array = (KL_module*)malloc(sizeof(KL_module)*brd->size);
    return brd;
}
void KL_destroyModBoard(KL_modboard& brd) {
    free(brd.array);
}
void KL_destroyModBoard(KL_modboard* brd) {
    return KL_destroyModBoard(*brd);
}
void KL_resetModBoard(KL_modboard& brd) {
    brd.size = 0;
    free(brd.array);
    brd.array = (KL_module*)malloc(sizeof(KL_module)*brd.size);
}
void KL_resetModBoard(KL_modboard* brd) {
    return KL_resetModBoard(*brd);
}
