/*
 * PropertiesTest.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_FilterSort
 * Version:  v4.0
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

#include <algorithm>
#include <rapidcsv/Properties.h>
#include "unittest.h"


int main()
{
  int rv = 0;

  try
  {
    //TEST(Properties, TestAddProperty)
    {
        properties::Properties props;
        props.AddProperty("key1", "value1");
        props.AddProperty("key2", "value2");
        props.AddProperty("key3", "value3");
        unittest::ExpectEqual(std::string, "value1", props.GetProperty("key1"));
        unittest::ExpectEqual(std::string, "value2", props.GetProperty("key2"));
        unittest::ExpectEqual(std::string, "value3", props.GetProperty("key3"));
        unittest::ExpectEqual(std::string, "default", props.GetProperty("foo", "default"));
        ExpectException(props.GetProperty("foo"), std::out_of_range);

        std::vector<std::string> names = props.GetPropertyNames();
        unittest::ExpectEqual(size_t, 3, names.size());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key1") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key2") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key3") != names.end());
    }

    //TEST(Properties, TestGetPropertyExpanded)
    {
        properties::Properties props;
        props.AddProperty("key1", "value1");
        props.AddProperty("key2", "${key1}/value2");
        props.AddProperty("key3", "value3/${key2}");
        props.AddProperty("key4", "value4s/${key3}/value4e");
        unittest::ExpectEqual(std::string, "value1", props.GetPropertyExpanded("key1"));
        unittest::ExpectEqual(std::string, "value1/value2", props.GetPropertyExpanded("key2"));
        unittest::ExpectEqual(std::string, "value3/value1/value2", props.GetPropertyExpanded("key3"));
        unittest::ExpectEqual(std::string, "value4s/value3/value1/value2/value4e", props.GetPropertyExpanded("key4"));
        props.AddProperty("key4", "value4s/${key6}/value4e");
        ExpectException(props.GetPropertyExpanded("key4"), std::out_of_range);
        unittest::ExpectEqual(std::string, "value4s/${key6}/value4e", props.GetProperty("key4"));

        std::vector<std::string> names = props.GetPropertyNames();
        unittest::ExpectEqual(size_t, 4, names.size());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key1") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key2") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key3") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key4") != names.end());
    }

    //TEST(Properties, TestGetPropertyExpandedrecurse)
    {
        properties::Properties props;
        props.AddProperty("key1", "value1");
        props.AddProperty("key2", "value2");
        props.AddProperty("key3", "value3");

        props.AddProperty("key231", "${key2}/${key3}/${key1}");
        unittest::ExpectEqual(std::string, "value2/value3/value1", props.GetPropertyExpanded("key231"));

        props.AddProperty("key2313", "${key231}/${key3}");
        unittest::ExpectEqual(std::string, "value2/value3/value1/value3", props.GetPropertyExpanded("key2313"));
    }

    //TEST(Properties, TestAddPropertyOvewrite)
    {
        properties::Properties props;
        props.AddProperty("key1", "value1");
        props.AddProperty("key2", "value2");
        props.AddProperty("key3", "value3");
        props.AddProperty("key3", "value333");
        unittest::ExpectEqual(std::string, "value1", props.GetProperty("key1"));
        unittest::ExpectEqual(std::string, "value2", props.GetProperty("key2"));
        unittest::ExpectEqual(std::string, "value333", props.GetProperty("key3"));
        unittest::ExpectEqual(std::string, "default", props.GetProperty("foo", "default"));
        ExpectException(props.GetProperty("foo"), std::out_of_range);

        std::vector<std::string> names = props.GetPropertyNames();
        unittest::ExpectEqual(size_t, 3, names.size());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key1") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key2") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key3") != names.end());
    }

    //TEST(Properties, TestRemoveProperty)
    {
        properties::Properties props;
        props.AddProperty("key1", "value1");
        props.AddProperty("key2", "value2");
        props.AddProperty("key3", "value3");
        props.RemoveProperty("key3");
        ExpectException(props.RemoveProperty("foo"), std::out_of_range);
        unittest::ExpectEqual(std::string, "value1", props.GetProperty("key1"));
        unittest::ExpectEqual(std::string, "value2", props.GetProperty("key2"));

        std::vector<std::string> names = props.GetPropertyNames();
        unittest::ExpectEqual(size_t, 2, names.size());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key1") != names.end());
        unittest::ExpectTrue(find(names.begin(), names.end(), "key2") != names.end());
    }

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}