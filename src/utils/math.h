/*
 * math.h
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
#ifndef KAROLSLIB_MATH_H
#define KAROLSLIB_MATH_H
#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846

struct V2i {
    int x,y;
};
struct V2f {
    float x,y;
};
struct V3i {
    int x,y,z;
};
struct V3f {
    float x,y,z;
};
typedef V2f V;
V2i operator+(const V2i& lvalue, const V2i& rvalue);
V2f operator+(const V2f& lvalue, const V2f& rvalue);
V3i operator+(const V3i& lvalue, const V3i& rvalue);
V3f operator+(const V3f& lvalue, const V3f& rvalue);
V2i operator-(const V2i& lvalue, const V2i& rvalue);
V2f operator-(const V2f& lvalue, const V2f& rvalue);
V3i operator-(const V3i& lvalue, const V3i& rvalue);
V3f operator-(const V3f& lvalue, const V3f& rvalue);
struct line {
    float x1,y1,x2,y2;
};
struct square {
    float x1,y1,x2,y2;
};

bool sqrInSqr(square sqr1, square sqr2);
bool fpil(line line, V point); //float point in line
bool fpilu(line line, V point, float unprec); //float point in line with unprecision
float fatp(V p1, V p2); //float angle to point

#endif
