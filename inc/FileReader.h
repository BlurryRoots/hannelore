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

		// move file pointer to end of file
		this->input_stream.seekg (0, std::ios::end);
		// read the file size by counting the pointer positions
		this->contents.resize (this->input_stream.tellg ());
		// reset the file pointer
		this->input_stream.seekg (0, std::ios::beg);
	}

	virtual
	~FileReader () {
		this->input_stream.close ();
	}

	std::string
	read () {
		this->input_stream.read (& this->contents[0], contents.size());

		return this->contents;
	}

};

#endif
