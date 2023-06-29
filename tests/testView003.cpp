// testView003.cpp - read column value

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
    "2,-4,16,256\n"
    "3,5,25,625\n"
    "4,-6,36,1296\n"
    "5,7,49,2401\n"
    "6,-8,64,4096\n"
    "7,9,81,6561\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    std::vector<int> ints;
    std::vector<std::string> strs;

    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    /////  Filter
    rapidcsv::FilterDocument<isFirstCellPositive> viewdoc(doc);

    ints = viewdoc.GetViewColumn<int>(0);
    unittest::ExpectEqual(size_t, ints.size(), 4);
    unittest::ExpectEqual(int, ints.at(0), 3);
    unittest::ExpectEqual(int, ints.at(3), 9);

    ints = viewdoc.GetViewColumn<int>("B");
    unittest::ExpectEqual(size_t, ints.size(), 4);
    unittest::ExpectEqual(int, ints.at(0), 9);
    unittest::ExpectEqual(int, ints.at(3), 81);

    strs = viewdoc.GetViewColumn<std::string>(2);
    unittest::ExpectEqual(size_t, strs.size(), 4);
    unittest::ExpectEqual(std::string, strs.at(1), "625");
    unittest::ExpectEqual(std::string, strs.at(2), "2401");

    /////   Sort
    const rapidcsv::SortParams<int> spA(1);
    rapidcsv::SortDocument viewdoc1(doc, spA);

    ints = viewdoc1.GetViewColumn<int>(0);
    unittest::ExpectEqual(size_t, ints.size(), 7);
    unittest::ExpectEqual(int, ints.at(0), -8);
    unittest::ExpectEqual(int, ints.at(6), 9);

    ints = viewdoc1.GetViewColumn<int>("B");
    unittest::ExpectEqual(size_t, ints.size(), 7);
    unittest::ExpectEqual(int, ints.at(0), 64);
    unittest::ExpectEqual(int, ints.at(6), 81);

    strs = viewdoc1.GetViewColumn<std::string>(2);
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "1296");
    unittest::ExpectEqual(std::string, strs.at(2), "256");

    ////  Filter + Sort
    const rapidcsv::SortParams<int, rapidcsv::e_SortOrder::DESCEND> spD(1);
    rapidcsv::FilterSortDocument<isFirstCellPositive, decltype(spD)> viewdoc2(doc, spD);

    ints = viewdoc2.GetViewColumn<int>(0);
    unittest::ExpectEqual(size_t, ints.size(), 4);
    unittest::ExpectEqual(int, ints.at(0), 9);
    unittest::ExpectEqual(int, ints.at(3), 3);

    ints = viewdoc2.GetViewColumn<int>("B");
    unittest::ExpectEqual(size_t, ints.size(), 4);
    unittest::ExpectEqual(int, ints.at(0), 81);
    unittest::ExpectEqual(int, ints.at(3), 9);

    strs = viewdoc2.GetViewColumn<std::string>(2);
    unittest::ExpectEqual(size_t, strs.size(), 4);
    unittest::ExpectEqual(std::string, strs.at(1), "2401");
    unittest::ExpectEqual(std::string, strs.at(2), "625");

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
