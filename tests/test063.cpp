// test063.cpp - custom conversion per call

#include <iomanip>
#include <math.h>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

// Conversion function for int data to be converted to fixed-point two decimal numbers
int ToVal(const std::string& pStr)
{
  return static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

struct StrVal
{
  int val = 0;
};

StrVal ToStrVal(const std::string& pStr)
{
  StrVal sVal;
  sVal.val = static_cast<int>(roundf(100.0f * std::stof(pStr)));
  return sVal;
}

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C,D\n"
    "1,1,10,100,1000\n"
    "2,0.1,0.01,0.001,0.006\n"
    "3,0.3,0.03,xyz,\n"
    "4,0.4,0.04,,abc\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    // ToVal
    unittest::ExpectEqual(int, doc.GetCell<ToVal>(0, 0), 100);
    unittest::ExpectEqual(int, doc.GetCell<ToVal>(1, "1"), 1000);
    unittest::ExpectEqual(int, doc.GetCell<ToVal>("A", 0), 100);
    unittest::ExpectEqual(int, doc.GetCell<ToVal>("B", "1"), 1000);

    using t_raw100multi = rapidcsv::S2TwrapperFunction<int, ToVal>;
    using t_wrp100multi = rapidcsv::S2TwrapperFunction<StrVal, ToStrVal>;

    std::tuple<int, long, int, StrVal> row1 = doc.GetRow< int, long,
                                                          t_raw100multi, t_wrp100multi >(0);

    unittest::ExpectEqual(int, std::get<0>(row1),  1);
    unittest::ExpectEqual(long, std::get<1>(row1),  10);
    unittest::ExpectEqual(int, std::get<2>(row1),  10000);
    unittest::ExpectEqual(int, std::get<3>(row1).val,  100000);

    unittest::ExpectEqual(int, doc.GetColumn<&ToVal>(0).at(0), 100);
    unittest::ExpectEqual(int, doc.GetColumn<&ToVal>("B").at(0), 1000);

    // ToStrVal
    unittest::ExpectEqual(int, doc.GetCell<ToStrVal>(0, 0).val, 100);
    unittest::ExpectEqual(int, doc.GetCell<ToStrVal>(1, "1").val, 1000);
    unittest::ExpectEqual(int, doc.GetCell<ToStrVal>("A", 0).val, 100);
    unittest::ExpectEqual(int, doc.GetCell<ToStrVal>("B", "1").val, 1000);

    std::tuple<int, StrVal, float, double> row2 = doc.GetRow< t_raw100multi, t_wrp100multi,
                                                              float, double >("2");
    unittest::ExpectEqual(int, std::get<0>(row2),  10);
    unittest::ExpectEqual(int, std::get<1>(row2).val,  1);
    unittest::ExpectEqual(float, std::get<2>(row2),  0.001f);
    unittest::ExpectEqual(double, std::get<3>(row2),  0.006);

    unittest::ExpectEqual(int, doc.GetColumn<&ToStrVal>(0).at(0).val, 100);
    unittest::ExpectEqual(int, doc.GetColumn<&ToStrVal>("B").at(0).val, 1000);

    std::tuple< float, double, float,
                rapidcsv::ConvertFromStr_gNaN<double>::return_type > row3 =
                       doc.GetRow< float, double,
                                   rapidcsv::ConvertFromStr_fNaN<float>,
                                   rapidcsv::ConvertFromStr_gNaN<double> >("3");

    unittest::ExpectEqual(float, std::get<0>(row3),  0.3f);
    unittest::ExpectEqual(double, std::get<1>(row3),  0.03);
    unittest::ExpectEqual(bool, std::isnan( std::get<2>(row3) ),  true);
    rapidcsv::ConvertFromStr_gNaN<double>::return_type row3_cell4 = std::get<3>(row3);
    unittest::ExpectEqual(size_t, row3_cell4.index(),  1);
    unittest::ExpectEqual(std::string, std::get<std::string>(row3_cell4),  "");

    std::tuple< float, double, float,
                rapidcsv::ConvertFromStr_gNaN<double>::return_type > row4 =
                       doc.GetRow< float, double,
                                   rapidcsv::ConvertFromStr_fNaN<float>,
                                   rapidcsv::ConvertFromStr_gNaN<double> >("4");

    unittest::ExpectEqual(float, std::get<0>(row4),  0.4f);
    unittest::ExpectEqual(double, std::get<1>(row4),  0.04);
    unittest::ExpectEqual(bool, std::isnan( std::get<2>(row4) ),  true);
    rapidcsv::ConvertFromStr_gNaN<double>::return_type row4_cell4 = std::get<3>(row4);
    unittest::ExpectEqual(size_t, row4_cell4.index(),  1);
    unittest::ExpectEqual(std::string, std::get<std::string>(row4_cell4),  "abc");

    // GetRow<>  using function address
    std::tuple<int, int, int, int> row1_1 = doc.GetRow< &ToVal, &ToVal, &ToVal, &ToVal >(0);

    unittest::ExpectEqual(int, std::get<0>(row1_1),  100);
    unittest::ExpectEqual(int, std::get<1>(row1_1),  1000);
    unittest::ExpectEqual(int, std::get<2>(row1_1),  10000);
    unittest::ExpectEqual(int, std::get<3>(row1_1),  100000);


    /*
     * all sytem types are supported by default. 
     * any type conversion errors are caught at compile time.
    // Missing custom conversion function
    ExpectException(doc.GetColumn<bool>(0), rapidcsv::no_converter);
    ExpectExceptionMsg(doc.GetColumn<bool>(0), rapidcsv::no_converter,
                       "unsupported conversion datatype");
    */
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
