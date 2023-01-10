/**
 * @file        sequence.hpp
 * @copyright   copyright(c) 2022 zjw. MIT
 * @author      zjw
 * @date        2023年1月7日
 * @brief       @namespace math::sequence 实现一些数列的算法
*/

#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <vector>
#include <optional>
#include <concepts>

namespace math
{
namespace sequence
{
    /**
     * @brief       计算两个数列的和，要求两个数列的维度相同
     * @param s1    左操作数
     * @param s2    右操作数
     * @return      计算结果。若两数列的维度不同，返回值是无效的
     * @exception   若内存不足，由C++标准库抛出：std::bad_alloc；
    */
    template<typename T,std::constructible_from<T> TF>
    std::optional<std::vector<T>> operator+(std::vector<T> const& s1,std::vector<TF> const& s2) 
    {
        using namespace std;

        if (s1.size() != s2.size())
            return {};

        auto result{s1};
        for (size_t i{};i < s1.size(); ++i)
            result.at(i) += s2.at(i);

        return result;
    }

    /**
     * @brief       计算数列的数乘
     * @param s1    指定数列
     * @param k     指定右操作数
     * @return      计算结果，
     * @exception   若内存不足，由C++标准库抛出：std::bad_alloc
    */
    template<typename T,std::constructible_from<T> TF>
    std::vector<T> operator*(std::vector<T> const& s1,TF const& k)
    {
        auto result{s1};

        for(auto& i : result)
            i *= k;
        return result;
    }

    /**
     * @brief       计算两向量的点乘，要求两数列的维度必须相同
     * @param left  左操作数
     * @param right 右操作数
     * @return      计算结果。如果两数列的维度不同，则返回值是无效的
     * @exception   若内存不足，抛出：std::bad_alloc
     */
    template<typename T,std::constructible_from<T> TF>
    std::optional<T> dot(std::vector<T> const& left,std::vector<TF> const& right)
    {
        using namespace std;

        if (left.size() != right.size())
            return {};

        T result{};
        for (size_t i{}; i < left.size(); ++i)
            result += left.at(i) * T(right.at(i));
        return result;
    }

    /**
     * @brief       计算两向量的叉乘
     * @param left  左操作数
     * @param right 右操作数
     * @return      计算结果。若两个数列的维度不同，返回值是无效的
     * @exception   若内存不足。则由C++标准库抛出: std::bad_alloc
     */
    template<typename T,std::constructible_from<T> TF>
    std::optional<std::vector<T>> cross(std::vector<T> const& left,std::vector<TF> const& right)
    {
        using namespace std;

        if (left.size() != right.size())
            return {};

        vector<T> result(left);
        for (size_t i{}; i < left.size(); ++i)
            result.at(i) = T(right.at(i)) * left.at(i);

        return result;
    }

    /**
     * @brief       判断两数列是否相等。要求：两数列维度相同
     * @param s1    第一个数列
     * @param s2    第二个数列
     * @return      计算结果。若两数列维度不同，返回值无效
     */
    template<typename T1,std::equality_comparable_with<T1> T2>
    std::optional<bool> operator == (std::vector<T1> const& s1,std::vector<T2> const& s2) noexcept
    {
        if (s1.size() != s2.size())
            return {};

        for (size_t i{}; i < s1.size(); ++i)
        {
            if (s1.at(i) != s2.at(i))
                return false;
        }

        return true;
    }

    /**
     * @brief       判断两数列是否成比例。要求：两数列维度相等
     * @param s1    第一个数列
     * @param s2    第二个数列
     * @return      true,两数列成比例；false，两数列不成比例。若两数列维数不同，返回值无效
     */
    template<typename T1,std::equality_comparable_with<T1> T2>
    std::optional<bool> prorate(std::vector<T1> const& s1,std::vector<T2> const& s2) noexcept
    {
        if (s1.size() != s2.size())
            return {};

        auto rate = s1.at(0) / s2.at(0);

        for (size_t i{}; i < s1.size(); ++i)
        {
            if (s2.at(i) * rate != s1.at(i))
                return false;
        }

        return true;
    }
}
}

#endif //SEQUENCE_HPP
