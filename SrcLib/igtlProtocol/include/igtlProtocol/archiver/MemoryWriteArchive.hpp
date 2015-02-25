/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IGTLPROTOCOL_ARCHIVER_MEMORYWRITEARCHIVE_HPP__
#define __IGTLPROTOCOL_ARCHIVER_MEMORYWRITEARCHIVE_HPP__

///// ugly but we must for windows system typedef SSIZE_T to ssize_t for portability
#if defined(_WIN32)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include "igtlProtocol/config.hpp"

#include <fwZip/IWriteArchive.hpp>
#include <fwZip/exception/Write.hpp>
#include <fwCore/macros.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/categories.hpp>

#include <archive.h>

#include <ctype.h>
#include <vector>

namespace igtlProtocol
{
namespace archiver
{

/**
 * @class MemoryArchiveSink
 *
 * @brief class provide utility to write a file to a archive. This class is used locally
 */
class MemoryArchiveSink
{
public:
    typedef char char_type;
    typedef ::boost::iostreams::sink_tag category;

    /**
     * @brief constructor
     *
     * @param[in] archive archive instance
     * @param[in] path virtual path in archive if file exist it get the stat of file otherwise create fake stat
     */
    MemoryArchiveSink(struct archive *archive, const ::boost::filesystem::path &path);

    /// Destructor
    ~MemoryArchiveSink();

    /**
     * @brief write buffer in archive buffer
     */
    void archive() throw(::fwZip::exception::Write);

    /**
     * @brief writter to write data in archive entry
     */
    std::streamsize write(const char *buf, std::streamsize n);

protected:
    /// instance archive
    struct archive *m_archive;

    /// dynamic buffer to write data
    std::vector<char> m_buffer;

    /// path of entry file
    ::boost::filesystem::path m_path;

    /// buffer write size is large because it is in memory
    static const size_t s_WRITE_BUFFER_SIZE = 20000;

};

/**
 * @class MemoryWriteArchive
 *
 * @brief MemoryWriteArchive is a memory archive writer
 */
class IGTLPROTOCOL_CLASS_API MemoryWriteArchive : public ::fwZip::IWriteArchive
{
public:

    typedef SPTR (MemoryWriteArchive) sptr;

    /// Constructor
    IGTLPROTOCOL_API MemoryWriteArchive(std::vector< char > &buffer);

    /// Destructor
    IGTLPROTOCOL_API ~MemoryWriteArchive();

    /**
     * @brief create new entry in archive and return output stream for this memory file
     *
     * @param[in] path file in archive
     * @return output stream of memory entry archive
     */
    IGTLPROTOCOL_API SPTR(std::ostream) createFile(const ::boost::filesystem::path &path);

    /**
     * @brief Write source file in memory archive
     *
     * @param[in] sourceFile source file
     * @param[in] path file in archive
     * @throw ::fwZip::exception::Write when file cannot be opened
     */
    IGTLPROTOCOL_API void putFile(const ::boost::filesystem::path &sourceFile,
                                  const ::boost::filesystem::path &path) throw(::fwZip::exception::Write);

    /**
     * @brief Create folder in archive
     *
     * @param[in] path folder to create in memory archive in reality it create nothing
     */
    IGTLPROTOCOL_API bool createDir(const ::boost::filesystem::path &path);

    /**
     * @return archive path
     */
    IGTLPROTOCOL_API const ::boost::filesystem::path getArchivePath() const;

    /**
     * @brief write all data stored in archive
     */
    IGTLPROTOCOL_API void writeArchive() throw(::fwZip::exception::Write);

public:
    /**
     * @brief open callback for archive instance
     *
     * @param[in] archive archive instance
     * @param[in] client_data contain a pointer std::vector internally
     */
    static int  open(struct archive *archive, void *client_data);

    /**
     * @brief write callback for archive instance
     *
     * @param[in] archive archive instance
     * @param[out] client_data contain a pointer to a std::vector
     * @param[in] buff buff to copy in vector
     * @param[in] n number of byte to write
     * @return number of byte have been written
     */
    static ssize_t write(struct archive *archive, void *client_data, const void *buff, size_t n);

    /**
     * @brief close callback for archive instance
     *
     *
     * @param[in] archive archive instance
     * @param[in] client_data callback user parameter
     */
    static int  close(struct archive *archive, void *client_data);

protected:

    typedef SPTR (::boost::iostreams::stream<MemoryArchiveSink>) StreamSPtr;

    /// archive path
    ::boost::filesystem::path m_archivePath;

    /// archive structure
    struct archive *m_archive;

    /// buffer where is stored archive data
    std::vector< char > &m_buffer;

    /// vector of stream memory archive files
    std::vector<StreamSPtr> m_sinks;
};

} // namespace archiver
} // namespace igtlProtocol

#endif // __IGTLPROTOCOL_ARCHIVER_MEMORYWRITEARCHIVE_HPP__
