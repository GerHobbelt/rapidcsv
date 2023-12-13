// testView005.cpp - read row value

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  return (std::stoi(dataRow.at(0))) >= 0;
}

// Conversion function for int data to be converted to fixed-point two decimal numbers
int ToVal(const std::string& pStr)
{
  return static_cast<int>(roundf(100.0f * std::stof(pStr)));
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
    std::tuple<int, unsigned, long> ints;
    std::vector<std::string> strs;

    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    /////  Filter
    rapidcsv::FilterDocument<isFirstCellPositive> viewdoc(doc);

    ints = viewdoc.GetViewRow<int, unsigned, long>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 5);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 25);
    unittest::ExpectEqual(long, std::get<2>(ints), 625);

    ints = viewdoc.GetViewRow<int, unsigned, long>("1");
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 3);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 9);
    unittest::ExpectEqual(long, std::get<2>(ints), 81);

    std::tuple<int, int, int> intsP = viewdoc.GetViewRow<&ToVal, &ToVal, &ToVal>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(intsP)>, 3);
    unittest::ExpectEqual(int, std::get<0>(intsP), 500);
    unittest::ExpectEqual(int, std::get<1>(intsP), 2500);
    unittest::ExpectEqual(int, std::get<2>(intsP), 62500);

    strs = viewdoc.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "3");
    unittest::ExpectEqual(std::string, strs.at(1), "9");
    unittest::ExpectEqual(std::string, strs.at(2), "81");

    ExpectExceptionMsg(viewdoc.GetViewRow<int COMMA unsigned COMMA long>("2"), std::out_of_range,
                       "rapidcsv::_ViewDocument::GetDocumentRowIdx(pRowName) : row filtered out");

    /////   Sort
    const rapidcsv::SortParams<int> spA(0);
    rapidcsv::SortDocument<decltype(spA)> viewdoc1(doc, spA);   // `<decltype(spA)>` mandatory for clang

    ints = viewdoc1.GetViewRow<int, unsigned, long>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), -6);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 36);
    unittest::ExpectEqual(long, std::get<2>(ints), 1296);

    ints = viewdoc1.GetViewRow<int, unsigned, long>("1");
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 3);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 9);
    unittest::ExpectEqual(long, std::get<2>(ints), 81);

    strs = viewdoc1.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "-8");
    unittest::ExpectEqual(std::string, strs.at(1), "64");
    unittest::ExpectEqual(std::string, strs.at(2), "4096");

    ////  Filter + Sort
    const rapidcsv::SortParams<int, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isFirstCellPositive, decltype(spD)> viewdoc2(doc, spD);

    ints = viewdoc2.GetViewRow<int, unsigned, long>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 7);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 49);
    unittest::ExpectEqual(long, std::get<2>(ints), 2401);

    ints = viewdoc2.GetViewRow<int, unsigned, long>("1");
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 3);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 9);
    unittest::ExpectEqual(long, std::get<2>(ints), 81);

    intsP = viewdoc2.GetViewRow<&ToVal, &ToVal, &ToVal>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(intsP)>, 3);
    unittest::ExpectEqual(int, std::get<0>(intsP), 700);
    unittest::ExpectEqual(int, std::get<1>(intsP), 4900);
    unittest::ExpectEqual(int, std::get<2>(intsP), 240100);

    strs = viewdoc2.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "9");
    unittest::ExpectEqual(std::string, strs.at(1), "81");
    unittest::ExpectEqual(std::string, strs.at(2), "6561");

    ExpectExceptionMsg(viewdoc.GetViewRow<int COMMA unsigned COMMA long>("2"), std::out_of_range,
                       "rapidcsv::_ViewDocument::GetDocumentRowIdx(pRowName) : row filtered out");

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
