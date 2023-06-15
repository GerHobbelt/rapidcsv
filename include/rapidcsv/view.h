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


  template< typename T_C,  // T_C :: T -> can be type such as int, double etc ;  XOR  C -> can be type of 'S2Tconverter'
            e_SortOrder SORT_ORDER = e_SortOrder::ASCEND >
  class SortParams
  {
    const size_t      _rawDataColumnIndex;

  public:
    using S2Tconv_type = t_S2Tconv<T_C>;
    using return_type = typename S2Tconv_type::return_type;

    constexpr static e_SortOrder sortOrder = SORT_ORDER;

    SortParams(const size_t RAW_DATA_COLUMN_INDEX) :
        _rawDataColumnIndex(RAW_DATA_COLUMN_INDEX)
    {}

    inline typename S2Tconv_type::return_type
    getValue(const Document::t_dataRow& row) const
    {
      const std::string& cellStr = row.at(_rawDataColumnIndex);
      return S2Tconv_type::ToVal(cellStr);
    }
  };

  template <typename, typename = void>
  struct is_SortParams : std::false_type {};

  template <typename CSP>
  struct is_SortParams< CSP, std::void_t< decltype(&CSP::getValue),    // check for the presence of 'NON static' member function CSP::getValue
                                          typename CSP::S2Tconv_type,  // check for the presence of type-def CSP::S2Tconv_type
                                          typename CSP::return_type    // check for the presence of type-def CSP::return_type
                                        >
                        >
          : std::is_same<decltype(&CSP::getValue), typename CSP::return_type(CSP::*)(const Document::t_dataRow& row) const> {}; // member is function

  template <typename CSP>
  concept c_SortParams = is_SortParams<CSP>::value;

  template <typename CSP>
  concept c_NOT_SortParams = !is_SortParams<CSP>::value;



  template<c_SortParams ... SPtypes>
  struct SortKeyFactory
  {
    //https://stackoverflow.com/questions/10014713/build-tuple-using-variadic-templates
    using t_sortKey = std::tuple<typename SPtypes::return_type ...>;

    inline static t_sortKey
    createSortKey(const Document::t_dataRow& rowData, const SPtypes& ... sp)
    {
      t_sortKey result;
      auto write_tuple = [&rowData, sp ... ] (auto&... wrt_result) -> void
      {
        ( (wrt_result = sp.getValue(rowData)), ... );  // comma operator ensures the element order is from left to right
      };
      std::apply(write_tuple, result);
      return result;
    }
  };

  template<c_SortParams ... SPtypes>
  struct _RowComparator
  {
  private:
    using t_tuple_SortParams = std::tuple<SPtypes ...>;
    using t_sortKey = typename SortKeyFactory<SPtypes ...>::t_sortKey;

    // https://stackoverflow.com/questions/5512910/explicit-specialization-of-template-class-member-function
    // https://stackoverflow.com/a/28697168/2299954
    template<size_t I,
             std::enable_if_t< I < (sizeof...(SPtypes)-1), int> = 0 >
    inline static bool _compare(const t_sortKey& lhCompositeKey,
                                const t_sortKey& rhCompositeKey)
    {
      using t_tupleSPtype = std::tuple_element_t<I, t_tuple_SortParams>;

      const typename t_tupleSPtype::return_type& lhVal = std::get<I>(lhCompositeKey);
      const typename t_tupleSPtype::return_type& rhVal = std::get<I>(rhCompositeKey);
      if constexpr ( t_tupleSPtype::sortOrder == e_SortOrder::ASCEND )
      {
        if(lhVal < rhVal)  return true;
        if(lhVal > rhVal)  return false;

        // lhVal == rhVal
        return _compare<I+1>(lhCompositeKey,rhCompositeKey);
      } else {
        if(lhVal > rhVal)  return true;
        if(lhVal < rhVal)  return false;

        // lhVal == rhVal
        return _compare<I+1>(lhCompositeKey,rhCompositeKey);
      }
    }

    // https://stackoverflow.com/a/28697168/2299954
    template<size_t I,
             std::enable_if_t< I >= (sizeof...(SPtypes)-1), int> = 0 >
    inline static bool _compare(const t_sortKey& lhCompositeKey,
                                const t_sortKey& rhCompositeKey)
    {
      using t_tupleSPtype = std::tuple_element_t<I, t_tuple_SortParams>;

      const typename t_tupleSPtype::return_type& lhVal = std::get<I>(lhCompositeKey);
      const typename t_tupleSPtype::return_type& rhVal = std::get<I>(rhCompositeKey);
      if constexpr ( t_tupleSPtype::sortOrder == e_SortOrder::ASCEND )
      {
          return (lhVal < rhVal);
      } else {
          return (lhVal > rhVal);
      }
    }


  public:
    bool operator() (const typename SortKeyFactory<SPtypes ...>::t_sortKey& lhCompositeKey,
                     const typename SortKeyFactory<SPtypes ...>::t_sortKey& rhCompositeKey) const
    {
      return _compare<0>(lhCompositeKey,rhCompositeKey);
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
    template< typename T_C >
    std::vector<typename t_S2Tconv<T_C>::return_type>
    GetViewColumn(const c_sizet_or_string auto& pColumnNameIdx) const
    {
      const size_t pColumnIdx = _document.GetColumnIdx(pColumnNameIdx);
      const size_t dataColumnIdx = _document._getDataColumnIndex(pColumnIdx).dataIdx;
      const size_t firstRowIdx = _document._getDataRowIndex(0).dataIdx;
      std::vector<typename t_S2Tconv<T_C>::return_type> column;
      for (auto itViewRowIdx = _mapViewRowIdx2RowIdx.begin();
           itViewRowIdx != _mapViewRowIdx2RowIdx.end(); ++itViewRowIdx)
      {
        const size_t rowIdx = firstRowIdx + static_cast<size_t>(*itViewRowIdx);
        const rapidcsv::Document::t_dataRow& row = _document._mData.at(rowIdx);
        if (dataColumnIdx < row.size())
        {
          const std::string& cellStrVal = row.at(dataColumnIdx);
          typename t_S2Tconv<T_C>::return_type val = t_S2Tconv<T_C>::ToVal(cellStrVal);
          column.push_back(val);
        } else {
          const std::string errStr = std::string(__RAPIDCSV_FILE__)+" : _ViewDocument::GetViewColumn()"+
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
      return GetViewColumn< S2TwrapperFunction<T, CONV_S2T> >(pColumnNameIdx);
    }

    /**
     * @brief   Get view-row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    inline size_t GetViewRowIdx(const std::string& pRowName) const
    {
      const size_t rowIdx = _document.GetRowIdx(pRowName);

      ssize_t viewRowIdx = _mapRowIdx2ViewRowIdx.at(rowIdx);
      if (viewRowIdx < 0)
      {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+" : _ViewDocument::GetViewRowIdx() : row filtered out: " + pRowName);
      }

      return static_cast<size_t>(viewRowIdx);
    }

    /**
     * @brief   Get document-row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    inline size_t GetDocumentRowIdx(const std::string& pRowName) const
    {
      const size_t rowIdx = _document.GetRowIdx(pRowName);

      ssize_t viewRowIdx = _mapRowIdx2ViewRowIdx.at(rowIdx);
      if (viewRowIdx < 0)
      {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+" : _ViewDocument::GetDocumentRowIdx() : row filtered out: " + pRowName);
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
    template< typename ... T_C >
    inline std::tuple<typename t_S2Tconv<T_C>::return_type ...>
    GetViewRow(const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      const size_t docRowIdx = GetDocumentRowIdx(pRowName_ViewRowIdx);
      return _document.GetRow< t_S2Tconv<T_C> ... >(docRowIdx);
    }

    inline std::vector<std::string>
    GetViewRow_VecStr(const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      const size_t docRowIdx = GetDocumentRowIdx(pRowName_ViewRowIdx);
      return _document.GetRow_VecStr(docRowIdx);
    }

/*
    NOTE: the code below doesn't compile, nor is it a good idea to
          have two variadic lists of T's and CONV_S2T's.

          Alternatively, if needed explicitly call using template 'S2TwrapperFunction<T, CONV_S2T>'
          GetRow< S2TwrapperFunction<T, CONV_S2T> , ... >(pRowName_ViewRowIdx);

          usage details refer test063.cpp

    template< typename ... T, auto ... (*CONV_S2T)(const std::string&) >
    inline std::vector< typename std::invoke_result_t< decltype(CONV_S2T),
                                                       const std::string&
                                                     >...
                      >
    GetViewRow(const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      return GetViewRow< S2TwrapperFunction<T, CONV_S2T>... >(pRowName_ViewRowIdx);
    }
*/

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    // TODO function and unit tests  for ARGS...
    template< typename T_C >
    inline typename t_S2Tconv<T_C>::return_type
    GetViewCell(const c_sizet_or_string auto& pColumnNameIdx,
	              const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      const size_t pColumnIdx = _document.GetColumnIdx(pColumnNameIdx);
      const size_t docRowIdx  = GetDocumentRowIdx(pRowName_ViewRowIdx);
      return _document.GetCell< t_S2Tconv<T_C> >(pColumnIdx, docRowIdx);
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline typename std::invoke_result_t< decltype(CONV_S2T),
                                          const std::string&
                                        >
    GetViewCell(const c_sizet_or_string auto& pColumnNameIdx,
	              const c_sizet_or_string auto& pRowName_ViewRowIdx) const
    {
      return GetViewCell< S2TwrapperFunction<T, CONV_S2T> >( pColumnNameIdx, pRowName_ViewRowIdx);
    }
  };


  typedef bool (* f_EvalBoolExpr)(const Document::t_dataRow& dataRow);

  /**
   * @brief     Class representing a CSV document filtered-view. The underlying 'Document' is viewed after
   *            applying filter on rows, based on cell values after type-conversion.
   */
  //TODO  create test testView....cpp  similar to test048.cpp and test049.cpp  which checks for NaN and null value for comparsion and sort
  template<f_EvalBoolExpr evaluateBooleanExpression, typename... Types>
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

  template<f_EvalBoolExpr evaluateBooleanExpression, c_SortParams ... SPtypes>
  class _FilterSortDocument : public _ViewDocument
  {
  private:
    using t_sortPredicate = _RowComparator<SPtypes ...>;
    using t_sortKey = typename SortKeyFactory<SPtypes ...>::t_sortKey;

    const t_sortPredicate _sortPredicate;
    std::map<t_sortKey, size_t,
             t_sortPredicate > _sortedData;

  protected:
    explicit _FilterSortDocument(const Document& document, const SPtypes& ... spArgs)
      : _ViewDocument(document), _sortPredicate(), _sortedData(_sortPredicate)
    {
      size_t rowIdx = 0;
      const ssize_t rowIdxStart = static_cast<ssize_t>(_getDataRowIndex(0).dataIdx);
      for (auto itRow  = _mData.begin() + rowIdxStart;
                itRow != _mData.end(); ++itRow, ++rowIdx)
      {
        if (evaluateBooleanExpression(*itRow))
        {
          t_sortKey rowIndexValues  = SortKeyFactory<SPtypes ...>::createSortKey((*itRow), spArgs ...);
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

  public:
    template< typename ... T_C >
    std::tuple<typename t_S2Tconv<T_C>::return_type ...>
    GetOrderedRow(const typename SortKeyFactory<SPtypes ...>::t_sortKey& pRowKey) const
    {
      size_t docRowIdx;
      try {
        docRowIdx  = _sortedData.at(pRowKey);
      } catch (std::out_of_range& err) {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+" : FilterSortDocument::GetOrderedRow() : "
              +" rowKey not found in 'sortedKeyMap'. For pRowKey="
              +ConvertFromTuple<typename SPtypes::S2Tconv_type ...>::ToStr(pRowKey) + " : ERROR: " + err.what());
      }
      return _document.GetRow< t_S2Tconv<T_C> ... >(docRowIdx);
    }

    inline std::vector<std::string>
    GetOrderedRow_VecStr(const typename SortKeyFactory<SPtypes ...>::t_sortKey& pRowKey) const
    {
      size_t docRowIdx;
      try {
        docRowIdx  = _sortedData.at(pRowKey);
      } catch (std::out_of_range& err) {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+" : FilterSortDocument::GetOrderedRow_VecStr() : "
              +" rowKey not found in 'sortedKeyMap'. For pRowKey="
              +ConvertFromTuple<typename SPtypes::S2Tconv_type ...>::ToStr(pRowKey) + " : ERROR: " + err.what());
      }
      return _document.GetRow_VecStr(docRowIdx);
    }

/*
    NOTE: the code below doesn't compile, nor is it a good idea to
          have two variadic lists of T's and CONV_S2T's.

          Alternatively, if needed explicitly call using template 'S2TwrapperFunction<T, CONV_S2T>'
          GetRow< S2TwrapperFunction<T, CONV_S2T> , ... >(pRowNameIdx);

          usage details refer test063.cpp

    template< typename ... T, auto ... (*CONV_S2T)(const std::string&) >
    inline std::tuple< typename std::invoke_result_t< decltype(CONV_S2T),
                                                      const std::string&
                                                   >...
                     >
    GetOrderedRow(const typename SortKeyFactory<SPtypes ...>::t_sortKey& pRowKey) const
    {
      return GetOrderedRow< S2TwrapperFunction<T, CONV_S2T>... >(pRowKey);
    }
*/

    template< typename T_C >
    typename t_S2Tconv<T_C>::return_type
    GetOrderedCell(const c_sizet_or_string auto& pColumnNameIdx,
	                 const t_sortKey& pRowKey) const
    {
      const size_t pColumnIdx = _document.GetColumnIdx(pColumnNameIdx);
      size_t docRowIdx;
      try {
        docRowIdx  = _sortedData.at(pRowKey);
      } catch (std::out_of_range& err) {
        throw std::out_of_range(std::string(__RAPIDCSV_FILE__)+" :FilterSortDocument::GetOrderedCell() : "
              +" rowKey not found in 'sortedKeyMap'. For pRowKey="
              +ConvertFromTuple<typename SPtypes::S2Tconv_type ...>::ToStr(pRowKey) + " : ERROR: " + err.what());
      }
      return _document.GetCell< t_S2Tconv<T_C> >(pColumnIdx, docRowIdx);
    }

    template< typename T, auto (*CONV_S2T)(const std::string&) >
    inline typename std::invoke_result_t< decltype(CONV_S2T),
                                          const std::string&
                                        >
    GetOrderedCell(const c_sizet_or_string auto& pColumnNameIdx,
	                 const t_sortKey& pRowKey) const
    {
      return GetOrderedCell< S2TwrapperFunction<T, CONV_S2T> >( pColumnNameIdx, pRowKey);
    }
  };

  // https://stackoverflow.com/questions/68058640/using-concepts-to-select-class-template-specialization?rq=3
  template<f_EvalBoolExpr evaluateBooleanExpression, typename ... T>
  class FilterSortDocument;


  template<f_EvalBoolExpr evaluateBooleanExpression, c_SortParams ... SPtypes>
  class FilterSortDocument <evaluateBooleanExpression, SPtypes...>     // IMPORTANT specialization declaration
           : public _FilterSortDocument<evaluateBooleanExpression, SPtypes...>
  {
  public:
    explicit FilterSortDocument(const Document& document, const SPtypes& ... spArgs)
      : _FilterSortDocument<evaluateBooleanExpression, SPtypes...>(document, spArgs ...)
    {}
  };

  template<f_EvalBoolExpr evaluateBooleanExpression, c_NOT_SortParams ... T>
  class FilterSortDocument <evaluateBooleanExpression, T...>     // IMPORTANT specialization declaration
           : public _FilterSortDocument<evaluateBooleanExpression, SortParams<T>...>
  {
  public:
    explicit FilterSortDocument(const Document& document, const SortParams<T>& ... spArgs)
      : _FilterSortDocument<evaluateBooleanExpression, SortParams<T>...>(document, spArgs ...)
    {}
  };



  constexpr bool selectAll(const Document::t_dataRow&) { return true; }

  template<typename ... T>
  using SortDocument = FilterSortDocument<selectAll, T ...>;
}
