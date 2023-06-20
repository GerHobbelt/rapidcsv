## class rapidcsv::ConvertFromStr_gNaN< T, S2T_FORMAT >

```c++
template<c_signaling_NaN T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
struct ConvertFromStr_gNaN
```

Convertor class implementation for any numeric types, with indicators for Not-A-Number (NaN) and Null values.  

**Template Parameters**
- `T` 'type' converted to, from string data.
- `S2T_FORMAT` Class which encapsulates conversion parameters/logic such as 'Locale'.

---

```c++
static return_type ToVal (const std::string & str)
```
Converts string holding a numerical value to numerical datatype representation. 

**Parameters**
- `str` input string. 

**Returns:**
- A std::variant<T, std::string> , holding either numerical-value, or raw string on conversion failure. It's a Numerical value if conversion succeeds. Else on conversion failure, the underlying string value that caused failure. 

---

```c++
template<typename ... ARGS>
static return_type ToVal_args (const std::string & str, ARGS &&... args)
```
Converts string holding a numerical value to numerical datatype representation. 

**Parameters**
- `str` input string. 
- `...args` variadic arguments that needs to be forwarded to underlying 'ConvertFromStr<T, S2T_FORMAT>::ToVal_args(str, ...)' 

**Returns:**
- A std::variant<T, std::string>. It's a Numerical value if conversion succeeds. Else on conversion failure, the underlying string value that caused failure. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

