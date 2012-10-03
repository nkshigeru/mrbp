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

#ifndef MRBP_VALUE_H_
#define MRBP_VALUE_H_

#include <mruby.h>
#include <mruby/numeric.h>

namespace mrbp {

//nil
static inline mrb_value value()
{
    return mrb_nil_value();
}

//bool
static inline mrb_value value(bool v)
{
    if (v)
    {
        return mrb_true_value();
    }
    else
    {
        return mrb_false_value();
    }
}

//int
static inline mrb_value value(mrb_int v)
{
    return mrb_fixnum_value(v);
}

//float
static inline mrb_value value(mrb_float v)
{
    return mrb_float_value(v);
}

//object
static inline mrb_value value(void* v)
{
    return mrb_obj_value(v);
}

//mrb_value
static inline mrb_value value(mrb_value v)
{
    return v;
}

template<typename T>
static inline bool check(mrb_value value);

template<>
static inline bool check<bool>(mrb_value value)
{
	return (mrb_type(value) == MRB_TT_TRUE) ||
        (mrb_type(value) == MRB_TT_FALSE && mrb_fixnum(value));
}

template<>
static inline bool check<mrb_int>(mrb_value value)
{
	return mrb_type(value) == MRB_TT_FIXNUM;
}

template<>
static inline bool check<mrb_float>(mrb_value value)
{
	return mrb_type(value) == MRB_TT_FLOAT;
}

template<>
static inline bool check<mrb_value>(mrb_value value)
{
	return true;
}

static inline void get(mrb_value value, bool& out)
{
	out = (mrb_type(value) != MRB_TT_FALSE);
}

static inline void get(mrb_value value, mrb_int& out)
{
    switch (mrb_type(value))
    {
    case MRB_TT_FIXNUM:
	    out = mrb_fixnum(value);
        break;
    case MRB_TT_FLOAT:
	    {
            mrb_float f = mrb_float(value);

            if (!FIXABLE(f)) {
                throw std::range_error("float too big for int");
            }
            out = static_cast<mrb_int>(f);
        }
        break;
    case MRB_TT_FALSE:
        out = 0;
        break;
    default:
        throw std::invalid_argument("can't convert to int");
        break;
    }
}

static inline void get(mrb_value value, mrb_float& out)
{
    switch (mrb_type(value))
    {
    case MRB_TT_FLOAT:
        out = mrb_float(value);
        break;
    case MRB_TT_FIXNUM:
	    out = static_cast<mrb_int>(mrb_fixnum(value));
        break;
    case MRB_TT_FALSE:
        out = 0;
        break;
    default:
        throw std::invalid_argument("can't convert to float");
        break;
    }
}

static inline void get(mrb_value value, mrb_value& out)
{
	out = value;
}



}

#endif /*MRBP_VALUE_H_*/