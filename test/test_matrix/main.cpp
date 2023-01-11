#include "matrix.hpp"
#include "det.hpp"

#include <iostream>

using namespace math;
using namespace std;

int main()
{
    matrix<double> A(2,3,{2,1,0,3,-1,2}),B(3,3,{1,4,2,7,2,0,-1,3,1});
    auto t_AB_2 {(A*B).value().transpose()};
    

    t_AB_2.operator<<(cout)<<endl;

    return 0;
}
