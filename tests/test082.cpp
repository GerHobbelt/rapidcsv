// test082.cpp - generate new document with insert row

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    ",A,B,C,D\n"
    "0,2,4,16,256\n"
    "1,3,9,81,6561\n"
    "2,4,16,256,65536\n"
    "3,5,25,625,390625\n"
  ;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::Document doc("", rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT), rapidcsv::SeparatorParams(',', false, false));

    doc.InsertRow<int, int, unsigned int, long>(0, std::tuple<int, int, unsigned int, long>{ 3, 9, 81, 6561 }, "1");
    doc.InsertRow<int, int, unsigned int, long>(0, std::tuple<int, int, unsigned int, long>{ 2, 4, 16, 256 }, "0");

    doc.InsertRow<int, int, unsigned int, long>(2, std::tuple<int, int, unsigned int, long>{ }, "_2_");
    doc.SetRow<int, int, unsigned int, long>(2, std::tuple<int, int, unsigned int, long>{ 4, 16, 256, 65536 });
    doc.SetRowName(2, "2");

    doc.InsertRow<int, int, unsigned int, long>(3, std::tuple<int, int, unsigned int, long>{ 5, 25, 625, 390625 }, "3");

    std::tuple<int, int, unsigned int, long> ints;
    ints = doc.GetRow<int, int, unsigned int, long>("1");

    unittest::ExpectEqual(size_t, std::tuple_size<decltype(ints)>{}, 4);
    unittest::ExpectEqual(int, std::get<0>(ints), 3);
    unittest::ExpectEqual(int, std::get<1>(ints), 9);

    doc.SetColumnName(0, "A");
    doc.SetColumnName(1, "B");
    doc.SetColumnName(2, "C");
    doc.SetColumnName(3, "D");

    doc.Save(path);

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
