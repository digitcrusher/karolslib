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
#include <src/utils/module.h>
#include <src/window/terminal.h>
#include <src/karolslib.h>

#if !defined(_WIN32)
#ifdef main
#undef main
#endif
#define KL_main main
#endif
int KL_main(int argc, char** argv) {
    return KL_user_main(argc, argv);
}
#if defined(_WIN32)
#ifdef WinMain
#undef WinMain
#endif
#define KL_WinMain WinMain
static void KL_unEscapeQuotes(char *arg) {
	char *last = NULL;
	while(*arg) {
		if(*arg == '"' && *last == '\\') {
			char *c_curr = arg;
			char *c_last = last;

			while(*c_curr) {
				*c_last = *c_curr;
				c_last = c_curr;
				c_curr++;
			}
			*c_last = '\0';
		}
		last = arg;
		arg++;
	}
}
static int KL_parseCommandLine(char *cmdline, char **argv) {
	char *bufp;
	char *lastp = NULL;
	int argc, last_argc;
	argc = last_argc = 0;
	for(bufp = cmdline; *bufp;) {
		/* Skip leading whitespace */
		while(*bufp == ' ') {
			++bufp;
		}
		/* Skip over argument */
		if(*bufp == '"') {
			++bufp;
			if(*bufp) {
				if(argv) {
					argv[argc] = bufp;
				}
				++argc;
			}
			/* Skip over word */
			while(*bufp && (*bufp != '"' || (lastp && *lastp == '\\'))) {
				lastp = bufp;
				++bufp;
			}
		}else {
			if(*bufp) {
				if(argv) {
					argv[argc] = bufp;
				}
				++argc;
			}
			/* Skip over word */
			while(*bufp && *bufp != ' ') {
				++bufp;
			}
		}
		if(*bufp) {
			if(argv) {
				*bufp = '\0';
			}
			++bufp;
		}
		/* Strip out \ from \" sequences */
		if(argv && last_argc != argc) {
			KL_unEscapeQuotes(argv[last_argc]);
		}
		last_argc = argc;
	}
	if(argv) {
		argv[argc] = NULL;
	}
	return argc;
}
HINSTANCE KL_hInstance;
HINSTANCE KL_hPrevInstance;
PSTR KL_szCmdLine;
int KL_iCmdShow;
int WINAPI KL_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    KL_hInstance = hInstance;
    KL_hPrevInstance = hPrevInstance;
    KL_szCmdLine = szCmdLine;
    KL_iCmdShow = iCmdShow;
    char** argv;
    int argc;
	argc = KL_parseCommandLine(szCmdLine, NULL);
	argv = (char**)malloc(sizeof(char*)*(strlen(szCmdLine)+1));
	if(argv == NULL) {
		return 0;
	}
	KL_parseCommandLine(szCmdLine, argv);
    return KL_main(argc, argv);
}
#endif

void KL_init() {
    //Set up stdmodbrd.
    KL_stdmodbrd = KL_createModBoard();
    //Set up stdterm.
    KL_stdterm = KL_createTerminal(TERMINAL_DEFAULT_BUFF_WIDTH, TERMINAL_DEFAULT_BUFF_HEIGHT, TERMINAL_DEFAULT_FLAGS, NULL, NULL);
    //Create module main and add it to the global modboard modbrd.
    KL_addMod(*KL_createModule("main", NULL), KL_stdmodbrd);
    //Create routine main and add it to module main from modbrd.
    KL_addRtn(*KL_createRoutine("main", (void (*)())NULL), KL_getMod("main", KL_stdmodbrd));
    //Create init.
    KL_addRtn(*KL_createRoutine("init", (void (*)())NULL), KL_getMod("main", KL_stdmodbrd));
    //Create deinit.
    KL_addRtn(*KL_createRoutine("deinit", (void (*)())NULL), KL_getMod("main", KL_stdmodbrd));
    //Iterate over the modules from modbrd.
    for(int i=0; i<KL_stdmodbrd->size; i++) {
        KL_routine* rtn;
        //Rtn has init? If yes call it.
        if((rtn = KL_getRtn("init", KL_getMod(i, KL_stdmodbrd)))) {
            if(rtn->func != NULL) {
                rtn->func();
            }
        }
    }
}
void KL_deinit() {
    //Iterate over the modules from modbrd.
    for(int i=0; i<KL_stdmodbrd->size; i++) {
        KL_routine* rtn;
        //Rtn has deinit? If yes call it.
        if((rtn = KL_getRtn("deinit", KL_getMod(i, KL_stdmodbrd)))) {
            if(rtn->func != NULL) {
                rtn->func();
            }
        }
    }
    //Delete stdmodbrd.
    KL_destroyModBoard(KL_stdmodbrd);
    //Delete stdterm.
    KL_destroyTerminal(KL_stdterm);
}
