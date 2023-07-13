Rapidcsv ( with Filter and Sort )
=====================

[//]: # (<img src="images/RefactoringInProgress.jpg" width="500">)

[//]: # (## Documentation is **NOT** updated to latest changes.)

Check [here](doc/UpstreamRepoChanges.md) for differences with [upstream repo](https://github.com/d99kris/rapidcsv).


| **Linux** | **Mac** | **Windows** |
|-----------|---------|-------------|
| [![Linux](https://github.com/panchaBhuta/rapidcsv_FilterSort/workflows/Linux/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions?query=workflow%3ALinux) | [![macOS](https://github.com/panchaBhuta/rapidcsv_FilterSort/workflows/macOS/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions?query=workflow%3AmacOS) | [![Windows](https://github.com/panchaBhuta/rapidcsv_FilterSort/workflows/Windows/badge.svg)](https://github.com/panchaBhuta/rapidcsv_FilterSort/actions?query=workflow%3AWindows) |

Rapidcsv is an easy-to-use C++ CSV parser library. It supports C++20 (and
later), is header-only and comes with a basic test suite.

The [library of upstream repo](https://github.com/d99kris/rapidcsv) was featured in book
[C++20 for Programmers](https://deitel.com/c-plus-plus-20-for-programmers/).

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a vector of floats.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/colhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex001.cpp#L1-L12



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

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/colrowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex002.cpp#L1-L16


### Row Headers Only
csv file with row header (no column header)

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/rowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex003.cpp#L1-L13


### No Headers
csv file with no column and row headers

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/nohdr.csv#L1-L5


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex004.cpp#L1-L15


Reading a File with Custom Separator
------------------------------------
For reading of files with custom separator (i.e. not comma), one need to
specify the SeparatorParams argument. The following example reads a file using
semi-colon as separator.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/semi.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex005.cpp#L1-L16



Supported Get/Set Data Types
----------------------------
The internal cell representation in the Document class is using std::string
and when other types are requested, standard conversion routines are used.
All standard conversions are relatively straight-forward, with the exception
of `char-types` or `bool`. For `char-types`, rapidcsv interprets the cell's (first)
byte as a character. For `bool`, the expected integer values are `0` or `1`.
The following example illustrates the supported data types.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/colrowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex006.cpp#L1-L21


### **rapidcsv::datelib::year_month_day** is the alias for `Date` type
As of writing this, `std::chrono` is not fully supported by various compilers. If that's the case then, alias `rapidcsv::datelib` which points to [date](https://github.com/HowardHinnant/date) is used. The default date format is _"%F"_ (i.e "%Y-%m-%d"). For configuring a different date format refer [exConv001.cpp](examples/exConv001.cpp) and [test001.cpp](tests/test001.cpp)


Global Custom Data Type Conversion
----------------------------------
One may override conversion routines (or add new ones) by implementing ToVal() and/or ToStr(). Below is an example of int conversion, to instead provide two decimal fixed-point numbers. Also see [tests/test035.cpp](tests/test035.cpp) for test using specialized instances of `ToVal()` and `ToStr()`.


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex008.cpp#L1-L19


Custom Data Type Conversion Per Call
------------------------------------
It is also possible to override conversions on a per-call basis, enabling more flexibility. This is illustrated in the following example. Additional conversion override usage can be found in the tests 
[tests/test063.cpp](tests/test063.cpp), 
[tests/test087.cpp](tests/test087.cpp) and
[tests/test092.cpp](tests/test092.cpp)

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex009.cpp#L1-L43


Reading CSV Data from a Stream or String
----------------------------------------
In addition to specifying a filename, rapidcsv supports constructing a Document
from a stream and, indirectly through stringstream, from a string. File streams
used with rapidcsv should be opened in `std::ios::binary` mode to enable full
functionality. Here is a simple example reading CSV data from a string:

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/ex007.cpp#L1-L25


View CSV data using 'filters' and/or 'sort' on Column(s)
--------------------------------------------------------- 
Similar to 'WHERE' clause in SQL, 'rapidcsv::FilterDocument' applies filters on one or more columns. <br>
Similar to 'ORDER BY' clause in SQL, 'rapidcsv::SortDocument' applies sorting on one or more columns. <br>
Similar to 'ORDER BY' clause in SQL, 'rapidcsv::FilterSortDocument' applies sorting on one or more columns. <br>
Refer [tests/testView001.cpp](tests/testView001.cpp)


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


Architecture Components and Overview
====================================
1. The CSV data is read by a object of `Document` class, either from file or stream. <br>
   `Document` has _Getters_ and _Setters_ template functions which provide access to columns, rows and cells. <br>
   The _Getter_ template can be instantiated either using `data-type`, `convertor-type` or `convertor-function-address`.
   ```c++
   document.Get***<T>(...);            //  1. T is 'data-type' such as int, long, float, double, ...
   document.Get***<C>(...);            //  2. C is 'convertor-type' statisfying concept 'c_S2Tconverter'
                                       //          for e.g : 'ConvertFromStr_fNaN<T>' or 'ConvertFromStr_gNaN<T>'
                                       //                    or "a user defined Converter class"
   document.Get***<&CONV_S2T>(...);    //  3. CONV_S2T is 'function-address' of signature 'R (*CONV_S2T)(const std::string&)'
                                       //     R  is either T or 'std::variant<T, std::string>'

   ```

   The _Setter_ template can be instantiated either using `data-type`, `convertor-type` or `convertor-function-address`.
   ```c++
   document.Set***<T>(...);            //  1. T is 'data-type' such as int, long, float, double, ...
   document.Set***<C>(...);            //  2. C is 'convertor-type' statisfying concept 'c_T2Sconverter'
                                       //          for e.g : 'ConvertFromVal_gNaN<T>' 
                                       //                    or "a user defined Converter class"
   document.Set***<&CONV_T2S>(...);    //  3. CONV_T2S is 'function-address' of signature 'std::string (*CONV_T2S)(const R&)'
                                       //     R  is either T or 'std::variant<T, std::string>'
   ```


   Column and Row names can be defined(if needed) using `LabelParams`. This are optional with default values. <br>
   Separator between adjacent cells are defined using `SeparatorParams`. This are optional with default values. <br>
   Separator between rows are defined using `LineReaderParams`. This are optional with default values. <br>
   In addittion UTF-16 and UTF-8 encoding can be enabled or disabled.

2. `ConvertFromStr<T, S2T_FORMAT = S2T_DefaultFormat<T>::type>::ToVal(...)` function(s) convert string to `T` data-type. <br>
   `ConvertFromVal<T, T2S_FORMAT = T2S_DefaultFormat<T>::type>::ToStr(...)` function(s) convert `T` data-type to string. <br>
   There are several specializations of `ConvertFromStr` and `ConvertFromVal`. <br>
   Each specialization is for certain types `T` determined by _`concepts`_ and format type (`S2T_FORMAT` or `T2S_FORMAT`). <br>
   Format types `S2T_FORMAT` and `T2S_FORMAT` (if needed), provide additional information for formatting for types(such as `Date`) or `Locale` specific info/params. In addition if multiple conversion speicalization/algorithm is available for a particular type, the alternative specialization can be selected by passing the appropriate `S2T_FORMAT`/`T2S_FORMAT`. User could also define  their own specialization of `ConvertFromStr` or `ConvertFromVal` (NOTE: new class type of `S2T_FORMAT` or `T2S_FORMAT` will also need to be defined). <br>

3. `FilterDocument`     : this view of data excludes the elements filtered out. <br>
   `SortDocument`       : this view of data sorts the elements by order defined. <br>
   `FilterSortDocument` : this view of data excludes the elements filtered out, and sorts the remaining elements by order defined.




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

Potential data-precision loss can happen at steps (1), (2) and (3).
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


One can also provide their own decimal precision parameter thru several ways. This is illustrated in the example [exConv001.cpp](examples/exConv001.cpp), where data conversion precision is checked by implementing a complete conversion cycle i.e    **data -> string -> data** .<br>

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/7cf48a26b130993f12c72f6a6f80f900099445ba/examples/exConv001.cpp#L1-L166

Note: If only data conversions is needed, then just include [rapidcsv/converter.h](include/rapidcsv/converter.h).


Locale Parsing Formats : String-to-T (i.e S2T)
----------------------------------------------
**rapidcsv_FilterSort** uses `std::sto[T^]()` conversion functions when parsing numeric-type values from string by default.

```cpp
template<typename T>
struct S2T_DefaultFormat< T,
                          typename  std::enable_if_t< is_integer_type<T>::value ||
                                                      std::is_floating_point_v<T>
                                                    >
                        > 
{
  using type = S2T_Format_std_StoT;     //  uses 'std::sto[T^](...)' function's for conversion
};

.......

T ConvertFromStr< T,
                  S2T_FORMAT = S2T_DefaultFormat<T>::type
                >::ToVal(const std::string& pStr);

// "S2T_DefaultFormat<T>::type" for numeric-types evaluates to  "rapidcsv::S2T_Format_std_StoT"
```

It is possible to configure **rapidcsv_FilterSort** to use locale dependent parsing by changing template-parameter `S2T_FORMAT=S2T_Format_StreamAsIs`, see for example [tests/test087.cpp](tests/test087.cpp).

```cpp
  doc.GetCell< rapidcsv::ConvertFromStr< float,
                                         rapidcsv::S2T_Format_StreamAsIs
                                       >
             >("A", "2");
```
... or configure **rapidcsv_FilterSort** to use either classic-locale parsing by setting template-parameter `S2T_FORMAT=S2T_Format_StreamUseClassicLocale`,

```cpp
  float f1 = doc.GetCell< rapidcsv::ConvertFromStr< float,
                                                    rapidcsv::S2T_Format_StreamUseClassicLocale
                                                  >
                        >("A", "2");
```

... or specify any specific locale, see for example [tests/test092.cpp](tests/test092.cpp)

```cpp
constexpr char de_Loc[] = "de_DE";  // string literal object with static storage duration

  using deLocal_iss = rapidcsv::S2T_Format_StreamUserLocale<de_Loc>;

  float f2 = doc.GetCell< rapidcsv::ConvertFromStr< float,
                                                    deLocal_iss
                                                  >
                        >("A", "2");
```

For date, the default conversion format is `S2T_Format_StreamYMD`. With format "%F" -> "%Y-%m-%d".

```cpp
  template<>
  struct S2T_DefaultFormat< datelib::year_month_day, void >
  {
    using type = S2T_Format_StreamYMD< defYMDfmt >; // %F -> "%Y-%m-%d"
  };
```

Locale Parsing Formats : T-to-String (i.e T2S)
----------------------------------------------
**rapidcsv_FilterSort** uses `std::sto[T^]()` conversion functions when parsing numeric-type values from string by default.

```cpp

template<c_floating_point T>
struct T2S_DefaultFormat< T, void >
{
  using type = T2S_Format_StreamDecimalPrecision<T>;    // check the warning note below
};

template<c_integer_type T>
struct T2S_DefaultFormat< T, void >
{
  using type = T2S_Format_std_TtoS;         //    uses 'std::to_string(...)' for conversion
};

.......

template<typename T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
struct ConvertFromVal;
std::string ConvertFromVal< T,
                            T2S_FORMAT = typename T2S_DefaultFormat<T>::type
                          >::ToStr(const T& val);

// "T2S_DefaultFormat<T>::type" for floating-types evaluates to  "rapidcsv::T2S_Format_StreamDecimalPrecision<T>"
// "T2S_DefaultFormat<T>::type" for  integer-types evaluates to  "rapidcsv::T2S_Format_std_TtoS"
```

### WARNING
With floating point types `std::to_string(...)` may yield unexpected results as the number of significant digits in the returned string can be zero, for e.g: `pVal = 1e-09`. The return value may differ significantly from what `std::cout` prints by default. That's why this particular specialization is disabled by default. In case if this is needed, enable it by defining macro `ENABLE_STD_TtoS`. <br> <br>

It is possible to configure **rapidcsv_FilterSort** to use locale dependent parsing by changing template-parameter `T2S_FORMAT=T2S_Format_StreamAsIs`.

```cpp
  doc.SetCell< rapidcsv::ConvertFromVal< float,
                                         rapidcsv::T2S_Format_StreamAsIs
                                       >
             >("A", "2", 1.234f);
```
... or configure **rapidcsv_FilterSort** to use either classic-locale parsing by setting template-parameter `T2S_FORMAT=T2S_Format_StreamUseClassicLocale`,

```cpp
  doc.SetCell< rapidcsv::ConvertFromVal< double,
                                         rapidcsv::T2S_Format_StreamUseClassicLocale
                                       >
             >("A", "2", 1.23456789);
```

... or specify any specific locale

```cpp
constexpr char de_Loc[] = "de_DE";  // string literal object with static storage duration

  using deLocal_iss = rapidcsv::T2S_Format_StreamUserLocale<de_Loc>;

  doc.SetCell< rapidcsv::ConvertFromVal< float,
                                         deLocal_iss
                                       >
             >("A", "2", 1.234f);
```

For date, the default conversion format is `T2S_Format_StreamYMD`. With format "%F" -> "%Y-%m-%d".

```cpp
  template<>
  struct T2S_DefaultFormat< datelib::year_month_day, void >
  {
    using type = T2S_Format_StreamYMD< defYMDfmt >; // %F -> "%Y-%m-%d"
  };
```



Reading a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
----------------------------------------------------------------------
By default rapidcsv throws an exception if one tries to convert non-numeric text to numeric data type, as it basically propagates the underlying conversion routines' exceptions to the calling application.

The reason for this is to ensure data correctness. If one wants to be able
to identify cells with invalid numbers for numeric data types, one can either use 
`ConvertFromStr_fNaN` or `ConvertFromStr_gNaN`.

```c++
  template<c_signaling_NaN T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  struct ConvertFromStr_fNaN
  {
    inline static    T
    ToVal(const std::string& str);
  };

  template<c_NOT_string T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
  struct ConvertFromStr_gNaN
  {
    inline static    std::variant<T, std::string>
    ToVal(const std::string& str);
  };
```

For floating-types which supports `std::numeric_limits<T>::signaling_NaN()`, consider using template-typename `ConvertFromStr_fNaN<T, S2T_FORMAT>`, if _signaling_NaN()_ (NaN -> Not a Number) return-value is required, instead of an error thrown.

Alternatively, template-typename `ConvertFromStr_gNaN<T, S2T_FORMAT>` can be used for all types(integer, floating, bool) to either read the underlying valid-number or the string which raised the conversion-error. The function `ConvertFromStr_gNaN<T, S2T_FORMAT>::ToVal` returns `std::variant<T, std::string>`.

Usage Example:

```cpp
rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(),
                       rapidcsv::SeparatorParams());
...
float cellVal_f = doc.GetCell<float, ConvertFromStr_fNaN<float>::ToVal>("colName", rowIdx);
...
std::variant<int, std::string> cellVal_i = doc.GetCell<int, ConvertFromStr_gNaN<int>::ToVal>("colName", rowIdx);
...
std::variant<double, std::string> cellVal_d = doc.GetCell<double, ConvertFromStr_gNaN<double>::ToVal>("colName", rowIdx);
```

Refer [test048.cpp](tests/test048.cpp) and [test049.cpp](tests/test049.cpp) for sample code.

Writing a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
----------------------------------------------------------------------
By default rapidcsv would not write to file non-numeric text, as it basically propagates the underlying conversion routines' exceptions to the calling application.

The reason for this is to ensure data correctness. If one wants to be able
to identify cells with invalid numbers for numeric data types, one can use 
`ConvertFromVal_gNaN`.

```c++
  template<c_NOT_string T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
  struct ConvertFromVal_gNaN
  {
    inline static std::string
    ToStr(const std::variant<T, std::string>& val)
  };
```

Template-typename `ConvertFromVal_gNaN<T, T2S_FORMAT>` can be used for all types(integer, floating, bool) to write the underlying valid-number or the string which raised the conversion-error. The function `ConvertFromVal_gNaN<T, T2S_FORMAT>::ToStr` has input-type `std::variant<T, std::string>`.



# API Documentation
The following classes makes up the Rapidcsv interface:

## Convertor classes
 - [class rapidcsv::ConvertFromStr< T, S2T_FORMAT >](doc/converter/rapidcsv_ConvertFromStr.md)
 - [class rapidcsv::ConvertFromStr_fNaN< T, S2T_FORMAT >](doc/converter/rapidcsv_ConvertFromStr_fNaN.md)
 - [class rapidcsv::ConvertFromStr_gNaN< T, S2T_FORMAT >](doc/converter/rapidcsv_ConvertFromStr_gNaN.md)
 - [class rapidcsv::ConvertFromVal< T, T2S_FORMAT >](doc/converter/rapidcsv_ConverterToVal.md)
 - [class rapidcsv::ConvertFromVal_gNaN< T, T2S_FORMAT >](doc/converter/rapidcsv_ConvertFromVal_gNaN.md)
 - [class rapidcsv::ConvertFromTuple< T_C >](doc/converter/rapidcsv_ConvertFromTuple.md)

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

