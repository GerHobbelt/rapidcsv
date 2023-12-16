Rapidcsv ( with Filter and Sort )
=================================

[//]: # (<img src="images/RefactoringInProgress.jpg" width="500">)

[//]: # (## Documentation is **NOT** updated to latest changes.)

Check [here](doc/UpstreamRepoChanges.md) for differences with [upstream repo](https://github.com/d99kris/rapidcsv).


| **Linux** | **Mac** | **Windows** |
|-----------|---------|-------------|
| [![Linux](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions/workflows/linux.yml/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions/workflows/linux.yml) | [![macOS](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions/workflows/macos.yml/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions/workflows/macos.yml) | [![Windows](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions/workflows/windows.yml/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions/workflows/windows.yml) |

Rapidcsv is an easy-to-use C++ CSV parser library. It supports C++20 (and
later), is header-only and comes with a basic test suite.

The [library of upstream repo](https://github.com/d99kris/rapidcsv) was featured in book
[C++20 for Programmers](https://deitel.com/c-plus-plus-20-for-programmers/).

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a vector of floats.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/colhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex001.cpp#L1-L12



Refer to section [More Examples](#more-examples) below for more examples.
The [tests](tests/) directory also contains many simple usage examples.

Supported Platforms
===================
Rapidcsv is implemented using C++20 with the intention of being portable. It's been tested on:

|  🖥️ OS ➡️ <br> 🤖 Compiler ⬇️ | **Ubuntu 22.04** | **macOS-(12,13)** | **macOS-11** | **Windows 10<br>VS 17 - 2022** | **Windows 10<br>VS 16 - 2019** |
|------------|------------------|--------------|--------------|-------------------------------|-------------------------------|
| **g++ (11,12)** | ✅ (default:11) | ✅ | ✅ | - | - |
| **g++ 13** | ✅ | ✅ | NA | - | - |
| **clang++ (**<span style="color:grey">13\#,14\#</span>**)** | ❌ | - | - | - | - |
| **clang++ 15** | ✅ | - | - | - | - |
| **AppleClang 14** | NA | ✅ (default) | NA | NA | NA |
| **msvc 19** | NA | NA | NA | ✅ (default) | NA |
| **clangCL 12** | - | - | - | - | ✅ |
| **clangCL 16** | - | - | - | ✅ | - |

<span style="color:grey">clang++ 13\#</span> : Last successful run with [Clang 13.0.1](https://github.com/panchaBhuta/converter/actions/runs/6524732682/job/17716666880) and `OS-id-version=Linux-6.2.0-1012-azure`. **\<chorno>** headers stopped compiling in Newer Ubuntu-image OS-version. Refer [ubuntu-latest runners have an incompatible combination of clang and libstdc++](https://github.com/actions/runner-images/issues/8659).

<span style="color:grey">clang++ 14\#</span> : Last successful run with [Clang 14.0.0](https://github.com/panchaBhuta/converter/actions/runs/6524732682/job/17716666951) and `OS-id-version=Linux-6.2.0-1012-azure`. **\<chorno>** headers stopped compiling in Newer Ubuntu-image OS-version. Refer [ubuntu-latest runners have an incompatible combination of clang and libstdc++](https://github.com/actions/runner-images/issues/8659).


[//]:  ❌


Installation
============

CMake FetchContent
------------------
Rapidcsv may be included in a CMake project using FetchContent. Refer to the
[CMake FetchContent Example Project](examples/cmake-fetchcontent) and in
particular its [CMakeLists.txt](examples/cmake-fetchcontent/CMakeLists.txt).

CMake ExternalProject_Add
-------------------------
Rapidcsv may be included in a CMake project using _ExternalProject_Add_. Refer [here](https://github.com/panchaBhuta/cmakeExampleExternalProjectAdd).

[//]: # (or Simply copy ...)
[//]: # (------------------)

[//]: # ([include/rapidcsv/rapidcsv.h]\(include/rapidcsv/rapidcsv.h\)  )
[//]: # ([include/rapidcsv/converter.h]\(include/rapidcsv/converter.h\)  )
[//]: # ([include/rapidcsv/view.h]\(include/rapidcsv/view.h\)  )

[//]: # (... to your project/include/rapidcsv/ directory and include it.)


[//]: # (TODO : add to 'vcpkg' and 'conan' packages)
[//]: # (Rapidcsv is also available via)
[//]: # ([vcpkg]\(https://vcpkg.io/en/packages.html\) and)
[//]: # ([conan]\(https://conan.io/center/rapidcsv\) )
[//]: # (package managers.)

More Examples
=============

Several of the following examples are also provided in the [examples/](examples/) directory.

These can be individually compiled using [manualBuild.sh](./manualBuild.sh) and executed in Linux and macOS. Example running ex001.cpp:

```sh
./manualBuild.sh examples ex001
./build-tmp/ex001

./manualBuild.sh tests test001
./build-tmp/test001
```



Reading a File with Column and Row Headers
------------------------------------------
By default rapidcsv treats the first row as column headers, and the first
column is treated as data. This allows accessing columns using their labels,
but not rows or cells (only using indices). In order to treat the first column
as row headers one needs to use LabelParams and set pRowNameIdx to 0.

### Column and Row Headers
csv file with both column and row headers

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/colrowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex002.cpp#L1-L16


### Row Headers Only
csv file with row header; i.e. no column header<br>

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/rowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex003.cpp#L1-L13


### No Headers
csv file with no column and row headers

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/nohdr.csv#L1-L5


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex004.cpp#L1-L15


Reading a File with Custom Separator
------------------------------------
For reading of files with custom separator (i.e. not comma), one need to
specify the SeparatorParams argument. The following example reads a file using
semi-colon as separator.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/semi.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex005.cpp#L1-L16



Supported Get/Set Data Types
----------------------------
The internal cell representation in the Document class is using std::string
and when other types are requested, [converter](https://github.com/panchaBhuta/converter/tree/main#supported-data-types-for-conversion) routines are used.
The following example illustrates the supported data types.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/colrowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex006.cpp#L1-L21


### **std::chrono::year_month_day** is the `Date` type used
Refer [Converter:Date types](https://github.com/panchaBhuta/converter#date-types) for more details. The default date format is _"%F"_ (i.e "%Y-%m-%d"). For configuring a different date format refer [exConv001.cpp](examples/exConv001.cpp) and [test001.cpp](tests/test001.cpp)


For string to date, the default conversion formater is `S2T_Format_StreamYMD`. With format "%F" -> "%Y-%m-%d".

```cpp
  template<>
  struct S2T_DefaultFormat< datelib::year_month_day, void >
  {
    using type = S2T_Format_StreamYMD< defYMDfmt >; // %F -> "%Y-%m-%d"
  };
```

For date to staring, the default conversion formater is `T2S_Format_StreamYMD`. With format "%F" -> "%Y-%m-%d".

```cpp
  template<>
  struct T2S_DefaultFormat< datelib::year_month_day, void >
  {
    using type = T2S_Format_StreamYMD< defYMDfmt >; // %F -> "%Y-%m-%d"
  };
```


Global Custom Data Type Conversion
----------------------------------
One may override conversion routines (or add new ones) by implementing converter function ToVal() or ToStr(). Below is an example of int conversion, to instead provide two decimal fixed-point numbers. Also see [tests/test035.cpp](tests/test035.cpp).


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex008.cpp#L1-L19


Custom Data Type Conversion Per Call
------------------------------------
It is also possible to override conversions on a per-call basis, enabling more flexibility. This is illustrated in the following example. Additional conversion override usage can be found in the tests 
[tests/test063.cpp](tests/test063.cpp), 
[tests/test087.cpp](tests/test087.cpp) and
[tests/test092.cpp](tests/test092.cpp)

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex009.cpp#L1-L43


Reading CSV Data from a Stream or String
----------------------------------------
In addition to specifying a filename, rapidcsv supports constructing a Document
from a stream and, indirectly through stringstream, from a string. File streams
used with rapidcsv should be opened in `std::ios::binary` mode to enable full
functionality. Here is a simple example reading CSV data from a string:

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/ex007.cpp#L1-L25

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


Data Conversion Precision
-------------------------
This is handled by the [converter](https://github.com/panchaBhuta/converter) library, refer [here](https://github.com/panchaBhuta/converter#data-conversion-precision) for more details.<br>
Refer [test036.cpp](tests/test036.cpp) and [test053.cpp](tests/test053.cpp)
for data-loss in **text -> type -> text**, conversion cycle.
Refer [test091.cpp](tests/test091.cpp),  to see the effect of lowered Decimal-Precision when writing the data and then retriving it back.


One can also provide their own decimal precision parameter thru several ways. This is illustrated in the example [exConv001.cpp](examples/exConv001.cpp), where data conversion precision is checked by implementing a complete conversion cycle i.e    **data -> string -> data** .<br>

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/76c1a563c298fcacfb91bcb60b65e8bc63db8556/examples/exConv001.cpp#L1-L166


Locale Parsing Formats : String-to-T (i.e S2T)
----------------------------------------------
**rapidcsv_FilterSort** uses [converter library](https://github.com/panchaBhuta/converter#locale-parsing-formats--string-to-t-ie-s2t) conversion functions when parsing numeric-type values from string. By default, during parsing 'locale' is not used.

```c++
    template< typename T_C >
    typename converter::t_S2Tconv_c<T_C>::return_type
    GetCell(const c_sizet_or_string auto& pColumnNameIdx,
            const c_sizet_or_string auto& pRowNameIdx) const
```

`converter::t_S2Tconv_c<T_C>` maps to default-conversion functions that are independent of any locale.

It is possible to configure **rapidcsv_FilterSort** to use locale dependent parsing by setting template-parameter `T_C` of function `Document::GetCell<T_C>(...)`, see example [tests/test087.cpp](tests/test087.cpp).

```c++
template<typename T>
using convertS2T_stream =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_StreamAsIs<T,
                                                               converter::FailureS2Tprocess::THROW_ERROR,
                                                               char>
                             >;
...
  doc.GetCell<convertS2T_stream<float>>("A", "2");
```


... or configure **rapidcsv_FilterSort** to use classic-locale parsing by setting template-parameter `T_C` of function `Document::GetCell<T_C>(...)`, see example [tests/test087.cpp](tests/test087.cpp).

```c++
template<typename T>
using convertS2T_streamClassic =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_StreamUseClassicLocale< T,
                                                                            converter::FailureS2Tprocess::THROW_ERROR,
                                                                            char>
                             >;
...
  doc.GetCell<convertS2T_streamClassic<float>>("A", "2");
```


... or configure **rapidcsv_FilterSort** to any of `std::stoi`, `std::stoll`, `std::sto*` for numeric-types. These functions uses current locale for formatting. Set template-parameter `T_C` of function `Document::GetCell<T_C>(...)`, see example [tests/test087.cpp](tests/test087.cpp).

```c++
template<typename T>
using convertS2T_StoT =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_std_StoT<T,
                                                             converter::FailureS2Tprocess::THROW_ERROR>
                             >;
...
  doc.GetCell<convertS2T_StoT<float>>("A", "2");
```


... or specify a specific locale, see example [tests/test087.cpp](tests/test087.cpp) and [tests/test092.cpp](tests/test092.cpp)

```c++
static constexpr char loc[] = "de_DE.UTF-8"; // uses comma (,) as decimal separator

template<typename T>
using deLocal_iss = converter::S2T_Format_StreamUserLocale<T, converter::FailureS2Tprocess::THROW_ERROR, char, loc>;

template<typename T>
using convertS2T_userLocale =
    converter::ConvertFromStr<T, deLocal_iss<T> >;
...
  doc.GetCell<convertS2T_userLocale<float>>("A", "2");
```

Locale Parsing Formats : T-to-String (i.e T2S)
----------------------------------------------
**rapidcsv_FilterSort** uses [converter library](https://github.com/panchaBhuta/converter#locale-parsing-formats--t-to-string-ie-t2s) conversion functions when parsing numeric-type values to string. By default, during parsing 'locale' is not used.

```c++
    template< typename T_C >
    void SetCell(const c_sizet_or_string auto& pColumnNameIdx,
                 const c_sizet_or_string auto& pRowNameIdx,
                 const typename converter::t_T2Sconv_c<T_C>::input_type& pCell)
```

`converter::t_T2Sconv_c<T_C>` maps to default-conversion functions that are independent of any locale.

It is possible to configure **rapidcsv_FilterSort** to use locale dependent parsing by setting template-parameter `T_C` of function `Document::SetCell<T_C>(...)`.

```c++
template<typename T>
using convertT2S_stream =
    converter::ConvertFromVal<T,
                              converter::T2S_Format_StreamAsIs<char>
                             >;
...
  doc.SetCell<convertT2S_stream<float>>("A", "2", 1.2f);
```


... or configure **rapidcsv_FilterSort** to use classic-locale parsing by setting template-parameter `T_C` of function `Document::SetCell<T_C>(...)`.

```c++
template<typename T>
using convertT2S_streamClassic =
    converter::ConvertFromVal<T,
                              converter::T2S_Format_StreamUseClassicLocale<char>
                             >;
...
  doc.SetCell<convertT2S_streamClassic<float>>("A", "2", 1.2f);
```


... or configure **rapidcsv_FilterSort** to use `std::to_string(val)` for integer-types. These functions uses current locale for formatting. Set template-parameter `T_C` of function `Document::SetCell<T_C>(...)`.

```c++
template<c_integer_type T>
using convertT2S_TtoS =
    converter::ConvertFromVal<T,
                              converter::T2S_Format_std_TtoS
                             >;
...
  doc.SetCell<convertT2S_TtoS<long>>("A", "2", 1234567L);
```


... or specify a specific locale

```c++
static constexpr char loc[] = "de_DE.UTF-8"; // uses comma (,) as decimal separator

template<typename T>
using deLocal_oss = converter::T2S_Format_StreamUserLocale<char, loc>;

template<typename T>
using convertT2S_userLocale =
    converter::ConvertFromVal<T, deLocal_oss<T> >;
...
  doc.SetCell<convertT2S_userLocale<float>>("A", "2", 1.234f);
```


... or override the default precision for floating-point types , with user defined precision. Check example [tests/test091.cpp](tests/test091.cpp)

```c++
template<converter::c_floating_point T>
constexpr int getLowerDecimalPrecision()
{
  if constexpr (std::is_same_v<T, float>)
    return FLT_DIG;
  else
  if constexpr (std::is_same_v<T, double>)
    return DBL_DIG;
  else
  if constexpr (std::is_same_v<T, long double>)
    return LDBL_DIG;
}
template<converter::c_floating_point T>
using T2S_Format_ldp = converter::T2S_Format_StreamDecimalPrecision<T, getLowerDecimalPrecision<T>() >;

template<converter::c_floating_point T>
using ConvertFromVal_lDP = converter::ConvertFromVal<T, T2S_Format_ldp<T> >;
...
  doc.SetCell< ConvertFromVal_lDP<long double> >("A", "ld", 3.14159265358979323846L);
```

... or even combine two formats using `converter::T2S_Format_floating_StreamCombine<...>`

```c++
static constexpr char loc[] = "de_DE.UTF-8"; // uses comma (,) as decimal separator

template<typename T>
using deLocal_oss = converter::T2S_Format_StreamUserLocale<char, loc>;

template<converter::c_floating_point T>
constexpr int getLowerDecimalPrecision()
{
  if constexpr (std::is_same_v<T, float>)
    return FLT_DIG;
  else
  if constexpr (std::is_same_v<T, double>)
    return DBL_DIG;
  else
  if constexpr (std::is_same_v<T, long double>)
    return LDBL_DIG;
}
template<converter::c_floating_point T>
using T2S_Format_ldp = converter::T2S_Format_StreamDecimalPrecision<T, getLowerDecimalPrecision<T>() >;

template<converter::c_floating_point T>
using T2S_Format_DE_ldp = converter::T2S_Format_floating_StreamCombine<T, deLocal_oss<T>, T2S_Format_ldp<T> >;

template<converter::c_floating_point T>
using ConvertFromVal_DE_lDP = converter::ConvertFromVal<T, T2S_Format_DE_ldp<T> >;
...
  doc.SetCell< ConvertFromVal_DE_lDP<long double> >("A", "ld", 3.14159265358979323846L);
```



Reading a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
----------------------------------------------------------------------
By default rapidcsv throws an exception if one tries to convert non-numeric text to numeric data type, as it basically propagates the underlying conversion routines' exceptions to the calling application. Read about [Invalid Numbers](https://github.com/panchaBhuta/converter#reading-a-string-with-invalid-numbers-eg-empty-string-as-numeric-data) from converter.

The reason for this is to ensure data correctness. If one wants to be able
to identify cells with invalid numbers for numeric data types, one can either use 
`ConvertFromStr_fNaN<T>` from [test048.cpp](tests/test048.cpp)
OR
`ConvertFromStr_gNaN` from [test049.cpp](tests/test049.cpp).

For floating-types which supports `std::numeric_limits<T>::signaling_NaN()`, consider using template-typename `ConvertFromStr_fNaN<T>`, if _signaling_NaN()_ (NaN -> Not a Number) return-value is required, instead of an error thrown.

```c++
template<typename T>
using ConvertFromStr_fNaN =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_std_CtoT<T, converter::FailureS2Tprocess::QUIET_NAN>
                             >;
```

Alternatively, template-typename `ConvertFromStr_gNaN<T>` can be used for all types(integer, floating, bool) to either read the underlying valid-number or the string which raised the conversion-error. The function `ConvertFromStr_gNaN<T>::ToVal(...)` returns `std::variant<T, std::string>`.

```c++
template<typename T>
using ConvertFromStr_gNaN =
    converter::ConvertFromStr<T,
                              converter::S2T_Format_std_CtoT<T, converter::FailureS2Tprocess::VARIANT_NAN>
                             >;
```

Usage Example:

```c++
rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(),
                       rapidcsv::SeparatorParams());
...
float cellVal_f = doc.GetCell< ConvertFromStr_fNaN<float> >("colName", rowIdx);
...
std::variant<int, std::string> cellVal_i = doc.GetCell< ConvertFromStr_gNaN<int> >("colName", rowIdx);
...
std::variant<double, std::string> cellVal_d = doc.GetCell< ConvertFromStr_gNaN<double> >("colName", rowIdx);
```


Writing a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
----------------------------------------------------------------------
By default rapidcsv would not write to file non-numeric text, as it basically propagates the underlying conversion routines' exceptions to the calling application. Read about [Invalid Numbers](https://github.com/panchaBhuta/converter#output-string-conversion-with-invalid-valuesnumbers-eg-empty-cells-as-input-numeric-data) from converter.

1. `NaN` for floating types
```c++
double dNaN =  std::numeric_limits<double>::quiet_NaN();
doc.SetCell< double >("colName", "rowName", dNaN);   //  cell will have string value as 'nan'
```

2. `std::variant` for any type
```c++
std::variant<int, std::string> iInvalid("NotAnInteger");
doc.SetCell<  std::variant<double,std::string>
           >("colName", "rowName", iInvalid);   //  cell will have string value as 'NotAnInteger'
```
<br>
<br>

View CSV data using 'filters' and/or 'sort' on Column(s)
========================================================
rapidcsv::FilterDocument
------------------------
Similar to 'WHERE' clause in SQL, [rapidcsv::FilterDocument](doc/view/rapidcsv_FilterDocument.md) applies filters on one or more columns.<br>
A filter is defined by a boolean function(refer `isFirstCellPositive`) which is template parameter to [rapidcsv::FilterDocument](doc/view/rapidcsv_FilterDocument.md).

Refer [tests/testView001.cpp](tests/testView001.cpp) <br>

```cpp
bool isFirstCellPositive(const rapidcsv::Document::t_dataRow& dataRow)
{
  // NOTE : raw-index=1 as   raw-index=0   is used for 'label' : refer-> rapidcsv::LabelParams(0, 0)
  return (std::stoi(dataRow.at(1))) >= 0;
}
...
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    /////  Filter
    rapidcsv::FilterDocument<isFirstCellPositive> viewdoc(doc);
```

rapidcsv::SortDocument
----------------------
Similar to 'ORDER BY' clause in SQL, `rapidcsv::SortDocument` applies sorting on one or more columns. <br>
[rapidcsv::SortParams](doc/view/rapidcsv_SortParams.md) is used to define column on which sorting needs by applied, the data-type of that column and sorting-order(default is ascending).

Refer [tests/testView001.cpp](tests/testView001.cpp) <br>
```cpp
    const rapidcsv::SortParams<int> spA(1);
    rapidcsv::SortDocument<decltype(spA)> viewdoc1(doc, spA);   // `<decltype(spA)>` mandatory for clang
```

Multiple columns can vebew specified for sorting(similar to 'ORDER BY' clause in SQL) thru variadic-arguments. Below snippet is for 2 columns.<br>
First column is 'Company-name' : [column-raw-index = 1] , [type -> std::string] , [order(default) -> rapidcsv::e_SortOrder::ASCEND]
Second column is 'trading-date' : [column-raw-index = 2] , [type -> std::chrono::year_month_day] , [order -> rapidcsv::e_SortOrder::DESCEND]
```c++
  rapidcsv::Document csvDoc(path, rapidcsv::LabelParams(0, 0));

  const rapidcsv::SortParams<std::string> spCompanyName(1);
  // sorts latest Traded data first
  const rapidcsv::SortParams<std::chrono::year_month_day> spTradingDate(2, rapidcsv::e_SortOrder::DESCEND);
  rapidcsv::SortDocument<decltype(spChangeDate), decltype(spCompanyName)> viewdoc(csvDoc, spChangeDate, spCompanyName);
```

rapidcsv::FilterSortDocument
----------------------------
When both filter and sorting is needed [rapidcsv::FilterSortDocument](doc/view/rapidcsv_FilterSortDocument.md). <br>
Both the filter-function and sort-details are passed to [rapidcsv::FilterSortDocument](doc/view/rapidcsv_FilterSortDocument.md)

Refer [tests/testView001.cpp](tests/testView001.cpp)
```c++
    /////  Filter + Sort
    const rapidcsv::SortParams<int, rapidcsv::e_SortOrder::DESCEND> spD(1);
    rapidcsv::FilterSortDocument<isFirstCellPositive, decltype(spD)> viewdoc2(doc, spD);
```
<br>

Similar to `rapidcsv::SortDocument`, multiple columns can be sorted thru variadic-arguments. Refer [tests/testViewB001.cpp](tests/testViewB001.cpp)
```c++
    rapidcsv::Document doc(path, rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_PRESENT,
                                                       rapidcsv::FlgRowName::RN_PRESENT));

    /////  Filter + Sort
    const rapidcsv::SortParams<int> spA(0);
    const rapidcsv::SortParams<int, rapidcsv::e_SortOrder::DESCEND> spD(1);
    rapidcsv::FilterSortDocument<isFirstCellPositive,
                                 rapidcsv::SortParams<int>,
                                 rapidcsv::SortParams<int, rapidcsv::e_SortOrder::DESCEND> > viewdoc1(doc, spA, spD);
```
<br>

If the sorted column has NaN (Not-A-Number) values, one can still sort using `converter::FailureS2Tprocess::VARIANT_NAN` template parameter. Refer [tests/testViewfNaN001.cpp](tests/testViewfNaN001.cpp)
```c++
template <typename T>
using _ConvS2T_NAN =
      converter::ConvertFromStr
          < T,
            converter::S2T_Format_std_CtoT
                < T,
                  converter::FailureS2Tprocess::VARIANT_NAN
                >
          >;
...
    /////  Filter + Sort
    const rapidcsv::SortParams<_ConvS2T_NAN<int>, rapidcsv::e_SortOrder::DESCEND> spD(0);
    rapidcsv::FilterSortDocument<isCellPositive, decltype(spD)> viewdoc2(doc, spD);
```

<br>
<br>

cpp-properties (Recursive-Variable)
===================================
A small C++ properties parser. The parser also supports one property embedded into the value of another
property. The embedded property will be replaced by its value recursively. 


## Examples
__input.properties__
```
key1=value1
key2=value2
key3=value3
key4=value4
dir1=dname1
dir2=dname2
dir3=dname3
dir123=${dir1}/${dir2}/${dir3}
dir123123=${dir123}/${dir123}
```

__output.properties__
```
key100=value100
key200=value200
key300=value300
```

__Main.cpp__
```c++
#include <iostream>
#include <vector>
#include <string>
#include <rapidcsv/Properties.h>


void TestRead() {
    properties::Properties props = properties::PropertyFile::deserialization("input.properties");
    std::vector<std::string> names = props.GetPropertyNames();
    for (std::vector<std::string>::const_iterator i = names.begin(); i != names.end(); ++i) {
        cout << *i << " = " << props.GetProperty(*i) << endl;
    }
}

void TestPropertyExpanded() {
    properties::Properties props = properties::PropertyFile::deserialization("input.properties");
    std::vector<std::string> names = props.GetPropertyNames();
    for (std::vector<std::string>::const_iterator i = names.begin(); i != names.end(); ++i) {
        cout << *i << " = " << props.GetPropertyExpanded(*i) << endl;
    }
}

void TestWrite() {
    properties::Properties props;
    props.AddProperty("key100", "value100");
    props.AddProperty("key200", "value200");
    props.AddProperty("key300", "value300");
    properties::PropertyFile::serialization("output.properties", props);
}

int main() {
    TestRead();
    TestPropertyExpanded();
    TestWrite();
    return 0;
}
```





<br>
<br>

Architecture Components and Overview
====================================
1. The CSV data is read by a object of `Document` class, either from file or stream. <br>
   `Document` has _Getters_ and _Setters_ template functions which provide access to columns, rows and cells. <br>
   The _Getter_ template can be instantiated either using `data-type`, `convertor-type` or `convertor-function-address`.
   ```c++
   document.Get***<T>(...);            //  1. T is 'data-type' such as int, long, float, double, ...
   document.Get***<C>(...);            //  2. C is 'convertor-type' satisfying concept 'converter::c_S2Tconverter'
                                       //          C can also be "a user defined Converter class"
   document.Get***<&CONV_S2T>(...);    //  3. CONV_S2T is 'function-address' of signature
                                       //          'R (*CONV_S2T)(const std::string&)'.
                                       //     R  is either type 'T' or 'std::variant<T, std::string>'
   ```

   The _Setter_ template can be instantiated either using `data-type`, `convertor-type` or `convertor-function-address`.
   ```c++
   document.Set***<T>(...);            //  1. T is 'data-type' such as int, long, float, double, ...
   document.Set***<C>(...);            //  2. C is 'convertor-type' satisfying concept 'converter::c_T2Sconverter'
                                       //          C can also be "a user defined Converter class"
   document.Set***<&CONV_T2S>(...);    //  3. CONV_T2S is 'function-address' of signature
                                      //           'std::string (*CONV_T2S)(const R&)'.
                                       //     R  is either type 'T' or 'std::variant<T, std::string>'
   ```


   Column and Row names can be defined(if needed) using `LabelParams`. This are optional with default values. <br>
   Separator between adjacent cells are defined using `SeparatorParams`. This are optional with default values. <br>
   Separator between rows are defined using `LineReaderParams`. This are optional with default values. <br>
   In addittion UTF-16 and UTF-8 encoding can be enabled or disabled.

2. For _string-to-Type_ and _Type-to-string_ conversions, these are delegated to [converter](https://github.com/panchaBhuta/converter), which is a header only library. Refer it's [architecture](https://github.com/panchaBhuta/converter#architecture-components-and-overview) documenttion.  <br>

3. `FilterDocument`     : this view of data excludes the elements filtered out. <br>
   `SortDocument`       : this view of data sorts the elements by order defined. <br>
   `FilterSortDocument` : this view of data excludes the elements filtered out, and sorts the remaining elements by order defined.

4. `Properties`         : Container for name-value pairs. <br>
   `PropertyFile`       : to deserialization(Read) and serialization(Write) `Properties` container to a file.

<br>
<br>

# API Documentation
The following classes makes up the Rapidcsv interface:

 ## CSV Document classes
 - [class rapidcsv::Document](doc/document/rapidcsv_Document.md)
 - [class rapidcsv::LabelParams](doc/document/rapidcsv_LabelParams.md)
 - [class rapidcsv::SeparatorParams](doc/document/rapidcsv_SeparatorParams.md)
 - [class rapidcsv::LineReaderParams](doc/document/rapidcsv_LineReaderParams.md)

 ## View classes
 - [class rapidcsv::SortParams](doc/view/rapidcsv_SortParams.md)
 - [class rapidcsv::SortKeyFactory](doc/view/rapidcsv_SortKeyFactory.md)
 - [class rapidcsv::_RowComparator](doc/view/rapidcsv__RowComparator.md)
 - [class rapidcsv::_ViewDocument](doc/view/rapidcsv__ViewDocument.md)
 - [class rapidcsv::FilterDocument](doc/view/rapidcsv_FilterDocument.md)
 - [class rapidcsv::FilterSortDocument](doc/view/rapidcsv_FilterSortDocument.md)

 ## Property classes
 - [class properties::Properties](doc/properties/properties_Properties.md)
 - [class properties::PropertyFile](doc/properties/properties_PropertyFile.md)

<br>
<br>

Technical Details
=================
Rapidcsv uses cmake for its tests. Commands to build and execute the test suite:

```bash
mkdir -p build && cd build
cmake -DRAPIDCSV_BUILD_TESTS=ON .. && make
ctest -C unit --output-on-failure && ctest -C perf --verbose
cd -
```

Rapidcsv uses [doxygenmd](https://github.com/d99kris/doxygenmd) to generate
its Markdown API documentation:

```bash
doxygenmd include/rapidcsv doc
```

Rapidcsv uses Uncrustify to ensure consistent code formatting:

```bash
uncrustify -c uncrustify.cfg --no-backup include/rapidcsv/rapidcsv.h
```

<br>
<br>

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

