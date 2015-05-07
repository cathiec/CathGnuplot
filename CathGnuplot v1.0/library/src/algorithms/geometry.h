//
//  Geometry.h
//  
//
//  Created by Xiao XU & Marco GUIDO on 15/4/1.
//
//

#ifndef _geometry
#define _geometry

#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define PI acos(-1)
#define EPS 1E-9

/////////////////////////////////////////////////////////////////////////////////////////////
//      2D
/////////////////////////////////////////////////////////////////////////////////////////////
typedef class g_2d_point
{

public:
    
    double x, y;
    
    g_2d_point(double _x = 0, double _y = 0):x(_x), y(_y) {}
    
    g_2d_point(const g_2d_point &other):x(other.x), y(other.y) {}
    
    bool operator == (g_2d_point other) const
    {
        return (fabs(x - other.x) < EPS) && (fabs(y - other.y) < EPS);
    }
    
    double get_x() const
    {
        return x;
    }
    
    double get_y() const
    {
        return y;
    }
    
    double get_z() const
    {
        return 0;
    }
    
}g_vector;
//g_vector(AB) = g_2d_point(B) - g_2d_point(A)

typedef vector<g_2d_point> g_set_2d;

g_vector operator+(g_vector  v1, g_vector  v2)
{
    return g_vector(v1.x + v2.x, v1.y + v2.y);
}

g_vector operator-(g_vector  v1, g_vector  v2)
{
    return g_vector(v1.x - v2.x, v1.y - v2.y);
}

g_vector operator*(g_vector  v, double p)
{
    return g_vector(v.x * p, v.y * p);
}

g_vector operator/(g_vector  v, double p)
{
    return g_vector(v.x / p, v.y / p);
}

double cross(g_vector v1, g_vector v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

double distance_2_points(g_2d_point p1, g_2d_point p2)
{
    return hypot(p1.x - p2.x, p1.y - p2.y);
}

bool are_collinear(g_2d_point p, g_2d_point a, g_2d_point b)
{
    return fabs(cross(b - a, p - a)) < EPS;
}

bool is_on_the_left(g_2d_point p, g_2d_point a, g_2d_point b)
{
    if(are_collinear(p, a, b))
        return (distance_2_points(p, b) - distance_2_points(a, b) < 0);
    else
        return cross(b - a, p - a) > 0;
}

double norm(g_vector v) {return v.x * v.x + v.y * v.y;}

double dot(g_vector v1, g_vector v2) {return v1.x * v2.x + v1.y * v2.y;}
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      3D
/////////////////////////////////////////////////////////////////////////////////////////////
typedef list<int>::iterator liit;

inline int sign(double d)
{
    if(d < -EPS)
        return -1;
    return (d > EPS) ? 1 : 0;
}

class g_3d_point
{
    
public:
    
    double x, y, z;
    
    g_3d_point(double _x = 0, double _y = 0, double _z = 0):x(_x), y(_y), z(_z) {}
    
    g_3d_point(const g_3d_point &other):x(other.x), y(other.y), z(other.z) {}
    
    void read()
    {
        scanf("%lf%lf%lf", &x, &y, &z);
    }
    
    bool operator==(const g_3d_point& other) const
    {
        return (!sign(x - other.x)) && (!sign(y - other.y)) && (!sign(z - other.z));
    }
    
    bool isOrg() const
    {
        return !sign(x) && !sign(y) && !sign(z);
    }
    
    g_3d_point operator-(g_3d_point other) const
    {
        return g_3d_point(x - other.x, y - other.y, z - other.z);
    }
    
    double get_x() const
    {
        return x;
    }
    
    double get_y() const
    {
        return y;
    }
    
    double get_z() const
    {
        return z;
    }
};

typedef vector<g_3d_point> g_set_3d;

bool cmp(g_3d_point p1, g_3d_point p2)
{
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y) || (p1.x == p2.x && p1.y == p2.y && p1.z < p2.z);
}

//向量st-->ed1和st-->ed2的叉积
g_3d_point xmul3d(g_3d_point st, g_3d_point ed1, g_3d_point ed2)
{
    ed1 = ed1 - st;
    ed2 = ed2 - st;
    return g_3d_point(ed1.y * ed2.z - ed2.y * ed1.z, ed1.z * ed2.x - ed2.z * ed1.x, ed1.x * ed2.y - ed2.x * ed1.y);
}

//向量(0,0)-->p1和(0,0)-->ed2的叉积
double dmul3d(g_3d_point p1, g_3d_point p2)
{
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

double dist3d(g_3d_point p1, g_3d_point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
/////////////////////////////////////////////////////////////////////////////////////////////

#endif
