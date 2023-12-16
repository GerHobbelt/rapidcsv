## class properties::PropertyFile

For serializing/deserializing `Properties` to/from a file.  

---

```c++
static Properties deserialization (const std::string & filePath)
```
Reconstructing a `Properties` data-structure from a property-file. 

**Parameters**
- `filePath` specifies the path of an existing property-file to populate the `Properties` data with. 

**Returns:**
- object of class `Properties` , a map of name-value pairs. 
- Exceptions:
- `std::format_error` when name-value pair is not in expected format [property_name=property_value] 

---

```c++
static void serialization (const std::string & filePath, const Properties & properties)
```
Writing a `Properties` data-structure to a property-file. 

**Parameters**
- `filePath` specifies the path of a property-file to write the `Properties` data. 
- `properties` object of type `Properties`, whose data is to be serialized to a file. 

---