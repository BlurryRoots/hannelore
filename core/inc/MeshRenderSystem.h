#pragma once

#ifndef hannelore_MeshRenderSystem_h
#define hannelore_MeshRenderSystem_h

#include <Mesh.h>
#include <MeshLoader.h>
#include <TextureLoader.h>
#include <ShaderProgram.h>
#include <Transform.h>
#include <MeshData.h>
#include <MaterialData.h>

#include <yanecos/EntityManager.h>
#include <yanecos/IDataProcessor.h>

#include <GLHelper.h>

#include <string>

class MeshRenderProcessor : blurryroots::yanecos::IDataProcessor {

public:
	static void
	render_model (
		const blurryroots::model::Mesh* mesh,
		const Transform& transform,
		const std::string& texture_key,
		TextureLoader& texture_loader,
		ShaderProgram& program
	);

	void
	activate (void) override final;

	void
	deactivate (void) override final;

	void
	on_initialize (void) override final;

	void
	on_update (double dt) override final;

	void
	on_render (ShaderProgram& program) override final;

	void
	on_viewport_changed (int width, int height) override final;

	MeshRenderProcessor (
		blurryroots::yanecos::EntityManager& entities,
		blurryroots::model::MeshLoader& mesh_loader,
		TextureLoader& texture_loader
	);

private:
	blurryroots::yanecos::EntityManager& m_entities;
	blurryroots::model::MeshLoader& m_mesh_loader;
	TextureLoader& m_texture_loader;

};

#endif