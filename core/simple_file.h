#ifndef __CORE__SIMPLE_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __CORE__SIMPLE_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ext_io_device.h>
#include <core/trolltech.h>


/* Classes */

/** Basic implementation of the file. */
class SimpleFile : public ExtIODevice
{
public:
	/**
	 * Constructor.
	 * @param path Path to the file.
	 */
	SimpleFile(const QString & path);

	/** Destructor. */
	~SimpleFile();

	/** Closes the file. */
	void close();

	/**
	 * Sets current position in the file.
	 * @param pos New position.
	 */
	bool seek(qint64 pos);

	/** Returns current position in the file. */
	qint64 pos() const;

	/** Returns size of the file. */
	qint64 size() const;

protected:
	/**
	 * Opens the file.
	 * @param mode File open mode.
	 */
	bool open(int mode);

	/**
	 * Reads data from the device.
	 * @param data Pointer to the buffer.
	 * @param maxSize Size of the buffer.
	 * @returns number of bytes read or -1 in case of error.
	 */
	qint64 readData(char * data, qint64 maxSize);

	/**
	 * Writes data from the device.
	 * @param data Pointer to the buffer.
	 * @param maxSize Size of the buffer.
	 * @returns number of bytes written or -1 in case of error.
	 */
	qint64 writeData(const char * data, qint64 maxSize);

private:
	QFile m_File;		/**< Handle of the file. */
};

#endif // __CORE__SIMPLE_FILE_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
