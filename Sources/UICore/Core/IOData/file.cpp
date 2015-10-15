/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/file.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/System/databuffer.h"
#if !defined(WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#undef max
#include <limits>
#include <algorithm>

namespace uicore
{

#if defined(WIN32)

	class FileImpl : public File
	{
	public:
		FileImpl() : handle(INVALID_HANDLE_VALUE) { }
		~FileImpl() { close(); }

		void close() override
		{
			if (handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(handle);
				handle = INVALID_HANDLE_VALUE;
			}
		}

		long long size() const override;

		long long seek(long long position) override;
		long long seek_from_current(long long offset) override;
		long long seek_from_end(long long offset) override;

		int try_read(void *data, int size) override;
		void write(const void *data, int size) override;

		FileImpl(const FileImpl &) = delete;
		FileImpl &operator=(const FileImpl &) = delete;

		HANDLE handle;
	};

	std::shared_ptr<File> File::open_existing(const std::string &filename, FileAccess access)
	{
		DWORD access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = FILE_READ_ACCESS; break;
		case FileAccess::write: access_flags = FILE_WRITE_ACCESS; break;
		case FileAccess::read_write: access_flags = FILE_READ_ACCESS | FILE_WRITE_ACCESS; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = CreateFile(Text::to_utf16(filename).c_str(), access_flags, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (file->handle == INVALID_HANDLE_VALUE)
			throw Exception("CreateFile failed");
		return file;
	}

	std::shared_ptr<File> File::open_always(const std::string &filename, FileAccess access)
	{
		DWORD access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = FILE_READ_ACCESS; break;
		case FileAccess::write: access_flags = FILE_WRITE_ACCESS; break;
		case FileAccess::read_write: access_flags = FILE_READ_ACCESS | FILE_WRITE_ACCESS; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = CreateFile(Text::to_utf16(filename).c_str(), access_flags, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (file->handle == INVALID_HANDLE_VALUE)
			throw Exception("CreateFile failed");
		return file;
	}

	std::shared_ptr<File> File::create_always(const std::string &filename, FileAccess access)
	{
		DWORD access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = FILE_READ_ACCESS; break;
		case FileAccess::write: access_flags = FILE_WRITE_ACCESS; break;
		case FileAccess::read_write: access_flags = FILE_READ_ACCESS | FILE_WRITE_ACCESS; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = CreateFile(Text::to_utf16(filename).c_str(), access_flags, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (file->handle == INVALID_HANDLE_VALUE)
			throw Exception("CreateFile failed");
		return file;
	}

	std::shared_ptr<File> File::create_new(const std::string &filename, FileAccess access)
	{
		DWORD access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = FILE_READ_ACCESS; break;
		case FileAccess::write: access_flags = FILE_WRITE_ACCESS; break;
		case FileAccess::read_write: access_flags = FILE_READ_ACCESS | FILE_WRITE_ACCESS; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = CreateFile(Text::to_utf16(filename).c_str(), access_flags, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		if (file->handle == INVALID_HANDLE_VALUE)
			throw Exception("CreateFile failed");
		return file;
	}

	long long FileImpl::size() const
	{
		LARGE_INTEGER size;
		size.QuadPart = 0;
		BOOL result = GetFileSizeEx(handle, &size);
		if (result == FALSE)
			throw Exception("GetFileSizeEx failed");
		return size.QuadPart;
	}

	long long FileImpl::seek(long long position)
	{
		LARGE_INTEGER offset2, new_pos;
		offset2.QuadPart = position;
		new_pos.QuadPart = 0;
		BOOL result = SetFilePointerEx(handle, offset2, &new_pos, FILE_BEGIN);
		if (result == FALSE)
			throw Exception("SetFilePointerEx failed");
		return new_pos.QuadPart;
	}

	long long FileImpl::seek_from_current(long long offset)
	{
		LARGE_INTEGER offset2, new_pos;
		offset2.QuadPart = offset;
		new_pos.QuadPart = 0;
		BOOL result = SetFilePointerEx(handle, offset2, &new_pos, FILE_CURRENT);
		if (result == FALSE)
			throw Exception("SetFilePointerEx failed");
		return new_pos.QuadPart;
	}

	long long FileImpl::seek_from_end(long long offset)
	{
		LARGE_INTEGER offset2, new_pos;
		offset2.QuadPart = offset;
		new_pos.QuadPart = 0;
		BOOL result = SetFilePointerEx(handle, offset2, &new_pos, FILE_END);
		if (result == FALSE)
			throw Exception("SetFilePointerEx failed");
		return new_pos.QuadPart;
	}

	int FileImpl::try_read(void *data, int size)
	{
		DWORD bytes_read = 0;
		BOOL result = ReadFile(handle, data, size, &bytes_read, 0);
		if (result == FALSE)
			throw Exception("ReadFile failed");
		return (int)bytes_read;
	}

	void FileImpl::write(const void *data, int size)
	{
		DWORD written = 0;
		BOOL result = WriteFile(handle, data, size, &written, 0);
		if (result == FALSE)
			throw Exception("WriteFile failed");
		if (written != size)
			throw Exception("Could not write all bytes to file");
	}

#else

	class FileImpl : public File
	{
	public:
		FileImpl() : handle(-1) { }
		~FileImpl() { close(); }

		void close() override
		{
			if (handle != -1)
			{
				::close(handle);
				handle = -1;
			}
		}

		long long size() const override;

		long long seek(long long position) override;
		long long seek_from_current(long long offset) override;
		long long seek_from_end(long long offset) override;

		int try_read(void *data, int size) override;
		void write(const void *data, int size) override;

		FileImpl(const FileImpl &) = delete;
		FileImpl &operator=(const FileImpl &) = delete;

		int handle;
	};

	std::shared_ptr<File> File::open_existing(const std::string &filename, FileAccess access)
	{
		int access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = O_RDONLY; break;
		case FileAccess::write: access_flags = O_WRONLY; break;
		case FileAccess::read_write: access_flags = O_RDWR; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = open(filename.c_str(), access_flags, 0);
		if (file->handle == -1)
			throw Exception("open failed");
		return file;
	}

	std::shared_ptr<File> File::open_always(const std::string &filename, FileAccess access)
	{
		int access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = O_RDONLY; break;
		case FileAccess::write: access_flags = O_WRONLY; break;
		case FileAccess::read_write: access_flags = O_RDWR; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = open(filename.c_str(), access_flags|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
		if (file->handle == -1)
			throw Exception("open failed");
		return file;
	}

	std::shared_ptr<File> File::create_always(const std::string &filename, FileAccess access)
	{
		int access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = O_RDONLY; break;
		case FileAccess::write: access_flags = O_WRONLY; break;
		case FileAccess::read_write: access_flags = O_RDWR; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = open(filename.c_str(), access_flags|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
		if (file->handle == -1)
			throw Exception("open failed");
		return file;
	}

	std::shared_ptr<File> File::create_new(const std::string &filename, FileAccess access)
	{
		int access_flags = 0;
		switch (access)
		{
		case FileAccess::read: access_flags = O_RDONLY; break;
		case FileAccess::write: access_flags = O_WRONLY; break;
		case FileAccess::read_write: access_flags = O_RDWR; break;
		}

		auto file = std::make_shared<FileImpl>();
		file->handle = open(filename.c_str(), access_flags|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
		if (file->handle == -1)
			throw Exception("open failed");
		return file;
	}

	long long FileImpl::size() const
	{
		off_t old_pos = lseek(handle, 0, SEEK_CUR);
		if (old_pos == (off_t)-1)
			throw Exception("lseek failed");

		off_t size = lseek(handle, 0, SEEK_END);
		if (size == (off_t)-1)
			throw Exception("lseek failed");

		lseek(handle, old_pos, SEEK_SET);

		return size;
	}

	long long FileImpl::seek(long long position)
	{
		off_t result = lseek(handle, position, SEEK_SET);
		if (result == (off_t)-1)
			throw Exception("lseek failed");
		return result;
	}

	long long FileImpl::seek_from_current(long long offset)
	{
		off_t result = lseek(handle, offset, SEEK_CUR);
		if (result == (off_t)-1)
			throw Exception("lseek failed");
		return result;
	}

	long long FileImpl::seek_from_end(long long offset)
	{
		off_t result = lseek(handle, offset, SEEK_END);
		if (result == (off_t)-1)
			throw Exception("lseek failed");
		return result;
	}

	int FileImpl::try_read(void *data, int size)
	{
		int result = ::read(handle, data, size);
		if (result == -1)
			throw Exception("read failed");
		return result;
	}

	void FileImpl::write(const void *data, int size)
	{
		int result = ::write(handle, data, size);
		if (result == -1)
			throw Exception("write failed");
		if (result != size)
			throw Exception("Could not write all bytes to file");
	}

#endif

	std::string File::read_all_text(const std::string &filename)
	{
		auto file = FileImpl::open_existing(filename);

		if (file->size() >= std::numeric_limits<size_t>::max() / 2)
			throw Exception("File too large!");

		std::string buffer;
		buffer.resize((size_t)file->size());

		if (buffer.length() > 0)
			file->read(&buffer[0], buffer.length());

		return buffer;
	}

	void File::write_all_text(const std::string &filename, const std::string &text)
	{
		auto file = FileImpl::create_always(filename);
		file->write(text.data(), text.length());
	}

	DataBufferPtr File::read_all_bytes(const std::string &filename)
	{
		auto file = FileImpl::open_existing(filename);

		if (file->size() >= std::numeric_limits<size_t>::max() / 2)
			throw Exception("File too large!");

		auto buffer = DataBuffer::create((size_t)file->size());

		if (buffer->size() > 0)
			file->read(buffer->data(), buffer->size());

		return buffer;
	}

	void File::write_all_bytes(const std::string &filename, const DataBufferPtr &data)
	{
		auto file = FileImpl::create_always(filename);
		file->write(data->data(), data->size());
	}

	void File::copy(const std::string &from, const std::string &to, bool copy_always)
	{
#ifdef WIN32
		BOOL result = CopyFile(Text::to_utf16(from).c_str(), Text::to_utf16(to).c_str(), copy_always ? FALSE : TRUE);
		if (result == FALSE)
			throw Exception("Unable to copy file");
#else
		auto input_file = File::open_existing(from);
		auto output_file = copy_always ? File::create_always(to) : File::create_new(to);
		long long pos = 0;
		long long size = input_file->size();
		auto buffer = DataBuffer::create(1024 * 1024);
		while (pos < size)
		{
			long long amount = std::min(1024 * 1024LL, size - pos);
			input_file->read(buffer->data(), amount);
			output_file->write(buffer->data(), amount);
			pos += amount;
		}
#endif
	}

	void File::remove(const std::string &filename)
	{
#ifdef WIN32
		BOOL result = DeleteFile(Text::to_utf16(filename).c_str());
		if (result == FALSE)
			throw Exception("Unable to delete file");
#else
		int result = unlink(filename.c_str());
		if (result == -1)
			throw Exception("Unable to delete file");
#endif
	}

	bool File::exists(const std::string &filename)
	{
#ifdef WIN32
		HANDLE file = CreateFile(Text::to_utf16(filename).c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, OPEN_EXISTING, 0, 0);
		if (file != INVALID_HANDLE_VALUE)
			CloseHandle(file);
		return file != INVALID_HANDLE_VALUE;
#else
		struct stat stFileInfo;
		return (stat(filename.c_str(), &stFileInfo) == 0);
#endif
	}
}
