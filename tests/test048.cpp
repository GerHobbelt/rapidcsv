// test048.cpp - default conversion to default values

#include <cmath>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

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

    unittest::ExpectEqual(int, doc.GetCell<int COMMA 1 COMMA 1>(0, 0), 0);
    unittest::ExpectEqual(long long, doc.GetCell<long long COMMA 1 COMMA 1>(1, 0), 0);
    unittest::ExpectEqual(unsigned int, doc.GetCell<unsigned int COMMA 1 COMMA 1>(2, 0), 0);

    unittest::ExpectTrue(std::isnan(doc.GetCell<double COMMA 1 COMMA 1>(0, 1)));
    unittest::ExpectTrue(std::isnan(doc.GetCell<long double COMMA 1 COMMA 1>(1, 1)));
    unittest::ExpectTrue(std::isnan(doc.GetCell<float COMMA 1 COMMA 1>(2, 1)));
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
