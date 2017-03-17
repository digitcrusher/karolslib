/*
 * karolslib.h
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
#ifndef _KAROLSLIB_H_
#define _KAROLSLIB_H_
#define KAROLSLIB_MAJOR_VERSION 0
#define KAROLSLIB_MINOR_VERSION 1

#define main karolslib_user_main
int karolslib_user_main(int argc, char** argv);
#if defined(_WIN32)
struct winmainargs {
    HINSTANCE hInstance;
    HINSTANCE hPrevInstance;
    PSRT szCmdLine;
    int iCmdShow;
}
extern winmainargs winargs;
#endif

void karolslib_init();

#endif
