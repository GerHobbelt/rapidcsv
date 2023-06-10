#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::tuple<float, float, float, float, unsigned long, float>
     hlocVa = doc.GetRow<float, float, float, float, unsigned long, float>("2017-02-22");
  std::cout << "Read " << std::tuple_size<decltype(hlocVa)>{} << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
