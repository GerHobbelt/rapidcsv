// test069.cpp - get row index, no row labels

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    ExpectException(doc.GetRowIdx("-"), std::out_of_range);
    ExpectException(doc.GetRowIdx("1"), std::out_of_range);
    ExpectException(doc.GetRowIdx("2"), std::out_of_range);
    ExpectException(doc.GetRowIdx("3"), std::out_of_range);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
