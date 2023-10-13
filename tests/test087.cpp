// test087.cpp - numeric c locale

#include <clocale>
#include <rapidcsv/rapidcsv.h>
#include "unittest.h"

template<typename T>
using convertS2T_stream =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_StreamAsIs<T,
                                                               converter::FailureS2Tprocess::THROW_ERROR,
                                                               char>
                             >;

template<typename T>
using convertS2T_streamClassic =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_StreamUseClassicLocale< T,
                                                                            converter::FailureS2Tprocess::THROW_ERROR,
                                                                            char>
                             >;


template<typename T>
using convertS2T_StoT =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_std_StoT<T,
                                                             converter::FailureS2Tprocess::THROW_ERROR>
                             >;



static constexpr char loc[] = "de_DE.UTF-8"; // uses comma (,) as decimal separator

template<typename T>
using deLocal_iss = converter::S2T_Format_StreamUserLocale<T, converter::FailureS2Tprocess::THROW_ERROR, char, loc>;

template<typename T>
using convertS2T_userLocale =
    converter::ConvertFromStr<T, deLocal_iss<T> >;



int main()
{
  int rv = 0;

  std::string path = unittest::TempPath();

  try
  {
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
      unittest::ExpectEqual(float, doc.GetCell<float>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<float>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<float>("C", "2"), 0.001f);

      unittest::ExpectEqual(float, doc.GetCell<convertS2T_stream<float>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_stream<float>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_stream<float>>("C", "2"), 0.001f);

      unittest::ExpectEqual(float, doc.GetCell<convertS2T_streamClassic<float>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_streamClassic<float>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_streamClassic<float>>("C", "2"), 0.001f);

      unittest::ExpectEqual(float, doc.GetCell<convertS2T_StoT<float>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_StoT<float>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_StoT<float>>("C", "2"), 0.001f);
    }

    {
      std::string csv =
        "-;A;B;C\n"
        "1;1;10;100\n"
        "2;0,1;0,01;0,001\n"
      ;

      unittest::WriteFile(path, csv);

      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0),
                             rapidcsv::SeparatorParams(';' /* pSeparator */));

      unittest::ExpectEqual(float, doc.GetCell<convertS2T_userLocale<float>>("A", "2"), 0.1f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_userLocale<float>>("B", "2"), 0.01f);
      unittest::ExpectEqual(float, doc.GetCell<convertS2T_userLocale<float>>("C", "2"), 0.001f);
    }

    try {
      char* C_locale = std::setlocale(LC_ALL, loc);
      if (C_locale == nullptr)
      {
        std::cout << "locale " << loc << " not available, skipping user-locale test.\n";
        // pass test for systems without locale present. for ci testing, make.sh
        // ensures that the necessary locale is installed.
        return 0;
      }
    }
    catch (const std::exception& ex)
    {
      std::cout << "locale " << loc << " not available (" << ex.what()
                << "), skipping test.\n";
      // pass test for systems without locale present. for ci testing, make.sh
      // ensures that the necessary locale is installed.
      return 0;
    }

    std::string csv =
      "-;A;B;C\n"
      "1;1;10;100\n"
      "2;0,1;0,01;0,001\n"
    ;

    unittest::WriteFile(path, csv);

    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0),
                            rapidcsv::SeparatorParams(';' /* pSeparator */));

/*
    // independent of system-locale
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_stream<float>>("A", "2"), 0.1f);
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_stream<float>>("B", "2"), 0.01f);
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_stream<float>>("C", "2"), 0.001f);

    // independent of system-locale
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_streamClassic<float>>("A", "2"), 0.1f);
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_streamClassic<float>>("B", "2"), 0.01f);
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_streamClassic<float>>("C", "2"), 0.001f);
*/
    // dependent on system-locale
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_StoT<float>>("A", "2"), 0.1f);
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_StoT<float>>("B", "2"), 0.01f);
    unittest::ExpectEqual(float, doc.GetCell<convertS2T_StoT<float>>("C", "2"), 0.001f);
  }
  catch (const std::exception& ex)
  {
    std::cout << "unexpected EXCEPTION : " << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
