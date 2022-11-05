## class rapidcsv::ConverterToVal< T, USE_NUMERIC_LOCALE, USE_NAN >

Class providing conversion from strings to numerical datatypes. Use specialization for custom datatype conversions.  

---

```c++
template<typename T , int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0> static std::enable_if<0 != USE_NAN, T>::type ToVal (const std::string & pStr)
```
Converts string holding a numerical value to numerical datatype representation. A conversion error would return a value of 'NaN' for particular data-type. 

**Parameters**
- `pStr` string holding a numerical value 

**Returns:**
- output T data-type 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

