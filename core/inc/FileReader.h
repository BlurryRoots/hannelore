#pragma once

#ifndef FileReader_h
#define FileReader_h

#include <string>

class FileReader {

public:
	std::string
	to_string (void) const;

	FileReader (std::string file_path);

	virtual
	~FileReader (void);
	
private:
	std::string m_path;
	std::string m_contents;

};

#endif
