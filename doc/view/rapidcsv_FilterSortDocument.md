## class rapidcsv::FilterSortDocument< evaluateBooleanExpression, SPtypes >

```c++
template<f_EvalBoolExpr evaluateBooleanExpression, c_SortParams ... SPtypes>
FilterSortDocument
```

Class representing a CSV document filtered-sort-view. The underlying 'Document' is viewed after applying filter on rows and then sorted on minimum one column, based on cell values after type-conversion.  

**Template Parameters**
- `evaluateBooleanExpression`  boolean function which determines row filtering.
- `SPtypes`  variadic list of SortParams satisfying concept 'c_SortParams', one for each column to be sorted.
---

```c++
template<typename T_C >
t_S2Tconv_c<T_C>::return_type
GetOrderedCell (const c_sizet_or_string auto & pColumnNameIdx,
                const t_sortKey & pRowKey)
```
Get cell either by it's index or name. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; xOR <br>
            C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowKey` tuple representing indexed-key to data-row. 

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowKey` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<typename T , auto(*)(const std::string &) CONV_S2T>
std::invoke_result_t< decltype(CONV_S2T), const std::string& >
GetOrderedCell (const c_sizet_or_string auto & pColumnNameIdx,
                const t_sortKey & pRowKey)
```
Get cell either by it's index or name. 

**Template Parameters**
- `CONV_S2T`   conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowKey` tuple representing indexed-key to data-row. 

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
 Else if *`CONV_S2T ≃ ConvertFromStr_gNaN<T>::ToVal`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowKey` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<typename ... T_C>
std::tuple<typename t_S2Tconv_c<T_C>::return_type ...>
GetOrderedRow (const typename SortKeyFactory< SPtypes ... >::t_sortKey & pRowKey)
```
Get row either by it's index or name. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ;   xOR <br>
            C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pRowKey` tuple representing indexed-key to data-row. 

**Returns:**
- *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowKey` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
template<auto ... CONV_S2T>
std::tuple< typename f_S2Tconv_c< CONV_S2T >::return_type... >
GetOrderedRow (const typename SortKeyFactory< SPtypes ... >::t_sortKey & pRowKey)
```
Get row either by it's index or name. 

**Template Parameters**
- `CONV_S2T`   conversion function of type 'R (*CONV_S2T)(const std::string&)'. 

**Parameters**
- `pRowNameIdx` row-name or zero-based row index. 

**Returns:**
- *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, <br>
 else the string value which caused failure during conversion. <br>
 If `pRowKey` belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

```c++
std::vector<std::string> GetOrderedRow_VecStr (const typename SortKeyFactory< SPtypes ... >::t_sortKey & pRowKey)
```
Get row either by it's index or name. 

**Parameters**
- `pRowKey` tuple representing indexed-key to data-row. 

**Returns:**
- `vector<std::string>` of row data. <br>
If 'pRowKey' belongs to a filtered out row, then 'out_of_range' error is thrown. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

