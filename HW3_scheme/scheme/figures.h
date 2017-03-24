#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include "figures.cpp"

typedef class Figure;
/*
0--------------oX
|
|
|
|
|
|
oY
*/
typedef class point;

const point& operator+ (point a, const point& b);
const point& operator- (point a, const point& b);
const point& operator* (point a, const point& b);
const point& operator* (point a, const float& b);
const point& operator* (const float& a, point b);

typedef class line;

typedef class triangle;

typedef class qadrangle;

typedef class rectangle;
/*
  B---(length)---C
  |              |
  A---(length)---D
*/

typedef class square;
/*
  B---C
  |   |
  A---D
*/

typedef class circle;

#endif // FIGURES_H_INCLUDED
