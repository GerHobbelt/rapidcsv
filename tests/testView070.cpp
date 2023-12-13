// testView070.cpp - get row index with column labels

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  return (std::stoi(dataRow.at(0))) >= 0;
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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    /////  Filter + Sort
    const rapidcsv::SortParams<int, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isFirstCellPositive, decltype(spD)> viewdoc2(doc, spD);

    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("-"), std::out_of_range,
                       "rapidcsv::Document::GetRowIdx(pRowName) row not found for 'pRowName'");
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowIdx("1"), 3);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("2"), std::out_of_range,
                       "rapidcsv::_ViewDocument::GetViewRowIdx(pRowName) : row filtered out");
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowIdx("3"), 2);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("6"), std::out_of_range,
                       "rapidcsv::_ViewDocument::GetViewRowIdx(pRowName) : row filtered out");
    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowIdx("7"), 0);

    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("-"), std::out_of_range,
                       "rapidcsv::Document::GetRowIdx(pRowName) row not found for 'pRowName'");
    unittest::ExpectEqual(size_t, viewdoc2.GetDocumentRowIdx("1"), 0);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("2"), std::out_of_range,
                       "rapidcsv::_ViewDocument::GetViewRowIdx(pRowName) : row filtered out");
    unittest::ExpectEqual(size_t, viewdoc2.GetDocumentRowIdx("3"), 2);
    ExpectExceptionMsg(viewdoc2.GetViewRowIdx("6"), std::out_of_range,
                       "rapidcsv::_ViewDocument::GetViewRowIdx(pRowName) : row filtered out");
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
