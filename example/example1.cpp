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

#include <iostream>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/array.h>

#include <mrbp/mrbp.hpp>

char rb_code[] =
    "p 'mrbp example1'    \n"
    "obj = MyClass.new    \n"
    "p obj                \n"
    "p obj.VoidFunction0  \n"
    "p obj.IntFunction0   \n"
    "p obj.Calc(3, 5)     \n"
    "p obj.GetArray(6)    \n";

/* expected output is

"mrbp example1"
-- MyClass::MyClass()
#<MyClass:0xc791d4>
-- MyClass::VoidFunction0()
nil
-- MyClass::IntFunction0()
1
-- MyClass::Calc(3 ,5)
8
-- MyClass::GetArray(6)
[0, 1, 2, 3, 4, 5]
-- MyClass::~MyClass()

*/

class MyClass
{
public:
    MyClass()
    {
        std::cout << "-- MyClass::MyClass()" << std::endl;
    }
    ~MyClass()
    {
        std::cout << "-- MyClass::~MyClass()" << std::endl;
    }
    void VoidFunction0()
    {
        std::cout << "-- MyClass::VoidFunction0()" << std::endl;
    }
    int IntFunction0()
    {
        std::cout << "-- MyClass::IntFunction0()" << std::endl;
        return 1;
    }
    int Calc(int a, int b)
    {
        std::cout << "-- MyClass::Calc(" << a << " ," << b << ")" << std::endl;
        return a + b;
    }
    mrb_value GetArray(mrb_state* mrb, mrb_value self, int n)
    {
        std::cout << "-- MyClass::GetArray(" << n << ")" << std::endl;
        mrb_value ary = mrb_ary_new(mrb);
        for(int i = 0; i < n; ++i)
        {
            mrb_ary_push(mrb, ary, mrbp::value(i));
        }
        return ary;
    }
};

MRBP_DEFINE_CLASS(MyClass)

int main()
{
    mrb_state* mrb = mrb_open();

    mrbp::class_init<MyClass>(mrb, "MyClass")
        .define_initialize()
        .define_method("VoidFunction0", mrbp::function<void (MyClass::*)(), &MyClass::VoidFunction0>())
        .define_method("IntFunction0", mrbp::function<int (MyClass::*)(), &MyClass::IntFunction0>())
        .define_method("Calc", mrbp::function<int (MyClass::*)(int, int), &MyClass::Calc>())
        .define_method("GetArray", mrbp::function_r<mrb_value (MyClass::*)(mrb_state* mrb, mrb_value self, int), &MyClass::GetArray>());

    mrb_load_string(mrb, rb_code);

    mrb_close(mrb);

#if defined(_MSC_VER) & defined(_DEBUG)
    system("PAUSE");
#endif

    return 0;
}