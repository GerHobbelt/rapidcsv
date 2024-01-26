/*
 * rapidcsv.h
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
 * URL:      https://github.com/d99kris/rapidcsv
 * Version:  8.80
 *
 * Copyright (C) 2017-2023 Kristofer Berggren
 * All rights reserved.
 *
 * rapidcsv is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <algorithm>

#ifdef HAS_CODECVT
#include <codecvt>
#include <locale>
#endif

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <converter/converter.h>

#define RAPIDCSV_VERSION_MAJOR 4
#define RAPIDCSV_VERSION_MINOR 0
#define RAPIDCSV_VERSION_PATCH 4

#define UPSTREAM___RAPIDCSV__VERSION 8.80

//  Project path is removed from the __FILE__
//  Resulting file-path is relative path from project-root-folder.
#if  RAPIDCSV_USE_FILEPREFIXMAP == 1
  // the project-prefix-path is removed via compilation directive file-prefix-map
  #define RAPIDCSV_FILE    __FILE__
#else
  // https://stackoverflow.com/questions/8487986/file-macro-shows-full-path/40947954#40947954
  // the project-prefix-path is skipped by offsetting to length of project-prefix-path
  //#define RAPIDCSV_FILE   (__FILE__ + RAPIDCSV_SOURCE_PATH_SIZE)  // gives lot of warnings on windows:clangCL
  #define RAPIDCSV_FILE   (&(__FILE__[RAPIDCSV_SOURCE_PATH_SIZE]))

#endif

// to handle windows back-slash path seperator
#define RAPIDCSV_PREFERRED_PATH    (std::filesystem::path(RAPIDCSV_FILE).make_preferred().string())


#if FLAG_RAPIDCSV_debug_log == 1
  #define RAPIDCSV_DEBUG_LOG(aMessage) { std::cout << aMessage << " :: file:" << RAPIDCSV_PREFERRED_PATH << ":" << __LINE__ << std::endl; }
#else
  #define RAPIDCSV_DEBUG_LOG(aMessage)
#endif



namespace rapidcsv
{
  /**
   * @brief     Class representing Version number of the project.
  */
  static constexpr struct {
    uint16_t major, minor, patch;
  } version = {
    RAPIDCSV_VERSION_MAJOR,
    RAPIDCSV_VERSION_MINOR,
    RAPIDCSV_VERSION_PATCH
  };

#if defined(_MSC_VER)
  static const bool sPlatformHasCR = true;
#else
  static const bool sPlatformHasCR = false;
