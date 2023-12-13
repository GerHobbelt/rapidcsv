// test047.cpp - exception on invalid conversion

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"


#if USE_FLOATINGPOINT_FROM_CHARS_1  ==  e_ENABLE_FEATURE
template<typename T>
using formatS2T_TE = converter::S2T_Format_std_CtoT<T, converter::FailureS2Tprocess::THROW_ERROR>;
#else
template<typename T>
using formatS2T_TE = converter::S2T_Format_std_StoT<T, converter::FailureS2Tprocess::THROW_ERROR>;
#endif

template<typename T>
using convertS2T_TE = converter::ConvertFromStr<T, formatS2T_TE<T> >;


int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,,x,#\n"
    "2,,y,$\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

    ExpectException(doc.GetCell<int>(0, 0), std::invalid_argument);
    ExpectException(doc.GetCell<int>(1, 0), std::invalid_argument);
    ExpectException(doc.GetCell<int>(2, 0), std::invalid_argument);

    //ExpectException(doc.GetCell<double COMMA formatS2T_TE<double> >(0, 1), std::invalid_argument);
    ExpectException(doc.GetCell<convertS2T_TE<double>>(0, 1), std::invalid_argument);
    ExpectException(doc.GetCell<convertS2T_TE<double>>(1, 1), std::invalid_argument);
    ExpectException(doc.GetCell<convertS2T_TE<double>>(2, 1), std::invalid_argument);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
