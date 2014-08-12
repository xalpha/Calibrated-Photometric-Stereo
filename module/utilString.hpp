#ifndef _UTILSTRING_H_
#define _UTILSTRING_H_

/*!
 * \file utilString.hpp
 *
 * \author Yuji Oyamada
 * \date 2013/12/06
 * \brief This file is utility to convert from/to \c std::string to/from another data type.
 *
 */
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
// for Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/StdVector>

//! replaces \c std::string from \c strBefore to \c strAfter.
std::string strReplace(
    std::string &str,
    const std::string &strBefore,
    const std::string &strAfter
)
{
    std::string::size_type Pos( str.find( strBefore ) );
    while( Pos != std::string::npos )
    {
        str.replace( Pos, strBefore.length(), strAfter );
        Pos = str.find( strBefore, Pos + strAfter.length() );
    }

    return str;
}

//------------------------------------------
//
//! \name Convert X to std::string
//@{
//------------------------------------------
//! converts any data type object to \c std::string
template <typename T>
inline std::string toString(
    const T val
)
{
    std::stringstream ss;
    ss << val;

    return ss.str();
}

//! converts \c vector of any data type objects to \c std::string
template <typename T>
inline std::string toString(
    const std::vector<T>& vec,
    const std::string delim = " "
)
{
    std::stringstream ss;
    for( size_t i = 0; i < vec.size(); ++i )
    {
        ss << vec[i] << delim;
    }

    return ss.str();
}

//! converts 2D \c vector of any data type objects to \c std::string
template <typename T>
inline std::string toString(
    const std::vector< std::vector<T> >& vec,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::stringstream ss;
    for( size_t i = 0; i < vec.size(); ++i )
    {
        for(size_t j = 0; j < vec[i].size(); ++j)
        {
            ss << vec[i][j] << delimRow;
        }
        ss << delimCol;
    }

    return ss.str();
}

//! converts \c Eigen::MatrixXd data to \c std::string, which delimits consective rows by \c delimeRow and consective columns by \c delimeCol.
std::string toString(
    const Eigen::MatrixXd& vec,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::stringstream ss;
    int row = vec.rows(), col = vec.cols();
    for( size_t i=0; i < col; i++ )
    {
        for( int j = 0; j < row; ++j )
        {
            ss << toString( vec(j,i) ) << delimRow;
        }
        ss << delimCol;
    }

    return ss.str();
}

//! converts \c Eigen::Vector3d data to \c std::string, which delimits consective rows by \c delimeRow and consective columns by \c delimeCol.
std::string toString(
    const std::vector< Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d> >& vec,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::stringstream ss;
    for( size_t i=0; i<vec.size(); i++ )
    {
        for( int j = 0; j < vec[i].size(); ++j )
        {
            ss << toString( vec[i](j) ) << delimRow;
        }
        ss << delimCol;
    }

    return ss.str();
}

//! converts \c Eigen::Vector2d data to \c std::string, which delimits consective rows by \c delimeRow and consective columns by \c delimeCol.
std::string toString(
    const std::vector< Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> >& vec,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::stringstream ss;
    for( size_t i=0; i<vec.size(); i++ )
    {
        for( int j = 0; j < vec[i].size(); ++j )
        {
            ss << toString( vec[i](j) ) << delimRow;
        }
        ss << delimCol;
    }

    return ss.str();
}

//! converts \c Eigen::Matrix data to \c std::string, which delimits consective rows by \c delimeRow and consective columns by \c delimeCol.
template <typename Scalar, int Rows, int Cols>
inline std::string toString(
    const Eigen::Matrix<Scalar, Rows, Cols>& mat,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::stringstream ss;

    for( int i = 0; i < mat.cols(); ++i )
    {
        for( int j = 0; j < mat.rows(); ++j )
        {
            ss << toString( mat(i*mat.rows()+j) ) << delimRow;
        }
        ss << delimCol;
    }

    return ss.str();
}

//@}
//------------------------------------------
//
//! \name Convert std::string to X
//@{
//------------------------------------------

