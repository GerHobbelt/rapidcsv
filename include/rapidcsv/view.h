/*
 * _ViewDocument.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_FilterSort
 * Version:  2.01
 *
 * Copyright (C) 2022-2023 Gautam Dhar
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

//TODO documentation

namespace rapidcsv
{
  enum e_SortOrder
  {
    ASCEND = 0,
    DESCEND
  };


  template< c_S2Tconverter S2Tconv >
  class SortParams
  {
  public:
    const size_t rawDataColumnIndex;
    const e_SortOrder sortOrder;

    using return_type = S2Tconv::return_type;

    SortParams(const size_t RAW_DATA_COLUMN_INDEX,
               const e_SortOrder SORT_ORDER = e_SortOrder::ASCEND) :
      rawDataColumnIndex(RAW_DATA_COLUMN_INDEX),
      sortOrder(SORT_ORDER)
    {}

    typename S2Tconv::return_type
    getValue(const Document::t_dataRow& row) const
    {
      const std::string& cellStr = row.at(rawDataColumnIndex);
      return S2Tconv::ToVal(cellStr);
    }
  };

  template<typename... SPtypes>
  struct SortKeyType
  {
    //https://stackoverflow.com/questions/10014713/build-tuple-using-variadic-templates
    using t_sortKey = std::tuple<decltype(SPtypes::return_type)...>;
  };

  template<typename SP, typename... SPtypes >  // SPtypes -> SortParams<ReturnTypes...>
  class _RowComparator
  {
  private:
    const _RowComparator<SPtypes... > _nextColumnInfo;
    const e_SortOrder _sortOrder;

    bool _ascendingOrder(const typename SortKeyType<SP, SPtypes...>::t_sortKey& lhVal,
                         const typename SortKeyType<SP, SPtypes...>::t_sortKey& rhVal) const
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

    bool _descendingOrder(const typename SortKeyType<SP, SPtypes...>::t_sortKey& lhVal,
                          const typename SortKeyType<SP, SPtypes...>::t_sortKey& rhVal) const
    {
      if (lhVal.value < rhVal.value)
      {
        return false;
      }
      if (lhVal.value > rhVal.value)
      {
        return true;
      }

      // lhVal == rhVal
      return _nextColumnInfo(lhVal.nextIndex, rhVal.nextIndex);
    }

  public:
    _RowComparator(const SP& pSortParams,
                   const SPtypes& ... spArgs)
      : _nextColumnInfo(spArgs...),
        _sortOrder( pSortParams.sortOrder )
    {}

    bool operator () (const typename SortKeyType<SP, SPtypes...>::t_sortKey& lhVal,
                      const typename SortKeyType<SP, SPtypes...>::t_sortKey& rhVal) const
    {
      if (e_SortOrder::ASCEND == _sortOrder)
      {
        return  _ascendingOrder(lhVal, rhVal);
      } else {
        return _descendingOrder(lhVal, rhVal);
      }
    }
  };

  template<typename SP>
  class _RowComparator<SP> // Template base case
  {
  private:
    const e_SortOrder _sortOrder;

    bool _ascendingOrder(const typename SortKeyType<SP>::t_sortKey& lhVal,
                         const typename SortKeyType<SP>::t_sortKey& rhVal) const
    {
      return lhVal.value < rhVal.value;
    }

    bool _descendingOrder(const typename SortKeyType<SP>::t_sortKey& lhVal,
                          const typename SortKeyType<SP>::t_sortKey& rhVal) const
    {
      return lhVal.value > rhVal.value;
    }

  public:
    _RowComparator(const SP& pSortParams)
      : _sortOrder(pSortParams.sortOrder)
    {}

    bool operator () (const typename SortKeyType<SP>::t_sortKey& lhVal,
                      const typename SortKeyType<SP>::t_sortKey& rhVal) const
    {
      if (e_SortOrder::ASCEND == _sortOrder)
      {
        return  _ascendingOrder(lhVal, rhVal);
      } else {
        return _descendingOrder(lhVal, rhVal);
      }
    }
  };


  /**
   * @brief     Class representing a CSV document view. The underlying 'Document' is viewed after
   *            applying filter and/or sort on rows, based on cell values after type-conversion.
   *            Only intended for converter internal usage, but exposed externally
   *            to allow specialization on datatypes.
   */
  class _ViewDocument
  {
  protected:
    const Document& _document;
    const std::vector<Document::t_dataRow>& _mData;
    std::vector< size_t> _mapViewRowIdx2RowIdx;
    std::vector<ssize_t> _mapRowIdx2ViewRowIdx;

    explicit _ViewDocument(const Document& document)
      : _document(document),     _mData(document._mData),
        _mapViewRowIdx2RowIdx(), _mapRowIdx2ViewRowIdx()
    {}

    inline RawIdx _getDataRowIndex(const size_t pRowIdx) const
    {
      return _document._getDataRowIndex(pRowIdx);
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
    // TODO function and unit tests  for ARGS...
    template< typename T, c_S2Tconverter S2Tconv = ConvertFromStr<T> >
    std::vector<typename S2Tconv::return_type>
    GetViewColumn(const c_sizet_or_string auto& pColumnNameIdx) const
    {
      const size_t pColumnIdx = _document.GetColumnIdx(pColumnNameIdx);
      const size_t dataColumnIdx = _document._getDataColumnIndex(pColumnIdx).dataIdx;
      const size_t firstRowIdx = _document._getDataRowIndex(0).dataIdx;
      std::vector<T> column;
      for (auto itViewRowIdx = _mapViewRowIdx2RowIdx.begin();
           itViewRowIdx != _mapViewRowIdx2RowIdx.end(); ++itViewRowIdx)
      {
        const size_t rowIdx = firstRowIdx + static_cast<size_t>(*itViewRowIdx);
        const rapidcsv::Document::t_dataRow& row = _document._mData.at(rowIdx);
        if (dataColumnIdx < row.size())
        {
          const std::string& cellStrVal = row.at(dataColumnIdx);
          typename S2Tconv::return_type val = S2Tconv::ToVal(cellStrVal);
          column.push_back(val);
        } else {
          const std::string errStr = std::string(__RAPIDCSV_FILE__)+":"+std::to_string(__LINE__)+
            " # requested column index " +
            std::to_string(pColumnIdx) + " >= " +
            std::to_string(row.size() - _document._getDataColumnIndex(0).dataIdx) +
            " (number of columns on row index " + std::to_string(rowIdx) + ")";
          throw std::out_of_range(errStr);
        }
      }
      return column;
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline std::vector< typename std::invoke_result_t< decltype(CONV_S2T),
                                                       const std::string&
                                                     >
                      >
    GetViewColumn(const c_sizet_or_string auto& pColumnNameIdx) const
    {
      return GetViewColumn<T, S2TwrapperFunction<T, CONV_S2T> >(pColumnNameIdx);
    }

    /**
     * @brief   Get view-row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    size_t GetViewRowIdx(const std::string& pRowName) const
    {
      const size_t rowIdx = _document.GetRowIdx(pRowName);

      ssize_t viewRowIdx = _mapRowIdx2ViewRowIdx.at(rowIdx);
      if (viewRowIdx < 0)
      {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+":"+std::to_string(__LINE__)+" row filtered out: " + pRowName);
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
      const size_t rowIdx = _document.GetRowIdx(pRowName);

      ssize_t viewRowIdx = _mapRowIdx2ViewRowIdx.at(rowIdx);
      if (viewRowIdx < 0)
      {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+":"+std::to_string(__LINE__)+" row filtered out: " + pRowName);
      }

      return rowIdx;
    }
    inline size_t GetDocumentRowIdx(const size_t& pViewRowIdx) const
    {
      return _mapViewRowIdx2RowIdx.at(pViewRowIdx);
    }
    inline size_t GetDocumentRowIdx(const int& pViewRowIdx) const
    {
      return _mapViewRowIdx2RowIdx.at(static_cast<size_t>(pViewRowIdx));
    }

    /**
     * @brief   Get row by name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data from view.
     */
    // TODO function and unit tests  for ARGS...
    template< typename T, c_S2Tconverter S2Tconv = ConvertFromStr<T> >
    inline std::vector<typename S2Tconv::return_type>
    GetViewRow(const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      const size_t docRowIdx = GetDocumentRowIdx(pRowName_ViewRowIdx);
      return _document.GetRow<T, S2Tconv >(docRowIdx);
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline std::vector< typename std::invoke_result_t< decltype(CONV_S2T),
                                                       const std::string&
                                                     >
                      >
    GetViewRow(const c_sizet_or_string auto& pRowNameIdx) const
    {
      return GetViewRow<T, S2TwrapperFunction<T, CONV_S2T> >(pRowNameIdx);
    }

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    // TODO function and unit tests  for ARGS...
    template< typename T, c_S2Tconverter S2Tconv = ConvertFromStr<T> >
    inline typename S2Tconv::return_type
    GetViewCell(const c_sizet_or_string auto& pColumnNameIdx,
	              const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      const size_t pColumnIdx = _document.GetColumnIdx(pColumnNameIdx);
      const size_t docRowIdx  = GetDocumentRowIdx(pRowName_ViewRowIdx);
      return _document.GetCell<T, S2Tconv >(pColumnIdx, docRowIdx);
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline typename std::invoke_result_t< decltype(CONV_S2T),
                                          const std::string&
                                        >
    GetViewCell(const c_sizet_or_string auto& pColumnNameIdx,
	              const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      return GetViewCell<T, S2TwrapperFunction<T, CONV_S2T> >( pColumnNameIdx, pRowName_ViewRowIdx);
    }
  };


  /**
   * @brief     Class representing a CSV document filtered-view. The underlying 'Document' is viewed after
   *            applying filter on rows, based on cell values after type-conversion.
   */
//TODO  create test testView....cpp  similar to test048.cpp and test049.cpp  which checks for NaN and null value for comparsion and sort
  template<Document::f_EvalBoolExpr evaluateBooleanExpression, typename... Types>
  class FilterDocument : public _ViewDocument
  {
  public:
    explicit FilterDocument(const Document& document)
      : _ViewDocument(document)
    {
      size_t rowIdx = 0;
      ssize_t viewRowIdx = 0;
      const ssize_t rowIdxStart = static_cast<ssize_t>(_getDataRowIndex(0).dataIdx);
      for (auto itRow  = _mData.begin() + rowIdxStart;
                itRow != _mData.end(); ++itRow, ++rowIdx)
      {
        if (evaluateBooleanExpression(*itRow))
        {
          _mapViewRowIdx2RowIdx.push_back(rowIdx);
          _mapRowIdx2ViewRowIdx.push_back(viewRowIdx);
          ++viewRowIdx;
        } else {
          _mapRowIdx2ViewRowIdx.push_back(-10);
        }
      }
    }
  };


  template<Document::f_EvalBoolExpr evaluateBooleanExpression, typename... SPtypes>
  class FilterSortDocument : public _ViewDocument
  {
  private:
    const _RowComparator<const SPtypes& ...> _sortPredicate;
    std::map<const typename SortKeyType<SPtypes& ...>::t_sortKey, std::size_t,
             _RowComparator<SPtypes&& ...> > _sortedData;

  public:
    explicit FilterSortDocument(const Document& document, const SPtypes& ... spArgs)
      : _ViewDocument(document), _sortPredicate(spArgs...), _sortedData(_sortPredicate)
    {
      size_t rowIdx = 0;
      const ssize_t rowIdxStart = static_cast<ssize_t>(_getDataRowIndex(0).dataIdx);
      for (auto itRow  = _mData.begin() + rowIdxStart;
                itRow != _mData.end(); ++itRow, ++rowIdx)
      {
        if (evaluateBooleanExpression(*itRow))
        {
          typename SortKeyType<SPtypes ...>::t_sortKey rowIndexValues{((spArgs.getValue(*itRow)), ...)};
          // std::cout << "rowIndexValues.value = " << rowIndexValues.value << std::endl;
          _sortedData[rowIndexValues] = rowIdx;
        }
        _mapRowIdx2ViewRowIdx.push_back(-10);
      }

      ssize_t viewRowIdx = 0;
      for (auto itMap  = _sortedData.begin();
                itMap != _sortedData.end(); ++itMap, ++viewRowIdx)
      {
        rowIdx = itMap->second;
        _mapViewRowIdx2RowIdx.push_back(rowIdx);
        _mapRowIdx2ViewRowIdx.at(rowIdx) = viewRowIdx;
        // std::cout << "rowIdx("<< rowIdx <<") <-> viewRowIdx(" << viewRowIdx << ")" << std::endl;
      }
    }

    template< typename T, c_S2Tconverter S2Tconv = ConvertFromStr<T> >
    std::vector<typename S2Tconv::return_type>
    GetDocRow(const t_sortKey& pRowKey) const
    {
      size_t docRowIdx;
      try {
        docRowIdx  = _sortedData.at(pRowKey);
      } catch (std::out_of_range& err) {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+":"+std::to_string(__LINE__)
              +" rowKey not found in 'sortedKeyMap'. FilterSortDocument::GetDocRow() : " + err.what());
      }
      return _document.GetRow<T, S2Tconv >(docRowIdx);
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline std::vector< typename std::invoke_result_t< decltype(CONV_S2T),
                                                       const std::string&
                                                     >
                      >
    GetDocRow(const t_sortKey& pRowKey) const
    {
      return GetDocRow<T, S2TwrapperFunction<T, CONV_S2T> >(pRowKey);
    }

    template< typename T, c_S2Tconverter S2Tconv = ConvertFromStr<T> >
    typename S2Tconv::return_type
    GetDocCell(const c_sizet_or_string auto& pColumnNameIdx,
	             const t_sortKey& pRowKey) const
    {
      const size_t pColumnIdx = _document.GetColumnIdx(pColumnNameIdx);
      size_t docRowIdx;
      try {
        docRowIdx  = _sortedData.at(pRowKey);
      } catch (std::out_of_range& err) {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+":"+std::to_string(__LINE__)
              +" rowKey not found in 'sortedKeyMap'. FilterSortDocument::GetDocCell() : " + err.what());
      }
      return _document.GetCell<T, S2Tconv >(pColumnIdx, docRowIdx);
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline typename std::invoke_result_t< decltype(CONV_S2T),
                                          const std::string&
                                        >
    GetDocCell(const c_sizet_or_string auto& pColumnNameIdx,
	             const t_sortKey& pRowKey) const
    {
      return GetDocCell<T, S2TwrapperFunction<T, CONV_S2T> >( pColumnNameIdx, pRowKey);
    }
  };


  constexpr bool selectAll(const Document::t_dataRow&) { return true; }
  template<typename... Types>
  class SortDocument : public FilterSortDocument<selectAll, Types...>
  {
  public:
    template<typename... SPtypes>
    explicit SortDocument(const Document& document, const SPtypes & ... spArgs)
      : FilterSortDocument<selectAll, Types...>(document, spArgs...)
    {}
  };
}
