#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/rowhdr.csv", rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING, rapidcsv::FlgRowName::RN_PRESENT));

  std::tuple<float, float, float, float, unsigned long, float>
     hlocVa = doc.GetRow<float, float, float, float, unsigned long, float>("2017-02-22");
  std::cout << "Read " << std::tuple_size<decltype(hlocVa)>{} << " values." << std::endl;
}