#endif
  static const std::vector<char> s_Utf8BOM = { '\xef', '\xbb', '\xbf' };

  enum FlgColumnName { CN_PRESENT, CN_MISSING };
  enum FlgRowName    { RN_PRESENT, RN_MISSING };

  /**
   * @brief     Datastructure holding parameters controlling which row and column should be
   *            treated as labels.
   */
  struct LabelParams
  {
    /**
     * @brief   Constructor
     * @param   pColumnNameFlg        specifies the zero-based row index of the column labels, setting
     *                                it to FlgColumnName::CN_MISSING prevents column lookup by label name, and gives access
     *                                to all rows as document data. Default: FlgColumnName::CN_PRESENT
     * @param   pRowNameFlg           specifies the zero-based column index of the row labels, setting
     *                                it to FlgRowName::RN_MISSING prevents row lookup by label name, and gives access
     *                                to all columns as document data. Default: FlgRowName::RN_MISSING
     */
    explicit LabelParams(const FlgColumnName pColumnNameFlg = FlgColumnName::CN_PRESENT,
                         const FlgRowName    pRowNameFlg    = FlgRowName::RN_MISSING)
      : mColumnNameFlg(pColumnNameFlg)
      , mRowNameFlg(pRowNameFlg)
    {}

    /**
     * @brief   specifies the zero-based row index of the column labels.
     */
    FlgColumnName mColumnNameFlg;

    /**
     * @brief   specifies the zero-based column index of the row labels.
     */
    FlgRowName    mRowNameFlg;
  };

  /**
   * @brief     Datastructure holding parameters controlling how the CSV data fields are separated.
   */
  struct SeparatorParams
  {
    /**
     * @brief   Constructor
     * @param   pSeparator            specifies the column separator (default ',').
     * @param   pTrim                 specifies whether to trim leading and trailing spaces from
     *                                cells read (default false).
     * @param   pHasCR                specifies whether a new document (i.e. not an existing document read)
     *                                should use CR/LF instead of only LF (default is to use standard
     *                                behavior of underlying platforms - CR/LF for Win, and LF for others).
     * @param   pQuotedLinebreaks     specifies whether to allow line breaks in quoted text (default false)
     * @param   pAutoQuote            specifies whether to automatically dequote data during read, and add
     *                                quotes during write (default true).
     * @param   pQuoteChar            specifies the quote character (default '\"').
     */
    explicit SeparatorParams(const char pSeparator = ',', const bool pTrim = false,
                             const bool pHasCR = sPlatformHasCR, const bool pQuotedLinebreaks = false,
                             const bool pAutoQuote = true, const char pQuoteChar = '"')
      : mSeparator(pSeparator)
      , mTrim(pTrim)
      , mHasCR(pHasCR)
      , mQuotedLinebreaks(pQuotedLinebreaks)
      , mAutoQuote(pAutoQuote)
      , mQuoteChar(pQuoteChar)
    {}

    /**
     * @brief   specifies the column separator.
     */
    char mSeparator;

    /**
     * @brief   specifies whether to trim leading and trailing spaces from cells read.
     */
    bool mTrim;

    /**
     * @brief   specifies whether new documents should use CR/LF instead of LF.
     */
    bool mHasCR;

    /**
     * @brief   specifies whether to allow line breaks in quoted text.
     */
    bool mQuotedLinebreaks;

    /**
     * @brief   specifies whether to automatically dequote cell data.
     */
    bool mAutoQuote;

    /**
     * @brief   specifies the quote character.
     */
    char mQuoteChar;
  };

  /**
   * @brief     Datastructure holding parameters controlling how special line formats should be
   *            treated.
   */
  struct LineReaderParams
  {
    /**
     * @brief   Constructor
     * @param   pSkipCommentLines     specifies whether to skip lines prefixed with
     *                                mCommentPrefix. Default: false
     * @param   pCommentPrefix        specifies which prefix character to indicate a comment
     *                                line. Default: #
     * @param   pSkipEmptyLines       specifies whether to skip empty lines. Default: false
     */
    explicit LineReaderParams(const bool pSkipCommentLines = false,
                              const char pCommentPrefix = '#',
                              const bool pSkipEmptyLines = false)
      : mSkipCommentLines(pSkipCommentLines)
      , mCommentPrefix(pCommentPrefix)
      , mSkipEmptyLines(pSkipEmptyLines)
    {}

    /**
     * @brief   specifies whether to skip lines prefixed with mCommentPrefix.
     */
    bool mSkipCommentLines;

    /**
     * @brief   specifies which prefix character to indicate a comment line.
     */
    char mCommentPrefix;

    /**
     * @brief   specifies whether to skip empty lines.
     */
    bool mSkipEmptyLines;
  };


  /**
   * @brief       To restrict a type to either numeric or string value.
   *              This mechanism reduces code duplicity of getter and setter functions,
   *              i.e 'Get...(c_sizet_or_string nameIdx, ...)' 
   *              and 'Set...(c_sizet_or_string nameIdx, ...)' 
   *              Instead of 'function-overload', we have 'variable-type-overload'.
   */
  template <typename SI>
  concept c_sizet_or_string = std::is_convertible_v<SI,size_t> ||
                              std::is_convertible_v<SI,std::string>;


  /**
   * @brief     Class representing a CSV document.
   */
  class Document
  {
  public:

    /**
     * @brief   type definition for a row of strings.
     */
    using t_dataRow = std::vector<std::string>;

    /**
     * @brief   Constructor
     * @param   pPath                 specifies the path of an existing CSV-file to populate the Document
     *                                data with.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pLineReaderParams     specifies how special line formats should be treated.
     */
    explicit Document(const std::string& pPath = std::string(),
                      const LabelParams& pLabelParams = LabelParams(),
                      const SeparatorParams& pSeparatorParams = SeparatorParams(),
                      const LineReaderParams& pLineReaderParams = LineReaderParams())
      : _mPath(pPath)
      , _mLabelParams(pLabelParams)
      , _mSeparatorParams(pSeparatorParams)
      , _mLineReaderParams(pLineReaderParams)
      , _mData()
      , _mColumnNamesIdx()
      , _mRowNamesIdx()
      , _mIdxColumnNames()
      , _mIdxRowNames()
      , _mFirstCornerCell()
    {
      if (!_mPath.empty())
      {
        _readCsv();
      }
    }

    /**
     * @brief   Constructor
     * @param   pStream               specifies a binary input stream to read CSV data from.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pLineReaderParams     specifies how special line formats should be treated.
     */
    explicit Document(std::istream& pStream,
                      const LabelParams& pLabelParams = LabelParams(),
                      const SeparatorParams& pSeparatorParams = SeparatorParams(),
                      const LineReaderParams& pLineReaderParams = LineReaderParams())
      : _mPath()
      , _mLabelParams(pLabelParams)
      , _mSeparatorParams(pSeparatorParams)
      , _mLineReaderParams(pLineReaderParams)
      , _mData()
      , _mColumnNamesIdx()
      , _mRowNamesIdx()
      , _mIdxColumnNames()
      , _mIdxRowNames()
      , _mFirstCornerCell()
    {
      _readCsv(pStream);
    }

    /**
     * @brief   Read Document data from file.
     * @param   pPath                 specifies the path of an existing CSV-file to populate the Document
     *                                data with.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pLineReaderParams     specifies how special line formats should be treated.
     */
    void Load(const std::string& pPath,
              const LabelParams& pLabelParams = LabelParams(),
              const SeparatorParams& pSeparatorParams = SeparatorParams(),
              const LineReaderParams& pLineReaderParams = LineReaderParams())
    {
      _mPath = pPath;
      _mLabelParams = pLabelParams;
      _mSeparatorParams = pSeparatorParams;
      _mLineReaderParams = pLineReaderParams;
      _readCsv();
    }

    /**
     * @brief   Read Document data from stream.
     * @param   pStream               specifies a binary input stream to read CSV data from.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pLineReaderParams     specifies how special line formats should be treated.
     */
    void Load(std::istream& pStream,
              const LabelParams& pLabelParams = LabelParams(),
              const SeparatorParams& pSeparatorParams = SeparatorParams(),
              const LineReaderParams& pLineReaderParams = LineReaderParams())
    {
      _mPath = "";
      _mLabelParams = pLabelParams;
      _mSeparatorParams = pSeparatorParams;
      _mLineReaderParams = pLineReaderParams;
      _readCsv(pStream);
    }

    /**
     * @brief   Write Document data to file.
     * @param   pPath                 optionally specifies the path where the CSV-file will be created
     *                                (if not specified, the original path provided when creating or
     *                                loading the Document data will be used).
     */
    void Save(const std::string& pPath = std::string())
    {
      if (!pPath.empty())
      {
        _mPath = pPath;
      }
      _writeCsv();
    }

    /**
     * @brief   Write Document data to stream.
     * @param   pStream               specifies a binary output stream to write the data to.
     */
    void Save(std::ostream& pStream) const
    {
      _writeCsv(pStream);
    }

    /**
     * @brief   Clears loaded Document data.
     *
     */
    void Clear()
    {
      _mData.clear();
      _mColumnNamesIdx.clear();
      _mRowNamesIdx.clear();
      _mIdxColumnNames.clear();
      _mIdxRowNames.clear();
      _mFirstCornerCell="";
#ifdef HAS_CODECVT
      mIsUtf16 = false;
      mIsLE = false;
#endif
      _mHasUtf8BOM = false;
    }

    /**
     * @brief   Destructor.
     *
     */
    virtual ~Document()
    {
      Clear();
    }

    /*
     *   To support getter and setter functions with variable of concept-type 'c_sizet_or_string',
     *   overloaded GetColumnIdx(numeric-type) is needed.
     */
    constexpr size_t GetColumnIdx(const size_t pColumnIdx) const
    {
      return pColumnIdx;
    }
    constexpr size_t GetColumnIdx(int pColumnIdx) const
    {
      assert(pColumnIdx>=0);
      return static_cast<size_t>(pColumnIdx);
    }
    /**
     * @brief   Get column index by name.
     * @param   pColumnName           column label name.
     * @returns zero-based column index.
     */
    size_t GetColumnIdx(const std::string& pColumnName) const
    {
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
      {
        if (_mColumnNamesIdx.find(pColumnName) != _mColumnNamesIdx.end())
        {
          return _mColumnNamesIdx.at(pColumnName);
        } else {
          static const std::string errMsg("rapidcsv::Document::GetColumnIdx(pColumnName) : column not found for 'pColumnName'");
          RAPIDCSV_DEBUG_LOG(errMsg << " : pColumnName='" << pColumnName << "'");
          throw std::out_of_range(errMsg);
        }
      } else {
        static const std::string errMsg("rapidcsv::Document::GetColumnIdx(pColumnName) : '_mLabelParams.mColumnNameFlg' is set as 'FlgColumnName::CN_MISSING'");
        RAPIDCSV_DEBUG_LOG(errMsg << " : pColumnName='" << pColumnName << "'");
        throw std::out_of_range(errMsg);
      }
    }

    /*
    https://en.cppreference.com/w/cpp/language/template_parameters
    template<typename T, typename ...ARGS,
             T (*CONV_S2T)(const std::string&, ARGS ...) =
                       &converter::ConvertFromStr<T, S2T_DefaultFormat<T>, ARGS...>::ToVal >
    std::vector<T> GetColumn(const auto& pColumnNameIdx,
                             ARGS&& ... args) const
    TODO function and unit tests  for ARGS...
    */
    // https://en.cppreference.com/w/cpp/language/template_parameters
    /**
     * @brief   Get column either by it's index or name.
     * @tparam  T_C                   T can be data-type such as int, double etc ;  xOR
     *                                C -> Conversion class satisfying concept 'c_S2Tconverter'.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @returns 'vector<R>' of column data. By default, R is usually same type as T.
     *          Else if 'C ≃ converter::ConvertFromStr_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value,
     *          else the string value which caused failure during conversion.
     */
    template< typename T_C >
    std::vector<typename converter::t_S2Tconv_c<T_C>::return_type>
    GetColumn(const c_sizet_or_string auto& pColumnNameIdx) const
    {
      const size_t pColumnIdx = GetColumnIdx(pColumnNameIdx);
      std::vector<typename converter::t_S2Tconv_c<T_C>::return_type> column;
      size_t rowIdx = 0;
      for (auto itRow = _mData.begin();
           itRow != _mData.end(); ++itRow, ++rowIdx)
      {
        if (pColumnIdx < itRow->size())
        {
          const std::string& cellStrVal = itRow->at(pColumnIdx);
          //typename converter::t_S2Tconv_c<T_C>::return_type val = converter::t_S2Tconv_c<T_C>::ToVal_args(cellStrVal, std::forward<ARGS>(args)...);
          typename converter::t_S2Tconv_c<T_C>::return_type val = converter::t_S2Tconv_c<T_C>::ToVal(cellStrVal);
          column.push_back(val);
        } else {
          static const std::string errMsg("rapidcsv::Document::GetColumn(pColumnNameIdx) : column not found for 'pColumnNameIdx'");
          #if FLAG_RAPIDCSV_debug_log == 1
            const size_t adjRowSize = itRow->size();
            RAPIDCSV_DEBUG_LOG(errMsg << " : pColumnNameIdx='" << pColumnNameIdx << "' : pColumnIdx{" << pColumnIdx << "} >= rowSize{"  \
                                      << adjRowSize << "} : (number of columns on row index=" << rowIdx << ")");
          #endif
          throw std::out_of_range(errMsg);
        }
      }
      return column;
    }

    /**
     * @brief   Get column either by it's index or name.
     * @tparam  CONV_S2T              conversion function.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @returns 'vector<R>' of column data. By default, R is usually same type as T.
     *          Else if 'CONV_S2T ≃ converter::ConvertFromStr_gNaN<T>::ToVal', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value,
     *          else the string value which caused failure during conversion.
     */
    template< auto (*CONV_S2T)(const std::string&) >
    inline std::vector< typename converter::f_S2Tconv_c< CONV_S2T >::return_type >
    GetColumn(const c_sizet_or_string auto& pColumnNameIdx) const
    {
      return GetColumn< converter::f_S2Tconv_c< CONV_S2T > >(pColumnNameIdx);
    }

    /**
     * @brief   Set column either by it's index or name.
     * @tparam  T_C                   T can be data-type such as int, double etc ;   xOR
     *                                C -> Conversion class satisfying concept 'c_T2Sconverter'.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @param   pColumn               'vector<R>' of column data. By default, R is usually same type as T.
     *                                Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     */
    template< typename T_C >
    void SetColumn(const c_sizet_or_string auto& pColumnNameIdx,
                   const std::vector<typename converter::t_T2Sconv_c<T_C>::input_type>& pColumn)
    {
      const size_t columnIdx = GetColumnIdx(pColumnNameIdx);

      while (pColumn.size() > _getDataRowCount())
      {
        t_dataRow row;
        row.resize(_getDataColumnCount());
        _mData.push_back(row);
      }

      if ((columnIdx + 1) > _getDataColumnCount())
      {
        for (auto itRow = _mData.begin(); itRow != _mData.end(); ++itRow)
        {
          itRow->resize(columnIdx + 1);
        }
      }

      size_t rowIdx = 0;
      for (auto itCell = pColumn.begin(); itCell != pColumn.end(); ++itCell, ++rowIdx)
      {
        _mData.at(rowIdx).at(columnIdx) = converter::t_T2Sconv_c<T_C>::ToStr(*itCell);
      }
    }

    // TODO unit tests
    /**
     * @brief   Set column either by it's index or name.
     * @tparam  CONV_T2S              conversion function of type 'std::string (*CONV_T2S)(const R&)'.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @param   pColumn               'vector<R>' of column data. By default, R usually is same type as T.
     *                                Else if 'CONV_T2S ≃ converter::ConvertFromVal_gNaN<T>::ToStr', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     */
    //template< typename T, typename R, std::string (*CONV_T2S)(const R&) >
    template< auto CONV_T2S >
    inline void SetColumn(const c_sizet_or_string auto& pColumnNameIdx,
                          const std::vector<typename converter::f_T2Sconv_c<CONV_T2S>::input_type>& pColumn)
    {
      SetColumn< converter::f_T2Sconv_c<CONV_T2S> >(pColumnNameIdx, pColumn);
    }

    /**
     * @brief   Remove column either by it's index or name.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     */
    void RemoveColumn(const c_sizet_or_string auto& pColumnNameIdx)
    {
      const size_t columnIdx = GetColumnIdx(pColumnNameIdx);
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
      {
        _mIdxColumnNames.erase(_mIdxColumnNames.begin() + static_cast<ssize_t>(columnIdx));
      }
      for (auto itRow = _mData.begin(); itRow != _mData.end(); ++itRow)
      {
        itRow->erase(itRow->begin() + static_cast<ssize_t>(columnIdx));
      }

      _updateColumnNames("rapidcsv::Document::RemoveColumn()");
    }

    /**
     * @brief   Insert column at specified index.
     * @tparam  T_C                   can be data-type such as int, double etc ;   xOR
     *                                C -> Conversion class satisfying concept 'c_T2Sconverter'.
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumn               'vector<R>' of column data (optional argument). By default, R is usually same type as T.
     *                                Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     * @param   pColumnName           column label name (optional argument)
     */
    template< typename T_C >
    void InsertColumn(const size_t pColumnIdx,
                      const std::vector<typename converter::t_T2Sconv_c<T_C>::input_type>& pColumn
                          = std::vector<typename converter::t_T2Sconv_c<T_C>::input_type>(),
                      const std::string& pColumnName = std::string())
    {
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_MISSING && !pColumnName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::InsertColumn() : _mLabelParams.mColumnNameFlg = FlgColumnName::CN_MISSING -> can't process Column-Name");
        throw std::out_of_range(errMsg);
      }

      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT && pColumnName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::InsertColumn() : _mLabelParams.mColumnNameFlg = FlgColumnName::CN_PRESENT -> Column-Name can't be empty string");
        throw std::out_of_range(errMsg);
      }

      std::vector<std::string> column;
      if (pColumn.empty())
      {
        column.resize(_getDataRowCount());
      } else {
        column.resize(pColumn.size());
        size_t rowIdx = 0;
        for (auto itCell = pColumn.begin(); itCell != pColumn.end(); ++itCell, ++rowIdx)
        {
          column.at(rowIdx) = converter::t_T2Sconv_c<T_C>::ToStr(*itCell);
        }
      }

      while (column.size() > _getDataRowCount())
      {
        t_dataRow row;
        row.resize(_getDataColumnCount());
        _mData.push_back(row);
      }

      size_t rowIdx = 0;
      for (auto itRow = _mData.begin(); itRow != _mData.end(); ++itRow, ++rowIdx)
      {
        itRow->insert(itRow->begin() + static_cast<ssize_t>(pColumnIdx), column.at(rowIdx));
      }

      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
      {
        _mIdxColumnNames.insert(_mIdxColumnNames.begin() + static_cast<ssize_t>(pColumnIdx), pColumnName);
      }

      _updateColumnNames("rapidcsv::Document::InsertColumn()");
    }

    // TODO unit tests
    /**
     * @brief   Insert column at specified index.
     * @tparam  CONV_T2S              conversion function of type 'std::string (*CONV_T2S)(const R&)'.
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumn               'vector<R>' of column data (optional argument). By default, R is usually same type as T.
     *                                Else if 'CONV_T2S ≃ converter::ConvertFromVal_gNaN<T>::ToStr', then 'R = std::variant<T, std::string>'
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     * @param   pColumnName           column label name (optional argument)
     */
    //template< typename T, typename R, std::string (*CONV_T2S)(const R&) >
    template< auto CONV_T2S >
    inline void InsertColumn(const size_t pColumnIdx,
                             const std::vector<typename converter::f_T2Sconv_c<CONV_T2S>::input_type>& pColumn
                                 = std::vector<typename converter::f_T2Sconv_c<CONV_T2S>::input_type>(),
                             const std::string& pColumnName = std::string())
    {
      InsertColumn< converter::f_T2Sconv_c<CONV_T2S> >(pColumnIdx, pColumn, pColumnName);
    }

    /**
     * @brief   Get number of data columns (excluding label columns).
     * @returns column count.
     */
    inline size_t GetColumnCount() const
    {
      if(_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
      {
        return _mIdxColumnNames.size();
      } else {
        return _mData.at(0).size();
      }
    }

    /*
     *   To support getter and setter functions with variable of concept-type 'c_sizet_or_string',
     *   overloaded GetRowIdx(numeric-type) is needed.
     */
    constexpr size_t GetRowIdx(const size_t& pRowIdx) const
    {
      return pRowIdx;
    }
    constexpr size_t GetRowIdx(const int& pRowIdx) const
    {
      assert(pRowIdx >= 0);
      return static_cast<size_t>(pRowIdx);
    }
    /**
     * @brief   Get row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    size_t GetRowIdx(const std::string& pRowName) const
    {
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        if (_mRowNamesIdx.find(pRowName) != _mRowNamesIdx.end())
        {
          return _mRowNamesIdx.at(pRowName);
        } else {
          static const std::string errMsg("rapidcsv::Document::GetRowIdx(pRowName) row not found for 'pRowName'");
          RAPIDCSV_DEBUG_LOG(errMsg << " : pRowName='" << pRowName << "'");
          throw std::out_of_range(errMsg);
        }
      } else {
        static const std::string errMsg("rapidcsv::Document::GetRowIdx(pRowName) : '_mLabelParams.mRowNameFlg' is set as 'FlgRowName::RN_MISSING'");
        RAPIDCSV_DEBUG_LOG(errMsg << " : pRowName='" << pRowName << "'");
        throw std::out_of_range(errMsg);
      }
    }

    // TODO function and unit tests  for ARGS...
    /**
     * @brief   Get row either by it's index or name.
     * @tparam  T_C                   T can be data-type such as int, double etc ;   xOR
     *                                C -> Conversion class satisfying concept 'c_S2Tconverter'.
     * @param   pRowNameIdx           row-name or zero-based row index.
     * @returns 'tuple<R...>' of row data. By default, R is usually same type as T.
     *          Else if 'C ≃ converter::ConvertFromStr_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value, 
     *          else the string value which caused failure during conversion.
     */
    template< typename ... T_C >
    std::tuple<typename converter::t_S2Tconv_c<T_C>::return_type ...>
    GetRow(const c_sizet_or_string auto& pRowNameIdx) const
    {
      const size_t pRowIdx = GetRowIdx(pRowNameIdx);
      const t_dataRow& rowData = _mData.at(pRowIdx);

      if( rowData.size() < sizeof...(T_C) )
      {
        static const std::string errMsg("rapidcsv::Document::GetRow(pRowNameIdx) :: ERROR : row-size less than tuple size");
        RAPIDCSV_DEBUG_LOG(errMsg << " : pRowNameIdx='" << pRowNameIdx << "' , row-size=" << rowData.size()
                                  << " , tuple-size=" << sizeof...(T_C) );
        throw std::out_of_range(errMsg);
      }

      //return std::make_tuple(S2Tconv::ToVal(rowData.at(colIdx++)) ...);  on ubuntu the tuple elements order gets reversed

      using _t_tuple_result = std::tuple<typename converter::t_S2Tconv_c<T_C>::return_type...>;
      _t_tuple_result result;
      converter::GetTuple< converter::t_S2Tconv_c<T_C>... >(rowData,result);
      return result;
    }

    /**
     * @brief   Get row either by it's index or name.
     * @tparam  CONV_S2T              conversion function of type 'R (*CONV_S2T)(const std::string&)'.
     * @param   pRowNameIdx           row-name or zero-based row index.
     * @returns 'tuple<R...>' of row data. By default, R is usually same type as T.
     *          Else if 'CONV_S2T ≃ converter::ConvertFromStr_gNaN<T>::ToVal', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value, 
     *          else the string value which caused failure during conversion.
     */
    template< auto ... CONV_S2T >
    inline std::tuple< typename converter::f_S2Tconv_c< CONV_S2T >::return_type... >
    GetRow(const c_sizet_or_string auto& pRowNameIdx) const
    {
      return GetRow< converter::f_S2Tconv_c< CONV_S2T >... >(pRowNameIdx);
    }

    /**
     * @brief   Get row either by it's index or name.
     * @param   pRowNameIdx           row-name or zero-based row index.
     * @returns 'vector<std::string>' of row data
     */
    inline std::vector<std::string>
    GetRow_VecStr(const c_sizet_or_string auto& pRowNameIdx) const
    {
      const size_t pRowIdx = GetRowIdx(pRowNameIdx);
      return _mData.at(pRowIdx);
    }

    /**
     * @brief   Set row either by it's index or name.
     * @tparam  T_C                   T can be data-type such as int, double etc ;  xOR
     *                                C -> Conversion class satisfying concept 'c_T2Sconverter'.
     * @param   pRowNameIdx           row-name or zero-based row index.
     * @param   pRow                  'tuple<R...>' of row data. By default, R is usually same type as T.
     *                                Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     */
    // TODO unit tests  for customised C
    template< typename ... T_C >
    void SetRow(const c_sizet_or_string auto& pRowNameIdx,
                const std::tuple<typename converter::t_T2Sconv_c<T_C>::input_type ...>& pRow)
    {
      const size_t rowIdx = GetRowIdx(pRowNameIdx);
      _resizeTable(rowIdx, sizeof...(T_C) );

      t_dataRow& rowData = _mData.at(rowIdx);

      converter::SetTuple< converter::t_T2Sconv_c<T_C>... >(pRow,rowData);
    }

    /**
     * @brief   Set row either by it's index or name.
     * @tparam  CONV_T2S              conversion function of type 'std::string (*CONV_T2S)(const R&)'.
     * @param   pRowNameIdx           row-name or zero-based row index.
     * @param   pRow                  'tuple<R...>' of row data. By default, R is usually same type as T.
     *                                Else if 'CONV_T2S ≃ converter::ConvertFromVal_gNaN<T>::ToStr', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     */
    // TODO unit tests
    template< auto ... CONV_T2S >
    void SetRow(const c_sizet_or_string auto& pRowNameIdx,
                const std::tuple<typename converter::f_T2Sconv_c<CONV_T2S>::input_type...>& pRow)
    {
      SetRow< converter::f_T2Sconv_c<CONV_T2S>... >(pRowNameIdx, pRow);
    }

    /**
     * @brief   Set row either by it's index or name.
     * @param   pRowNameIdx           row-name or zero-based row index.
     * @param   pRow                  'vector<string>' of row data.
     */
    void SetRow_VecStr(const c_sizet_or_string auto& pRowNameIdx,
                       const std::vector<std::string>& pRow)
    {
      const size_t pRowIdx = GetRowIdx(pRowNameIdx);
      _resizeTable(pRowIdx, pRow.size());

      auto itRowCell = _mData.at(pRowIdx).begin();
      for(auto itEle = pRow.begin(); itEle != pRow.end(); ++itEle, ++itRowCell)
      {
        (*itRowCell) = (*itEle);
      }
    }

    /**
     * @brief   Remove row either by it's index or name.
     * @param   pRowNameIdx           row-name or zero-based row-index.
     */
    inline void RemoveRow(const c_sizet_or_string auto& pRowNameIdx)
    {
      const size_t pRowIdx = GetRowIdx(pRowNameIdx);
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        _mIdxRowNames.erase(_mIdxRowNames.begin() + static_cast<ssize_t>(pRowIdx));
      }
      _mData.erase(_mData.begin() + static_cast<ssize_t>(pRowIdx));
      _updateRowNames("rapidcsv::Document::RemoveRow()");
    }

    /**
     * @brief   Insert row at specified index.
     * @tparam  T_C                   T can be data-type such as int, double etc ;    xOR
     *                                C -> Conversion class satisfying concept 'c_T2Sconverter'.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  'tuple<R...>' of row data. By default, R is usually same type as T.
     *                                Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     * @param   pRowName              row label name (optional argument).
     */
    // TODO unit tests  for customised C
    template< typename ... T_C >
    void InsertRow(const size_t pRowIdx,
                   const std::tuple<typename converter::t_T2Sconv_c<T_C>::input_type ...>& pRow
                       = std::tuple<typename converter::t_T2Sconv_c<T_C>::input_type ...>(),
                   const std::string& pRowName = std::string())
    {
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_MISSING && !pRowName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::InsertRow() : _mLabelParams.mRowNameFlg = FlgRowName::RN_MISSING -> can't process Row-Name");
        throw std::out_of_range(errMsg);
      }

      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT && pRowName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::InsertRow() : _mLabelParams.mRowNameFlg = FlgRowName::RN_PRESENT -> Row-Name can't be empty string");
        throw std::out_of_range(errMsg);
      }

      t_dataRow rowData;
      if constexpr (sizeof...(T_C) == 0)
      {
        rowData.resize(_getDataColumnCount());
      } else {
        rowData.resize(sizeof...(T_C));

        converter::SetTuple< converter::t_T2Sconv_c<T_C>... >(pRow, rowData);
      }

      while (pRowIdx > _getDataRowCount())
      {
        t_dataRow tempRow;
        tempRow.resize(_getDataColumnCount());
        _mData.push_back(tempRow);
      }

      _mData.insert(_mData.begin() + static_cast<ssize_t>(pRowIdx), rowData);

      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        _mIdxRowNames.insert(_mIdxRowNames.begin() + static_cast<ssize_t>(pRowIdx), pRowName);
      }

      _updateRowNames("rapidcsv::Document::InsertRow()");
    }

    /**
     * @brief   Insert row at specified index.
     * @tparam  CONV_T2S              conversion function of type 'std::string (*CONV_T2S)(const R&)'.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  'tuple<R...>' of row data. By default, R is usually same type as T.
     *                                Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     * @param   pRowName              row label name (optional argument).
     */
    // TODO unit tests
    template<auto ... CONV_T2S>
    inline void InsertRow(const size_t pRowIdx,
                          const std::tuple<typename converter::f_T2Sconv_c<CONV_T2S>::input_type...>& pRow
                              = std::tuple<typename converter::f_T2Sconv_c<CONV_T2S>::input_type...>(),
                          const std::string& pRowName = std::string())
    {
      InsertRow< converter::f_T2Sconv_c<CONV_T2S>... >(pRowIdx, pRow, pRowName);
    }

    /**
     * @brief   Insert row at specified index.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  'vector<string>' of row data (optional argument).
     * @param   pRowName              row label name (optional argument).
     */
    void InsertRow_VecStr(const size_t pRowIdx,
                          const std::vector<std::string>& pRow
                                         = std::vector<std::string>(),
                          const std::string& pRowName = std::string())
    {
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_MISSING && !pRowName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::InsertRow_VecStr() : _mLabelParams.mRowNameFlg = FlgRowName::RN_MISSING -> can't process Row-Name");
        throw std::out_of_range(errMsg);
      }

      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT && pRowName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::InsertRow_VecStr() : _mLabelParams.mRowNameFlg = FlgRowName::RN_PRESENT -> Row-Name can't be empty string");
        throw std::out_of_range(errMsg);
      }

      t_dataRow row;
      if (pRow.empty())
      {
        row.resize(_getDataColumnCount());
      } else {
        row.resize(pRow.size());
        size_t colIdx = 0;
        for (auto itCol = pRow.begin(); itCol != pRow.end(); ++itCol, ++colIdx)
        {
          row.at(colIdx) = (*itCol);
        }
      }

      while (pRowIdx > _getDataRowCount())
      {
        t_dataRow tempRow;
        tempRow.resize(_getDataColumnCount());
        _mData.push_back(tempRow);
      }

      _mData.insert(_mData.begin() + static_cast<ssize_t>(pRowIdx), row);

      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        SetRowName(pRowIdx, pRowName);
      }

      _updateRowNames("rapidcsv::Document::InsertRow_VecStr()");
    }

    /**
     * @brief   Get number of data rows (excluding label rows).
     * @returns row count.
     */
    inline size_t GetRowCount() const
    {
      if(_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        return _mIdxRowNames.size();
      } else {
        return _mData.size();
      }
    }

    // TODO function and unit tests  for ARGS...
    /**
     * @brief   Get cell either by it's index or name.
     * @tparam  T_C                   T can be data-type such as int, double etc ;   xOR
     *                                C -> Conversion class satisfying concept 'c_S2Tconverter'.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @param   pRowNameIdx           row-name or zero-based row-index.
     * @returns cell data of type R. By default, R is usually same type as T.
     *          Else if 'C ≃ converter::ConvertFromStr_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value,
     *          else the string value which caused failure during conversion.
     */
    template< typename T_C >
    typename converter::t_S2Tconv_c<T_C>::return_type
    GetCell(const c_sizet_or_string auto& pColumnNameIdx,
            const c_sizet_or_string auto& pRowNameIdx) const
    {
      const size_t pColumnIdx = GetColumnIdx(pColumnNameIdx);
      const size_t pRowIdx = GetRowIdx(pRowNameIdx);

      return converter::t_S2Tconv_c<T_C>::ToVal(_mData.at(pRowIdx).at(pColumnIdx));
    }

    // TODO function and unit tests  for ARGS...
    /**
     * @brief   Get cell either by it's index or name.
     * @tparam  CONV_S2T              conversion function.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @param   pRowNameIdx           row-name or zero-based row-index.
     * @returns cell data of type R. By default, R is usually same type as T.
     *          Else if 'CONV_S2T ≃ converter::ConvertFromStr_gNaN<T>::ToVal', then 'R = std::variant<T, std::string>'.
     *          On conversion success variant has the converted value,
     *          else the string value which caused failure during conversion.
     */
    template< auto (*CONV_S2T)(const std::string&) >
    inline typename converter::f_S2Tconv_c< CONV_S2T >::return_type
    GetCell(const c_sizet_or_string auto& pColumnNameIdx,
            const c_sizet_or_string auto& pRowNameIdx) const
    {
      return GetCell< converter::f_S2Tconv_c< CONV_S2T > >(pColumnNameIdx, pRowNameIdx);
    }

    // TODO function and unit tests  for ARGS...
    /**
     * @brief   Set cell either by it's index or name.
     * @tparam  T_C                   T can be data-type such as int, double etc ;     xOR
     *                                C -> Conversion class satisfying concept 'c_T2Sconverter'.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @param   pRowNameIdx           row-name or zero-based row-index.
     * @param   pCell                 cell data. By default, R is usually same type as T.
     *                                Else if 'C ≃ converter::ConvertFromVal_gNaN<T>', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     */
    template< typename T_C >
    void SetCell(const c_sizet_or_string auto& pColumnNameIdx,
                 const c_sizet_or_string auto& pRowNameIdx,
                 const typename converter::t_T2Sconv_c<T_C>::input_type& pCell)
    {
      const size_t pColumnIdx = GetColumnIdx(pColumnNameIdx);
      const size_t pRowIdx = GetRowIdx(pRowNameIdx);

      while ((pRowIdx + 1) > _getDataRowCount())
      {
        t_dataRow row;
        row.resize(_getDataColumnCount());
        _mData.push_back(row);
      }

      if ((pColumnIdx + 1) > _getDataColumnCount())
      {
        for (auto itRow = _mData.begin(); itRow != _mData.end(); ++itRow)
        {
          itRow->resize(pColumnIdx + 1);
        }
      }

      _mData.at(pRowIdx).at(pColumnIdx) = converter::t_T2Sconv_c<T_C>::ToStr(pCell);
    }

    // TODO function and unit tests  for ARGS...
    /**
     * @brief   Set cell either by it's index or name.
     * @tparam  CONV_T2S              conversion function of type 'std::string (*CONV_T2S)(const R&)'.
     * @param   pColumnNameIdx        column-name or zero-based column-index.
     * @param   pRowNameIdx           row-name or zero-based row-index.
     * @param   pCell                 cell data. By default, R is usually same type as T.
     *                                Else if 'CONV_T2S ≃ converter::ConvertFromVal_gNaN<T>::ToStr', then 'R = std::variant<T, std::string>'.
     *                                On conversion success variant has the converted value,
     *                                else the string value which caused failure during conversion.
     */
    //template<typename T, typename R, std::string (*CONV_T2S)(const R&) >
    template<auto CONV_T2S >
    inline void SetCell(const c_sizet_or_string auto& pColumnNameIdx,
                        const c_sizet_or_string auto& pRowNameIdx,
                        const typename converter::f_T2Sconv_c<CONV_T2S>::input_type& pCell)
    {
      SetCell< converter::f_T2Sconv_c<CONV_T2S> >(pColumnNameIdx, pRowNameIdx, pCell);
    }

    /**
     * @brief   Get column name
     * @param   pColumnIdx            zero-based column index.
     * @returns column name.
     */
    inline std::string GetColumnName(const size_t pColumnIdx) const
    {
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_MISSING)
      {
        static const std::string errMsg("rapidcsv::Document::GetColumnName(pColumnIdx) : _mLabelParams.mColumnNameFlg = FlgColumnName::CN_MISSING");
        throw std::out_of_range(errMsg);
      }

      return _mIdxColumnNames.at(pColumnIdx);
    }

    /**
     * @brief   Set column name
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumnName           column name.
     */
    void SetColumnName(const size_t pColumnIdx, const std::string& pColumnName)
    {
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_MISSING)
      {
        static const std::string errMsg("rapidcsv::Document::SetColumnName() : _mLabelParams.mColumnNameFlg = FlgColumnName::CN_MISSING");
        throw std::out_of_range(errMsg);
      }

      if (pColumnName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::SetColumnName() : _mLabelParams.mColumnNameFlg = FlgColumnName::CN_PRESENT -> Column-Name cann't be empty string");
        throw std::out_of_range(errMsg);
      }

      auto search = _mColumnNamesIdx.find(pColumnName);
      if( search != _mColumnNamesIdx.end() )
      {
        if (search->second == pColumnIdx)
          return; // same data exists: do nothing : return

        static const std::string errMsg("rapidcsv::Document::SetColumnName() : 'pColumnIdx' has to be unique, duplicate names not allowed");
        RAPIDCSV_DEBUG_LOG(errMsg << " : pColumnIdx='" << pColumnIdx << "'");
        throw std::out_of_range(errMsg);
      }

      if (pColumnIdx >= _mIdxColumnNames.size())
      {
        _mIdxColumnNames.resize(pColumnIdx + 1);
      } else {
        std::string prvName = _mIdxColumnNames.at(pColumnIdx);
        _mColumnNamesIdx.erase(prvName);
      }

      _mColumnNamesIdx[pColumnName] = pColumnIdx;
      _mIdxColumnNames.at(pColumnIdx) = pColumnName;
    }

    /**
     * @brief   Get column names
     * @returns vector of column names.
     */
    inline std::vector<std::string> GetColumnNames() const
    {
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
      {
        return _mIdxColumnNames;
      }

      return std::vector<std::string>();
    }

    /**
     * @brief   Get row name
     * @param   pRowIdx               zero-based column index.
     * @returns row name.
     */
    inline std::string GetRowName(const size_t pRowIdx) const
    {
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_MISSING)
      {
        static const std::string errMsg("rapidcsv::Document::GetRowName(pRowIdx) : _mLabelParams.mRowNameFlg == FlgRowName::RN_MISSING");
        throw std::out_of_range(errMsg);
      }

      return _mIdxRowNames.at(pRowIdx);
    }

    /**
     * @brief   Set row name
     * @param   pRowIdx               zero-based row index.
     * @param   pRowName              row name.
     */
    void SetRowName(size_t pRowIdx, const std::string& pRowName)
    {
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_MISSING)
      {
        static const std::string errMsg("rapidcsv::Document::SetRowName() : _mLabelParams.mRowNameFlg == FlgRowName::RN_MISSING");
      }

      if (pRowName.empty())
      {
        static const std::string errMsg("rapidcsv::Document::SetRowName() : _mLabelParams.mRowNameFlg = FlgRowName::RN_PRESENT -> Row-Name cann't be empty string");
        throw std::out_of_range(errMsg);
      }

      auto search = _mRowNamesIdx.find(pRowName);
      if( search != _mRowNamesIdx.end() )
      {
        if (search->second == pRowIdx)
          return; // same data exists: do nothing : return

        static const std::string errMsg("rapidcsv::Document::SetRowName() : 'pRowName' has to be unique, duplicate names not allowed");
        RAPIDCSV_DEBUG_LOG(errMsg << " : pRowName='" << pRowName << "'");
        throw std::out_of_range(errMsg);
      }

      if (pRowIdx >= _mIdxRowNames.size())
      {
        _mIdxRowNames.resize(pRowIdx + 1);
      } else {
        std::string prvName = _mIdxRowNames.at(pRowIdx);
        _mRowNamesIdx.erase(prvName);
      }

      _mRowNamesIdx[pRowName] = pRowIdx;
      _mIdxRowNames.at(pRowIdx) = pRowName;
    }

    /**
     * @brief   Get row names
     * @returns vector of row names.
     */
    inline std::vector<std::string> GetRowNames() const
    {
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        return _mIdxRowNames;
      }

      return std::vector<std::string>();
    }

  private:
    void _readCsv()
    {
      std::ifstream stream;
      stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      stream.open(_mPath, std::ios::binary);
      _readCsv(stream);
    }

    void _readCsv(std::istream& pStream)
    {
      Clear();
      pStream.seekg(0, std::ios::end);
      std::streamsize length = pStream.tellg();
      pStream.seekg(0, std::ios::beg);

#ifdef HAS_CODECVT
      std::vector<char> bom2b(2, '\0');
      if (length >= 2)
      {
        pStream.read(bom2b.data(), 2);
        pStream.seekg(0, std::ios::beg);
      }

      static const std::vector<char> bomU16le = { '\xff', '\xfe' };
      static const std::vector<char> bomU16be = { '\xfe', '\xff' };
      if ((bom2b == bomU16le) || (bom2b == bomU16be))
      {
        mIsUtf16 = true;
        mIsLE = (bom2b == bomU16le);

        std::wifstream wstream;
        wstream.exceptions(std::wifstream::failbit | std::wifstream::badbit);
        wstream.open(_mPath, std::ios::binary);
        if (mIsLE)
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff,
                                                           static_cast<std::codecvt_mode>(std::consume_header |
                                                                                          std::little_endian)>));
        }
        else
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff,
                                                           std::consume_header>));
        }
        std::wstringstream wss;
        wss << wstream.rdbuf();
        std::string utf8 = _toString(wss.str());
        std::stringstream ss(utf8);
        _parseCsv(ss, static_cast<std::streamsize>(utf8.size()));
      }
      else
