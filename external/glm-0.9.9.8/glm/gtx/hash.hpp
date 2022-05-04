/// @ref gtx_hash
/// @file glm/gtx/hash.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_hash GLM_GTX_hash
/// @ingroup gtx
///
/// Include <glm/gtx/hash.hpp> to use the features of this extension.
///
/// Add std::hash support for glm types

#pragma once

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	ifndef GLM_ENABLE_EXPERIMENTAL
#		pragma message("GLM: GLM_GTX_hash is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it.")
#	else
#		pragma message("GLM: GLM_GTX_hash extension included")
#	endif
#endif

#include <functional>

#include "glm-0.9.9.8/glm/vec2.hpp"
#include "glm-0.9.9.8/glm/vec3.hpp"
#include "glm-0.9.9.8/glm/vec4.hpp"
#include "glm-0.9.9.8/glm/gtc/vec1.hpp"

#include "glm-0.9.9.8/glm/gtc/quaternion.hpp"
#include "dual_quaternion.hpp"

#include "glm-0.9.9.8/glm/mat2x2.hpp"
#include "glm-0.9.9.8/glm/mat2x3.hpp"
#include "glm-0.9.9.8/glm/mat2x4.hpp"

#include "glm-0.9.9.8/glm/mat3x2.hpp"
#include "glm-0.9.9.8/glm/mat3x3.hpp"
#include "glm-0.9.9.8/glm/mat3x4.hpp"

#include "glm-0.9.9.8/glm/mat4x2.hpp"
#include "glm-0.9.9.8/glm/mat4x3.hpp"
#include "glm-0.9.9.8/glm/mat4x4.hpp"

#if !GLM_HAS_CXX11_STL
#	error "GLM_GTX_hash requires C++11 standard library support"
#endif

namespace std
{
	template<typename T, glm::qualifier Q>
	struct hash<glm::vec<1, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::vec<1, T, Q> const& v) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::vec<2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::vec<2, T, Q> const& v) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::vec<3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::vec<3, T, Q> const& v) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::vec<4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::vec<4, T, Q> const& v) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::qua<T,Q>>
	{
		GLM_FUNC_DECL size_t operator()(glm::qua<T, Q> const& q) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::tdualquat<T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::tdualquat<T,Q> const& q) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<2, 2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<2, 2, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<2, 3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<2, 3, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<2, 4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<2, 4, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<3, 2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<3, 2, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<3, 3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<3, 3, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<3, 4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<3, 4, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<4, 2, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<4, 2, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<4, 3, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<4, 3, T,Q> const& m) const;
	};

	template<typename T, glm::qualifier Q>
	struct hash<glm::mat<4, 4, T,Q> >
	{
		GLM_FUNC_DECL size_t operator()(glm::mat<4, 4, T,Q> const& m) const;
	};
} // namespace std

#include "hash.inl"
