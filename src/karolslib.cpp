/*
 * karolslib.cpp
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
#include <stdlib.h>
#include <src/module.h>
#include <src/karolslib.h>

#ifdef main
#undef main
#endif
#define karolslib_main main
int karolslib_main(int argc, char** argv) {
    return karolslib_user_main(argc, argv);
}
#if defined(_WIN32)
winmainargs winargs;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSRT szCmdLine, int iCmdShow) {
    winargs.hInstance = hInstance;
    winargs.hPrevInstance = hPrevInstance;
    winargs.szCmdLine = szCmdLine;
    winargs.iCmdShow = iCmdShow;
    char** argv;
    int argc;
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if(argv == NULL) {
        return 0;
    }
    return karolslib_main(argc, argv);
}
#endif

void karolslib_init() {
    //Create module main and add it to the global modboard modbrd.
    addMod(*createModule("main", NULL), modbrd);
    //Create routine main and add it to module main from modbrd.
    addRtn(*createRoutine("main", (void (*)())NULL), getMod("main", modbrd));
    //Create init.
    addRtn(*createRoutine("init", (void (*)())NULL), getMod("main", modbrd));
    //Iterate over the modules from modbrd.
    for(int i=0; i<modbrd->size; i++) {
        routine* rtn;
        //Rtn has init? If yes call it.
        if((rtn = getRtn("init", getMod(i, modbrd)))) {
            if(rtn->func != NULL) {
                rtn->func();
            }
        }
    }
}
