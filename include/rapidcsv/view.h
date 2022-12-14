/*
 * ViewDocument.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_FilterSort
 * Version:  2.0.fs-8.68
 *
 * Copyright (C) 2022-2022 Gautam Dhar
 * All rights reserved.
 *
 * rapidcsv_CT is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <string>

#include <rapidcsv/rapidcsv.h>

namespace rapidcsv
{
  enum e_SortOrder
  {
    ASCEND = 0,
    DESCEND
  };

  template<typename T>
  class SortParams
  {
  public:
    const size_t rawDataColumnIndex;
    const f_ConvFuncToVal<T> convFuncToVal;
    const e_SortOrder sortOrder;

    constexpr SortParams(const size_t RAW_DATA_COLUMN_INDEX,
                         const f_ConvFuncToVal<T> pConvFuncToVal,
                         const e_SortOrder SORT_ORDER = e_SortOrder::ASCEND) :
      rawDataColumnIndex(RAW_DATA_COLUMN_INDEX),
      convFuncToVal(pConvFuncToVal),
      sortOrder(SORT_ORDER)
    {
    }

    T getValue(const Document::t_dataRow& row) const
    {
      const std::string& cellStr = row.at(rawDataColumnIndex);
      return convFuncToVal(cellStr);
    }
  };

  template<typename T, typename... Types>
  class RowIndex
  {
  public:
    const T value;
    const RowIndex<Types...> nextIndex;

    template<typename... SPtypes>
    RowIndex(const Document::t_dataRow& row,
             const SortParams<T>& pSortParams,
             const SPtypes & ... spArgs)
      : value(pSortParams.getValue(row)),
      nextIndex(row, spArgs...)
    {
    }

    bool operator < (const RowIndex<T, Types...>& other) const
    {
      if (value < other.value)
      {
        return true;
      }
      if (value > other.value)
      {
        return false;
      }

      return nextIndex < other.nextIndex;
    }

    bool operator > (const RowIndex<T, Types...>& other) const
    {
      if (value > other.value)
      {
        return true;
      }
      if (value < other.value)
      {
        return false;
      }

      return nextIndex > other.nextIndex;
    }
  };

  template<typename T>
  class RowIndex<T> // Template base case
  {
  public:
    const T value;

    RowIndex(const Document::t_dataRow& row,
             const SortParams<T>& pSortParams)
      : value(pSortParams.getValue(row))
    {
    }

    inline bool operator < (const RowIndex<T>& other) const
    {
      return value < other.value;
    }

    inline bool operator > (const RowIndex<T>& other) const
    {
      return value > other.value;
    }
  };

  template<typename T, typename... Types>
  class RowComparator
  {
  private:
    const RowComparator<Types...> _nextColumnInfo;

    typedef bool (RowComparator<T, Types...>::* tf_sortOrder)
      (const RowIndex<T, Types...>& lhVal,
      const RowIndex<T, Types...>& rhVal) const;

    const tf_sortOrder _sortOrder;

    bool _ascendingOrder(const RowIndex<T, Types...>& lhVal,
                         const RowIndex<T, Types...>& rhVal) const
    {
      if (lhVal.value < rhVal.value)
      {
        return true;
      }
      if (lhVal.value > rhVal.value)
      {
        return false;
      }

      // lhVal == rhVal
      return _nextColumnInfo(lhVal.nextIndex, rhVal.nextIndex);
    }

    bool _descendingOrder(const RowIndex<T, Types...>& lhVal,
                          const RowIndex<T, Types...>& rhVal) const
    {
      if (lhVal.value > rhVal.value)
      {
        return true;
      }
      if (lhVal.value < rhVal.value)
      {
        return false;
      }

      // lhVal == rhVal
      return _nextColumnInfo(lhVal.nextIndex, rhVal.nextIndex);
    }

  public:
    template<typename... SPtypes>
    RowComparator(const SortParams<T>& pSortParams,
                  const SPtypes & ... spArgs)
      : _nextColumnInfo(spArgs...),
      _sortOrder((e_SortOrder::ASCEND == pSortParams.sortOrder) ?
                 &RowComparator<T, Types...>::_ascendingOrder :
                 &RowComparator<T, Types...>::_descendingOrder)
    {
    }

    bool operator () (const RowIndex<T, Types...>& lhVal,
                      const RowIndex<T, Types...>& rhVal) const
    {
      return (this->*_sortOrder)(lhVal, rhVal);
    }
  };

  template<typename T>
  class RowComparator<T> // Template base case
  {
  private:

    typedef bool (RowComparator<T>::* tf_sortOrder)
      (const RowIndex<T>& lhVal,
      const RowIndex<T>& rhVal) const;

    const tf_sortOrder _sortOrder;

    bool _ascendingOrder(const RowIndex<T>& lhVal,
                         const RowIndex<T>& rhVal) const
    {
      return lhVal.value < rhVal.value;
    }

    bool _descendingOrder(const RowIndex<T>& lhVal,
                          const RowIndex<T>& rhVal) const
    {
      return lhVal.value > rhVal.value;
    }

  public:
    RowComparator(const SortParams<T>& pSortParams)
      : _sortOrder((e_SortOrder::ASCEND == pSortParams.sortOrder) ?
                   &RowComparator<T>::_ascendingOrder :
                   &RowComparator<T>::_descendingOrder)
    {
    }

    bool operator () (const RowIndex<T>& lhVal, const RowIndex<T>& rhVal) const
    {
      return (this->*_sortOrder)(lhVal, rhVal);
    }
  };

  template<Document::f_EvalBoolExpr evaluateBooleanExpression, typename... Types>
  class ViewDocument
  {
  protected:
    explicit ViewDocument(const Document& document)
      : _document(document), _mapViewRowIdx2RowIdx(), _mapRowIdx2ViewRowIdx()
    {
      size_t rowIdx = 0;
      ssize_t viewRowIdx = 0;
      const ssize_t rowIdxStart = static_cast<ssize_t>(_document.GetDataRowIndex(0).dataIdx);
      for (auto itRow = _document.mData.begin() + rowIdxStart;
           itRow != _document.mData.end(); ++itRow, ++rowIdx)
      {
        if (evaluateBooleanExpression(*itRow))
        {
          _mapViewRowIdx2RowIdx.push_back(rowIdx);
          _mapRowIdx2ViewRowIdx.push_back(viewRowIdx);
          ++viewRowIdx;
        }
        else
        {
          _mapRowIdx2ViewRowIdx.push_back(-10);
        }
      }
    }

    template<typename... SPtypes>
    explicit ViewDocument(const Document& document, const SPtypes & ... spArgs)
      : _document(document), _mapViewRowIdx2RowIdx(), _mapRowIdx2ViewRowIdx()
    {
      RowComparator<Types...> sortPredicate(spArgs...);
      std::map<const RowIndex<Types...>, size_t, RowComparator<Types...>> sortedData(sortPredicate);
      size_t rowIdx = 0;
      const ssize_t rowIdxStart = static_cast<ssize_t>(_document.GetDataRowIndex(0).dataIdx);
      for (auto itRow = _document.mData.begin() + rowIdxStart;
           itRow != _document.mData.end(); ++itRow, ++rowIdx)
      {
        if (evaluateBooleanExpression(*itRow))
        {
          RowIndex<Types...> rowIndexValues((*itRow), spArgs...);
          // std::cout << "rowIndexValues.value = " << rowIndexValues.value << std::endl;
          sortedData[rowIndexValues] = rowIdx;
        }
        _mapRowIdx2ViewRowIdx.push_back(-10);
      }

      ssize_t viewRowIdx = 0;
      for (auto itMap = sortedData.begin();
           itMap != sortedData.end(); ++itMap, ++viewRowIdx)
      {
        rowIdx = itMap->second;
        _mapViewRowIdx2RowIdx.push_back(rowIdx);
        _mapRowIdx2ViewRowIdx.at(rowIdx) = viewRowIdx;
        // std::cout << "rowIdx("<< rowIdx <<") <-> viewRowIdx(" << viewRowIdx << ")" << std::endl;
      }
    }

  public:
    /**
     * @brief   Get number of view rows (excluding label rows).
     * @returns view-row count.
     */
    inline size_t GetViewRowCount() const
    {
      return _mapViewRowIdx2RowIdx.size();
    }

    /**
     * @brief   Get column by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of column data minus the elements filtered out.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    std::vector<T> GetViewColumn(const size_t pColumnIdx,
                                 f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T, USE_NUMERIC_LOCALE,
                                                                                   USE_NAN>::ToVal) const
    {
      const size_t dataColumnIdx = _document.GetDataColumnIndex(pColumnIdx).dataIdx;
      const size_t firstRowIdx = _document.GetDataRowIndex(0).dataIdx;
      std::vector<T> column;
      for (auto itViewRowIdx = _mapViewRowIdx2RowIdx.begin();
           itViewRowIdx != _mapViewRowIdx2RowIdx.end(); ++itViewRowIdx)
      {
        const size_t rowIdx = firstRowIdx + static_cast<size_t>(*itViewRowIdx);
        const rapidcsv::Document::t_dataRow& row = _document.mData.at(rowIdx);
        if (dataColumnIdx < row.size())
        {
          const std::string& cellStrVal = row.at(dataColumnIdx);
          T val = pConvertToVal(cellStrVal);
          column.push_back(val);
        }
        else
        {
          const std::string errStr = "ViewDocument::GetViewColumn() # requested column index " +
            std::to_string(pColumnIdx) + " >= " +
            std::to_string(row.size() - _document.GetDataColumnIndex(0).dataIdx) +
            " (number of columns on row index " + std::to_string(rowIdx) + ")";
          throw std::out_of_range(errStr);
        }
      }
      return column;
    }

    /**
     * @brief   Get column by name.
     * @param   pColumnName           column label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of column data minus the elements filtered out.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    std::vector<T> GetViewColumn(const std::string& pColumnName,
                                 f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T, USE_NUMERIC_LOCALE,
                                                                                   USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = _document.GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      return GetViewColumn<T, USE_NUMERIC_LOCALE, USE_NAN>(static_cast<size_t>(columnIdx), pConvertToVal);
    }

    /**
     * @brief   Get view-row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    size_t GetViewRowIdx(const std::string& pRowName) const
    {
      ssize_t rowIdx = _document.GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      size_t row_idx = static_cast<size_t>(rowIdx);
      ssize_t viewRowIdx = _mapRowIdx2ViewRowIdx.at(static_cast<size_t>(row_idx));
      if (viewRowIdx < 0)
      {
        throw std::out_of_range("row filtered out: " + pRowName);
      }

      return static_cast<size_t>(viewRowIdx);
    }

    /**
     * @brief   Get document-row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    size_t GetDocumentRowIdx(const std::string& pRowName) const
    {
      ssize_t rowIdx = _document.GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      size_t row_idx = static_cast<size_t>(rowIdx);
      ssize_t viewRowIdx = _mapRowIdx2ViewRowIdx.at(static_cast<size_t>(row_idx));
      if (viewRowIdx < 0)
      {
        throw std::out_of_range("row filtered out: " + pRowName);
      }

      return row_idx;
    }

    /**
     * @brief   Get row by view-index.
     * @param   pViewRowIdx           zero-based row view-index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data from view.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    std::vector<T> GetViewRow(const size_t pViewRowIdx,
                              f_ConvFuncToVal<T> pConvertToVal =
                                ConverterToVal<T, USE_NUMERIC_LOCALE, USE_NAN>::ToVal) const
    {
      const size_t rowIdx = _mapViewRowIdx2RowIdx.at(pViewRowIdx);
      return _document.GetRow<T, USE_NUMERIC_LOCALE, USE_NAN>(rowIdx, pConvertToVal);
    }

    /**
     * @brief   Get row by name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data from view.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    std::vector<T> GetViewRow(const std::string& pRowName,
                              f_ConvFuncToVal<T> pConvertToVal =
                                ConverterToVal<T, USE_NUMERIC_LOCALE, USE_NAN>::ToVal) const
    {
      const size_t docRowIdx = GetDocumentRowIdx(pRowName);

      return _document.GetRow<T, USE_NUMERIC_LOCALE, USE_NAN>(docRowIdx, pConvertToVal);
    }

    /**
     * @brief   Get view-cell by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pViewRowIdx           zero-based row view-index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    T GetViewCell(const size_t pColumnIdx, const size_t pViewRowIdx,
                  f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T, USE_NUMERIC_LOCALE, USE_NAN>::ToVal) const
    {
      const size_t rowIdx = _mapViewRowIdx2RowIdx.at(pViewRowIdx);
      return _document.GetCell<T, USE_NUMERIC_LOCALE, USE_NAN>(pColumnIdx, rowIdx, pConvertToVal);
    }

    /**
     * @brief   Get cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    T GetViewCell(const std::string& pColumnName, const std::string& pRowName,
                  f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T, USE_NUMERIC_LOCALE, USE_NAN>::ToVal) const
    {
      const size_t docRowIdx = GetDocumentRowIdx(pRowName);

      const ssize_t columnIdx = _document.GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return _document.GetCell<T, USE_NUMERIC_LOCALE, USE_NAN>(
        static_cast<size_t>(columnIdx), docRowIdx, pConvertToVal);
    }

    /**
     * @brief   Get cell by column name and row index.
     * @param   pColumnName           column label name.
     * @param   pViewRowIdx           zero-based row view-index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    T GetViewCell(const std::string& pColumnName, const size_t pViewRowIdx,
                  f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T, USE_NUMERIC_LOCALE, USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = _document.GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return GetViewCell<T, USE_NUMERIC_LOCALE, USE_NAN>(static_cast<size_t>(columnIdx), pViewRowIdx, pConvertToVal);
    }

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE = 1, int USE_NAN = 0>
    T GetViewCell(const size_t pColumnIdx, const std::string& pRowName,
                  f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T, USE_NUMERIC_LOCALE, USE_NAN>::ToVal) const
    {
      const size_t docRowIdx = GetDocumentRowIdx(pRowName);

      return _document.GetCell<T, USE_NUMERIC_LOCALE, USE_NAN>(pColumnIdx, docRowIdx, pConvertToVal);
    }

  private:
    const Document& _document;
    std::vector<size_t> _mapViewRowIdx2RowIdx;
    std::vector<ssize_t> _mapRowIdx2ViewRowIdx;
  };


  template<Document::f_EvalBoolExpr evaluateBooleanExpression, typename... Types>
  class FilterDocument : public ViewDocument<evaluateBooleanExpression, Types...>
  {
  public:
    explicit FilterDocument(const Document& document)
      : ViewDocument<evaluateBooleanExpression, Types...>(document)
    {
    }
  };


  constexpr bool selectAll(const Document::t_dataRow&)
  {
    return true;
  }
  template<typename... Types>
  class SortDocument : public ViewDocument<selectAll, Types...>
  {
  public:
    template<typename... SPtypes>
    explicit SortDocument(const Document& document, const SPtypes & ... spArgs)
      : ViewDocument<selectAll, Types...>(document, spArgs...)
    {
    }
  };


  template<Document::f_EvalBoolExpr evaluateBooleanExpression, typename... Types>
  class FilterSortDocument : public ViewDocument<evaluateBooleanExpression, Types...>
  {
  public:
    template<typename... SPtypes>
    explicit FilterSortDocument(const Document& document, const SPtypes & ... spArgs)
      : ViewDocument<evaluateBooleanExpression, Types...>(document, spArgs...)
    {
    }
  };
}
