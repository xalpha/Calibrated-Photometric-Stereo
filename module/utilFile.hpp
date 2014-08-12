#ifndef _UTILFILE_H_
#define _UTILFILE_H_

/*!
 * \file utilFile.hpp
 *
 * \author Yuji Oyamada
 * \date 2013/12/07
 * \brief This file is utility to handling files based on boost filesystem.
 *
 */

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "utilString.hpp"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

namespace UtilFile{

/*!
 * \class FileName
 *
 * \brief is a class handling a filename.
 *
 * \author Yuji Oyamada
 *
 */
class FileName{
public:
    //--------------------------------------------------------
    //
    //! \name Constructors / Destructor / Instance Management
    //@{
    //--------------------------------------------------------
    //! Destructor.
    ~FileName(){};
    //! Default constructor.
    FileName(){}
    //! constructs with given information.
    FileName(
        const std::string _fullname,
        const std::string _filename,
        const std::string _stem,
        const std::string _ext,
        const std::string _dir
    ) : fullname(_fullname),
        filename(_filename),
        stem(_stem),
        ext(_ext),
        dir(_dir)
    {}
    //! constructs with a fullpath \c fullname_.
    FileName(const std::string fullname_){SetFileName(fullname_);}
    //@}

    //------------------------------------------
    //
    //! \name Get / Set private member variables
    //@{
    //------------------------------------------
    //! sets all varialbes by decomposing a fullpath \c fullname_.
    void SetFileName(const std::string fullname_);
    /// @brief returns fullname
    std::string GetFullName(void) const {return fullname;}
    /// @brief returns filename
    std::string GetFileName(void) const {return filename;}
    /// @brief returns stem
    std::string GetStem(void) const {return stem;}
    /// @brief returns ext
    std::string GetExtension(void) const {return ext;}
    /// @brief returns dir
    std::string GetDirectoryName(void) const {return dir;}
    //@}

private:
    //------------------------------------------
    //
    //! \name Private variables
    //@{
    //------------------------------------------
    std::string fullname;   //!< Full path of the file (/etc/data/test.txt)
    std::string filename;   //!< Filename of the file (test.txt)
    std::string stem;       //!< Filename except its extension (test)
    std::string ext;        //!< Extension of the file (txt)
    std::string dir;        //!< Directory name (/etc/data)
    //@}
};

void FileName::SetFileName(
    const std::string fullname_
)
{
    fullname = fullname_;
    boost::filesystem::path p(fullname);
    filename = p.filename().c_str();
    stem = p.stem().c_str();
    ext = p.extension().c_str();
    dir = p.parent_path().c_str();
}

bool checkFileExist(
    std::string strName
)
{
    std::ifstream ifs( strName.c_str() );
    if( ifs.fail() )
    {
        return false;
    }
    ifs.close();

    return true;
}

/// @brief This function stores filename into a list.
// @param[in]	p				Path of the file containing both directory and file names.
// @param[in]	strExt			Target file extension.
// @param[out]	strFile			Obtained list of files of interest.
void StoreFile(
    const boost::filesystem::path& p,
    const std::string strExt,
    std::vector<std::string>& strFile
)
{
    if(!boost::filesystem::is_directory(p))
    { // if p is not directory
        if((p.extension() == strExt ) || (matchString(strExt, std::string("*"))))
        { // store file name if extension is matched
            strFile.push_back( p.generic_string() );
        }
    }
}

/// @brief This function stores a filename.
// @param[in]	p				Path of the file containing both directory and file names.
// @param[in]	strExt			Target file extension.
// @param[out]	strFile			Obtained file of interest.
void StoreFile(
    const boost::filesystem::path& p,
    const std::string strExt,
    std::string& strFile
)
{
    if(!boost::filesystem::is_directory(p))
    { // if p is not directory
        if((p.extension() == strExt ) || (matchString(strExt, std::string("*"))))
        { // store file name if extension is matched
            strFile = p.generic_string();
        }
    }
}

//------------------------------------------
//
//! \name Get a list of files
//@{
//------------------------------------------

//! @brief This function gets a list of files of interest.
//! Note that getFileList() function does not support wildcard.
//! @param[in]	strDir			Target directory.
//! @param[in]	strExt			Target file extension.
//! @param[out]	strFile			Obtained list of files of interest.
//! @param[in]	flagRecursive	Flag specifing whether file search is recursively executed or not.
void GetFileList(
    const std::string strDir,
    const std::string strExt,
    std::vector<std::string> &strFile,
    bool flagRecursive = true
)
{
    const boost::filesystem::path path( strDir );
    std::cout << "Start loading " << strExt << " files in a directory " << strDir;
    if( !flagRecursive )
    {
        std::cout << " recursively";
    }
    std::cout << std::endl;

    if( flagRecursive )
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::recursive_directory_iterator(path),
                boost::filesystem::recursive_directory_iterator()
            )
        )
        {
            StoreFile( p, strExt, strFile );
        }
    }
    else
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::directory_iterator(path),
                boost::filesystem::directory_iterator()
            )
        )
        {
            StoreFile( p, strExt, strFile );
        }
    }
}

