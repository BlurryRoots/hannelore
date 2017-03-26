#include <MeshRenderSystem.h>

void
MeshRenderSystem::render_model (
	const blurryroots::model::Mesh* mesh,
	const Transform& transform,
	const std::string& texture_key,
	TextureLoader& texture_loader,
	ShaderProgram& program
	) {
	texture_loader.bind (texture_key);
	glBindVertexArray (mesh->vertex_array_object);

	// calculate and forward mesh transform
	program.set_uniform_mat4 ("m",
		transform.to_matrix ()
		);

	int size;
	glGetBufferParameteriv (
		GL_ELEMENT_ARRAY_BUFFER,
		GL_BUFFER_SIZE,
		&size
		);
	THROW_IF (0 >= size,
		"Invalid element buffer!"
		);

	// draw all the triangles!
	int element_count = size / sizeof (mesh->shapes[0].mesh.indices.at (0));
	int real_element_count = mesh->shapes[0].mesh.indices.size ();
	THROW_IF (element_count != real_element_count,
		"Unequal element_count ", std::to_string (element_count),
		" vs ", std::to_string (real_element_count)
		);
	glDrawElements (GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);

	glBindVertexArray (0);
	texture_loader.unbind (texture_key);
}

void
MeshRenderSystem::activate (void) {

}

void
MeshRenderSystem::deactivate (void) {

}

void
MeshRenderSystem::on_initialize (void) {

}

void
MeshRenderSystem::on_update (double dt) {

}

void
MeshRenderSystem::on_render (ShaderProgram& program) {
	for (auto entity_id : m_entities.get_entities_with_all<Transform, MeshData, MaterialData> ()) {
		auto material_data = m_entities.get_entity_data<MaterialData> (entity_id);

		auto mesh_data = m_entities.get_entity_data<MeshData> (entity_id);
		auto mesh = m_mesh_loader.get (mesh_data->key);

		auto transform = m_entities.get_entity_data<Transform> (entity_id);

		MeshRenderSystem::render_model (mesh, *transform, material_data->texture_name, m_texture_loader, program);
	}
}

MeshRenderSystem::MeshRenderSystem (
	blurryroots::yanecos::EntityManager& entities,
	blurryroots::model::MeshLoader& mesh_loader,
	TextureLoader& texture_loader
)
: m_entities (entities)
, m_mesh_loader (mesh_loader)
, m_texture_loader (texture_loader) {
}
