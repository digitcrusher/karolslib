/*
 * module.h
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
#ifndef KAROLSLIB_MODULE_H
#define KAROLSLIB_MODULE_H

struct KL_routine {
    char* name; //Name for the routine - useful for name searching.
    void (*func)(); //Pointer to the function of type void ().
};
struct KL_module { //Struct module contains routines.
    char* name; //Same for routine except the name is for the module.
    void* data; //Pointer to module's data which may contain useful data, might be NULL.
    int size; //Number of routines the module has.
    KL_routine* rtns; //Pointer to the routines.
};
struct KL_modboard { //Struct modboard contains array of modules, modbrd is the global modboard.
    int size; //Number of modules.
    KL_module* array; //Pointer to the modules.
};

KL_routine*  KL_createRoutine(const char* name, void (*func)()); //Creates a struct and returns a pointer to it
void         KL_destroyRoutine(KL_routine& rtn); //Deallocate rtn.
void         KL_destroyRoutine(KL_routine* rtn);
void         KL_addRtn(KL_routine rtn, KL_module& mod); //Add rtn to mod
void         KL_addRtn(KL_routine rtn, KL_module* mod);
KL_routine*  KL_getRtn(int x, KL_module& mod);  //Get routine x from mod.
KL_routine*  KL_getRtn(int x, KL_module* mod);
KL_routine*  KL_getRtn(const char* x, KL_module& mod);
KL_routine*  KL_getRtn(const char* x, KL_module* mod);
KL_module*   KL_createModule(const char* name, void* data);
void         KL_destroyModule(KL_module& mod);
void         KL_destroyModule(KL_module* mod);
void         KL_addMod(KL_module mod, KL_modboard& brd);
void         KL_addMod(KL_module mod, KL_modboard* brd);
KL_module*   KL_getMod(int x, KL_modboard& brd);
KL_module*   KL_getMod(int x, KL_modboard* brd);
KL_module*   KL_getMod(const char* x, KL_modboard& brd);
KL_module*   KL_getMod(const char* x, KL_modboard* brd);
KL_modboard* KL_createModBoard();
void         KL_destroyModBoard(KL_modboard& brd);
void         KL_destroyModBoard(KL_modboard* brd);
void         KL_resetModBoard(KL_modboard& brd);
void         KL_resetModBoard(KL_modboard* brd);

extern KL_modboard* KL_stdmodbrd; //Global modboard

#endif
