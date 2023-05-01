// test092.cpp - numeric c++ locale

#include <clocale>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string loc = "de_DE"; // uses comma (,) as decimal separator
  try
  {
    std::locale::global(std::locale(loc));
  }
  catch (const std::exception& ex)
  {
    std::cout << "locale " << loc << " not available (" << ex.what()
              << "), skipping test.\n";
    // pass test for systems without locale present. for ci testing, make.sh
    // ensures that the necessary locale is installed.
    return 0;
  }

  std::string path = unittest::TempPath();

  try
  {
    {
      std::string csv =
        "-;A;B;C\n"
        "1;1;10;100\n"
        "2;0,1;0,01;0,001\n"
      ;

      unittest::WriteFile(path, csv);

      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0),
                             rapidcsv::SeparatorParams(';' /* pSeparator */));
      unittest::ExpectEqual(float, doc.GetCell<float>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<float>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<float>("C", "2"), 0.001f);
    }

    {
      std::string csv =
        "-,A,B,C\n"
        "1,1,10,100\n"
        "2,0.1,0.01,0.001\n"
      ;

      unittest::WriteFile(path, csv);

      rapidcsv::LabelParams labelParams(0, 0);
      rapidcsv::SeparatorParams separatorParams;
      rapidcsv::Document doc(path, labelParams, separatorParams);
      unittest::ExpectEqual(float, doc.GetCell<float COMMA &rapidcsv::ConvertFromStr<float COMMA rapidcsv::S2T_Format_StreamAsIs>::ToVal>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<float COMMA &rapidcsv::ConvertFromStr<float COMMA rapidcsv::S2T_Format_StreamAsIs>::ToVal>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<float COMMA &rapidcsv::ConvertFromStr<float COMMA rapidcsv::S2T_Format_StreamAsIs>::ToVal>("C", "2"), 0.001f);
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
