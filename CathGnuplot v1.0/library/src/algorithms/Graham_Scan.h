//
//  Graham_Scan.h
//  
//
//  Created by Xiao XU & Marco GUIDO on 15/4/1.
//
//

#ifndef _Graham_Scan
#define _Graham_Scan

#include "geometry.h"
#include <vector>

g_2d_point pivot(0, 0);

bool angle_compare(g_2d_point a, g_2d_point b)
{
    if(are_collinear(b, pivot, a))
        return distance_2_points(pivot, a) < distance_2_points(pivot, b);
    double d1x = a.x - pivot.x, d1y = a.y - pivot.y, d2x = b.x - pivot.x, d2y = b.y - pivot.y;
    return (atan2(d1y, d1x) - atan2(d2y, d2x)) > 0;
}

std::vector<g_2d_point> Graham_Scan(std::vector<g_2d_point> P)
{
    int i, j, n = (int)P.size();
    if(n <= 3)
    {
        if(!(P[0] == P[n - 1]))
            P.push_back(P[0]);
        return P;
    }
    int P0 = 0;
    for(int i = 1; i < n; i++)
        if(P[i].y < P[P0].y || (P[i].y == P[P0].y && P[i].x > P[P0].x))
            P0 = i;
    g_2d_point temp = P[0];
    P[0] = P[P0];
    P[P0] = temp;
    pivot = P[0];
    sort(++P.begin(), P.end(), angle_compare);
    std::vector<g_2d_point> S;
    S.push_back(P[n - 1]);
    S.push_back(P[0]);
    S.push_back(P[1]);
    i = 2;
    while(i < n)
    {
        j = (int)S.size() - 1;
        if(is_on_the_left(P[i], S[j], S[j - 1]))
            S.push_back(P[i++]);
        else
            S.pop_back();
    }
    return S;
}

#endif
