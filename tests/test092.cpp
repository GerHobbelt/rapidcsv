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

template<typename T>
using deLocal_iss = converter::S2T_Format_StreamUserLocale<T, converter::FailureS2Tprocess::THROW_ERROR, char, de_Loc>;

int main()
{
  int rv = 0;

  try
  {
    std::istringstream iss;
    deLocal_iss<float>::streamUpdate(iss);
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

      rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT),
                             rapidcsv::SeparatorParams(';' /* pSeparator */));
      unittest::ExpectEqual(float, doc.GetCell<converter::ConvertFromStr<float COMMA deLocal_iss<float>>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<converter::ConvertFromStr<float COMMA deLocal_iss<float>>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<converter::ConvertFromStr<float COMMA deLocal_iss<float>>>("C", "2"), 0.001f);

      unittest::DeleteFile(path);
    }

    {
      std::string csv =
        "-,A,B,C\n"
        "1,1,10,100\n"
        "2,0.1,0.01,0.001\n"
      ;

      unittest::WriteFile(path, csv);

      rapidcsv::LabelParams labelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT);
      rapidcsv::SeparatorParams separatorParams;
      rapidcsv::Document doc(path, labelParams, separatorParams);
      unittest::ExpectEqual(float, doc.GetCell<converter::ConvertFromStr<float COMMA converter::S2T_Format_StreamAsIs<float COMMA converter::FailureS2Tprocess::THROW_ERROR COMMA char>>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<converter::ConvertFromStr<float COMMA converter::S2T_Format_StreamAsIs<float COMMA converter::FailureS2Tprocess::THROW_ERROR COMMA char>>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<converter::ConvertFromStr<float COMMA converter::S2T_Format_StreamAsIs<float COMMA converter::FailureS2Tprocess::THROW_ERROR COMMA char>>>("C", "2"), 0.001f);

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
