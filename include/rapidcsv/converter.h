/*
 * converter.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_FilterSort
 * Version:  2.01.fs-8.75
 *
 * Copyright (C) 2022-2023 Gautam Dhar
 * All rights reserved.
 *
 * rapidcsv_FilterSort is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <functional>
#include <iostream>
#include <cfloat>
#include <climits>
#include <cmath>
#include <limits>
#include <type_traits>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <variant>
#include <locale>
#include <stdexcept>

#if  USE_CHRONO == 1
  #include <chrono>
  #if  DUSE_CHRONO_TOSTREAM != 1
    //#include <string_view>
    //#include <format>
    #include <cstring>
  #endif
#else
  #include <date/date.h>
#endif

/*
#define SHOW(...) \
    std::cout << std::setw(29) << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'
// usage :: SHOW( std::is_integral_v<float> );
*/

#if  USE_MACROPREFIXMAP == 1
  #define __RAPIDCSV_FILE__    __FILE__
#else
  // https://stackoverflow.com/questions/8487986/file-macro-shows-full-path/40947954#40947954
  #define __RAPIDCSV_FILE__   (__FILE__ + RAPIDCSV_SOURCE_PATH_SIZE)
#endif

/*
 *  NUMERIC_LOCALE : Number locales are specific settings for the 1000 separators and decimals.
 *                   Some countries use 1.000,00. Others switch the dot and the comma for 1,000.00.
 *                   Same goes for dates as there are different conventions on how to display
 *                   the day, the month, the year.
 */
namespace rapidcsv
{
#if  USE_CHRONO == 1
  namespace datelib = std::chrono;
#else
  namespace datelib = date;
#endif




  // [=============================================================[   common helpers
  template <typename T>
  concept c_canBsigned_char = ( std::is_same_v<T, char> ||
                                std::is_same_v<T, wchar_t> );

  template <typename T, typename = void >
  struct is_signed_char {
    static constexpr bool value = std::is_same_v<T, signed char>;
  };
  template <c_canBsigned_char T>
  struct is_signed_char<T, void> {
    // plain chars can be either signed or unsigned as they are machine dependent, but printable characters are always positive
    static constexpr bool
    value = (static_cast<int>((std::numeric_limits<T>::min)()) < 0);
    // warning C4003: not enough arguments for function-like macro invocation 'min'
    // work-around is (std::numeric_limits<T>::min)()
  };
  template <typename T>
  concept c_signed_char = is_signed_char<T>::value;

  template <typename T, typename = void >
  struct is_unsigned_char {
    static constexpr bool
    value = ( std::is_same_v<T, unsigned char> ||
              std::is_same_v<T, char8_t> ||
              std::is_same_v<T, char16_t> ||
              std::is_same_v<T, char32_t> );
  };
  template <c_canBsigned_char T>
  struct is_unsigned_char<T, void> {
    // plain chars can be either signed or unsigned as they are machine dependent, but printable characters are always positive
    static constexpr bool
    value = (static_cast<int>((std::numeric_limits<T>::min)()) == 0);
  };
  template <typename T>
  concept c_unsigned_char = is_unsigned_char<T>::value;

  template <typename T>
  struct is_char {
    static constexpr bool
    value = is_signed_char<T>::value || is_unsigned_char<T>::value;
  };
  template <typename T>
  concept c_char = is_char<T>::value;

  // std::integral is true for 'bool' and 'char'.
  // Conversion for 'bool' and 'char' is different from other 'integral-s'.
  // Hence need for a 'concept' to segregate from 'bool' and 'char'
  template<typename T>
  struct is_integer_type
  {
    static constexpr bool value = std::is_integral_v<T> &&
                                  (!is_char<T>::value) &&
                                  (!std::is_same_v<T, bool>);
  };
  template <typename T>
  concept c_integer_type = is_integer_type<T>::value;

  //template <typename T>
  //concept c_std_integral = is_integral_v<T>;

  // Going with the flow, and just wrapping up floating_point
  template<typename T>
  concept c_floating_point = std::is_floating_point_v<T>;

  template<typename T>
  concept c_signaling_NaN = std::numeric_limits<T>::has_signaling_NaN;

  template<typename T>
  concept c_number_type = std::is_arithmetic_v<T> && (!is_char<T>::value);

  template<typename T>
  concept c_NOT_string = (!std::is_same_v<T, std::string>);
  // ]=============================================================]   common helpers



  // [=============================================================[ COMMON_FORMAT
  template <typename IOSS>
  struct is_iostream {
    static constexpr bool
    value = std::is_same_v<std::istringstream,IOSS> ||
            std::is_same_v<std::ostringstream,IOSS>;
  };
  template <typename IOSS>
  concept c_iostream = is_iostream<IOSS>::value;


  template<c_iostream IOSS>
  struct Format_StreamAsIs
  {
    using stream_type = IOSS;
    constexpr static inline void streamUpdate([[maybe_unused]] IOSS& ss) {}
  };

  template<c_iostream IOSS>
  struct Format_StreamUseClassicLocale
  {
    using stream_type = IOSS;
    static inline void streamUpdate(IOSS& ss)
    {
      const std::locale& ulocale = std::locale::classic();
      ss.imbue(ulocale);
    }
  };

  template<c_iostream IOSS, const char* usrLoc>
  struct Format_StreamUserLocale
  {
    using stream_type = IOSS;
    static inline void streamUpdate(IOSS& ss)
    {
      const std::locale userlocale(usrLoc);
      ss.imbue(userlocale);
    }
  };

  constexpr char defYMDfmt[] = "%F";  // string literal object with static storage duration
  template<c_iostream IOSS, const char* _ymdFormat = defYMDfmt> // %F -> "%Y-%m-%d"
  struct Format_StreamYMD
  {
    using stream_type = IOSS;
    constexpr static const char* ymdFormat = _ymdFormat;
    constexpr static inline void streamUpdate([[maybe_unused]] IOSS& ss) {}
  };

  template <typename, typename = void>
  struct has_streamUpdate : std::false_type {};

