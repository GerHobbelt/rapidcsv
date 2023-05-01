// test091.cpp - writing / reading high-precision floating point numbers

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

#define GET_TYPENAME(T) #T

template<typename T>
void checkRoundTripConversion_Val1Val2(
            int testID, const std::string& type,
            const std::string& expected_str,
            const T& val_ddp, const T& val_ldp)
{
  // val_ddp -> value read from text using Default Decimal Precision
  // val_ldp -> value read from text using Lower   Decimal Precision
  if( val_ddp != val_ldp )
  {
    std::cout << std::setprecision(LDBL_DIG+5);
    std::cout << "test091 :: round conversion value does not match for testID=" << testID << " type=" << type << " ..." << std::endl;
    std::cout << "           assgn_val{" << expected_str  << "} ->  val_ddp{" << val_ddp << "}" << std::endl;
    std::cout << "           assgn_val{" << expected_str  << "} ->  val_ldp{" << val_ldp << "}" << std::endl;
    std::cout << "             val_ddp{" << val_ddp << "} != val_ldp{" << val_ldp  << "}" << std::endl << std::endl;
  }
}

template<rapidcsv::c_floating_point T>
constexpr int getLowerDecimalPrecision()
{
  if constexpr (std::is_same_v<T, float>)
    return FLT_DIG;
  else
  if constexpr (std::is_same_v<T, double>)
    return DBL_DIG;
  else
  if constexpr (std::is_same_v<T, long double>)
    return LDBL_DIG;
}
template<rapidcsv::c_floating_point T>
using T2S_Format_ldp = rapidcsv::T2S_Format_StreamDecimalPrecision<T, getLowerDecimalPrecision<T>() >;

template<rapidcsv::c_floating_point T>
struct functionAddress_lDP {
  constexpr static std::string (*CONV_T2S)(const T&) =
                          &rapidcsv::ConvertFromVal<T, T2S_Format_ldp<T> >::ToStr;
};

const float f1 = 3.14159f;
const float f2 = 3.1415926535f;
const float f3 = 3.141592653589793f;
const double d1 = 3.14159;
const double d2 = 3.141592653589793;
const double d3 = 3.14159265358979323846;
const long double ld1 = 3.14159L;
const long double ld2 = 3.141592653589793L;
const long double ld3 = 3.14159265358979323846L;

float f1_ddp, f2_ddp, f3_ddp;
double d1_ddp, d2_ddp, d3_ddp;
long double ld1_ddp, ld2_ddp, ld3_ddp;

int test_a()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "f,0,0,0\n"
    "d,0,0,0\n"
    "ld,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    {
      rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));

      doc1.SetCell<float>("A", "f", f1);
      doc1.SetCell<float>("B", "f", f2);
      doc1.SetCell<float>("C", "f", f3);

      doc1.SetCell<double>("A", "d", d1);
      doc1.SetCell<double>("B", "d", d2);
      doc1.SetCell<double>("C", "d", d3);

      doc1.SetCell<long double>("A", "ld", ld1);
      doc1.SetCell<long double>("B", "ld", ld2);
      doc1.SetCell<long double>("C", "ld", ld3);

      doc1.Save();
    }

    {
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(0, 0));

      unittest::ExpectEqual(float, (f1_ddp = doc2.GetCell<float>("A", "f")), f1);
      unittest::ExpectEqual(float, (f2_ddp = doc2.GetCell<float>("B", "f")), f2);
      unittest::ExpectEqual(float, (f3_ddp = doc2.GetCell<float>("C", "f")), f3);

      unittest::ExpectEqual(double, (d1_ddp = doc2.GetCell<double>("A", "d")), d1);
      unittest::ExpectEqual(double, (d2_ddp = doc2.GetCell<double>("B", "d")), d2);
      unittest::ExpectEqual(double, (d3_ddp = doc2.GetCell<double>("C", "d")), d3);

      unittest::ExpectEqual(long double, (ld1_ddp = doc2.GetCell<long double>("A", "ld")), ld1);
      unittest::ExpectEqual(long double, (ld2_ddp = doc2.GetCell<long double>("B", "ld")), ld2);
      unittest::ExpectEqual(long double, (ld3_ddp = doc2.GetCell<long double>("C", "ld")), ld3);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}

