#ifndef FileReader_H
#define FileReader_H

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
