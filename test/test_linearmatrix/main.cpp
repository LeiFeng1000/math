#include <iostream>
#include "linear_equations.hpp"

using namespace math;
using namespace std;

int main(int argc,char* argv[])
{
    linear_equations<double> linear(3,4,{1,2,3,-1,-1,2,-1,-3,-5,2,1,0});
    linear.calculate();

    cout << linear.x_n(1).value() << endl;

    linear.calculate_inverse();
    cout << linear.x_n(1).value() << endl;
}
