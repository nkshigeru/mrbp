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

namespace mrbp {

static inline mrb_value value()
{
    return mrb_nil_value();
}

static inline mrb_value value(int v)
{
    return mrb_fixnum_value(v);
}

static inline mrb_value value(mrb_value v)
{
    return v;
}

template<typename T>
static inline bool check(mrb_value value);

template<>
static inline bool check<mrb_value>(mrb_value value)
{
	return true;
}

static inline void get(mrb_value value, mrb_value& out)
{
	out = value;
}

static inline void get(mrb_value value, int& out)
{
	out = mrb_fixnum(value);
}

}

#endif /*MRBP_VALUE_H_*/