## class converter::ConvertFromTuple< T_C >

```c++
template<typename ... T_C>
struct ConvertFromTuple
```
Convertor class implementation for tuple type, with underlying elements(of different types) individually converted to string.  

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; xOR <br>
            C -> Conversion class statisfying concept 'c_S2Tconverter'. 

---

```c++
static std::string ToStr (std::tuple< typename t_T2Sconv< T_C >::input_type ... > const & theTuple)
```
Converts variable holding 'tuple' value to string. <br>
The output string has the format '[ ele0, ele1, ele2 ...]'. <br>
Each element of the tuple is converted to string using 't_T2Sconv<T_C>::ToStr(...)'. 

**Parameters**
- `theTuple` input tuple. 

**Returns:**
- string. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

