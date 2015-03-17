#ifndef __FILEREADER_H__
#define __FILEREADER_H__

#include <IResource.h>

#include <stdexcept>

class FileReader : private IResource {

private:
	std::string path;
	std::ios::openmode mode;
	std::string contents;
	std::ifstream input_stream;

public:
	FileReader (std::string file_path)
	: path (file_path)
	, mode (std::ifstream::in)
	, input_stream () {
		//prepare f to throw if failbit gets set
		std::ios_base::iostate exceptionMask =
			this->input_stream.exceptions () | std::ios::failbit;
		this->input_stream.exceptions (exceptionMask);
		//
		this->input_stream.open (this->path, this->mode);
	}

	virtual
	~FileReader (void) {
		this->input_stream.close ();
	}

	std::string
	to_string (void) {
		// move file pointer to end of file
		this->input_stream.seekg (0, std::ios::end);
		// read the file size by counting the pointer positions
		size_t length = this->input_stream.tellg ();
		// reset the file pointer
		this->input_stream.seekg (0, std::ios::beg);
		// create buffer to read in
		char buffer[length + 1];
		// read character until EOF
		this->input_stream.read (buffer, length);
		// terminate string
		buffer[length] = '\0';
		// reset the file pointer to be reusable
		this->input_stream.seekg (0, std::ios::beg);

		// create a string object
		return std::string (buffer);
	}

};

#endif
