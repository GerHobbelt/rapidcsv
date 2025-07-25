// test036.cpp - supported datatypes

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"
#include "utilities.h"

/*
using BigFloat do a precision comparison later. TODO
#include "BigFloat.h"
*/


int main()
{
  int rv = 0;

  std::string input[] = { "32767", "2147483647", "9223372036854775807",
                          "65535", "4294967295", "18446744073709551615",
                          "3.3123412E38", "1.6E308", "1.6E308",
                          "1.6e+308", "8.589973e9", "9007199254740993",
                          "1.123456789012345678901234567890",
                          "1.123456789012345678901234567890",
                          "1.123456789012345678901234567890" };

  std::string csv =
    "int,"+input[0]+"\n"
    "long,"+input[1]+"\n"
    "long long,"+input[2]+"\n"
    "unsigned,"+input[3]+"\n"
    "unsigned long,"+input[4]+"\n"
    "unsigned long long,"+input[5]+"\n"
    "float,"+input[6]+"\n"
    "double,"+input[7]+"\n"
    "long double,"+input[8]+"\n"
    "long double,"+input[9]+"\n"
    "float,"+input[10]+"\n"
    "double,"+input[11]+"\n"
    "float,"+input[12]+"\n"
    "double,"+input[13]+"\n"
    "long double,"+input[14]+"\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING, rapidcsv::FlgRowName::RN_MISSING));

    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0), 32767);

    unittest::ExpectEqual(long, doc.GetCell<long>(1, 1), 2147483647);

    unittest::ExpectEqual(long long, doc.GetCell<long long>(1, 2), 9223372036854775807);

    unittest::ExpectEqual(unsigned, doc.GetCell<unsigned>(1, 3), 65535);

    unittest::ExpectEqual(unsigned long, doc.GetCell<unsigned long>(1, 4), 4294967295);

    unittest::ExpectEqual(unsigned long long, doc.GetCell<unsigned long long>(1, 5), 18446744073709551615llu);

    // refer https://en.cppreference.com/w/cpp/io/manip/fixed

    unittest::STRING2VALUECHECK(float, "test036-06",              \
          input[6], doc.GetCell<float>(1, 6), 3.3123412E38L,      \
          doc.GetCell<std::string>(1, 6));

    unittest::STRING2VALUECHECK(double, "test036-07",             \
          input[7], doc.GetCell<double>(1, 7), 1.6E308,           \
          doc.GetCell<std::string>(1, 7));

    unittest::STRING2VALUECHECK(long double, "test036-08",        \
          input[8], doc.GetCell<long double>(1, 8), 1.6E308L,     \
          doc.GetCell<std::string>(1, 8));

    unittest::STRING2VALUECHECK(long double, "test036-09",        \
          input[9], doc.GetCell<long double>(1, 9), 1.6E308L,     \
          doc.GetCell<std::string>(1, 9));

    unittest::STRING2VALUECHECK(float, "test036-10",              \
          input[10], doc.GetCell<float>(1, 10), 8.589973e9f,      \
          doc.GetCell<std::string>(1, 10));

    unittest::STRING2VALUECHECK(double, "test036-11",                   \
          input[11], doc.GetCell<double>(1, 11), 9007199254740993,      \
          doc.GetCell<std::string>(1, 11));

    unittest::STRING2VALUECHECK(float, "test036-12",                               \
          input[12], doc.GetCell<float>(1, 12), 1.123456789012345678901234567890L, \
          doc.GetCell<std::string>(1, 12));

    unittest::STRING2VALUECHECK(double, "test036-13",                               \
          input[13], doc.GetCell<double>(1, 13), 1.123456789012345678901234567890L, \
          doc.GetCell<std::string>(1, 13));

    unittest::STRING2VALUECHECK(long double, "test036-14",                               \
          input[14], doc.GetCell<long double>(1, 14), 1.123456789012345678901234567890L, \
          doc.GetCell<std::string>(1, 14));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
