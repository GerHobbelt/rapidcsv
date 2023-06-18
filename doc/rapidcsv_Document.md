## class rapidcsv::Document

Class representing a CSV document.  

---

```c++
Document (const std::string & pPath = std::string(),
          const LabelParams & pLabelParams = LabelParams(),
          const SeparatorParams & pSeparatorParams = SeparatorParams(),
          const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Constructor. 

**Parameters**
- `pPath` specifies the path of an existing CSV-file to populate the Document data with. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
Document (std::istream & pStream,
          const LabelParams & pLabelParams = LabelParams(),
          const SeparatorParams & pSeparatorParams = SeparatorParams(),
          const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Constructor. 

**Parameters**
- `pStream` specifies a binary input stream to read CSV data from. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
void Clear ()
```
Clears loaded Document data. 

---

```c++
template<typename T_C >
t_S2Tconv<T_C>::return_type
GetCell (const c_sizet_or_string auto & pColumnNameIdx,
         const c_sizet_or_string auto & pRowNameIdx)
```
Get cell either by it's index or name.

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowNameIdx` column-name or zero-based row-index. 

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
  Else if *`C = ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
  On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T , auto(*)(const std::string &) CONV_S2T>
std::invoke_result_t< decltype(CONV_S2T), const std::string& >
GetCell (const c_sizet_or_string auto & pColumnNameIdx,
         const c_sizet_or_string auto & pRowNameIdx)
```
Get cell either by it's index or name. 
Template Parameters:
- `T`             'type' converted to, from string data, using conversion function. 
- `CONV_S2T`      conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowNameIdx` column-name or zero-based row-index. 

**Returns:**
- cell data of type R. By default, R is usually same type as T. <br>
  Else if CONV_S2T similar to function *`ConvertFromStr_gNaN<T>::ToVal`*, then *`R = std::variant<T, std::string>`*. <br>
  On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T_C >
std::vector<typename t_S2Tconv<T_C>::return_type>
GetColumn (const c_sizet_or_string auto & pColumnNameIdx)
```
Get column either by it's index or name.

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 

**Returns:**
- *`vector<R>`* of column data. By default, R is usually same type as T. <br>
  Else if *`C = ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
  On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T , auto(*)(const std::string &) CONV_S2T>
std::vector< typename std::invoke_result_t< decltype(CONV_S2T), const std::string& > >
GetColumn (const c_sizet_or_string auto & pColumnNameIdx)
```
Get column either by it's index or name.

**Template Parameters**
- `T`                     'type' converted to, from string data, using conversion function. 
- `CONV_S2T`               conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 

**Returns:**
- *`vector<R>`* of column data. By default, R is usually same type as T. <br>
  Else if CONV_S2T is similar to function *`ConvertFromStr_gNaN<T>::ToVal`*, then *`R = std::variant<T, std::string>`*. <br>
  On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
size_t GetColumnCount ()
```
Get number of data columns (excluding label columns). 

**Returns:**
- column count. 

---

```c++
size_t GetColumnIdx (const std::string & pColumnName)
```
Get column index by name. 

**Parameters**
- `pColumnName` column label name. 

**Returns:**
- zero-based column index. 

---

```c++
std::string GetColumnName (const size_t pColumnIdx)
```
Get column name. 

**Parameters**
- `pColumnIdx` zero-based column index. 

**Returns:**
- column name. 

---

```c++
std::vector<std::string> GetColumnNames ()
```
Get column names. 

**Returns:**
- vector of column names. 

---

```c++
template<typename ... T_C>
std::tuple<typename t_S2Tconv<T_C>::return_type ...>
GetRow (const c_sizet_or_string auto & pRowNameIdx)
```
Get row either by it's index or name.

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_S2Tconverter'. 

**Parameters**
- `pRowNameIdx` row-name or zero-based row index. 

**Returns:**
- *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
  Else if *`C = ConvertFromStr_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. 

---

```c++
std::vector<std::string>
GetRow_VecStr (const c_sizet_or_string auto & pRowNameIdx)
```
Get row either by it's index or name. 

**Parameters**
- `pRowNameIdx` row-name or zero-based row index. 

**Returns:**
- *`vector<std::string>`* of row data 

---

```c++
size_t GetRowCount ()
```
Get number of data rows (excluding label rows). 

**Returns:**
- row count. 

---

```c++
size_t GetRowIdx (const std::string & pRowName)
```
Get row index by name. 

**Parameters**
- `pRowName` row label name. 

**Returns:**
- zero-based row index. 

---

```c++
std::string GetRowName (const size_t pRowIdx)
```
Get row name. 

**Parameters**
- `pRowIdx` zero-based column index. 

**Returns:**
- row name. 

---

```c++
std::vector<std::string> GetRowNames ()
```
Get row names. 

**Returns:**
- vector of row names. 

---

```c++
template<typename T_C >
void InsertColumn (const size_t pColumnIdx,
                   const std::vector< typename t_T2Sconv< T_C >::input_type > & pColumn
                       = std::vector< typename t_T2Sconv< T_C >::input_type>(),
                   const std::string & pColumnName = std::string())
