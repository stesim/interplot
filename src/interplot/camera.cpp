#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include <glm/ext.hpp>

namespace interplot
{

Camera::Camera()
	: m_bViewModified( true ),
	m_vecPosition( 0.0f ),
	m_quatRotation( 1.0f, 0.0f, 0.0f, 0.0f ),
	m_quatRotationConj( glm::conjugate( m_quatRotation ) ),
	m_vecForward( 0.0f, 0.0f, -1.0f ),
	m_vecRight( 1.0f, 0.0f, 0.0f ),
	m_vecUp( 0.0f, 1.0f, 0.0f ),
	m_matView( 1.0f ),
	m_matNormal( 1.0f ),
	m_bProjectionModified( true ),
	m_fFov( 70_deg ),
	m_fNearPlane( 0.1f ),
	m_fFarPlane( 100.0f ),
	m_ivecResolution( 640, 480 ),
	m_matProjection( 1.0f ),
	m_matViewProjection( 1.0f )
{
}

Camera::~Camera()
{
}

void Camera::setPosition( const glm::vec3& pos )
{
	m_vecPosition = pos;

	m_bViewModified = true;
}

void Camera::translate( const glm::vec3& offset )
{
	m_vecPosition += offset;

	m_bViewModified = true;
}

void Camera::setOrientation( const glm::quat& orient )
{
	m_quatRotation     = glm::normalize( orient );
	m_quatRotationConj = glm::conjugate( m_quatRotation );

	m_vecForward       = m_quatRotation * -vec_z;
	m_vecRight         = m_quatRotation *  vec_x;
	m_vecUp            = m_quatRotation *  vec_y;

	m_bViewModified = true;
}

void Camera::setFov( float fov )
{
	m_fFov = fov;

	m_bProjectionModified = true;
}

void Camera::setClippingPlanes( float near, float far )
{
	m_fNearPlane = near;
	m_fFarPlane  = far;

	m_bProjectionModified = true;
}

void Camera::setResolution( const glm::ivec2& res )
{
	m_ivecResolution = res;

	m_bProjectionModified = true;
}

void Camera::updateMatrices()
{
	if( m_bViewModified )
	{
		m_matView = glm::mat4_cast( m_quatRotationConj ) *
			glm::translate( glm::mat4( 1.0f ), -m_vecPosition );

		m_matNormal = glm::transpose( glm::mat3_cast( m_quatRotation ) );

		m_bViewModified = false;
	}

	if( m_bProjectionModified )
	{
//		m_matProjection = glm::perspectiveFov(
//				m_fFov,
//				(float)m_ivecResolution[ 0 ],
//				(float)m_ivecResolution[ 1 ],
//				m_fNearPlane,
//				m_fFarPlane );
		m_matProjection = glm::perspective(
				m_fFov,
				(float)m_ivecResolution[ 0 ] / (float)m_ivecResolution[ 1 ],
				m_fNearPlane,
				m_fFarPlane );

		m_bProjectionModified = false;
	}

	m_matViewProjection = m_matProjection * m_matView;
}

glm::vec3 Camera::getForwardVector() const
{
	return m_vecForward;
}

glm::vec3 Camera::getRightVector() const
{
	return m_vecRight;
}

glm::vec3 Camera::getUpVector() const
{
	return m_vecUp;
}

}