#endif
      {
        // check for UTF-8 Byte order mark and skip it when found
        if (length >= 3)
        {
          std::vector<char> bom3b(3, '\0');
          pStream.read(bom3b.data(), 3);

          if (bom3b != s_Utf8BOM)
          {
            // file does not start with a UTF-8 Byte order mark
            pStream.seekg(0, std::ios::beg);
          }
          else
          {
            // file did start with a UTF-8 Byte order mark, simply skip it
            length -= 3;
            _mHasUtf8BOM = true;
          }
        }

        _parseCsv(pStream, length);
      }
    }

    void _parseCsv(std::istream& pStream, std::streamsize p_FileLength)
    {
      const std::streamsize bufLength = 64 * 1024;
      std::vector<char> buffer(bufLength);
      t_dataRow row;
      std::string cell;
      bool quoted = false;
      int cr = 0;
      int lf = 0;

      while (p_FileLength > 0)
      {
        const std::streamsize toReadLength = std::min<std::streamsize>(p_FileLength, bufLength);
        pStream.read(buffer.data(), toReadLength);

        // With user-specified istream opened in non-binary mode on windows, we may have a
        // data length mismatch, so ensure we don't parse outside actual data length read.
        const std::streamsize readLength = pStream.gcount();
        if (readLength <= 0)
        {
          break;
        }

        for (size_t i = 0; i < static_cast<size_t>(readLength); ++i)
        {
          if (buffer[i] == _mSeparatorParams.mQuoteChar)
          {
            if (cell.empty() || (cell[0] == _mSeparatorParams.mQuoteChar))
            {
              quoted = !quoted;
            }
            cell += buffer[i];
          }
          else if (buffer[i] == _mSeparatorParams.mSeparator)
          {
            if (!quoted)
            {
              row.push_back(_unquote(_trim(cell)));
              cell.clear();
            }
            else
            {
              cell += buffer[i];
            }
          }
          else if (buffer[i] == '\r')
          {
            if (_mSeparatorParams.mQuotedLinebreaks && quoted)
            {
              cell += buffer[i];
            }
            else
            {
              ++cr;
            }
          }
          else if (buffer[i] == '\n')
          {
            if (_mSeparatorParams.mQuotedLinebreaks && quoted)
            {
              cell += buffer[i];
            }
            else
            {
              ++lf;
              if (_mLineReaderParams.mSkipEmptyLines && row.empty() && cell.empty())
              {
                // skip empty line
              }
              else
              {
                row.push_back(_unquote(_trim(cell)));

                if (_mLineReaderParams.mSkipCommentLines && !row.at(0).empty() &&
                    (row.at(0)[0] == _mLineReaderParams.mCommentPrefix))
                {
                  // skip comment line
                }
                else
                {
                  _mData.push_back(row);
                }

                cell.clear();
                row.clear();
                quoted = false;
              }
            }
          }
          else
          {
            cell += buffer[i];
          }
        }
        p_FileLength -= readLength;
      }

      // Handle last line without linebreak
      if (!cell.empty() || !row.empty())
      {
        row.push_back(_unquote(_trim(cell)));
        cell.clear();
        _mData.push_back(row);
        row.clear();
      }

      // Assume CR/LF if at least half the linebreaks have CR
      _mSeparatorParams.mHasCR = (cr > (lf / 2));

      _mFirstCornerCell = "";
      if ( _mData.size() && ( _mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT ||
                              _mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT) )
      {
        auto& firstRow = _mData.at(0);
        if ( firstRow.size() && ( _mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT &&
                                  _mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT) )
        {
          _mFirstCornerCell = firstRow.at(0);

          // remove corner cell, as next step column-headers are extracted
          firstRow.erase(firstRow.begin());
        }

        if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
        {
          _mIdxColumnNames = firstRow;
          _mData.erase(_mData.begin()); // remove column-header from Data
        }

        if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
        {
          _mIdxRowNames.clear();
          for(auto& rowN : _mData)
          {
            _mIdxRowNames.push_back(rowN.at(0));
            rowN.erase(rowN.begin()); // remove row-header from datas
          }
        }
      }

      // Set up column labels
      _updateColumnNames("rapidcsv::Document::_parseCsv()");

      // Set up row labels
      _updateRowNames("rapidcsv::Document::_parseCsv()");
    }

    void _writeCsv() const
    {
#ifdef HAS_CODECVT
      if (mIsUtf16)
      {
        std::stringstream ss;
        _writeCsv(ss);
        std::string utf8 = ss.str();
        std::wstring wstr = _toWString(utf8);

        std::wofstream wstream;
        wstream.exceptions(std::wofstream::failbit | std::wofstream::badbit);
        wstream.open(_mPath, std::ios::binary | std::ios::trunc);

        if (mIsLE)
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff,
                                                           static_cast<std::codecvt_mode>(std::little_endian)>));
        }
        else
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff>));
        }

        wstream << static_cast<wchar_t>(0xfeff);
        wstream << wstr;
      }
      else
