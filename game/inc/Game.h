#pragma once

#ifndef Game_h
#define Game_h

#define PI_OVER_2 1.57079632679f

#include <Fonts.h>

#include <IGame.h>
#include <MeshRenderSystem.h>
#include <ShaderProgramBuilder.h>
#include <CameraData.h>
#include <CameraProcessor.h>
#include <MaterialData.h>
#include <MeshData.h>
#include <MeshLoader.h>
#include <ShaderProgram.h>
#include <TextureLoader.h>
#include <ScreenSpaceTextProcessor.h>

#include <Hashing.h>

#include <yanecos/EntityManager.h>

#include <vector>

#define SID(s) \
	blurryroots::hashing::djb::hash (s)
typedef
	unsigned int
	sid_t
	;

class Game : public IGame {

public:
	bool m_is_running;
	ShaderProgram m_shader_program;
	int framebuffer_width, framebuffer_height;

	struct LightData {
		float light_radius;
		float light_intensity;
		glm::vec3 light_color;
		bool complex_attenuation;
	} m_light_data;

	//Transform models[4];

	FONScontext* m_font_context;
	fsuint m_text_buffer;;
	std::vector<fsuint> m_text_ids;
	long m_framecounter = 0;

public:
	void
	on_initialize (void);

	void
	on_framebuffer (int width, int height);

	void
	on_window_closing_request (void);

	void
	on_update (double dt);

	void
	on_render (void);
	
	void
	on_key_down (KeyCode key, int scancode, KeyModifier mods);

	void
	on_key_up (KeyCode key, int scancode, KeyModifier mods);

	void
	on_mouse_enter (void);

	void
	on_mouse_leave (void);

	void
	on_mouse_button (int button, int action, int mods);

	void
	on_mouse_move (double xpos, double ypos);

	void
	on_mouse_scroll (double xoffset, double yoffset);

	void
	on_joystick_connection (int joystick_id, bool connected);

	void
	on_quit (void);

	void
	on_dispose (void);

	bool
	is_running (void) const;

	void
	quit (void);

	Game (void)
	: m_change_intensity (false)
	, m_suzanne_speed (0.8f)
	, m_entities ()
	, m_mesh_loader ()
	, m_texture_loader ()
	, m_camera_processor (m_entities)
	, m_mesh_render_processor (m_entities, m_mesh_loader, m_texture_loader)
	, m_ss_text_processor (m_entities) {}
	
private:
	void
	toggle_attenuation_complexity ();

	void
	signal_intensity_toggle ();

private:
	bool m_change_intensity;
	float m_suzanne_speed;
	blurryroots::yanecos::EntityManager m_entities;

	TextureLoader m_texture_loader;
	blurryroots::model::MeshLoader m_mesh_loader;

	CameraProcessor m_camera_processor;
	MeshRenderProcessor m_mesh_render_processor;
	ScreenSpaceTextProcessor m_ss_text_processor;

	blurryroots::yanecos::EntityID m_suzanne_id;
	blurryroots::yanecos::EntityID m_light_id;

};

#endif