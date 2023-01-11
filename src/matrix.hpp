/**
 * @file matrix.h
 * @copyright copyright(c) 2022 zjw.MIT
 * @author zjw
 * @date 2023年1月2日
 * @brief class @ref matrix 实现矩阵类
 */
#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#include <thread>
#include <shared_mutex>
#include <optional>

#include "numerical_table.hpp"
#include "det.hpp"

namespace math {

/**
 * @brief The matrix class 实现矩阵类，列主序
 *
 * @fn matrix operator+(matrix const& right) const;
 * @fn det det() const;                                         //若矩阵是方阵，返回矩阵的行列式
 * @fn matrix operator*(double t) noexcept;                     //计算矩阵的t倍
 * @fn matrix operator*(matrix const& right) const noexcept;    //计算*this * right
 * @fn matrix adjoint() const ;                                 //若矩阵是方阵，则计算矩阵的伴随矩阵
 * @fn matrix inverse() const noexcept;                         //计算矩阵的逆矩阵
 */
template<typename T>
class matrix : public numerical_table<T>
{
public:
    //默认构造，{1}
    matrix() noexcept
        : matrix(1,1,{T(1)})
    {

    }

    /**
     * @brief           使用初始值构造矩阵
     * @param M         指定行数
     * @param N         指定列数
     * @param data      指定初始值。data是以列主序的
     * @exception       若内存不足，由C++标准库抛出：std::bad_alloc；由 math::numerical_table 抛出的异常
    */
    matrix(size_t M,size_t N,std::initializer_list<T> const& data)
        : numerical_table<T>(M,N,data)
    {

    }

    matrix(size_t M,size_t N,std::vector<T> const& data)
        : numerical_table<T>(M,N,data)
    {
        
    }

    //复制
    matrix(matrix const& other)
        : numerical_table<T>(other)
    {

    }
    matrix operator=(matrix const& other)
    {
        if (this == &other)
            return *this;

        numerical_table<T>::operator=(other);
        return *this;
    }

    //移动
    matrix(matrix&& other)
        : numerical_table<T>(std::move(other))
    {

    }
    matrix operator=(matrix&& other)
    {
        if (this == &other)
            return *this;

        numerical_table<T>::operator=(std::move(other));
        return *this;
    }

    /**
     * @brief       计算*this + right
     * @param right 右操作数。要求: right 和 *this必须是同型矩阵，否者返回值是无效的
     * @return      计算结果
    */
    std::optional<matrix> operator+(matrix const& right) noexcept   //const
    {
        if (this->get_M() != right.get_M() or this->get_N() != right.get_N())
            return {};

        auto result{*this};

        using namespace sequence;
        for (size_t i{1};i <= result.get_M();++i)
        {
            result->set_row(i,*(*this->get_row(i) + *right.get_row(i)));
        }
        return result;
    }

    /**
     * @brief       计算：*this += right
     * @param right 右操作数。要求：*this 和 right 必须是同型矩阵，否者返回值是无效
     * @return      计算结果
    */
    std::optional<matrix> operator += (matrix const& right) noexcept   
    {
        if (!homotype_matrix(right))
            return {};

        for (size_t i{1}; i <= this->get_M(); ++i)
        {
            this->set_row(i,*(*this->get_row(i) + *right.get_row(i)));
        }

        return *this;
    }

    /**
     * @brief       计算矩阵的行列式。要求：矩阵是方阵，否者返回值是无效的
     * @return      矩阵的行列式
     */
    std::optional<math::det<T>> det() const noexcept    //const
    {
        using namespace std;

        if (!this->square_matrix())
            return {};

        math::det<T> result(this->get_M(),{T{}});
        for (size_t i{1}; i <= this->get_M(); ++i)
        {
            result.set_row(i,this->get_row(i).value());
        }
        return result;
    }

    /**
     * @brief       计算矩阵的t倍
     * @param t     指定倍数。要求：t != 0
     * @return      计算结果
     */
    template<std::constructible_from<T> TF>
    std::optional<matrix> operator*(TF const& t) noexcept
    {
        using namespace std;
        using namespace sequence;

        if (t == 0)
            return {};

        auto result { *this };

        return (result *= t).value();
    }

    /**
     * @brief       计算：计算矩阵的t倍
     * @param t     指定倍数，要求：t != 0
     * @return      计算结果
     */
    template<std::constructible_from<T> TF>
    std::optional<matrix> operator*=(TF const& t) noexcept
    {
        using namespace sequence;

        if (t == 0)
            return {};

        for (size_t i{1}; i <= this->get_M(); ++i)
            this->set_row(i,*this->get_row(i) * t);
        return *this;
    }

    /**
     * @brief       计算*this * right
     * @param right 右乘矩阵。要求：两个矩阵是可乘的，否者返回值是无效的
     * @return      计算结果
     */
    std::optional<matrix> operator*(matrix const& right) const
    {
        using namespace std;
        if (this->get_N() != right.get_M())
            return {};

        using namespace sequence;
        matrix<T> result(this->get_M(),right.get_N(),{T{}});

        for (size_t i{1};i <= result.get_M();++i)
            for (size_t j{1};j <= result.get_N();++j)
            {
                result.set_element(i,j,*dot(*this->get_row(i),*right.get_column(j)));
            }
        return result;
    }