//! @brief This function gets a list of files of interest.
//! Note that getFileList() function does not support wildcard.
//! @param[in]	strDir			Target directory.
//! @param[in]	strExt			Target file extension.
//! @param[out]	fileName		Obtained list of files of interest as \c std::vector<FileName>.
//! @param[in]	flagRecursive	Flag specifing whether file search is recursively executed or not.
void GetFileList(
    const std::string strDir,
    const std::string strExt,
    std::vector<FileName>& fileName,
    bool flagRecursive = true
)
{
    fileName.clear();
    const boost::filesystem::path path( strDir );
    std::cout << "Start loading " << strExt << " files in a directory " << strDir;
    if( !flagRecursive )
    {
        std::cout << " recursively";
    }
    std::cout << std::endl;

    if( flagRecursive )
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::recursive_directory_iterator(path),
                boost::filesystem::recursive_directory_iterator()
            )
        )
        {
            if( p.extension().string().find( strExt ) != std::string::npos )
            {
                fileName.push_back(FileName(
                    p.c_str(),
                    p.filename().c_str(),
                    p.stem().c_str(),
                    p.extension().c_str(),
                    p.parent_path().c_str()
                    )
                );
            }
        }
    }
    else
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::directory_iterator(path),
                boost::filesystem::directory_iterator()
            )
        )
        {
            if( p.extension().string().find( strExt ) != std::string::npos )
            {
                fileName.push_back(FileName(
                    p.c_str(),
                    p.filename().c_str(),
                    p.stem().c_str(),
                    p.extension().c_str(),
                    p.parent_path().c_str()
                    )
                );
            }
        }
    }
}

//! @brief This function gets a list of files of interest.
//! Note that getFileList() function does not support wildcard.
//! @param[in]	strDir			Target directory.
//! @param[in]	strBase			part of string of interests.
//! @param[in]	strExt			Target file extension.
//! @param[out]	strFile			Obtained list of files of interest as \c std::vector<std::string>.
//! @param[in]	flagRecursive	Flag specifing whether file search is recursively executed or not.
void GetFileList(
    const std::string strDir,
    const std::string strBase,
    const std::string strExt,
    std::vector<std::string> &strFile,
    bool flagRecursive = true
)
{
    const boost::filesystem::path path( strDir );
    std::cout << "Start loading " << strExt << " files in a directory " << strDir;
    if( !flagRecursive )
    {
        std::cout << " recursively";
    }
    std::cout << std::endl;

    if( flagRecursive )
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::recursive_directory_iterator(path),
                boost::filesystem::recursive_directory_iterator()
            )
        )
        {
            if( p.filename().string().find( strBase ) != std::string::npos )
            {
                StoreFile( p, strExt, strFile );
            }
        }
    }
    else
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::directory_iterator(path),
                boost::filesystem::directory_iterator()
            )
        )
        {
            if( p.filename().string().find( strBase ) != std::string::npos )
            {
                StoreFile( p, strExt, strFile );
            }
        }
    }
}

