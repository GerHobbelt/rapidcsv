// test049.cpp - default conversion to custom values

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

template<typename T>
using ConvertFromStr_gNaN = converter::ConvertFromStr<T, converter::S2T_Format_std_CtoT<T, converter::FailureS2Tprocess::VARIANT_NAN>>;


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

    //doc.GetCell< ConvertFromStr_gNaN<int> >(0, 1);
    std::variant<int, std::string> val_i = doc.GetCell< ConvertFromStr_gNaN<int> >(0, 1);
    unittest::ExpectEqual(size_t, val_i.index(),1);
    unittest::ExpectEqual(std::string, std::get<std::string>(val_i), std::string(""));

    std::variant<long long, std::string> val_ll = doc.GetCell< ConvertFromStr_gNaN<long long> >(1, 1);
    unittest::ExpectEqual(size_t, val_ll.index(),1);
    unittest::ExpectEqual(std::string, std::get<std::string>(val_ll), std::string("y"));

    std::variant<unsigned int, std::string> val_ui = doc.GetCell< ConvertFromStr_gNaN<unsigned int> >(2, 1);
    unittest::ExpectEqual(size_t, val_ui.index(),1);
    unittest::ExpectEqual(std::string, std::get<std::string>(val_ui), std::string("$"));

    //doc.GetCell< ConvertFromStr_gNaN<double> >(0, 1);
    std::variant<double, std::string> val_d = doc.GetCell< ConvertFromStr_gNaN<double> >(0, 1);
    unittest::ExpectEqual(size_t, val_d.index(),1);
    unittest::ExpectEqual(std::string, std::get<std::string>(val_d), std::string(""));

    std::variant<long double, std::string> val_ld = doc.GetCell< ConvertFromStr_gNaN<long double> >(1, 1);
    unittest::ExpectEqual(size_t, val_ld.index(),1);
    unittest::ExpectEqual(std::string, std::get<std::string>(val_ld), std::string("y"));

    std::variant<float, std::string> val_f = doc.GetCell< ConvertFromStr_gNaN<float> >(2, 1);
    unittest::ExpectEqual(size_t, val_f.index(),1);
    unittest::ExpectEqual(std::string, std::get<std::string>(val_f), std::string("$"));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