  template <typename FMT>
  struct has_streamUpdate<FMT, std::void_t< decltype(FMT::streamUpdate),  // check for the presence of member   FMT::streamUpdate
                                            typename FMT::stream_type     // check for the presence of type-def FMT::stream_type
                                          >
                         >
           : std::is_same<decltype(&FMT::streamUpdate), void(*)(typename FMT::stream_type&)>  // member is function
  {};

  template <typename, typename = void >
  struct is_formatSS : std::false_type {};

  template <typename FMT>
  struct is_formatSS< FMT,
                      typename std::enable_if_t<   has_streamUpdate<FMT>::value &&
                                                   is_iostream<typename FMT::stream_type>::value
                                               >
                    >
           : std::true_type
  {};
  template <typename FMT>
  concept c_formatSS = is_formatSS<FMT>::value;

  template <typename, typename = void>
  struct is_formatYMDss : std::false_type {};

  template <typename FMT>
  struct is_formatYMDss< FMT,
                         std::void_t<decltype(FMT::ymdFormat)>  // check for the presence of FMT::ymdFormat
                       >
           : is_formatSS<FMT>
  {};
  template <typename FMT>
  concept c_formatYMDss = is_formatYMDss<FMT>::value;

  template<c_iostream IOSS,
           c_formatSS FORMAT_1,
           c_formatSS FORMAT_2 >
      requires std::is_same_v<IOSS,typename FORMAT_1::stream_type> &&
               std::is_same_v<IOSS,typename FORMAT_2::stream_type>
  struct Format_StreamCombineFormat
  {
    using stream_type = IOSS;
    static inline void streamUpdate(IOSS& ss)
    {
      FORMAT_2::streamUpdate(ss);
      FORMAT_1::streamUpdate(ss);
    }
  };

  template<c_floating_point T>
  constexpr int getDecimalPrecision()
  {
    /*
       Q: why +3 in the return ?
       A: There is loss of data-precision at several places, for e.g:
          float pi_val = 3.1415926535f;  // (1)
          std::ostringstream oss;
          oss << pi_val; std::string pi_str = oss.str();  // (2)
          std::istringstream iss(pi_str); float pi_read;
          iss >> pi_read; // (3)

          The data-precision loss is happening at steps (1), (2) and (3)
          The precision-loss at (1), where in rvalue-raw is not captured exactly
          in lvalue-variable. This is system dependent.
          The value in lvalue-variable(pi_val) should be written to text at a
          higher precision digits for float(hence +3). As we need to eliminate the
          precision loss happening at steps (2) and (3).
          The repeatability or read-write accuracy can only be achieved by using
          higher precision of that specified by precision accuracy for float.

          Refer test091, which fails when +3 is removed here.
    */
    //  https://www.learncpp.com/cpp-tutorial/floating-point-numbers/
    //  https://en.cppreference.com/w/cpp/types/numeric_limits/digits10
    if constexpr (std::is_same_v<T, float>)
      return FLT_DIG+3;  // (4 bytes) -> 6 significant digits, typically 7
      //  (6 for IEEE float)
    else
    if constexpr (std::is_same_v<T, double>)
      return DBL_DIG+3;  // (8 bytes) -> 15 significant digits, typically 16
      //  (15 for IEEE double)
    else
    if constexpr (std::is_same_v<T, long double>)
      return LDBL_DIG+3;  // (if 16 bytes) -> 33-36 significant digits
      //  (18 for 80-bit Intel long double; 33 for IEEE quadruple)
  }
  // ]=============================================================] COMMON_FORMAT


  // [=============================================================[ S2T_FORMAT
  template <typename T, typename T2,
            T (*StoT)( const std::string& str, std::size_t* pos, int base) >
  inline T str2T( const std::string& str, std::size_t* pos = nullptr, int base = 10 )
  {
    if constexpr( std::is_same_v<T, T2> ) {
      return StoT(str, pos, base);
    } else {
      return static_cast<T>(StoT(str, pos, base));
    }
  }

  struct S2T_Format_WorkAround {};
  struct S2T_Format_std_StoT {};
  using S2T_Format_StreamAsIs = Format_StreamAsIs<std::istringstream>;
  using S2T_Format_StreamUseClassicLocale = Format_StreamUseClassicLocale<std::istringstream>;
  template <const char* usrLoc>
  using S2T_Format_StreamUserLocale = Format_StreamUserLocale<std::istringstream, usrLoc>;
  template < const char* formatYMD = defYMDfmt > // %F -> "%Y-%m-%d"
  using S2T_Format_StreamYMD = Format_StreamYMD<std::istringstream, formatYMD>;


  template <typename, typename = void>
  struct is_formatISS : std::false_type {};

  template <typename FMT>
  struct is_formatISS<  FMT,
                        typename std::enable_if_t< std::is_same_v<typename FMT::stream_type, std::istringstream>
                                                 >
                     >
            : is_formatSS<FMT>
  {};
  template <typename FMT>
  concept c_formatISS = is_formatISS<FMT>::value;


  template <typename, typename = void>
  struct is_formatYMDiss : std::false_type {};

  template <typename FMT>
  struct is_formatYMDiss< FMT,
                          typename std::enable_if_t< std::is_same_v<typename FMT::stream_type, std::istringstream>
                                                   >
                        >
            : is_formatYMDss<FMT>
  {};
  template <typename FMT>
  concept c_formatYMDiss = is_formatYMDiss<FMT>::value;


  template<typename T, typename = void >
  struct S2T_DefaultFormat 
  {
    using type = S2T_Format_StreamUseClassicLocale;
  };

  template<typename T>
  struct S2T_DefaultFormat< T,
                            typename  std::enable_if_t< is_integer_type<T>::value ||
                                                        std::is_floating_point_v<T>
                                                      >
                          > 
  {
    using type = S2T_Format_std_StoT;
  };

  template<typename T>
  struct S2T_DefaultFormat< T,
                            typename  std::enable_if_t< std::is_same_v<T, std::string> ||
                                                        is_char<T>::value ||
                                                        std::is_same_v<T, bool>
                                                      >
                          >
  {
    using type = S2T_Format_WorkAround;
  };

