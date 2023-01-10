/**
 * @file det.hpp
 * @copyright copyright(c) 2022 zjw.MIT
 * @author zjw
 * @date 2023年1月1日
 * @brief class @ref @det 实现一个行列式. class @ref inverse_number 函数对象，计算数列中某个元素的逆序数。class @ref elements_inverse_number 函数对象，计算数列中所有元素的逆序数
*/

#ifndef DET_HPP
#define DET_HPP

#include <type_traits>
#include <thread>
#include <vector>
#include <numeric>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#include "numerical_table.hpp"

namespace math
{
/**
 * @brief class inverse_number 计算数列中某个元素的逆序数
 * @tparam T 元素类型
 * @tparam cmp_type 定义标准排列，默认为: std::less
 * 
 * @return 返回计算的结果
*/
struct inverse_number
{
    template<typename forward_it_type,typename cmp_type = std::less<typename forward_it_type::value_type>>
    size_t operator()(forward_it_type first,forward_it_type index,cmp_type cmp = {})
    {        
        size_t count(0);
        while (first != index)
        {
            if (cmp(*index,*first))
                ++count;
            ++first;
        }
        return count;
    }
};

/**
 * @brief class elements_inverse_number 计算数列中所有元素的逆序数和
 * @tparam T 元素类型
 * @tparam cmp_type 定义标准排列，默认为: std::less
 * 
 * @return 返回计算的结果
*/
struct elements_inverse_number
{
    template<typename forward_it_type,typename cmp_type>
    size_t operator()(forward_it_type first,forward_it_type last,cmp_type cmp)
    {
        if (first == last)
            return 0;

        size_t count{};
        auto tfirst{first};
        while (++tfirst != last)
        {
            count += inverse_number()(first,tfirst);
        }
        return count;
    }
};

/**
 * @brief class det 行列式，行主序
 * @tparam T 元素类型
 * 
 * @fn void set_N(size_t N) noexcept;   //设置行列式的维度
 * @fn size_t get_N() const noexcept;     //设置行列式的维度
 * @fn void set_element(size_t row,size_t column, T const& value) noexcept;  //设置元素
 * @fn std::vector<T> get_element(size_t row,size_t column) const noexcept;   //获得元素
 * @fn void set_row(size_t row,std::vector<T> const& valus) noexcept;   //设置行向量
 * @fn std::vector<T> get_row(size_t row) const noexcept;     //获得行向量
 * @fn void set_column(size_t column) noexcept;     //设置列向量
 * @fn std::vector<T> get_column(size_t column) const noexcept;   //获得列向量
 *
 * @fn det transpose() const;   //返回行列式的转置，行列式的转置与原行列式相等，即：T(D) = D.参见：《工程数学.线性代数》第六版
 * @fn void swap_row(size_t i,size_t j) ;   //交换行列式两行
 * @fn void swap_column(size_t i,size_t j); //交换行列式两列
 *
 * @fn det algebraic_complement_minor() const;  //计算代数余子式
 * 
 * @fn double general_calculate() const noexcept;   //n阶行列式定义法计算行列式的值
*/
template<typename T>
class det
{
public:
    //默认构造，{1}
    det() noexcept
        : det(1,{T{1}})
    {

    }

    /**
     * @brief       初始值构造
     * @param N     行列式阶数
     * @param data  行列式数据，以列主序方式填充行列式，不足元素数使用0填充，多余的元素忽略
     * 
     * @exception   抛出由 math::numerical_table 引发的异常
    */
   template<std::constructible_from<T> TF>
   det(size_t N,std::initializer_list<TF> const& data)
    : _nt(N,N,data)
    {
        
    }

    /**
     * @brief       初始值构造
     * @param N     行列式阶数
     * @param data  行列式数据，以列主序方式填充行列式，不足元素数使用0填充，多余的元素忽略
     * 
     * @exception   抛出由 math::numerical_table 引发的异常
    */
   template<std::constructible_from<T> TF>
   det(size_t N,std::vector<TF> const& data)
    : _nt(N,N,data)
    {
        
    }

    //复制
    det(det const& other)
        : _nt(other._nt)
    {

    }
    det operator=(det const& other)
    {
        if (this == &other)
            return *this;

        _nt = other._nt;
        return *this;
    }

    //移动
    det(det&& other)
        : _nt(std::move(other._nt))
    {

    }
    det operator=(det&& other)
    {
        if (this == &other)
            return *this;

        _nt = std::move(other._nt);
        return *this;
    }

    /**
     * @brief       设置行列式的维度
     * @param N     指定行列式的维度
    */
   void set_N(size_t N)
   {
        _nt.set_M(N);
        _nt.set_N(N);
   }

    /**
     * @brief       获取行列式的维度
     * @return      行列式的维度
    */
   size_t get_N() const noexcept
   {
        return _nt.get_M();
   }

    /**
     * @brief       设置行列式的(i,j)元
     * @param i     指定行元
     * @param j     指定列元
     * @param value 指定目标值
    */
   template<std::constructible_from<T> TF>
   void set_element(size_t i,size_t j,TF const& value) 
   {
        _nt.set_element(i,j,value);
   }

   /**
    * @brief        获取行列式的(i,j)元
    * @param i      指定行元
    * @param j      指定列元
    * @return       行列式的(i,j)元
   */
   std::optional<T> get_element(size_t i,size_t j) const 
   {
        return _nt.get_element(i,j);
   }

    /**
     * @brief           设置行列式的行向量
     * @param row       指定行
     * @param values    指定目标行向量。要求values的元素数与行列式的维度相同
    */
    template<std::constructible_from<T> TF>
    void set_row(size_t row,std::vector<TF> const& values) 
    {
        _nt.set_row(row,values);
    }

