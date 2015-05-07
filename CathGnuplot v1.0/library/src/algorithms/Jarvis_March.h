//
//  Graham_Scan.h
//  
//
//  Created by Xiao XU & Marco GUIDO on 15/4/1.
//
//

#ifndef _Jarvis_March
#define _Jarvis_March

#include "geometry.h"


g_2d_point jarvis_pivot(0, 0);

bool jarvis_angle_compare(g_2d_point a, g_2d_point b)
{
    if(are_collinear(b, jarvis_pivot, a))
        return distance_2_points(jarvis_pivot, a) > distance_2_points(jarvis_pivot, b);
    double d1x = a.x - jarvis_pivot.x, d1y = a.y - jarvis_pivot.y, d2x = b.x - jarvis_pivot.x, d2y = b.y - jarvis_pivot.y;
    return (atan2(d1y, d1x) - atan2(d2y, d2x)) > 0;
}

vector<g_2d_point> Jarvis_March(vector<g_2d_point> P)
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
    vector<g_2d_point> S;
    S.push_back(P[P0]);
    g_2d_point first_line = g_2d_point(P[0].x +1, P[0].y);
        /*for (int p=0; p< P.size(); p++)
        {
            cout << "(" << P[p].x << "; " << P[p].y << "); ";
            cout << endl;
        }*/
    while(true)
    {
        jarvis_pivot.x = S[S.size() -1].x;
        jarvis_pivot.y = S[S.size() -1].y;
        g_2d_point candidate = -1;
        for (int i=0; i< n; i++)
        {
            if (! (P[i] == S[S.size()-1]))
            {
                g_2d_point to_compare;
                if (S.size() == 1)to_compare = first_line;
                else to_compare = S[S.size() -2]; 
                //if (! (candidate == -1)) cout << "cpr : (" << P[i].x << "; " << P[i].y << "); " << is_on_the_left(candidate, P[i], jarvis_pivot)  << endl;
                //else cout << "cpr : (" << P[i].x << "; " << P[i].y << "); " << 1 << endl;
                if ( (candidate == -1) ||
                    is_on_the_left(candidate, P[i], jarvis_pivot)) candidate = P[i];
            }
        }
        /*for (int z=0; z< S.size(); z++)
        {
            cout << "(" << S[z].x << "; " << S[z].y << "); ";
            cout << endl;
        }*/
        if (S.size()== 1 || ! (S[0] == candidate))
        {
            //cout << "putted" << endl;
            S.push_back(candidate);
        }
        else
        {
            //cout << "ended" << endl;
            break;            
        }
    }
    return S;
}

#endif
