// testViewYmd005.cpp - read row value

#include <rapidcsv/view.h>
#include "unittest.h"

namespace rdb = std::chrono;


bool isYear2016(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : data-zero-index is label
  return ( converter::ConvertFromStr< rdb::year_month_day >::ToVal(dataRow.at(0)).year() == rdb::year(2016) );
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

    rapidcsv::Document doc("../../tests/msft.csv", rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT, rapidcsv::FlgRowName::RN_MISSING));

    /////  Filter
    rapidcsv::FilterDocument<isYear2016> viewdoc(doc);

    strs = viewdoc.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "62.959999");
    unittest::ExpectEqual(std::string, strs.at(5), "25579900");
    unittest::ExpectEqual(std::string, strs.at(6), "61.765546");

/*
    strs = viewdoc.GetViewRow_VecStr("2016-12-29");     // date-column is not used as label
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "62.860001");
    unittest::ExpectEqual(std::string, strs.at(5), "10250600");
    unittest::ExpectEqual(std::string, strs.at(6), "62.520968");
*/

    /////   Sort
    const rapidcsv::SortParams<rdb::year_month_day> spA(0);
    rapidcsv::SortDocument<decltype(spA)> viewdoc1(doc, spA);   // `<decltype(spA)>` mandatory for clang

    strs = viewdoc1.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "25.499999");
    unittest::ExpectEqual(std::string, strs.at(5), "1031788800");
    unittest::ExpectEqual(std::string, strs.at(6), "0.065616");

/*
    strs = viewdoc1.GetViewRow_VecStr("1986-03-14");     // date-column is not used as label
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "28.00");
    unittest::ExpectEqual(std::string, strs.at(5), "308160000");
    unittest::ExpectEqual(std::string, strs.at(6), "0.06796");
*/

    rdb::year_month_day tDate{rdb::year{1986}, rdb::month{3}, rdb::day{17} };
    rapidcsv::SortKeyFactory<decltype(spA)>::t_sortKey riDate{tDate};

    using t_hloc = std::tuple<rdb::year_month_day, double,double,double,double,unsigned long,double>;
    t_hloc hlocV_ac = viewdoc1.GetRow_IndexKey<rdb::year_month_day, double,double,double,double,unsigned long,double>(riDate);
    unittest::ExpectEqual(double,        std::get<1>(hlocV_ac), 29.000001);
    unittest::ExpectEqual(unsigned long, std::get<5>(hlocV_ac), 133171200);
    unittest::ExpectEqual(double,        std::get<6>(hlocV_ac), 0.069131);

    strs = viewdoc1.GetRow_IndexKey_VecStr(riDate);
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "29.000001");
    unittest::ExpectEqual(std::string, strs.at(5), "133171200");
    unittest::ExpectEqual(std::string, strs.at(6), "0.069131");

    ////  Filter + Sort
    const rapidcsv::SortParams<rdb::year_month_day, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isYear2016, decltype(spD)> viewdoc2(doc, spD);

    strs = viewdoc2.GetViewRow_VecStr(0);
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "62.959999");
    unittest::ExpectEqual(std::string, strs.at(5), "25579900");
    unittest::ExpectEqual(std::string, strs.at(6), "61.765546");

/*
    strs = viewdoc2.GetViewRow_VecStr("2016-12-29");     // date-column is not used as label
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "62.860001");
    unittest::ExpectEqual(std::string, strs.at(5), "10250600");
    unittest::ExpectEqual(std::string, strs.at(6), "62.520968");
*/

    rdb::year_month_day tDate2{rdb::year{2016}, rdb::month{12}, rdb::day{28} };
    rapidcsv::SortKeyFactory<decltype(spD)>::t_sortKey riDate2(tDate2);

    hlocV_ac = viewdoc2.GetRow_IndexKey<rdb::year_month_day, double,double,double,double,unsigned long,double>(riDate2);
    unittest::ExpectEqual(double,        std::get<1>(hlocV_ac), 63.400002);
    unittest::ExpectEqual(unsigned long, std::get<5>(hlocV_ac), 14653300);
    unittest::ExpectEqual(double,        std::get<6>(hlocV_ac), 62.610426);

    strs = viewdoc2.GetRow_IndexKey_VecStr(riDate2);
    unittest::ExpectEqual(size_t, strs.size(), 7);
    unittest::ExpectEqual(std::string, strs.at(1), "63.400002");
    unittest::ExpectEqual(std::string, strs.at(5), "14653300");
    unittest::ExpectEqual(std::string, strs.at(6), "62.610426");

    //std::string errMsgPre_v = (std::filesystem::path("include/rapidcsv/view.h")).make_preferred().string();
    //std::string errMsg = errMsgPre_v + " : FilterSortDocument::GetRow_IndexKey_VecStr() : rowKey not found in 'sortedKeyMap'. For pRowKey=[2015-12-28] : ERROR: " + mapErr;
    std::string errMsg = "rapidcsv::FilterSortDocument::GetRow_IndexKey_VecStr(...) : rowKey not found in 'sortedKeyMap'";
    rdb::year_month_day tDate3{rdb::year{2015}, rdb::month{12}, rdb::day{28} };
    rapidcsv::SortKeyFactory<decltype(spD)>::t_sortKey riDate3(tDate3);
    ExpectExceptionMsg(viewdoc2.GetRow_IndexKey_VecStr(riDate3), std::out_of_range, errMsg );
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
