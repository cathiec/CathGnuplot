//
//  3D_Convex_Hull.h
//  
//
//  Created by Xiao XU & Marco GUIDO on 15/4/1.
//
//

#ifndef _3D_Convex_Hull
#define _3D_Convex_Hull

#include "geometry.h"

using namespace std;

struct g_face
{
    list<int> pos;
    g_3d_point v;  //normal
    double d; //length
};

struct convex3d
{
    g_3d_point *ps;       //set of points
    int pn;               //number of points
    list<g_face> convex;  //result containing the facets
    
    int **fsign;
    
    convex3d(int np)
    {
        ps = new g_3d_point[np];
        fsign = new int *[np];
        for(int i = 0; i < np; i++)
        {
            *(fsign + i) = new int[np];
        }
    }
    
    ~convex3d()
    {
        delete [] ps;
        for(int i = 0; i < pn; i++)
        {
            delete [] *(fsign + i);
        }
        delete [] fsign;
    }
    
    // add a facet
    void addFace(int a, int b, int c){
        g_face tf;
        tf.pos.push_back(a);
        tf.pos.push_back(b);
        tf.pos.push_back(c);
        tf.v = xmul3d(ps[a], ps[b], ps[c]);
        tf.d = -dmul3d(ps[a], tf.v);
        convex.push_back(tf);
    }
    // we process the facet when adding a new point
    int handleFace(g_face f, int i){
        int s = sign(dmul3d(f.v, ps[i]) + f.d);
        liit now, nxt;
        now = f.pos.begin();
        nxt = f.pos.begin();
        nxt++;
        for(; nxt != f.pos.end(); nxt++, now++){
            fsign[*now][*nxt] = s;
        }
        fsign[*now][*f.pos.begin()] = s;
        return s;
    }
    // determine if the point is on the facet
    bool inFace(g_face f, int i){
        liit now, nxt;
        int pn, nn, s;
        now = f.pos.begin();
        nxt = f.pos.begin();
        nxt++;
        for(pn = nn = 0; nxt != f.pos.end(); nxt++, now++){
            s = sign(dmul3d(xmul3d(ps[*now], ps[*nxt], ps[i]), f.v));
            if(s == 1) pn++;
            else if(s == -1) nn++;
        }
        s = sign(dmul3d(xmul3d(ps[*now], ps[*f.pos.begin()], ps[i]), f.v));
        if(s == 1) pn++;
        else if(s == -1) nn++;
        if(pn >= 1 && nn >= 1) return false;
        return true;
    }
    // extend/delete the facet when adding a new point
    bool extFace(g_face& f, int i){
        liit now, nxt;
        bool flag = false;
        now = f.pos.begin();
        nxt = f.pos.begin();
        nxt++;
        if(fsign[*now][*nxt] == 0){
            list<int> tpos;
            while(true){
                if(sign(dmul3d(xmul3d(ps[i], ps[*now], ps[*nxt]), f.v)) >= 1){
                    break;
                }
                now++;
                if(now == f.pos.end()) now = f.pos.begin();
                nxt++;
                if(nxt == f.pos.end()) nxt = f.pos.begin();
            }
            tpos.push_back(*now);
            int st = *now;
            while(*nxt != st){
                if(sign(dmul3d(xmul3d(ps[*now], ps[i], ps[*nxt]), f.v)) >= 0){
                    break;
                }
                tpos.push_back(*nxt);
                now++;
                if(now == f.pos.end()) now = f.pos.begin();
                nxt++;
                if(nxt == f.pos.end()) nxt = f.pos.begin();
            }
            tpos.push_back(i);
            while(true){
                now++;
                if(now == f.pos.end()) now = f.pos.begin();
                nxt++;
                if(nxt == f.pos.end()) nxt = f.pos.begin();
                if(*now == st) break;
                if(sign(dmul3d(xmul3d(ps[i], ps[*now], ps[*nxt]), f.v)) >= 1){
                    tpos.push_back(*now);
                }
            }
            f.pos = tpos;
            
        }else if(fsign[*now][*nxt] > 0){
            for(; nxt != f.pos.end(); now++, nxt++){
                if(fsign[*nxt][*now] < 0){
                    addFace(*now, *nxt, i);
                }
            }
            if(fsign[*f.pos.begin()][*now] < 0){
                addFace(*now, *f.pos.begin(), i);
            }
            flag = true;
        }
        return flag;
    }
    //calcule the convex hull
    void calConvex(){
        sort(ps, ps+pn, cmp);
        pn = unique(ps, ps+pn) - ps;
        convex.clear();
        if(pn <= 2)
            return;
        int a, b, c;
        double ab, bc, ac;
        a = 0;
        b = 1;
        for(c = 2; c < pn; c++)
        {
            ab = dist3d(ps[a], ps[b]);
            bc = dist3d(ps[b], ps[c]);
            ac = dist3d(ps[a], ps[c]);
            if(sign(ab+bc-ac) == 0){
                b = c;
            }else if(sign(ab+ac-bc) == 0){
                a = c;
            }else if(sign(ac+bc-ab) != 0){
                break;
            }
        }
        if(c == pn)
            return;
        int i, size, j;
        list<g_face>::iterator it;
        addFace(a, b, c);
        addFace(a, c, b);
        for(i = c + 1; i < pn; i++)
        {
            size = convex.size();
            for(it = convex.begin(), j = 0; j < size; j++, it++)
                if(handleFace(*it, i) == 0 && inFace(*it, i))
                    break;
            if(j < size)
                continue;
            for(it = convex.begin(), j = 0; j < size; j++)
                if(extFace(*it, i))
                    it = convex.erase(it);
                else
                    it++;
        }
    }
};

// calculate 3D convex hull of a set of 3D points
vector<g_3d_point> Convex_Hull_3D(vector<g_3d_point> p)
{
    convex3d cv3(p.size());
    vector<g_3d_point> result;
    int i;
    for(i = 0; i < p.size(); i++)
        cv3.ps[i] = p[i];
    cv3.pn = p.size();
    cv3.calConvex();
    list<g_face>::iterator it;
    liit j;
    g_3d_point x;
    for(it = cv3.convex.begin(); it != cv3.convex.end(); it++)
        for(j = (it->pos).begin(); j != (it->pos).end(); j++)
        {
            x = cv3.ps[*j];
            result.push_back(g_3d_point(x));
        }
    return result;
}

#endif