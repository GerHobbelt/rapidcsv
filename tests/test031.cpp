// test031.cpp - generate new document by row, no row/column labels

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "2,4,16,256\n"
    "3,9,81,6561\n"
    "4,16,256,65536\n"
    "5,25,625,390625\n"
  ;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(',', false, false));

    doc.SetRow<int, int>(0, std::tuple<int, int>{ 2, 4 });
    doc.SetRow<int, int, unsigned int, long>(1, std::tuple<int, int, unsigned int, long>{ 3, 9, 81, 6561 });
    doc.SetRow<int, int, unsigned int, long>(2, std::tuple<int, int, unsigned int, long>{ 4, 16, 256, 65536 });
    doc.SetRow<int, int, unsigned int, long>(3, std::tuple<int, int, unsigned int, long>{ 5, 25, 625, 390625 });

    doc.SetCell<unsigned int>(2, 0, 16);
    doc.SetCell<long>(3, 0, 256);

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