//! splits \c std::string data, using \c delim as separator, to a set of \c std::string data as \c std::vector<std::string> data.
std::vector< std::string > splitString(
    const std::string &str,
    const std::string &delim
)
{
    std::string _str = str;
    std::vector< std::string > res;
    int p;
    while( (p = _str.find(delim)) != _str.npos ){
        res.push_back(_str.substr(0, p));
        _str = _str.substr(p+delim.size());
    }
    if(_str.size() > 0)
    {
        res.push_back(_str);
    }
    return res;
}

//! splits \c std::string data, using \c delimCol and \c delimRow as separators, to a set of \c std::vector< std::vector< std::string > > data.
std::vector< std::vector< std::string > > splitString(
    const std::string &str,
    const std::string &delimCol,
    const std::string &delimRow
)
{
    std::vector< std::vector< std::string > > res;
    std::vector< std::string > tmp;
    size_t posCurrent = 0;
    size_t posFound;
    size_t lenDelim = delimCol.size();

    while( (posFound = str.find(delimCol, posCurrent)) != std::string::npos )
    {
        res.push_back(
            splitString( std::string( str, posCurrent, posFound - posCurrent ), delimRow )
        );
        posCurrent = posFound + lenDelim;
    }

    return res;
}

//! converts \c std::string data to \c T data.
template <typename T>
inline void str2scalar(
    std::string str,
    T& result
)
{
    std::stringstream ss;
    ss << str;
    ss.seekg( 0, std::ios::beg );
    ss >> result;
}

//! returns \c std::string as X format.
template <typename T>
inline T str2scalar(std::string str){}
//! returns \c std::string as \c unsigned char data.
template <>
inline unsigned char str2scalar<unsigned char>(
    std::string str
)
{
    unsigned char retval;
    str2scalar(str, retval);
    return retval;
}
//! returns \c std::string as \c char data.
template <>
inline char str2scalar<char>(
    std::string str
)
{
    char retval;
    str2scalar(str, retval);
    return retval;
}
//! returns \c std::string as \c int data.
template <>
inline int str2scalar<int>(
    std::string str
)
{
    int retval;
    str2scalar(str, retval);
    return retval;
}
//! returns \c std::string as \c size_t data.
template <>
inline size_t str2scalar<size_t>(
    std::string str
)
{
    size_t retval;
    str2scalar(str, retval);
    return retval;
}
//! returns \c std::string as \c float data.
template <>
inline float str2scalar<float>(
    std::string str
)
{
    float retval;
    str2scalar(str, retval);
    return retval;
}
//! returns \c std::string as \c double data.
template <>
inline double str2scalar<double>(
    std::string str
)
{
    double retval;
    str2scalar(str, retval);
    return retval;
}

//! converts \c std::string data to \c std::vector<T> data.
template <typename T>
inline void str2vector(
    const std::string& str,
    std::vector<T>& result
)
{
    std::stringstream ss;

    result.clear();
    std::string str2 = str;
    // remove any ";" from str
    str2 = strReplace(str2, ";", "");
    str2 = strReplace(str2, ";", "");
    // remove " " at the end of str
    while(str2.find(" ", str2.size()-1) != std::string::npos)
    {
        str2= str2.substr(0, str2.size()-1);
    }
    if(str2.find(" ") == std::string::npos)
    {
        result.push_back(str2scalar<T>(str2));
    }
    else
    {
        ss << str2;
        ss.seekg( 0, std::ios::beg );
        while( !ss.eof() )
        {
            T tmp;
            ss >> tmp;
            result.push_back( tmp );
        }
    }
}

//! converts \c std::string data to \c std::vector<T> data.
template <typename T>
inline std::vector<T> str2vector(std::string str){}

template <>
inline std::vector<unsigned char> str2vector<unsigned char>(
    std::string str
)
{
    std::vector<unsigned char> tmp;
    str2vector(str,tmp);
    return tmp;
}
template <>
inline std::vector<char> str2vector<char>(
    std::string str
)
{
    std::vector<char> tmp;
    str2vector(str,tmp);
    return tmp;
}
template <>
inline std::vector<size_t> str2vector<size_t>(
    std::string str
)
{
    std::vector<size_t> tmp;
    str2vector(str,tmp);
    return tmp;
}
template <>
inline std::vector<int> str2vector<int>(
    std::string str
)
{
    std::vector<int> tmp;
    str2vector(str,tmp);
    return tmp;
}
template <>
inline std::vector<float> str2vector<float>(
    std::string str
)
{
    std::vector<float> tmp;
    str2vector(str,tmp);
    return tmp;
}
template <>
inline std::vector<double> str2vector<double>(
    std::string str
)
{
    std::vector<double> tmp;
    str2vector(str,tmp);
    return tmp;
}

