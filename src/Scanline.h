#ifndef BASASUYA_SCANLINE_H
#define BASASUYA_SCANLINE_H

#include "Obj.h"
#include "Tool.h"
#include <vector>
#include "Point.h"
#include <set>
#include <map>
#include <algorithm>

class Scanline {
public:
	std::vector<std::vector<Edge> > ETable;
	std::vector<std::vector<Poly> > PTable;
	std::vector<Edge> AET;
	std::vector<Poly> APT;
	int height;
	int width;

	void setSize(int _width, int _height);
    Color getColor(Point<float> normal);
	void BuildTable(Obj &obj);
	void UpdateActiveEdge();
	void UpdateActivePoly();
	void ComputeBuffer(int y, std::vector<Color> &buffer, Obj &obj);
	void scan(Obj &obj, std::vector<std::vector<Color> > &);
	int UpdateFlagByID(int id);
	Color pixelByID(int,float ,float);
};


#endif