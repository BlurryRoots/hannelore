#ifndef blurryroots_model_MeshLoader
#define blurryroots_model_MeshLoader

#include <IDisposable.h>
#include <Mesh.h>
#include <ShaderProgram.h>

#include <glm/glm.hpp> 
#include <glm/vec3.hpp>

#include <unordered_map>
#include <functional>

namespace blurryroots { namespace model {

class MeshLoader : IDisposable {

public:
	void
	dispose (void);

	void
	dispose (Mesh* mesh);

	void
	load (
		const std::string& path,
		ShaderProgram& program,
		const std::string& key
	);

	Mesh*
	get (const std::string& key);

	Mesh
	create_mesh (const std::string& path, ShaderProgram& program);

	MeshLoader (void);
	MeshLoader (const MeshLoader& other);

private:
	std::unordered_map<std::string, Mesh*> meshes;

	void
	load_attributed_buffer_data (
		const GLvoid* data_ptr,
		std::size_t byte_size,
		GLenum data_type,
		GLuint target_buffer_id,
		GLint target_attribute_id,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	);

	void
	load_attributed_buffer_data_from (
		const std::vector<float>& data_vector,
		GLuint target_buffer_id,
		GLint target_attribute_id,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	);

	void
	load_attributed_buffer_data_from (
		const std::vector<unsigned int>& data_vector,
		GLuint target_buffer_id,
		GLint target_attribute_id,
		std::size_t components,
		GLenum buffer_type,
		GLenum buffer_strategy
	);

};

}}

#endif
