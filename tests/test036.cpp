// test036.cpp - supported datatypes

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

/*
using BigFloat do a precision comparison later. TODO
#include "BigFloat.h"
*/

#define GET_TYPENAME(T) #T

template<typename T>
void checkRoundTripConversion_txt2Val2txt(
            int testID, const std::string& type,
            const std::string& str, const T& val)
{
  std::ostringstream oss;
  //oss << std::setprecision(std::numeric_limits<T>::digits10 + 1);
  oss << std::setprecision(LDBL_DIG + 5);
  oss << val;

  if(str.compare(oss.str())!=0)
  {
    // TODO use BigFloat comparision here
    std::cout << std::setprecision(LDBL_DIG + 5);
    std::cout << "test036 :: round conversion value does not match for testID=" << testID << " type=" << type << " ..." << std::endl;
    std::cout << "           text1{" << str << "} ->   val{" << val << "}" << std::endl;
    std::cout << "             val{" << val << "} -> text2{" << oss.str() << "}" << std::endl;
    std::cout << "           text2{" << oss.str() << "} != text1{" << str << "}" << std::endl << std::endl;
  }
}

int main()
{
  int rv = 0;

  std::string input[] = { "32767", "2147483647", "9223372036854775807",
                          "65535", "4294967295", "18446744073709551615",
                          "3.3123412E38f", "1.6E308", "1.6E308L",
                          "1.6e+308", "8.589973e9f", "9007199254740993",
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
    checkRoundTripConversion_txt2Val2txt(0, GET_TYPENAME(int),
                              input[0], doc.GetCell<int>(1, 0));

    unittest::ExpectEqual(long, doc.GetCell<long>(1, 1), 2147483647);
    checkRoundTripConversion_txt2Val2txt(1, GET_TYPENAME(long),
                              input[1], doc.GetCell<long>(1, 1));

    unittest::ExpectEqual(long long, doc.GetCell<long long>(1, 2), 9223372036854775807);
    checkRoundTripConversion_txt2Val2txt(2, GET_TYPENAME(long long),
                              input[2], doc.GetCell<long long>(1, 2));

    unittest::ExpectEqual(unsigned, doc.GetCell<unsigned>(1, 3), 65535);
    checkRoundTripConversion_txt2Val2txt(3, GET_TYPENAME(unsigned),
                              input[3], doc.GetCell<unsigned>(1, 3));

    unittest::ExpectEqual(unsigned long, doc.GetCell<unsigned long>(1, 4), 4294967295);
    checkRoundTripConversion_txt2Val2txt(4, GET_TYPENAME(unsigned long),
                          input[4], doc.GetCell<unsigned long>(1, 4));

    unittest::ExpectEqual(unsigned long long, doc.GetCell<unsigned long long>(1, 5), 18446744073709551615llu);
    checkRoundTripConversion_txt2Val2txt(5, GET_TYPENAME(unsigned long long),
                          input[5], doc.GetCell<unsigned long long>(1, 5));

    // refer https://en.cppreference.com/w/cpp/io/manip/fixed

    checkRoundTripConversion_txt2Val2txt(6, GET_TYPENAME(float),
                          input[6], doc.GetCell<float>(1, 6));

    checkRoundTripConversion_txt2Val2txt(7, GET_TYPENAME(double),
                          input[7], doc.GetCell<double>(1, 7));

    checkRoundTripConversion_txt2Val2txt(8, GET_TYPENAME(long double),
                          input[8], doc.GetCell<long double>(1, 8));

    unittest::ExpectEqual(long double, doc.GetCell<long double>(1, 9), 1.6e+308L);
    checkRoundTripConversion_txt2Val2txt(9, GET_TYPENAME(long double),
                          input[9], doc.GetCell<long double>(1, 9));

    checkRoundTripConversion_txt2Val2txt(10, GET_TYPENAME(float),
                          input[10], doc.GetCell<float>(1, 10));

    checkRoundTripConversion_txt2Val2txt(11, GET_TYPENAME(double),
                          input[11], doc.GetCell<double>(1, 11));

    checkRoundTripConversion_txt2Val2txt(12, GET_TYPENAME(float),
                          input[12], doc.GetCell<float>(1, 12));

    checkRoundTripConversion_txt2Val2txt(13, GET_TYPENAME(double),
                          input[13], doc.GetCell<double>(1, 13));

    checkRoundTripConversion_txt2Val2txt(14, GET_TYPENAME(long double),
                          input[14], doc.GetCell<long double>(1, 14));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
