//
//  cath_gnuplot.h
//  
//
//  Created by 徐笑 Xiao XU on 15/5/1.
//
//

#ifndef cath_gnuplot_h
#define cath_gnuplot_h

#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

#include "src/algorithms/random_data.h"

#include "src/algorithms/3D_Convex_Hull.h"
#include "src/algorithms/Graham_Scan.h"
#include "src/algorithms/Brute_Force.h"
#include "src/algorithms/Jarvis_March.h"
#include "src/algorithms/Quickhull.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//      convert a number to a string
/////////////////////////////////////////////////////////////////////////////////////////////
// double -> string
std::string number_to_string(double number)
{
    char temp1[256];
    sprintf(temp1, "%lf", number);
    std::string temp2 = temp1;
    return temp2;
}

// int -> string
std::string number_to_string(int number)

{
    char temp1[256];
    sprintf(temp1, "%d", number);
    std::string temp2 = temp1;
    return temp2;
}
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      timing and waiting
/////////////////////////////////////////////////////////////////////////////////////////////
void g_wait(int sec)
{
    double act = 1.0 / sec;
    for(int i = 1; i <= sec; i++)
    {
        sleep(1);
        std::cout << "\r\tPlotting... " << int(act * i * 100) << " %";
        fflush(stdout);
    }
    std::cout << "\tFinished..." << std::endl;
    //fflush(stdout);
}
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      read/save a set of points from/to a file
/////////////////////////////////////////////////////////////////////////////////////////////
// read a set of 2D points from a file
std::vector<g_2d_point> read_2D_set_from_file(std::string file_path)
{
    std::ifstream file;
    file.open(file_path);
    double x, y;
    std::vector<g_2d_point> s;
    while(true)
    {
        file >> x >> y;
        if(file.eof())
            break;
        s.push_back(g_2d_point(x, y));
    }
    file.close();
    return s;
}

// read a set of 3D points from a file
std::vector<g_3d_point> read_3D_set_from_file(std::string file_path)
{
    std::ifstream file;
    file.open(file_path);
    double x, y, z;
    std::vector<g_3d_point> s;
    while(true)
    {
        file >> x >> y >> z;
        if(file.eof())
            break;
        s.push_back(g_3d_point(x, y, z));
    }
    file.close();
    return s;
}

// save a set of 2D points to a file
void save_2D_set_to_file(std::vector<g_2d_point> s, std::string file_path)
{
    std::ofstream file;
    file.open(file_path);
    for(int i = 0; i < s.size(); i++)
        file << s[i].x << " " << s[i].y << endl;
    file.close();
}

// save a set of 3D points to a file
void save_3D_set_to_file(std::vector<g_3d_point> s, std::string file_path)
{
    std::ofstream file;
    file.open(file_path);
    for(int i = 0; i < s.size(); i++)
        file << s[i].x << " " << s[i].y << " " << s[i].z << endl;
    file.close();
}
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      Gnuplot path
/////////////////////////////////////////////////////////////////////////////////////////////
// Gnuplot path in Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
std::string m_sGNUPlotFileName = "pgnuplot.exe";
std::string m_sGNUPlotPath = "C:/program files/gnuplot/bin";
// Gnuplot path in Linux/IOS
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
std::string m_sGNUPlotFileName = "gnuplot";
std::string m_sGNUPlotPath = "/usr/local/bin";
#endif

