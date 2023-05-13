// testView001.cpp - read cell value

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : at index=1 as zero-index is label
  return (std::stoi(dataRow.at(1))) >= 0;
}


int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,3,16,256\n"
    "3,5,25,625\n"
    "4,5,36,1296\n"
    "5,7,49,2401\n"
    "6,7,64,4096\n"
    "7,7,81,6561\n"
    "8,-1,2,3\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    /////   Sort
    const rapidcsv::SortParams<int> spA(1);
    const rapidcsv::SortParams<int> spD(2, rapidcsv::e_SortOrder::DESCEND);
    rapidcsv::FilterSortDocument<isFirstCellPositive, int, int> viewdoc1(doc, spA, spD);

    unittest::ExpectEqual(size_t, viewdoc1.GetViewRowCount(), 7);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(1, 0), 16);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(2, 0), 256);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("A", 1), 3);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("B", 1), 9);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("C", 1), 81);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("A", "4"), 5);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("B", "4"), 36);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("C", "4"), 1296);

    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(0, 6), "7");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(1, 6), "49");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(2, 6), "2401");
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
