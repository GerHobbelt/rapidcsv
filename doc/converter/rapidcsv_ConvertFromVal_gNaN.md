## class rapidcsv::ConvertFromVal_gNaN< T, T2S_FORMAT >

```c++
template<c_NOT_string T, typename T2S_FORMAT = typename T2S_DefaultFormat<T>::type >
struct ConvertFromVal_gNaN
```

Convertor class implementation for any numeric types, with indicators for Not-A-Number (NaN) and Null values.  

**Template Parameters**
- `T` 'type' converted from, to string.
- `T2S_FORMAT` Class which encapsulates conversion parameters/directives (optional argument). <br>
                            Specialization for custom datatype conversions, using this template parameter.

---

```c++
static std::string ToStr (const input_type & val)
```

Converts integer datatype to string. 

**Parameters**
- `val` A std::variant<T, std::string> , holding either numerical-value or raw string. 

**Returns:**
- string holding a integer represenation or raw-string of the variant<T,string> type. Else throws error on conversion failure. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

