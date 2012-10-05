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

#define MRBP_FUNCTION BOOST_JOIN(member_function, MRBP_NUM_ARGS)
// ==> member_function3

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
	arg_holder<A##i>::type a##i; get(mrb, mrb->stack[(i)+(offset)+1], a##i);

template<typename R, typename T MRBP_COMMA MRBP_TEMPLATE_PARMS, R (T::*f)(MRBP_TEMPLATE_ARGS)>
struct MRBP_FUNCTION 
    : function_base<MRBP_FUNCTION<R, T MRBP_COMMA MRBP_TEMPLATE_ARGS, f> >,
      function_aspec<MRBP_NUM_ARGS>
{

    template<typename R>
    struct call {
        mrb_value operator()(R (T::*f)(MRBP_TEMPLATE_ARGS), T* thiz MRBP_COMMA MRBP_FUNTION_ARGS)
        {
            return value((thiz->*f)(MRBP_CALL_ARGS));
        }
    };
    template<>
    struct call<void> {
        mrb_value operator()(void (T::*f)(MRBP_TEMPLATE_ARGS), T* thiz MRBP_COMMA MRBP_FUNTION_ARGS)
        {
            (thiz->*f)(MRBP_CALL_ARGS);
            return mrbp::value();
        }
    };

    static mrb_value AS_METHOD(mrb_state* mrb, mrb_value self)
    {
        T* thiz = 0;
        get(mrb, self, thiz);
        if (thiz)
        {
            return MemberFunc1(mrb, self, thiz);
        }
        return mrbp::value();
    }
    static mrb_value MemberFunc1(mrb_state* mrb, mrb_value self, T* thiz)
    {
        if (mrb->ci->argc >= MRBP_NUM_ARGS)
        {
            BOOST_PP_REPEAT(MRBP_NUM_ARGS, MRBP_GET_ARGS, 0)
            return call<R>()(f, thiz MRBP_COMMA MRBP_CALL_ARGS);
        }  
        return mrbp::value();
    }
};

template<typename R, typename T MRBP_COMMA MRBP_TEMPLATE_PARMS, R (T::*f)(MRBP_TEMPLATE_ARGS)>
struct function<R (T::*)(MRBP_TEMPLATE_ARGS), f> : public MRBP_FUNCTION<R, T MRBP_COMMA MRBP_TEMPLATE_ARGS, f> {};

#undef MRBP_FUNCTION
#undef MRBP_COMMA
#undef MRBP_TEMPLATE_PARMS
#undef MRBP_TEMPLATE_ARGS
#undef MRBP_CALL_ARGS
#undef MRBP_GET_ARGS