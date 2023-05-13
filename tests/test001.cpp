// test001.cpp - read cell value

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"


constexpr char dmY_fmt[] = "%d-%m-%Y";  // string literal object with static storage duration
namespace rdb = rapidcsv::datelib;
constexpr rdb::year_month_day (*To_dmY)(const std::string& str) =
                    &rapidcsv::ConvertFromStr< rdb::year_month_day, rapidcsv::S2T_Format_StreamYMD< dmY_fmt > >::ToVal;

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C,D,E\n"
    "1,3,9,81,2023-05-15,27-12-2020\n"
    "2,4,16,256,1997-07-23,28-02-1980\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc.GetCell<int>(2, 0), 81);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 1), 4); // (0, 1) -> ("A", "2")
    unittest::ExpectEqual(int, doc.GetCell<int>("A", 1), 4);
    unittest::ExpectEqual(int, doc.GetCell<int>(0, "2"), 4);
    unittest::ExpectEqual(int, doc.GetCell<int>("A", "2"), 4);

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("C", "2"), "256");

    unittest::ExpectEqual(rdb::year_month_day, doc.GetCell<rdb::year_month_day>("D", "1"),
                          rdb::year_month_day{rdb::year{2023} COMMA rdb::month{5} COMMA rdb::day{15} });
    unittest::ExpectEqual(rdb::year_month_day, doc.GetCell<rdb::year_month_day>("D", "2"),
                          rdb::year_month_day{rdb::year{1997} COMMA rdb::month{7} COMMA rdb::day{23} });

    unittest::ExpectEqual(rdb::year_month_day, doc.GetCell<rdb::year_month_day COMMA To_dmY >("E", "1"),
                          rdb::year_month_day{rdb::year{2020} COMMA rdb::month{12} COMMA rdb::day{27} });
    unittest::ExpectEqual(rdb::year_month_day, doc.GetCell<rdb::year_month_day COMMA To_dmY >("E", "2"),
                          rdb::year_month_day{rdb::year{1980} COMMA rdb::month{2} COMMA rdb::day{28} });

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
