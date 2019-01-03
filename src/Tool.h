#ifndef BASASUYA_TOOL_H
#define BASASUYA_TOOL_H

#define Color unsigned char
const int INF = 0x3f3f3f3f;

struct endpoint {
	float zl;
	float zr;
	int order;
    endpoint(float a, float b, int c):zl(a), zr(b), order(c){};
};

struct Edge {
	float x;
	float dy;
	float dx;
	int polyID;
};


struct Poly {
	int polyID;
	float a, b, c, d;
	int dy;
	Color color;
	bool flag;
};



#endif