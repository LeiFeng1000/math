/**
 * @file      linear_equations.hpp
 * @author      ZJW
 * @date        2023年1月14日
 * 
 * @brief       @class linear_equations 实现一个线性方程组
*/

#include "numerical_table.hpp"
#include "det.hpp"
#include "matrix.hpp"
#include <gtest/gtest.h>

namespace math
{
    /**
     * @author  ZJW
     * @date    2023年1月14日
     * 
     * @brief       @class linear_equations 实现一个线性方程组。类的前n-1列为方程组系数，第n列方程组常数项
     * @tparam T    
    */
    template<typename T>
    class linear_equations
    {
    public:
        /**
         * @brief       默认构造函数，{1,0}
        */        
        explicit linear_equations() noexcept
            : _data{1,0,{T(1),T(0)}}
        {
            
        }

        /**
         * @brief       使用初始值构造。初始值以列主序
         * @param M     指定方程组行数。
         * @param data  指定初始值。  
        */
        template<std::constructible_from<T> TF>
        explicit linear_equations(size_t m,size_t n,std::initializer_list<TF> const& data) noexcept
            : _data(m,n,data)
        {

        }

        /**
         * @brief       计算：方程的未知数的解
         * @return      void
        */
        void calculate() noexcept
        {
            if (_data.get_M() <= 1)
            {
                return;
            }

            _x.clear();

            //系数矩阵
            det<T> coefficient_det(_data.get_N()-1,{T{}});
            //增广矩阵
            det<T> augmented_det(_data.get_N()-1,{T{}});

            for (size_t i{1}; i < _data.get_N(); ++i)
            {
                coefficient_det.set_column(i,_data.get_column(i).value());
                augmented_det.set_column(i,_data.get_column(i).value());
            }

            auto x { coefficient_det.general_calculate() };
            if (x == 0)
                return ;
            for (size_t i{1}; i < _data.get_N(); ++i)
            {
                if (i != 1)
                {
                    augmented_det.set_column(i,*_data.get_column(_data.get_N()));
                    augmented_det.set_column(i-1,*_data.get_column(i-1));
                }
                else
                    augmented_det.set_column(i,*_data.get_column(_data.get_N()));

                _x.push_back(augmented_det.general_calculate() / x);
            }
        }
    
        /**
         * @brief       计算：方程的未知数的解。通过逆矩阵方法
         * @return      void
         */
        void calculate_inverse() noexcept
        {
            if (_data.get_M() <= 1)
            {
                return;
            }

            ///系数矩阵
            matrix<T> ceofficient(_data.get_M(),_data.get_N()-1,{T{}});
            //常数矩阵
            matrix<T> constant(_data.get_M(),1,_data.get_column(_data.get_N()).value());

            for (size_t i{1};i < _data.get_N(); ++i)
            {
                ceofficient.set_column(i,_data.get_column(i).value());
            }

            auto x = (ceofficient.inverse().value() * constant).value();

            _x.clear();
            _x = x.get_column(1).value();
        }

        /**
         * @brief       获取：未知数的解
         * @param i     指定未知数的索引。要求：i的索引必须有效，否则返回值无效
         * @return      未知数的解
        */
        std::optional<T> x_n(size_t n) const noexcept
        {
            if (n >= _x.size())
            {
                return {};
            }

            return _x[n-1];
        }
        

    private:
        numerical_table<T> _data;
        std::vector<T> _x;
    };
}
