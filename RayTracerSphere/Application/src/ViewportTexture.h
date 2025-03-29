#pragma once

#include "Engine.h"

class ViewportTexture
{
public:
	std::shared_ptr<Engine::VertexArray> VA;
	std::shared_ptr<Engine::Shader> Shader;
	Engine::ShaderMetadata ShaderMeta;
public:
	ViewportTexture();

	void Draw() const;
	void Load();
	bool IsShaderLoaded() const { return m_IsShaderLoaded; }

private:
	bool m_IsShaderLoaded = false;

	void InitializeVertexArray();
};