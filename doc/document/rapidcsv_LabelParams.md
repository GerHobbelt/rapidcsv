## class rapidcsv::LabelParams

Datastructure holding parameters controlling which row and column should be treated as labels.  

---

```c++
explicit LabelParams(const FlgColumnName pColumnNameFlg = FlgColumnName::CN_PRESENT,
                     const FlgRowName    pRowNameFlg    = FlgRowName::RN_MISSING)
```
Constructor. 

**Parameters**
- `pColumnNameFlg` specifies the zero-based row index of the column labels, setting it to _FlgColumnName::CN_MISSING_ prevents column lookup by label name, and gives access to all rows as document data. Default: _FlgColumnName::CN_PRESENT_
- `pRowNameIdx` specifies the zero-based column index of the row labels, setting it to _FlgRowName::RN_MISSING_ prevents row lookup by label name, and gives access to all columns as document data. Default: _FlgRowName::RN_MISSING_

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

