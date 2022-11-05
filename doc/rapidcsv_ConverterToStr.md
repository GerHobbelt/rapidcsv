## class rapidcsv::ConverterToStr< T, USE_NUMERIC_LOCALE >

Class providing conversion from numerical datatypes to strings. Use specialization for custom datatype conversions.  

---

```c++
template<typename T , int USE_NUMERIC_LOCALE = 0> static std::enable_if<!std::is_same<T,std::string>::value && !std::is_same<T,char>::value, std::string>::type ToStr (const T & pVal)
```
Converts a numerical value to string. 

**Parameters**
- `pVal` numerical value 

**Returns:**
- string 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