    /**
     * @brief       获取行列式的行向量
     * @param row   指定行
     * @return      行列式的第row行
    */
    std::optional<std::vector<T>> get_row(size_t row) const 
    {
        return _nt.get_row(row);
    }

    /**
     * @brief           设置行列式的列向量
     * @param column    指定列
     * @param values    指定目标数列。要求values的元素数和行列式的维度相同
    */
    template<std::constructible_from<T> TF>
    void set_column(size_t column,std::vector<TF> const& values) 
    {
        _nt.set_column(column,values);
    }

    /**
     * @brief           获取行列式的列向量
     * @param column    指定列
     * @return          行列式的column列
    */
    std::optional<std::vector<T>> get_column(size_t column) const 
    {
        return _nt.get_column(column);
    }

    //交换行列式的任意两行
    void swap_row(size_t i,size_t j)
    {
        return _nt.swap_row(i,j);
    }

    //交换行列式的任意两列
    void swap_column(size_t i,size_t j)
    {
        return _nt.swap_column(i,j);
    }

    /**
     * @brief       计算：行列式的M(i,j)
     * @param i     指定行元
     * @param j     指定列元
     * @return      计算结果。若(i,j)是无效的，则返回值是无效的
     */
    std::optional<det> m_i_j(size_t i,size_t j) const noexcept
    {
        using namespace std;
        using namespace sequence;

        if (i < 1 or i > this->get_N() or
            j < 1 or j > this->get_N() or
            this->get_N() == 1)
            return {};

        det<T> result(this->get_N() - 1,{T{}});

        for (size_t index{1}; index <= this->get_N(); ++index)
        {
            if (index == i)
                continue;
            else
            {
                auto line = *this->get_row(index);
                line.erase(line.begin() + (j - 1));
                if (index < i)
                    result.set_row(index,line);
                else
                    result.set_row(index-1,line);
            }
        }
        return result;
    }
    //计算行列式(i,j)元的代数余子式
    std::optional<det> algebraic_complement_minor(size_t i,size_t j) const
    {
        using namespace std;
        using namespace sequence;

        auto result = this->m_i_j(i,j).value();
        if ((i + j) % 2 == 1)
            for (size_t row{1}; row <= result.get_N(); ++row)
                result.set_row(row,(*result.get_row(row) * T(-1)));

        return result;
    }

    //计算行列式的转置
    det transpose() const
    {
        det result;
        result._nt = _nt.transpose();

        return result;
    }

    //通过n阶行列式的定义法求行列式的值
    double general_calculate() const noexcept
    {
        if (get_N() == 1)
            return *get_element(1,1);

        using namespace std;
        vector<size_t> indices(get_N());
        iota(indices.begin(),indices.end(),1);
        double result{0};

        do
        {
            //计算逆序数
            auto count =  math::elements_inverse_number()(indices.begin(),indices.end(),std::less<size_t>());
            double tresult{};
            if (count % 2 == 0)
                tresult = 1;
            else 
                tresult = -1;

            for (size_t index {1}; index <= get_N(); ++index)
            {
                if (get_element(index,indices[index-1]) == 0)
                {
                    tresult = 0;
                    break;
                }
                
                tresult *= *get_element(index,indices[index-1]);
            }

            result += tresult;
        } while (next_permutation(indices.begin(),indices.end()));
        
        return result;
    }

    /**
     * @brief       将行列式消元
     */
    void elimination() noexcept
    {
        using namespace math::sequence;

        if (this->get_N() == 1)
            return;

        for (size_t r1{1}; r1 < this->get_N(); ++r1)
        {
            //查找首个非0行
            if (this->get_element(r1,r1).value() == 0)
            {
                for (size_t r2{r1 + 1}; r2 <= this->get_N(); ++r2)
                {
                    if (this->get_element(r1,r2).value() != 0)
                    {
                        this->swap_row(r1,r2);
                        break;
                    }
                }

                if (this->get_element(r1,r1).value() == 0)
                    continue;
            }


            for (size_t r2{r1 + 1}; r2 <= this->get_N(); ++r2)
            {
                this->set_row(r2,
                              (this->get_row(r1).value() * (-this->get_element(r2,r1).value() / this->get_element(r1,r1).value()) +
                              this->get_row(r2).value()).value());
            }
        }
    }

    /**
     * @brief   通过消元法，解行列式的值
     * @return  计算结果
     */
    double elimination_calculate() const noexcept
    {
        if (this->get_N() == 1)
            return get_element(1,1).value();

        auto tmp{*this};
        double result{1};

        tmp.elimination();

        for (size_t i{1}; i <= get_N(); ++i)
            result *= tmp.get_element(i,i).value();

        return result;
    }

    /**
     * @brief       将数表输出目标
     * @param out_target    指定目标
     */
    std::ostream& operator<<(std::ostream& out_target) const noexcept
    {
        return _nt.operator<<(out_target);
    }
protected:
    /**
     * @brief   判断行列式是否为0
     * @return  true,行列式为0；false,行列式不为0.
     */
    bool zero() const noexcept
    {
        if (this->get_N() == 1)
            return get_element(1,1) == 0;

        using namespace math::sequence;

        for (size_t r{1}; r < this->get_N(); ++r)
        {
            for (size_t c{r + 1}; r <= this->get_N(); ++c)
            {
                if (prorate(this->get_row(r).value(),this->get_row(c).value()))
                    return true;
            }
            return false;
        }
    }
private:
    numerical_table<T> _nt;
};
}

#endif //DET_HPP