```
Insert column at specified index.

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_T2Sconverter'. 

**Parameters**
- `pColumnIdx`      zero-based column index. 
- `pColumn`         *`vector<R>`* of column data (optional argument). By default, R is usually same type as T. <br>
                    Else if *`C = ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
                    On conversion success variant has the converted value, else the string value which caused failure during conversion. 
- `pColumnName`     column label name (optional argument).

---

```c++
template<typename T , typename R , std::string(*)(const R &) CONV_T2S>
void InsertColumn (const size_t pColumnIdx,
                   const std::vector< R > & pColumn = std::vector<R>(),
                   const std::string & pColumnName = std::string())
```
Insert column at specified index. 
Template Parameters:
- `T` 'type' converted from, to string data, using conversion function. 
- `R` can be T or *`std::variant<T, std::string>`*. <br>
      On conversion success variant has the converted value, else the string value which caused failure during conversion. 
- `CONV_T2S` conversion function. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pColumn` *`vector<R>`* of column data (optional argument). By default, R is usually same type as T. <br>
            Else if CONV_T2S is similar to function *`ConvertFromVal_gNaN<T>::ToStr`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 
- `pColumnName` column label name (optional argument) 

---

```c++
template<typename T , std::string(*)(const T &) CONV_T2S>
void InsertColumn (const size_t pColumnIdx,
                   const std::vector< T > & pColumn = std::vector<T>(),
                   const std::string & pColumnName = std::string())
```
Insert column at specified index.

**Template Parameters**
- `T`            'type' converted from, to string data, using conversion function. 
- `CONV_T2S`     conversion function. 

**Parameters**
- `pColumnIdx`         zero-based column index. 
- `pColumn`            *`vector<T>`* of column data (optional argument). 
- `pColumnName`        column label name (optional argument) 

---

```c++
template<typename ... T_C>
void InsertRow (const size_t pRowIdx,
                const std::tuple< typename t_T2Sconv< T_C >::input_type ... > & pRow
                    = std::tuple< typename t_T2Sconv< T_C >::input_type ...>(),
                const std::string & pRowName = std::string())
```
Insert row at specified index.

**Template Parameters:**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_T2Sconverter'. 

**Parameters**
- `pRowIdx`         zero-based row index. 
- `pRow`            *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
                    Else if *`C = ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
                    On conversion success variant has the converted value, else the string value which caused failure during conversion. 
- `pRowName`        row label name (optional argument). 

---

```c++
void InsertRow_VecStr (const size_t pRowIdx,
                       const std::vector< std::string > & pRow = std::vector< std::string >(),
                       const std::string & pRowName = std::string())
```
Insert row at specified index. 

**Parameters**
- `pRowIdx`         zero-based row index. 
- `pRow`            *`vector<string>`* of row data (optional argument). 
- `pRowName`        row label name (optional argument). 

---

```c++
void Load (const std::string & pPath,
           const LabelParams & pLabelParams = LabelParams(),
           const SeparatorParams & pSeparatorParams = SeparatorParams(),
           const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Read Document data from file. 

**Parameters**
- `pPath` specifies the path of an existing CSV-file to populate the Document data with. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
void Load (std::istream & pStream,
           const LabelParams & pLabelParams = LabelParams(),
           const SeparatorParams & pSeparatorParams = SeparatorParams(),
           const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Read Document data from stream. 

**Parameters**
- `pStream` specifies a binary input stream to read CSV data from. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
void RemoveColumn (const c_sizet_or_string auto & pColumnNameIdx)
```
Remove column either by it's index or name. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 

---

```c++
void RemoveRow (const c_sizet_or_string auto & pRowNameIdx)
```
Remove row either by it's index or name. 

**Parameters**
- `pRowNameIdx` row-name or zero-based row-index. 

---

```c++
void Save (const std::string & pPath = std::string())
```
Write Document data to file. 

**Parameters**
- `pPath` optionally specifies the path where the CSV-file will be created (if not specified, the original path provided when creating or loading the Document data will be used). 

---

```c++
void Save (std::ostream & pStream)
```
Write Document data to stream. 

**Parameters**
- `pStream` specifies a binary output stream to write the data to. 

---

