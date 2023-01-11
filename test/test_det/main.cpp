#include "det.hpp"
#include "numerical_table.hpp"
#include "matrix.hpp"
#include "det.hpp"

#include <iostream>
#include <bitset>

using namespace std;

int main(int argc,char* argv[])
{
    math::det<double> det(4,std::initializer_list<double>{
    6,1,1,1,
    6,3,1,1,
    6,1,3,1,
    6,1,1,3,});

    cout << det.general_calculate() << endl;

    cout << det.elimination_calculate() << endl;

    det.operator<<(cout);

    auto adet { det.A_i_j(1,1).value() };

    cout << det.elimination_calculate() << endl;
    cout << det.get_element(1,1).value() * adet.elimination_calculate() << endl;
    adet.operator<<(cout);

    return 0;
}
