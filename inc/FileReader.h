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
	, contents ()
	, input_stream () {
		//prepare f to throw if failbit gets set
		/*std::ios_base::iostate exceptionMask =
			this->input_stream.exceptions () | std::ios::failbit;
		this->input_stream.exceptions (exceptionMask);*/

		this->input_stream.open (this->path, this->mode); {
			std::string line;
			while (std::getline (this->input_stream, line)) {
				this->contents += line + "\n";
			}
		}
		this->input_stream.close ();
	}

	virtual
	~FileReader (void) {
		//this->input_stream.close ();
	}

	std::string
	to_string (void) {
		return this->contents;
	}

};

#endif
