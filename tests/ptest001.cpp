// ptest001.cpp - file load and get data by column, row and cell

#include <rapidcsv/rapidcsv.h>
#include "perftest.h"
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    perftest::Timer timer;

    for (int i = 0; i < 10; ++i)
    {
      timer.Start();

      rapidcsv::Document doc("../tests/msft.csv", rapidcsv::LabelParams(0, 0));
      [[maybe_unused]] 	const std::vector<double> column = doc.GetColumn<double>("Close");
      [[maybe_unused]] 	const std::tuple<float, float, float, float, unsigned long, float>
             row = doc.GetRow<float, float, float, float, unsigned long, float>("2016-05-23");
      [[maybe_unused]] 	const double cell = doc.GetCell<double>("Close", "2016-05-23");

      timer.Stop();
    }

    timer.ReportMedian();
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
