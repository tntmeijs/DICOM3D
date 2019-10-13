#ifndef DICOM3D_TRANSFORM_HPP
#define DICOM3D_TRANSFORM_HPP

// GLM
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

namespace dcm
{
	/**
	 * Everything that is needed to transform objects in 3D space
	 */
	class DCMTransform
	{
	public:
		DCMTransform();
		~DCMTransform();

		// Update internal matrices
		void Update();

		// Set the rotation in Euler angles (degrees)
		void SetRotationEulerAngles(float angle, const glm::vec3& axis);

		// Get the world matrix
		glm::mat4 GetWorldMatrix() const;

		// Orient the transform to look at the target position
		void LookAtTarget(const glm::vec3& target);

	public:
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;

		glm::vec3 up;
		glm::vec3 forward;
		glm::vec3 right;

	private:
		glm::mat4 m_world_matrix;
		glm::mat4 m_rotation_matrix;
		glm::mat4 m_translation_matrix;
		glm::mat4 m_scale_matrix;
	};
}

#endif //! DICOM3D_TRANSFORM_HPP
