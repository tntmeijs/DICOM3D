#include "transform.hpp"

dcm::DCMTransform::DCMTransform() :
	position({ 0.0f, 0.0f, 0.0f }),
	scale({ 1.0f, 1.0f, 1.0f }),
	rotation({ 0.0f, 0.0f, 0.0f, 1.0f }),
	up({ 0.0f, 1.0f, 0.0f }),
	forward({ 0.0f, 0.0f, 1.0f }),
	right({ 1.0f, 0.0f, 0.0f }),
	m_world_matrix(1.0f),
	m_rotation_matrix(1.0f),
	m_translation_matrix(1.0f),
	m_scale_matrix(1.0f)
{
}

dcm::DCMTransform::~DCMTransform()
{
}

void dcm::DCMTransform::Update()
{
	m_rotation_matrix = glm::toMat4(rotation);
	m_scale_matrix = glm::scale(scale);
	m_translation_matrix = glm::translate(position);
	m_world_matrix = m_translation_matrix * m_rotation_matrix * m_scale_matrix;

	// Update directional vectors
	up = m_rotation_matrix * glm::vec4(up, 0.0f);
	forward = m_rotation_matrix * glm::vec4(forward, 0.0f);
	right = m_rotation_matrix * glm::vec4(right, 0.0f);
}

void dcm::DCMTransform::SetRotationEulerAngles(float angle, const glm::vec3& axis)
{
	rotation = glm::angleAxis(angle, axis);
}

glm::mat4 dcm::DCMTransform::GetWorldMatrix() const
{
	return m_world_matrix;
	return glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);
}