// Gnuplot default terminal in Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
std::string terminal_std = "windows";
// Gnuplot default terminal in Linux
#elif ( defined(unix) || defined(__unix) || defined(__unix__) ) && !defined(__APPLE__)
std::string terminal_std = "wxt";
// Gnuplot default terminal in IOS
#elif defined(__APPLE__)
std::string terminal_std = "aqua";
#endif
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      these 3 functions are used for searching Gnuplot path
/////////////////////////////////////////////////////////////////////////////////////////////
template <typename Container>
void stringtok (Container &container, std::string const &in, const char * const delimiters = " \t\n")
{
    const std::string::size_type len = in.length();
    std::string::size_type i = 0;
    while ( i < len )
    {
        i = in.find_first_not_of (delimiters, i);
        if (i == std::string::npos)
            return;
        std::string::size_type j = in.find_first_of (delimiters, i);
        if (j == std::string::npos)
        {
            container.push_back (in.substr(i));
            return;
        }
        else
            container.push_back (in.substr(i, j - i));
        i = j + 1;
    }
    return;
}

bool file_exists(const std::string &filename, int mode)
{
    if ( mode < 0 || mode > 7)
    {
        std::cout << "\tIn function \"Gnuplot::file_exists\": mode has to be an integer between 0 and 7" << endl;
        return false;
    }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if (_access(filename.c_str(), mode) == 0)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        if (access(filename.c_str(), mode) == 0)
#endif
            return true;
        else
            return false;
    
}

