#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include <converter/converter.h>

struct format_num {};

template<converter::c_floating_point T>
struct format_oss {
    using stream_type = std::ostringstream;
    static void streamUpdate(std::ostringstream& oss)
    {
      std::cout << "call from -> static void format_oss<T>::streamUpdate(std::ostringstream& oss);" << std::endl;
      oss.precision( std::numeric_limits<T>::digits10 + 1 );
      oss << std::fixed;
    }
};

template<converter::c_floating_point T>
constexpr int getHigherDecimalPrecision()
{
  if constexpr (std::is_same_v<T, float>)
    return FLT_DIG+10;
  else
  if constexpr (std::is_same_v<T, double>)
    return DBL_DIG+10;
  else
  if constexpr (std::is_same_v<T, long double>)
    return LDBL_DIG+10;
}
template<converter::c_floating_point T>
using T2S_Format_hdp = converter::T2S_Format_StreamDecimalPrecision<T, getHigherDecimalPrecision<T>() >;

namespace converter
{
  template<>
  struct ConvertFromStr<int, format_num>
  // struct ConvertFromStr<int, format_num>  #As ConvertFromStr is
  // template-alias-declarations cannot override it
  {
    static int ToVal(const std::string& pStr)
    {
      return static_cast<int>(roundf(100.0f * std::stof(pStr)));
    }
  };


  template<c_floating_point T>
  struct ConvertFromVal<T, format_num>
  // struct ConvertFromStr<int, format_num>  #As ConvertFromStr is
  // template-alias-declarations cannot override it
  {
    static std::string ToStr(const T& val)
    {
      std::cout << "call from -> static std::string ConvertFromVal<T, format_num>::ToStr(const T& val);" << std::endl;
      std::ostringstream oss;
      oss.precision( std::numeric_limits<T>::digits10 + 1 );
      oss << std::fixed;
      oss << val;
      return oss.str();
    }
  };
}

template<typename T>
std::string ToStr(const T& val)
{
  std::cout << "call from -> std::string ToStr<T>(const T& val);" << std::endl;
  std::ostringstream oss;
  oss.precision( std::numeric_limits<T>::digits10 + 1 );
  oss << std::fixed;
  oss << val;
  return oss.str();
}

template<typename T>
void testType(const std::string& typeData, const T& orgT)
{
  const std::string strT = converter::ConvertFromVal<T>::ToStr(orgT);
  const T convT = converter::ConvertFromStr<T>::ToVal(strT);

  std::cout << std::setprecision(25) << "org" << typeData << " = " << orgT << " ; str"
            << typeData << " = " << strT << " ; conv-" << typeData << " = " << convT << std::endl;
}

constexpr char dmY_fmt[] = "%d-%m-%Y";  // string literal object with static storage duration

int main()
{

  testType<int>("Int", -100);
  testType<long>("Long", -10000000);
  testType<long long>("LongLong", -10000000000);
  testType<unsigned>("Unsigned", 100);
  testType<unsigned long>("UnsignedLong", 10000000);
  testType<unsigned long long>("UnsignedLongLong", 10000000000);
  testType<float>("Float", 313.1234567890123456789012345F);
  testType<double>("Double", 3462424.1234567890123456789012345);
  testType<long double>("LongDouble", 23453.1234567890123456789012345L);
  testType<char>("Char", 'G');
  testType<bool>("Bool", true);
  namespace rdb = converter::datelib;
  testType<rdb::year_month_day>("YearMonthDay", rdb::year_month_day{rdb::year{2023}, rdb::month{3}, rdb::day{14} } );

  typename std::string (*dmY_To_str)(const rdb::year_month_day& str) =
              &converter::ConvertFromVal< rdb::year_month_day,
                                          converter::T2S_Format_StreamYMD< dmY_fmt >
                                        >::ToStr;

  typename rdb::year_month_day (*To_dmY)(const std::string& str) =
              &converter::ConvertFromStr<  rdb::year_month_day,
                                           converter::S2T_Format_StreamYMD< dmY_fmt >
                                        >::ToVal;
  {
    rdb::year_month_day orgT{rdb::year{1980}, rdb::month{8}, rdb::day{17}};
    const std::string strT = dmY_To_str(orgT);
    const rdb::year_month_day convT = To_dmY(strT);

    std::cout << std::setprecision(25) << "orgYMD = " << orgT << " ; str_dmY"
              << " = " << strT << " ; convYMD" << " = " << convT << std::endl;
  }

  std::cout << "=============   using specialization" << std::endl;

  const long double orgLongDouble  = 100.1234567890123456789012345L;

  std::cout << "-------------   case 1 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;

  // call to -> static std::string ConvertFromVal<T, format_num> >::ToStr(const T& val);
  const std::string strLongDouble1 = converter::ConvertFromVal<long double,
                                                               format_num
                                                              >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt1 = converter::ConvertFromStr<int,format_num>::ToVal(strLongDouble1);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble1 = "
            << strLongDouble1 << " ; convInt1 = " << convInt1 << std::endl;

  std::cout << "-------------   case 2 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;
  const std::string strLongDouble2 = converter::ConvertFromVal<long double,
                                                               format_oss<long double>
                                                              >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt2 = converter::ConvertFromStr<int,format_num>::ToVal(strLongDouble2);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble2 = "
            << strLongDouble2 << " ; convInt2 = " << convInt2 << std::endl;

  std::cout << "-------------   case 3 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;

  std::cout << "call to -> int getHigherDecimalPrecision<T>();" << std::endl;
  std::cout << "check the higher precision of 'strLongDouble3'" << std::endl;
  const std::string strLongDouble3 = converter::ConvertFromVal<long double,
                                                               T2S_Format_hdp<long double>
                                                              >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt3 = converter::ConvertFromStr<int,format_num>::ToVal(strLongDouble3);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble3 = "
            << strLongDouble3 << " ; convInt3 = " << convInt3 << std::endl;
}
