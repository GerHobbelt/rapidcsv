// testView012.cpp - read cell value, no row labels

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : at index=0 as no label
  return (std::stoi(dataRow.at(0))) >= 0;
}


int main()
{
  int rv = 0;

  std::string csv =
    "A,B,C\n"
    "3,9,81\n"
    "-4,16,256\n"
    "5,25,625\n"
    "-6,36,1296\n"
    "7,49,2401\n"
    "-8,64,4096\n"
    "9,81,6561\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    /////  Filter
    rapidcsv::FilterDocument<isFirstCellPositive> viewdoc(doc);


    unittest::ExpectEqual(int, viewdoc.GetViewRowCount(), 4);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(2, 0), 81);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("A", 1), 5);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("B", 1), 25);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("C", 1), 625);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("A", 2), 7);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("B", 2), 49);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("C", 2), 2401);

    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>(0, 3), "9");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>(1, 3), "81");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>(2, 3), "6561");

    /////   Sort
    const rapidcsv::SortParams<int> spA(0);
    rapidcsv::SortDocument<int> viewdoc1(doc, spA);

    unittest::ExpectEqual(int, viewdoc1.GetViewRowCount(), 7);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(0, 0), -8);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(1, 0), 64);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(2, 0), 4096);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("A", 1), -6);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("B", 1), 36);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("C", 1), 1296);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("A", 5), 7);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("B", 5), 49);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("C", 5), 2401);

    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(0, 6), "9");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(1, 6), "81");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(2, 6), "6561");

    ////  Filter + Sort
    const rapidcsv::SortParams<int> spD(0, rapidcsv::e_SortOrder::DESCEND);
    rapidcsv::FilterSortDocument<isFirstCellPositive, int> viewdoc2(doc, spD);

    unittest::ExpectEqual(int, viewdoc2.GetViewRowCount(), 4);

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(0, 0), 9);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(1, 0), 81);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(2, 0), 6561);

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(0, 3), 3);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(1, 3), 9);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(2, 3), 81);

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("A", 1), 7);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("B", 1), 49);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("C", 1), 2401);

    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>(0, 2), "5");
    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>(1, 2), "25");
    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>(2, 2), "625");

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
