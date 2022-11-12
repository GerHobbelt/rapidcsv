Rapidcsv
========

| **Linux** | **Mac** | **Windows** |
|-----------|---------|-------------|
| [![Linux](https://github.com/panchaBhuta/rapidcsv_CT/workflows/Linux/badge.svg)](https://github.com/panchaBhuta/rapidcsv_CT/actions?query=workflow%3ALinux) | [![macOS](https://github.com/panchaBhuta/rapidcsv_CT/workflows/macOS/badge.svg)](https://github.com/panchaBhuta/rapidcsv_CT/actions?query=workflow%3AmacOS) | [![Windows](https://github.com/panchaBhuta/rapidcsv_CT/workflows/Windows/badge.svg)](https://github.com/panchaBhuta/rapidcsv_CT/actions?query=workflow%3AWindows) |

Rapidcsv is an easy-to-use C++ CSV parser library. It supports C++11 (and
later), is header-only and comes with a basic test suite.

The [library on upstream repo](https://github.com/d99kris/rapidcsv) was showcased in the book
[C++20 for Programmers](https://deitel.com/c-plus-plus-20-for-programmers/).

Differences with the [upstream repo](https://github.com/d99kris/rapidcsv)
-------------------------------------------------------------------------
### Removed header `<typeinfo>` and call to function `typeid(...)`
Inclusion of header `<typeinfo>` in template based library is a bit anticlimatic, particularly when
C++ template supports specialization based on types. Even though the `typeid(...)` function used in
branch conditions of class-member-functions `void Converter::ToStr(const T& pVal, std::string& pStr) const` and
`void Converter::ToVal(const std::string& pStr, T& pVal) const` in upstream
[rapidcsv.h](https://github.com/d99kris/rapidcsv/blob/eaf04fec409fdba3b263c7a52e5dc24e5bf9d4f3/src/rapidcsv.h)
gets evaluated and pruned out during compilation, and binary code wouldn't have the branching-conditions.
Using template-specialization, is a better alternative to `typeid(...)` where possible.
Class-static-functions `T ConverterToVal<T,USE_NUMERIC_LOCALE=1, USE_NAN=0>::ToVal(const std::string & pStr)` and
`std::string ConverterToStr<T,USE_NUMERIC_LOCALE=0>::ToStr(const T & pVal)` uses template-specialization
based on type.

### Removed `struct ConverterParams`
`struct ConverterParams` functionality is provided through template parameters
`int USE_NUMERIC_LOCALE, int USE_NAN` and static-function `T struct NaNaccess<T,USE_NAN=1>::getNaN()`.

When `USE_NUMERIC_LOCALE=0` would trigger conversions using string-stream. This is default for `ConverterToStr`
as string-stream preserves the decimal portion of floating-point-numbers. Using numeric-local functions
for floating-points from value to string effects the precision of the number in string form.

When `USE_NUMERIC_LOCALE=1` would trigger conversions using numeric-local. This is default for `ConverterToVal`.
For e.g `int` -> function `int std::stoi(const std::string&)` is used. 

### Removed `class no_converter : public std::exception`
For types which are not supported will generate compile-time error. This approach is better then getting
a run-time exception.

### Minor Performance gains
Inside some of the for-loops in upstream-repo, Conditional branching based on result of `std::distance(...)`
has been eliminated by shifting-ahead the starting iterator of the for-loop.

### Code changes if anyone wants to upgrade from the [upstream repo](https://github.com/d99kris/rapidcsv)
Where `struct ConverterParams` is passed as a parameter to `class Document` constructor, delete this parameter.
Instead pass as template parameters for `<typename T, int USE_NUMERIC_LOCALE, int USE_NAN>` for all Getter functions
and pass as template parameter for `<typename T, int USE_NUMERIC_LOCALE>` for all Setter functions of `Document` objects.

If any converter functions i.e `void Converter<T>::ToStr(const T& pVal, std::string& pStr) const` OR 
`void Converter<T>::ToVal(const std::string& pStr, T& pVal) const` were overloaded, then change the signature to static functions
`static T ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal(const std::string & pStr)` OR
`static std::string ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr(const T & pVal)`

As unsupported types will result in compilation error, exception for `class no_converter` needs to be removed from your code.

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a
vector of floats.

[colhdr.csv](examples/colhdr.csv) content:
```
    Open,High,Low,Close,Volume,Adj Close
    64.529999,64.800003,64.139999,64.620003,21705200,64.620003
    64.419998,64.730003,64.190002,64.620003,20235200,64.620003
    64.330002,64.389999,64.050003,64.360001,19259700,64.360001
    64.610001,64.949997,64.449997,64.489998,19384900,64.489998
    64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex001.cpp](examples/ex001.cpp) content:
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
Rapidcsv is implemented using C++11 with the intention of being portable. It's
been tested on:
- macOS Big Sur 11.0
- Ubuntu 20.04 LTS
- Windows 10 / Visual Studio 2019

Installation
============
Simply copy
[include/rapidcsv/rapidcsv.h](https://raw.githubusercontent.com/panchaBhuta/rapidcsv_CT/blob/master/include/rapidcsv/rapidcsv.h)
[include/rapidcsv/converter.h](https://raw.githubusercontent.com/panchaBhuta/rapidcsv_CT/blob/master/include/rapidcsv/converter.h)
to your project/include directory and include it. 

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
All standard conversions are relatively straight-forward, with the
exception of `char` for which rapidcsv interprets the cell's (first) byte
as a character. The following example illustrates the supported data types.

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
One may override conversion routines (or add new ones) by implementing ToVal()
and/or ToStr(). Below is an example overriding int conversion, to instead provide
two decimal fixed-point numbers. Also see 
[tests/test035.cpp](https://github.com/panchaBhuta/rapidcsv_CT/blob/master/tests/test035.cpp)
for a test overriding ToVal() and ToStr().

[ex008.cpp](examples/ex008.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include <cmath>

    #include <rapidcsv/rapidcsv.h>

    namespace rapidcsv
    {
      template<>
      struct ConverterToVal<int,1,0>
      {
        static int ToVal(const std::string& pStr)
        {
          return static_cast<int>(roundf(100.0f * std::stof(pStr)));
        }
      };
    }

    int main()
    {
      rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

      std::vector<int> close = doc.GetColumn<int>("Close");
      std::cout << "close[0]  = " << close[0] << std::endl;
      std::cout << "close[1]  = " << close[1] << std::endl;
    }
```

Custom Data Type Conversion Per Call
------------------------------------
It is also possible to override conversions on a per-call basis, enabling more
flexibility. This is illustrated in the following example. Additional conversion
override usage can be found in the test 
[tests/test063.cpp](https://github.com/panchaBhuta/rapidcsv_CT/blob/master/tests/test063.cpp)

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
      std::cout << "fixpointfunc    = " << doc.GetCell<int>("Close", "2017-02-21", ConvFixPoint) << "\n";

      auto convFixLambda = [](const std::string& pStr) { return static_cast<int>(roundf(100.0f * stof(pStr))); };
      std::cout << "fixpointlambda  = " << doc.GetCell<int>("Close", "2017-02-21", convFixLambda) << "\n";

      std::cout << "mystruct        = " << doc.GetCell<MyStruct>("Close", "2017-02-21", ConvMyStruct).val << "\n";
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
If only data conversions is needed, then just include [rapidcsv/rapidcsv.h](include/rapidcsv/rapidcsv.h).
This is illustrated in the following example, where data conversion precision is checked
by implementing a complete conversion cycle i.e    data -> string -> data

[ex010.cpp](examples/ex010.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include <iomanip>
    #include <cmath>

    #include <rapidcsv/converter.h>

    namespace rapidcsv
    {
      template<>
      struct ConverterToVal<int,2,0>
      {
        static int ToVal(const std::string& pStr)
        {
          return static_cast<int>(roundf(100.0f * std::stof(pStr)));
        }
      };

      template<typename T>
      struct ConverterToStr<T,2>
      {
        static std::string ToStr(const typename
                                 std::enable_if<std::is_floating_point<T>::value, T>::type & val)
        {
          std::ostringstream out;
          out.precision( std::numeric_limits<T>::digits10 + 1 );
          out << std::fixed << val;
          return out.str();
        }
      };
    }

    template<typename T>
    void testType(const std::string& typeData, const T& orgT)
    {
      const std::string strT = rapidcsv::ConverterToStr<T>::ToStr(orgT);
      const T convT = rapidcsv::ConverterToVal<T>::ToVal(strT);

      std::cout << std::setprecision(25) << "org" << typeData << " = " << orgT << " ; str"
                << typeData << " = " << strT << " ; conv" << typeData << " = " << convT << std::endl; }

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

      std::cout << "using specialization" << std::endl;

      const long double orgLongDouble = 100.1234567890123456789012345L;
      const std::string strLongDouble = rapidcsv::ConverterToStr<long double,2>::ToStr(orgLongDouble);
      const int convInt = rapidcsv::ConverterToVal<int,2,0>::ToVal(strLongDouble);

      std::cout << "orgLongDouble = " << orgLongDouble << " ; strLongDouble = "
                << strLongDouble << " ; convInt = " << convInt << std::endl;

    }
```

Reading a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
-----------------------------------------------------------------------
By default rapidcsv throws an exception if one tries to access non-numeric
data as a numeric data type, as it basically propagates the underlying
conversion routines' exceptions to the calling application.

The reason for this is to ensure data correctness. If one wants to be able
to read data with invalid numbers as numeric data types, one can set
template param `USE_NAN=1` to configure the converter to default to a 'NAN' value.
The value is configurable (by Specialized Class implementation of struct NaNaccess\<T,USE_NAN\>,
refer `struct NaNaccess<T,1>`) and by default it's `std::numeric_limits<T>::signaling_NaN()` 
for float-point types, and 0 for integer types. Example:

```cpp
    rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(),
                           rapidcsv::SeparatorParams());
    ...
    int cellVAl = doc.GetCell("colName", rowIdx, rapidcsv::ConverterToVal<int,1,1>::ToVal);
```

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

CMake FetchContent
------------------
Rapidcsv may be included in a CMake project using FetchContent. Refer to the
[CMake FetchContent Example Project](examples/cmake-fetchcontent) and in
particular its [CMakeLists.txt](examples/cmake-fetchcontent/CMakeLists.txt).

Locale Independent Parsing
--------------------------
Rapidcsv uses locale-dependent conversion functions when parsing float-type values from string
by default ( `T ConverterToVal<T,USE_NUMERIC_LOCALE=1,0>::ToVal(const std::string & pStr)` ). 
It is possible to configure rapidcsv to use locale independent
parsing by setting template-parameter `USE_NUMERIC_LOCALE=0` in `doc.GetCell<float, 0, 0>`, see for example
[tests/test087.cpp](https://github.com/panchaBhuta/rapidcsv_CT/blob/master/tests/test087.cpp)

Rapidcsv uses string-stream when converting any type values to string
by default ( `std::string ConverterToStr<T,USE_NUMERIC_LOCALE=0>::ToStr(const T & pVal)` ). 
It is possible to configure rapidcsv to use locale-dependent for integer-types by 
setting template-parameter `USE_NUMERIC_LOCALE=1` in `doc.SetCell<float, 0>`.

WARNING :: With floating point types std::to_string may yield unexpected results as the number
           of significant digits in the returned string can be zero, for e.g: pVal = 1e-09.
           The return value may differ significantly from what std::cout prints by default.
           That's why this particular specialization is with USE_NUMERIC_LOCALE=-1  and not with 1.

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

Alternatives
============
There are many CSV parsers for C++, for example:
- [Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser)
- [Vince's CSV Parser](https://github.com/vincentlaucsb/csv-parser)

License
=======
Rapidcsv is distributed under the BSD 3-Clause license. See
[LICENSE](https://github.com/panchaBhuta/rapidcsv_CT/blob/master/LICENSE) file.

Contributions
=============
Bugs, PRs, etc are welcome on the GitHub project page
https://github.com/panchaBhuta/rapidcsv_CT/tree/master

Keywords
========
c++, c++11, csv parser, comma separated values, single header library.

