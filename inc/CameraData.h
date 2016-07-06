#ifndef hannelore_CameraData_h
#define hannelore_CameraData_h

#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4

struct CameraData : public blurryroots::yanecos::Data<CameraData> {

	float field_of_view;
	float near;
	float far;
	float aspect_ratio;

	glm::mat4 view;
	glm::mat4 projection;

	int yaw;
	int pitch;

	bool is_currently_controlled_camera;

};

#endif