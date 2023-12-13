// testViewfNaN001.cpp - read cell value. sort column has NaN values

#include <rapidcsv/view.h>
#include "unittest.h"


#if USE_FLOATINGPOINT_FROM_CHARS_1  ==  e_ENABLE_FEATURE
template <typename T>
using _ConvS2T_NAN =
            converter::ConvertFromStr<T,
                                      converter::S2T_Format_std_CtoT<T,
                                                                     converter::FailureS2Tprocess::VARIANT_NAN> >;
#else
template <typename T>
using _ConvS2T_NAN =
            converter::ConvertFromStr<T,
                                      converter::S2T_Format_std_StoT<T,
                                                                     converter::FailureS2Tprocess::VARIANT_NAN> >;
#endif



bool isCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  return (std::stoi(dataRow.at(2))) >= 0;
}


int main()
{
  int rv = 0;

  // NOTE : Column 'A' values are unique as it is used for sorting
  std::string csv =
    "-,A,B,C\n"
    "0_0,0,0,0\n"
    "1_F,1,1,-1\n"
    "2_1,-2,4,16\n"
    "3_F,3,9,-81\n"
    "4_2,4,16,256\n"
    "5_F,5,25,-625\n"
    "6_3,yy,36,1296\n"
    "7_F,7,49,-2401\n"
    "8_4,-8,64,4096\n"
    "9_F,zz,81,-6561\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    /////  Filter
    rapidcsv::FilterDocument<isCellPositive> viewdoc(doc);

    unittest::ExpectEqual(size_t, viewdoc.GetViewRowCount(), 5);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(0, 0), 0);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(1, 0), 0);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>(2, 0), 0);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("A", 1), -2);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("B", 1), 4);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("C", 1), 16);

    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("A", "4_2"), 4);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("B", "4_2"), 16);
    unittest::ExpectEqual(int, viewdoc.GetViewCell<int>("C", "4_2"), 256);

    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>(0, 3),     "yy");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>(1, 3),     "36");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>(2, "6_3"), "1296");

    /////   Sort
    const rapidcsv::SortParams< _ConvS2T_NAN<int> > spA(0);  // column A
    rapidcsv::SortDocument<decltype(spA)> viewdoc1(doc, spA);   // `<decltype(spA)>` mandatory for clang

    unittest::ExpectEqual(size_t, viewdoc1.GetViewRowCount(), 10);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(0, 0), -8);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(1, 0), 64);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(2, 0), 4096);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("A", 1), -2);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("B", 1), 4);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("C", 1), 16);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("A", "0_0"), 0);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("B", "0_0"), 0);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>("C", 2),     0);

    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(0, 7),     7);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(1, 7),     49);
    unittest::ExpectEqual(int, viewdoc1.GetViewCell<int>(2, "7_F"), -2401);

    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(0, 8),     "yy");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(1, 8),     "36");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(2, "6_3"), "1296");

    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(0, 9),     "zz");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(1, 9),     "81");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>(2, "9_F"), "-6561");

    /////  Filter + Sort
    const rapidcsv::SortParams<_ConvS2T_NAN<int>, rapidcsv::e_SortOrder::DESCEND> spD(0);  // column A
    rapidcsv::FilterSortDocument<isCellPositive, decltype(spD)> viewdoc2(doc, spD);

    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowCount(), 5);

    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>(0, 0),     "yy");
    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>(1, 0),     "36");
    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>(2, "6_3"), "1296");

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(0, 1),     4);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(1, 1),     16);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(2, "4_2"), 256);

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("A", "0_0"), 0);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("B", "0_0"), 0);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("C", 2),     0);

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("A", 3),     -2);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("B", 3),     4);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>("C", "2_1"), 16);

    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(0, 4),     -8);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(1, 4),     64);
    unittest::ExpectEqual(int, viewdoc2.GetViewCell<int>(2, "8_4"), 4096);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
