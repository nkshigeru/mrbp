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

#ifndef MRBP_CLASS_H_
#define MRBP_CLASS_H_

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/class.h>

#include "mrbp/function.hpp"

namespace mrbp {

template<typename T>
static inline void wrap_delete(mrb_state* mrb, void* ptr)
{
    delete static_cast<T*>(ptr);
}

template<typename T>
struct class_def {};

template<typename T>
struct class_def_base
{
    typedef void (*free_func_t)(mrb_state*, void*);
    static free_func_t free_func()
    {
        return &wrap_delete<T>;
    }

    static struct mrb_data_type* data_type() {
        static struct mrb_data_type static_data_type = { class_def<T>::name(), class_def<T>::free_func() };
        return &static_data_type;
    }
};

#define MRBP_DEFINE_CLASS(CLS)                      \
namespace mrbp                                      \
{                                                   \
template<>                                          \
struct class_def<CLS> : public class_def_base<CLS>  \
{                                                   \
    static const char* name()                       \
    {                                               \
        return #CLS;                                \
    }                                               \
};                                                  \
}

template<typename T>
struct class_init_under_module;

struct module_init
{
    module_init(mrb_state* mrb, const char* name) : mrb(mrb)
    {
        cls = mrb_define_module(mrb, name);
    }
    
    module_init& define_module_function(const char* func_name, mrb_func_t func, int aspec)
    {
        mrb_define_module_function(mrb, cls, func_name, func, aspec);
        return *this;
    }
    
    template<typename F>
    module_init& define_module_function(const char* func_name, F&)
    {
        return define_module_function(func_name, &F::AS_CLASS_METHOD, F::ASPEC);
    }
    
    template<typename V>
    module_init& define_const(const char* name, V v)
    {
        mrb_define_const(mrb, cls, name, value(v));
        return *this;
    }
    
    template<typename C>
    class_init_under_module<C> begin_class_init(const char* name)
    {
        return class_init_under_module<C>(*this, name);
    }
    
    mrb_state* mrb;
    struct RClass* cls;
};

template<typename T, typename D>
struct class_init_impl
{
    typedef class_def<T> class_def_t;

    class_init_impl(mrb_state* mrb) : mrb(mrb) {}
    class_init_impl(mrb_state* mrb, struct RClass* cls) : mrb(mrb), cls(cls) {}

    template<typename F>
    D& define_initialize(F&)
    {
        mrb_define_method(mrb, cls, "initialize", &initialize_function<T, F>::AS_METHOD, initialize_function<T, F>::ASPEC);
        return static_cast<D&>(*this);
    }

    D& define_initialize()
    {
		return define_initialize(empty_function_def());
    }

    D& define_method(const char* func_name, mrb_func_t func, int aspec)
    {
        mrb_define_method(mrb, cls, func_name, func, aspec);
        return static_cast<D&>(*this);
    }

    template<typename F>
    D& define_method(const char* func_name, F&)
    {
        return define_method(func_name, &F::AS_METHOD, F::ASPEC);
    }

    D& define_class_method(const char* func_name, mrb_func_t func, int aspec)
    {
        mrb_define_class_method(mrb, cls, func_name, func, aspec);
        return static_cast<D&>(*this);
    }

    template<typename F>
    D& define_class_method(const char* func_name, F&)
    {
        return define_class_method(func_name, &F::AS_CLASS_METHOD, F::ASPEC);
    }
    
    template<typename V>
    D& define_const(const char* name, V v)
    {
        mrb_define_const(mrb, cls, name, value(v));
        return static_cast<D&>(*this);
    }
    
    mrb_state* mrb;
    struct RClass* cls;
    
private:
    class_init_impl();
};

template<typename T>
struct class_init : class_init_impl<T, class_init<T> >
{
    typedef class_init_impl<T, class_init<T> > super_t;

    class_init(mrb_state* mrb, const char* name) : super_t(mrb)
    {
        cls = mrb_define_class(mrb, name, mrb->object_class);
        MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
    }

    class_init(mrb_state* mrb, struct RClass* cls) : super_t(mrb, cls)
    {
    }
	
private:
    class_init();
    
};

template<typename T>
struct class_init_under_module : class_init_impl<T, class_init_under_module<T> >
{
    typedef class_init_impl<T, class_init_under_module<T> > super_t;

    class_init_under_module(module_init& outer, const char* name) : super_t(outer.mrb), outer(outer)
    {
        cls = mrb_define_class_under(mrb, outer.cls, name, mrb->object_class);
        MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
    }
    
    module_init& end_class_init()
    {
        return outer;
    }
    
    module_init& outer;
};

}

#endif /*MRBP_CLASS_H_*/