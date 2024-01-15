/*
 * Properties.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_FilterSort
 * Version:  v4.0.0
 *
 * Copyright (C) 2022-2023 Gautam Dhar
 * All rights reserved.
 * 
 * rapidcsv_FilterSort is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 *
 * ***********************************************************************************
 *
 * URL:      https://github.com/fredyw/cpp-properties
 * Version:  v1.1.0
 * 
 * Copyright (c) 2014 Fredy Wijaya
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

#include <stdexcept>
//#include <format>  https://stackoverflow.com/questions/61441494/how-to-use-the-format-header
// as of writing this code (Dec-2023) , <format> is supported by 3 compiler-versions

#include <converter/converter.h>
#include <rapidcsv/rapidcsv.h>

namespace properties
{
  /**
   * @brief     Datastructure mapping a variable identifier and it's value.
   */
  class Properties
  {
  public:
    /**
     * @brief   Constructor
    */
    Properties()
      : _keys(),
        _properties()
    {}

    /**
     * @brief   Destructor
    */
    virtual ~Properties()
    {
      _keys.clear();
      _properties.clear();
    }

    /**
     * @brief   Gets the raw-string property value from a given key.
     *          If the value contains any '${inner-key}' , those keys are NOT replaced.
     * @param   key           Property name.
     * @returns property string-value.
     * @throws  `std::out_of_range` exception when a given key does not exist.
     */
    std::string GetProperty(const std::string& key) const
    {
      auto fdIter = _properties.find(key);
      if (fdIter == _properties.end())
      {
        throw std::out_of_range("Properties::GetProperty(key=" + key + ") : key does not exist");
      }
      return fdIter->second;
    }

    /**
     * @brief   Gets the type property value from a given key.
     *          If the value contains any '${inner-key}' , those keys are NOT replaced.
     * @tparam  T_C                   T can be data-type such as int, double etc ;   xOR
     *                                C -> Conversion class satisfying concept 'c_S2Tconverter'.
     *                                For T -> std::string , use function `AddProperty()` instead. 
     * @param   key           Property name.
     * @returns property value of type R. By default, R is usually same type as T.
     *          Else if 'C ≃ converter::ConvertFromStr_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value,
     *          else the string value which caused failure during conversion.
     * @throws  `std::out_of_range` exception when a given key does not exist.
     */
    template<converter::c_NOT_basic_string T_C>
    typename converter::t_S2Tconv_c<T_C>::return_type
    GetPropertyConv(const std::string& key) const
    {
      const std::string strVal = GetProperty(key);
      return converter::t_S2Tconv_c<T_C>::ToVal(strVal);
    }

    /**
     * @brief   Gets the raw string property value from a given key. Use a default value if not found.
     *          If the value contains any '${inner-key}' , those keys are NOT replaced.
     * @param   key           Property name.
     * @param   defaultValue  Return default value if `key` not found.
     * @returns property string value.
     * @throws  `std::out_of_range` exception when a given key does not exist.
     */
    std::string GetProperty(const std::string& key, const std::string& defaultValue) const
    {
      auto fdIter = _properties.find(key);
      if (fdIter == _properties.end())
      {
        return defaultValue;
      }
      return fdIter->second;
    }

    /**
     * @brief   Gets the raw property value from a given key. Use a default value if not found.
     *          If the value contains any '${inner-key}' , those keys are NOT replaced.
     * @tparam  T_C                   T can be data-type such as int, double etc ;   xOR
     *                                C -> Conversion class satisfying concept 'c_S2Tconverter'.
     *                                For T -> std::string , use function `AddProperty()` instead. 
     * @param   key           Property name.
     * @param   defaultValue  Return default value if `key` not found.
     * @returns property value of type R. By default, R is usually same type as T.
     *          Else if 'C ≃ converter::ConvertFromStr_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value,
     *          else the string value which caused failure during conversion.
     * @throws  `std::out_of_range` exception when a given key does not exist.
     */
    template<converter::c_NOT_basic_string T_C>
    typename converter::t_S2Tconv_c<T_C>::return_type
    GetPropertyConv(const std::string& key,
                    const typename converter::t_S2Tconv_c<T_C>::return_type& defaultValue) const
    {
      auto fdIter = _properties.find(key);
      if (fdIter == _properties.end())
      {
        return defaultValue;
      }
      return converter::t_S2Tconv_c<T_C>::ToVal(fdIter->second);
    }

    /**
     * @brief   Gets the raw property value from a given key.
     *          If the value contains any '${inner-key}' ,  those keys are replaced with their values.
     * @param   key           Property name.
     * @returns property value in string.
     * @throws  `std::out_of_range` exception when a given key does not exist.
     *          `std::length_error` when recursive inner expansion level is more than 10.
     *          `std::invalid_argument` when closing brace '}' not found in value.
     */
    inline std::string GetPropertyExpanded(const std::string& key) const
    {
      return _GetPropertyExpanded(key,0);
    }

    /**
     * @brief   Gets the list of property names.
     * @returns list of property values in string.
     */
    inline std::vector<std::string> GetPropertyNames() const
    {
      return _keys;
    }


    /**
     * @brief   Adds a new property. If the property already exists, it'll overwrite the old one.
     * @param   key           Property name.
     * @param   value         Property string value.
     */
    void AddProperty(const std::string& key, const std::string& value)
    {
      auto isInsert = _properties.insert_or_assign(key, value);
      if (isInsert.second)
      {
        _keys.push_back(key);
      }
    }

    /**
     * @brief   Adds a new property. If the property already exists, it'll overwrite the old one.
     * @tparam  T_C                   T can be data-type such as int, double etc ;     xOR
     *                                C -> Conversion class satisfying concept 'c_T2Sconverter'.
     *                                For T -> std::string , use function `AddProperty()` instead. 
     * @param   key           Property name.
     * @param   value         Property value. By default, R is usually same type as T.
     *                        Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                        On conversion success variant has the converted value,
     *                        else the string value which caused failure during conversion.
     */
    template<converter::c_NOT_basic_string T_C>
    inline void AddPropertyConv(const std::string& key,
                                const typename converter::t_T2Sconv_c<T_C>::input_type& value)
    {
      const std::string strValue = converter::t_T2Sconv_c<T_C>::ToStr(value);
      AddProperty(key, strValue);
    }

    /**
     * @brief   Removes the property from a given key.
     * @param   key           Property name.
     * @throws  `std::out_of_range` exception when a given key does not exist.
     */
    void RemoveProperty(const std::string& key)
    {
      std::map<std::string, std::string>::size_type eleSize = _properties.erase(key);
      if (0 == eleSize)
      {
        throw std::out_of_range("Properties::RemoveProperty(key='" + key + "') does not exist");
      }
      _keys.erase(std::remove(_keys.begin(), _keys.end(), key), _keys.end());
    }

  private:
    std::string _GetPropertyExpanded(const std::string& key, const size_t iter) const
    {
      if( iter > 10)
      {
        // to prevent infinite cyclic expansion
        throw std::length_error("Properties::_GetPropertyExpanded() : recursion expansion at key='" + key + "' reached more than 10, which is not supported.");
      }

      std::string val = GetProperty(key);
      std::string::size_type ns = val.find("${");
      while (ns != std::string::npos)
      {
        ns+=2;
        const std::string::size_type ne = val.find("}", ns);
        if (ne != std::string::npos)
        {
          const std::string innerKey = val.substr(ns,(ne-ns));
          const std::string innerVal = _GetPropertyExpanded(innerKey,iter+1);
          val = val.replace(ns-2,(ne-ns)+3,innerVal);
        } else {
          throw std::invalid_argument("Properties::_GetPropertyExpanded() : closing brace '}' not found in value='"+val+"'");
        }
        ns = val.find("${", ns-2);
      }
      return val;
    }

    // to preserve the order
    std::vector<std::string> _keys;
    std::map<std::string, std::string> _properties;
};


  /**
   * @brief     For serializing/deserializing `Properties` to/from a file.
   */
  class PropertyFile
  {
  public:
    /**
     * @brief               Reconstructing a 'Properties' data-structure from a property-file.
     * @param    filePath   specifies the path of an existing property-file to populate the Properties data with.
     * @returns  object of class 'Properties' ,  a map of name-value pairs.
     * @throws   `std::format_error` when name-value pair is not in expected format [property_name=property_value]
     */
    static Properties deserialization(const std::string& filePath)
    {
      rapidcsv::Document propertyFile(filePath,
                                      rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING,
                                                            rapidcsv::FlgRowName::   RN_PRESENT),
                                      rapidcsv::SeparatorParams('=', true, rapidcsv::sPlatformHasCR, false, false),
                                      rapidcsv::LineReaderParams(true, '#', true)
                                     );

      Properties properties;
      for(const std::string& propertyName : propertyFile.GetRowNames())
      {
        std::vector<std::string> propertyValue = propertyFile.GetRow_VecStr(propertyName);
        if(propertyValue.size() != 1)
        {
          // use std::format_error (instead of std::domain_error) , when "#include <format>" gets supported" 
          //throw std::format_error("PropertyFile::deserialization() : In propertyFile("+filePath+") , for property("+propertyValue.at(1)+") : doesn't follow [property_name=property_value] format.");
          throw std::domain_error("PropertyFile::deserialization() : In propertyFile("+filePath+") , for property("+propertyValue.at(1)+") : doesn't follow [property_name=property_value] format.");
        }
        properties.AddProperty(propertyName, propertyValue.at(0));
      }
      return properties;
    }

    /**
     * @brief               Writing a 'Properties' data-structure to a property-file.
     * @param    filePath   specifies the path of a property-file to write the Properties data.
     * @param    properties object of type 'Properties', whose data is to be serialized to a file.
     */
    static void serialization(const std::string& filePath, const Properties& properties)
    {
      rapidcsv::Document propertyFile("",
                                      rapidcsv::LabelParams(rapidcsv::FlgColumnName::CN_MISSING,
                                                            rapidcsv::FlgRowName::   RN_MISSING),
                                      rapidcsv::SeparatorParams('=', true, rapidcsv::sPlatformHasCR, false, false),
                                      rapidcsv::LineReaderParams(true, '#', true)
                                     );

      size_t rowIdx = 0;
      for(const std::string& propertyName : properties.GetPropertyNames())
      {
        const std::string propertyValue = properties.GetProperty(propertyName);
        std::vector<std::string> propertyPair {propertyName, propertyValue};
        propertyFile.SetRow_VecStr(rowIdx, propertyPair);
        ++rowIdx;
      }
      propertyFile.Save(filePath);
    }
  };

} /* namespace properties */

