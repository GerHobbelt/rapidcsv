// test053.cpp - write numbers with scientific notation

#include <cmath>
#include <iomanip>

#include <rapidcsv/rapidcsv.h>

#include "unittest.h"

/*
using BigFloat do a precision comparison later. TODO
#include "BigFloat.h"
*/

#define GET_TYPENAME(T) #T

template<typename T>
void checkRoundTripConversion_Val2txt(
            int testID, const std::string& type,
            const T& stored_val, const std::string& expected_str)
{
  std::ostringstream oss;
  //oss << std::setprecision(std::numeric_limits<T>::digits10 + 1);
  oss << std::setprecision(LDBL_DIG+5);
  oss << stored_val;

  if(expected_str.compare(oss.str())!=0)
  {
    // TODO use BigFloat comparision here
    std::cout << std::setprecision(LDBL_DIG+5);
    std::cout << "test053 :: round conversion value does not match for testID=" << testID << " type=" << type << " ..." << std::endl;
    std::cout << "       assgn_val{" << expected_str  << "} ->   stored_val{" << stored_val   << "}" << std::endl;
    std::cout << "      stored_val{" << stored_val    << "} ->      val2txt{" << oss.str()    << "}" << std::endl;
    std::cout << "         val2txt{" << oss.str()     << "} !=    assgn_val{" << expected_str << "}" << std::endl << std::endl;
  }
}

/*
using BigFloat do a precision comparison later. TODO
#include "BigFloat.h"
*/

int main()
{
  int rv = 0;

  std::string csvref =
    "-,A,B,C\n"
    "1,1.2e10,2.00E-07,1e100\n"
  ;

  std::string csv =
    "-,A,B,C\n"
    "1,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));

    //std::string tstr = rapidcsv::ConvertFromVal<double,format_type>::ToStr(1.2e10);
    //std::string tstr = rapidcsv::ConvertFromVal<double>::ToStr(1.2e10);
    doc1.SetCell<double, &rapidcsv::ConvertFromVal<double>::ToStr>("A", "1", 1.2e10);
    doc1.SetCell<double>("B", "1", 2.00E-07);
    // std::cout << "doc1.GetCell<double>(\"B\", \"1\") = " << std::setprecision(10) << doc1.GetCell<double>("B", "1") << std::endl;
    doc1.SetCell<double>("C", "1", 1e100);

    doc1.Save();

    rapidcsv::Document doc2(path, rapidcsv::LabelParams(0, 0));

    double eps1 = 1.0e7;
    double exp1 = 1.2e10;
    double val1 = doc2.GetCell<double>("A", "1");
    unittest::ExpectTrue(fabs(val1 - exp1) < eps1);
    checkRoundTripConversion_Val2txt(0, GET_TYPENAME(double),
                                        val1, "1.2e10"); 

    double eps2 = 1.0e-10;
    double exp2 = 2.00e-07;
    double val2 = doc2.GetCell<double>("B", "1");
    unittest::ExpectTrue(fabs(val2 - exp2) < eps2);
    checkRoundTripConversion_Val2txt(1, GET_TYPENAME(double),
                                        val2, "2.00e-07"); 

    double eps3 = 1e97;
    double exp3 = 1e100;
    double val3 = doc2.GetCell<double>("C", "1");
    unittest::ExpectTrue(fabs(val3 - exp3) < eps3);
    checkRoundTripConversion_Val2txt(2, GET_TYPENAME(double),
                                        val3, "1e100"); 
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
