#include <string>
#include <cassert>
#include <iostream>

int main()
{
  const std::string __file__(__FILE__);
  const std::string expectedFile("tests/checkMacroPrefixMap.cpp");

  std::cout<< "__file__(" << __file__ << ") ?==? expectedFile(" << expectedFile << ")" << std::endl;

  assert(__file__.compare(expectedFile) == 0);
}
