// testViewYmd001.cpp - read cell value

#include <rapidcsv/view.h>
#include "unittest.h"

namespace rdb = rapidcsv::datelib;


bool isYear2016(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : data-zero-index is label
  return ( rapidcsv::ConvertFromStr< rdb::year_month_day >::ToVal(dataRow.at(0)).year() == rdb::year(2016) );
}


int main()
{
  int rv = 0;

  try
  {
    rapidcsv::Document doc("../tests/msft.csv", rapidcsv::LabelParams(0, 0));

    /////  Filter
    rapidcsv::FilterDocument<isYear2016> viewdoc(doc);

    unittest::ExpectEqual(size_t, viewdoc.GetViewRowCount(), 252);

    unittest::ExpectEqual(double, viewdoc.GetViewCell<double>(0, 0), 62.959999);
    unittest::ExpectEqual(unsigned long, viewdoc.GetViewCell<unsigned long>(4, 0), 25579900);
    unittest::ExpectEqual(long double, viewdoc.GetViewCell<long double>(5, 0), 61.765546L);

    unittest::ExpectEqual(double, viewdoc.GetViewCell<double>("Open", 1), 62.860001);
    unittest::ExpectEqual(unsigned long, viewdoc.GetViewCell<unsigned long>("Volume", 1), 10250600);
    unittest::ExpectEqual(long double, viewdoc.GetViewCell<long double>("Adj Close", 1), 62.520968L);

    unittest::ExpectEqual(double, viewdoc.GetViewCell<double>("Open", "2016-12-28"), 63.400002);
    unittest::ExpectEqual(unsigned long, viewdoc.GetViewCell<unsigned long>("Volume", "2016-12-28"), 14653300);
    unittest::ExpectEqual(long double, viewdoc.GetViewCell<long double>("Adj Close", "2016-12-28"), 62.610426L);

    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("Open", "2016-12-27"), "63.209999");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("Volume", "2016-12-27"), "11763200");
    unittest::ExpectEqual(std::string, viewdoc.GetViewCell<std::string>("Adj Close", "2016-12-27"), "62.898675");

    /////   Sort
    const rapidcsv::SortParams<rdb::year_month_day> spA(0);
    rapidcsv::SortDocument viewdoc1(doc, spA);

    unittest::ExpectEqual(size_t, viewdoc1.GetViewRowCount(), 7804);

    unittest::ExpectEqual(double, viewdoc1.GetViewCell<double>(0, 0), 25.499999);
    unittest::ExpectEqual(unsigned long, viewdoc1.GetViewCell<unsigned long>(4, 0), 1031788800);
    unittest::ExpectEqual(long double, viewdoc1.GetViewCell<long double>(5, 0), 0.065616L);

    unittest::ExpectEqual(double, viewdoc1.GetViewCell<double>("Open", 1), 28.00);
    unittest::ExpectEqual(unsigned long, viewdoc1.GetViewCell<unsigned long>("Volume", 1), 308160000);
    unittest::ExpectEqual(long double, viewdoc1.GetViewCell<long double>("Adj Close", 1), 0.06796L);

    unittest::ExpectEqual(double, viewdoc1.GetViewCell<double>("Open", "1986-03-17"), 29.000001);
    unittest::ExpectEqual(unsigned long, viewdoc1.GetViewCell<unsigned long>("Volume", "1986-03-17"), 133171200);
    unittest::ExpectEqual(long double, viewdoc1.GetViewCell<long double>("Adj Close", "1986-03-17"), 0.069131L);

    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>("Open", "1986-03-18"), "29.499999");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>("Volume", "1986-03-18"), "67766400");
    unittest::ExpectEqual(std::string, viewdoc1.GetViewCell<std::string>("Adj Close", "1986-03-18"), "0.067374");

    rdb::year_month_day tDate{rdb::year{1986}, rdb::month{3}, rdb::day{19} };
    rapidcsv::SortKeyFactory<decltype(spA)>::t_sortKey riDate{tDate};
    unittest::ExpectEqual(double, viewdoc1.GetOrderedCell<double>("Open", riDate), 28.75);
    unittest::ExpectEqual(unsigned long, viewdoc1.GetOrderedCell<unsigned long>("Volume", riDate), 47894400);
    unittest::ExpectEqual(long double, viewdoc1.GetOrderedCell<long double>("Adj Close", riDate), 0.066202L);

    /////  Filter + Sort
    const rapidcsv::SortParams<rdb::year_month_day, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isYear2016, decltype(spD)> viewdoc2(doc, spD);

    unittest::ExpectEqual(size_t, viewdoc2.GetViewRowCount(), 252);

    unittest::ExpectEqual(double, viewdoc2.GetViewCell<double>(0, 0), 62.959999);
    unittest::ExpectEqual(unsigned long, viewdoc2.GetViewCell<unsigned long>(4, 0), 25579900);
    unittest::ExpectEqual(long double, viewdoc2.GetViewCell<long double>(5, 0), 61.765546L);

    unittest::ExpectEqual(double, viewdoc2.GetViewCell<double>("Open", 1), 62.860001);
    unittest::ExpectEqual(unsigned long, viewdoc2.GetViewCell<unsigned long>("Volume", 1), 10250600);
    unittest::ExpectEqual(long double, viewdoc2.GetViewCell<long double>("Adj Close", 1), 62.520968L);

    unittest::ExpectEqual(double, viewdoc2.GetViewCell<double>("Open", "2016-12-28"), 63.400002);
    unittest::ExpectEqual(unsigned long, viewdoc2.GetViewCell<unsigned long>("Volume", "2016-12-28"), 14653300);
    unittest::ExpectEqual(long double, viewdoc2.GetViewCell<long double>("Adj Close", "2016-12-28"), 62.610426L);

    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>("Open", "2016-12-27"), "63.209999");
    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>("Volume", "2016-12-27"), "11763200");
    unittest::ExpectEqual(std::string, viewdoc2.GetViewCell<std::string>("Adj Close", "2016-12-27"), "62.898675");

    rdb::year_month_day tDate2{rdb::year{2016}, rdb::month{12}, rdb::day{23} };
    rapidcsv::SortKeyFactory<decltype(spD)>::t_sortKey riDate2{tDate2};
    unittest::ExpectEqual(double, viewdoc2.GetOrderedCell<double>("Open", riDate2), 63.450001);
    unittest::ExpectEqual(unsigned long, viewdoc2.GetOrderedCell<unsigned long>("Volume", riDate2), 12403800);
    unittest::ExpectEqual(long double, viewdoc2.GetOrderedCell<long double>("Adj Close", riDate2), 62.858919L);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
