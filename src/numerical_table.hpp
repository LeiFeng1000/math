/**
 * @file numerical_table.h
 * @copyright copyright(c) 2022 zjw.MIT
 * @author zjw
 * @brief class @ref numerical_table 实现一个数表。class @ref sequence 提供一些数列计算的静态函数
 */
#ifndef NUMERICAL_TABLE_H
#define NUMERICAL_TABLE_H

#include <vector>
#include <type_traits>
#include <iterator>
#include <string>
#include <stdexcept>
#include <optional>

#include <thread>
#include <mutex>
#include <shared_mutex>

#include "sequence.hpp"

namespace math
{
/**
 * @brief @class numerical_table        实现一个数表,以列主序。类被设计为线程安全的，读取时采用 std::shared_lock 进行锁；写入时采用 std::lock_guard 进行锁
 * @tparam T 数据类型
 *
 * @fn void set_M(size_t M);            //设置数表的行数
 * @fn size_t get_M() const noexcept;   //获得数表的行数
 * @fn void set_N(size_t N);            //设置数表的列数
 * @fn void get_N() const noexcept;     //获得数表的列数
 *
 * @fn T get_element(size_t row,size_t column) const;               //设置数表的元素
 * @fn void set_element(size_t row,size_t column,T const& value);   //获得元素
 *
 * @fn std::vector<T> get_row(size_t row);                      //获得行向量
 * @fn void set_row(size_t row,std::vector<T> const& values);   //设置行向量，values的元素数必须与数表匹配
 *
 * @fn std::vector<T> get_column(size_t column);                    //获得列向量
 * @fn void set_column(size_t column,std::vector<T> const& values); //设置列向量，values的元素数必须与数表匹配
 *
 * @fn void swap_row(size_t i,size_t j);    //交换两行
 * @fn void swap_column(size_t i,size_t j); //交换两列
 *
 * @fn numerical_table transpose() const;   //返回数表的转置
 */
template<typename T>
class numerical_table
{
public:
    //默认构造，1行1列:{1}
    numerical_table()
        : numerical_table(1,1,{1})
    {

    }

    /**
     * @brief       初始值构造数表，
     * @param M     指定行数
     * @param N     指定列数
     * @param data  初始值，初始值以列主序
     * @exception   若内存不足，由C++标准库抛出: std::bad_alloc。 若M 或者 N 等于0，抛出: std::invalid_argument
     */
    template<std::constructible_from<T> TF>
    numerical_table(size_t M,size_t N,std::initializer_list<TF> const& data)
        : _M(M),
          _N(N),
          _data(data.begin(),data.end())
    {
        using namespace std;

        if (_M == 0)
            throw invalid_argument(string(__FUNCTION__) + " M == 0");
        if (_N == 0)
            throw invalid_argument(string(__FUNCTION__) + " N == 0");

        _data.resize(_M * _N);
    }

    /**
     * @brief       初始值构造数表，
     * @param M     指定行数
     * @param N     指定列数
     * @param data  初始值，初始值以列主序
     * @exception   若内存不足，由C++标准库抛出: std::bad_alloc。 若M 或者 N 等于0，抛出: std::invalid_argument
     */
    template<std::constructible_from<T> TF>
    numerical_table(size_t M,size_t N,std::vector<TF> const& data)
        : _M(M),
          _N(N),
          _data(data.begin(),data.end())
    {
        using namespace std;

        if (_M == 0)
            throw invalid_argument(string(__FUNCTION__) + " M == 0");
        if (_N == 0)
            throw invalid_argument(string(__FUNCTION__) + " N == 0");

        _data.resize(_M * _N);
    }

    //复制
    numerical_table(numerical_table const& other)
        :   _M(other.get_M()),
            _N(other.get_N()),
            _data(other._data)
    {
        
    }
    numerical_table& operator=(numerical_table const& other)
    {
        if (this == &other)
            return *this;

        _data = other._data;
        _M = other._M;
        _N = other._N;
        return *this;
    }

    //移动
    //被移动后的对象置为：{1}
    //这样做的目的是：保证数表任何时候都是一个有效的数表
    numerical_table(numerical_table&& other)
        :   _M(other.get_M()),
            _N(other.get_N()),
            _data(std::move(other._data))
    {
        other._M = other._N = 1;
        other._data.assign({1});
    }
    numerical_table operator=(numerical_table&& other)
    {
        if (this == &other)
            return *this;

        _data = std::move(other._data);
        _M = other._M;
        _N = other._N;

        other._data.assign({1});
        other._N = other._M = 1;
        
        return *this;
    }

    /**
        * @brief        判断两个矩阵是否为同型数表
        * @param right  右操作数
        * @return       true,为同型；false,非同型
    */
    bool homotype_matrix(numerical_table const& right) const noexcept 
    {
        return get_M() == right.get_M() and get_N() == right.get_N();
    }

    /**
     * @brief     判断矩阵是否为方阵
     * @return    true,为方阵；false,非方阵
     */
    bool square_matrix() const noexcept 
    {
        return get_M() == get_N();
    }

    /**
     * @brief       设置数表的行数
     * @param M     指定行数
     * @exception   若内存不足，由C++标准库抛出: std::bad_alloc。
    */
    void set_M(size_t M) noexcept
    {
        using namespace std;

        if (M == 0)
            return ;

        auto tmp = this->transpose();
        tmp.set_N(M);
        *this = tmp.transpose();
    }