    /**
     * @brief       计算矩阵的伴随矩阵。要求：矩阵是方阵，否则返回值是无效的；要求：矩阵不是1维的，否则返回值是无效的。该函数使用多线程计算，计算期间使用共享锁锁定
     * @return      矩阵的伴随矩阵
     */
    std::optional<matrix> adjoint() const
    {
        using namespace std;
        {
            if (this->get_M() != this->get_N())
                return {};
            if (this->get_M() == 1 or this->get_N() == 1)
                return {};
        }

        matrix<T> result(this->get_M(),this->get_N(),{T{}});
        auto tdet = this->det().value();

        vector<jthread> arr_thread{};

        for (size_t i{1};i <= this->get_M();++i)
        {
            for (size_t j{1};j <= this->get_N();++j)
            {
                arr_thread.push_back(jthread([&](size_t r,size_t c){
                                         auto ttdet = tdet.m_i_j(r,c);
                                         result.set_element(r,c,ttdet.value().general_calculate());
                                     },i,j));
            }
        }

        for (auto& i : arr_thread)
            i.join();

        return result;
    }

    /**
     * @brief           计算矩阵的逆矩阵，若矩阵不可逆，返回值是无效的；若矩阵不是方阵，返回值是无效的
     * @return          计算结果
     * @exception       若内存不足，抛出C++标准库抛出的异常;
     */
    std::optional<matrix> inverse() const
    {
        auto vdet = this->det();
        if (!vdet)
            return {};

        auto adjoint_matrix =  this->adjoint().value();
        return adjoint_matrix * (1 / T(vdet->general_calculate()));
    }

    /**
     * @brief       计算：row * t
     * @param row   指定行
     * @param t     指定倍数。要求：t != 0
     * @return      void
     */
    template<std::constructible_from<T> TF>
    void row_times(size_t row,TF const& t) noexcept
    {
        if (row < 1 or row > this->get_M())
            return;

        using namespace sequence;

        this->set_row(row,(*this->get_row(row) * T(t)));
    }

    /**
     * @brief           计算： column * t
     * @param column    指定列
     * @param t         指定倍数
     * @return          void
     */
    template<std::constructible_from<T> TF>
    void column_times(size_t column,TF const& t) noexcept
    {
        if (column < 1 or column > this->get_N())
            return ;

        this->set_column(column,*(this->get_column(column) * T(t)));
    }

    /**
     * @brief       计算：row_1 + k * row_2
     * @param row_1 指定左操作数
     * @param row_2 指定右操作数
     * @param k     指定倍数
     * @return      void
     */
    template<std::constructible_from<T> TF>
    void row_add_times_row(size_t row_1,size_t row_2,TF const& k) noexcept
    {
        if (row_1 < 1 or row_1 > this->get_M() or
            row_2 < 1 or row_2 > this->get_M())
            return ;

        using namespace sequence;

        this->set_row(row_1,*(*this->get_row(row_1) + (*this->get_row(row_2) * T(k))));
    }

    /**
     * @brief           计算：column_1 + k * column_2
     * @param column_1  指定左操作数
     * @param column_2  指定右操作数
     * @param k         指定倍数
     * @return          void
     */
    template<std::constructible_from<T> TF>
    void column_add_times_column(size_t column_1,size_t column_2,TF const& k) noexcept
    {
        if (column_1 < 1 or column_1 > this->get_N() or
            column_2 < 1 or column_2 > this->get_N())
            return;

        return this->set_column(column_1,*(*this->get_column(column_1) + *(*this->get_column(column_2) * T(k))));
    }

    /**
     * @brief       将矩阵化为行阶梯矩阵
     * @return      void
     */
    void elimination() noexcept
    {
        //对于行向量或列向量。无须进行化简
        if (this->get_N() == 1 or this->get_M() == 1)
            return ;

        for (size_t i{1}; i < this->get_N(); ++i)
        {
            //找到首个非0行
            if (*this->get_element(i,i) == 0)
            {
                for (size_t j{i + 1}; j <= this->get_M(); ++j)
                {
                    if (*this->get_element(j,i) != 0)
                        this->swap_row(i,j);
                }

                if (*this->get_element(i,i) == 0)
                    continue;
            }

            //将行最简化
            this->row_times(i,1 / *this->get_element(i,i));

            for (size_t j{i + 1}; j <= this->get_M(); ++j)
            {
                auto k = *this->get_element(j,i) / *this->get_element(i,i);
                this->row_add_times_row(j,i,-k);
            }

            //向上最简化
            for (size_t i{this->get_M()}; i > 1; --i)
            {
                if (*this->get_element(i,i) == 0)
                    continue;

                for (size_t j{i - 1}; j > 0; --j)
                {
                    auto k = *this->get_element(j,i);

                    this->row_add_times_row(j,i,-k);
                }
            }
        }
    }
};

} // namespace math

#endif // MATH_MATRIX_H
