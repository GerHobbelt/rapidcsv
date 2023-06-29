## class rapidcsv::_RowComparator< SPtypes ... >

```c++
template<c_SortParams ... SPtypes>
struct _RowComparator
```
Comparator Class for sortKey. Comparision algorith is defined here.  

**Template Parameters**
- `SPtypes`          variadic list of SortParams, one for each column to be sorted.
---

```c++
bool operator() (const typename SortKeyFactory< SPtypes ... >::t_sortKey & lhCompositeKey,
                 const typename SortKeyFactory< SPtypes ... >::t_sortKey & rhCompositeKey)
```
Compare two index keys. 

**Parameters**
- `lhCompositeKey` key to be compared. 
- `rhCompositeKey` key to be compared. 

**Returns:**
- bool value based on key-columns and ordering specification of each column. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