    /**
     * @brief       获得数表的行数
     * @return      数表的行数
    */
    size_t get_M() const noexcept 
    {
        return _M;
    }

    /**
     * @brief       设置数表的列数
     * @param N     指定数表的列数
     * @exception   若内存不足，由C++标准库抛出: std::bad_alloc
    */
    void set_N(size_t N) noexcept
    {
        using namespace std;

        if (N == 0)
            return;

        _N = N;
        _data.resize(_M * _N);
    }

    /**
     * @brief       获得数表的列数
     * @return      数表的列数
    */
    size_t get_N() const noexcept 
    {
        return _N;
    }

    /**
     * @brief       获得数表的(i,j)元
     * @param i     指定行元
     * @param j     指定列元
     * @return      数表的(i,j)元。若 i 或者 j 无效，则返回值是无效的
    */
    std::optional<T> get_element(size_t row,size_t column) const noexcept 
    {
        using namespace std;

        if (row > get_M() or column > get_N())
            return {};
        
        --row;
        --column;

        return _data[column * get_M() + row];
    }

    /**
     * @brief       设置数表的(i,j)元
     * @param i     指定行元
     * @param j     指定列元
     * @param value 指定目标值
    */
    template<std::constructible_from<T> TF>
    void set_element(size_t i,size_t j,TF const& value) noexcept
    {
        using namespace std;

        if (i > get_M() or j > get_N())
            return ;

        --i;
        --j;

        _data[j * _M + i] = T(value);
    }

    /**
     * @brief       获得数表的行向量。
     * @param row   指定行
     * @return      数表的row行向量。若row无效，则返回值是无效的。
    */
    std::optional<std::vector<T>> get_row(size_t row) const noexcept 
    {
        using namespace std;

        if (row > get_M())
            return {};

        vector<T> line(get_N(),0);
        for (size_t i{1};i <= get_N();++i)
            line[i -1] = *get_element(row,i);
        return line;
    }

    /**
     * @brief           设置数表的行向量。可能有缺陷，例如：同时对同一行修改
     * @param row       指定行
     * @param values    目标数列。要求数列的元素数与数表的列数相同
    */
    template<std::constructible_from<T> TF>
    void set_row(size_t row,std::vector<TF> const& values) noexcept
    {
        using namespace std;

        if (row > get_M())
            return ;

        if (values.size() != get_N())
            return;

        for (size_t i{1};i <= get_N(); ++i)
            set_element(row,i,values[i -1]);
    }

    /**
     * @brief           获取数表的列向量
     * @param column    指定列
     * @return          数表的column列向量。若column无效，则返回值无效
    */
    std::optional<std::vector<T>> get_column(size_t column) const noexcept   
    {
        using namespace std;

        if (column > get_N())
            return {};

        vector<T> line(get_M(),0);
        for (size_t i{1};i <= _M;++i)
            line[i - 1] = get_element(i,column).value();
        return line;
    }

    /**
     * @brief           设置数表的列向量
     * @param column    指定列
     * @param values    指定目标数列。要求values元素数等于数表的行数
    */
    template<std::constructible_from<T> TF>
    void set_column(size_t column,std::vector<TF> const& values) noexcept
    {
        using namespace std;

        if (column > get_M())
            return;

        if (values.size() != get_M())
            return;

        for (size_t i{1};i <= get_M();++i)
            set_element(i,column,values[i-1]);
    }

    /**
     * @brief       交换数表的任意两行
     * @param i     指定行
     * @param j     指定行
    */
    void swap_row(size_t i,size_t j) noexcept
    {
        if (i > get_M() or j > get_M())
            return;

        auto line { get_row(i) };
        set_row(i,*get_row(j));
        set_row(j,*line);
    }

    /**
     * @brief       交换任意两列
     * @param i     指定列
     * @param j     指定列
    */
    void swap_column(size_t i,size_t j) noexcept
    {
        if (i > get_N() or j > get_N())
            return ;

        auto column { get_column(i) };
        set_column(i,*get_column(j));
        set_column(j,*column);
    }

    /**
     * @brief       计算数表的转置
     * @exception   若内存不足，由C++标准库抛出：std::bad_alloc
    */
    numerical_table<T> transpose()  const noexcept 
    {
        numerical_table<T> result(get_N(),get_M(),{T{}});

        for (size_t row{1}; row <= _M;++row)
            for (size_t column{1}; column <= _N;++column)
            {
                result.set_element(column,row,*get_element(row,column));
            }
        return result;
    }

    /**
     * @brief               输出数表
     * @param out_target    指定输出目标
     */
    std::ostream& operator<<(std::ostream& out_target) const noexcept
    {
        out_target << std::string("matrix ") << this->get_M() << ' ' << this->get_N() << '\n';

        auto out_line = [&](std::vector<T> const& line) noexcept
        {
            for (auto& i : line)
            {
                out_target << i << ' ';
            }
            out_target << std::string("\n");
        };

        for (size_t i{1}; i <= this->get_M(); ++i)
            out_line(this->get_row(i).value());
        return out_target;
    }
private:
    size_t _M,_N;
    std::vector<T> _data;
};

}
#endif // NUMERICAL_TABLE_H
