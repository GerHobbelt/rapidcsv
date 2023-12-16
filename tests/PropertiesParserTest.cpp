/*
 * PropertiesParserTest.h
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

#include <cstdio>
#include <algorithm>
#include <stdexcept>

#include <rapidcsv/Properties.h>

#include "unittest.h"

int main(int argc, char* argv[])
{
  int rv = 0;

  try
  {
    if(argc != 2)
    {
      throw std::invalid_argument("expected one argument as path to properties-folder");
    }

    const std::string propertiesPath{argv[1]};
    //TEST(PropertiesParser, TestRead)
    {
      properties::Properties props = properties::PropertyFile::deserialization(propertiesPath + "/test1.properties");
      unittest::ExpectEqual(std::string, "foo", props.GetProperty("name1"));
      unittest::ExpectEqual(std::string, "hello", props.GetProperty("msg1"));

      unittest::ExpectEqual(std::string, "bar", props.GetProperty("name2"));
      unittest::ExpectEqual(std::string, "bye", props.GetProperty("msg2"));

      unittest::ExpectEqual(std::string, "dname1", props.GetProperty("dir1"));
      unittest::ExpectEqual(std::string, "dname2", props.GetProperty("dir2"));
      unittest::ExpectEqual(std::string, "dname3", props.GetProperty("dir3"));
      unittest::ExpectEqual(std::string, "dname1/dname2/dname3", props.GetPropertyExpanded("dir123"));
      unittest::ExpectEqual(std::string, "dname1/dname2/dname3/dname1/dname2/dname3", props.GetPropertyExpanded("dir123123"));
    }

    //TEST(PropertiesParser, TestReadInvalidFile)
    {
      ExpectException(properties::PropertyFile::deserialization(propertiesPath + "/test2.properties"), std::out_of_range);
    }

    //TEST(PropertiesParser, TestReadFileNotFound)
    {
      ExpectException(properties::PropertyFile::deserialization("somewhere"), std::ios_base::failure);
    }

    //TEST(PropertiesParser, TestWrite)
    {
      properties::Properties inProps;
      inProps.AddProperty("key1", " value1");
      inProps.AddProperty("key2", "value2   ");
      inProps.AddProperty("key3", "value3");
      inProps.AddProperty("key4", "value4");
      inProps.AddProperty("key5", " value5 ");  // test : trim leading and trailing spaces

      properties::PropertyFile::serialization(propertiesPath + "/out.properties", inProps);

      properties::Properties outProps = properties::PropertyFile::deserialization(propertiesPath + "/out.properties");

      std::vector<std::string> names = outProps.GetPropertyNames();
      unittest::ExpectEqual(size_t, 5, names.size());
      unittest::ExpectEqual(std::string, "value1", outProps.GetProperty("key1"));
      unittest::ExpectEqual(std::string, "value2", outProps.GetProperty("key2"));
      unittest::ExpectEqual(std::string, "value3", outProps.GetProperty("key3"));
      unittest::ExpectEqual(std::string, "value4", outProps.GetProperty("key4"));
      unittest::ExpectEqual(std::string, "value5", outProps.GetProperty("key5"));
    }

  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}