# Differences with the [upstream repo](https://github.com/d99kris/rapidcsv)

If one wants to port from [upstream repo](https://github.com/d99kris/rapidcsv) and use this repo instead, some the changes needs to be made in your code base. Refer below for the same.


## Use 'ConvertFromVal' and 'ConvertFromStr' instead of 'Convertor'

call, Class-static-functions ...

```cpp
static std::string ConvertFromVal<T, T2S_FORMAT>::ToStr(const T& pVal);
                    AND
static T ConvertFromStr<T, S2T_FORMAT>::ToVal(const std::string& pStr);
```

.. instead of class-member-functions in upstream [d99kris/.../rapidcsv.h](https://github.com/d99kris/rapidcsv/blob/master/src/rapidcsv.h) ...

```cpp
void Converter<T>::ToStr(const T& pVal, std::string& pStr) const;
                    AND
void Converter<T>::ToVal(const std::string& pStr, T& pVal) const;
```

<br>

## Removed `struct ConverterParams`
`struct ConverterParams`, which had several member variables to determine the run time conversion behavior has been done away with. <br> <br>
Instead use template-prameters 'S2T_FORMAT' and 'T2S_FORMAT' which indicates the conversion algorithm for type-to-string and string-to-type respectively.

Default 'S2T_FORMAT' types are provided by `rapidcsv::S2T_DefaultFormat<T>::type`.
```
* for T = {integer-types}        ->   S2T_FORMAT = rapidcsv::S2T_Format_std_StoT
* for T = {floating-types}       ->   S2T_FORMAT = rapidcsv::S2T_Format_std_StoT
* for T = {string, char, bool}   ->   S2T_FORMAT =  rapidcsv::S2T_Format_WorkAround
* for T = {year_month_day}       ->   S2T_FORMAT =  rapidcsv::S2T_Format_StreamYMD< _"%F"_ >
```

_S2T_Format_StreamAsIs_ &nbsp; , &nbsp; _S2T_Format_StreamUseClassicLocale_ &nbsp; and &nbsp; _S2T_Format_StreamUserLocale_ &nbsp; use &nbsp; _`std::istringstream`_ &nbsp; for &nbsp; **"string to T"** &nbsp; conversion.
<br> <br>

Default 'T2S_FORMAT' types are provided by `rapidcsv::T2S_DefaultFormat<T>::type`.
```
* for T = {integer-types}        ->   T2S_FORMAT = rapidcsv::T2S_Format_std_TtoS
* for T = {floating-types}       ->   T2S_FORMAT = rapidcsv::T2S_Format_StreamDecimalPrecision< _T_ >
* for T = {string, char, bool}   ->   T2S_FORMAT =  rapidcsv::T2S_Format_WorkAround
* for T = {year_month_day}       ->   T2S_FORMAT =  rapidcsv::T2S_Format_StreamYMD< _"%F"_ >
```

_T2S_Format_StreamAsIs_ &nbsp; , &nbsp; _T2S_Format_StreamUseClassicLocale_ &nbsp; , &nbsp; _T2S_Format_StreamDecimalPrecision\<T>_ &nbsp; and &nbsp; _T2S_Format_StreamUserLocale_ &nbsp; use &nbsp; _`std::ostringstream`_ &nbsp; for &nbsp; **"T to string"** &nbsp; conversion.
<br> <br>

One can also provide their own implementations of _T2S_FORMAT_ or _S2T_FORMAT_. For implentation details, refer **exConv001.cpp, ex008.cpp, ex009.cpp**.

<br>


## Removed `class no_converter : public std::exception`
For types which are not supported will generate compile-time error. This approach is better then getting a run-time exception.

<br>

## Refactored constructor of `LabelParams`
In the upstream repo, the labels-usage for row and column could be turned off by passing `-1`. Instead of `int` , enum's `FlgColumnName` and `FlgRowName` are passed to the constrcutor.

```c++
  enum FlgColumnName { CN_PRESENT, CN_MISSING };
  enum FlgRowName    { RN_PRESENT, RN_MISSING };
...
    explicit LabelParams(const FlgColumnName pColumnNameFlg = FlgColumnName::CN_PRESENT,
                         const FlgRowName    pRowNameFlg    = FlgRowName::RN_MISSING)
```

`FlgColumnName::CN_PRESENT` would imply that the column labels(headers) are available in the first row of the CSV file.
`FlgRowName::RN_PRESENT` would imply that the row labels are available in the first column of the CSV file.


<br>

# Performance gains
Removed header `<typeinfo>` and calls to function `typeid(...)`.
In template based code, calling any _RTTI_ functions such as `typeid(...)` is unnecessary, as templates are working with _types_ itself. 

As C++ template supports specialization based on types, calls to `typeid(...)` for run-time branching-conditions inside of class-member-functions takes a performance hit.

Instead use of template class specializations and compile time code selection/elimination using  **constexpr** eliminates run-time conditional branching, thereby improving performance.

Inside some of the for-loops in upstream-repo, conditional branching based on result of `std::distance(...)` has been eliminated by shifting-ahead the starting iterator of the for-loop.

Function addresses are passed as template-parameter, instead of upstream function-parameter. This helps the compiler make inline/direct call(where possible) instead of pointer-indirection.

## Added feature : View rows using Filter and Sort on columns
Filter on rows using template class `FilterDocument`. Similar to SQL `WHERE`.  
Sort rows using template alias `SortDocument`. Similar to SQL `ORDER BY`.  
Both filter and sort is provided by template class `FilterSortDocument`.

For usage examples refer any of [tests/testView*.cpp](tests/testView001.cpp)

