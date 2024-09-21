// test098.cpp - empty cells
#include <tuple>

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    // empty cells without trailing linebreak on last line
    {
      std::string csv =
        ",\n"
        ",\n"
        ","
        ;

      std::istringstream sstream(csv);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING, rapidcsv::FlgRowName::RN_MISSING));
      unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);
      unittest::ExpectEqual(size_t, std::tuple_size<decltype(doc.GetRow<std::string,std::string>(0))>::value, 2);
      unittest::ExpectEqual(size_t, doc.GetRow_VecStr(0).size(), 2);
      unittest::ExpectEqual(size_t, std::tuple_size<decltype(doc.GetRow<std::string,std::string>(1))>::value, 2);
      unittest::ExpectEqual(size_t, doc.GetRow_VecStr(1).size(), 2);
      unittest::ExpectEqual(size_t, std::tuple_size<decltype(doc.GetRow<std::string,std::string>(2))>::value, 2);
      unittest::ExpectEqual(size_t, doc.GetRow_VecStr(2).size(), 2);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1 /* col */, 0 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1 /* col */, 1 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1 /* col */, 2 /* row */), "");
    }

    // empty cells with trailing linebreak on last line
    {
      std::string csv =
        ",\n"
        ",\n"
        ",\n"
        ;

      std::istringstream sstream(csv);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING, rapidcsv::FlgRowName::RN_MISSING));
      unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);
      unittest::ExpectEqual(size_t, std::tuple_size<decltype(doc.GetRow<std::string,std::string>(0))>::value, 2);
      unittest::ExpectEqual(size_t, doc.GetRow_VecStr(0).size(), 2);
      unittest::ExpectEqual(size_t, std::tuple_size<decltype(doc.GetRow<std::string,std::string>(1))>::value, 2);
      unittest::ExpectEqual(size_t, doc.GetRow_VecStr(1).size(), 2);
      unittest::ExpectEqual(size_t, std::tuple_size<decltype(doc.GetRow<std::string,std::string>(2))>::value, 2);
      unittest::ExpectEqual(size_t, doc.GetRow_VecStr(2).size(), 2);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0 /* col */, 0 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0 /* col */, 1 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0 /* col */, 2 /* row */), "");
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
