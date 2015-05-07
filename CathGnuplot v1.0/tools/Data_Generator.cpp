//
//  Data_Generator.cpp
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
//argv[1]: dimension
//argv[2]: data file name
//argv[3]: size
{
    cout << endl;
    
    // check the arguments of main function
    if(argc < 4)
    {
        cout << "Error: Lack of arguments." << endl;
        cout << "Usage:\n\tGenerate_Data <dimension> <datafile> <size>" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "<dimension>" << endl;
        cout << "\t\tdimension of the points to be generated" << endl;
        cout << "\t\t(either 2 or 3)" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "<datafile>" << endl;
        cout << "\t\tname of the out put file" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "<size>" << endl;
        cout << "\t\tnumber of points to be generated" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "Example1:" << endl;
        cout << "\tGenerate_Data 2 data_2D.txt 10000" << endl;
        cout << "Example2:" << endl;
        cout << "\tGenerate_Data 3 data_3D.txt 20000" << endl;
        cout << "--------------------------------------------------------" << endl;
        return -1;
    }

    int dimension = atoi(argv[1]);
    string filename = argv[2];
    int size = atoi(argv[3]);
    
    cout << "Outputting " << size << " " << dimension << "D points in the file " << filename << " ..." << endl;
    
    //generate the points
    int state = generate_random_data(dimension, filename, size);
    if(state == 0)
    {
        cout << "Data generation succeeded." << endl;
    }
    else
    {
        cout << "Data generation failed." << endl;
    }
    
    cout << endl;
    
    return state;
}