#ifndef CAMERA_PROCESSOR_H
#define CAMERA_PROCESSOR_H

// GLM
#include <glm/glm.h>
#include <glm/matrix.h>
#include <glm/projection.h>
#include <glm/quaternion.h>
#include <glm/transform.h>
#include <glm/utils.h>
#include <glm/vector.h>

#include <Transform.h>

struct CameraData {
	float field_of_view;
	float near;
	float far;
	float aspect_ratio;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 movement;
	glm::vec3 looking;
};

class CameraProcessor {

private:
	Transform transform;
	CameraData data;

public:
	CameraProcessor ()
	: transform () {
	}

	void
	on_viewport_changed ()

};

#endif