//! converts \c std::string data to \c Eigen::Matrix data using \c delimRow and \c delimCol as separators.
template <typename T>
inline void str2vector2d(
    const std::string str,
    std::vector< std::vector<T> >& vec,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::vector< std::vector< std::string > > strArray = splitString(
        str,
        delimCol,
        delimRow
    );
    size_t _Cols = strArray.size();
    size_t _Rows = strArray[0].size();
    vec = std::vector< std::vector<T> >(_Cols, std::vector<T>(_Rows));
    for( size_t i = 0; i < strArray.size(); ++i )
    {
        for( size_t j = 0; j < strArray[i].size(); ++j )
        {
            str2scalar( strArray[i][j], vec[i][j] );
        }
    }
}
template <typename T>
inline std::vector< std::vector<T> > str2vector2d(std::string str){}

template <>
inline std::vector< std::vector<unsigned char> > str2vector2d<unsigned char>(
    std::string str
)
{
    std::vector< std::vector<unsigned char> > tmp;
    str2vector2d(str,tmp);
    return tmp;
}
template <>
inline std::vector< std::vector<char> > str2vector2d<char>(
    std::string str
)
{
    std::vector< std::vector<char> > tmp;
    str2vector2d(str,tmp);
    return tmp;
}
template <>
inline std::vector< std::vector<size_t> > str2vector2d<size_t>(
    std::string str
)
{
    std::vector< std::vector<size_t> > tmp;
    str2vector2d(str,tmp);
    return tmp;
}
template <>
inline std::vector< std::vector<int> > str2vector2d<int>(
    std::string str
)
{
    std::vector< std::vector<int> > tmp;
    str2vector2d(str,tmp);
    return tmp;
}
template <>
inline std::vector< std::vector<float> > str2vector2d<float>(
    std::string str
)
{
    std::vector< std::vector<float> > tmp;
    str2vector2d(str,tmp);
    return tmp;
}
template <>
inline std::vector< std::vector<double> > str2vector2d<double>(
    std::string str
)
{
    std::vector< std::vector<double> > tmp;
    str2vector2d(str,tmp);
    return tmp;
}

//! converts \c std::string data to \c Eigen::Matrix data using \c delimRow and \c delimCol as separators.
template <typename Scalar, int Rows, int Cols>
inline void str2eigen(
    const std::string str,
    Eigen::Matrix<Scalar, Rows, Cols>& mat,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::vector< std::vector< std::string > > strArray = splitString(
        str,
        delimCol,
        delimRow
    );
    size_t _Cols = strArray.size();
    size_t _Rows = strArray[0].size();
    mat.resize( _Rows, _Cols );
    for( size_t i = 0; i < strArray.size(); ++i )
    {
        for( size_t j = 0; j < strArray[i].size(); ++j )
        {
            str2scalar( strArray[i][j], mat(i*strArray[i].size()+j) );
        }
    }
}

//! converts \c std::string data to \c Eigen::Matrix3d data.
void str2eigenMatrix(
    std::string str,
    Eigen::Matrix3d& result
)
{
    double buf;
    size_t start=0, i=0;
    size_t finish = str.find_first_of( ';' );
    while( finish != std::string::npos )
    {
        // get the element
        str2scalar( str.substr( start, finish - start), buf );
        result( i ) = buf;

        // advance indices
        start=finish + 1;
        finish = str.find_first_of( ';', start );
        ++i;
    }
}