```c++
template<typename T_C >
void SetCell (const c_sizet_or_string auto & pColumnNameIdx,
              const c_sizet_or_string auto & pRowNameIdx,
              const typename t_T2Sconv< T_C >::input_type & pCell)
```
Set cell either by it's index or name.

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_T2Sconverter'. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowNameIdx` column-name or zero-based row-index. 
- `pCell` cell data. By default, R is usually same type as T. <br>
            Else if *`C = ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T , typename R , std::string(*)(const R &) CONV_T2S>
void SetCell (const c_sizet_or_string auto & pColumnNameIdx,
              const c_sizet_or_string auto & pRowNameIdx,
              const R & pCell)
```
Set cell either by it's index or name.

**Template Parameters**
- `T` 'type' converted from, to string data, using conversion function. 
- `R` can be T or *`std::variant<T, std::string>`*. <br>
      On conversion success variant has the converted value, else the string value which caused failure during conversion. 
- `CONV_T2S` conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowNameIdx` column-name or zero-based row-index. 
- `pCell` cell data. By default, R is usually same type as T. <br>
            Else if CONV_T2S is similar to function *`ConvertFromVal_gNaN<T>::ToStr`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T , std::string(*)(const T &) CONV_T2S>
void SetCell (const c_sizet_or_string auto & pColumnNameIdx,
              const c_sizet_or_string auto & pRowNameIdx, const T & pCell)
```
Set cell either by it's index or name.

**Template Parameters**
- `T` 'type' converted from, to string data, using conversion function. 
- `CONV_T2S` conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pRowNameIdx` column-name or zero-based row-index. 
- `pCell` cell data. By default, R is usually same type as T. <br>
            Else if CONV_T2S is similar to function *`ConvertFromVal_gNaN<T>::ToStr`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T_C >
void SetColumn (const c_sizet_or_string auto & pColumnNameIdx,
                const std::vector< typename t_T2Sconv< T_C >::input_type > & pColumn)
```
Set column either by it's index or name.

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_T2Sconverter'. 

**Parameters**
- `pColumnNameIdx`     column-name or zero-based column-index. 
- `pColumn`            *`vector<R>`* of column data. By default, R is usually same type as T. <br>
                       Else if *`C = ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
                       On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T , typename R , std::string(*)(const R &) CONV_T2S>
void SetColumn (const c_sizet_or_string auto & pColumnNameIdx,
                const std::vector< R > & pColumn)
```
Set column either by it's index or name.

**Template Parameters**
- `T` 'type' converted from, to string data, using conversion function. 
- `R` can be T or *`std::variant<T, std::string>`*. <br>
      On conversion success variant has the converted value, else the string value which caused failure during conversion. 
- `CONV_T2S` conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pColumn` *`vector<R>`* of column data. By default, R usually is same type as T. <br>
            Else if CONV_T2S is similar to function *`ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
template<typename T , std::string(*)(const T &) CONV_T2S>
void SetColumn (const c_sizet_or_string auto & pColumnNameIdx,
                const std::vector< T > & pColumn)
```
Set column either by it's index or name.

**Template Parameters**
- `T` 'type' converted from, to string data, using conversion function. 
- `CONV_T2S` conversion function. 

**Parameters**
- `pColumnNameIdx` column-name or zero-based column-index. 
- `pColumn` *`vector<R>`* of column data. By default, R usually is same type as T. <br>
            Else if CONV_T2S is similar to function *`ConvertFromVal_gNaN<T>:ToStr`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
void SetColumnName (size_t pColumnIdx, const std::string & pColumnName)
```
Set column name. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pColumnName` column name. 

---

```c++
template<typename ... T_C>
void SetRow (const c_sizet_or_string auto & pRowNameIdx,
             const std::tuple< typename t_T2Sconv< T_C >::input_type ... > & pRow)
```
Set row either by it's index or name. 

**Template Parameters**
- `T_C`    T can be data-type such as int, double etc ; <br>
            XOR C -> Conversion class statisfying concept 'c_T2Sconverter'. 

**Parameters**
- `pRowNameIdx` row-name or zero-based row index. 
- `pRow` *`tuple<R...>`* of row data. By default, R is usually same type as T. <br>
            Else if *`C = ConvertFromVal_gNaN<T>`*, then *`R = std::variant<T, std::string>`*. <br>
            On conversion success variant has the converted value, else the string value which caused failure during conversion. 

---

```c++
void SetRow_VecStr (const c_sizet_or_string auto & pRowNameIdx,
                    const std::vector< std::string > & pRow)
```
Set row either by it's index or name. 

**Parameters**
- `pRowNameIdx` row-name or zero-based row index. 
- `pRow` *`vector<string>`* of row data. 

---

```c++
void SetRowName (size_t pRowIdx, const std::string & pRowName)
```
Set row name. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pRowName` row name. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