  template<>
  struct S2T_DefaultFormat<datelib::year_month_day, void>
  {
    using type = S2T_Format_StreamYMD< defYMDfmt >; // %F -> "%Y-%m-%d"
  };


  template <typename, typename = void>
  struct is_S2Tconverter : std::false_type {};

  template <typename CFS>
  struct is_S2Tconverter< CFS, std::void_t< decltype(CFS::ToVal),      // check for the presence of 'static' member function CFS::ToVal
                                            typename CFS::value_type,  // check for the presence of type-def CFS::value_type
                                            typename CFS::return_type  // check for the presence of type-def CFS::return_type
                                          >
                        >
           : std::is_same<decltype(&CFS::ToVal), typename CFS::return_type(*)(const std::string&)>  // member is function
  {};

  template <typename CFS>
  concept c_S2Tconverter = is_S2Tconverter<CFS>::value;

  template <typename CFS>
  concept c_NOT_S2Tconverter = !is_S2Tconverter<CFS>::value;


  template< typename T,
            auto (*CONV_S2T)(const std::string&)
          >
  struct S2TwrapperFunction
  {
    using value_type  = T;
    using return_type = typename std::invoke_result_t< decltype(CONV_S2T),
                                                       const std::string& >;

    inline static return_type
    ToVal(const std::string& str)
    {
      return CONV_S2T(str);
    }
  };

  // ]=============================================================] S2T_FORMAT


  // [=============================================================[ ConvertFromStr

  // https://stackoverflow.com/questions/68058640/using-concepts-to-select-class-template-specialization?rq=3
  template<typename T, typename S2T_FORMAT = S2T_Format_StreamAsIs >
  struct _ConvertFromStr;

    // https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/
  /**
   * @brief     Convertor class implementation for any types, FROM string using 'std::istringstream'.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted to, from string data.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic such as 'Locale'.
   */
  template< c_NOT_string T, c_formatISS S2T_FORMAT >
  struct _ConvertFromStr<T, S2T_FORMAT>
  {
    using value_type  = T;
    using return_type = T;

