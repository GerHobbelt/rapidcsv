// test028.cpp - set row values, no row/column labels

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "3,9,81\n"
    "4,16,256\n"
  ;

  std::string csv =
    "0,0,0\n"
    "0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING, rapidcsv::FlgRowName::RN_MISSING));

    doc.SetRow<int, int, unsigned int>(0, std::tuple<int, int, unsigned int>{ 3, 9, 81 });
    doc.SetRow_VecStr(1, std::vector<std::string>{ "4", "16", "256" });

    std::tuple<int, long, unsigned int> ints;
    std::vector<std::string> strs;

    ints = doc.GetRow<int, long, unsigned int>(0);
    unittest::ExpectEqual(size_t, std::tuple_size<decltype(ints)>{}, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 3);
    unittest::ExpectEqual(long, std::get<1>(ints), 9);
    unittest::ExpectEqual(unsigned int, std::get<2>(ints), 81);

    strs = doc.GetRow_VecStr(1);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "4");
    unittest::ExpectEqual(std::string, strs.at(1), "16");
    unittest::ExpectEqual(std::string, strs.at(2), "256");

    doc.Save();

    std::string csvread = unittest::ReadFile(path);

    unittest::ExpectEqual(std::string, csvref, csvread);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
