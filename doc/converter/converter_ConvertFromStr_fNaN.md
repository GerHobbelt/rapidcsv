## class converter::ConvertFromStr_fNaN< T, S2T_FORMAT >

```c++
template<c_signaling_NaN T, typename S2T_FORMAT = typename S2T_DefaultFormat<T>::type >
struct ConvertFromStr_fNaN
```

Convertor class implementation for (floating) types which support default Not-A-Number (NaN) values.  

**Template Parameters**
- `T` 'type' converted to, from string data. Floating types which supports signaling_NaN.
- `S2T_FORMAT` Class which encapsulates conversion parameters/logic such as 'Locale'.

---

```c++
static T ToVal (const std::string & str)
```
Converts string holding a numerical value to numerical data-type representation. It's a wrapper over 'ConvertFromStr<T, S2T_FORMAT>::ToVal(str)' ; returning a signaling_NaN() in case of conversion failure. 

**Parameters**
- `str` input string. 

**Returns:**
- Numerical value if conversion succeeds. Else returns std::numeric_limits\<T>::signaling_NaN() on conversion failure. 

---

```c++
template<typename ... ARGS>
static T ToVal_args (const std::string & str, ARGS &&... args)
```
Converts string holding a numerical value to numerical data-type representation. 

**Parameters**
- `...args` variadic arguments that needs to be forwarded to underlying 'ConvertFromStr<T, S2T_FORMAT>::ToVal_args(str, ...)' 
- `str` input string. 

**Returns:**
- Numerical value if conversion succeeds. Else returns std::numeric_limits\<T>::signaling_NaN() on conversion failure. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

