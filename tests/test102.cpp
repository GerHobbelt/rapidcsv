// test102.cpp - exception message removing out-of-range row

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C,D\n"
    "1,3,9,81,6561\n"
    "2,4,16,256\n"
    "3,9,81,6561\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    doc.RemoveRow("1");
    unittest::ExpectEqual(std::string, doc.GetRowName(0), "2");

    doc.RemoveRow("3");
    unittest::ExpectEqual(std::string, doc.GetRowName(0), "2");

    // doc has row 2 (0) now, thus 1 is out of range.
    ExpectExceptionMsg(doc.RemoveRow(1), std::out_of_range,
                       "row out of range(_mData): '1:1'");

    ExpectExceptionMsg(doc.RemoveRow("1"), std::out_of_range,
                       "rapidcsv::Document::GetRowIdx(pRowName) row not found for pRowName='1'");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
