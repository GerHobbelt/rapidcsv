#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/nohdr.csv", rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING, rapidcsv::FlgRowName::RN_MISSING));

  std::vector<float> close = doc.GetColumn<float>(5);
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>(4, 2);
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
