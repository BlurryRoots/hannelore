#ifndef __FILEREADER_H__
#define __FILEREADER_H__

#include <IResource.h>

class FileReader : private IResource {

private:
	std::string path;
	std::ios::openmode mode;
	std::string contents;
	std::ifstream input_stream;

public:
	FileReader (std::string file_path)
		: 	path (file_path),
			mode (std::ifstream::in),
			input_stream (this->path, this->mode) {
		if (! this->input_stream.is_open ()) {
			throw (errno);
		}

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
