/**
 * @brief       complex.hpp
 * @copyright   copyright(c) 2022 zjw.MIT
 * @author      ZJW
 * @date        2023年1月12日
 * 
 * @brief       @class complex 实现一个复数类
*/

#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <type_traits>
#include <vector>
#include <compare>

namespace math
{
    /**
     * @author      zjw
     * @date        2023年1月12日
     * 
     * @brief       @class complex 实现一个复数类
    */
    class complex 
    {
    public:
        /**
         * @brief   构造。
         * @param x 指定实部
         * @param y 指定虚部
        */
        explicit complex(double x = 1,double y = 0) noexcept
            :   _x(x),
                _y(y)
        {

        }

        complex(complex const&) noexcept = default;
        complex(complex&&) noexcept = default;
        complex& operator=(complex&&) noexcept = default;
        complex& operator=(complex const&) noexcept = default;

        /**
         * @brief       比较：复数
         * @param right 指定右操作数
         * @return      比较结果
        */
        auto operator<=>(complex const& right) const noexcept
        {
            if (_x == right._x and _y == right._y)
            {
                return std::strong_ordering::less;
            }
        }

        /**
         * @brief       计算：this += right
         * @param right 指定右操作数
         * @return      *this
        */
        complex operator += (complex const& right) noexcept
        {
            _x += right._x;
            _y += right._y;

            return *this;
        }

        /**
         * @brief       计算：两个复数的和
         * @param right 指定右操作数
         * @return      计算结果
        */
        complex operator+(complex const& right) noexcept 
        {
            auto result {*this};
            result += right;
            return result;
        }

        /**
         * @brief       计算：两个复数的乘积
         * @param right 右操作数
         * @return      *this
        */
        complex operator*=(complex const& right) noexcept
        {
            _x = _x * right._x - _y * right._y;
            _y = _x * right._y + _y * right._x;

            return *this;
        }

        /**
         * @brief       计算：两个复数的矩阵
         * @param right 右操作数
         * @return      计算结果
        */
        complex operator*(complex const& right) const noexcept 
        {
            auto result { *this };
            result *= right;
            return result;
        }
    private:
        double  _x,     //实部
                _y;     //虚部
    }
}
#endif  //COMPLEX_HPP