#include "transform.hpp"

dcm::DCMTransform::DCMTransform() :
	position({ 0.0f, 0.0f, 0.0f }),
	rotation({ 0.0f, 0.0f, 0.0f, 1.0f }),
	up({ 0.0f, 1.0f, 0.0f }),
	forward({ 0.0f, 0.0f, 1.0f }),
	right({ 1.0f, 0.0f, 0.0f })
{
}

dcm::DCMTransform::~DCMTransform()
{
}

void dcm::DCMTransform::Update()
{
	// Update directional vectors
	up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	forward = rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

void dcm::DCMTransform::SetRotationEulerAngles(float angle, const glm::vec3& axis)
{
	rotation = glm::angleAxis(glm::radians(angle), axis);
}

void dcm::DCMTransform::SetRotationEulerAngles(float x, float y, float z)
{
	rotation = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
}

void dcm::DCMTransform::LookAtTarget(const glm::vec3& target)
{
	glm::vec3 direction = glm::normalize(target - position);
	rotation = glm::quatLookAtLH(direction, up);
}
