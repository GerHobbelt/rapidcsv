// test091.cpp - writing / reading high-precision floating point numbers

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"
#include "utilities.h"


template<converter::c_floating_point T>
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
template<converter::c_floating_point T>
using T2S_Format_ldp = converter::T2S_Format_StreamDecimalPrecision<T, getLowerDecimalPrecision<T>() >;

template<converter::c_floating_point T>
using ConvertFromVal_lDP = converter::ConvertFromVal<T, T2S_Format_ldp<T> >;


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
      rapidcsv::Document doc1(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

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
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

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
      // test with different conversion parameters than the default.
      rapidcsv::Document doc1(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

      doc1.SetCell< ConvertFromVal_lDP<float> >("A", "f", f1);
      doc1.SetCell< ConvertFromVal_lDP<float> >("B", "f", f2);
      doc1.SetCell< ConvertFromVal_lDP<float> >("C", "f", f3);

      doc1.SetCell< ConvertFromVal_lDP<double> >("A", "d", d1);
      doc1.SetCell< ConvertFromVal_lDP<double> >("B", "d", d2);
      doc1.SetCell< ConvertFromVal_lDP<double> >("C", "d", d3);

      doc1.SetCell< ConvertFromVal_lDP<long double> >("A", "ld", ld1);
      doc1.SetCell< ConvertFromVal_lDP<long double> >("B", "ld", ld2);
      doc1.SetCell< ConvertFromVal_lDP<long double> >("C", "ld", ld3);

      doc1.Save();
    }

    {
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

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

      unittest::STRING2VALUECHECK(float, "test091-01",              \
            "3.14159f", doc2.GetCell<float>("A", "f"), 3.14159f,    \
            doc2.GetCell<std::string>("A", "f"));
      unittest::STRING2VALUECHECK(float, "test091-02",                     \
            "3.1415926535f", doc2.GetCell<float>("B", "f"), 3.1415926535f, \
            doc2.GetCell<std::string>("B", "f"));
      unittest::STRING2VALUECHECK(float, "test091-03",                               \
            "3.141592653589793f", doc2.GetCell<float>("C", "f"), 3.141592653589793f, \
            doc2.GetCell<std::string>("C", "f"));

      unittest::STRING2VALUECHECK(double, "test091-04",            \
            "3.14159", doc2.GetCell<double>("A", "d"), 3.14159,    \
            doc2.GetCell<std::string>("A", "d"));
      unittest::STRING2VALUECHECK(double, "test091-05",                             \
            "3.141592653589793", doc2.GetCell<double>("B", "d"), 3.141592653589793, \
            doc2.GetCell<std::string>("B", "d"));
      unittest::STRING2VALUECHECK(double, "test091-06",                                       \
            "3.14159265358979323846", doc2.GetCell<double>("C", "d"), 3.14159265358979323846, \
            doc2.GetCell<std::string>("C", "d"));

      unittest::STRING2VALUECHECK(long double, "test091-07",               \
            "3.14159L", doc2.GetCell<long double>("A", "ld"), 3.14159L,    \
            doc2.GetCell<std::string>("A", "ld"));
      unittest::STRING2VALUECHECK(long double, "test091-08",                                \
            "3.141592653589793L", doc2.GetCell<long double>("B", "ld"), 3.141592653589793L, \
            doc2.GetCell<std::string>("B", "ld"));
      unittest::STRING2VALUECHECK(long double, "test091-09",                                          \
            "3.14159265358979323846L", doc2.GetCell<long double>("C", "ld"), 3.14159265358979323846L, \
            doc2.GetCell<std::string>("C", "ld"));
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