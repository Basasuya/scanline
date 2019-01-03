#ifndef BASASUYA_OBJ_H
#define BASASUYA_OBJ_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <fstream>
#include <cmath>
#include "Point.h"
#include "Tool.h"

class Obj {
public:
	std::vector< Point<float> > vertex;
	std::vector< Point<float> > normal;
	std::vector<Color> color;
	std::vector< std::vector<int> > face;
	int n_vertex;
	int n_face;
	Point<float> center;
	float minx;
	float miny;
	float maxx;
	float maxy;

public:
	Obj();
	int getFaceNum();
	void load(std::string);
	void scale(int ,int );
};


#endif