#ifndef FileReader_H
#define FileReader_H

#include <Util.h>

#include <fstream>

class FileReader {

public:
	std::string
	to_string (void) const {
		return m_contents;
	}

	FileReader (std::string file_path)
	: m_path (file_path)
	, m_contents () {
		std::ifstream input_stream;
		input_stream.open (m_path, std::ifstream::in); 
		if (input_stream.is_open ()) {
			std::string line;
			while (std::getline (input_stream, line)) {
				m_contents += line + "\n";
			}

			input_stream.close ();
		}
		else {
			DEBUG_ERROR ("Could not open file at %s\n", m_path);
		}
	}

	virtual
	~FileReader (void) {}
	
private:
	std::string m_path;
	std::string m_contents;

};

#endif
