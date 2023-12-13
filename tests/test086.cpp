// test086.cpp - exception message reading out-of-range column

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    unittest::ExpectEqual(std::string, doc.GetColumnName(0), "A");
    unittest::ExpectEqual(std::string, doc.GetColumnName(1), "B");
    unittest::ExpectEqual(std::string, doc.GetColumnName(2), "C");

    ExpectExceptionMsg(doc.GetColumn<int>(2), std::out_of_range,
                       "rapidcsv::Document::GetColumn(pColumnNameIdx) : column not found for 'pColumnNameIdx'");
    ExpectExceptionMsg(doc.GetColumn<int>("C"), std::out_of_range,
                       "rapidcsv::Document::GetColumn(pColumnNameIdx) : column not found for 'pColumnNameIdx'");
    ExpectExceptionMsg(doc.GetColumn<int>(3), std::out_of_range,
                       "rapidcsv::Document::GetColumn(pColumnNameIdx) : column not found for 'pColumnNameIdx'");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
