## class properties::Properties

Datastructure mapping a variable identifier and it's value.  

---

```c++
void AddProperty (const std::string & key, const std::string & value)
```
Adds a new property. If the property already exists, it'll overwrite the old one. 

**Parameters**
- `key` Property name. 
- `value` Property string value. 

---

```c++
template<converter::c_NOT_string T_C>
void AddPropertyConv (const std::string & key,
                      const typename converter::t_T2Sconv_c< T_C >::input_type & value)
```
Adds a new property. If the property already exists, it'll overwrite the old one. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; xOR <br>
            C -> Conversion class satisfying concept 'c_S2Tconverter'.<br>
            For T -> std::string , use function `AddProperty()` instead. 

**Parameters**
- `key` Property name. 
- `value` Property value. By default, R is usually same type as T. <br>
 Else if *`C ≃ converter::ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, else the string value which caused failure during conversion.

---

```c++
std::string GetProperty (const std::string & key)
```
Gets the raw-string property value from a given key. If the value contains any `${inner-key}` , those keys are NOT replaced. 

**Parameters**
- `key` Property name. 

**Returns:**
- property string-value. 

**Exceptions**
- `std::out_of_range` exception when a given key does not exist. 

---

```c++
std::string GetProperty (const std::string & key, const std::string & defaultValue)
```
Gets the raw string property value from a given key. Use a default value if not found. If the value contains any `${inner-key}` , those keys are NOT replaced. 

**Parameters**
- `key` Property name. 
- `defaultValue` Return default value if `key` not found. 

**Returns:**
- property string value. 

**Exceptions**
- `std::out_of_range` exception when a given key does not exist. 

---

```c++
template<converter::c_NOT_string T_C>
typename converter::t_S2Tconv_c<T_C>::return_type
GetPropertyConv (const std::string & key)
```
Gets the type property value from a given key. If the value contains any `${inner-key}` , those keys are NOT replaced. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; xOR <br>
            C -> Conversion class satisfying concept 'c_S2Tconverter'.<br>
            For T -> std::string , use function `AddProperty()` instead. 

**Parameters**
- `key` Property name. 

**Returns:**
- property value of type R. By default, R is usually same type as T.<br>
 Else if *`C ≃ converter::ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*.<br>
 On conversion success variant has the converted value, else the string value which caused failure during conversion. 

**Exceptions**
- `std::out_of_range` exception when a given key does not exist. 

---

```c++
template<converter::c_NOT_string T_C>
typename converter::t_S2Tconv_c<T_C>::return_type
GetPropertyConv (const std::string & key,
                 const typename converter::t_S2Tconv_c< T_C >::return_type & defaultValue)
```
Gets the raw property value from a given key. Use a default value if not found. If the value contains any `${inner-key}` , those keys are NOT replaced. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; xOR <br>
            C -> Conversion class satisfying concept 'c_S2Tconverter'.<br>
            For T -> std::string , use function `AddProperty()` instead. 

**Parameters**
- `key` Property name. 
- `defaultValue` Return default value if `key` not found. 

**Returns:**
- property value of type R. By default, R is usually same type as T.<br>
 Else if *`C ≃ converter::ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*.<br>
 On conversion success variant has the converted value, else the string value which caused failure during conversion. 

**Exceptions**
- `std::out_of_range` exception when a given key does not exist. 

---

```c++
std::string GetPropertyExpanded (const std::string & key)
```
Gets the raw property value from a given key. If the value contains any `${inner-key}` , those keys are replaced with their values. 

**Parameters**
- `key` Property name. 

**Returns:**
- property value in string. 

**Exceptions**
- `std::out_of_range` exception when a given key does not exist.<br>
 `std::length_error` when recursive inner expansion level is more than 10.<br>
 `std::invalid_argument` when closing brace '}' not found in value. 

---

```c++
std::vector<std::string> GetPropertyNames ()
```
Gets the list of property names. 

**Returns:**
- list of property values in string. 

---

```c++
void RemoveProperty (const std::string & key)
```
Removes the property from a given key. 

**Parameters**
- `key` Property name. 

**Exceptions**
- `std::out_of_range` exception when a given key does not exist. 

---