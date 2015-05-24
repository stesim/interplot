#pragma once

#include "base.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace interplot
{

class Camera
{
public:
	Camera();
	~Camera();

	void                    setPosition( const glm::vec3& pos );
	inline const glm::vec3& getPosition() const
	{
		return m_vecPosition;
	}
	void                    translate( const glm::vec3& offset );

	void                    setOrientation( const glm::quat& orient );
	inline const glm::quat& getOrientation() const
	{
		return m_quatRotation;
	}
	inline void             rotate( const glm::quat& offset )
	{
		setOrientation( offset * m_quatRotation );
	}

	const glm::mat4& getViewMatrix() const
	{
		assert( !m_bViewModified );
		return m_matView;
	}

	const glm::mat3& getNormalMatrix() const
	{
		assert( !m_bViewModified );
		return m_matNormal;
	}


	void         setFov( float fov );
	inline float getFov() const
	{
		return m_fFov;
	}

	void        setClippingPlanes( float near, float far );
	inline void getClippingPlanes( float& near, float& far ) const
	{
		near = m_fNearPlane;
		far  = m_fFarPlane;
	}

	void                     setResolution( const glm::ivec2& res );
	inline const glm::ivec2& getResolution() const
	{
		return m_ivecResolution;
	}

	const glm::mat4& getProjectionMatrix() const
	{
		assert( !m_bProjectionModified );
		return m_matProjection;
	}

	void updateMatrices();

	const glm::mat4& getViewProjectionMatrix() const
	{
		assert( !m_bViewModified && !m_bProjectionModified );
		return m_matViewProjection;
	}

	glm::vec3 getForwardVector() const;
	glm::vec3 getRightVector() const;
	glm::vec3 getUpVector() const;

private:
	bool       m_bViewModified;
	glm::vec3  m_vecPosition;
	glm::quat  m_quatRotation;
	glm::quat  m_quatRotationConj;
	glm::vec3  m_vecForward;
	glm::vec3  m_vecRight;
	glm::vec3  m_vecUp;
	glm::mat4  m_matView;
	glm::mat3  m_matNormal;

	bool       m_bProjectionModified;
	float      m_fFov;
	float      m_fNearPlane;
	float      m_fFarPlane;
	glm::ivec2 m_ivecResolution;
	glm::mat4  m_matProjection;

	glm::mat4  m_matViewProjection;
};

}
