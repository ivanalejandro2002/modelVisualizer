#pragma once
#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld eps = 1e-9, inf = numeric_limits<ld>::max(), pi = acos(-1);
bool geq(ld a, ld b){return a-b >= -eps;}     //a >= b
bool leq(ld a, ld b){return b-a >= -eps;}     //a <= b
bool ge(ld a, ld b){return a-b > eps;}        //a > b
bool le(ld a, ld b){return b-a > eps;}        //a < b
bool eq(ld a, ld b){return abs(a-b) <= eps;}  //a == b
bool neq(ld a, ld b){return abs(a-b) > eps;}  //a != b
struct pointGeom{
	ld x, y;
	pointGeom(): x(0), y(0){}
	pointGeom(ld x, ld y): x(x), y(y){}

	pointGeom operator+(const pointGeom & p) const{return pointGeom(x + p.x, y + p.y);}
	pointGeom operator-(const pointGeom & p) const{return pointGeom(x - p.x, y - p.y);}
	pointGeom operator*(const ld & k) const{return pointGeom(x * k, y * k);}
	pointGeom operator/(const ld & k) const{return pointGeom(x / k, y / k);}

	pointGeom operator+=(const pointGeom & p){*this = *this + p; return *this;}
	pointGeom operator-=(const pointGeom & p){*this = *this - p; return *this;}
	pointGeom operator*=(const ld & p){*this = *this * p; return *this;}
	pointGeom operator/=(const ld & p){*this = *this / p; return *this;}

	pointGeom rotate(const ld & a) const{return pointGeom(x*cos(a) - y*sin(a), x*sin(a) + y*cos(a));}
	pointGeom perp() const{return pointGeom(-y, x);}
	ld ang() const{
		ld a = atan2l(y, x); a += le(a, 0) ? 2*pi : 0; return a;
	}
	ld dot(const pointGeom & p) const{return x * p.x + y * p.y;}
	ld cross(const pointGeom & p) const{return x * p.y - y * p.x;}
	ld norm() const{return x * x + y * y;}
	ld length() const{return sqrtl(x * x + y * y);}
	pointGeom unit() const{return (*this) / length();}

	bool operator==(const pointGeom & p) const{return eq(x, p.x) && eq(y, p.y);}
	bool operator!=(const pointGeom & p) const{return !(*this == p);}
	bool operator<(const pointGeom & p) const{return le(x, p.x) || (eq(x, p.x) && le(y, p.y));}
	bool operator>(const pointGeom & p) const{return ge(x, p.x) || (eq(x, p.x) && ge(y, p.y));}
	bool half(const pointGeom & p) const{return le(p.cross(*this), 0) || (eq(p.cross(*this), 0) && le(p.dot(*this), 0));}
};

int sgn(ld x){
	if(ge(x, 0)) return 1;
	if(le(x, 0)) return -1;
	return 0;
}

bool pointGeomInLine(const pointGeom & a, const pointGeom & v, const pointGeom & p){
	//line a+tv, pointGeom p
	return eq((p - a).cross(v), 0);
}

bool pointGeomInSegment(const pointGeom & a, const pointGeom & b, const pointGeom & p){
	//segment ab, pointGeom p
	return pointGeomInLine(a, b - a, p) && leq((a - p).dot(b - p), 0);
}

int intersectSegmentsInfo(const pointGeom & a, const pointGeom & b, const pointGeom & c, const pointGeom & d){
	//segment ab, segment cd
	pointGeom v1 = b - a, v2 = d - c;
	int t = sgn(v1.cross(c - a)), u = sgn(v1.cross(d - a));
	if(t == u){
		if(t == 0){
			if(pointGeomInSegment(a, b, c) || pointGeomInSegment(a, b, d) || pointGeomInSegment(c, d, a) || pointGeomInSegment(c, d, b)){
				return -1; //infinity pointGeoms
			}else{
				return 0; //no pointGeom
			}
		}else{
			return 0; //no pointGeom
		}
	}else{
		return sgn(v2.cross(a - c)) != sgn(v2.cross(b - c)); //1: single pointGeom, 0: no pointGeom
	}
}

pointGeom intersectLines(const pointGeom & a1, const pointGeom & v1, const pointGeom & a2, const pointGeom & v2){
	//lines a1+tv1, a2+tv2
	//assuming that they intersect
	ld det = v1.cross(v2);
	return a1 + v1 * ((a2 - a1).cross(v2) / det);
}

int intersectLineSegmentInfo(const pointGeom & a, const pointGeom & v, const pointGeom & c, const pointGeom & d){
	//line a+tv, segment cd
	pointGeom v2 = d - c;
	ld det = v.cross(v2);
	if(eq(det, 0)){
		if(eq((c - a).cross(v), 0)){
			return -1; //infinity points
		}else{
			return 0; //no point
		}
	}else{
		return sgn(v.cross(c - a)) != sgn(v.cross(d - a)); //1: single point, 0: no point
	}
}

vector<pointGeom> cutPolygon(const vector<pointGeom> & P, const pointGeom & a, const pointGeom & v){
	//returns the part of the convex polygon P on the left side of line a+tv
	int n = P.size();
	vector<pointGeom> lhs;
	for(int i = 0; i < n; ++i){
		if(geq(v.cross(P[i] - a), 0)){
			lhs.push_back(P[i]);
		}
		if(intersectLineSegmentInfo(a, v, P[i], P[(i+1)%n]) == 1){
			pointGeom p = intersectLines(a, v, P[i], P[(i+1)%n] - P[i]);
			if(p != P[i] && p != P[(i+1)%n]){
				lhs.push_back(p);
			}
		}
	}
	return lhs;
}