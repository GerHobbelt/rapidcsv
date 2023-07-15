// test template instantiation is happening as expected


#include <cmath>
#include <iomanip>

#include <rapidcsv/rapidcsv.h>

#include "unittest.h"


int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
    "text,asfa,dfsdfg,sdgd\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::SeparatorParams seppar;
    seppar.mQuotedLinebreaks = true;
    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1), seppar);

    doc.GetCell< converter::ConvertFromStr<char, converter::S2T_Format_StreamAsIs> >(3, 0);
    doc.GetCell< converter::ConvertFromStr<int, converter::S2T_Format_StreamAsIs> >(3, 1);
    doc.GetCell< converter::ConvertFromStr<unsigned int, converter::S2T_Format_StreamAsIs> >(3, 2);
    doc.GetCell<std::string>(3, 3);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
