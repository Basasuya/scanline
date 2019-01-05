#ifndef BASASUYA_POINT_H
#define BASASUYA_POINT_H
#include <iostream>
#include <cstring>




template <typename T>
struct Point {
	T x, y, z;
	Point() {}
	Point(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	
  Point<T> operator + (Point<T> &other) {
		return Point<T>(x + other.x, y + other.y, z + other.z);
	}
	
  Point<T> operator - (const Point<T>& other) {
		return Point<T>(x - other.x, y - other.y, z - other.z);
	}
	
	Point<T> operator *(const Point<T>& other) {
		return Point<T>(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
	}
    
	Point<T> operator /(const T &num) {
		return Point<T>(x / num, y / num, z / num);
	}
	
	Point<T> operator += (Point<T> &other) {
		return Point<T>(x + other.x, y + other.y, z + other.z);
	}
	
	Point<T> operator *(const T &num) {
		return Point<T>(x*num, y*num, z*num);
	}
};


#endif