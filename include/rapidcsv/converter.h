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

  // TODO - comments 



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
    value = (static_cast<int>(std::numeric_limits<T>::min()) < 0);
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
    value = (static_cast<int>(std::numeric_limits<T>::min()) == 0);
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

  template<typename T>
  concept c_floating_point = std::is_floating_point_v<T>;

  template<typename T>
  concept c_signaling_NaN = std::numeric_limits<T>::has_signaling_NaN;

  template<typename T>
  concept c_number_type = std::is_arithmetic_v<T> && (!is_char<T>::value);
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

  template <typename, typename = void>
  struct has_streamUpdate : std::false_type {};

  template <typename FMT>
  struct has_streamUpdate<FMT, std::void_t<decltype(FMT::streamUpdate),  // check for the presence of FMT::streamUpdate
                                           typename FMT::stream_type>    // check for the presence of FMT::stream_type
                         >
           : std::is_same<decltype(&FMT::streamUpdate), void(*)(typename FMT::stream_type&)>
  {};

  template <typename, typename = void >
  struct is_formatSS : std::false_type {};

  template <typename FMT>
  struct is_formatSS< FMT,
                      typename std::enable_if<
                          has_streamUpdate<FMT>::value &&
                          is_iostream<typename FMT::stream_type>::value
                      >::type
                    >
           : std::true_type
  {};
  template <typename FMT>
  concept c_formatSS = is_formatSS<FMT>::value;

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

  template <typename, typename = void>
  struct is_formatISS : std::false_type {};

  template <typename FMT>
  struct is_formatISS<  FMT,
                        typename std::enable_if<
                            is_formatSS<FMT>::value
                            && std::is_same_v<typename FMT::stream_type,std::istringstream>
                        >::type
                     >
            : std::true_type
  {};
  template <typename FMT>
  concept c_formatISS = is_formatISS<FMT>::value;

  //template<typename T, typename ... ARGS >
  //using f_defConvFuncToVal = T(const std::string & str, ARGS&& ... args);

  template<typename T, typename = void >
  struct S2T_DefaultFormat 
  {
    using type = S2T_Format_StreamUseClassicLocale;
  };

  template<typename T>
  struct S2T_DefaultFormat< T,
                            typename  std::enable_if<
                                        is_integer_type<T>::value ||
                                        std::is_floating_point_v<T>
                                      >::type
                          > 
  {
    using type = S2T_Format_std_StoT;
  };

  template<typename T>
  struct S2T_DefaultFormat< T,
                            typename std::enable_if<
                              std::is_same_v<T, std::string> ||
                              is_char<T>::value ||
                              std::is_same_v<T, bool>
                            >::type
                          > 
  {
    using type = S2T_Format_WorkAround;
  };

  template<>
  struct S2T_DefaultFormat<datelib::year_month_day, void>
  {
    using type = S2T_Format_StreamAsIs;
  };
  // ]=============================================================] S2T_FORMAT


  // [=============================================================[ ConvertFromStr
    // https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/
  //template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
  template<typename T, typename S2T_FORMAT = S2T_Format_StreamAsIs >
  struct _ConvertFromStr
  {
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
    inline static
    typename std::enable_if_t<is_formatISS<S2T_FORMAT>::value
                              && (!std::is_same_v<T, std::string>), T>
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
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<T,S2T_FORMAT_STREAM>::ToVal(const std::string& str)' ::: str='";
        eoss << str << "'  istringstream-conversion failed...";
        eoss << std::boolalpha << "   iss.fail() = " << iss.fail()
                               << " : iss.bad() = " << iss.bad()
                               << " : iss.eof() = " << iss.eof() << std::endl;
        throw std::invalid_argument(eoss.str());
      }
      return val;
    }
  };
 
  template <c_integer_type T>
  struct _ConvertFromStr<T, S2T_Format_std_StoT >
  {
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

    inline static T
    ToVal(const std::string& str)
    {
      return ToVal_args(str);
    }
  };

  template<c_floating_point T>
  struct _ConvertFromStr<T, S2T_Format_std_StoT >
  {
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

    inline static T
    ToVal(const std::string& str)
    {
      return ToVal_args(str);
    }
  };

  template<>
  struct _ConvertFromStr<std::string, S2T_Format_WorkAround>
  {
    inline static std::string ToVal(const std::string& str) { return str; }
  };

  template<c_char T>
  struct _ConvertFromStr< T, S2T_Format_WorkAround >
  {
    // TODO unit tests
    inline static T
    ToVal(const std::string& str)
    {
      if(str.length()>1)
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<T,S2T_Format_WorkAround>::ToVal(const std::string& str)' ::: for T=char-type-(un)signed, str='";
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

  template<>
  struct _ConvertFromStr<bool, S2T_Format_WorkAround>
  {
    // TODO unit tests
    inline static bool
    ToVal(const std::string& str)
    {
      unsigned long val = std::stoul(str); //(str, pos, base);
      if(val > 1)
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<bool,S2T_Format_WorkAround>::ToVal(const std::string& str)' ::: str='";
        eoss << str << "' which violates expected rule # ( val==0 || val==1 )";
        throw std::invalid_argument(eoss.str());
      }
      return val;
    }
  };

  template<c_formatISS S2T_FORMAT_STREAM>
  struct _ConvertFromStr< datelib::year_month_day, S2T_FORMAT_STREAM>
  {
    // TODO unit tests
    inline static datelib::year_month_day
    ToVal(const std::string& str)
    {
      return ToVal_args( str, static_cast<const std::string::value_type*>("%F")); // %F -> "%Y-%m-%d"
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
      S2T_FORMAT_STREAM::streamUpdate(iss);
      datelib::from_stream(iss, fmt, ymd, abbrev, offset);
      if (iss.fail() || iss.bad() ) // || !iss.eof())
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'T _ConvertFromStr<datelib::year_month_day, S2T_FORMAT_STREAM>::ToVal(const std::string& str)' ::: str='";
        eoss << str << "'  istringstream-conversion failed...";
        eoss << std::boolalpha << "   iss.fail() = " << iss.fail()
                               << " : iss.bad() = " << iss.bad()
                               << " : iss.eof() = " << iss.eof() << std::endl;
        throw std::invalid_argument(eoss.str());
      }
      return ymd;
    }
  };

  template<c_signaling_NaN T, typename S2T_FORMAT >
  // this implimentation works only for floating types
  // refer  https://en.cppreference.com/w/cpp/types/numeric_limits/has_signaling_NaN
  struct _ConvertFromStr_fNaN
  {
    inline static T ToVal(const std::string& str) noexcept
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
    inline static T ToVal_args(const std::string& str, ARGS&& ... args) noexcept
    {
      try
      {
        return _ConvertFromStr<T, S2T_FORMAT>::ToVal_args(str, std::forward<ARGS>(args)...);
      } catch (...) {
        return std::numeric_limits<T>::signaling_NaN();
      }
    }
  };

  enum gNaN { isNull=0, isNaN };
  template<c_number_type T, typename S2T_FORMAT>
  struct _ConvertFromStr_gNaN
  {
    inline static std::variant<T, gNaN>
    ToVal(const std::string& str) noexcept
    {
      T val;
      try
      {
        val = _ConvertFromStr<T, S2T_FORMAT>::ToVal(str);
        if constexpr (std::numeric_limits<T>::has_signaling_NaN)
        {
          if (std::isnan(val)) throw std::exception();
        }
      } catch (...) {
        if(str.compare("") == 0) {
          return std::variant<T, gNaN>{gNaN::isNull};
        } else {
          return std::variant<T, gNaN>{gNaN::isNaN};
        }
      }
      return std::variant<T, gNaN>{ val };
    }

    // TODO unit tests
    template<typename ... ARGS>
    inline static std::variant<T, gNaN>
    ToVal_args(const std::string& str, ARGS&& ... args) noexcept
    {
      T val;
      try
      {
        val = _ConvertFromStr<T, S2T_FORMAT>::ToVal_args(str, std::forward<ARGS>(args)...);
        if constexpr (std::numeric_limits<T>::has_signaling_NaN)
        {
          if (std::isnan(val)) throw std::exception();
        }
      } catch (...) {
        if(str.compare("") == 0) {
          return std::variant<T, gNaN>{gNaN::isNull};
        } else {
          return std::variant<T, gNaN>{gNaN::isNaN};
        }
      }
      return std::variant<T, gNaN>{ val };
    }
  };

  template<typename T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  using ConvertFromStr = _ConvertFromStr<T, S2T_FORMAT >;

  template<c_signaling_NaN T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  using ConvertFromStr_fNaN = _ConvertFromStr_fNaN<T, S2T_FORMAT >;

  template<c_number_type T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  using ConvertFromStr_gNaN = _ConvertFromStr_gNaN<T, S2T_FORMAT >;
  // ]=============================================================] ConvertFromStr





  // [=============================================================[ T2S_FORMAT
  struct T2S_Format_WorkAround {};
  struct T2S_Format_std_TtoS {};
  using T2S_Format_StreamAsIs = Format_StreamAsIs<std::ostringstream>;
  using T2S_Format_StreamUseClassicLocale = Format_StreamUseClassicLocale<std::ostringstream>;
  template <const char* usrLoc>
  using T2S_Format_StreamUserLocale = Format_StreamUserLocale<std::ostringstream, usrLoc>;

  template <typename, typename = void>
  struct is_formatOSS : std::false_type {};

  template <typename FMT>
  struct is_formatOSS<  FMT,
                        typename std::enable_if<
                            is_formatSS<FMT>::value
                              && std::is_same_v<typename FMT::stream_type, std::ostringstream>
                        >::type
                     >
            : std::true_type
  {};

  template <typename FMT>
  concept c_formatOSS = is_formatOSS<FMT>::value;

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
  struct T2S_DefaultFormat<T, typename std::enable_if_t<
                                              (   std::is_same_v<T, std::string> ||
                                                  is_char<T>::value ||
                                                  std::is_same_v<T, bool>   ) >
                          >
  {
    using type = T2S_Format_WorkAround;
  };

  template<>
  struct T2S_DefaultFormat<datelib::year_month_day, void>
  {
    using type = T2S_Format_StreamAsIs;
  };
  // ]=============================================================] T2S_FORMAT




  // [=============================================================[ ConvertFromVal
  template<typename T, typename T2S_FORMAT = T2S_Format_StreamAsIs >
  struct _ConvertFromVal
  {
    inline static
    typename std::enable_if_t<is_formatOSS<T2S_FORMAT>::value
                              && (!std::is_same_v<T, std::string>), std::string>
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
        eoss << "ERROR : rapidcsv :: in function 'std::string _ConvertFromVal<T, T2S_FORMAT_STREAM>::ToStr(const T& val)' ::: ";
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
  template<c_integer_type T>
  struct _ConvertFromVal<T, T2S_Format_std_TtoS >
  {
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

  template<>
  struct _ConvertFromVal<std::string, T2S_Format_WorkAround>
  {
    inline static std::string ToStr(const std::string& val) { return val; }
  };

  template<c_char T>
  struct _ConvertFromVal<T, T2S_Format_WorkAround>
  {
    // TODO unit tests
    inline static std::string
    ToStr(const T& val)
    {
      return std::string(1, val);
    }
  };

  template<>
  struct _ConvertFromVal<bool, T2S_Format_WorkAround>
  {
    // TODO unit tests
    inline static std::string ToStr(const bool& val)
    {
      return std::to_string(static_cast<int>(val));
    }
  };

  template<c_formatOSS T2S_FORMAT_STREAM>
  struct _ConvertFromVal< datelib::year_month_day, T2S_FORMAT_STREAM>
  {
    // TODO unit tests
    inline static std::string
    ToStr(  const datelib::year_month_day& val)
    {
      return ToStr_args( val, static_cast<const std::string::value_type*>("%F")); // %F -> "%Y-%m-%d"
    }
  
    // TODO unit tests
    inline static std::string
    ToStr_args(  const datelib::year_month_day& val,
                 const std::string::value_type* fmt,
                 std::string* abbrev  = nullptr,
                 std::chrono::seconds* offset_sec = nullptr)
    {
      std::ostringstream oss;
      T2S_FORMAT_STREAM::streamUpdate(oss);
#if  USE_CHRONO == 1
      // refer https://omegaup.com/docs/cpp/en/cpp/chrono/local_t/to_stream.html
      std::chrono::to_stream(oss, fmt, val, abbrev, offset_sec);
#else
      using CT = std::chrono::seconds;
      date::fields<CT> fds{val};
      date::to_stream(oss, fmt, fds, abbrev, offset_sec);
#endif
      if (oss.fail() || oss.bad()) // || oss.eof())
      {
        std::ostringstream eoss;
        eoss << __RAPIDCSV_FILE__ << ":" << __LINE__ << " ";
        eoss << "ERROR : rapidcsv :: in function 'std::string _ConvertFromVal<datelib::year_month_date, T2S_FORMAT_STREAM>::ToStr(const datelib::year_month_date& val)' ::: ";
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

  template<typename T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
  using ConvertFromVal = _ConvertFromVal<T, T2S_FORMAT >;
  // ]=============================================================] ConvertFromVal

}
