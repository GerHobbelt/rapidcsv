// test048.cpp - default conversion to default values

#include <cmath>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

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

    //doc.GetCell<int, &rapidcsv::ConvertFromStr_fNaN<int>::ToVal>(0, 0); // will not compile
    //doc.GetCell<long long, &rapidcsv::ConvertFromStr_fNaN<long long>::ToVal>(1, 0); // will not compile
    //doc.GetCell<unsigned int, &rapidcsv::ConvertFromStr_fNaN<unsigned int>::ToVal>(2, 0); // will not compile

    //doc.GetCell<double, &rapidcsv::ConvertFromStr_fNaN<double>::ToVal>(0, 1);
    unittest::ExpectTrue(std::isnan(doc.GetCell<double COMMA &rapidcsv::ConvertFromStr_fNaN<double>::ToVal>(0, 1)));
    unittest::ExpectTrue(std::isnan(doc.GetCell<long double COMMA &rapidcsv::ConvertFromStr_fNaN<long double>::ToVal>(1, 1)));
    unittest::ExpectTrue(std::isnan(doc.GetCell<float COMMA &rapidcsv::ConvertFromStr_fNaN<float>::ToVal>(2, 1)));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