#endif
      {
        std::ofstream stream;
        stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        stream.open(_mPath, std::ios::binary | std::ios::trunc);
        if (_mHasUtf8BOM)
        {
          stream.write(s_Utf8BOM.data(), 3);
        }

        _writeCsv(stream);
      }
    }

    void _writeCsv(std::ostream& pStream) const
    {
      if ( _mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT &&
           _mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        _writeCell(pStream, _mFirstCornerCell);
        if(_mIdxColumnNames.size())
        {
          pStream << _mSeparatorParams.mSeparator;
        }
      }

      if ( _mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT )
      {
        for (auto columnNameIter = _mIdxColumnNames.begin(); columnNameIter != _mIdxColumnNames.end(); ++columnNameIter )
        {
          _writeCell(pStream, *columnNameIter);

          if (std::distance(columnNameIter, _mIdxColumnNames.end()) > 1)
          {
            pStream << _mSeparatorParams.mSeparator;
          }
        }
        pStream << (_mSeparatorParams.mHasCR ? "\r\n" : "\n");
      }

      size_t rowIdx = 0;
      for (auto itr = _mData.begin(); itr != _mData.end(); ++itr, ++rowIdx)
      {
        if(_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
        {
          _writeCell(pStream, _mIdxRowNames.at(rowIdx));
          if(itr->size())
          {
            pStream << _mSeparatorParams.mSeparator;
          }
        }

        for (auto itc = itr->begin(); itc != itr->end(); ++itc)
        {
          _writeCell(pStream, *itc);

          if (std::distance(itc, itr->end()) > 1)
          {
            pStream << _mSeparatorParams.mSeparator;
          }
        }
        pStream << (_mSeparatorParams.mHasCR ? "\r\n" : "\n");
      }
    }

    void _writeCell(std::ostream& pStream, std::string cellVal) const
    {
      if (_mSeparatorParams.mAutoQuote &&
          ((cellVal.find(_mSeparatorParams.mSeparator) != std::string::npos) ||
           (cellVal.find(' ') != std::string::npos)))
      {
        // escape quotes in string
        const std::string quoteCharStr = std::string(1, _mSeparatorParams.mQuoteChar);
        _replaceString(cellVal, quoteCharStr, quoteCharStr + quoteCharStr);

        pStream << quoteCharStr << cellVal << quoteCharStr;
      }
      else
      {
        pStream << cellVal;
      }
    }

    inline size_t _getDataRowCount() const
    {
      return _mData.size();
    }

    inline size_t _getDataColumnCount() const
    {
      return (_mData.size() > 0) ? _mData.at(0).size() : 0;
    }

    std::string _trim(const std::string& pStr) const
    {
      if (_mSeparatorParams.mTrim)
      {
        std::string str = pStr;

        // ltrim
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) { return !isspace(ch); }));

        // rtrim
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) { return !isspace(ch); }).base(), str.end());

        return str;
      }
      else
      {
        return pStr;
      }
    }

    std::string _unquote(const std::string& pStr) const
    {
      if (_mSeparatorParams.mAutoQuote && (pStr.size() >= 2) &&
          (pStr.front() == _mSeparatorParams.mQuoteChar) &&
          (pStr.back() == _mSeparatorParams.mQuoteChar))
      {
        // remove start/end quotes
        std::string str = pStr.substr(1, pStr.size() - 2);

        // unescape quotes in string
        const std::string quoteCharStr = std::string(1, _mSeparatorParams.mQuoteChar);
        _replaceString(str, quoteCharStr + quoteCharStr, quoteCharStr);

        return str;
      }
      else
      {
        return pStr;
      }
    }

    void _updateColumnNames([[maybe_unused]]const std::string& calleeFunction)
    {
      _mColumnNamesIdx.clear();
      if (_mLabelParams.mColumnNameFlg == FlgColumnName::CN_PRESENT)
      {
        size_t i = 0;
        for (auto& columnName : _mIdxColumnNames)
        {
          if (columnName.empty())
          {
            static const std::string errMsg("rapidcsv::Document::_updateColumnNames() : _mLabelParams.mColumnNameFlg = FlgColumnName::CN_PRESENT -> Column-Name can't be empty string");
            RAPIDCSV_DEBUG_LOG(errMsg << " : index=" << i << " calleeFunction='" << calleeFunction << "'");
            throw std::out_of_range(errMsg);
          }

          if( _mColumnNamesIdx.find(columnName) != _mColumnNamesIdx.end() )
          {
            static const std::string errMsg("rapidcsv::Document::_updateColumnNames() : 'columnName' has to be unique, duplicate names not allowed");
            RAPIDCSV_DEBUG_LOG(errMsg << " : columnName='" << columnName << "'");
            throw std::out_of_range(errMsg);
          }

          _mColumnNamesIdx[columnName] = i++;
        }
      } else {
        _mIdxColumnNames.clear();
      }
    }

    void _updateRowNames([[maybe_unused]]const std::string& calleeFunction)
    {
      _mRowNamesIdx.clear();
      if (_mLabelParams.mRowNameFlg == FlgRowName::RN_PRESENT)
      {
        size_t i = 0;
        for (auto& rowName : _mIdxRowNames)
        {
          if (rowName.empty())
          {
            static const std::string errMsg("rapidcsv::Document::_updateRowNames() : _mLabelParams.mRowNameFlg = FlgRowName::RN_PRESENT -> Row-Name can't be empty string");
            RAPIDCSV_DEBUG_LOG(errMsg << " : index=" << i << " calleeFunction='" << calleeFunction << "'");
            throw std::out_of_range(errMsg);
          }

          if( _mRowNamesIdx.find(rowName) != _mRowNamesIdx.end() )
          {
            static const std::string errMsg("rapidcsv::Document::_updateRowNames() : 'rowName' has to be unique, duplicate names not allowed");
            RAPIDCSV_DEBUG_LOG(errMsg << " : rowName='" << rowName << "'");
            throw std::out_of_range(errMsg);
          }

          _mRowNamesIdx[rowName] = i++;
        }
      } else {
        _mIdxRowNames.clear();
      }
    }

