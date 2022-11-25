// testView042.cpp - test GetColumnCount() and GetRowCount()

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : at index=1 as zero-index is label
  return ( std::stoi(dataRow.at(1)) ) >= 0;
}

int main()
{
  int rv = 0;

  std::string csv =
    "-,-11,B,C\n"
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
    rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));
    /////   Sort
    const rapidcsv::SortParams<int> spA(1, rapidcsv::ToVal<int,1>);
    rapidcsv::SortDocument<int> viewdoc1(doc1, spA);

    unittest::ExpectEqual(size_t, doc1.GetColumnCount(), 3);
    unittest::ExpectEqual(size_t, viewdoc1.GetViewRowCount(), 7);

    rapidcsv::Document doc2(path, rapidcsv::LabelParams(-1, -1));
    /////  Filter + Sort
    const rapidcsv::SortParams<int> spD(1, rapidcsv::ToVal<int,1>, rapidcsv::e_SortOrder::DESCEND);
    rapidcsv::FilterSortDocument<isFirstCellPositive, int> viewdoc2(doc2, spD);

    unittest::ExpectEqual(size_t, doc2.GetColumnCount(), 4);
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowCount(), 4);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
