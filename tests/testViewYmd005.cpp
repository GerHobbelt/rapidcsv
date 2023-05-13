// testViewYmd005.cpp - read row value

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
  std::string mapErr;
  try {
    std::map<int,int> rdmp;
    rdmp.at(0);
  } catch (std::out_of_range& err) {
    mapErr = err.what();
  }

  try
  {
    std::vector<std::string> strs;

    rapidcsv::Document doc("../tests/msft.csv", rapidcsv::LabelParams(0, 0));

    /////  Filter
    rapidcsv::FilterDocument<isYear2016> viewdoc(doc);

    strs = viewdoc.GetViewRow<std::string>(0);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.959999");
    unittest::ExpectEqual(std::string, strs.at(4), "25579900");
    unittest::ExpectEqual(std::string, strs.at(5), "61.765546");

    strs = viewdoc.GetViewRow<std::string>("2016-12-29");
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.860001");
    unittest::ExpectEqual(std::string, strs.at(4), "10250600");
    unittest::ExpectEqual(std::string, strs.at(5), "62.520968");

    /////   Sort
    const rapidcsv::SortParams<rdb::year_month_day> spA(0);
    rapidcsv::SortDocument<rdb::year_month_day> viewdoc1(doc, spA);

    strs = viewdoc1.GetViewRow<std::string>(0);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "25.499999");
    unittest::ExpectEqual(std::string, strs.at(4), "1031788800");
    unittest::ExpectEqual(std::string, strs.at(5), "0.065616");

    strs = viewdoc1.GetViewRow<std::string>("1986-03-14");
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "28.00");
    unittest::ExpectEqual(std::string, strs.at(4), "308160000");
    unittest::ExpectEqual(std::string, strs.at(5), "0.06796");

    rdb::year_month_day tDate{rdb::year{1986}, rdb::month{3}, rdb::day{17} };
    rapidcsv::RowIndex<rdb::year_month_day> riDate(tDate);
    strs = viewdoc1.GetSortRow<std::string>(riDate);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "29.000001");
    unittest::ExpectEqual(std::string, strs.at(4), "133171200");
    unittest::ExpectEqual(std::string, strs.at(5), "0.069131");

    ////  Filter + Sort
    const rapidcsv::SortParams<rdb::year_month_day> spD(0, rapidcsv::e_SortOrder::DESCEND);
    rapidcsv::FilterSortDocument<isYear2016, rdb::year_month_day> viewdoc2(doc, spD);

    strs = viewdoc2.GetViewRow<std::string>(0);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.959999");
    unittest::ExpectEqual(std::string, strs.at(4), "25579900");
    unittest::ExpectEqual(std::string, strs.at(5), "61.765546");

    strs = viewdoc2.GetViewRow<std::string>("2016-12-29");
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.860001");
    unittest::ExpectEqual(std::string, strs.at(4), "10250600");
    unittest::ExpectEqual(std::string, strs.at(5), "62.520968");

    rdb::year_month_day tDate2{rdb::year{2016}, rdb::month{12}, rdb::day{28} };
    rapidcsv::RowIndex<rdb::year_month_day> riDate2(tDate2);
    strs = viewdoc2.GetSortRow<std::string>(riDate2);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "63.400002");
    unittest::ExpectEqual(std::string, strs.at(4), "14653300");
    unittest::ExpectEqual(std::string, strs.at(5), "62.610426");

    std::string errMsg = "include/rapidcsv/view.h:450 rowKey not found in 'sortedKeyMap'. FilterSortDocument::GetSortedRow() : " + mapErr;
    rdb::year_month_day tDate3{rdb::year{2015}, rdb::month{12}, rdb::day{28} };
    rapidcsv::RowIndex<rdb::year_month_day> riDate3(tDate3);
    ExpectExceptionMsg(viewdoc2.GetSortRow<std::string>(riDate3), std::out_of_range, errMsg );
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
