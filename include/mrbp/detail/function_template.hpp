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

#define MRBP_FUNCTION BOOST_JOIN(function, MRBP_NUM_ARGS)
// ==> function3

#if  MRBP_NUM_ARGS == 0
#  define MRBP_COMMA
#else
#  define MRBP_COMMA ,
#endif // MRBP_NUM_ARGS > 0

#define MRBP_TEMPLATE_PARMS BOOST_PP_ENUM_PARAMS(MRBP_NUM_ARGS, typename A)
// ==> typename A0, typename A1, typename A2 
#define MRBP_TEMPLATE_ARGS BOOST_PP_ENUM_PARAMS(MRBP_NUM_ARGS, A)
// ==> A0, A1, A2
#define MRBP_FUNTION_ARGS BOOST_PP_ENUM_BINARY_PARAMS(MRBP_NUM_ARGS, A, a)
// ==> A0 a0, A1 a1, A2 a2
#define MRBP_CALL_ARGS BOOST_PP_ENUM_PARAMS(MRBP_NUM_ARGS, a)
// ==> a0, a1, a2

#define MRBP_GET_ARGS(x, i, offset) \
	A##i a##i; get(mrb->stack[(i)+(offset)+1], a##i);

template<typename R MRBP_COMMA MRBP_TEMPLATE_PARMS>
static inline mrb_value call(R (*f)(MRBP_TEMPLATE_ARGS) MRBP_COMMA MRBP_FUNTION_ARGS)
{
    return value((*f)(MRBP_CALL_ARGS));
}

template<MRBP_TEMPLATE_PARMS>
static inline mrb_value call(void (*f)(MRBP_TEMPLATE_ARGS) MRBP_COMMA MRBP_FUNTION_ARGS)
{
    (*f)(MRBP_CALL_ARGS);
    return mrbp::value();
}

template<typename R MRBP_COMMA MRBP_TEMPLATE_PARMS, R (*f)(MRBP_TEMPLATE_ARGS)>
struct MRBP_FUNCTION 
    : function_base<MRBP_FUNCTION<R MRBP_COMMA MRBP_TEMPLATE_ARGS, f> >,
      function_aspec<MRBP_NUM_ARGS>
{
    static mrb_value Func(mrb_state* mrb, mrb_value self)
    {
        if (mrb->ci->argc >= MRBP_NUM_ARGS)
        {
            BOOST_PP_REPEAT(MRBP_NUM_ARGS, MRBP_GET_ARGS, 0)
            return call(f, MRBP_CALL_ARGS);
        }
        return mrbp::value();
    }
#if  MRBP_NUM_ARGS == 0

#else
    static mrb_value Func1(mrb_state* mrb, mrb_value self, A0 a0)
    {
        if (mrb->ci->argc >= (MRBP_NUM_ARGS-1))
        {
	        BOOST_PP_REPEAT_FROM_TO(1, MRBP_NUM_ARGS, MRBP_GET_ARGS, -1)
            return call(f, MRBP_CALL_ARGS);
        }
        return mrbp::value();
    }
#endif
};

template<typename R MRBP_COMMA MRBP_TEMPLATE_PARMS, R (*f)(MRBP_TEMPLATE_ARGS)>
struct function<R (*)(MRBP_TEMPLATE_ARGS), f> : public MRBP_FUNCTION<R MRBP_COMMA MRBP_TEMPLATE_ARGS, f> {};

#undef MRBP_FUNCTION
#undef MRBP_COMMA
#undef MRBP_TEMPLATE_PARMS
#undef MRBP_TEMPLATE_ARGS
#undef MRBP_CALL_ARGS
#undef MRBP_GET_ARGS