//! @brief This function gets a list of files of interest.
//! Note that getFileList() function does not support wildcard.
//! @param[in]	strDir			Target directory.
//! @param[in]	strBase			part of string of interests.
//! @param[in]	strExt			Target file extension.
//! @param[out]	fileName		Obtained list of files of interest as \c std::vector<FileName>.
//! @param[in]	flagRecursive	Flag specifing whether file search is recursively executed or not.
void GetFileList(
    const std::string strDir,
    const std::string strBase,
    const std::string strExt,
    std::vector<FileName>& fileName,
    bool flagRecursive = true
)
{
    fileName.clear();
    const boost::filesystem::path path( strDir );
    std::cout << "Start loading " << strExt << " files in a directory " << strDir;
    if( !flagRecursive )
    {
        std::cout << " recursively";
    }
    std::cout << std::endl;

    if( flagRecursive )
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::recursive_directory_iterator(path),
                boost::filesystem::recursive_directory_iterator()
            )
        )
        {
            if( p.filename().string().find( strBase ) != std::string::npos )
            {
                if( p.extension().string().find( strExt ) != std::string::npos )
                {
                    fileName.push_back(FileName(
                        p.c_str(),
                        p.filename().c_str(),
                        p.stem().c_str(),
                        p.extension().c_str(),
                        p.parent_path().c_str()
                        )
                    );
                }
            }
        }
    }
    else
    {
        BOOST_FOREACH(
            const boost::filesystem::path& p,
            std::make_pair(
                boost::filesystem::directory_iterator(path),
                boost::filesystem::directory_iterator()
            )
        )
        {
            if( p.filename().string().find( strBase ) != std::string::npos )
            {
                if( p.extension().string().find( strExt ) != std::string::npos )
                {
                    fileName.push_back(FileName(
                        p.c_str(),
                        p.filename().c_str(),
                        p.stem().c_str(),
                        p.extension().c_str(),
                        p.parent_path().c_str()
                        )
                    );
                }
            }
        }
    }
}

//! @brief This function gets a list of files of interest.
//! Note that getFileList() function does not support wildcard.
//! @param[in]	strDir			Target directory.
//! @param[in]	strBase			part of string of interests.
//! @param[in]	strExt			Target file extension.
//! @return 	strFile			Obtained list of files of interest as \c std::vector<std::string>.
std::vector<std::string> getFilesFromDirectory(
    const std::string strDir,
    const std::string strBase,
    const std::string strExt
)
{
    std::vector<std::string> nameFiles;
    std::vector<boost::filesystem::path> pathEach;
    boost::filesystem::path pathDir( strDir.c_str() );
    copy( boost::filesystem::directory_iterator(pathDir), boost::filesystem::directory_iterator(), back_inserter(pathEach) );

    for( std::vector<boost::filesystem::path>::const_iterator it(pathEach.begin()), it_end(pathEach.end()); it != it_end; ++it )
    {
        if( boost::filesystem::is_regular_file( *it ) )
        {
            if( (*it).extension().string().find( strExt ) != std::string::npos )
            { // select by file extension
                if( (*it).filename().string().find( strBase ) != std::string::npos )
                { // select by file extension
                    nameFiles.push_back( (*it).filename().string() );
                }
            }
        }
    }

    return nameFiles;
}

//! @brief This function gets a list of files of interest.
//! Note that getFileList() function does not support wildcard.
//! @param[in]	strDir			Target directory.
//! @param[in]	strBase			part of string of interests.
//! @param[in]	strExt			Target file extension.
//! @param[out]	fileName		Obtained list of files of interest as \c std::vector<FileName>.
//! @param[out] fileIndex       Index of the obtained filename list as \c std::vector<int>.
//! @param[in]	flagRecursive	Flag specifing whether file search is recursively executed or not.
void GetFileList(
    const std::string strDir,
    const std::string strBase,
    const std::string strExt,
    std::vector<FileName>& fileName,
    std::vector<int>& fileIndex,
    bool flagRecursive = true
)
{
    int tmp;
    std::string strFile;
    fileIndex.clear();

    // get file name
    GetFileList(
        strDir,
        strBase,
        strExt,
        fileName,
        flagRecursive
    );

    // get index of each file from its file name
    for(int i = 0; i < fileName.size(); ++i)
    {
        strFile = fileName[i].GetStem();
        strFile.erase(
            strFile.find_first_of(strBase),
            strBase.length()
        );
        str2scalar(strFile, tmp);
        fileIndex.push_back(tmp);
    }
}
//@}

};

#endif
