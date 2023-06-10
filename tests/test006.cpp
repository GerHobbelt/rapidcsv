// test006.cpp - set row values

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string csv =
    "-,A,B,C\n"
    "1,0,0,0\n"
    "2,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    doc.SetRow<int, long, unsigned int>(0, std::tuple<int, long, unsigned int>{ 3, 9, 81 });
    doc.SetRow_VecStr("2", std::vector<std::string>{ "4", "16", "256" });

    std::tuple<int, long, unsigned int> ints;
    std::vector<std::string> strs;

    ints = doc.GetRow<int, long, unsigned int>("1");
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
