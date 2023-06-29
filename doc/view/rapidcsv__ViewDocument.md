## class rapidcsv::_ViewDocument

Class representing a CSV document view. The underlying 'Document' is viewed after applying filter and/or sort on rows, based on cell values after type-conversion. Only intended for internal usage, but exposed externally to allow specialization on datatypes.  

---

```c++
size_t GetDocumentRowIdx (const std::string & pRowName)
```
Get actual document-row index by name. 

**Parameters**
- `pRowName` row label name. 

**Returns:**
- zero-based row index. This is the row-index as seen in the underlying CSV file. <br>
 If `pRowName` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
size_t GetDocumentRowIdx (const size_t & pViewRowIdx)
```
Get actual document-row index by viewRowId. 

**Parameters**
- `pViewRowIdx` row label name. 

**Returns:**
- zero-based row index. This is the row-index as seen in the underlying CSV file. <br>
 If `pRowName` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<typename T_C >
t_S2Tconv_c<T_C>::return_type
GetViewCell (const c_sizet_or_string auto & pColumnNameIdx,
             const c_sizet_or_string auto & pRowName_ViewRowIdx)
```
Get cell by column index and row-name/zero-based-row-view-index. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; xOR <br>
            C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column index. 
- `pRowName_ViewRowIdx` row-name or zero-based row-view-index(i.e index after removing filtered rows). 

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowName_ViewRowIdx` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<auto(*)(const std::string &) CONV_S2T>
f_S2Tconv_c< CONV_S2T >::return_type
GetViewCell (const c_sizet_or_string auto & pColumnNameIdx,
             const c_sizet_or_string auto & pRowName_ViewRowIdx)
```
Get cell by column index and row-name/zero-based-row-view-index. 

**Template Parameters**
- `CONV_S2T`   conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column index. 
- `pRowName_ViewRowIdx` row-name or zero-based row-view-index(i.e index after removing filtered rows). 

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
 Else if *`CONV_S2T ≃ ConvertFromStr_gNaN<T>::ToVal`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowName_ViewRowIdx` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<typename T_C >
std::vector<typename t_S2Tconv_c<T_C>::return_type>
GetViewColumn (const c_sizet_or_string auto & pColumnNameIdx)
```
Get column by index. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ;   xOR <br>
            C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 

**Returns:**
- *`vector<R>`* of column data, excluding the elements filtered out. <br>
 By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. 

---

```c++
template<auto(*)(const std::string &) CONV_S2T>
std::vector< typename f_S2Tconv_c< CONV_S2T >::return_type >
GetViewColumn (const c_sizet_or_string auto & pColumnNameIdx)
```
Get column either by it's index or name. 

**Template Parameters**
- `CONV_S2T`   conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 

**Returns:**
- *`vector<R>`* of column data, excluding the elements filtered out. <br>
 By default, R is usually same type as T. <br>
 Else if *`CONV_S2T ≃ ConvertFromStr_gNaN<T>::ToVal`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. 

---

```c++
template<typename ... T_C>
std::tuple<typename t_S2Tconv_c<T_C>::return_type ...>
GetViewRow (const c_sizet_or_string auto & pRowName_ViewRowIdx)
```
Get row either by it's index or name. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ;   xOR <br>
            C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pRowName_ViewRowIdx` row-name or zero-based row-view-index(i.e index after removing filtered rows). 

**Returns:**
- *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowName_ViewRowIdx` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<auto ... CONV_S2T>
std::tuple< typename f_S2Tconv_c< CONV_S2T >::return_type ... >
 GetViewRow (const c_sizet_or_string auto & pRowName_ViewRowIdx)
```
Get row either by it's index or name. 

**Template Parameters**
- `CONV_S2T`   conversion function of type 'R (*CONV_S2T)(const std::string&)'. 

**Parameters**
- `pRowName_ViewRowIdx` row-name or zero-based row-view-index(i.e index after removing filtered rows). 

**Returns:**
- *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowName_ViewRowIdx` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
std::vector<std::string> GetViewRow_VecStr (const c_sizet_or_string auto & pRowName_ViewRowIdx)
```
Get row either by it's index or name. 

**Parameters**
- `pRowName_ViewRowIdx` row-name or zero-based row-view-index(i.e index after removing filtered rows). 

**Returns:**
- `vector<string>` of row data from view. <br>
  If `pRowName_ViewRowIdx` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
size_t GetViewRowCount ()
```
Get number of view rows (excluding label rows). 

**Returns:**
- view-row count. 

---

```c++
size_t GetViewRowIdx (const std::string & pRowName)
```
Get view-row index by name. 

**Parameters**
- `pRowName` row label name. 

**Returns:**
- zero-based row index. After removal of all filtered rows, the new row-index of remaining rows. <br>
  If `pRowName` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

