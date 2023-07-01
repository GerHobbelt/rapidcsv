// test092.cpp - numeric c++ locale

#include <clocale>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

/*
 * To run this test, locale 'de_DE' needs to be installed on the system.
 * on ubuntu steps are...
 * $ locale -a                                     # to check existing locale(s)
 * $ sudo apt-get install language-pack-de
 * $ locale -a                                     # check again
 * $ sudo locale-gen de_DE
 * $ sudo update-locale
 * 
 * refer ::: https://askubuntu.com/questions/76013/how-do-i-add-locale-to-ubuntu-server
 */


// string literal object with static storage duration
constexpr char de_Loc[] = "de_DE"; // uses comma (,) as decimal separator

int main()
{
  int rv = 0;

  using deLocal_iss = rapidcsv::S2T_Format_StreamUserLocale<de_Loc>;
  try
  {
    std::istringstream iss;
    deLocal_iss::streamUpdate(iss);
  }
  catch (const std::exception& ex)
  {
    std::cout << "locale " << de_Loc << " not available (" << ex.what()
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
      unittest::ExpectEqual(float, doc.GetCell<rapidcsv::ConvertFromStr<float COMMA deLocal_iss>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<rapidcsv::ConvertFromStr<float COMMA deLocal_iss>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<rapidcsv::ConvertFromStr<float COMMA deLocal_iss>>("C", "2"), 0.001f);

      unittest::DeleteFile(path);
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
      unittest::ExpectEqual(float, doc.GetCell<rapidcsv::ConvertFromStr<float COMMA rapidcsv::S2T_Format_StreamAsIs>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<rapidcsv::ConvertFromStr<float COMMA rapidcsv::S2T_Format_StreamAsIs>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<rapidcsv::ConvertFromStr<float COMMA rapidcsv::S2T_Format_StreamAsIs>>("C", "2"), 0.001f);

      unittest::DeleteFile(path);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;

    unittest::DeleteFile(path);
  }

  return rv;
}
