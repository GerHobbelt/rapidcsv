// testViewYmd003.cpp - read column value

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
    std::vector<rdb::year_month_day> tradingDate;
    std::vector<double> open;
    std::vector<unsigned long> volume;
    std::vector<long double> adjClose;

    rapidcsv::Document doc("../tests/msft.csv", rapidcsv::LabelParams(0, -1));

    /////  Filter
    rapidcsv::FilterDocument<isYear2016> viewdoc(doc);

    tradingDate = viewdoc.GetViewColumn<rdb::year_month_day>("Date");
    unittest::ExpectEqual(size_t, tradingDate.size(), 252);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(0), rdb::year{2016}/12/30);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(1), rdb::year{2016}/12/29);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(2), rdb::year{2016}/12/28);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(3), rdb::year{2016}/12/27);

    open = viewdoc.GetViewColumn<double>(1);
    unittest::ExpectEqual(size_t, open.size(), 252);
    unittest::ExpectEqual(double, open.at(0), 62.959999);
    unittest::ExpectEqual(double, open.at(1), 62.860001);
    unittest::ExpectEqual(double, open.at(2), 63.400002);
    unittest::ExpectEqual(double, open.at(3), 63.209999);

    volume = viewdoc.GetViewColumn<unsigned long>("Volume");
    unittest::ExpectEqual(size_t, volume.size(), 252);
    unittest::ExpectEqual(unsigned long, volume.at(0), 25579900);
    unittest::ExpectEqual(unsigned long, volume.at(1), 10250600);
    unittest::ExpectEqual(unsigned long, volume.at(2), 14653300);
    unittest::ExpectEqual(unsigned long, volume.at(3), 11763200);

    adjClose = viewdoc.GetViewColumn<long double>("Adj Close");
    unittest::ExpectEqual(size_t, adjClose.size(), 252);
    unittest::ExpectEqual(long double, adjClose.at(0), 61.765546L);
    unittest::ExpectEqual(long double, adjClose.at(1), 62.520968L);
    unittest::ExpectEqual(long double, adjClose.at(2), 62.610426L);
    unittest::ExpectEqual(long double, adjClose.at(3), 62.898675L);

    /////   Sort
    const rapidcsv::SortParams<rdb::year_month_day> spA(0);
    rapidcsv::SortDocument<decltype(spA)> viewdoc1(doc, spA);   // `<decltype(spA)>` mandatory for clang

    tradingDate = viewdoc1.GetViewColumn<rdb::year_month_day>("Date");
    unittest::ExpectEqual(size_t, tradingDate.size(), 7804);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(0), rdb::year{1986}/03/13);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(1), rdb::year{1986}/03/14);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(2), rdb::year{1986}/03/17);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(3), rdb::year{1986}/03/18);

    open = viewdoc1.GetViewColumn<double>(1);
    unittest::ExpectEqual(size_t, open.size(), 7804);
    unittest::ExpectEqual(double, open.at(0), 25.499999);
    unittest::ExpectEqual(double, open.at(1), 28.00);
    unittest::ExpectEqual(double, open.at(2), 29.000001);
    unittest::ExpectEqual(double, open.at(3), 29.499999);

    volume = viewdoc1.GetViewColumn<unsigned long>("Volume");
    unittest::ExpectEqual(size_t, volume.size(), 7804);
    unittest::ExpectEqual(unsigned long, volume.at(0), 1031788800);
    unittest::ExpectEqual(unsigned long, volume.at(1), 308160000);
    unittest::ExpectEqual(unsigned long, volume.at(2), 133171200);
    unittest::ExpectEqual(unsigned long, volume.at(3), 67766400);

    adjClose = viewdoc1.GetViewColumn<long double>("Adj Close");
    unittest::ExpectEqual(size_t, adjClose.size(), 7804);
    unittest::ExpectEqual(long double, adjClose.at(0), 0.065616L);
    unittest::ExpectEqual(long double, adjClose.at(1), 0.06796L);
    unittest::ExpectEqual(long double, adjClose.at(2), 0.069131L);
    unittest::ExpectEqual(long double, adjClose.at(3), 0.067374L);

    ////  Filter + Sort
    const rapidcsv::SortParams<rdb::year_month_day, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isYear2016, decltype(spD)> viewdoc2(doc, spD);

    tradingDate = viewdoc2.GetViewColumn<rdb::year_month_day>("Date");
    unittest::ExpectEqual(size_t, tradingDate.size(), 252);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(0), rdb::year{2016}/12/30);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(1), rdb::year{2016}/12/29);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(2), rdb::year{2016}/12/28);
    unittest::ExpectEqual(rdb::year_month_day, tradingDate.at(3), rdb::year{2016}/12/27);

    open = viewdoc2.GetViewColumn<double>(1);
    unittest::ExpectEqual(size_t, open.size(), 252);
    unittest::ExpectEqual(double, open.at(0), 62.959999);
    unittest::ExpectEqual(double, open.at(1), 62.860001);
    unittest::ExpectEqual(double, open.at(2), 63.400002);
    unittest::ExpectEqual(double, open.at(3), 63.209999);

    volume = viewdoc2.GetViewColumn<unsigned long>("Volume");
    unittest::ExpectEqual(size_t, volume.size(), 252);
    unittest::ExpectEqual(unsigned long, volume.at(0), 25579900);
    unittest::ExpectEqual(unsigned long, volume.at(1), 10250600);
    unittest::ExpectEqual(unsigned long, volume.at(2), 14653300);
    unittest::ExpectEqual(unsigned long, volume.at(3), 11763200);

    adjClose = viewdoc2.GetViewColumn<long double>("Adj Close");
    unittest::ExpectEqual(size_t, adjClose.size(), 252);
    unittest::ExpectEqual(long double, adjClose.at(0), 61.765546L);
    unittest::ExpectEqual(long double, adjClose.at(1), 62.520968L);
    unittest::ExpectEqual(long double, adjClose.at(2), 62.610426L);
    unittest::ExpectEqual(long double, adjClose.at(3), 62.898675L);

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
