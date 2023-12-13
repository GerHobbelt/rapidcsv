#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/semi.csv", rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_PRESENT),
                         rapidcsv::SeparatorParams(';'));

  std::vector<float> close = doc.GetColumn<float>("Close");
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