//! converts \c std::string data to \c std::vector<Eigen::Vector3d> data using \c delimRow and \c delimCol as separators.
void str2eigenVector(
    std::string str,
    std::vector< Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d> >& result,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::vector< std::vector< std::string > > strArray = splitString(
        str,
        delimCol,
        delimRow
    );

    Eigen::Vector3d tmp;
    for( size_t i = 0; i < strArray.size(); ++i )
    {
        str2scalar( strArray[i][0], tmp(0) );
        str2scalar( strArray[i][1], tmp(1) );
        str2scalar( strArray[i][2], tmp(2) );
        result.push_back( tmp );
    }
}

//! converts \c std::string data to \c std::vector<Eigen::Vector2d> data using \c delimRow and \c delimCol as separators.
void str2eigenVector(
    std::string str,
    std::vector< Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d> >& result,
    const std::string delimRow = " ",
    const std::string delimCol = ";"
)
{
    std::vector< std::vector< std::string > > strArray = splitString(
        str,
        delimCol,
        delimRow
    );

    Eigen::Vector2d tmp;
    for( size_t i = 0; i < strArray.size(); ++i )
    {
        str2scalar( strArray[i][0], tmp(0) );
        str2scalar( strArray[i][1], tmp(1) );
        result.push_back( tmp );
    }
}

//@}
//------------------------------------------
//
//! \name Match std::string with X
//@{
//------------------------------------------
//! matches to \c std::string data.
bool matchString(
    const std::string str1,
    const std::string str2
)
{
    if( str1.compare( str2 ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//! matches \c std::string and \c std::vector<std::string> and returns the index of matched object or -1 if not matched.
int matchString(
    const std::string str1,
    const std::vector< std::string > str2
)
{
    int retVal = -1;
    for( int n = 0; n < str2.size(); ++n )
    {
        if(matchString( str1, str2[n] ))
        retVal = n;
    }

    return retVal;
}

//@}
//------------------------------------------
//
//! \name functions for input/output via console
//@{
//------------------------------------------
//! outputs \c std::vector<T> data on console.
template <typename T>
inline void showVector(
    const std::vector< T > vec,
    const std::string str
)
{
    for( size_t n = 0; n < vec.size(); ++n )
    {
        std::cout << str << "[" << n << "] = " << vec[n] << std::endl;
    }
}

//! outputs \c ste::vector<std::vector<T>> data on console.
template <typename T>
inline void showVector(
    const std::vector< std::vector< T > > vec,
    const std::string str
)
{
    for( size_t n = 0; n < vec.size(); ++n )
    {
        std::cout << str << "[" << n << "] = (	";
        for( size_t m = 0; m < vec[n].size(); ++m )
        {
            std::cout << vec[n][m] << "	";
        }
        std::cout << ")" << std::endl;
    }
}

//! outputs \c std::vector<T> data on console.
int askInput( const int valDefault )
{
    std::string strTmp;

    std::getline(std::cin, strTmp);
    if( strTmp == "" )
    {
        return valDefault;
    }
    else
    {
        return atoi( strTmp.c_str() );
    }
}

//! asks any input via console and returns the input as \c std::string.
std::string askInput( const std::string valDefault )
{
    std::string strTmp;

    std::getline(std::cin, strTmp);
    if( strTmp == "" )
    {
        return valDefault;
    }
    else
    {
        return strTmp;
    }
}

//@}
//------------------------------------------
//
//! \name file loading
//@{
//------------------------------------------
//! splits \c std::string to \c std::vector<double> using \c delim as separator.
std::vector<double> splitStringToDouble(
    const std::string &str,
    const std::string &delim
)
{
    std::vector< double > res;
    std::stringstream ss(str);
    std::string strTmp;
    double tmp;

    while( std::getline(ss, strTmp, ',') )
    {
        str2scalar(strTmp, tmp);
        res.push_back( tmp );
    }

    return res;
}

//! load \c strFile and convert the loaded data to \c std::vector<std::vector<std::string>> using \c delim as separator.
void FiletoString(
    const std::string strFile,
    std::vector< std::vector< std::string > >& strData,
    const std::string delim = ","
)
{
    std::ifstream ifs(strFile.c_str());
    std::string buf;
    int idxLine = 0;
    strData.clear();
    while(ifs && std::getline(ifs, buf))
    {
        strData.push_back(splitString(buf, delim));
        ++idxLine;
    }
}

#endif
