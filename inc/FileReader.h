#ifndef FileReader_H
#define FileReader_H

#include <IResource.h>

#include <stdexcept>

class FileReader {

public:
	std::string
	to_string (void) const {
		return this->contents;
	}

	FileReader (std::string file_path)
	: path (file_path)
	, contents () {
		std::ifstream input_stream;
		input_stream.open (this->path, std::ifstream::in);  {
			std::string line;
			while (std::getline (input_stream, line)) {
				this->contents += line + "\n";
			}
		}
		input_stream.close ();
	}

	virtual
	~FileReader (void) {}
	
private:
	std::string path;
	std::string contents;

};

#endif
