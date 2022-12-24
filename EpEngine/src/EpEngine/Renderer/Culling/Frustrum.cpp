#include "pch.h"
#include "Frustrum.h"

#include <glm/gtc/matrix_transform.hpp>

namespace EpEngine
{
	static glm::vec4 s_planes[6];
	
	bool Frustrum::Intersects(AABB aabb)
	{
		return	s_planes[0].x * (s_planes[0].x < 0 ? aabb.Min.x : aabb.Max.x) + s_planes[0].y * (s_planes[0].y < 0 ? aabb.Min.y : aabb.Max.y) + s_planes[0].z * (s_planes[0].z < 0 ? aabb.Min.z : aabb.Max.z) >= -s_planes[0].w &&
				s_planes[1].x * (s_planes[1].x < 0 ? aabb.Min.x : aabb.Max.x) + s_planes[1].y * (s_planes[1].y < 0 ? aabb.Min.y : aabb.Max.y) + s_planes[1].z * (s_planes[1].z < 0 ? aabb.Min.z : aabb.Max.z) >= -s_planes[1].w &&
				s_planes[2].x * (s_planes[2].x < 0 ? aabb.Min.x : aabb.Max.x) + s_planes[2].y * (s_planes[2].y < 0 ? aabb.Min.y : aabb.Max.y) + s_planes[2].z * (s_planes[2].z < 0 ? aabb.Min.z : aabb.Max.z) >= -s_planes[2].w &&
				s_planes[3].x * (s_planes[3].x < 0 ? aabb.Min.x : aabb.Max.x) + s_planes[3].y * (s_planes[3].y < 0 ? aabb.Min.y : aabb.Max.y) + s_planes[3].z * (s_planes[3].z < 0 ? aabb.Min.z : aabb.Max.z) >= -s_planes[3].w &&
				s_planes[4].x * (s_planes[4].x < 0 ? aabb.Min.x : aabb.Max.x) + s_planes[4].y * (s_planes[4].y < 0 ? aabb.Min.y : aabb.Max.y) + s_planes[4].z * (s_planes[4].z < 0 ? aabb.Min.z : aabb.Max.z) >= -s_planes[4].w &&
				s_planes[5].x * (s_planes[5].x < 0 ? aabb.Min.x : aabb.Max.x) + s_planes[5].y * (s_planes[5].y < 0 ? aabb.Min.y : aabb.Max.y) + s_planes[5].z * (s_planes[5].z < 0 ? aabb.Min.z : aabb.Max.z) >= -s_planes[5].w;
	}

	void Frustrum::SetPlanes(const glm::mat4& vp)
	{
		s_planes[0] = glm::vec4(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0], vp[3][3] + vp[3][0]);
		s_planes[1] = glm::vec4(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0], vp[3][3] - vp[3][0]);
		s_planes[2] = glm::vec4(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1], vp[3][3] + vp[3][1]);
		s_planes[3] = glm::vec4(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1], vp[3][3] - vp[3][1]);
		s_planes[4] = glm::vec4(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2], vp[3][3] + vp[3][2]);
		s_planes[5] = glm::vec4(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2], vp[3][3] - vp[3][2]);
	}
}