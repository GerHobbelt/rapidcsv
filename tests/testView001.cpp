// testView001.cpp - read cell value

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : at index=1 as zero-index is label
  return ( std::stoi(dataRow.at(1)) ) >= 0;
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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
    rapidcsv::ViewDocument<isFirstCellPositive> viewdoc(doc);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(2, 0), 81);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("A", 1), 5);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("B", 1), 25);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("C", 1), 625);

    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("A", "5"), "7");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("B", "5"), "49");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("C", "5"), "2401");

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("A", 3), 9);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("B", 3), 81);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("C", 3), 6561);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
