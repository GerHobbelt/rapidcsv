// test078.cpp - include Windows.h before rapidcsv.h

#if defined(_MSC_VER)
#include <Windows.h>
#endif

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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    // below functions should use all template functions
    unittest::ExpectEqual(int, doc.GetColumn<int>(0).at(0), 3);
    unittest::ExpectEqual(int, doc.GetColumn<int>("A").at(0), 3);

    doc.SetColumn<int>(0, std::vector<int>({ 4, 5 }));
    unittest::ExpectEqual(int, doc.GetColumn<int>(0).at(0), 4);
    doc.SetColumn<int>("A", std::vector<int>({ 5, 6 }));
    unittest::ExpectEqual(int, doc.GetColumn<int>("A").at(0), 5);

    doc.InsertColumn<int>(0, std::vector<int>({ 7, 8 }), "A2");
    unittest::ExpectEqual(int, doc.GetColumn<int>("A2").at(0), 7);

    std::tuple<int, long, unsigned int> ints;
    ints = doc.GetRow<int, long, unsigned int>(0);
    unittest::ExpectEqual(int, std::get<0>(ints), 7);
    ints = doc.GetRow<int, long, unsigned int>("1");
    unittest::ExpectEqual(int, std::get<0>(ints), 7);

    doc.SetRow<int, long, unsigned int, long>(0, std::tuple<int, long, unsigned int, long>{ 9, 3, 9, 81 });
    std::tuple<int, long, unsigned int, long> ints2 = doc.GetRow<int, long, unsigned int, long>(0);
    unittest::ExpectEqual(int, std::get<0>(ints2), 9);

    doc.SetRow<int, long, unsigned int, long>("1", std::tuple<int, long, unsigned int, long>{ 9, 3, 9, 81 });
    ints2 = doc.GetRow<int, long, unsigned int, long>("1");
    unittest::ExpectEqual(int, std::get<0>(ints2), 9);

    doc.InsertRow<int, long, unsigned int, long>(0, std::tuple<int, long, unsigned int, long>{ 1, 2, 3, 4 }, "1B");
    ints = doc.GetRow<int, long, unsigned int>("1B");
    unittest::ExpectEqual(int, std::get<0>(ints), 1);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>("A2", "1B"), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>(0, "1B"), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>("A2", 0), 1);

    doc.SetCell<int>(1, 1, 111);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 1), 111);
    doc.SetCell<int>("A", "2", 222);
    unittest::ExpectEqual(int, doc.GetCell<int>("A", "2"), 222);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
