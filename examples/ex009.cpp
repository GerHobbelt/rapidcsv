#include <iostream>
#include <vector>
#include <cmath>

#include <rapidcsv/rapidcsv.h>

int ConvFixPoint(const std::string& pStr)
{
  return static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

struct MyStruct
{
  int val = 0;
};

MyStruct ConvMyStruct(const std::string& pStr)
{
  MyStruct ms;
  ms.val = static_cast<int>(roundf(100.0f * std::stof(pStr)));
  return ms;
}

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::cout << "regular         = " << doc.GetCell<int>("Close", "2017-02-21") << "\n";
  std::cout << "fixpointfunc    = " << doc.GetCell<&ConvFixPoint>("Close", "2017-02-21") << "\n";

  /*
    Lambda-expressions are not allowed in unevaluated expressions, template arguments,
    alias declarations, typedef declarations, and anywhere in a function
    (or function template) declaration except the function body and the function's default
    arguments.  -- (until C++20)  -- perhaps when bumping up to C++2Z, the below 2 lines
    can be uncommented
  */
  // TODO C++2Z check if it complies
  //auto convFixLambda = [](const std::string& pStr) { return static_cast<int>(roundf(100.0f * stof(pStr))); };
  //std::cout << "fixpointlambda  = " << doc.GetCell<convFixLambda>("Close", "2017-02-21") << "\n";

  std::cout << "mystruct        = " << doc.GetCell<&ConvMyStruct>("Close", "2017-02-21").val << "\n";
}
