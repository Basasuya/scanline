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
private:
	std::vector<std::vector<Edge> > ETable;
	std::vector<Edge> AET;
	int height;
	int width;
public:
	void setSize(int _width, int _height);
	void BuildTable(Obj &obj);
	void UpdateActiveEdge();
	void ComputeBuffer(int y, std::vector<Color> &buffer, Obj &obj);
	void scan(Obj &obj, std::vector<std::vector<Color> > &);
};


#endif