Rapidcsv (FilterSort)
=====================

| **Linux** | **Mac** | **Windows** |
|-----------|---------|-------------|
| [![Linux](https://github.com/panchaBhuta/rapidcsv_FilterSort/workflows/Linux/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions?query=workflow%3ALinux) | [![macOS](https://github.com/panchaBhuta/rapidcsv_FilterSort/workflows/macOS/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions?query=workflow%3AmacOS) | [![Windows](https://github.com/panchaBhuta/rapidcsv_FilterSort/workflows/Windows/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions?query=workflow%3AWindows) |

Rapidcsv is an easy-to-use C++ CSV parser library. It supports C++20 (and
later), is header-only and comes with a basic test suite.

The [library of upstream repo](https://github.com/d99kris/rapidcsv) was featured in book
[C++20 for Programmers](https://deitel.com/c-plus-plus-20-for-programmers/).

Differences with the [upstream repo](https://github.com/d99kris/rapidcsv)
-------------------------------------------------------------------------
## Refactored 'Convertor' template-class to template-typenames 'ConvertFromVal' and 'ConvertFromStr'

class-member-functions in upstream [d99kris/.../rapidcsv.h](https://github.com/d99kris/rapidcsv/blob/master/src/rapidcsv.h) has been refactored away ...

```cpp
void Converter<T>::ToStr(const T& pVal, std::string& pStr) const;
                    AND
void Converter<T>::ToVal(const std::string& pStr, T& pVal) const;
```

... instead call, Class-static-functions ...

```cpp
static std::string _ConvertFromVal<T, T2S_FORMAT>::ToStr(const T& pVal);
                    AND
static T _ConvertFromStr<T, S2T_FORMAT>::ToVal(const std::string& pStr);
```

### Removed `struct ConverterParams`
Instead use, typename(s) 'S2T_FORMAT' and 'T2S_FORMAT' which indicates the conversion algorithm for type-to-string and string-to-type repectively.

Default 'S2T_FORMAT' types are provided by `rapidcsv::S2T_DefaultFormat<T>::type`.
* for T = {integer-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; S2T_FORMAT = rapidcsv::S2T_Format_std_StoT
* for T = {floating-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; -> &nbsp; &nbsp; S2T_FORMAT = rapidcsv::S2T_Format_std_StoT
* for T = {string, char, bool} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; S2T_FORMAT =  rapidcsv::S2T_Format_WorkAround
* for T = {year_month_day} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; S2T_FORMAT =  rapidcsv::S2T_Format_StreamAsIs

_S2T_Format_StreamAsIs_ and _S2T_Format_StreamUseClassicLocale_ use _std::istringstream_ for string to T conversion. 

Default 'T2S_FORMAT' types are provided by `rapidcsv::T2S_DefaultFormat<T>::type`.
* for T = {integer-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; T2S_FORMAT = rapidcsv::T2S_Format_std_TtoS
* for T = {floating-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; -> &nbsp; &nbsp; T2S_FORMAT = rapidcsv::T2S_Format_StreamDecimalPrecision\<T>
* for T = {string, char, bool} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; T2S_FORMAT =  rapidcsv::T2S_Format_WorkAround
* for T = {year_month_day} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; T2S_FORMAT =  rapidcsv::T2S_Format_StreamAsIs

_T2S_Format_StreamAsIs_ , _T2S_Format_StreamUseClassicLocale_  and _T2S_Format_StreamDecimalPrecision\<T>_ use _std::ostringstream_ for T to string conversion.

One can also provide their own implementations of _T2S_FORMAT_ or _S2T_FORMAT_. For implentation details, refer **exConv001.cpp, ex008.cpp, ex009.cpp**.


### Removed `class no_converter : public std::exception`
For types which are not supported will generate compile-time error. This approach is better then getting a run-time exception.

## Performance gains
Removed header `<typeinfo>` and call to function `typeid(...)`.
In template based library usage of `typeid(...)` is unnecessary as we templates are working with _types_ itself. When C++ template supports specialization based on types, call to `typeid(...)` for run-time branching-conditions inside of class-member-functions takes a performance hit. Instead use of template class specializations and compile time code selection/elimination using  **constexpr** eliminates conditional branching at run-time, thereby improving performance.

Inside some of the for-loops in upstream-repo, Conditional branching based on result of `std::distance(...)` has been eliminated by shifting-ahead the starting iterator of the for-loop.

Function addresses are template-parameter instead of previously function-parameter. This helps the compiler make inline/direct call instead if pointer-indirection.

## View rows using Filter and Sort on columns
Filter on rows using template class `FilterDocument`. Similar to SQL `WHERE`.  
Sort rows using template class `SortDocument`. Similar to SQL `ORDER BY`.  
Both filter and sort is provided by template class `FilterSortDocument`.

For usage examples refer any of [tests/testView*.cpp](tests/testView001.cpp)

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a
vector of floats.

[colhdr.csv](examples/colhdr.csv) content:
```csv
Open,High,Low,Close,Volume,Adj Close
64.529999,64.800003,64.139999,64.620003,21705200,64.620003
64.419998,64.730003,64.190002,64.620003,20235200,64.620003
64.330002,64.389999,64.050003,64.360001,19259700,64.360001
64.610001,64.949997,64.449997,64.489998,19384900,64.489998
64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex001.cpp](examples/ex001.cpp) content:

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/ae8ab2a4d9e7bf38c67ebabdd2978f81739d766c/examples/ex001.cpp#L8-L19


```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/colhdr.csv");

  std::vector<float> col = doc.GetColumn<float>("Close");
  std::cout << "Read " << col.size() << " values." << std::endl;
}
```

Refer to section [More Examples](#more-examples) below for more examples.
The [tests](tests/) directory also contains many simple usage examples.

Supported Platforms
===================
Rapidcsv is implemented using C++20 with the intention of being portable. It's been tested on:
- Ubuntu 22.04 LTS
- macOS Ventura 13.0
- Windows 10 / Visual Studio 2019

Installation
============

CMake FetchContent
------------------
Rapidcsv may be included in a CMake project using FetchContent. Refer to the
[CMake FetchContent Example Project](examples/cmake-fetchcontent) and in
particular its [CMakeLists.txt](examples/cmake-fetchcontent/CMakeLists.txt).

CMake ExternalProject_Add
-------------------------
Rapidcsv may be included in a CMake project using ExternalProject_Add. Refer [here](https://github.com/panchaBhuta/cmakeExampleExternalProjectAdd)

or Simply copy ...
------------------

[include/rapidcsv/rapidcsv.h](include/rapidcsv/rapidcsv.h)  
[include/rapidcsv/converter.h](include/rapidcsv/converter.h)  
[include/rapidcsv/view.h](include/rapidcsv/view.h)

... to your project/include/rapidcsv/ directory and include it. OR alternatively you can call script ...  


[//]: # (TODO : add to 'vcpkg' and 'conan' packages)
[//]: # (Rapidcsv is also available via)
[//]: # ([vcpkg]\(https://vcpkg.io/en/packages.html\) and)
[//]: # ([conan]\(https://conan.io/center/rapidcsv\) )
[//]: # (package managers.)

More Examples
=============

Several of the following examples are also provided in the `examples/`
directory and can be executed directly under Linux and macOS. Example running
ex001.cpp:

```
./examples/ex001.cpp
```


Reading a File with Column and Row Headers
------------------------------------------
By default rapidcsv treats the first row as column headers, and the first
column is treated as data. This allows accessing columns using their labels,
but not rows or cells (only using indices). In order to treat the first column
as row headers one needs to use LabelParams and set pRowNameIdx to 0.

### Column and Row Headers
[colrowhdr.csv](examples/colrowhdr.csv) content:
```
Date,Open,High,Low,Close,Volume,Adj Close
2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003
2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003
2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001
2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998
2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex002.cpp](examples/ex002.cpp) content:
```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::vector<float> close = doc.GetRow<float>("2017-02-22");
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
```

### Row Headers Only
[rowhdr.csv](examples/rowhdr.csv) content:
```
2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003
2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003
2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001
2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998
2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex003.cpp](examples/ex003.cpp) content:
```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/rowhdr.csv", rapidcsv::LabelParams(-1, 0));

  std::vector<std::string> row = doc.GetRow<std::string>("2017-02-22");
  std::cout << "Read " << row.size() << " values." << std::endl;
}
```

### No Headers
[nohdr.csv](examples/nohdr.csv) content:
```
64.529999,64.800003,64.139999,64.620003,21705200,64.620003
64.419998,64.730003,64.190002,64.620003,20235200,64.620003
64.330002,64.389999,64.050003,64.360001,19259700,64.360001
64.610001,64.949997,64.449997,64.489998,19384900,64.489998
64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex004.cpp](examples/ex004.cpp) content:
```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/nohdr.csv", rapidcsv::LabelParams(-1, -1));

  std::vector<float> close = doc.GetColumn<float>(5);
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>(4, 2);
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
```

Reading a File with Custom Separator
------------------------------------
For reading of files with custom separator (i.e. not comma), one need to
specify the SeparatorParams argument. The following example reads a file using
semi-colon as separator.

[semi.csv](examples/semi.csv) content:
```
Date;Open;High;Low;Close;Volume;Adj Close
2017-02-24;64.529999;64.800003;64.139999;64.620003;21705200;64.620003
2017-02-23;64.419998;64.730003;64.190002;64.620003;20235200;64.620003
2017-02-22;64.330002;64.389999;64.050003;64.360001;19259700;64.360001
2017-02-21;64.610001;64.949997;64.449997;64.489998;19384900;64.489998
2017-02-17;64.470001;64.690002;64.300003;64.620003;21234600;64.620003
```

[ex005.cpp](examples/ex005.cpp) content:
```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/semi.csv", rapidcsv::LabelParams(0, 0),
                          rapidcsv::SeparatorParams(';'));

  std::vector<float> close = doc.GetColumn<float>("Close");
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
```

Supported Get/Set Data Types
----------------------------
The internal cell representation in the Document class is using std::string
and when other types are requested, standard conversion routines are used.
All standard conversions are relatively straight-forward, with the exception
of `char-types` or `bool`. For `char-types`, rapidcsv interprets the cell's (first)
byte as a character. For `bool`, the expected integer values are `0` or `1`.
The following example illustrates the supported data types.

[colrowhdr.csv](examples/colrowhdr.csv) content:
```
Date,Open,High,Low,Close,Volume,Adj Close
2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003
2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003
2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001
2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998
2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex006.cpp](examples/ex006.cpp) content:
```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::cout << doc.GetCell<std::string>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<int>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<long long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<unsigned>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<unsigned long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<unsigned long long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<float>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<double>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<long double>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<char>("Volume", "2017-02-22") << std::endl;
}

```

Global Custom Data Type Conversion
----------------------------------
One may override conversion routines (or add new ones) by implementing ToVal() and/or ToStr(). Below is an example of int conversion, to instead provide two decimal fixed-point numbers. Also see [tests/test035.cpp](tests/test035.cpp) for test using specialized instances of `ToVal()` and `ToStr()`.

[ex008.cpp](examples/ex008.cpp) content:
```cpp
#include <iostream>
#include <vector>
#include <cmath>

#include <rapidcsv/rapidcsv.h>

int ToVal(const std::string& pStr)
{
  return static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::vector<int> close = doc.GetColumn<int, &ToVal >("Close");
  std::cout << "close[0]  = " << close[0] << std::endl;
  std::cout << "close[1]  = " << close[1] << std::endl;
}
```

Custom Data Type Conversion Per Call
------------------------------------
It is also possible to override conversions on a per-call basis, enabling more
flexibility. This is illustrated in the following example. Additional conversion
override usage can be found in the test 
[tests/test063.cpp](tests/test063.cpp), 
[tests/test087.cpp](tests/test087.cpp) and
[tests/test092.cpp](tests/test092.cpp)

[ex009.cpp](examples/ex009.cpp) content:
```cpp
#include <iostream>
#include <vector>
#include <cmath>

#include <rapidcsv/rapidcsv.h>

int ConvFixPoint(const std::string& pStr)
{
  return static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

struct MyStruct
{
  int val = 0;
};

MyStruct ConvMyStruct(const std::string& pStr)
{
  MyStruct ms;
  ms.val = static_cast<int>(roundf(100.0f * std::stof(pStr)));
  return ms;
}

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::cout << "regular         = " << doc.GetCell<int>("Close", "2017-02-21") << "\n";
  std::cout << "fixpointfunc    = " << doc.GetCell<int, &ConvFixPoint>("Close", "2017-02-21") << "\n";

  /*
    Lambda-expressions are not allowed in unevaluated expressions, template arguments,
    alias declarations, typedef declarations, and anywhere in a function
    (or function template) declaration except the function body and the function's default
    arguments.  -- (until C++20)  -- perhaps when bumping up to C++2Z, the below 2 lines
    can be uncommented
  */
  // TODO C++2Z check if it complies
  //auto convFixLambda = [](const std::string& pStr) { return static_cast<int>(roundf(100.0f * stof(pStr))); };
  //std::cout << "fixpointlambda  = " << doc.GetCell<int, convFixLambda>("Close", "2017-02-21") << "\n";

  std::cout << "mystruct        = " << doc.GetCell<MyStruct, &ConvMyStruct>("Close", "2017-02-21").val << "\n";
}
```

Reading CSV Data from a Stream or String
----------------------------------------
In addition to specifying a filename, rapidcsv supports constructing a Document
from a stream and, indirectly through stringstream, from a string. File streams
used with rapidcsv should be opened in `std::ios::binary` mode to enable full
functionality. Here is a simple example reading CSV data from a string:

[ex007.cpp](examples/ex007.cpp) content:
```cpp
#include <iostream>
#include <vector>

#include <rapidcsv/rapidcsv.h>

int main()
{
  const std::string& csv =
    "Date,Open,High,Low,Close,Volume,Adj Close\n"
    "2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003\n"
    "2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003\n"
    "2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001\n"
    "2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998\n"
    "2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003\n"
  ;

  std::stringstream sstream(csv);
  rapidcsv::Document doc(sstream, rapidcsv::LabelParams(0, 0));

  std::vector<float> close = doc.GetColumn<float>("Close");
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
```

Data Conversion Precision
-------------------------
For floating-point numbers, the default decimal precision is determined by the return value of the call to `getDecimalPrecision<T>()`, this value is passed as template parameter to class-template `T2S_Format_StreamDecimalPrecision`.

```cpp
  template<c_floating_point T, int decimalPrecision = getDecimalPrecision<T>()>
  struct T2S_Format_StreamDecimalPrecision;
```

There is loss of data-precision at several places, for e.g:

```cpp
float pi_val = 3.1415926535f;                   // (1)
std::ostringstream oss;

oss << pi_val; std::string pi_str = oss.str();  // (2)

std::istringstream iss(pi_str);
float pi_read;

iss >> pi_read;                                 // (3)
```

The data-precision loss is happening at steps (1), (2) and (3).
The precision-loss at (1), where in rvalue-raw is not captured exactly
in lvalue-variable. This is system dependent.
The value in lvalue-variable(pi_val) should be written to text at a
higher precision digits for float(hence +3). As we need to eliminate the
precision loss happening at steps (2) and (3).
The repeatability or read-write accuracy can only be achieved by using
higher precision of that specified by precision accuracy for float.

Refer [test036.cpp](tests/test036.cpp) and [test053.cpp](tests/test053.cpp)
for data-loss in text -> type -> text, conversion cycle.
Refer [test091.cpp](tests/test091.cpp),  to see the effect of lowered Decimal-Precision when writing the data and then retriving it back.


One can also provide their own decimal precision parameter thru several ways. This is illustrated in the following example, where data conversion precision is checked by implementing a complete conversion cycle i.e    data -> string -> data  

Note: If only data conversions is needed, then just include [rapidcsv/converter.h](include/rapidcsv/converter.h).

[exConv001.cpp](examples/exConv001.cpp) content:
```cpp
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include <rapidcsv/converter.h>

struct format_num {};

template<rapidcsv::c_floating_point T>
struct format_oss {
    using stream_type = std::ostringstream;
    static void streamUpdate(std::ostringstream& oss)
    {
      std::cout << "call from -> static void format_oss<T>::streamUpdate(std::ostringstream& oss);" << std::endl;
      oss.precision( std::numeric_limits<T>::digits10 + 1 );
      oss << std::fixed;
    }
};

template<rapidcsv::c_floating_point T>
constexpr int getHigherDecimalPrecision()
{
  if constexpr (std::is_same_v<T, float>)
    return FLT_DIG+10;
  else
  if constexpr (std::is_same_v<T, double>)
    return DBL_DIG+10;
  else
  if constexpr (std::is_same_v<T, long double>)
    return LDBL_DIG+10;
}
template<rapidcsv::c_floating_point T>
using T2S_Format_hdp = rapidcsv::T2S_Format_StreamDecimalPrecision<T, getHigherDecimalPrecision<T>() >;

namespace rapidcsv
{
  template<>
  struct _ConvertFromStr<int, format_num>
  //struct ConvertFromStr<int, format_num>  #As ConvertFromStr is a typename connot override it
  {
    static int ToVal(const std::string& pStr)
    {
      return static_cast<int>(roundf(100.0f * std::stof(pStr)));
    }
  };


  template<c_floating_point T>
  struct _ConvertFromVal<T, format_num>
  //struct _ConvertFromVal<T, format_num >  #As ConvertFromVal is a typename connot override it
  {
    static std::string ToStr(const T& val)
    {
      std::cout << "call from -> static std::string _ConvertFromVal<T, format_num>::ToStr(const T& val);" << std::endl;
      std::ostringstream oss;
      oss.precision( std::numeric_limits<T>::digits10 + 1 );
      oss << std::fixed;
      oss << val;
      return oss.str();
    }
  };
}

template<typename T>
std::string ToStr(const T& val)
{
  std::cout << "call from -> std::string ToStr<T>(const T& val);" << std::endl;
  std::ostringstream oss;
  oss.precision( std::numeric_limits<T>::digits10 + 1 );
  oss << std::fixed;
  oss << val;
  return oss.str();
}

template<typename T>
void testType(const std::string& typeData, const T& orgT)
{
  const std::string strT = rapidcsv::ConvertFromVal<T>::ToStr(orgT);
  const T convT = rapidcsv::ConvertFromStr<T>::ToVal(strT);

  std::cout << std::setprecision(25) << "org" << typeData << " = " << orgT << " ; str"
            << typeData << " = " << strT << " ; conv" << typeData << " = " << convT << std::endl;
}

int main()
{
  testType<int>("Int", -100);
  testType<long>("Long", -10000000);
  testType<long long>("LongLong", -10000000000);
  testType<unsigned>("Unsigned", 100);
  testType<unsigned long>("UnsignedLong", 10000000);
  testType<unsigned long long>("UnsignedLongLong", 10000000000);
  testType<float>("Float", 313.1234567890123456789012345F);
  testType<double>("Double", 3462424.1234567890123456789012345);
  testType<long double>("LongDouble", 23453.1234567890123456789012345L);
  testType<char>("Char", 'G');
  testType<bool>("Bool", true);

  std::cout << "=============   using specialization" << std::endl;

  const long double orgLongDouble  = 100.1234567890123456789012345L;

  std::cout << "-------------   case 1 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;

  // call to -> static std::string _ConvertFromVal<T, format_num> >::ToStr(const T& val);
  const std::string strLongDouble1 = rapidcsv::ConvertFromVal<long double,
                                                              format_num
                                                            >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt1 = rapidcsv::ConvertFromStr<int,format_num>::ToVal(strLongDouble1);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble1 = "
            << strLongDouble1 << " ; convInt1 = " << convInt1 << std::endl;

  std::cout << "-------------   case 2 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;
  const std::string strLongDouble2 = rapidcsv::ConvertFromVal<long double,
                                                              format_oss<long double>
                                                             >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt2 = rapidcsv::ConvertFromStr<int,format_num>::ToVal(strLongDouble2);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble2 = "
            << strLongDouble2 << " ; convInt2 = " << convInt2 << std::endl;

  std::cout << "-------------   case 3 ::     long-double -> string -> int" << std::endl;
  std::cout << "                      step1.  long-double -> string" << std::endl;

  std::cout << "call to -> int getHigherDecimalPrecision<T>();" << std::endl;
  std::cout << "check the higher precision of 'strLongDouble3'" << std::endl;
  const std::string strLongDouble3 = rapidcsv::ConvertFromVal<long double,
                                                              T2S_Format_hdp<long double>
                                                             >::ToStr(orgLongDouble);
  std::cout << "                      step2.  string -> int" << std::endl;
  const int convInt3 = rapidcsv::ConvertFromStr<int,format_num>::ToVal(strLongDouble3);

  std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble3 = "
            << strLongDouble3 << " ; convInt3 = " << convInt3 << std::endl;
}
```

Locale Parsing Formats
----------------------
Rapidcsv_FilterSort uses std::sto`T`() conversion functions when parsing numeric-type values from string by default

```cpp
T ConvertFromStr<T, S2T_FORMAT = S2T_DefaultFormat<T>::type>::ToVal(const std::string& pStr);

// "S2T_DefaultFormat<T>::type" for numeric-types evaluates to  "rapidcsv::S2T_Format_std_StoT"
```

It is possible to configure rapidcsv_FilterSort to use locale dependent parsing by setting template-parameter `S2T_FORMAT=S2T_Format_StreamAsIs`, see for example [tests/test087.cpp](tests/test087.cpp)

```cpp
  doc.GetCell<float, &rapidcsv::ConvertFromStr<float, rapidcsv::S2T_Format_StreamAsIs>::ToVal>("A", "2");
```
... or configure rapidcsv_FilterSort to use either classic-locale parsing by setting template-parameter `S2T_FORMAT=S2T_Format_StreamUseClassicLocale`,

```cpp
  doc.GetCell<float, &rapidcsv::ConvertFromStr<float, rapidcsv::S2T_Format_StreamUseClassicLocale>::ToVal>("A", "2");
```

#### WARNING
With floating point types `std::to_string(...)` may yield unexpected results as the number of significant digits in the returned string can be zero, for e.g: pVal = 1e-09. The return value may differ significantly from what `std::cout` prints by default. That's why this particular specialization is disabled by default. In case if this is needed enable it by defining maco `ENABLE_STD_TtoS`

```cpp
template<c_floating_point T>
  struct _ConvertFromVal<T, T2S_Format_std_TtoS >;
```


Reading a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
----------------------------------------------------------------------
By default rapidcsv throws an exception if one tries to convert non-numeric text to numeric data type, as it basically propagates the underlying conversion routines' exceptions to the calling application.

The reason for this is to ensure data correctness. If one wants to be able
to read data with invalid numbers as numeric data types, one can set
template param `CONV_S2T` to configure the converter to default to a 'NAN' value on error.

For floating-types which supports `std::numeric_limits<T>::signaling_NaN()`, consider using template-typename `ConvertFromStr_fNaN<T, S2T_FORMAT>`, if _signaling_NaN()_ (NaN -> Not a Number) return-value is required, instead of an error thrown.

Alternatively, template-typename `ConvertFromStr_gNaN<T, S2T_FORMAT>` can be used for all numeric types(integer, floating, bool) to identify from return value either a valid-number or isNull or isNaN. The function `ConvertFromStr_gNaN<T, S2T_FORMAT>::ToVal` returns `std::variant<T, gNaN>`.

```cpp
enum gNaN { isNull=0, isNaN };

static std::variant<T, gNaN> ConvertFromStr_gNaN<T, S2T_FORMAT>::ToVal(const std::string& pStr);
```

Usage Example:

```cpp
rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(),
                       rapidcsv::SeparatorParams());
...
float cellVAl_f = doc.GetCell<float, ConvertFromStr_fNaN<float>::ToVal>("colName", rowIdx);
...
std::variant<int, gNaN> cellVAl_i = doc.GetCell<int, ConvertFromStr_gNaN<int>::ToVal>("colName", rowIdx);
...
std::variant<double, gNaN> cellVAl_d = doc.GetCell<double, ConvertFromStr_gNaN<double>::ToVal>("colName", rowIdx);
```

Refer [test048.cpp](tests/test048.cpp) and [test049.cpp](tests/test049.cpp) for sample code.


Check if a Column Exists
------------------------
Rapidcsv provides the methods GetColumnNames() and GetRowNames() to retrieve
the column and row names. To check whether a particular column name exists
one can for example do:

```cpp
rapidcsv::Document doc("file.csv");
std::vector<std::string> columnNames = doc.GetColumnNames();
bool columnExists =
  (std::find(columnNames.begin(), columnNames.end(), "A") != columnNames.end());
```

Handling Quoted Cells
---------------------
By default rapidcsv automatically dequotes quoted cells (i.e. removes the encapsulating
`"` characters from `"example quoted cell"`). This functionality may be disabled by
passing `pAutoQuote = false` in `SeparatorParams`, example:

```cpp
rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(),
                       rapidcsv::SeparatorParams(',' /* pSeparator */,
                                                 false /* pTrim */,
                                                 rapidcsv::sPlatformHasCR /* pHasCR */,
                                                 false /* pQuotedLinebreaks */,
                                                 false /* pAutoQuote */));
```

Skipping Empty and Comment Lines
--------------------------------
Rapidcsv reads all lines by default, but may be called to ignore comment lines
starting with a specific character, example:

```cpp
rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(),
                       rapidcsv::LineReaderParams(true /* pSkipCommentLines */,
                                                  '#' /* pCommentPrefix */));
```

Using LineReaderParams it is also possible to skip empty lines, example:

```cpp
rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(),
                       rapidcsv::LineReaderParams(false /* pSkipCommentLines */,
                                                  '#' /* pCommentPrefix */,
                                                  true /* pSkipEmptyLines */));
```

UTF-16 and UTF-8
----------------
Rapidcsv's preferred encoding for non-ASCII text is UTF-8. UTF-16 LE and
UTF-16 BE can be read and written by rapidcsv on systems where codecvt header
is present. Define HAS_CODECVT before including rapidcsv.h in order to enable
the functionality. Rapidcsv unit tests automatically detects the presence of
codecvt and sets HAS_CODECVT as needed, see [CMakeLists.txt](CMakeLists.txt)
for reference. When enabled, the UTF-16 encoding of any loaded file is
automatically detected.


API Documentation
=================
The following classes makes up the Rapidcsv interface:
 - [class rapidcsv::Document](doc/rapidcsv_Document.md)
 - [class rapidcsv::LabelParams](doc/rapidcsv_LabelParams.md)
 - [class rapidcsv::SeparatorParams](doc/rapidcsv_SeparatorParams.md)
 - [class rapidcsv::LineReaderParams](doc/rapidcsv_LineReaderParams.md)
 - [class rapidcsv::ConverterToVal< T, USE_NUMERIC_LOCALE, USE_NAN >](doc/rapidcsv_ConverterToVal.md)
 - [class rapidcsv::ConverterToStr< T, USE_NUMERIC_LOCALE >](doc/rapidcsv_ConverterToStr.md)
 - [class rapidcsv::NaNaccess< T, USE_NUMERIC_LOCALE >](doc/rapidcsv_NaNaccess.md)

Technical Details
=================
Rapidcsv uses cmake for its tests. Commands to build and execute the test suite:

    mkdir -p build && cd build && cmake -DRAPIDCSV_BUILD_TESTS=ON .. && make && ctest -C unit --output-on-failure && ctest -C perf --verbose ; cd -

Rapidcsv uses [doxygenmd](https://github.com/d99kris/doxygenmd) to generate
its Markdown API documentation:

    doxygenmd include doc

Rapidcsv uses Uncrustify to ensure consistent code formatting:

    uncrustify -c uncrustify.cfg --no-backup include/rapidcsv/rapidcsv.h

Rapidcsv can be installed using the following command:

    ./install.sh '/path/to/install/dir/'

Alternatives
============
There are many CSV parsers for C++, for example:
- [Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser)
- [Vince's CSV Parser](https://github.com/vincentlaucsb/csv-parser)

License
=======
Rapidcsv is distributed under the BSD 3-Clause license. See
[LICENSE](https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/master/LICENSE) file.

Contributions
=============
Bugs, PRs, etc are welcome on the GitHub project page
https://github.com/panchaBhuta/rapidcsv_FilterSort/tree/master

Keywords
========
c++, c++20, csv parser, comma separated values, single header library.

