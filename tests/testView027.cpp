// testView027.cpp - read row values, no row/column labels

#include <rapidcsv/view.h>
#include "unittest.h"

bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : at index=0 as zero-index is label
  return (std::stoi(dataRow.at(0))) >= 0;
}


int main()
{
  int rv = 0;

  std::string csv =
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
    std::tuple<int, unsigned, long> ints;
    std::vector<std::string> strs;

    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1));

    /////  Filter
    rapidcsv::FilterDocument<isFirstCellPositive> viewdoc(doc);

    ints = viewdoc.GetViewRow<int, unsigned, long>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), 5);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 25);
    unittest::ExpectEqual(long, std::get<2>(ints), 625);

    std::string errMsgPre_r = (std::filesystem::path("include/rapidcsv/rapidcsv.h")).make_preferred().string();
    ExpectExceptionMsg(viewdoc.GetViewRow<int COMMA unsigned COMMA long>("1"), std::out_of_range,
                       errMsgPre_r + ": Document::GetRowIdx(pRowName) row not found: 1");
    /*
       unittest::ExpectEqual(size_t, ints.size(), 3);
       unittest::ExpectEqual(int, ints.at(0), 3);
       unittest::ExpectEqual(int, ints.at(1), 9);
       unittest::ExpectEqual(int, ints.at(2), 81);
     */

    strs = viewdoc.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "3");
    unittest::ExpectEqual(std::string, strs.at(1), "9");
    unittest::ExpectEqual(std::string, strs.at(2), "81");

    // ExpectExceptionMsg(viewdoc.GetViewRow<int COMMA unsigned COMMA long>("2"), std::out_of_range,
    //                    errMsgPre_r + ": Document::GetRowIdx(pRowName) row not found: 2");

    /////   Sort
    const rapidcsv::SortParams<int> spA(0);
    rapidcsv::SortDocument<int> viewdoc1(doc, spA);

    ints = viewdoc1.GetViewRow<int, unsigned, long>(1);
    unittest::ExpectEqual(size_t, std::tuple_size_v<decltype(ints)>, 3);
    unittest::ExpectEqual(int, std::get<0>(ints), -6);
    unittest::ExpectEqual(unsigned, std::get<1>(ints), 36);
    unittest::ExpectEqual(long, std::get<2>(ints), 1296);

    ExpectExceptionMsg(viewdoc1.GetViewRow<int COMMA unsigned COMMA long>("1"), std::out_of_range,
                       errMsgPre_r + ": Document::GetRowIdx(pRowName) row not found: 1");
    /*
       unittest::ExpectEqual(size_t, ints.size(), 3);
       unittest::ExpectEqual(int, ints.at(0), 3);
       unittest::ExpectEqual(int, ints.at(1), 9);
       unittest::ExpectEqual(int, ints.at(2), 81);
     */

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

    ExpectExceptionMsg(viewdoc2.GetViewRow<int COMMA unsigned COMMA long>("1"), std::out_of_range,
                       errMsgPre_r + ": Document::GetRowIdx(pRowName) row not found: 1");
    /*
       unittest::ExpectEqual(size_t, ints.size(), 3);
       unittest::ExpectEqual(int, ints.at(0), 3);
       unittest::ExpectEqual(int, ints.at(1), 9);
       unittest::ExpectEqual(int, ints.at(2), 81);
     */

    strs = viewdoc2.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "9");
    unittest::ExpectEqual(std::string, strs.at(1), "81");
    unittest::ExpectEqual(std::string, strs.at(2), "6561");

    // ExpectExceptionMsg(viewdoc.GetViewRow<int COMMA unsigned COMMA long>("2"), std::out_of_range,
    //                    errMsgPre_r + ": Document::GetRowIdx(pRowName) row not found: 2");

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
