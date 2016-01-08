#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <cstdio>

#define CLASSNAME test

using namespace CPPUNIT_NS;
class CLASSNAME : public TestFixture {
    CPPUNIT_TEST_SUITE(CLASSNAME);
    CPPUNIT_TEST(test_double);
    CPPUNIT_TEST(test_int);
    CPPUNIT_TEST_SUITE_END();

    void test_double() {
        double a = 2.2, b = 5.8;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(8, a+b, 1e-3);
    }
    void test_int() {
        int a = 1;
        CPPUNIT_ASSERT_EQUAL(1, a);
    }


    public:
    void setUp() { // 始めに呼ばれる
    }
    void tearDown() { //後で呼ばれる
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASSNAME);
#undef CLASSNAME 
