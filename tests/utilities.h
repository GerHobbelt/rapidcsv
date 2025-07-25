#pragma once

#include <iostream>
#include <cmath>
#include <typeinfo>
#include <algorithm>


#include "unittest.h"


#define STRING2VALUECHECK( t, testID, strInput, valStrConv, valExpected,   \
                           strRoundtripActual)                             \
            checkRoundTrip_txt2Val2txt<t>(                                 \
              testID, strInput, valStrConv, valExpected,                   \
              strRoundtripActual, strInput,                                \
              __FILE__, __LINE__)


#define CHECKROUNDTRIP( t, testID, strInput, valExpected,   \
                        strRoundtripExpected)               \
            checkRoundTripConversion_txt2Val2txt<t>(        \
              testID, strInput, valExpected,                \
              strRoundtripExpected,                         \
              __FILE__, __LINE__)


namespace unittest
{
  template<typename T>
  inline std::string getTypeName();

  template<>
  inline std::string getTypeName<float>() { return "float"; }
  template<>
  inline std::string getTypeName<double>() { return "double"; }
  template<>
  inline std::string getTypeName<long double>() { return "long_double"; }


  template<c_floating_point T>
  void checkRoundTrip_txt2Val2txt( const std::string& testID,
              const std::string& strInput,
              const T valStrConv, const T valExpected,
              const std::string& strRoundtripActual,
              const std::string& strRoundtripExpected,
              const std::string& filePath, int lineNo,
              int decimalPrecision = std::numeric_limits<T>::digits10,
              char decimalSeperator = '.', char currencySeperator = '_',
              bool stringent_check = true)
  {
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout << std::setprecision(30); //decimalPrecision + 20);
    std::cout << "#############  testID = " << testID << std::endl;
    std::cout << "decimalPrecision = " << decimalPrecision<< " : std::numeric_limits<"
              << getTypeName<T>() << ">::digits10 = " << std::numeric_limits<T>::digits10 << std::endl;
    std::cout << "          strInput = ";
    std::cout.width(33);
    std::cout << strInput << std::endl;
    std::cout << "        valStrConv = ";
    std::cout.width(33);
    std::cout << valStrConv <<         " :          valExpected = " << valExpected << std::endl;
    unittest::ExpectEqualFun<T>(valExpected, valStrConv, "valExpected", "valStrConv", filePath, lineNo);

  //std::cout << "strRoundtripActual = ";
    std::cout << "strCellStoreActual = ";
    std::cout.width(33);
    std::cout << strRoundtripActual << " : strRoundtripExpected = " << strRoundtripExpected << std::endl;

    if(!stringent_check)
    {
      std::cout << "####### WARNING : skipping stringent-checks" << std::endl;
      return;
    }

    if(strRoundtripExpected.compare(strRoundtripActual)!=0)
    {
      std::string _strRoundtripExpected(strRoundtripExpected);
      std::string _strRoundtripActual(strRoundtripActual);

      if( (!_strRoundtripExpected.empty()) &&
          ( _strRoundtripExpected.back() == 'f' || _strRoundtripExpected.back() == 'L' ) )
      {
        _strRoundtripExpected.pop_back();  // remove trailing 'f' or 'L' if any
      }

      if(currencySeperator != '_')
      {
        _strRoundtripExpected.erase(
            std::remove(_strRoundtripExpected.begin(), _strRoundtripExpected.end(), currencySeperator),
            _strRoundtripExpected.end());
        _strRoundtripActual.erase(
            std::remove(_strRoundtripActual.begin(), _strRoundtripActual.end(), currencySeperator),
            _strRoundtripActual.end() );
      }
      std::string::size_type decSep_expected  = _strRoundtripExpected.find(decimalSeperator);
      std::string::size_type decSep_actual    = _strRoundtripActual.find(decimalSeperator);
      if(decSep_expected != std::string::npos  && decSep_actual != std::string::npos)
      {
        std::cout << "+++++++++++++   fraction check     ++++++++++++++" << std::endl;
        // both input and round-trip-string have decimal seperator
        // for e.g.: '3.3123412e+38'
        if(decSep_expected == decSep_actual)
        {
          // floating-point number(s) of both input and round-trip-string, their fraction part in the the same magnitude range
          // for e.g.: '3.3123412e+38' the fraction part here is '3.3123412'
          std::string significantDigits_expected = _strRoundtripExpected.substr(0, static_cast<size_t>(decimalPrecision)+1); // +1 for decimal-character
          std::string significantDigits_actual   = _strRoundtripActual.substr(0, static_cast<size_t>(decimalPrecision)+1); // +1 for decimal-character

          std::cout << "significant digits check : significantDigits_expected=" << significantDigits_expected
                    << " : significantDigits_actual=" << significantDigits_actual << std::endl;
          unittest::ExpectEqualFun<std::string>(significantDigits_expected, significantDigits_actual,
                "significantDigits_expected", "significantDigits_actual", filePath, lineNo);

          std::istringstream iss_expected(_strRoundtripExpected);
          std::string exponentExpectedStr;
          bool expectedHasExponent = false;
          int expectedExponent = 0;
          if(std::getline(iss_expected, exponentExpectedStr, 'e') &&
             std::getline(iss_expected, exponentExpectedStr, 'e'))  // getline() called twice since 2nd token is exponent
          {
            expectedHasExponent = true;
            std::istringstream expExpected_iss(exponentExpectedStr);
            expExpected_iss >> expectedExponent;
          }

          std::istringstream iss_actual(_strRoundtripActual);
          std::string exponentActualStr;
          bool actualHasExponent = false;
          int actualExponent = 0;
          if(std::getline(iss_actual, exponentActualStr, 'e') &&
             std::getline(iss_actual, exponentActualStr, 'e'))  // getline() called twice since 2nd token is exponent
          {
            actualHasExponent = true;
            std::istringstream expActual_iss(exponentActualStr);
            expActual_iss >> actualExponent;
          }

          if(expectedHasExponent == actualHasExponent)
          {
            if(expectedHasExponent)
            {
              std::cout << "exponent check : expectedExponent= " << expectedExponent
                        << " : actualExponent= " << actualExponent << std::endl;
              unittest::ExpectEqualFun<int>(expectedExponent, actualExponent, "expectedExponent", "actualExponent", filePath, lineNo);
            } else {
              std::cout << "no exponent component on input and converted string" << std::endl;
            }
            return;
          } else {
            std::cout << "exponent component mismatch: _strRoundtripExpected=" << _strRoundtripExpected << std::endl;
            std::cout << "exponent component mismatch:   _strRoundtripActual=" << _strRoundtripActual << std::endl;
          }
        } else {
          std::cout << "decimal position mismatch between _strRoundtripExpected=" << _strRoundtripExpected
                    << " and _strRoundtripActual=" << _strRoundtripActual << std::endl;
        }
      } else if(decSep_expected == std::string::npos  && decSep_actual == std::string::npos) {
        std::cout << "integer significant-digits check" << std::endl;
        // both input and round-trip-string DONOT have decimal seperator
        // for e.g.: '8589973000'
        std::string significantDigits_expected = _strRoundtripExpected.substr(0, static_cast<size_t>(decimalPrecision));
        std::string significantDigits_actual = _strRoundtripActual.substr(0, static_cast<size_t>(decimalPrecision));

        unittest::ExpectEqualFun<std::string>(significantDigits_expected, significantDigits_actual,
              "significantDigits_expected", "significantDigits_actual", filePath, lineNo);
        return;
      }

      std::ostringstream ossConv;
      ossConv << std::setprecision(30); //std::numeric_limits<T>::digits10 + 20);
      ossConv << valStrConv;

      std::cout << testID << " :: WARNING :: roundtrip conversion value does not match for type=" << getTypeName<T>() << " ..." << std::endl;
      //std::cout << "      input-text{" << strRoundtripExpected      << "} -> valStrConv{" << ossConv.str() << "}" << std::endl;
      //std::cout << "         valStrConv{" << ossConv.str() << "} -> roundtrip-text{" << strRoundtripActual << "}" << std::endl;
      //std::cout << "      input-text{" << strRoundtripExpected << "} != roundtrip-text{" << strRoundtripActual << "}" << std::endl;

      //unittest::ExpectEqual(std::string, strRoundtripExpected, strRoundtripActual);
    }
  }


  template<converter::c_floating_point T,
          typename TConvertFromStr = converter::ConvertFromStr<T>,
          typename TConvertFromVal = converter::ConvertFromVal<T> >
  void checkRoundTripConversion_txt2Val2txt( const std::string& testID,
              const std::string& strInput,
              //const T valStrConv,
              const T valExpected,
              //const std::string& strRoundtripActual,
              const std::string& strRoundtripExpected,
              const std::string& filePath, int lineNo,
              int decimalPrecision = std::numeric_limits<T>::digits10,
              char decimalSeperator = '.', char currencySeperator = '_',
              bool stringent_check = true)
  {
    const T valStrConv = TConvertFromStr::ToVal(strInput);
    const std::string strRoundtripActual = TConvertFromVal::ToStr(valStrConv);
    checkRoundTrip_txt2Val2txt<T>(testID, strInput, valStrConv, valExpected,
          strRoundtripActual, strRoundtripExpected, filePath, lineNo,
          decimalPrecision, decimalSeperator, currencySeperator, stringent_check);
  }

}