    /*
        https://stackoverflow.com/questions/52299062/template-specialization-for-sfinae
        That is simply how SFINAE works ;)

        As you know, you have to "create a failure" within the template declaration and not inside the template definition. As this:

        template < typename X, typename = ... here is the code which may generate an error     during instantiation >
        void Bla() {}

        The only chance to put some "code" in the declaration is to define something in the template parameter list or inside the template function declaration itself like:

        template < typename X>
        void Bla( ... something which may generates an error )   <<== also works on return type
        {}
    */
    // also refer  ::  https://stackoverflow.com/questions/11055923/stdenable-if-parameter-vs-template-parameter/11056146#11056146
    /**
     * @brief   Converts string holding a numerical value to numerical datatype representation.
     * @param   str                 input string.
     * @returns Numerical value if conversion succeeds.
     *          Else 'std::invalid_argument' on conversion failure.
     */
    inline static T
    ToVal(const std::string& str)
    {
      using S2T_FORMAT_STREAM = S2T_FORMAT;
      T val;
      std::istringstream iss(str);
      S2T_FORMAT_STREAM::streamUpdate(iss);
      iss >> val;
      if (iss.fail() || iss.bad()) // || iss.eof())
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << " : ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<c_NOT_string T, c_formatISS S2T_FORMAT>::ToVal(const std::string& str)' ::: str='";
        eoss << str << "'  istringstream-conversion failed...";
        eoss << std::boolalpha << "   iss.fail() = " << iss.fail()
                               << " : iss.bad() = " << iss.bad()
                               << " : iss.eof() = " << iss.eof() << std::endl;
        throw std::invalid_argument(eoss.str());
      }
      return val;
    }
  };
 
  /**
   * @brief     Convertor class implementation for integer types from string.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted to, from string data.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic.
   */
  template <c_integer_type T>
  struct _ConvertFromStr<T, S2T_Format_std_StoT >
  {
    using value_type  = T;
    using return_type = T;

    // TODO unit tests
    inline static T
    ToVal_args(const std::string& str, std::size_t* pos = nullptr, int base = 10)
    {
      //std::cout << "_ConvertFromStr<T,S2T_Format_std_StoT>::ToVal(str,...)" << std::endl;
      /*
       * Size of int and short are implementation defined.
       * int and short are 2 bytes on 16 compilers.
       * One 32 bit compilers, int is 4 bytes, while short remains 16 bits. 
       *
       * check for byte sizes of each integral type here ...
       * https://cplusplus.com/forum/general/140825/
       * https://en.cppreference.com/w/cpp/language/types
       */
      if constexpr(std::is_signed_v<T>)
      {
        if constexpr( std::is_same_v<T,long long> ) {
          return str2T<T, long long, &std::stoll>(str, pos, base);
        } else
        if constexpr( std::is_same_v<T,long> ) {
          return str2T<T, long, &std::stol>(str, pos, base);
        } else
        if constexpr( std::is_same_v<T,int> ) {
          return str2T<T, int, &std::stoi>(str, pos, base);
        } else {
          //static_assert(false, "COMPILER-ERROR : rapidcsv :: in function 'T ConvertFromStr<T,S2T_Format_std_StoT>::ToVal(const std::string& str)' ::: rule-violation # ( sizeof(T)==sizeof(long long) || sizeof(T)==sizeof(long) || sizeof(T)==sizeof(int) )");
        }
      } else
      if constexpr(std::is_unsigned_v<T>) {
        if constexpr( std::is_same_v<T,unsigned long long> ) {
          return str2T<T, unsigned long long, &std::stoull>(str, pos, base);
        } else
        if constexpr( std::is_same_v<T,unsigned long> ) {
          return str2T<T, unsigned long, &std::stoul>(str, pos, base);
        } else
        if constexpr( std::is_same_v<T,unsigned> ) {
          // as std::stou(...) is not part of the standard. Hence just static_cast from ul -> u
          return static_cast<unsigned>(std::stoul(str, pos, base));
        } else {
          //static_assert(false, "COMPILER-ERROR : rapidcsv :: in function 'T ConvertFromStr<T,S2T_Format_std_StoT>::ToVal(const std::string& str)' ::: rule-violation # ( sizeof(T)==sizeof(unsigned long long) || sizeof(T)==sizeof(unsigned long) || sizeof(T)==sizeof(unsigned int) )");
        }
      } else {
        //static_assert(false, "COMPILER-ERROR : rapidcsv :: in function 'T ConvertFromStr<T,S2T_Format_std_StoT>::ToVal(const std::string& str)' ::: nexpected error condition reached.");
      }
    }

    /**
     * @brief   Converts string holding a integer represenation to integer datatype.
     * @param   str                 input string.
     * @returns Numerical value if conversion succeeds.
     *          Else throws error on conversion failure.
     */
    inline static T
    ToVal(const std::string& str)
    {
      return ToVal_args(str);
    }
  };

  /**
   * @brief     Convertor class implementation for floating types from string.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted to, from string data.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic.
   */
  template<c_floating_point T>
  struct _ConvertFromStr<T, S2T_Format_std_StoT >
  {
    using value_type  = T;
    using return_type = T;

    // TODO unit tests
    inline static T
    ToVal_args(const std::string& str, std::size_t* pos = nullptr)
    {
      if constexpr( std::is_same_v<T, float> ) {
        return std::stof(str, pos);
      } else
      if constexpr( std::is_same_v<T, double> ) {
        return std::stod(str, pos);
      } else
      if constexpr( std::is_same_v<T, long double> ) {
        return std::stold(str, pos);
      }
    }

    /**
     * @brief   Converts string holding a numerical value to numerical datatype representation.
     * @param   str                 input string.
     * @returns Numerical value if conversion succeeds.
     *          Else throws error on conversion failure.
     */
    inline static T
    ToVal(const std::string& str)
    {
      return ToVal_args(str);
    }
  };

  /**
   * @brief     Specialized implementation handling string to string conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   */
  template<>
  struct _ConvertFromStr<std::string, S2T_Format_WorkAround>
  {
    using value_type  = std::string;
    using return_type = std::string;

    /**
     * @brief   Converts string to string.
     * @param   str                 input string.
     * @returns string.
     */
    inline static std::string ToVal(const std::string& str) { return str; }
  };

  /**
   * @brief     Specialized implementation handling string to char conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'char-type' converted to, from string data.
   */
  template<c_char T>
  struct _ConvertFromStr< T, S2T_Format_WorkAround >
  {
    using value_type  = T;
    using return_type = T;

    // TODO unit tests
    /**
     * @brief   Converts string holding char value.
     * @param   str                 input string.
     * @returns char-type.
     */
    inline static T
    ToVal(const std::string& str)
    {
      if(str.length()>1)
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << " : ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<c_char T,S2T_Format_WorkAround>::ToVal(const std::string& str)' ::: for T=char-type-(un)signed, str='";
        eoss << str << "' which violates expected rule # ( str.length()==1 )";
        throw std::invalid_argument(eoss.str());
      }
      if constexpr( std::is_same<T, std::string::value_type>::value ) {
        return str[0];
      } else {
        return static_cast<T>(str[0]);
      }
    }
  };

  /**
   * @brief     Specialized implementation handling string to bool conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   */
  template<>
  struct _ConvertFromStr<bool, S2T_Format_WorkAround>
  {
    using value_type  = bool;
    using return_type = bool;

    // TODO unit tests
    /**
     * @brief   Converts string holding bool value.
     * @param   str                 input string.
     * @returns bool.
     */
    inline static bool
    ToVal(const std::string& str)
    {
      unsigned long val = std::stoul(str); //(str, pos, base);
      if(val > 1)
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << " : ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<bool,S2T_Format_WorkAround>::ToVal(const std::string& str)' ::: str='";
        eoss << str << "' which violates expected rule # ( val==0 || val==1 )";
        throw std::invalid_argument(eoss.str());
      }
      return val;
    }
  };

  /**
   * @brief     Specialized implementation handling string to 'year_month_day' conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr')
   *            to allow specialization for custom datatype conversions.
   * @tparam  S2T_FORMAT_YMD        Class which encapsulates conversion parameters/logic such as 'Locale' specific for 'year_month_day'.
   */
  template<c_formatYMDiss S2T_FORMAT_YMD>
  struct _ConvertFromStr< datelib::year_month_day, S2T_FORMAT_YMD>
  {
    using value_type  = datelib::year_month_day;
    using return_type = datelib::year_month_day;

    /**
     * @brief   Converts string holding 'year_month_day' value. The string has the format "%F" -> "%Y-%m-%d"
     * @param   str                 input string.
     * @returns 'year_month_day'.
     */
    inline static datelib::year_month_day
    ToVal(const std::string& str)
    {
      return ToVal_args( str, S2T_FORMAT_YMD::ymdFormat); // %F -> "%Y-%m-%d"
    }

    // TODO unit tests
    inline static datelib::year_month_day
    ToVal_args(  const std::string& str,
                 const std::string::value_type* fmt,
                 std::string* abbrev  = nullptr,
                 std::chrono::minutes* offset = nullptr)
    {
      datelib::year_month_day ymd;
      std::istringstream iss(str);
      S2T_FORMAT_YMD::streamUpdate(iss);
      datelib::from_stream(iss, fmt, ymd, abbrev, offset);
      if (iss.fail() || iss.bad() ) // || !iss.eof())
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << " : ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<datelib::year_month_day, S2T_FORMAT_YMD>::ToVal_args(const std::string& str)' ::: str='";
        eoss << str << "'  istringstream-conversion failed...";
        eoss << std::boolalpha << "   iss.fail() = " << iss.fail()
                               << " : iss.bad() = " << iss.bad()
                               << " : iss.eof() = " << iss.eof() << std::endl;
        throw std::invalid_argument(eoss.str());
      }
      return ymd;
    }
  };

  /**
   * @brief     Convertor class implementation for (floating) types which support default Not-A-Number (NaN) values.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr_fNaN')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted to, from string data. Floating types which supports signaling_NaN.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic such as 'Locale'.
   */
  template<c_signaling_NaN T, typename S2T_FORMAT >
  struct _ConvertFromStr_fNaN
  {
    using value_type  = T;
    using return_type = T;

    // this implimentation works only for floating types
    // refer  https://en.cppreference.com/w/cpp/types/numeric_limits/has_signaling_NaN
    /**
     * @brief   Converts string holding a numerical value to numerical datatype representation.
     * @param   str                 input string.
     * @returns Numerical value if conversion succeeds.
     *          Else std::numeric_limits<T>::signaling_NaN() on conversion failure.
     */
    inline static T ToVal(const std::string& str)// noexcept
    {
      try
      {
        return _ConvertFromStr<T, S2T_FORMAT>::ToVal(str);
      } catch (...) {
        return std::numeric_limits<T>::signaling_NaN();
      }
    }

    // TODO unit tests
    template<typename ... ARGS>
    inline static T ToVal_args(const std::string& str, ARGS&& ... args)
    {
      try
      {
        return _ConvertFromStr<T, S2T_FORMAT>::ToVal_args(str, std::forward<ARGS>(args)...);
      } catch (...) {
        return std::numeric_limits<T>::signaling_NaN();
      }
    }
  };

  /**
   * @brief   Convertor class implementation for any numeric types, with indicators for Not-A-Number (NaN) and Null values
   *          Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromStr_gNaN')
   *          to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted to, from string data.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic such as 'Locale'.
   */
  template<c_number_type T, typename S2T_FORMAT = S2T_Format_StreamAsIs >
  struct _ConvertFromStr_gNaN
  {
    using value_type  = T;
    using return_type = std::variant<T, std::string>;

    /**
     * @brief   Converts string holding a numerical value to numerical datatype representation.
     * @param   str                 input string.
     * @returns A std::variant<T, gNaN>.
     *          It's a Numerical value if conversion succeeds.
     *          Else on conversion failure ...
     *          Either gNan::isNull if there is no data (empty string),
     *          OR     gNaN::isNaN  if string data doesn't convert to type T.
     */
    inline static return_type
    ToVal(const std::string& str)// noexcept
    {
      T val;
      try
      {
        val = _ConvertFromStr<T, S2T_FORMAT>::ToVal(str);
        if constexpr (std::numeric_limits<T>::has_signaling_NaN)
        {
          if(std::isnan(val))
            return return_type{str};
        }
      } catch (...) {
        return return_type{str};
      }
      return return_type{ val };
    }

    // TODO unit tests
    template<typename ... ARGS>
    inline static return_type
    ToVal_args(const std::string& str, ARGS&& ... args)
    {
      T val;
      try
      {
        val = _ConvertFromStr<T, S2T_FORMAT>::ToVal_args(str, std::forward<ARGS>(args)...);
        if constexpr (std::numeric_limits<T>::has_signaling_NaN)
        {
          if(std::isnan(val))
            return return_type{str};
        }
      } catch (...) {
        return return_type{str};
      }
      return return_type{ val };
    }
  };

  /**
   * @brief   Convertor 'Alias type' for any types, FROM string using 'std::istringstream'.
   * @tparam  T                     'type' converted to, from string data.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic (optional argument).
   *                                Specialization for custom datatype conversions, using this template parameter.
   */
  template<typename T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  using ConvertFromStr = _ConvertFromStr<T, S2T_FORMAT >;

  /**
   * @brief   Convertor 'Alias type' for (floating) types which support default Not-A-Number (NaN) values.
   * @tparam  T                     'type' converted to, from string data. Floating types which supports signaling_NaN.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic (optional argument).
   *                                Specialization for custom datatype conversions, using this template parameter.
   */
  template<c_signaling_NaN T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  using ConvertFromStr_fNaN = _ConvertFromStr_fNaN<T, S2T_FORMAT >;

  /**
   * @brief   Convertor 'Alias type' for any numeric types on successful conversion,
   *          else the string value which caused conversion failure.
   * @tparam  T                     'type' converted to, from string data.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic (optional argument).
   *                                Specialization for custom datatype conversions, using this template parameter.
   */
  template<c_number_type T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  using ConvertFromStr_gNaN = _ConvertFromStr_gNaN<T, S2T_FORMAT >;



  template< typename T_C >  // T_C :: T -> can be type such as int, double etc ;  XOR  C -> can be type of 'S2Tconverter'
  using t_S2Tconv = std::conditional_t< is_S2Tconverter< T_C >::value ,
                                          T_C ,
                                          ConvertFromStr< T_C >
                                      >;
  // ]=============================================================] ConvertFromStr





  // [=============================================================[ T2S_FORMAT
  struct T2S_Format_WorkAround {};
  struct T2S_Format_std_TtoS {};
  using T2S_Format_StreamAsIs = Format_StreamAsIs<std::ostringstream>;
  using T2S_Format_StreamUseClassicLocale = Format_StreamUseClassicLocale<std::ostringstream>;
  template <const char* usrLoc>
  using T2S_Format_StreamUserLocale = Format_StreamUserLocale<std::ostringstream, usrLoc>;
  template < const char* formatYMD = defYMDfmt > // %F -> "%Y-%m-%d"
  using T2S_Format_StreamYMD = Format_StreamYMD<std::ostringstream, formatYMD>;

  template <typename, typename = void>
  struct is_formatOSS : std::false_type {};

  template <typename FMT>
  struct is_formatOSS<  FMT,
                        typename std::enable_if_t< std::is_same_v< typename FMT::stream_type, std::ostringstream>
                                                 >
                     >
            : is_formatSS<FMT>
  {};
  template <typename FMT>
  concept c_formatOSS = is_formatOSS<FMT>::value;


  template <typename, typename = void>
  struct is_formatYMDoss : std::false_type {};

  template <typename FMT>
  struct is_formatYMDoss< FMT,
                          typename std::enable_if_t< std::is_same_v< typename FMT::stream_type, std::ostringstream>
                                                                   >
                        >
            : is_formatYMDss<FMT>
  {};
  template <typename FMT>
  concept c_formatYMDoss = is_formatYMDoss<FMT>::value;


  template<c_floating_point T, int decimalPrecision = getDecimalPrecision<T>()>
  struct T2S_Format_StreamDecimalPrecision
  {
    using stream_type = std::ostringstream;
    static void streamUpdate(std::ostringstream& oss)
    {
      oss << std::defaultfloat << std::setprecision(decimalPrecision);
      /*
       * NOTE : S2T_Format_StreamDecimalPrecision is not needed as
       *        it doesn't seem to have any effect as per code
       *        
       * long double pi2;
       * std::istringstream iss("3.141592653589793239");
       * iss >> std::setprecision(5) >> pi2;
       * std::cout << std::setprecision(19) << pi2 << '\n';
       * -------
       * 3.141592653589793239    :: even after setting 'iss >> std::setprecision(5)', the 'pi' output is precise upto 19 digits
       * -------
       *   1234567801234567890   :: precision digits
       * 
       * refer ->  https://en.cppreference.com/w/cpp/io/manip/setprecision
       * refer ->  https://en.cppreference.com/w/cpp/io/manip/fixed
       */
    }
  };

  template<c_floating_point T,
           c_formatOSS FORMAT_1 = T2S_Format_StreamDecimalPrecision<T> ,
           c_formatOSS FORMAT_2 = T2S_Format_StreamUseClassicLocale >
  using T2S_Format_floating_StreamCombine = Format_StreamCombineFormat<
                                              std::ostringstream,
                                              FORMAT_1, FORMAT_2>;

  template<typename T, typename = void >
  struct T2S_DefaultFormat
  {
    using type = T2S_Format_StreamUseClassicLocale;
  };

  template<c_floating_point T>
  struct T2S_DefaultFormat<T, void >
  {
    using type = T2S_Format_StreamDecimalPrecision<T>;
  };

  template<c_integer_type T>
  struct T2S_DefaultFormat<T, void >
  {
    using type = T2S_Format_std_TtoS;
  };

  template<typename T>
  struct T2S_DefaultFormat< T,
                            typename std::enable_if_t< std::is_same_v<T, std::string> ||
                                                       is_char<T>::value ||
                                                       std::is_same_v<T, bool>
                                                     >
                          >
  {
    using type = T2S_Format_WorkAround;
  };

  template<>
  struct T2S_DefaultFormat<datelib::year_month_day, void>
  {
    using type = T2S_Format_StreamYMD< defYMDfmt >; // %F -> "%Y-%m-%d"
  };

  template <typename, typename = void>
  struct is_T2Sconverter : std::false_type {};

  template <typename CFV>
  struct is_T2Sconverter< CFV, std::void_t< decltype(CFV::ToStr),     // check for the presence of member   CFV::ToStr
                                            typename CFV::value_type, // check for the presence of type-def CFV::value_type
                                            typename CFV::input_type  // check for the presence of type-def CFV::input_type
                                          >
                        >
           : std::is_same<decltype(&CFV::ToStr), std::string(*)(const typename CFV::input_type&)>  // member is function
  {};

  template <typename CFV>
  concept c_T2Sconverter = is_T2Sconverter<CFV>::value;

  template <typename CFV>
  concept c_NOT_T2Sconverter = !is_T2Sconverter<CFV>::value;


  template< typename T, typename TI,
            std::string (*CONV_T2S)(const TI&)
          >
  struct T2SwrapperFunction
  {
    using value_type = T;
    using input_type = TI;

    inline static std::string
    //typename std::enable_if_t< (!std::is_same_v<T, TI>),std::string>
    ToStr(const input_type& val)
    {
      return CONV_T2S(val);
    }
  };

