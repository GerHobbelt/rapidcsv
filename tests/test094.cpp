// test094.cpp - write UTF-8 BOM

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string origCsvWithBom =
    "\xef\xbb\xbfID\n"
    "1\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, origCsvWithBom);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_MISSING));
    doc.Save();

    std::string readCsvWithBom = unittest::ReadFile(path);
    unittest::ExpectEqual(std::string, origCsvWithBom, readCsvWithBom);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
