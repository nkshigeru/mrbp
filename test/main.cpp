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

#define BOOST_TEST_MODULE mrbp_test
#include <boost/test/unit_test.hpp>

#include <mruby.h>
#include <mrbp/mrbp.hpp>

struct mrb_fixture
{
    mrb_fixture()
    {
        mrb = mrb_open();
    }
    ~mrb_fixture()
    {
        mrb_close(mrb);
    }
    mrb_state* mrb;
};

BOOST_FIXTURE_TEST_SUITE(s, mrb_fixture)

BOOST_AUTO_TEST_CASE(value_test)
{
    // nil value
    mrb_value nil_value = mrbp::value();
    BOOST_CHECK(mrb_nil_p(nil_value));

    // bool value
    mrb_value true_value = mrbp::value(true);
    BOOST_CHECK(mrb_type(true_value) == MRB_TT_TRUE);

    mrb_value false_value = mrbp::value(false);
    BOOST_CHECK(mrb_type(false_value) == MRB_TT_FALSE && mrb_fixnum(false_value));

    // int value
    mrb_value int_value = mrbp::value(1);
    BOOST_CHECK(mrb_type(int_value) == MRB_TT_FIXNUM);
    BOOST_CHECK(mrb_fixnum(int_value) == 1);

    // float value
    mrb_value float_value = mrbp::value(0.5);
    BOOST_CHECK(mrb_type(float_value) == MRB_TT_FLOAT);
    BOOST_CHECK(mrb_float(float_value) == 0.5);

    // any object pointer
    struct RClass* cls = mrb_class_new(mrb, mrb->object_class);
    mrb_value class_value = mrbp::value(cls);
    BOOST_CHECK(mrb_type(class_value) == MRB_TT_CLASS);
    BOOST_CHECK((void*)mrb_object(class_value) == (void*)cls);

    // any mrb_value
    mrb_value a = mrb_fixnum_value(1);
    mrb_value any_value = mrbp::value(a);
    BOOST_CHECK(mrb_obj_equal(mrb, any_value, a));
}

BOOST_AUTO_TEST_CASE(check_test)
{
    mrb_value nil_value = mrb_nil_value();
    mrb_value true_value = mrb_true_value();
    mrb_value false_value = mrb_false_value();
    mrb_value int_value = mrb_fixnum_value(1);
    mrb_value float_value = mrb_float_value(0.5);

    // check bool
    BOOST_CHECK( !mrbp::check<bool>(nil_value) );
    BOOST_CHECK( mrbp::check<bool>(true_value) );
    BOOST_CHECK( mrbp::check<bool>(false_value) );
    BOOST_CHECK( !mrbp::check<bool>(int_value) );
    BOOST_CHECK( !mrbp::check<bool>(float_value) );

    // check int
    BOOST_CHECK( !mrbp::check<mrb_int>(nil_value) );
    BOOST_CHECK( !mrbp::check<mrb_int>(true_value) );
    BOOST_CHECK( !mrbp::check<mrb_int>(false_value) );
    BOOST_CHECK( mrbp::check<mrb_int>(int_value) );
    BOOST_CHECK( !mrbp::check<mrb_int>(float_value) );

    // check float
    BOOST_CHECK( !mrbp::check<mrb_float>(nil_value) );
    BOOST_CHECK( !mrbp::check<mrb_float>(true_value) );
    BOOST_CHECK( !mrbp::check<mrb_float>(false_value) );
    BOOST_CHECK( !mrbp::check<mrb_float>(int_value) );
    BOOST_CHECK( mrbp::check<mrb_float>(float_value) );

    // check mrb_value
    BOOST_CHECK( mrbp::check<mrb_value>(nil_value) );
    BOOST_CHECK( mrbp::check<mrb_value>(true_value) );
    BOOST_CHECK( mrbp::check<mrb_value>(false_value) );
    BOOST_CHECK( mrbp::check<mrb_value>(int_value) );
    BOOST_CHECK( mrbp::check<mrb_value>(float_value) );
}

BOOST_AUTO_TEST_CASE(get_test)
{
    mrb_value nil_value = mrb_nil_value();
    mrb_value true_value = mrb_true_value();
    mrb_value false_value = mrb_false_value();
    mrb_value int_value = mrb_fixnum_value(1);
    mrb_value float_value = mrb_float_value(0.5);

    // get bool
    bool out_bool_value;
    mrbp::get(mrb, nil_value, out_bool_value);
    BOOST_CHECK( !out_bool_value );
    mrbp::get(mrb, true_value, out_bool_value);
    BOOST_CHECK( out_bool_value );
    mrbp::get(mrb, false_value, out_bool_value);
    BOOST_CHECK( !out_bool_value );
    mrbp::get(mrb, int_value, out_bool_value);
    BOOST_CHECK( out_bool_value );
    mrbp::get(mrb, float_value, out_bool_value);
    BOOST_CHECK( out_bool_value );

    // get int
    mrb_int out_int_value;
    mrbp::get(mrb, nil_value, out_int_value);
    BOOST_CHECK( out_int_value == 0 );
    BOOST_CHECK_THROW( mrbp::get(mrb, true_value, out_int_value), std::exception );
    mrbp::get(mrb, false_value, out_int_value);
    BOOST_CHECK( out_int_value == 0 );
    mrbp::get(mrb, int_value, out_int_value);
    BOOST_CHECK( out_int_value == 1 );
    mrbp::get(mrb, float_value, out_int_value);
    BOOST_CHECK( out_int_value == 0 );

    // get float
    mrb_float out_float_value;
    mrbp::get(mrb, nil_value, out_float_value);
    BOOST_CHECK( out_float_value == 0 );
    BOOST_CHECK_THROW( mrbp::get(mrb, true_value, out_float_value), std::exception );
    mrbp::get(mrb, false_value, out_float_value);
    BOOST_CHECK( out_float_value == 0 );
    mrbp::get(mrb, int_value, out_float_value);
    BOOST_CHECK( out_float_value == 1 );
    mrbp::get(mrb, float_value, out_float_value);
    BOOST_CHECK( out_float_value == 0.5 );

    // get mrb_value
    mrb_value out_value;
    mrbp::get(mrb, nil_value, out_value);
    BOOST_CHECK(mrb_obj_equal(mrb, nil_value, out_value));
    mrbp::get(mrb, true_value, out_value);
    BOOST_CHECK(mrb_obj_equal(mrb, true_value, out_value));
    mrbp::get(mrb, false_value, out_value);
    BOOST_CHECK(mrb_obj_equal(mrb, false_value, out_value));
    mrbp::get(mrb, int_value, out_value);
    BOOST_CHECK(mrb_obj_equal(mrb, int_value, out_value));
    mrbp::get(mrb, float_value, out_value);
    BOOST_CHECK(mrb_obj_equal(mrb, float_value, out_value));
}

BOOST_AUTO_TEST_SUITE_END()

