## class rapidcsv::NaNaccess< T, int USE_NAN >

Class declaration providing Not-A-Number (NaN) for numerical datatypes when converting from strings.

---

## class rapidcsv::NaNaccess< T, 0 >

Specialized Class implementation when Not-A-Number (NaN) is disabled.  

---

## class rapidcsv::NaNaccess< T, 1 >

Specialized Class implementation for default Not-A-Number (NaN) values when enabled.  

---

```c++
template<typename T > static std::enable_if<std::is_arithmetic<T>::value, T>::type getNaN ()
```
If 'numeric_limits' provides 'signaling_NaN' retun that else zero. 

**Returns:**
- 'signaling_NaN()' (for floating numbers) OR 'zero'. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

