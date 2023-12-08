## class rapidcsv::SortParams< T_C, SORT_ORDER >

```c++
template<typename T_C , e_SortOrder SORT_ORDER = e_SortOrder::ASCEND>
SortParams;
```
Class representing sort/ordering parameters of CSV document.

**Template Parameters**
- `T_C`          type info of the column to be sorted. <br>
                  T can be data-type such as int, double etc ; xOR <br>
                  C -> Conversion class satisfying concept *`c_S2Tconverter`*.
- `SORT_ORDER`    ascending or descending order. Default ascending.

---

```c++
SortParams (const size_t rawDataColumnIndex)
```
Constructor.

**Parameters**
- `rawDataColumnIndex` Specifies the column index in CSV file.

---

```c++
S2Tconv_type::return_type
getValue (const Document::t_dataRow & pRowData)
```
Gets cell-data(convertedfrom string) from Data-row.

**Parameters**
- `pRowData` data row. vector<string>

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
 Else if *`C ≃ ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
 On conversion success variant has the converted value, else the string value which caused failure during conversion.

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

