#include "fpoint.h"
#include <cmath>

FPoint::FPoint(float x0, float y0)
:x(x0)
,y(y0)
{}

FPoint::FPoint(const FPoint& p){
	*this = p;
}

FPoint& FPoint::operator=(const FPoint& p){
	if(this == &p){
		return *this;
	}
	x = p.x;
	y = p.y;
	return *this;
}

FPoint FPoint::operator+(const FPoint& p){
	return FPoint(x + p.x, y + p.y);
}

FPoint FPoint::operator-(const FPoint& p){
	return FPoint(x - p.x, y - p.y);
}

FPoint FPoint::operator*(float val){
	return FPoint(x * val, y * val);;
}

FPoint FPoint::operator/(float val){
	return FPoint(x / val, y / val);;
}

bool FPoint::operator==(const FPoint& r){
	FPoint p(*this-r);
	return ((p.x*p.x + p.y*p.y) <= (M_ACCURACY*M_ACCURACY));
}

FPoint::~FPoint(){
}

float FPoint::Length(){
	return sqrtf(x*x + y*y);
}
