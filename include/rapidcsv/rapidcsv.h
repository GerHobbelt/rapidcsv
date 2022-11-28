/*
 * rapidcsv.h
 *
 * URL:      https://github.com/panchaBhuta/rapidcsv_CT
 * Version:  1.0.ct-8.67
 *
 * Copyright (C) 2022-2022 Gautam Dhar
 * All rights reserved.
 * rapidcsv_CT is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 *
 * ***********************************************************************************
 *
 * URL:      https://github.com/d99kris/rapidcsv
 * Version:  8.68
 *
 * Copyright (C) 2017-2022 Kristofer Berggren
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

#include <rapidcsv/converter.h>

namespace rapidcsv
{
#if defined(_MSC_VER)
  static const bool sPlatformHasCR = true;
#else
  static const bool sPlatformHasCR = false;
#endif


  /**
   * @brief     Datastructure holding parameters controlling which row and column should be
   *            treated as labels.
   */
  struct LabelParams
  {
    /**
     * @brief   Constructor
     * @param   pColumnNameIdx        specifies the zero-based row index of the column labels, setting
     *                                it to -1 prevents column lookup by label name, and gives access
     *                                to all rows as document data. Default: 0
     * @param   pRowNameIdx           specifies the zero-based column index of the row labels, setting
     *                                it to -1 prevents row lookup by label name, and gives access
     *                                to all columns as document data. Default: -1
     */
    explicit LabelParams(const ssize_t pColumnNameIdx = 0, const ssize_t pRowNameIdx = -1)
      : mColumnNameIdx(pColumnNameIdx)
      , mRowNameIdx(pRowNameIdx)
    {
      if (mColumnNameIdx < -1)
      {
        const std::string errStr = "invalid column name index " +
          std::to_string(mColumnNameIdx) + " < -1";
        throw std::out_of_range(errStr);
      }

      if (mRowNameIdx < -1)
      {
        const std::string errStr = "invalid row name index " +
          std::to_string(mRowNameIdx) + " < -1";
        throw std::out_of_range(errStr);
      }
    }

    /**
     * @brief   specifies the zero-based row index of the column labels.
     */
    ssize_t mColumnNameIdx;

    /**
     * @brief   specifies the zero-based column index of the row labels.
     */
    ssize_t mRowNameIdx;
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
    {
    }

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
    {
    }

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

//  class SortParams;

  /**
   * @brief     Class representing a CSV document.
   */
  class Document
  {
  public:

    using t_dataRow = std::vector<std::string>;
    typedef bool (*f_EvalBoolExpr)(const t_dataRow& dataRow);

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
      : mPath(pPath)
      , mLabelParams(pLabelParams)
      , mSeparatorParams(pSeparatorParams)
      , mLineReaderParams(pLineReaderParams)
      , mData()
      , mColumnNames()
      , mRowNames()
    {
      if (!mPath.empty())
      {
        ReadCsv();
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
      : mPath()
      , mLabelParams(pLabelParams)
      , mSeparatorParams(pSeparatorParams)
      , mLineReaderParams(pLineReaderParams)
      , mData()
      , mColumnNames()
      , mRowNames()
    {
      ReadCsv(pStream);
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
      mPath = pPath;
      mLabelParams = pLabelParams;
      mSeparatorParams = pSeparatorParams;
      mLineReaderParams = pLineReaderParams;
      ReadCsv();
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
      mPath = "";
      mLabelParams = pLabelParams;
      mSeparatorParams = pSeparatorParams;
      mLineReaderParams = pLineReaderParams;
      ReadCsv(pStream);
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
        mPath = pPath;
      }
      WriteCsv();
    }

    /**
     * @brief   Write Document data to stream.
     * @param   pStream               specifies a binary output stream to write the data to.
     */
    void Save(std::ostream& pStream)
    {
      WriteCsv(pStream);
    }

    /**
     * @brief   Clears loaded Document data.
     *
     */
    void Clear()
    {
      mData.clear();
      mColumnNames.clear();
      mRowNames.clear();
#ifdef HAS_CODECVT
      mIsUtf16 = false;
      mIsLE = false;
#endif
    }

    /**
     * @brief   Get column index by name.
     * @param   pColumnName           column label name.
     * @returns zero-based column index.
     */
    ssize_t GetColumnIdx(const std::string& pColumnName) const
    {
      if (mLabelParams.mColumnNameIdx >= 0)
      {
        if (mColumnNames.find(pColumnName) != mColumnNames.end())
        {
          return static_cast<ssize_t>(mColumnNames.at(pColumnName)) - static_cast<ssize_t>(GetDataColumnIndex(0));
        }
      }
      return -1;
    }

    /**
     * @brief   Get column by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of column data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetColumn(const size_t pColumnIdx,
                             f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);
      std::vector<T> column;
      size_t rowIdx = 0;
      for (auto itRow = mData.begin()+static_cast<ssize_t>(GetDataRowIndex(0));
           itRow != mData.end(); ++itRow, ++rowIdx)
      {
        if (dataColumnIdx < itRow->size())
        {
          const std::string& cellStrVal = itRow->at(dataColumnIdx);
          T val = pConvertToVal(cellStrVal);
          column.push_back(val);
        } else {
          const std::string errStr = "Document::GetColumn() # requested column index " +
            std::to_string(pColumnIdx) + " >= " +
            std::to_string(itRow->size() - GetDataColumnIndex(0)) +
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
     * @returns vector of column data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetColumn(const std::string& pColumnName,
                             f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      return GetColumn<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(columnIdx), pConvertToVal);
    }

    /**
     * @brief   Set column by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumn               vector of column data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetColumn(const size_t pColumnIdx, const std::vector<T>& pColumn,
                   f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);

      while (GetDataRowIndex(pColumn.size()) > GetDataRowCount())
      {
        t_dataRow row;
        row.resize(GetDataColumnCount());
        mData.push_back(row);
      }

      if ((dataColumnIdx + 1) > GetDataColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(GetDataColumnIndex(dataColumnIdx + 1));
        }
      }

      size_t rowIdx = GetDataRowIndex(0);
      for (auto itRow = pColumn.begin(); itRow != pColumn.end(); ++itRow, ++rowIdx)
      {
        mData.at(rowIdx).at(dataColumnIdx) = pConvertToStr(*itRow);
      }
    }

    /**
     * @brief   Set column by name.
     * @param   pColumnName           column label name.
     * @param   pColumn               vector of column data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetColumn(const std::string& pColumnName, const std::vector<T>& pColumn,
                   f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      SetColumn<T,USE_NUMERIC_LOCALE>(static_cast<size_t>(columnIdx), pColumn, pConvertToStr);
    }

    /**
     * @brief   Remove column by index.
     * @param   pColumnIdx            zero-based column index.
     */
    void RemoveColumn(const size_t pColumnIdx)
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        itRow->erase(itRow->begin() + static_cast<ssize_t>(dataColumnIdx));
      }

      UpdateColumnNames();
    }

    /**
     * @brief   Remove column by name.
     * @param   pColumnName           column label name.
     */
    void RemoveColumn(const std::string& pColumnName)
    {
      ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      RemoveColumn(static_cast<size_t>(columnIdx));
    }

    /**
     * @brief   Insert column at specified index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumn               vector of column data (optional argument).
     * @param   pColumnName           column label name (optional argument).
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void InsertColumn(const size_t pColumnIdx, const std::vector<T>& pColumn = std::vector<T>(),
                      const std::string& pColumnName = std::string(),
                      f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);

      std::vector<std::string> column;
      if (pColumn.empty())
      {
        column.resize(GetDataRowCount());
      }
      else
      {
        column.resize(GetDataRowIndex(pColumn.size()));
        size_t rowIdx = GetDataRowIndex(0);
        for (auto itRow = pColumn.begin(); itRow != pColumn.end(); ++itRow, ++rowIdx)
        {
          column.at(rowIdx) = pConvertToStr(*itRow);
        }
      }

      while (column.size() > GetDataRowCount())
      {
        t_dataRow row;
        const size_t columnCount = std::max<size_t>(GetDataRowIndex(0), GetDataColumnCount());
        row.resize(columnCount);
        mData.push_back(row);
      }

      size_t rowIdx = 0;
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow, ++rowIdx)
      {
        itRow->insert(itRow->begin() + static_cast<ssize_t>(dataColumnIdx), column.at(rowIdx));
      }

      if (!pColumnName.empty())
      {
        SetColumnName(pColumnIdx, pColumnName);
      }

      UpdateColumnNames();
    }

    /**
     * @brief   Get number of data columns (excluding label columns).
     * @returns column count.
     */
    size_t GetColumnCount() const
    {
      const size_t firstRowSize = ((mData.size() > 0) ? mData.at(0).size() : 0);
      const size_t firstDataColumnIndex = GetDataColumnIndex(0);
      return ((firstRowSize>firstDataColumnIndex) ? (firstRowSize-firstDataColumnIndex) : 0);
    }

    /**
     * @brief   Get row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    ssize_t GetRowIdx(const std::string& pRowName) const
    {
      if (mLabelParams.mRowNameIdx >= 0)
      {
        if (mRowNames.find(pRowName) != mRowNames.end())
        {
          return static_cast<ssize_t>(mRowNames.at(pRowName)) - static_cast<ssize_t>(GetDataRowIndex(0));
        }
      }
      return -1;
    }

    /**
     * @brief   Get row by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetRow(const size_t pRowIdx,
                          f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);
      std::vector<T> row;
      const ssize_t colIdx = static_cast<ssize_t>(GetDataColumnIndex(0));
      for (auto itCol = mData.at(dataRowIdx).begin() + colIdx; itCol != mData.at(dataRowIdx).end(); ++itCol)
      {
        T val = pConvertToVal(*itCol);
        row.push_back(val);
      }
      return row;
    }

    /**
     * @brief   Get row by name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns vector of row data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    std::vector<T> GetRow(const std::string& pRowName,
                          f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }
      return GetRow<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(rowIdx), pConvertToVal);
    }

    /**
     * @brief   Set row by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  vector of row data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetRow(const size_t pRowIdx, const std::vector<T>& pRow,
                f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);

      while ((dataRowIdx + 1) > GetDataRowCount())
      {
        t_dataRow row;
        row.resize(GetDataColumnCount());
        mData.push_back(row);
      }

      if (pRow.size() > GetDataColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(GetDataColumnIndex(pRow.size()));
        }
      }

      size_t colIdx = GetDataColumnIndex(0);
      for (auto itCol = pRow.begin(); itCol != pRow.end(); ++itCol, ++colIdx)
      {
        mData.at(dataRowIdx).at(colIdx) = pConvertToStr(*itCol);
      }
    }

    /**
     * @brief   Set row by name.
     * @param   pRowName              row label name.
     * @param   pRow                  vector of row data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetRow(const std::string& pRowName, const std::vector<T>& pRow,
                f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }
      return SetRow<T,USE_NUMERIC_LOCALE>(static_cast<size_t>(rowIdx), pRow, pConvertToStr);
    }

    /**
     * @brief   Remove row by index.
     * @param   pRowIdx               zero-based row index.
     */
    void RemoveRow(const size_t pRowIdx)
    {
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);
      mData.erase(mData.begin() + static_cast<ssize_t>(dataRowIdx));
      UpdateRowNames();
    }

    /**
     * @brief   Remove row by name.
     * @param   pRowName              row label name.
     */
    void RemoveRow(const std::string& pRowName)
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      RemoveRow(static_cast<size_t>(rowIdx));
    }

    /**
     * @brief   Insert row at specified index.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  vector of row data (optional argument).
     * @param   pRowName              row label name (optional argument).
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void InsertRow(const size_t pRowIdx, const std::vector<T>& pRow = std::vector<T>(),
                   const std::string& pRowName = std::string(),
                   f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const size_t rowIdx = GetDataRowIndex(pRowIdx);

      t_dataRow row;
      if (pRow.empty())
      {
        row.resize(GetDataColumnCount());
      }
      else
      {
        row.resize(GetDataColumnIndex(pRow.size()));
        size_t colIdx = GetDataColumnIndex(0);
        for (auto itCol = pRow.begin(); itCol != pRow.end(); ++itCol, ++colIdx)
        {
          row.at(colIdx) = pConvertToStr(*itCol);
        }
      }

      while (rowIdx > GetDataRowCount())
      {
        t_dataRow tempRow;
        tempRow.resize(GetDataColumnCount());
        mData.push_back(tempRow);
      }

      mData.insert(mData.begin() + static_cast<ssize_t>(rowIdx), row);

      if (!pRowName.empty())
      {
        SetRowName(pRowIdx, pRowName);
      }

      UpdateRowNames();
    }

    /**
     * @brief   Get number of data rows (excluding label rows).
     * @returns row count.
     */
    size_t GetRowCount() const
    {
      const size_t dataRowSize = mData.size();
      const size_t firstDataRowIndex = GetDataRowIndex(0);
      return (dataRowSize>firstDataRowIndex) ? (dataRowSize-firstDataRowIndex) : 0;
    }

    /**
     * @brief   Get cell by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowIdx               zero-based row index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetCell(const size_t pColumnIdx, const size_t pRowIdx,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);

      T val = pConvertToVal(mData.at(dataRowIdx).at(dataColumnIdx));
      return val;
    }

    /**
     * @brief   Get cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetCell(const std::string& pColumnName, const std::string& pRowName,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      return GetCell<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(columnIdx),
                                                   static_cast<size_t>(rowIdx), pConvertToVal);
    }

    /**
     * @brief   Get cell by column name and row index.
     * @param   pColumnName           column label name.
     * @param   pRowIdx               zero-based row index.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetCell(const std::string& pColumnName, const size_t pRowIdx,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return GetCell<T,USE_NUMERIC_LOCALE,USE_NAN>(static_cast<size_t>(columnIdx), pRowIdx, pConvertToVal);
    }

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pConvertToVal         conversion function (optional argument).
     * @returns cell data.
     */
    template<typename T, int USE_NUMERIC_LOCALE=1, int USE_NAN=0>
    T GetCell(const size_t pColumnIdx, const std::string& pRowName,
              f_ConvFuncToVal<T> pConvertToVal = ConverterToVal<T,USE_NUMERIC_LOCALE,USE_NAN>::ToVal) const
    {
      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      return GetCell<T,USE_NUMERIC_LOCALE,USE_NAN>(pColumnIdx, static_cast<size_t>(rowIdx), pConvertToVal);
    }

    /**
     * @brief   Set cell by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pCell                 cell data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetCell(const size_t pColumnIdx, const size_t pRowIdx, const T& pCell,
                 f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);

      while ((dataRowIdx + 1) > GetDataRowCount())
      {
        t_dataRow row;
        row.resize(GetDataColumnCount());
        mData.push_back(row);
      }

      if ((dataColumnIdx + 1) > GetDataColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(dataColumnIdx + 1);
        }
      }

      mData.at(dataRowIdx).at(dataColumnIdx) = pConvertToStr(pCell);
    }

    /**
     * @brief   Set cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @param   pCell                 cell data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetCell(const std::string& pColumnName, const std::string& pRowName, const T& pCell,
                 f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      SetCell<T,USE_NUMERIC_LOCALE>(static_cast<size_t>(columnIdx), static_cast<size_t>(rowIdx), pCell, pConvertToStr);
    }

    /**
     * @brief   Set cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowIdx               zero-based row index.
     * @param   pCell                 cell data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetCell(const std::string& pColumnName, const size_t pRowIdx, const T& pCell,
                 f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      SetCell<T,USE_NUMERIC_LOCALE>(static_cast<size_t>(columnIdx), pRowIdx, pCell, pConvertToStr);
    }

    /**
     * @brief   Set cell by name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pCell                 cell data.
     * @param   pConvertToStr         conversion function (optional argument).
     */
    template<typename T, int USE_NUMERIC_LOCALE=0>
    void SetCell(const size_t pColumnIdx, const std::string& pRowName, const T& pCell,
                 f_ConvFuncToStr<T> pConvertToStr = ConverterToStr<T,USE_NUMERIC_LOCALE>::ToStr)
    {
      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      SetCell<T,USE_NUMERIC_LOCALE>(pColumnIdx, static_cast<size_t>(rowIdx), pCell, pConvertToStr);
    }

    /**
     * @brief   Get column name
     * @param   pColumnIdx            zero-based column index.
     * @returns column name.
     */
    std::string GetColumnName(const size_t pColumnIdx)
    {
      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);
      if (mLabelParams.mColumnNameIdx < 0)
      {
        throw std::out_of_range("column name row index < 0: " + std::to_string(mLabelParams.mColumnNameIdx));
      }

      return mData.at(static_cast<size_t>(mLabelParams.mColumnNameIdx)).at(dataColumnIdx);
    }

    /**
     * @brief   Set column name
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumnName           column name.
     */
    void SetColumnName(size_t pColumnIdx, const std::string& pColumnName)
    {
      if (mLabelParams.mColumnNameIdx < 0)
      {
        throw std::out_of_range("column name row index < 0: " + std::to_string(mLabelParams.mColumnNameIdx));
      }

      const size_t dataColumnIdx = GetDataColumnIndex(pColumnIdx);
      mColumnNames[pColumnName] = dataColumnIdx;

      // increase table size if necessary:
      const size_t rowIdx = static_cast<size_t>(mLabelParams.mColumnNameIdx);
      if (rowIdx >= mData.size())
      {
        mData.resize(rowIdx + 1);
      }
      auto& row = mData[rowIdx];
      if (dataColumnIdx >= row.size())
      {
        row.resize(dataColumnIdx + 1);
      }

      mData.at(static_cast<size_t>(mLabelParams.mColumnNameIdx)).at(dataColumnIdx) = pColumnName;
    }

    /**
     * @brief   Get column names
     * @returns vector of column names.
     */
    std::vector<std::string> GetColumnNames()
    {
      if (mLabelParams.mColumnNameIdx >= 0)
      {
        return std::vector<std::string>(mData.at(static_cast<size_t>(mLabelParams.mColumnNameIdx)).begin() +
                                                 static_cast<ssize_t>(GetDataColumnIndex(0)),
                                        mData.at(static_cast<size_t>(mLabelParams.mColumnNameIdx)).end());
      }

      return std::vector<std::string>();
    }

    /**
     * @brief   Get row name
     * @param   pRowIdx               zero-based column index.
     * @returns row name.
     */
    std::string GetRowName(const size_t pRowIdx)
    {
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);
      if (mLabelParams.mRowNameIdx < 0)
      {
        throw std::out_of_range("row name column index < 0: " + std::to_string(mLabelParams.mRowNameIdx));
      }

      return mData.at(dataRowIdx).at(static_cast<size_t>(mLabelParams.mRowNameIdx));
    }

    /**
     * @brief   Set row name
     * @param   pRowIdx               zero-based row index.
     * @param   pRowName              row name.
     */
    void SetRowName(size_t pRowIdx, const std::string& pRowName)
    {
      const size_t dataRowIdx = GetDataRowIndex(pRowIdx);
      mRowNames[pRowName] = dataRowIdx;
      if (mLabelParams.mRowNameIdx < 0)
      {
        throw std::out_of_range("row name column index < 0: " + std::to_string(mLabelParams.mRowNameIdx));
      }

      // increase table size if necessary:
      if (dataRowIdx >= mData.size())
      {
        mData.resize(dataRowIdx + 1);
      }
      auto& row = mData[dataRowIdx];
      if (mLabelParams.mRowNameIdx >= static_cast<ssize_t>(row.size()))
      {
        row.resize(static_cast<size_t>(mLabelParams.mRowNameIdx) + 1);
      }

      mData.at(dataRowIdx).at(static_cast<size_t>(mLabelParams.mRowNameIdx)) = pRowName;
    }

    /**
     * @brief   Get row names
     * @returns vector of row names.
     */
    std::vector<std::string> GetRowNames() const
    {
      std::vector<std::string> rownames;
      if (mLabelParams.mRowNameIdx >= 0)
      {
        const ssize_t rowIdx = static_cast<ssize_t>(GetDataRowIndex(0));
        for (auto itRow = mData.begin() + rowIdx; itRow != mData.end(); ++itRow)
        {
          rownames.push_back(itRow->at(static_cast<size_t>(mLabelParams.mRowNameIdx)));
        }
      }
      return rownames;
    }

  private:
    void ReadCsv()
    {
      std::ifstream stream;
      stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      stream.open(mPath, std::ios::binary);
      ReadCsv(stream);
    }

    void ReadCsv(std::istream& pStream)
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
        wstream.open(mPath, std::ios::binary);
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
        std::string utf8 = ToString(wss.str());
        std::stringstream ss(utf8);
        ParseCsv(ss, static_cast<std::streamsize>(utf8.size()));
      }
      else