bool get_program_path()
{
    std::string tmp = m_sGNUPlotPath + "/" + m_sGNUPlotFileName;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if(file_exists(tmp, 0))
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if(file_exists(tmp, 1))
#endif
        return true;
    char *path;
    path = getenv("PATH");
    if (path == NULL)
    {
        std::cout << "Path is not set" << std::endl;
        return false;
    }
    else
    {
        std::vector<std::string> ls;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
        stringtok(ls,path,";");
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        stringtok(ls,path,":");
#endif
        for(std::vector<std::string>::const_iterator i = ls.begin(); i != ls.end(); ++i)
        {
            tmp = (*i) + "/" + m_sGNUPlotFileName;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
            if(file_exists(tmp, 0)) // check existence
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
            if(file_exists(tmp, 1)) // check existence and execution permission
#endif      
            {
                m_sGNUPlotPath = *i; // set m_sGNUPlotPath
                return true;
            }
        }
        tmp = "\tCan't find gnuplot neither in PATH nor in \"" + m_sGNUPlotPath + "\"";
        std::cout << tmp << std::endl;
        m_sGNUPlotPath = "";
        return false;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      controller containing a pipe to Gnuplot
/////////////////////////////////////////////////////////////////////////////////////////////
class g_controller
{
    
private:
    
    FILE *gp;
    
public:
    
    // constructor
    // create a pipe in constructor
    g_controller()
    {
        std::string g_path = m_sGNUPlotPath + "/" + m_sGNUPlotFileName;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
        gp = _popen(g_path.c_str(),"w");
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        gp = popen(g_path.c_str(),"w");
#endif
        if(gp == NULL)
        {
            fprintf(stderr, "\tError: Can't find %s.", g_path.c_str());
            exit(EXIT_FAILURE);
        }
    }
    
    // destructor
    // close the pipe in destructor
    ~g_controller()
    {
        //sleep(1);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
        if(_pclose(gp) == -1)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        if(pclose(gp) == -1)
#endif
            std::cout << "Error: Problem closing communication to gnuplot" << std::endl;
    }
    
    // execute an operation in Gnuplot command-line
    void execute(std::string gcmd)
    {
        fputs((gcmd + "\n").c_str(), gp);
        fflush(gp);
    }
    
    void execute(const char *gcmd)
    {
        execute(std::string(gcmd));
    }
    
};
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
//      a Gnuplot canvas
/////////////////////////////////////////////////////////////////////////////////////////////
static int g_canvas_counter = 0;

static int delay_for_plotting = 4;

class g_canvas
{
    
private:
    
    // graph number
    int number;
    
    // gnuplot controller
    g_controller ctr;
    
    // data size
    int data_size;
    
    // path to temporary file
    std::string temp_file_path;
    
    // plotting mode
    std::string plot_mode;
    
    // line color
    std::string line_color;
    
    // line width
    std::string line_width;
    
    // point type
    std::string point_type;
    
    // point size
    std::string point_size;
    
    // plot a graph according to a file containing a set of points
    void plot_points(const std::string file_path, int dimension)
    {
        if((dimension != 2) && (dimension != 3))
            std::cout << "\tError: Dimension error." << endl;
        else
        {
            std::string str_plot;
            if(dimension == 2)
                str_plot = "plot \"";
            else
                str_plot = "splot \"";
            std::cout << ">> Canvas " << number << ":" << std::endl;
            std::string gcmd;
            gcmd = str_plot + file_path + "\" w " + plot_mode + " lc " + line_color + " lw " + line_width + " pt " + point_type + " ps " + point_size;
            execute(gcmd);
            g_wait(delay_for_plotting);
        }
    }
    
    // plot a graph according to a set of points
    template<class T>
    void plot_points(const std::vector<T> s, int dimension)
    {
        if((dimension != 2) && (dimension != 3))
            std::cout << "\tError: Dimension error." << endl;
        else
        {
            std::ofstream temp_file;
            temp_file.open(temp_file_path);
            for(int i = 0; i < s.size(); i++)
                if(dimension == 2)
                    temp_file << s[i].x << " " << s[i].y << std::endl;
                else
                    temp_file << s[i].x << " " << s[i].y << " " << s[i].get_z() << std::endl;
            temp_file.close();
            plot_points(temp_file_path, dimension);
        }
    }
    
    // plot two graphes in the same canvas in order to compare two sets of points
    // we can give a title for each sub-graph
    template<class T1, class T2>
    void compare_points(const T1 s1, const T2 s2, int dimension, std::string title1 = "", std::string title2 = "")
    {
        execute("set multiplot");
        execute("set size 1, 0.5");
        execute("set origin 0, 0");
        if(title1 != "")
            set_title(title1);
        plot_points(s1, dimension);
        execute("set size 1, 0.5");
        execute("set origin 0, 0.5");
        if(title2 != "")
            set_title(title2);
        plot_points(s2, dimension);
        execute("unset multiplot");
        reset_origin();
        reset_size();
        reset_title();
    }
    
    // save a graph representing a set of points in an eps file
    // we can give a title
    template<class T>
    void save_points_to_eps(std::string file_name, T s, int dimension, std::string title = "")
    {
        if((dimension != 2) && (dimension != 3))
            std::cout << "\tError: Dimension error." << endl;
        else
        {
            std::string gcmd;
            gcmd = "set terminal postscript color";
            execute(gcmd);
            gcmd = "set output \"" + file_name + ".eps\"";
            execute(gcmd);
            if(title != "")
                set_title(title);
            plot_points(s, dimension);
            std::cout << "\tFile created:" << "\t\t" << file_name << ".eps" << endl;
            reset_title();
            reset_terminal();
            reset_output();
        }
    }
    
public:
    
    // constructor
    g_canvas():number(g_canvas_counter++)
    {
        temp_file_path = "src/temp/" + number_to_string(number) + ".gtemp";
        std::ofstream temp_file;
        temp_file.open(temp_file_path);
        temp_file.close();
        reset();
    }
    
    // destructor
    ~g_canvas()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        std::string sys_cmd = "rm " + temp_file_path;
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
        std::string sys_cmd = "del " + temp_file_path;
#endif
        system(sys_cmd.c_str());
        //std::cout << temp_file_path << " deleted" << std::endl;
    }
    
    // execute an operation
    void execute(std::string gcmd)
    {
        ctr.execute(gcmd);
    }
    void execute(const char *gcmd)
    {
        ctr.execute(std::string(gcmd));
    }
    
    // reset graph origin to default value
    void reset_origin()
    {
        execute("set origin");
    }
    
    // reset graph size to default value
    void reset_size()
    {
        execute("set size");
    }
    
    // reset terminal to default value
    void reset_terminal()
    {
        std::string gcmd;
        gcmd = "set terminal " + terminal_std;
        execute(gcmd);
    }
    
    // reset title to default value
    void reset_title()
    {
        std::string gcmd;
        gcmd = "set title \"Canvas " + number_to_string(number) + "\"";
        execute(gcmd);
    }
    
    // reset output to default value
    void reset_output()
    {
        execute("set output");
    }
    
    // reset the canvas
    void reset()
    {
        execute("reset");
        data_size = 0;
        execute("clear");
        plot_mode = "points";
        line_color = "3";
        line_width = "2";
        point_type = "7";
        point_size = "0.5";
        execute("unset key");
        reset_title();
        reset_terminal();
        reset_output();
        execute("set xlabel \"X\"");
        execute("set ylabel \"Y\"");
    }
    
    // set the title
    void set_title(std::string title)
    {
        std::string gcmd;
        gcmd = "set title \"" + title + "\"";
        execute(gcmd);
    }
    
    // clean the canvas
    void clean()
    {
        execute("clear");
    }
    
    // replot the last graph
    void replot()
    {
        execute("replot");
    }
    
    // plot a 2D graph according to a file containing a set of 2D points
    void plot_2d_points(const std::string file_path)
    {
        plot_points(file_path, 2);
    }
    
    // plot a 2D graph according to a set of 2D points
    void plot_2d_points(const std::vector<g_2d_point> s)
    {
        plot_points(s, 2);
    }
    
    // plot two 2D graphes in the same canvas in order to compare two sets of 2D points
    // we can give a title for each sub-graph
    template<class T1, class T2>
    void compare_2d_points(const T1 s1, const T2 s2, std::string title1 = "", std::string title2 = "")
    {
        compare_points(s1, s2, 2, title1, title2);
    }
    
    // save a 2D graph representing a set of 2D points in an eps file
    // we can give a title
    template<class T>
    void save_2d_points_to_eps(std::string file_name, T s, std::string title = "")
    {
        save_points_to_eps(file_name, s, 2, title);
    }
    
    // plot a 3D graph according to a file containing a set of 3D points
    void plot_3d_points(const std::string file_path)
    {
        plot_points(file_path, 3);
    }
    
    // plot a 3D graph according to a set of 3D points
    void plot_3d_points(const std::vector<g_3d_point> s)
    {
        plot_points(s, 3);
    }
    
    // plot two 3D graphes in the same canvas in order to compare two sets of 3D points
    // we can give a title for each sub-graph
    template<class T1, class T2>
    void compare_3d_points(const T1 s1, const T2 s2, std::string title1 = "", std::string title2 = "")
    {
        compare_points(s1, s2, 3, title1, title2);
    }
    
    // save a 3D graph representing a set of 3D points in an eps file
    // we can give a title
    template<class T>
    void save_3d_points_to_eps(std::string file_name, T s, std::string title = "")
    {
        save_points_to_eps(file_name, s, 3, title);
    }
    
};
/////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////
// initialization of cath_gnuplot
/////////////////////////////////////////////////////////////////////////////////////////////
bool find_gnuplot()
{
    std::cout << std::endl;
    std::cout << ">> Initializing of Gnuplot..." << std::endl;
    if(get_program_path())
    {
        std::cout << "\tGnuplot found: " << m_sGNUPlotPath << "/" << m_sGNUPlotFileName << std::endl;
        return true;
    }
    else
    {
        std::cout << "\tError: Initialization failed..." << std::endl << std::endl;
        return false;
    }
}

void g_init()
{
    if(find_gnuplot())
    {
        std::cout << "\tInitialization of CathGnuplot succeeded..." << std::endl;
        //fflush(stdout);
    }
    else
        exit(-1);
}

void g_end()
{
    std::cout << ">> Closing Gnuplot..." << std::endl;
}

#define g_init g_init();
#define g_end g_end();
/////////////////////////////////////////////////////////////////////////////////////////////

#endif
