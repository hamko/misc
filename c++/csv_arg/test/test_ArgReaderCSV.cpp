#include <cppunit/extensions/HelperMacros.h>
#include "ArgsReaderCSV.hpp"
#include <iostream>
#include <cstdio>

#define CLASSNAME testArgReaderCSV

using namespace CPPUNIT_NS;
class CLASSNAME : public TestFixture {
  CPPUNIT_TEST_SUITE(CLASSNAME);
  CPPUNIT_TEST(test_getArg);
  CPPUNIT_TEST_SUITE_END();

  void test_getArg() {
    ArgsReaderCSV Args("test.csv");
    CPPUNIT_ASSERT_EQUAL((std::string)"ccwh.txt", Args.getArg("ccwh", "filename"));
  }

  public:
  void setUp() { // 始めに呼ばれる
  }
  void tearDown() { //後で呼ばれる
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASSNAME);
#undef CLASSNAME 