#endif
      {
        // check for UTF-8 Byte order mark and skip it when found
        if (length >= 3)
        {
          std::vector<char> bom3b(3, '\0');
          pStream.read(bom3b.data(), 3);
          static const std::vector<char> bomU8 = { '\xef', '\xbb', '\xbf' };
          if (bom3b != bomU8)
          {
            // file does not start with a UTF-8 Byte order mark
            pStream.seekg(0, std::ios::beg);
          }
          else
          {
            // file did start with a UTF-8 Byte order mark, simply skip it
            length -= 3;
          }
        }

        ParseCsv(pStream, length);
      }
    }

    void ParseCsv(std::istream& pStream, std::streamsize p_FileLength)
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
          if (buffer[i] == mSeparatorParams.mQuoteChar)
          {
            if (cell.empty() || (cell[0] == mSeparatorParams.mQuoteChar))
            {
              quoted = !quoted;
            }
            cell += buffer[i];
          }
          else if (buffer[i] == mSeparatorParams.mSeparator)
          {
            if (!quoted)
            {
              row.push_back(Unquote(Trim(cell)));
              cell.clear();
            }
            else
            {
              cell += buffer[i];
            }
          }
          else if (buffer[i] == '\r')
          {
            if (mSeparatorParams.mQuotedLinebreaks && quoted)
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
            if (mSeparatorParams.mQuotedLinebreaks && quoted)
            {
              cell += buffer[i];
            }
            else
            {
              ++lf;
              if (mLineReaderParams.mSkipEmptyLines && row.empty() && cell.empty())
              {
                // skip empty line
              }
              else
              {
                row.push_back(Unquote(Trim(cell)));

                if (mLineReaderParams.mSkipCommentLines && !row.at(0).empty() &&
                    (row.at(0)[0] == mLineReaderParams.mCommentPrefix))
                {
                  // skip comment line
                }
                else
                {
                  mData.push_back(row);
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
        row.push_back(Unquote(Trim(cell)));
        cell.clear();
        mData.push_back(row);
        row.clear();
      }

      // Assume CR/LF if at least half the linebreaks have CR
      mSeparatorParams.mHasCR = (cr > (lf / 2));

      // Set up column labels
      UpdateColumnNames();

      // Set up row labels
      UpdateRowNames();
    }

    void WriteCsv() const
    {
#ifdef HAS_CODECVT
      if (mIsUtf16)
      {
        std::stringstream ss;
        WriteCsv(ss);
        std::string utf8 = ss.str();
        std::wstring wstr = ToWString(utf8);

        std::wofstream wstream;
        wstream.exceptions(std::wofstream::failbit | std::wofstream::badbit);
        wstream.open(mPath, std::ios::binary | std::ios::trunc);

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
        stream.open(mPath, std::ios::binary | std::ios::trunc);
        WriteCsv(stream);
      }
    }

    void WriteCsv(std::ostream& pStream) const
    {
      for (auto itr = mData.begin(); itr != mData.end(); ++itr)
      {
        for (auto itc = itr->begin(); itc != itr->end(); ++itc)
        {
          if (mSeparatorParams.mAutoQuote &&
              ((itc->find(mSeparatorParams.mSeparator) != std::string::npos) ||
               (itc->find(' ') != std::string::npos)))
          {
            // escape quotes in string
            std::string str = *itc;
            const std::string quoteCharStr = std::string(1, mSeparatorParams.mQuoteChar);
            ReplaceString(str, quoteCharStr, quoteCharStr + quoteCharStr);

            pStream << quoteCharStr << str << quoteCharStr;
          }
          else
          {
            pStream << *itc;
          }

          if (std::distance(itc, itr->end()) > 1)
          {
            pStream << mSeparatorParams.mSeparator;
          }
        }
        pStream << (mSeparatorParams.mHasCR ? "\r\n" : "\n");
      }
    }

    inline size_t GetDataRowCount() const
    {
      return mData.size();
    }

    inline size_t GetDataColumnCount() const
    {
      return (mData.size() > 0) ? mData.at(0).size() : 0;
    }

    /**
     * @brief   Get actual index, taking into account header-row if present.
     * @param   pRowIdx               zero-based row index.
     * @returns zero-based row index when header-row is absent OR one-based row index when header-row is present.
     */
    inline size_t GetDataRowIndex(const size_t pRowIdx) const
    {
      return pRowIdx + static_cast<size_t>(mLabelParams.mColumnNameIdx + 1);
    }

    /**
     * @brief   Get actual index, taking into account header-column if present.
     * @param   pColumnIdx            zero-based column index.
     * @returns zero-based column index when header-column is absent OR one-based column index when header-column is present.
     */
    inline size_t GetDataColumnIndex(const size_t pColumnIdx) const
    {
      return pColumnIdx + static_cast<size_t>(mLabelParams.mRowNameIdx + 1);
    }

    std::string Trim(const std::string& pStr) const
    {
      if (mSeparatorParams.mTrim)
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

    std::string Unquote(const std::string& pStr) const
    {
      if (mSeparatorParams.mAutoQuote && (pStr.size() >= 2) &&
          (pStr.front() == mSeparatorParams.mQuoteChar) &&
          (pStr.back() == mSeparatorParams.mQuoteChar))
      {
        // remove start/end quotes
        std::string str = pStr.substr(1, pStr.size() - 2);

        // unescape quotes in string
        const std::string quoteCharStr = std::string(1, mSeparatorParams.mQuoteChar);
        ReplaceString(str, quoteCharStr + quoteCharStr, quoteCharStr);

        return str;
      }
      else
      {
        return pStr;
      }
    }

    void UpdateColumnNames()
    {
      mColumnNames.clear();
      if ((mLabelParams.mColumnNameIdx >= 0) &&
          (static_cast<ssize_t>(mData.size()) > mLabelParams.mColumnNameIdx))
      {
        size_t i = 0;
        for (auto& columnName : mData[static_cast<size_t>(mLabelParams.mColumnNameIdx)])
        {
          mColumnNames[columnName] = i++;
        }
      }
    }

    void UpdateRowNames()
    {
      mRowNames.clear();
      if ((mLabelParams.mRowNameIdx >= 0) && (mData.size() > GetDataRowIndex(0)))
      {
        size_t i = 0;
        for (auto& dataRow : mData)
        {
          if (static_cast<ssize_t>(dataRow.size()) > mLabelParams.mRowNameIdx)
          {
            mRowNames[dataRow[static_cast<size_t>(mLabelParams.mRowNameIdx)]] = i++;
          }
        }
      }
    }

#ifdef HAS_CODECVT
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable: 4996)
#endif
    static std::string ToString(const std::wstring& pWStr)
    {
      return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{ }.to_bytes(pWStr);
    }

    static std::wstring ToWString(const std::string& pStr)
    {
      return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{ }.from_bytes(pStr);
    }
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
#endif

    static void ReplaceString(std::string& pStr, const std::string& pSearch, const std::string& pReplace)
    {
      size_t pos = 0;

      while ((pos = pStr.find(pSearch, pos)) != std::string::npos)
      {
        pStr.replace(pos, pSearch.size(), pReplace);
        pos += pReplace.size();
      }
    }

    std::string mPath;
    LabelParams mLabelParams;
    SeparatorParams mSeparatorParams;
    LineReaderParams mLineReaderParams;
    std::vector<t_dataRow> mData;
    std::map<std::string, size_t> mColumnNames;
    std::map<std::string, size_t> mRowNames;
#ifdef HAS_CODECVT
    bool mIsUtf16 = false;
    bool mIsLE = false;
#endif


    template<Document::f_EvalBoolExpr evaluateBooleanExpression, typename... Types>
    friend struct ViewDocument;
  };
}
