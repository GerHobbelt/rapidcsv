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
  std::string mapErr;  // error message are different across various OS's
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

    strs = viewdoc.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.959999");
    unittest::ExpectEqual(std::string, strs.at(4), "25579900");
    unittest::ExpectEqual(std::string, strs.at(5), "61.765546");

    strs = viewdoc.GetViewRow_VecStr("2016-12-29");
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.860001");
    unittest::ExpectEqual(std::string, strs.at(4), "10250600");
    unittest::ExpectEqual(std::string, strs.at(5), "62.520968");

    /////   Sort
    const rapidcsv::SortParams<rdb::year_month_day> spA(0);
    rapidcsv::SortDocument<decltype(spA)> viewdoc1(doc, spA);   // `<decltype(spA)>` mandatory for clang

    strs = viewdoc1.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "25.499999");
    unittest::ExpectEqual(std::string, strs.at(4), "1031788800");
    unittest::ExpectEqual(std::string, strs.at(5), "0.065616");

    strs = viewdoc1.GetViewRow_VecStr("1986-03-14");
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "28.00");
    unittest::ExpectEqual(std::string, strs.at(4), "308160000");
    unittest::ExpectEqual(std::string, strs.at(5), "0.06796");

    rdb::year_month_day tDate{rdb::year{1986}, rdb::month{3}, rdb::day{17} };
    rapidcsv::SortKeyFactory<decltype(spA)>::t_sortKey riDate{tDate};

    using t_hloc = std::tuple<double,double,double,double,unsigned long,double>;
    //t_hloc hlocV_ac;
    auto hlocV_ac = viewdoc1.GetOrderedRow<double,double,double,double,unsigned long,double>(riDate);
    static_assert(std::is_same_v< t_hloc, decltype(hlocV_ac)>);
    unittest::ExpectEqual(double, std::get<0>(hlocV_ac), 29.000001);
    unittest::ExpectEqual(unsigned long, std::get<4>(hlocV_ac), 133171200);
    unittest::ExpectEqual(long, std::get<5>(hlocV_ac), 0.069131);

    strs = viewdoc1.GetOrderedRow_VecStr(riDate);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "29.000001");
    unittest::ExpectEqual(std::string, strs.at(4), "133171200");
    unittest::ExpectEqual(std::string, strs.at(5), "0.069131");

    ////  Filter + Sort
    const rapidcsv::SortParams<rdb::year_month_day, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isYear2016, decltype(spD)> viewdoc2(doc, spD);

    strs = viewdoc2.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.959999");
    unittest::ExpectEqual(std::string, strs.at(4), "25579900");
    unittest::ExpectEqual(std::string, strs.at(5), "61.765546");

    strs = viewdoc2.GetViewRow_VecStr("2016-12-29");
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "62.860001");
    unittest::ExpectEqual(std::string, strs.at(4), "10250600");
    unittest::ExpectEqual(std::string, strs.at(5), "62.520968");

    rdb::year_month_day tDate2{rdb::year{2016}, rdb::month{12}, rdb::day{28} };
    rapidcsv::SortKeyFactory<decltype(spD)>::t_sortKey riDate2(tDate2);

    hlocV_ac = viewdoc2.GetOrderedRow<double,double,double,double,unsigned long,double>(riDate2);
    unittest::ExpectEqual(double, std::get<0>(hlocV_ac), 63.400002);
    unittest::ExpectEqual(unsigned long, std::get<4>(hlocV_ac), 14653300);
    unittest::ExpectEqual(long, std::get<5>(hlocV_ac), 62.610426);

    strs = viewdoc2.GetOrderedRow_VecStr(riDate2);
    unittest::ExpectEqual(size_t, strs.size(), 6);
    unittest::ExpectEqual(std::string, strs.at(0), "63.400002");
    unittest::ExpectEqual(std::string, strs.at(4), "14653300");
    unittest::ExpectEqual(std::string, strs.at(5), "62.610426");

    std::string errMsgPre_v = (std::filesystem::path("include/rapidcsv/view.h")).make_preferred().string();
    std::string errMsg = errMsgPre_v + " : FilterSortDocument::GetOrderedRow_VecStr() : rowKey not found in 'sortedKeyMap'. For pRowKey=[2015-12-28] : ERROR: " + mapErr;
    rdb::year_month_day tDate3{rdb::year{2015}, rdb::month{12}, rdb::day{28} };
    rapidcsv::SortKeyFactory<decltype(spD)>::t_sortKey riDate3(tDate3);
    ExpectExceptionMsg(viewdoc2.GetOrderedRow_VecStr(riDate3), std::out_of_range, errMsg );
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
