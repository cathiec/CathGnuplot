//
//  Convex_Hull.cpp
//  
//
//  Created by Xiao XU on 15/4/10.
//
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <string.h>
#include "cath_gnuplot.h"

using namespace std;

int main(int argc, char **argv)
//argv[1]: dimension and display
//argv[2]: data file name
//argv[3]: algo name
{
    cout << endl;
    
    // check the arguments of main function
    if(argc < 3)
    {
        cout << "Error: Lack of arguments." << endl;
        cout << "Usage:\n\tConvex_Hull <mode> <datafile> [algo for 2D]" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "<mode>" << endl;
        cout << "\t\t2  :\t2D without display" << endl;
        cout << "\t\t2+ :\t2D with display" << endl;
        cout << "\t\t3  :\t3D without display" << endl;
        cout << "\t\t3+ :\t3D with display" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "<datafile>" << endl;
        cout << "\t\tname of the file containing the data" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "[algo for 2D] optional for 2D" << endl;
        cout << "This argument is only for 2D." << endl;
        cout << "\t\tGraham_Scan(default for 2D)" << endl;
        cout << "\t\tBrute_Force" << endl;
        cout << "\t\tQuickhull" << endl;
        cout << "\t\tJarvis_March" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "Example1:" << endl;
        cout << "\tConvex_Hull 2+ data.txt Brute_Force" << endl;
        cout << "Example2:" << endl;
        cout << "\tConvex_Hull 3 data.txt" << endl;
        cout << "--------------------------------------------------------" << endl;
        return -1;
    }
    
    // dimension and display
    bool display = false;
    int dimension;
    string d = argv[1];
    if(d[0] == '2')
        dimension = 2;
    else if(d[0] == '3')
        dimension = 3;
    else
    {
        cout << endl << "Error: Dimension must be either 2 or 3." << endl << endl;
        return 1;
    }
    if(d[1] == '+')
        display = true;
    cout << "- Dimension:\t" << dimension << endl;
    cout << "- Display:\t" << (display ? "Yes" : "No") << endl;
    
    // data file
    string filename = argv[2];
    ifstream data;
    data.open(filename);
    if(!data)
    {
        cout << endl << "Error: Cannot find the data file." << endl << endl;
        return 2;
    }
    data.close();
    cout << "- Input Data:\t" << filename << endl;
    
    // algo
    int algo = 1;
    if(argc > 3)
    {
        if(strcmp(argv[3], "Graham_Scan") == 0)
            algo = 1;
        else if(strcmp(argv[3], "Brute_Force") == 0)
            algo = 2;
        else if(strcmp(argv[3], "Quickhull") == 0)
            algo = 3;
        else if(strcmp(argv[3], "Jarvis_March") == 0)
            algo = 4;
        else
        {
            cout << endl << "Error: Unkown Algo." << endl << endl;
            return 3;
        }
    }
    cout << "- Algo:\t\t";
    if(dimension == 2)
    {
        switch(algo)
        {
            case 1:
                cout << "Graham_Scan";
                break;
            case 2:
                cout << "Brute_Force";
                break;
            case 3:
                cout << "Quickhull";
                break;
            case 4:
                cout << "Jarvis_March";
                break;
            default: break;
        }
        
    }
    else
    {
        cout << "Incremental Algo";
    }
    cout << endl;
    
    // read data
    g_set_2d s2d, cv2d;
    g_set_3d s3d, cv3d;
    if(dimension == 2)
        s2d = read_2D_set_from_file(filename);
    else
        s3d = read_3D_set_from_file(filename);
    
    // calculate convex hull
    if(dimension == 2)
    {
        switch(algo)
        {
            case 1:
                cv2d = Graham_Scan(s2d);
                break;
            case 2:
                cv2d = Brute_Force(s2d);
                break;
            case 3:
                cv2d = Quickhull(s2d);
                break;
            case 4:
                cv2d = Jarvis_March(s2d);
                break;
            default: break;
        }
    }
    else
        cv3d = Convex_Hull_3D(s3d);
    
    // display
    if(display)
    {
        cout << endl;
        g_init;
        g_canvas c;
        if(dimension == 2)
        {
            c.compare_2d_points(s2d, cv2d, "original", "convex hull");
            c.save_2d_points_to_eps("result", cv2d, "convex hull");
        }
        else
        {
            c.compare_3d_points(s3d, cv3d, "original", "convex hull");
            c.save_3d_points_to_eps("result", cv3d, "convex hull");
        }
        g_end;
    }
    else
    {
        if(dimension == 2)
            save_2D_set_to_file(cv2d, "result.txt");
        else
            save_3D_set_to_file(cv3d, "result.txt");
    }
    
    cout << endl;
    
    return 0;
}