#ifdef HAS_CODECVT
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable: 4996)
#endif
    static std::string _toString(const std::wstring& pWStr)
    {
      return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{ }.to_bytes(pWStr);
    }

    static std::wstring _toWString(const std::string& pStr)
    {
      return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{ }.from_bytes(pStr);
    }
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
#endif

    static void _replaceString(std::string& pStr, const std::string& pSearch, const std::string& pReplace)
    {
      size_t pos = 0;

      while ((pos = pStr.find(pSearch, pos)) != std::string::npos)
      {
        pStr.replace(pos, pSearch.size(), pReplace);
        pos += pReplace.size();
      }
    }

    std::string _mPath;
    LabelParams _mLabelParams;
    SeparatorParams _mSeparatorParams;
    LineReaderParams _mLineReaderParams;
    std::vector<t_dataRow> _mData;
    std::map<std::string, size_t> _mColumnNamesIdx;
    std::map<std::string, size_t> _mRowNamesIdx;
    std::vector<std::string>      _mIdxColumnNames;
    std::vector<std::string>      _mIdxRowNames;
    std::string                   _mFirstCornerCell;  // applicable only when both Row and Column Lables are PRESENT
#ifdef HAS_CODECVT
    bool mIsUtf16 = false;
    bool mIsLE = false;
#endif
    bool _mHasUtf8BOM = false;


    void _resizeTable(const size_t pRowIdx, const size_t pRowSize)
    {
      while ((pRowIdx + 1) > _getDataRowCount())
      {
        t_dataRow row;
        row.resize(_getDataColumnCount());
        _mData.push_back(row);
      }

      if (pRowSize > _getDataColumnCount())
      {
        for (auto itRow = _mData.begin(); itRow != _mData.end(); ++itRow)
        {
          itRow->resize(pRowSize);
        }
      }
    }

    friend class _ViewDocument;
  };
}
