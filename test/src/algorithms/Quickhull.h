//
//  Graham_Scan.h
//  
//
//  Created by Xiao XU & Marco GUIDO on 15/4/1.
//
//

#ifndef _Quickhull
#define _Quickhull
#define LEFT 0
#define RIGHT 1

#include "geometry.h"



double distance_to_line(g_2d_point p, g_2d_point a, g_2d_point b)
{
    g_vector ap = p - a, ab = b - a;
    double u = dot(ap, ab) / norm(ab);
    g_2d_point c = a + ab * u;
    return distance_2_points(p, c);
}


void Joint_side(vector<g_2d_point> P, vector<g_2d_point> &S, g_2d_point begin, g_2d_point end, bool side)
{
    /*cout << "(" << begin.x << "; " << begin.y << "); ";
    cout << endl;
    cout << "(" << end.x << "; " << end.y << "); ";
    cout << endl;*/

    g_2d_point candidate = -1;
    double distance = -1;

    for (int i=0; i< P.size(); i++)
    {
        if (!(are_collinear(P[i], begin, end)) && is_on_the_left(P[i], begin, end) == side)
        {
            double new_distance = distance_to_line(P[i], begin, end);
            if (new_distance > distance)
            {
                distance = new_distance;
                candidate = P[i];
            }
        }
    }
    if (! (candidate == -1))
    {
        S.push_back(candidate);
        //cout << "(" << candidate.x << "; " << candidate.y << "); ";
        //cout << "---------------" << endl;
        Joint_side(P, S, begin, candidate, side);
        Joint_side(P, S, candidate, end, side);
    }
}

vector<g_2d_point> Quickhull(vector<g_2d_point> P)
{
    int i, j, n = (int)P.size();
    if(n <= 3)
    {
        if(!(P[0] == P[n - 1]))
            P.push_back(P[0]);
        return P;
    }
    int Pinit = 0;
    for(int i = 1; i < n; i++)
        if(P[i].x < P[Pinit].x || (P[i].x == P[Pinit].x && P[i].y > P[Pinit].y))
            Pinit = i;
    int Pend = 0;
    for(int i = 1; i < n; i++)
        if(P[i].x > P[Pend].x || (P[i].x == P[Pend].x && P[i].y > P[Pend].y))
            Pend = i;

    vector<g_2d_point> S;
    S.push_back(P[Pinit]);
    S.push_back(P[Pend]);

    Joint_side(P, S, P[Pinit], P[Pend], true);
    Joint_side(P, S, P[Pinit], P[Pend], false);
    /*for (int z=0; z< S.size(); z++)
    {
        cout << "(" << S[z].x << "; " << S[z].y << "); ";
        cout << endl;
    }*/
    return S;
}

bool Quicktest()
{
    return true;
}
#endif
