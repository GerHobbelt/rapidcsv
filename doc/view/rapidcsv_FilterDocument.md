## class rapidcsv::FilterDocument< evaluateBooleanExpression >

```c++
template<f_EvalBoolExpr evaluateBooleanExpression>
FilterDocument
```

Class representing a CSV document filtered-view. <br>
The underlying 'Document' is viewed after applying filter on rows, based on cell values after type-conversion.  

**Template Parameters**
- `evaluateBooleanExpression`  boolean function which determines row filtering.
---

```c++
FilterDocument (const Document & document)
```
Constructor. After excluding the rows as defined by 'evaluateBooleanExpression', creates bi-directional map between view-rows and actual-csv-rows. 

**Parameters**
- `document` 'Document' object with CSV data. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

