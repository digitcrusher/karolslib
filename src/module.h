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
#ifndef _KAROLSLIB_MODULE_H_
#define _KAROLSLIB_MODULE_H_

struct routine {
    char* name; //Name for the routine - useful for name searching.
    void (*func)(); //Pointer to the function of type void ().
};
struct module { //Struct module contains routines.
    char* name; //Same for routine except the name is for the module.
    void* data; //Pointer to module's data which may contain useful data, might be NULL.
    int size; //Number of routines the module has.
    routine* rtns; //Pointer to the routines.
};
struct modboard { //Struct modboard contains array of modules, modbrd is the global modboard.
    int size; //Number of modules.
    module* array; //Pointer to the modules.
};

routine* createRoutine(const char* name, void (*func)()); //Creates the said struct and returns a pointer to it
void deleteRoutine(routine& rtn); //Deallocate rtn.
void deleteRoutine(routine* rtn);
void addRtn(routine rtn, module& mod); //Add rtn to mod
void addRtn(routine rtn, module* mod);
routine* getRtn(int x, module& mod);  //Get routine x from mod.
routine* getRtn(int x, module* mod);
routine* getRtn(const char* x, module& mod);
routine* getRtn(const char* x, module* mod);
module* createModule(const char* name, void* data);
void deleteModule(module& mod);
void deleteModule(module* mod);
void addMod(module mod, modboard& brd);
void addMod(module mod, modboard* brd);
module* getMod(int x, modboard& brd);
module* getMod(int x, modboard* brd);
module* getMod(const char* x, modboard& brd);
module* getMod(const char* x, modboard* brd);
modboard* createModBoard();
void deleteModBoard(modboard& brd);
void deleteModBoard(modboard* brd);
void resetModBoard(modboard& brd);
void resetModBoard(modboard* brd);

extern modboard* modbrd; //Global modboard

#endif
