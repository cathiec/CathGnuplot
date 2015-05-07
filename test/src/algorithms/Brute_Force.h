//
//  Graham_Scan.h
//  
//
//  Created by Xiao XU & Marco GUIDO on 15/4/1.
//
//

#ifndef _Brute_Force
#define _Brute_Force

#include "geometry.h"

double angle(g_2d_point a, g_2d_point o, g_2d_point b)
{
    g_vector oa = a - o, ob = b - o;
    return acos(dot(oa, ob) / sqrt(norm(oa) * norm(ob)));
}

//test if point is in polygon
bool is_inside(g_2d_point p, const vector<g_2d_point> &poly)
{
    if((int)poly.size() == 0) return false;
    double sum = 0;
    for(int i = 0;i < (int)poly.size() - 1;i++)
    {
        if(is_on_the_left(poly[i + 1], p, poly[i]))
            sum += angle(poly[i], p, poly[i + 1]);
        else sum -= angle(poly[i], p, poly[i + 1]);
    }
    return (fabs(fabs(sum) - 2 * PI) < EPS);
}


vector<g_2d_point> Brute_Force(vector<g_2d_point> P)
{
    int n = (int)P.size();
    vector<g_2d_point> S;
    for(int i = 0; i < n; i++) S.push_back(P[i]);
    for (int a=0 ; a<n; a++)
    {
        for (int b=0 ; b<n && b<a; b++)
        {
            for (int c=0 ; c<n && c<b; c++)
            {
                vector<g_2d_point> S_prime;
                S_prime.push_back(P[a]);
                S_prime.push_back(P[b]);
                S_prime.push_back(P[c]);
                S_prime.push_back(g_2d_point(P[a].x, P[a].y));
                bool ok = true;
                int d = 0;
                while(d < S.size())
                {
                    if (is_inside(S[d], S_prime))
                        S.erase(S.begin()+d);
                    else
                        d++;
                }

            }
        }        
    }
    return S;
}

#endif
