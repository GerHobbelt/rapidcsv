// testView070.cpp - get row index with column labels

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
    // Row Label
    // ↓
    // -,  A,  B,  C   ← Column Label
    // 1,  3,  9,  81
    // 2, -4,  16, 256
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    /////  Filter + Sort
    const rapidcsv::SortParams<int> spD(1, rapidcsv::e_SortOrder::DESCEND);
    rapidcsv::FilterSortDocument<isFirstCellPositive, int> viewdoc2(doc, spD);

    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("-"), std::out_of_range, "include/rapidcsv/rapidcsv.h:573 row not found: -");
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowIdx("1"), 3);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("2"), std::out_of_range, "include/rapidcsv/view.h:306 row filtered out: 2");
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowIdx("3"), 2);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("6"), std::out_of_range, "include/rapidcsv/view.h:306 row filtered out: 6");
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowIdx("7"), 0);

    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("-"), std::out_of_range, "include/rapidcsv/rapidcsv.h:573 row not found: -");
    unittest::ExpectEqual(size_t, viewdoc2.GetDocumentRowIdx("1"), 0);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("2"), std::out_of_range, "include/rapidcsv/view.h:306 row filtered out: 2");
    unittest::ExpectEqual(size_t, viewdoc2.GetDocumentRowIdx("3"), 2);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("6"), std::out_of_range, "include/rapidcsv/view.h:306 row filtered out: 6");
    unittest::ExpectEqual(size_t, viewdoc2.GetDocumentRowIdx("7"), 6);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
