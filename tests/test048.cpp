// test048.cpp - default conversion to default values

#include <cmath>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

template<typename T>
using ConvertFromStr_fNaN = converter::ConvertFromStr<T, converter::S2T_Format_std_CtoT<T, converter::FailureS2Tprocess::QUIET_NAN>>;

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,,x,#\n"
    "2,,y,$\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams());
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 0), "x");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 0), "#");

    ExpectException(doc.GetCell<int>(0, 0), std::invalid_argument);
    ExpectException(doc.GetCell<long long>(1, 0), std::invalid_argument);
    ExpectException(doc.GetCell<unsigned int>(2, 0), std::invalid_argument);

    //doc.GetCell<int, ConvertFromStr_fNaN<int> >(0, 0); // will not compile as 'int' doesnot support has_signaling_NaN
    //doc.GetCell<long long, ConvertFromStr_fNaN<long long> >(1, 0); // will not compile as 'long long' doesnot support has_signaling_NaN
    //doc.GetCell<unsigned int, ConvertFromStr_fNaN<unsigned int> >(2, 0); // will not compile as 'unsigned int' doesnot support has_signaling_NaN

    doc.GetCell< ConvertFromStr_fNaN<double> >(0, 1);
    unittest::ExpectTrue(std::isnan(doc.GetCell< ConvertFromStr_fNaN<double> >(0, 1)));
    unittest::ExpectTrue(std::isnan(doc.GetCell< ConvertFromStr_fNaN<long double> >(1, 1)));
    unittest::ExpectTrue(std::isnan(doc.GetCell< ConvertFromStr_fNaN<float> >(2, 1)));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
