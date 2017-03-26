#ifndef blurryroots_TextureLoader_h
#define blurryroots_TextureLoader_h

#include <IDisposable.h>
#include <TextureInformation.h>
#include <Util.h>

#include <stb_image.h>

#include <unordered_map>
#include <stdexcept>

class TextureLoader
: IDisposable {

public:
	void
	dispose (void);

	void
	load (std::string path, std::string key, GLuint texture_unit);

	void
	bind (const std::string& key);

	void
	unbind (const std::string& key);

	void
	unload (const std::string& key);

	TextureInformation*
	get_info (const std::string& key) const;

	GLuint
	get_handle (std::string key) const;

	TextureLoader (void);

	virtual
	~TextureLoader (void);

private:
	std::unordered_map<std::string, TextureInformation*> m_texture_infos;

	static TextureInformation*
	read_texture (const std::string& path);

	static void
	dispose_info (TextureInformation* info);

};

#endif
