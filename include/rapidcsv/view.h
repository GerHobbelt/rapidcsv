/*
 * ViewDocument.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_CT/tree/convertor
 * Version:  1.0.ct-8.67
 *
 * Copyright (C) 2022-2022 Gautam Dhar
 * All rights reserved.
 *
 * rapidcsv_CT is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <vector>
#include <iostream>
#include <string>

#include <rapidcsv/rapidcsv.h>

namespace rapidcsv
{
  template<Document::f_EvalBoolExpr evaluateBooleanExpression>
  struct ViewDocument
  {
    explicit ViewDocument(const Document& document)
      : _document(document), _mapViewRowIdx2RowIdx(), _mapRowIdx2ViewRowIdx()
    {
        size_t rowIdx = 0;
        ssize_t viewRowIdx = 0;
        const ssize_t colIdxStart = static_cast<ssize_t>(_document.GetDataColumnIndex(0));
        for (auto itRow = _document.mData.begin() + colIdxStart;
             itRow != document.mData.end(); ++itRow, ++rowIdx)
        {
          if(evaluateBooleanExpression(*itRow))
          {
            _mapViewRowIdx2RowIdx.push_back(rowIdx);
            _mapRowIdx2ViewRowIdx.push_back(viewRowIdx);
            ++viewRowIdx;
          } else {
            _mapRowIdx2ViewRowIdx.push_back(-10);
          }
        }
    }

    /**
     * @brief   Get number of view rows (excluding label rows).
     * @returns view-row count.
     */
    size_t GetViewRowCount() const
    {
      return _mapViewRowIdx2RowIdx.size();
    }

    /**
     * @brief   Get column by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of column data minus the elements filtered out.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetViewColumn(const size_t pColumnIdx,
                                 f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      return _document._GetColumn<T,USE_NUMERIC_LOCALE,USE_NAN,evaluateBooleanExpression>(pColumnIdx, pConvertToVal);
    }

    /**
     * @brief   Get column by name.
     * @param   pColumnName           column label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of column data minus the elements filtered out.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetViewColumn(const std::string& pColumnName,
                                 f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = _document.GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      return GetViewColumn<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(columnIdx), pConvertToVal);
    }

    /**
     * @brief   Get row by view-index.
     * @param   pViewRowIdx           zero-based row view-index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data from view.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetViewRow(const size_t pViewRowIdx,
                              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const size_t rowIdx = _mapViewRowIdx2RowIdx.at(pViewRowIdx);
      return _document.GetRow<T,USE_NUMERIC_LOCALE,USE_NAN>(rowIdx, pConvertToVal);
    }

    /**
     * @brief   Get row by name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data from view.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetViewRow(const std::string& pRowName,
                              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      ssize_t rowIdx = _document.GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      if (_mapRowIdx2ViewRowIdx.at(static_cast<size_t>(rowIdx)) < 0)
      {
        throw std::out_of_range("row filtered out: " + pRowName);
      }

      return _document.GetRow<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(rowIdx), pConvertToVal);
    }

    /**
     * @brief   Get view-cell by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pViewRowIdx           zero-based row view-index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetViewCell(const size_t pColumnIdx, const size_t pViewRowIdx,
                  f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const size_t rowIdx = _mapViewRowIdx2RowIdx.at(pViewRowIdx);
      return _document.GetCell<T,USE_NUMERIC_LOCALE,USE_NAN>(pColumnIdx, rowIdx, pConvertToVal);
    }

    /**
     * @brief   Get cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetViewCell(const std::string& pColumnName, const std::string& pRowName,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t rowIdx = _document.GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      if (_mapRowIdx2ViewRowIdx.at(static_cast<size_t>(rowIdx)) < 0)
      {
        throw std::out_of_range("row filtered out: " + pRowName);
      }

      const ssize_t columnIdx = _document.GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return _document.GetCell<T,USE_NUMERIC_LOCALE,USE_NAN>(
                  static_cast<size_t>(columnIdx), static_cast<size_t>(rowIdx), pConvertToVal);
    }

    /**
     * @brief   Get cell by column name and row index.
     * @param   pColumnName           column label name.
     * @param   pViewRowIdx           zero-based row view-index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetViewCell(const std::string& pColumnName, const size_t pViewRowIdx,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = _document.GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return GetViewCell<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(columnIdx), pViewRowIdx, pConvertToVal);
    }

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetViewCell(const size_t pColumnIdx, const std::string& pRowName,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t rowIdx = _document.GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      if (_mapRowIdx2ViewRowIdx.at(static_cast<size_t>(rowIdx)) < 0)
      {
        throw std::out_of_range("row filtered out: " + pRowName);
      }

      return _document.GetCell<T,USE_NUMERIC_LOCALE,USE_NAN>(pColumnIdx, static_cast<size_t>(rowIdx), pConvertToVal);
    }
  
  private:
    const Document& _document;
    std::vector<size_t> _mapViewRowIdx2RowIdx;
    std::vector<ssize_t> _mapRowIdx2ViewRowIdx;
  };
}