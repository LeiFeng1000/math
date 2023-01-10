#include "matrix.hpp"
#include "det.hpp"

#include <format>
#include <iostream>

using namespace math;
using namespace std;

int main()
{
    det<double> det(2,{3,2,-2,1});
    cout << format("D:{}\n",det.general_calculate());

    math::det<double> det_1(2,{12,1,-2,1});
    cout << format("D1:{}\n",det_1.general_calculate());

    math::det<double> det_2(2,{3,2,12,1});
    cout << format("D2:{}\n",det_2.general_calculate());

    cout << format("x1 = {}\n",det_1.general_calculate()/det.general_calculate());
    cout << format("x2 = {}\n",det_2.general_calculate()/det.general_calculate());

    det.operator<<(cout);
    return 0;
}
