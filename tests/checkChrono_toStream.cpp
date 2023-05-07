#include <sstream>
#include <string>
#include <cassert>

//  ./manualBuild.sh tests checkChrono_toStream -DUSE_CHRONO=1

#if  USE_CHRONO == 1
  #include <chrono>
  namespace datelib = std::chrono;
#else
  #include <date/date.h>
  namespace datelib = date;
#endif

std::string FromYMD(const datelib::year_month_day& pYmd, std::string::value_type* fmt)
{
    std::ostringstream oss;

    datelib::to_stream(oss, fmt, pYmd);
    return oss.str();
}

int main()
{
    std::string dateExp("2016-12-11");
    datelib::year_month_day ymd = datelib::year(2016)/12/11;
    std::string dateCon = FromYMD(ymd, const_cast<char*> ("%F"));
    assert(dateExp == dateCon);
}
