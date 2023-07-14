// test template instantiation is happening as expected


#include <cmath>
#include <iomanip>

#include <rapidcsv/rapidcsv.h>

#include "unittest.h"


int main()
{
  using T2S_Format_SDPdouble =  converter::T2S_Format_StreamDecimalPrecision<double>;

  static_assert(std::is_same_v<converter::S2T_Format_StreamUseClassicLocale::stream_type, std::istringstream>);
  static_assert(std::is_same_v<converter::T2S_Format_StreamAsIs::stream_type, std::ostringstream>);
  static_assert(std::is_same_v<T2S_Format_SDPdouble::stream_type, std::ostringstream>);

  // checks for typename 'STREAM_FORMAT::stream_type' and static function 'STREAM_FORMAT::streamUpdate' with specific signature
  static_assert(converter::has_streamUpdate<converter::S2T_Format_StreamUseClassicLocale>::value);
  static_assert(converter::has_streamUpdate<converter::T2S_Format_StreamAsIs>::value);
  static_assert(converter::has_streamUpdate<T2S_Format_SDPdouble>::value);

  static_assert(converter::is_formatSS<converter::S2T_Format_StreamUseClassicLocale>::value);
  static_assert(converter::is_formatSS<converter::T2S_Format_StreamAsIs>::value);
  static_assert(converter::is_formatSS<T2S_Format_SDPdouble>::value);

  static_assert(converter::is_formatISS<converter::S2T_Format_StreamUseClassicLocale>::value);
  static_assert(converter::is_formatOSS<converter::T2S_Format_StreamAsIs>::value);
  static_assert(converter::is_formatOSS<T2S_Format_SDPdouble>::value);
  static_assert(converter::is_formatOSS<typename converter::T2S_Format_floating_StreamCombine< double > >::value);
  static_assert(converter::is_formatOSS<
                                        typename  converter::T2S_Format_floating_StreamCombine<
                                                        double,
                                                        T2S_Format_SDPdouble,
                                                        typename converter::T2S_Format_StreamUseClassicLocale
                                                  >
                                      >::value);

  // check for default S2T_FORMAT types
  static_assert(std::is_same_v<typename converter::S2T_DefaultFormat<int>::type, converter::S2T_Format_std_StoT>);
  static_assert(std::is_same_v<typename converter::S2T_DefaultFormat<double>::type, converter::S2T_Format_std_StoT>);
  static_assert(std::is_same_v<typename converter::S2T_DefaultFormat<std::string>::type, converter::S2T_Format_WorkAround>);
  static_assert(std::is_same_v<typename converter::S2T_DefaultFormat<char>::type, converter::S2T_Format_WorkAround>);
  static_assert(std::is_same_v<typename converter::S2T_DefaultFormat<bool>::type, converter::S2T_Format_WorkAround>);
  static_assert(std::is_same_v<typename converter::S2T_DefaultFormat<converter::datelib::year_month_day>::type, converter::S2T_Format_StreamYMD< converter::defYMDfmt > >);

  // check for default T2S_FORMAT types
  static_assert(std::is_same_v<typename converter::T2S_DefaultFormat<int>::type, converter::T2S_Format_std_TtoS>);
  static_assert(std::is_same_v<typename converter::T2S_DefaultFormat<double>::type, T2S_Format_SDPdouble>);
  static_assert(std::is_same_v<typename converter::T2S_DefaultFormat<std::string>::type, converter::T2S_Format_WorkAround>);
  static_assert(std::is_same_v<typename converter::T2S_DefaultFormat<char>::type, converter::T2S_Format_WorkAround>);
  static_assert(std::is_same_v<typename converter::T2S_DefaultFormat<bool>::type, converter::T2S_Format_WorkAround>);
  static_assert(std::is_same_v<typename converter::T2S_DefaultFormat<converter::datelib::year_month_day>::type, converter::T2S_Format_StreamYMD< converter::defYMDfmt > >);

  static_assert(std::is_same_v< decltype(&converter::ConvertFromVal<double>::ToStr), std::string(*)(const double&)>);
  static_assert(std::is_same_v< decltype(&converter::ConvertFromStr<int>::ToVal),    int(*)(const std::string&)>);

  static_assert(std::is_same_v< decltype(&converter::ConvertFromVal<int, converter::T2S_Format_StreamAsIs>::ToStr), std::string(*)(const int&)>);
  static_assert(std::is_same_v< decltype(&converter::ConvertFromStr<int, converter::S2T_Format_StreamAsIs>::ToVal), int(*)(const std::string&)>);

  // this fails to compile as designed
  //static_assert(std::is_same_v< decltype(&converter::ConvertFromVal<double, converter::T2S_Format_std_TtoS>::ToStr), std::string(*)(const double&)>);


  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
    "text,asfa,dfsdfg,sdgd\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::SeparatorParams seppar;
    seppar.mQuotedLinebreaks = true;
    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1), seppar);

    doc.GetCell< converter::ConvertFromStr<char, converter::S2T_Format_StreamAsIs> >(3, 0);
    doc.GetCell< converter::ConvertFromStr<int, converter::S2T_Format_StreamAsIs> >(3, 1);
    doc.GetCell< converter::ConvertFromStr<unsigned int, converter::S2T_Format_StreamAsIs> >(3, 2);
    doc.GetCell<std::string>(3, 3);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
