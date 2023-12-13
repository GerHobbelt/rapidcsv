#include <iostream>
#include <vector>
#include <cmath>

#include <rapidcsv/rapidcsv.h>

int ToVal(const std::string& pStr)
{
  return static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT));

  std::vector<int> close = doc.GetColumn< &ToVal >("Close");
  std::cout << "close[0]  = " << close[0] << std::endl;
  std::cout << "close[1]  = " << close[1] << std::endl;
}
