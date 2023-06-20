## class rapidcsv::ConvertFromStr< T, S2T_FORMAT >

```c++
template<typename T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
struct ConvertFromStr;
```
Class declaration of convertor FROM string to any type's.  

**Template Parameters**
- `T` 'type' converted to, from string data.
- `S2T_FORMAT` Class which encapsulates conversion parameters/directives such as using 'Locale'. <br>
                            Optional argument, default's to 'S2T_DefaultFormat<T>::type'.
---
---


## class rapidcsv::ConvertFromStr< T, S2T_FORMAT >

```c++
template< c_NOT_string T, c_formatISS S2T_FORMAT >
struct ConvertFromStr<T, S2T_FORMAT>
```

Convertor class implementation for any (no-string)type's, FROM string; using 'std::istringstream'.  

**Template Parameters**
- `T` 'type' converted to, from string data. (Not Applicable for 'string to string' conversion.)
- `S2T_FORMAT` Class which encapsulates conversion parameters/directives such as using 'Locale'.

---

```c++
static T ToVal (const std::string & str)
```
Converts string holding a numerical value to numerical datatype representation. 

**Parameters**
- `str` input string. 

**Returns:**
- Numerical value if conversion succeeds. Else throws 'std::invalid_argument' on conversion failure. 

---
---


## class rapidcsv::ConvertFromStr< T, S2T_Format_std_StoT >

```c++
template <c_integer_type T>
struct ConvertFromStr<T, S2T_Format_std_StoT >
```

Convertor class implementation for integer types FROM string.  

**Template Parameters**
- `T` 'integer-type' converted to, from string data.

---

```c++
static T ToVal (const std::string & str)
```
Converts string holding a integer represenation to integer datatype. 

**Parameters**
- `str` input string. 

**Returns:**
- Numerical value if conversion succeeds. Else throws error on conversion failure. 

---

```c++
static T ToVal_args (const std::string & str,
                     std::size_t * pos = nullptr,
                     int base = 10)
```
Converts string holding a integer represenation to integer datatype. 

**Parameters**
- `str` input string. 
- `pos` address of an integer to store the number of characters processed. 
- `base` the number base. 

**Returns:**
- Numerical value if conversion succeeds. Else throws error on conversion failure. 

---
---


## class rapidcsv::ConvertFromStr< T, S2T_Format_std_StoT >

```c++
template<c_floating_point T>
struct ConvertFromStr<T, S2T_Format_std_StoT >
```

Convertor class implementation for floating types from string.

**Template Parameters**
- `T`                     'floating-type' converted to, from string data.

---

```c++
static T ToVal (const std::string & str)
```

Converts string holding a floating-number represenation to floating datatype.

**Parameters**
- `str` input string. 

**Returns:**
- Numerical value if conversion succeeds. Else throws error on conversion failure. 

---

```c++
static T ToVal_args (const std::string & str,
                     std::size_t * pos = nullptr)
```

Converts string holding a floating-number represenation to floating datatype.

**Parameters**
- `str` input string. 
- `pos` address of an integer to store the number of characters processed. 

**Returns:**
- Numerical value if conversion succeeds. Else throws error on conversion failure. 

---
---


## class rapidcsv::ConvertFromStr< std::string, S2T_Format_WorkAround >

```c++
template<>
struct ConvertFromStr<std::string, S2T_Format_WorkAround>
```

Specialized implementation handling string to string conversion.  

---

```c++
static std::string ToVal (const std::string & str)
```
Converts string to string. 

**Parameters**
- `str` input string. 

**Returns:**
- string. 

---
---


## class rapidcsv::ConvertFromStr< T, S2T_Format_WorkAround >

```c++
template<c_char T>
struct ConvertFromStr< T, S2T_Format_WorkAround >
```

Specialized implementation handling string to char conversion.  

**Template Parameters**
- `T` 'char-type' converted to, from string data.

---

```c++
static T ToVal (const std::string & str)
```
Converts string holding char value. 

**Parameters**
- `str` input string. 

**Returns:**
- char-type. 

---
---


## class rapidcsv::ConvertFromStr< bool, S2T_Format_WorkAround >

```c++
template<>
struct ConvertFromStr<bool, S2T_Format_WorkAround>
```

Specialized implementation handling string to bool conversion.  

---

```c++
static bool ToVal (const std::string & str)
```
Converts string holding bool value. 

**Parameters**
- `str` input string. 

**Returns:**
- bool. 

---
---


## class rapidcsv::ConvertFromStr< datelib::year_month_day, S2T_FORMAT_YMD >

```c++
template<c_formatYMDiss S2T_FORMAT_YMD>
struct ConvertFromStr< datelib::year_month_day, S2T_FORMAT_YMD>
```

Specialized implementation handling string to 'year_month_day' conversion.  

**Template Parameters**
- `S2T_FORMAT_YMD` Class which encapsulates conversion parameters/logic such as 'Locale' specific for 'year_month_day'.

---

```c++
static datelib::year_month_day ToVal (const std::string & str)
```
Converts string holding 'year_month_day' value. The string has the format '%F' -> '%Y-%m-%d'. 

**Parameters**
- `str` input string. 

**Returns:**
- 'year_month_day'. 

---

```c++
static datelib::year_month_day ToVal_args (const std::string & str,
                                           const std::string::value_type * fmt,
                                           std::string * abbrev = nullptr,
                                           std::chrono::minutes * offset = nullptr)
```
Converts string holding 'year_month_day' value. The string has the format '%F' -> '%Y-%m-%d'. 

**Parameters**
- `str` input string. 
- `fmt` a format string 
- `abbrev` if not null, pointer to an object that will hold the time zone abbreviation or name corresponding to the Z specifier 
- `offset` if not null, pointer to an object that will hold the offset from UTC corresponding to the z specifier 

**Returns:**
- 'year_month_day'. 

---


