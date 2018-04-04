//
// Created by haijunz on 18-4-3.
//
// http://www.cnblogs.com/qicosmos/p/3379865.html


#ifndef ALGO_H
#define ALGO_H

#include "commons.h"





template<typename R=void>
struct CommCommand
{
private:
    std::function < R()> m_f;

public:
    template< class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
    void Wrap(F && f, Args && ... args)
    {
        m_f = [&]{return f(args...); };
    }

    template<class C, class... DArgs, class P, class... Args>
    void Wrap(R(C::*f)(DArgs...) const, P && p, Args && ... args)
    {
        m_f = [&, f]{return (*p.*f)( args...); };
    }

// non-const member function
    template<class C, class... DArgs, class P, class... Args>
    void Wrap(R(C::*f)(DArgs...), P && p, Args && ... args)
    {
        m_f = [&, f]{return (*p.*f)( args...); };
    }

    R Excecute()
    {
        return m_f();
    }
};











int foo();






















#endif //CMC_CMC_H
