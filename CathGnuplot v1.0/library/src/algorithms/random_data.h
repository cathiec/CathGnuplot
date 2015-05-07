//
//  Graham_Scan.h
//
//
//  Created by Xiao XU on 15/4/1.
//
//

#ifndef _random_data
#define _random_data

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <cmath>

// generate a random data set with given size in given dimension 
int generate_random_data(int dimension, std::string filename, int size)
{
    if((dimension != 2) && (dimension != 3))
    {
        std::cout << std::endl << "Error: Dimension must be either 2 or 3." << std::endl << std::endl;
        return 1;
    }
    std::ofstream data;
    data.open(filename);
    if(!data)
    {
        std::cout << std::endl << "Error: Cannot create the data file." << std::endl << std::endl;
        return 2;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0,3);
    for(int i = 0; i < size; i++)
    {
        data << d(gen);
        data << " ";
        data << d(gen);
        if(dimension == 3)
        {
            data << " ";
            data << d(gen);
        }
        data << std::endl;
    }
    data.close();
    return 0;
}

#endif