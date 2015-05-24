#pragma once

#include "base.h"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace interplot
{
	
class Transform
{
public:
	inline void computeMatrix()
	{
		matrix = glm::scale(
				glm::translate( glm::mat4( 1.0 ), location ) *
					glm::mat4_cast( rotation ),
				scale );

		matrixNormal = glm::transpose( glm::mat3_cast(
					glm::conjugate( rotation ) ) );
		matrixNormal[ 0 ] /= scale.x;
		matrixNormal[ 1 ] /= scale.y;
		matrixNormal[ 2 ] /= scale.z;
	}

public:
	glm::quat  rotation;
	glm::vec3  location;
	glm::vec3  scale;
	glm::mat4  matrix;
	glm::mat3  matrixNormal;
};

}
