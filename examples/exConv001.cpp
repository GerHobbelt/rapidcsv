#if 0
TMP=$(mktemp -d)
c++ -std=c++11 -I src -o ${TMP}/a.out ${0} && ${TMP}/a.out ${@:1} ; RV=${?}
rm -rf ${TMP}
exit ${RV}
#endif

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include <rapidcsv/converter.h>

struct format_num {};

template<rapidcsv::c_floating_point T>
struct format_oss {
    using stream_type = std::ostringstream;
    static void streamUpdate(std::ostringstream& oss)
    {
      std::cout << "call from -> static void format_oss<T>::streamUpdate(std::ostringstream& oss);" << std::endl;
      oss.precision( std::numeric_limits<T>::digits10 + 1 );
      oss << std::fixed;
    }
};

template<rapidcsv::c_floating_point T>
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
template<rapidcsv::c_floating_point T>
using T2S_Format_hdp = rapidcsv::T2S_Format_StreamDecimalPrecision<T, getHigherDecimalPrecision<T>() >;

namespace rapidcsv
{
  template<>
  struct _ConvertFromStr<int, format_num>
  //struct ConvertFromStr<int, format_num>  #As ConvertFromStr is a typename connot override it
  {
    static int ToVal(const std::string& pStr)
    {
      return static_cast<int>(roundf(100.0f * std::stof(pStr)));
    }
  };


  template<c_floating_point T>
  struct _ConvertFromVal<T, format_num>
  //struct _ConvertFromVal<T, format_num >  #As ConvertFromVal is a typename connot override it
  {
    static std::string ToStr(const T& val)
    {
      std::cout << "call from -> static std::string _ConvertFromVal<T, format_num>::ToStr(const T& val);" << std::endl;
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
  const std::string strT = rapidcsv::ConvertFromVal<T>::ToStr(orgT);
  const T convT = rapidcsv::ConvertFromStr<T>::ToVal(strT);

  std::cout << std::setprecision(25) << "org" << typeData << " = " << orgT << " ; str"
            << typeData << " = " << strT << " ; conv" << typeData << " = " << convT << std::endl;
}

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

  std::cout << "=============   using specialization" << std::endl;

  const long double orgLongDouble  = 100.1234567890123456789012345L;

  std::cout << "-------------   case 1 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;

  // call to -> static std::string _ConvertFromVal<T, format_num> >::ToStr(const T& val);
  const std::string strLongDouble1 = rapidcsv::ConvertFromVal<long double,
                                                              format_num
                                                            >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt1 = rapidcsv::ConvertFromStr<int,format_num>::ToVal(strLongDouble1);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble1 = "
            << strLongDouble1 << " ; convInt1 = " << convInt1 << std::endl;

  std::cout << "-------------   case 2 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;
  const std::string strLongDouble2 = rapidcsv::ConvertFromVal<long double,
                                                              format_oss<long double>
                                                             >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt2 = rapidcsv::ConvertFromStr<int,format_num>::ToVal(strLongDouble2);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble2 = "
            << strLongDouble2 << " ; convInt2 = " << convInt2 << std::endl;

  std::cout << "-------------   case 3 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;

  std::cout << "call to -> int getHigherDecimalPrecision<T>();" << std::endl;
  std::cout << "check the higher precision of 'strLongDouble3'" << std::endl;
  const std::string strLongDouble3 = rapidcsv::ConvertFromVal<long double,
                                                              T2S_Format_hdp<long double>
                                                             >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt3 = rapidcsv::ConvertFromStr<int,format_num>::ToVal(strLongDouble3);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble3 = "
            << strLongDouble3 << " ; convInt3 = " << convInt3 << std::endl;
}
