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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    unittest::ExpectEqual(std::string, doc.GetColumnName(0), "A");
    unittest::ExpectEqual(std::string, doc.GetColumnName(1), "B");
    unittest::ExpectEqual(std::string, doc.GetColumnName(2), "C");

    std::string errMsgPre_r = (std::filesystem::path("include/rapidcsv/rapidcsv.h")).make_preferred().string();
    ExpectExceptionMsg(doc.GetColumn<int>(2), std::out_of_range,
                       errMsgPre_r + " : Document::GetColumn() # requested column index 2 >= 2 (number of columns on row index 1)");
    ExpectExceptionMsg(doc.GetColumn<int>("C"), std::out_of_range,
                       errMsgPre_r + " : Document::GetColumn() # requested column index 2 >= 2 (number of columns on row index 1)");
    ExpectExceptionMsg(doc.GetColumn<int>(3), std::out_of_range,
                       errMsgPre_r + " : Document::GetColumn() # requested column index 3 >= 3 (number of columns on row index 0)");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
