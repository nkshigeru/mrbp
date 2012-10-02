// mrbp - mruby C++ wrapper
// Copyright (c) nkshigeru 2012
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef MRBP_FUNCTION_H_
#define MRBP_FUNCTION_H_

#include <mruby.h>

#include <boost/config.hpp> //for BOOST_JOIN
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

namespace mrbp {

template<int N>
struct function_aspec
{
    static const int NUM_ARGS = N;
    int aspec()
    {
        return ARGS_REQ(N);
    }
};

template<>
struct function_aspec<0>
{
    static const int NUM_ARGS = 0;
    int aspec()
    {
        return ARGS_NONE();
    }
};

template<typename D>
struct function_base {
    mrb_func_t func()
    {
        return &D::Func;
    }
};

struct empty_function_def : function_aspec<0>
{
private:
	mrb_func_t func();
	static mrb_func_t Call();
};

template<typename T, typename F>
struct initialize_function : function_base<initialize_function<T, F> >, function_aspec<F::NUM_ARGS>
{
    static mrb_value Func(mrb_state *mrb, mrb_value self)
    {
        T* thiz = new T();

        DATA_PTR(self) = static_cast<void*>(thiz);
        DATA_TYPE(self) = class_def<T>::data_type();

        F::Func1(mrb, self, thiz);

        return self;
    }
};

template<typename T>
struct initialize_function<T, empty_function_def> : function_base<initialize_function<T, empty_function_def> >, function_aspec<empty_function_def::NUM_ARGS>
{
    static mrb_value Func(mrb_state *mrb, mrb_value self)
    {
        T* thiz = new T();

        DATA_PTR(self) = static_cast<void*>(thiz);
        DATA_TYPE(self) = class_def<T>::data_type();

        return self;
    }
};

template<typename F, F f>
struct function;

#define MRBP_NUM_ARGS 0
#include "detail/function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 1
#include "detail/function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 2
#include "detail/function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 3
#include "detail/function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 4
#include "detail/function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 0
#include "detail/member_function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 1
#include "detail/member_function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 2
#include "detail/member_function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 3
#include "detail/member_function_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 4
#include "detail/member_function_template.hpp"
#undef MRBP_NUM_ARGS

template<typename F, F f>
struct function_r;

#define MRBP_NUM_ARGS 0
#include "detail/member_function_r_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 1
#include "detail/member_function_r_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 2
#include "detail/member_function_r_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 3
#include "detail/member_function_r_template.hpp"
#undef MRBP_NUM_ARGS

#define MRBP_NUM_ARGS 4
#include "detail/member_function_r_template.hpp"
#undef MRBP_NUM_ARGS

}

#endif /*MRBP_FUNCTION_H_*/