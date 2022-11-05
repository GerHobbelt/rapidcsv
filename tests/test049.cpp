// test049.cpp - default conversion to custom values

#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

namespace rapidcsv
{
  template<typename T>
  struct NaNaccess<T,2>
  {
    static T getNaN()
    {
      if(std::is_integral<T>::value)
        return T(1);
      if(std::is_floating_point<T>::value)
        return T(0.0);
    }
  };
}

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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams());

    unittest::ExpectEqual(int, doc.GetCell<int COMMA 1 COMMA 2>(0, 0), 1);
    unittest::ExpectEqual(long long, doc.GetCell<long long COMMA 1 COMMA 2>(1, 0), 1);
    unittest::ExpectEqual(unsigned int, doc.GetCell<unsigned int COMMA 1 COMMA 2>(2, 0), 1);

    unittest::ExpectEqual(double, doc.GetCell<double COMMA 1 COMMA 2>(0, 1), 0.0);
    unittest::ExpectEqual(long double, doc.GetCell<long double COMMA 1 COMMA 2>(1, 1), 0.0);
    unittest::ExpectEqual(float, doc.GetCell<float COMMA 1 COMMA 2>(2, 1), 0.0);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
