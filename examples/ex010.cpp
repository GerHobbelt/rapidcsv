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

namespace rapidcsv
{
  template<>
  struct ConverterToVal<int,2,0>
  {
    static int ToVal(const std::string& pStr)
    {
      return static_cast<int>(roundf(100.0f * std::stof(pStr)));
    }
  };

  template<typename T>
  struct ConverterToStr<T,2>
  {
    static std::string ToStr(const typename
                             std::enable_if<std::is_floating_point<T>::value, T>::type & val)
    {
      std::ostringstream out;
      out.precision( std::numeric_limits<T>::digits10 + 1 );
      out << std::fixed << val;
      return out.str();
    }
  };
}

template<typename T>
void testType(const std::string& typeData, const T& orgT)
{
  const std::string strT = rapidcsv::ConverterToStr<T>::ToStr(orgT);
  const T convT = rapidcsv::ConverterToVal<T>::ToVal(strT);

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

  std::cout << "using specialization" << std::endl;

  const long double orgLongDouble = 100.1234567890123456789012345L;
  const std::string strLongDouble = rapidcsv::ConverterToStr<long double,2>::ToStr(orgLongDouble);
  const int convInt = rapidcsv::ConverterToVal<int,2,0>::ToVal(strLongDouble);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble = "
            << strLongDouble << " ; convInt = " << convInt << std::endl;

}
