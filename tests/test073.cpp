// test073.cpp - get column index, shift column name index

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

    /*
     * After introducing enum's rapidcsv::FlgColumnName and rapidcsv::FlgRowName ;
     * constructor for rapidcsv::LabelParams has changed accordingly and no longer
     * supports arbitary row-header or column-header.
     * This test is no longer applicable.
     */

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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(1));

    ExpectException(doc.GetColumnIdx("-"), std::out_of_range);
    ExpectException(doc.GetColumnIdx("A"), std::out_of_range);
    ExpectException(doc.GetColumnIdx("B"), std::out_of_range);
    ExpectException(doc.GetColumnIdx("C"), std::out_of_range);
    ExpectException(doc.GetColumnIdx("D"), std::out_of_range);

    unittest::ExpectEqual(size_t, doc.GetColumnIdx("1"), 0);
    unittest::ExpectEqual(size_t, doc.GetColumnIdx("3"), 1);
    unittest::ExpectEqual(size_t, doc.GetColumnIdx("9"), 2);
    unittest::ExpectEqual(size_t, doc.GetColumnIdx("81"), 3);
    ExpectException(doc.GetColumnIdx("91"), std::out_of_range);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
