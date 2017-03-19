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
#include <src/terminal.h>
#include <src/karolslib.h>

#if !defined(_WIN32)
#ifdef main
#undef main
#endif
#define karolslib_main main
#endif
int karolslib_main(int argc, char** argv) {
    return karolslib_user_main(argc, argv);
}
#if defined(_WIN32)
#ifdef WinMain
#undef WinMain
#endif
#define karolslib_WinMain WinMain
static void karolslib_unEscapeQuotes(char *arg) {
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
static int karolslib_parseCommandLine(char *cmdline, char **argv) {
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
			karolslib_unEscapeQuotes(argv[last_argc]);
		}
		last_argc = argc;
	}
	if(argv) {
		argv[argc] = NULL;
	}
	return argc;
}
HINSTANCE karolslib_hInstance;
HINSTANCE karolslib_hPrevInstance;
PSTR karolslib_szCmdLine;
int karolslib_iCmdShow;
int WINAPI karolslib_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    karolslib_hInstance = hInstance;
    karolslib_hPrevInstance = hPrevInstance;
    karolslib_szCmdLine = szCmdLine;
    karolslib_iCmdShow = iCmdShow;
    char** argv;
    int argc;
	argc = karolslib_parseCommandLine(szCmdLine, NULL);
	argv = (char**)malloc(sizeof(char*)*(strlen(szCmdLine)+1));
	if(argv == NULL) {
		return 0;
	}
	karolslib_parseCommandLine(szCmdLine, argv);
    return karolslib_main(argc, argv);
}
#endif

void karolslib_init() {
    //Set up stdmodbrd.
    stdmodbrd = createModBoard();
    //Set up stdterm.
    stdterm = createTerminal(TERMINAL_DEFAULT_BUFF_WIDTH, TERMINAL_DEFAULT_BUFF_HEIGHT, TERMINAL_DEFAULT_FLAGS, NULL, NULL);
    //Create module main and add it to the global modboard modbrd.
    addMod(*createModule("main", NULL), stdmodbrd);
    //Create routine main and add it to module main from modbrd.
    addRtn(*createRoutine("main", (void (*)())NULL), getMod("main", stdmodbrd));
    //Create init.
    addRtn(*createRoutine("init", (void (*)())NULL), getMod("main", stdmodbrd));
    //Iterate over the modules from modbrd.
    for(int i=0; i<stdmodbrd->size; i++) {
        routine* rtn;
        //Rtn has init? If yes call it.
        if((rtn = getRtn("init", getMod(i, stdmodbrd)))) {
            if(rtn->func != NULL) {
                rtn->func();
            }
        }
    }
}
