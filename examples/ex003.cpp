#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/rowhdr.csv", rapidcsv::LabelParams(-1, 0));

  std::vector<std::string> row = doc.GetRow<std::string>("2017-02-22");
  std::cout << "Read " << row.size() << " values." << std::endl;
}
