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

		// Set the rotation in Euler angles (agrees)
		void SetRotationEulerAngles(float x, float y, float z);

		// Rotate the transform to look at the specified position
		void LookAtTarget(const glm::vec3& target);

	public:
		glm::vec3 position;
		glm::quat rotation;

		glm::vec3 up;
		glm::vec3 forward;
		glm::vec3 right;
	};
}

#endif //! DICOM3D_TRANSFORM_HPP
