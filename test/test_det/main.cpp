#include "det.hpp"
#include "numerical_table.hpp"
#include "matrix.hpp"
#include "det.hpp"

#include <iostream>
#include <bitset>

using namespace std;

int main(int argc,char* argv[])
{
    if (argc == 1)
    {
        cout << "Error: \n"
             << "\t -m \t 指定行数\n"
             << "\t -n \t 指定列数\n"
             << "\t -t \t 指定数据，以列主序\n";
        return -1;
    }

    double row{},column{};
    std::vector<double> matrix{};

    std::bitset<3> parset_status{false};    //{m,n,t}
    for (size_t i{0};i < argc; ++i)
    {
        if (parset_status.test(0))
        {
            row = strtod(argv[i],nullptr);
            parset_status.set(0,false);

            continue;
        }
        else if (parset_status.test(1))
        {
            column = strtod(argv[i],nullptr);
            parset_status.set(1,false);

            continue;
        }
        else if (parset_status.test(2))
        {
            auto tmp = strtod(argv[i],nullptr);
            if (tmp == 0)
            matrix.push_back(strtod(argv[i],nullptr));
        }
        else 
        {
            if (argv[i] == string("-m"))
                parset_status.set(0,true);
            else if (argv[i] == string("-n"))
                parset_status.set(1,true);
            else if (argv[i] == string("-t"))
                parset_status.set(2,true);
            else 
                continue;
        }
    }

    if (row < 1 or column < 1)
    {
        cout << "指定行或列小于0.\n";
        return -1;
    }
    
    if (matrix.size() != row * column)
    {
        cout << "给定数据元素数不全\n";
        return -1;
    }

    math::det<double> det(row,vector<double>{matrix.begin(),matrix.end()});

    det.elimination();

    return 0;
}
