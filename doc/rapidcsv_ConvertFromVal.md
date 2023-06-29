## class rapidcsv::ConvertFromVal< T, T2S_FORMAT >

```c++
template<typename T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
struct ConvertFromVal;
```

Class declaration for any types, TO string using 'std::ostringstream'.  

**Template Parameters**
- `T` 'type' converted from, to string data.
- `T2S_FORMAT` Class which encapsulates conversion parameters/directives such as 'Locale'. <br>
                            Optional argument, default's to 'T2S_DefaultFormat\<T>::type'.
---
---


## class rapidcsv::ConvertFromVal< T, T2S_FORMAT >

```c++
template<c_NOT_string T, c_formatOSS T2S_FORMAT >
struct ConvertFromVal<T,T2S_FORMAT>
```

Convertor class implementation for any types, TO string; using 'std::ostringstream'.  

**Template Parameters**
- `T` 'type' converted from, to string data. (Not Applicable for string to string conversion)
- `T2S_FORMAT` Class which encapsulates conversion parameters/directives such as 'Locale'.
---

```c++
static std::string ToStr (const T & val)
```
Converts numerical datatype from string holding a numerical value. 

**Parameters**
- `val` input numerical value. 

**Returns:**
- Output string if conversion succeeds. Else throws 'std::invalid_argument' on conversion failure. 

---
---


## class rapidcsv::ConvertFromVal< T, T2S_Format_std_TtoS >

```c++
  template<c_integer_type T>
  struct ConvertFromVal<T, T2S_Format_std_TtoS >
```

Convertor class implementation from integer types TO string.  

**Template Parameters**
 - `T` 'type' converted from, to string data.
---

```c++
static std::string ToStr (const T & val)
```
Converts integer datatype to string. 

**Parameters**
- `val` numerical value. 

**Returns:**
- string holding a integer represenation. Else throws error on conversion failure. 

---
---


## class rapidcsv::ConvertFromVal< std::string, T2S_Format_WorkAround >

```c++
template<>
struct ConvertFromVal<std::string, T2S_Format_WorkAround>
```

Specialized implementation handling string to string conversion.  

---

```c++
static std::string ToStr (const std::string & val)
```
Converts string to string. 

**Parameters**
- `val` input string. 

**Returns:**
- string. 

---
---


## class rapidcsv::ConvertFromVal< T, T2S_Format_WorkAround >

```c++
template<c_char T>
struct ConvertFromVal<T, T2S_Format_WorkAround>
```

Specialized implementation handling char to string conversion.  

**Template Parameters**
- `T` 'char-type' converted from, to string data.
---

```c++
static std::string ToStr (const T & val)
```
Converts char value to string. 

**Parameters**
- `val` input string. 

**Returns:**
- string. 

---
---


## class rapidcsv::ConvertFromVal< bool, T2S_Format_WorkAround >

```c++
template<>
struct ConvertFromVal<bool, T2S_Format_WorkAround>
```

Specialized implementation handling bool to string conversion.  

---

```c++
static std::string ToStr (const bool & val)
```
Converts bool value to string. 

**Parameters**
- `val` input bool value. 

**Returns:**
- string. either '0' or '1' (for : 'false' or 'true') 

---
---


## class rapidcsv::ConvertFromVal< datelib::year_month_day, T2S_FORMAT_YMD >

```c++
template<c_formatYMDoss T2S_FORMAT_YMD>
struct ConvertFromVal< datelib::year_month_day, T2S_FORMAT_YMD>
```

Specialized implementation handling 'year_month_day' to string conversion.  

**Template Parameters**
- `T2S_FORMAT` Class which encapsulates conversion parameters/directives (optional argument).<br>
                            Specialization for custom datatype conversions, using this template parameter.
---

```c++
static std::string ToStr (const datelib::year_month_day & val)
```

Converts variable holding 'year_month_day' value to string. The string has the format '%F' -> '%Y-%m-%d'. 

**Parameters**
- `val` input 'year_month_day'. 

**Returns:**
- string. 

---

```c++
static std::string ToStr_args (const datelib::year_month_day & val,
                               const std::string::value_type * fmt,
                               std::string * abbrev = nullptr,
                               std::chrono::seconds * offset_sec = nullptr)
```
Converts variable holding 'year_month_day' value to string. The string has the format '%F' -> '%Y-%m-%d'. 

**Parameters**
- `val` input 'year_month_day'. 
- `fmt` a format string 
- `abbrev` if not null, pointer to an object that will hold the time zone abbreviation or name corresponding to the Z specifier 
- `offset_sec` if not null, pointer to an object that will hold the offset from UTC corresponding to the z specifier 

**Returns:**
- string. 

---
---

## function rapidcsv::SetTuple< c_T2Sconverter ... T2Sconv >

```c++
template <c_T2Sconverter ... T2Sconv>
inline void SetTuple(const std::tuple<typename T2Sconv::input_type ...>& dataTuple,
                      size_t colIdx,
                      std::vector<std::string>& dataVec);
```

populate a vector of string from a tuple.

**Template Parameters**
- `T2Sconv`             converter types that satisfies concept 'c_T2Sconverter'.

**Parameters**
- `dataTuple`           values stored in the tuple after performing string-to-value conversion.
- `colIdx`              start id of dataVec in case vector starts with the column-name.
- `dataVec`             vector of string, having string representation of numeric values.


---
---

## class rapidcsv::t_T2Sconv< T_C >

If a 'type-C' satisfies concept 'c_T2Sconverter', then use that 'type-C'; else assume it's a 'type-T' and bumped up using 'ConvertFromVal< type-T >' to create a class staisfying concept 'c_T2Sconverter'. This mechanism enables 'template-converter-algorithm' to handle both 'type-C' and 'type-T' using the same code base, (i.e. reduces code duplicity).  

```c++
template< typename T_C >
struct t_T2Sconv;
```

**Template Parameters**
 - `T_C`                   T can be data-type such as int, double etc ; xOR  C -> Conversion class statisfying concept 'c_T2Sconverter'.

---
```c++
using conv_type;
```
a converter alias that satisfies concept 'c_T2Sconverter'


---
---

## class rapidcsv::f_T2Sconv< CONV_T2S >

convert a function with signature 'auto (*CONV_S2T)(const std::string&)' to a converter type that satisfies concept 'c_T2Sconverter'.  

```c++
template<auto CONV_T2S >
struct f_T2Sconv;
```

**Template Parameters**
- `CONV_T2S`                   a function with signature 'std::string (*CONV_T2S)(const T_V&)'. T_V represents either numeric type 'T' or 'variant std::variant<T, std::string>'

---
```c++
using conv_type;
```
a converter alias that satisfies concept 'c_T2Sconverter'

---
---