/*
  TODO :: this specialization is not recognized in Document::SetColumn(...)
  template< typename T,
            std::string (*CONV_T2S)(const T&)
          >
  struct T2SwrapperFunction<T,T,CONV_T2S>
  {
    using value_type = T;
    using input_type = T;

    inline static std::string
    ToStr(const input_type& val)
    {
      return CONV_T2S(val);
    }
  };
*/
  // ]=============================================================] T2S_FORMAT




  // [=============================================================[ ConvertFromVal

  // https://stackoverflow.com/questions/68058640/using-concepts-to-select-class-template-specialization?rq=3
  template<typename T, typename T2S_FORMAT = T2S_Format_StreamAsIs >
  struct _ConvertFromVal;

  /**
   * @brief     Convertor class implementation for any types, TO string using 'std::ostringstream'.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromVal')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted from, to string data.
   * @tparam  T2S_FORMAT            Class which encapsulates conversion parameters/logic such as 'Locale' (optional argument).
   */
  template<c_NOT_string T, c_formatOSS T2S_FORMAT >
  struct _ConvertFromVal<T,T2S_FORMAT>
  {
    using value_type = T;
    using input_type = T;

    /**
     * @brief   Converts numerical datatype from string holding a numerical value.
     * @param   val                 input numerical value.
     * @returns Output string if conversion succeeds.
     *          Else 'std::invalid_argument' on conversion failure.
     */
    inline static std::string
    ToStr(const T& val)
    {
      using T2S_FORMAT_STREAM = T2S_FORMAT;
      std::ostringstream oss;
      T2S_FORMAT_STREAM::streamUpdate(oss);
      oss << val;
      if (oss.fail() || oss.bad()) // || oss.eof())
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'std::string _ConvertFromVal<c_NOT_string T, c_formatOSS T2S_FORMAT>::ToStr(const T& val)' ::: ";
        try {
          eoss << "val='" << val << "'";
        } catch (...) {} // do-nothing on error
        eoss << " ostringstream-conversion failed.";
        eoss << std::boolalpha << "   iss.fail() = " << oss.fail()
                               << " : iss.bad() = " << oss.bad()
                               << " : iss.eof() = " << oss.eof() << std::endl;
        throw std::invalid_argument(eoss.str());
      }
      return oss.str();
    }
  };

  // for types refer :: https://en.cppreference.com/w/cpp/language/type
  /**
   * @brief     Convertor class implementation from integer types TO string.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromVal')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'type' converted from, to string data.
   */
  template<c_integer_type T>
  struct _ConvertFromVal<T, T2S_Format_std_TtoS >
  {
    using value_type = T;
    using input_type = T;

    /**
     * @brief   Converts integer datatype to string.
     * @param   val                 numerical value.
     * @returns string holding a integer represenation.
     *          Else throws error on conversion failure.
     */
    inline static std::string
    ToStr( const T& val)
    {
      // refer :: https://en.cppreference.com/w/cpp/string/basic_string/to_string
      return std::to_string(val);
    }
  };

  // DONOT USE
  template<c_floating_point T>
  struct _ConvertFromVal<T, T2S_Format_std_TtoS >
  {
    using value_type = T;
    using input_type = T;

    inline static
#ifdef ENABLE_STD_TtoS
    std::string
#else
    // on purpose, rigged for compilation failure if used.
    // for explanation read warning note below
    std::enable_if_t<!std::is_floating_point_v<T>, std::string>
#endif
    ToStr(const T& val)
    {
      /*
         WARNING :: With floating point types std::to_string may yield unexpected results as the number
                    of significant digits in the returned string can be zero, for e.g: pVal = 1e-09

                    The return value may differ significantly from what std::cout prints by default, see the example.

          That's why this specialization is enabled with macro ENABLE_STD_TtoS

         std::cout: 23.43
         to_string: 23.430000

         std::cout: 1e-09
         to_string: 0.000000

         std::cout: 1e+40
         to_string: 10000000000000000303786028427003666890752.000000

         std::cout: 1e-40
         to_string: 0.000000

         std::cout: 1.23457e+08
         to_string: 123456789.000000
       */
      return std::to_string(val);
    }
  };

  /**
   * @brief     Specialized implementation handling string to string conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromVal')
   *            to allow specialization for custom datatype conversions.
   */
  template<>
  struct _ConvertFromVal<std::string, T2S_Format_WorkAround>
  {
    using value_type = std::string;
    using input_type = std::string;

    /**
     * @brief   Converts string to string.
     * @param   val                 input string.
     * @returns string.
     */
    inline static std::string ToStr(const std::string& val) { return val; }
  };

  /**
   * @brief     Specialized implementation handling char to string conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromVal')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T                     'char-type' converted from, to string data.
   */
  template<c_char T>
  struct _ConvertFromVal<T, T2S_Format_WorkAround>
  {
    using value_type = T;
    using input_type = T;

    // TODO unit tests
    /**
     * @brief   Converts char value to string.
     * @param   val                 input string.
     * @returns string.
     */
    inline static std::string
    ToStr(const T& val)
    {
      return std::string(1, val);
    }
  };

  /**
   * @brief     Specialized implementation handling bool to string conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromVal')
   *            to allow specialization for custom datatype conversions.
   */
  template<>
  struct _ConvertFromVal<bool, T2S_Format_WorkAround>
  {
    using value_type = bool;
    using input_type = bool;

    // TODO unit tests
    /**
     * @brief   Converts bool value to string.
     * @param   val                 input bool value.
     * @returns string. either "0" or "1"  (for : 'false' or 'true')
     */
    inline static std::string ToStr(const bool& val)
    {
      return std::to_string(static_cast<int>(val));
    }
  };

  /**
   * @brief     Specialized implementation handling 'year_month_day' to string conversion.
   *            Only intended for converter internal usage, but exposed externally (thru type-alias 'ConvertFromVal')
   *            to allow specialization for custom datatype conversions.
   * @tparam  T2S_FORMAT_YMD        Class which encapsulates conversion parameters/logic such as 'Locale' specific for 'year_month_day'.
   */
  template<c_formatYMDoss T2S_FORMAT_YMD>
  struct _ConvertFromVal< datelib::year_month_day, T2S_FORMAT_YMD>
  {
    using value_type = datelib::year_month_day;
    using input_type = datelib::year_month_day;

    /**
     * @brief   Converts variable holding 'year_month_day' value to string. The string has the format "%F" -> "%Y-%m-%d"
     * @param   val                 input 'year_month_day'.
     * @returns string.
     */
    inline static std::string
    ToStr(  const datelib::year_month_day& val)
    {
      return ToStr_args( val, T2S_FORMAT_YMD::ymdFormat); // %F -> "%Y-%m-%d"
    }
  
    // TODO unit tests
    inline static std::string
    ToStr_args(  const datelib::year_month_day& val,
                 const std::string::value_type* fmt,
                 std::string* abbrev  = nullptr,
                 std::chrono::seconds* offset_sec = nullptr)
    {
      std::ostringstream oss;
      T2S_FORMAT_YMD::streamUpdate(oss);

#if  USE_CHRONO == 1
  #if  DUSE_CHRONO_TOSTREAM == 1
      // As of writing this code, no compiler supports chrono::to_stream() yet.
      // This code here is for future reference once compiler starts supporting.
      // refer https://omegaup.com/docs/cpp/en/cpp/chrono/local_t/to_stream.html
      // no OS in particular
      std::chrono::to_stream(oss, fmt, val, abbrev, offset_sec); // does not compile
  #else
      // msvc supports only from_stream and not to_stream.
      // date-lib is not compatible with msvc (min/max macro clash), therefore
      // we are left with std::format() alternative
      //WINDOWS
      /*  this does not work
      std::string_view fmt_view{fmt};
      std::make_format_args val_fmt_arg{val};
      oss << std::vformat(fmt_view, val_fmt_arg);  // for now , no support for abbrev, offset_sec
      */
      // due to limitations of underlying libs, forced to brute force
      // The conversion here handles a limited sub-set of conversion specifiers

      std::function<void(char)> write;
      write = [&oss,&write,&val] (char convSpecifier) -> void
      {
        switch(convSpecifier)
        {
          case 'Y' :
            oss << std::setfill('0') << std::setw(4) << static_cast<int>(val.year()); break;
          case 'm':
            oss << std::setfill('0') << std::setw(2) << static_cast<unsigned>(val.month()); break;
          case 'd':
            oss << std::setfill('0') << std::setw(2) << static_cast<unsigned>(val.day()); break;
          case 'F': //  "%Y-%m-%d"
            write('Y'); oss << '-'; write('m'); oss << '-'; write('d'); break;
          case 'D': //  "%m/%d/%y"
            write('m'); oss << '/'; write('d'); oss << '/'; write('y'); break;
          case 'y' :
            oss << std::setfill('0') << std::setw(2) << (static_cast<int>(val.year()) % 100); break;
          case 'b' :
            switch(static_cast<unsigned>(val.month()))
            {
              case  1: oss << "Jan"; break; case  2: oss << "Feb"; break; case  3: oss << "Mar"; break;
              case  4: oss << "Apr"; break; case  5: oss << "May"; break; case  6: oss << "Jun"; break;
              case  7: oss << "Jul"; break; case  8: oss << "Aug"; break; case  9: oss << "Sep"; break;
              case 10: oss << "Oct"; break; case 11: oss << "Nov"; break; case 12: oss << "Dec"; break;
              default:
                oss << '%' << convSpecifier;
            }
            break;
          case 'B' :
            switch(static_cast<unsigned>(val.month()))
            {
              case  1: oss << "Janary";  break; case  2: oss << "February"; break; case  3: oss << "March";     break;
              case  4: oss << "April";   break; case  5: oss << "May";      break; case  6: oss << "June";      break;
              case  7: oss << "July";    break; case  8: oss << "August";   break; case  9: oss << "September"; break;
              case 10: oss << "October"; break; case 11: oss << "November"; break; case 12: oss << "December";  break;
              default:
                oss << '%' << convSpecifier;
            }
            break;
          default:
            oss << '%' << convSpecifier;
        }
      };

      if (strlen(fmt)>1)
      {
        enum _ParseState { READ, REPLACE };
        _ParseState state = _ParseState::READ;
        char present = fmt[0];
        if( '%' == present )
          state = _ParseState::REPLACE;
        for(size_t iii = 1; iii < strlen(fmt); ++iii)
        {
          present = fmt[iii];
          if(_ParseState::REPLACE == state)
          {
            write(present);
            state = _ParseState::READ;
          } else {
            if( '%' == present )
            {
              state = _ParseState::REPLACE;
            } else {
              oss << present;
              //state = _ParseState::READ;
            }
          }
        }
      } else {
        if (strlen(fmt)==1)
          oss << fmt[1];
      }
  #endif
#else
      // gcc and clang does not support the required 'chrono' functionality as of writing this code
      // UNIX-like , macOS
      using CS = std::chrono::seconds;
      date::fields<CS> fds{val};
      date::to_stream(oss, fmt, fds, abbrev, offset_sec);
#endif
      if (oss.fail() || oss.bad()) // || oss.eof())
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'std::string _ConvertFromVal<datelib::year_month_date, T2S_FORMAT_YMD>::ToStr_args(const datelib::year_month_date& val)' ::: ";
        try {
          eoss << "year{" << val.year() << "}-month{" << val.month() << "}-day{" << val.day() << "}' : val.ok()=" << val.ok() << " format='" << fmt << "'";
        } catch (...) {} // do-nothing on error
        eoss << " ostringstream-conversion failed.";
        eoss << std::boolalpha << "   iss.fail() = " << oss.fail()
                               << " : iss.bad() = " << oss.bad()
                               << " : iss.eof() = " << oss.eof() << std::endl;
        throw std::invalid_argument(eoss.str());
      }
      return oss.str();
    }
  };

  template<typename T, typename T2S_FORMAT = T2S_Format_StreamAsIs >
  struct _ConvertFromVal_gNaN
  {
    using value_type = T;
    using input_type = std::variant<T, std::string>;

    inline static std::string
    ToStr(const input_type& val)
    {
      if(val.index()==0)
      {
        return _ConvertFromVal<T, T2S_FORMAT>::ToStr(std::get<T>(val));
      } else {
        return std::get<std::string>(val);
      }
    }
  };

  /**
   * @brief     Convertor 'Alias type' for any types, TO string using 'std::istringstream'.
   * @tparam  T                     'type' converted from, to string.
   * @tparam  S2T_FORMAT            Class which encapsulates conversion parameters/logic (optional argument).
   *                                Specialization for custom datatype conversions, using this template parameter.
   */
  template<typename T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
  using ConvertFromVal = _ConvertFromVal<T, T2S_FORMAT >;

  //  ConvertFromVal_fNaN  not needed

  template<typename T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
  using ConvertFromVal_gNaN = _ConvertFromVal_gNaN<T, T2S_FORMAT >;



  template< typename T_C >  // T_C :: T -> can be type such as int, double etc ;  XOR  C -> can be type of 'T2Sconverter'
  using t_T2Sconv = std::conditional_t< is_T2Sconverter< T_C >::value ,
                                          T_C ,
                                          ConvertFromVal< T_C >
                                      >;

  template< typename ... T_C >
  struct ConvertFromTuple
  {
    inline static
    std::string ToStr(std::tuple<typename t_S2Tconv<T_C>::return_type ...> const& theTuple)
    {
      std::stringstream ss;
      std::apply
      (
        [&ss] (typename t_S2Tconv<T_C>::return_type const&... tupleArgs)
        {
          ss << '[';
          std::size_t n{0};
          ((ss << tupleArgs << (++n != sizeof...(T_C) ? ", " : "")), ...);
          ss << ']';
        }, theTuple
      );
      return ss.str();
    }
  };
  // ]=============================================================] ConvertFromVal

}
