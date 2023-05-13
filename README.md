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
## Refactored 'Convertor' template-class to template-alias-declarations 'ConvertFromVal' and 'ConvertFromStr'

class-member-functions in upstream [d99kris/.../rapidcsv.h](https://github.com/d99kris/rapidcsv/blob/master/src/rapidcsv.h) has been refactored away ...

```cpp
void Converter<T>::ToStr(const T& pVal, std::string& pStr) const;
                    AND
void Converter<T>::ToVal(const std::string& pStr, T& pVal) const;
```

... instead call, Class-static-functions ...

```cpp
static std::string ConvertFromVal<T, T2S_FORMAT>::ToStr(const T& pVal);
                    AND
static T ConvertFromStr<T, S2T_FORMAT>::ToVal(const std::string& pStr);
```

Note: `ConvertFromVal` and `ConvertFromStr` are alias declarations of template-classes `_ConvertFromVal` and `_ConvertFromStr` respectively.


### Removed `struct ConverterParams`
`struct ConverterParams`, which had several member variables to determine the run time conversion behaviour has been done away with.
Instead use template-prameters 'S2T_FORMAT' and 'T2S_FORMAT' which indicates the conversion algorithm for type-to-string and string-to-type respectively.

Default 'S2T_FORMAT' types are provided by `rapidcsv::S2T_DefaultFormat<T>::type`.
* for T = {integer-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; S2T_FORMAT = rapidcsv::S2T_Format_std_StoT
* for T = {floating-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; -> &nbsp; &nbsp; S2T_FORMAT = rapidcsv::S2T_Format_std_StoT
* for T = {string, char, bool} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; S2T_FORMAT =  rapidcsv::S2T_Format_WorkAround
* for T = {year_month_day} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; S2T_FORMAT =  rapidcsv::S2T_Format_StreamYMD< _"%F"_ >

_S2T_Format_StreamAsIs_ , _S2T_Format_StreamUseClassicLocale_ and _S2T_Format_StreamUserLocale_ use _std::istringstream_ for **string to T** conversion. 

Default 'T2S_FORMAT' types are provided by `rapidcsv::T2S_DefaultFormat<T>::type`.
* for T = {integer-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; T2S_FORMAT = rapidcsv::T2S_Format_std_TtoS
* for T = {floating-types} &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; -> &nbsp; &nbsp; T2S_FORMAT = rapidcsv::T2S_Format_StreamDecimalPrecision< _T_ >
* for T = {string, char, bool} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; T2S_FORMAT =  rapidcsv::T2S_Format_WorkAround
* for T = {year_month_day} &nbsp; &nbsp; &nbsp;-> &nbsp; &nbsp; T2S_FORMAT =  rapidcsv::T2S_Format_StreamYMD< _"%F"_ >

_T2S_Format_StreamAsIs_ , _T2S_Format_StreamUseClassicLocale_  , _T2S_Format_StreamDecimalPrecision\<T>_ and _T2S_Format_StreamUserLocale_ use _std::ostringstream_ for **T to string** conversion.

One can also provide their own implementations of _T2S_FORMAT_ or _S2T_FORMAT_. For implentation details, refer **exConv001.cpp, ex008.cpp, ex009.cpp**.


### Removed `class no_converter : public std::exception`
For types which are not supported will generate compile-time error. This approach is better then getting a run-time exception.

## Performance gains
Removed header `<typeinfo>` and calls to function `typeid(...)`.
In template based code, calling any _RTTI_ functions such as `typeid(...)` is unnecessary, as templates are working with _types_ itself.
As C++ template supports specialization based on types, calls to `typeid(...)` for run-time branching-conditions inside of class-member-functions takes a performance hit.
Instead use of template class specializations and compile time code selection/elimination using  **constexpr** eliminates run-time conditional branching, thereby improving performance.

Inside some of the for-loops in upstream-repo, conditional branching based on result of `std::distance(...)` has been eliminated by shifting-ahead the starting iterator of the for-loop.

Function addresses are passed as template-parameter, instead of previously function-parameter. This helps the compiler make inline/direct call instead of pointer-indirection.

## Added feature : View rows using Filter and Sort on columns
Filter on rows using template class `FilterDocument`. Similar to SQL `WHERE`.  
Sort rows using template class `SortDocument`. Similar to SQL `ORDER BY`.  
Both filter and sort is provided by template class `FilterSortDocument`.

For usage examples refer any of [tests/testView*.cpp](tests/testView001.cpp)

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a vector of floats.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/colhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex001.cpp#L1-L12



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

... to your project/include/rapidcsv/ directory and include it.


[//]: # (TODO : add to 'vcpkg' and 'conan' packages)
[//]: # (Rapidcsv is also available via)
[//]: # ([vcpkg]\(https://vcpkg.io/en/packages.html\) and)
[//]: # ([conan]\(https://conan.io/center/rapidcsv\) )
[//]: # (package managers.)

More Examples
=============

Several of the following examples are also provided in the `examples/`
directory. And can be individually compiled using `manualBuild.sh` and executed in Linux and macOS. Example running ex001.cpp:

```
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

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/colrowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex002.cpp#L1-L15


### Row Headers Only

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/rowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex003.cpp#L1-L12


### No Headers

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/nohdr.csv#L1-L5


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex004.cpp#L1-L15


Reading a File with Custom Separator
------------------------------------
For reading of files with custom separator (i.e. not comma), one need to
specify the SeparatorParams argument. The following example reads a file using
semi-colon as separator.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/semi.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex005.cpp#L1-L16



Supported Get/Set Data Types
----------------------------
The internal cell representation in the Document class is using std::string
and when other types are requested, standard conversion routines are used.
All standard conversions are relatively straight-forward, with the exception
of `char-types` or `bool`. For `char-types`, rapidcsv interprets the cell's (first)
byte as a character. For `bool`, the expected integer values are `0` or `1`.
The following example illustrates the supported data types.

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/colrowhdr.csv#L1-L6


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex006.cpp#L1-L21


**rapidcsv::datelib::year_month_day** is the alias Date Type
------------------------------------------------------------
As of writing this, `std::chrono` is not fully supported by various compilers. If that's the case then, alias `rapidcsv::datelib` which points to [date](https://github.com/HowardHinnant/date) is used. The default date format is _"%F"_ (i.e "%Y-%m-%d"). For configuring a different date format refer [exConv001.cpp](examples/exConv001.cpp) and [test001.cpp](tests/test001.cpp)


Global Custom Data Type Conversion
----------------------------------
One may override conversion routines (or add new ones) by implementing ToVal() and/or ToStr(). Below is an example of int conversion, to instead provide two decimal fixed-point numbers. Also see [tests/test035.cpp](tests/test035.cpp) for test using specialized instances of `ToVal()` and `ToStr()`.


https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex008.cpp#L1-L19


Custom Data Type Conversion Per Call
------------------------------------
It is also possible to override conversions on a per-call basis, enabling more flexibility. This is illustrated in the following example. Additional conversion override usage can be found in the test 
[tests/test063.cpp](tests/test063.cpp), 
[tests/test087.cpp](tests/test087.cpp) and
[tests/test092.cpp](tests/test092.cpp)

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex009.cpp#L1-L43


Reading CSV Data from a Stream or String
----------------------------------------
In addition to specifying a filename, rapidcsv supports constructing a Document
from a stream and, indirectly through stringstream, from a string. File streams
used with rapidcsv should be opened in `std::ios::binary` mode to enable full
functionality. Here is a simple example reading CSV data from a string:

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/ex007.cpp#L1-L25

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

https://github.com/panchaBhuta/rapidcsv_FilterSort/blob/24444bb7d56454fc8a0de783f8729f43a4aa2772/examples/exConv001.cpp#L1-L174


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

... or specify any specific locale, see for example [tests/test092.cpp](tests/test092.cpp)

```cpp
constexpr char de_Loc[] = "de_DE";  // string literal object with static storage duration

  using deLocal_iss = rapidcsv::S2T_Format_StreamUserLocale<de_Loc>;

  doc.GetCell<float, &rapidcsv::ConvertFromStr<float, deLocal_iss>::ToVal>("A", "2");
```

#### WARNING
With floating point types `std::to_string(...)` may yield unexpected results as the number of significant digits in the returned string can be zero, for e.g: pVal = 1e-09. The return value may differ significantly from what `std::cout` prints by default. That's why this particular specialization is disabled by default. In case if this is needed enable it by defining macro `ENABLE_STD_TtoS`

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
[//]: # (TODO : update the below readme's)
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

```bash
mkdir -p build && cd build
cmake -DRAPIDCSV_BUILD_TESTS=ON .. && make
ctest -C unit --output-on-failure && ctest -C perf --verbose
cd -
```

Rapidcsv uses [doxygenmd](https://github.com/d99kris/doxygenmd) to generate
its Markdown API documentation:

```bash
doxygenmd include doc
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