int test_b()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "f,0,0,0\n"
    "d,0,0,0\n"
    "ld,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    {
      rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));

      doc1.SetCell<float, *functionAddress_lDP<float>::CONV_T2S>("A", "f", f1);
      doc1.SetCell<float, *functionAddress_lDP<float>::CONV_T2S>("B", "f", f2);
      doc1.SetCell<float, *functionAddress_lDP<float>::CONV_T2S>("C", "f", f3);

      doc1.SetCell<double, *functionAddress_lDP<double>::CONV_T2S>("A", "d", d1);
      doc1.SetCell<double, *functionAddress_lDP<double>::CONV_T2S>("B", "d", d2);
      doc1.SetCell<double, *functionAddress_lDP<double>::CONV_T2S>("C", "d", d3);

      doc1.SetCell<long double, *functionAddress_lDP<long double>::CONV_T2S>("A", "ld", ld1);
      doc1.SetCell<long double, *functionAddress_lDP<long double>::CONV_T2S>("B", "ld", ld2);
      doc1.SetCell<long double, *functionAddress_lDP<long double>::CONV_T2S>("C", "ld", ld3);

      doc1.Save();
    }

    {
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(0, 0));

/*
      unittest::ExpectEqual(float, doc2.GetCell<float>("A", "f"), f1);
      unittest::ExpectEqual(float, doc2.GetCell<float>("B", "f"), f2);
      unittest::ExpectEqual(float, doc2.GetCell<float>("C", "f"), f3);

      unittest::ExpectEqual(double, doc2.GetCell<double>("A", "d"), d1);
      unittest::ExpectEqual(double, doc2.GetCell<double>("B", "d"), d2);
      unittest::ExpectEqual(double, doc2.GetCell<double>("C", "d"), d3);

      unittest::ExpectEqual(long double, doc2.GetCell<long double>("A", "ld"), ld1);
      unittest::ExpectEqual(long double, doc2.GetCell<long double>("B", "ld"), ld2);
      unittest::ExpectEqual(long double, doc2.GetCell<long double>("C", "ld"), ld3);
*/

      checkRoundTripConversion_Val1Val2(1, GET_TYPENAME(float), "3.14159f",
                                        f1_ddp, doc2.GetCell<float>("A", "f"));
      checkRoundTripConversion_Val1Val2(2, GET_TYPENAME(float), "3.1415926535f",
                                        f2_ddp, doc2.GetCell<float>("B", "f")); 
      checkRoundTripConversion_Val1Val2(3, GET_TYPENAME(float), "3.141592653589793f",
                                        f3_ddp, doc2.GetCell<float>("C", "f"));

      checkRoundTripConversion_Val1Val2(4, GET_TYPENAME(double), "3.14159",
                                        d1_ddp, doc2.GetCell<double>("A", "d")); 
      checkRoundTripConversion_Val1Val2(5, GET_TYPENAME(double), "3.141592653589793",
                                        d2_ddp, doc2.GetCell<double>("B", "d")); 
      checkRoundTripConversion_Val1Val2(6, GET_TYPENAME(double), "3.14159265358979323846",
                                        d3_ddp, doc2.GetCell<double>("C", "d")); 

      checkRoundTripConversion_Val1Val2(7, GET_TYPENAME(long double), "3.14159L",
                                        ld1_ddp, doc2.GetCell<long double>("A", "ld")); 
      checkRoundTripConversion_Val1Val2(8, GET_TYPENAME(long double), "3.141592653589793L",
                                        ld2_ddp, doc2.GetCell<long double>("B", "ld")); 
      checkRoundTripConversion_Val1Val2(9, GET_TYPENAME(long double), "3.14159265358979323846L",
                                        ld3_ddp, doc2.GetCell<long double>("C", "ld"));
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}

int main()
{
  int a = test_a();
  test_b();

  return a;
}