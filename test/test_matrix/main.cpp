#include "matrix.hpp"
#include "det.hpp"

#include <iostream>

using namespace math;
using namespace std;

int main()
{
    matrix<double> A(3,3,{1,2,3,2,2,4,3,1,3});

    auto det_A { A.det().value() };
    auto    A_11_det_A { det_A.A_i_j(1,1).value() },
            A_12_det_A { det_A.A_i_j(1,2).value() },
            A_13_det_A { det_A.A_i_j(1,3).value() },
            A_21_det_A { det_A.A_i_j(2,1).value() },
            A_22_det_A { *det_A.A_i_j(2,2) },
            A_23_det_A { *det_A.A_i_j(2,3) },
            A_31_det_A { *det_A.A_i_j(3,1) },
            A_32_det_A { *det_A.A_i_j(3,2) },
            A_33_det_A { *det_A.A_i_j(3,3) };

    cout << A_11_det_A.elimination_calculate() << '\t' << A_12_det_A.elimination_calculate() << '\t' << A_13_det_A.elimination_calculate() << endl;
    cout << A_21_det_A.elimination_calculate() << '\t' << A_22_det_A.elimination_calculate() << '\t' << A_23_det_A.elimination_calculate() << endl;
    cout << A_31_det_A.elimination_calculate() << '\t' << A_32_det_A.elimination_calculate() << '\t' << A_23_det_A.elimination_calculate() << endl;

    return 0;
}
