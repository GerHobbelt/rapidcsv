// testView040b.cpp - multiple translation units, part 2

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  return (std::stoi(dataRow.at(0))) >= 0;
}

int help_func()
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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    /////  Filter
    rapidcsv::FilterDocument<isFirstCellPositive> viewdoc(doc);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(2, 0), 81);

    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("C", "2"), "256");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
