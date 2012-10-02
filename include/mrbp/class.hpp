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

#define MRBP_DEFINE_CLASS(CLS, NAME)                \
namespace mrbp                                      \
{                                                   \
template<>                                          \
struct class_def<CLS> : public class_def_base<CLS>  \
{                                                   \
    static const char* name()                       \
    {                                               \
        return NAME;                                \
    }                                               \
};                                                  \
}

template<typename T>
struct class_init
{
    typedef class_def<T> class_def_t;

    class_init<T>(mrb_state* mrb) : mrb(mrb)
    {
        cls = mrb_define_class(mrb, class_def_t::name(), mrb->object_class);
        MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
    }
	
    template<typename FDef>
    class_init<T>& define_initialize(FDef& fdef)
    {
        initialize_function<T, FDef> wrap_initialize;
        mrb_define_method(mrb, cls, "initialize", wrap_initialize.func(), wrap_initialize.aspec());
        return *this;
    }

    class_init<T>& define_initialize()
    {
		return define_initialize(empty_function_def());
    }

    class_init<T>& define_method(const char* func_name, mrb_func_t func, int aspec)
    {
        mrb_define_method(mrb, cls, func_name, func, aspec);
        return *this;
    }

    template<typename FDef>
    class_init<T>& define_method(const char* func_name, FDef& fdef)
    {
        return define_method(func_name, fdef.func(), fdef.aspec());
    }
	
	mrb_state* mrb;
    RClass* cls;

private:
    class_init<T>();
	class_init<T>(const class_init<T>&);
	class_init<T>& operator=(const class_init<T>&);
    
};

}

#endif /*MRBP_CLASS_H_*/