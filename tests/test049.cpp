// test049.cpp - default conversion to custom values

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

    //doc.GetCell<int, &rapidcsv::ConvertFromStr_gNaN<int>::ToVal>(0, 1);
    std::variant<int, rapidcsv::gNaN> val_i = doc.GetCell<int, &rapidcsv::ConvertFromStr_gNaN<int>::ToVal>(0, 1);
    unittest::ExpectEqual(int, val_i.index(),1);
    unittest::ExpectEqual(int, int(std::get<rapidcsv::gNaN>(val_i)), int(rapidcsv::gNaN::isNull));
    std::variant<long long, rapidcsv::gNaN> val_ll = doc.GetCell<long long, &rapidcsv::ConvertFromStr_gNaN<long long>::ToVal>(1, 1);
    unittest::ExpectEqual(int, val_ll.index(),1);
    unittest::ExpectEqual(int, int(std::get<rapidcsv::gNaN>(val_ll)), int(rapidcsv::gNaN::isNaN));
    std::variant<unsigned int, rapidcsv::gNaN> val_ui = doc.GetCell<unsigned int, &rapidcsv::ConvertFromStr_gNaN<unsigned int>::ToVal>(2, 1);
    unittest::ExpectEqual(int, val_ui.index(),1);
    unittest::ExpectEqual(int, int(std::get<rapidcsv::gNaN>(val_ui)), int(rapidcsv::gNaN::isNaN));

    //doc.GetCell<double, &rapidcsv::ConvertFromStr_gNaN<double>::ToVal>(0, 1);
    std::variant<double, rapidcsv::gNaN> val_d = doc.GetCell<double, &rapidcsv::ConvertFromStr_gNaN<double>::ToVal>(0, 1);
    unittest::ExpectEqual(int, val_d.index(),1);
    unittest::ExpectEqual(int, int(std::get<rapidcsv::gNaN>(val_d)), int(rapidcsv::gNaN::isNull));
    std::variant<long double, rapidcsv::gNaN> val_ld = doc.GetCell<long double, &rapidcsv::ConvertFromStr_gNaN<long double>::ToVal>(1, 1);
    unittest::ExpectEqual(int, val_ld.index(),1);
    unittest::ExpectEqual(int, int(std::get<rapidcsv::gNaN>(val_ld)), int(rapidcsv::gNaN::isNaN));
    std::variant<float, rapidcsv::gNaN> val_f = doc.GetCell<float, &rapidcsv::ConvertFromStr_gNaN<float>::ToVal>(2, 1);
    unittest::ExpectEqual(int, val_f.index(),1);
    unittest::ExpectEqual(int, int(std::get<rapidcsv::gNaN>(val_f)), int(rapidcsv::gNaN::isNaN));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
