// test066.cpp - test UTF-8 Byte order mark skipping

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvWithBom =
    "\xef\xbb\xbfID\n"
    "1\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csvWithBom);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_MISSING));
    unittest::ExpectEqual(size_t, doc.GetRowCount(), 1);
    unittest::ExpectEqual(std::string, doc.GetColumn<std::string>("ID")[0], "1");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
