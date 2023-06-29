## class rapidcsv::SortKeyFactory< SPtypes ...>

```c++
template<c_SortParams ... SPtypes>
SortKeyFactory
```
Factory Class for creating sort keys using list of SortParams.  

**Template Parameters**
- `SPtypes`          variadic list of SortParams, one for each column to be sorted.
---

```c++
static t_sortKey createSortKey (const Document::t_dataRow & pRowData, const SPtypes &... sp)
```
create sortKey for indexing. 

**Parameters**
- `pRowData` data row. vector<string> 
- `sp` sorting information, one for each ordering-column. 

**Returns:**
- tuple representing indexed-key of data-row. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

