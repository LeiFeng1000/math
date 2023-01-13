/**
 * @brief       complex_plane.hpp
 * @copyright   copyright(c) 2022 zjw.MIT
 * @author      ZJW
 * @date        2023年1月13日
 * 
 * @brief       @class complex_plane 实现一个复平面类
*/

#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <variant>
#include "complex.h"

namespace math
{
    /**
     * @author  zjw
     * @date    2023年1月13日
     * 
     * @brief   @class complex_plane 实现一个复平面类
    */
    class complex_plane
    {
    public:
        /**
         * @brief       构造
         * @param x     @ref complex
         * @param y     @ref complex
        */
        complex_plane(double x,double y) noexcept
            : _complex(x,y)
        {
        }

        /**
         * @brief       计算：复平面的模
         * @return      计算结果
        */
        double modulus() const noexcept
        {
            std::sqrt(_complex.x() * _complex.x() + _complex.y() * _complex.y());
        }
    private:
        std::variant<math::complex> _complex;       //代表当前复平面的复数

    };
}

#endif //COMPLEX_HPP