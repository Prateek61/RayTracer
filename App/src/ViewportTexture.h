#pragma once

#include "Engine.h"

class ViewportTexture
{
public:
	uint32_t Width, Height;
	uint32_t TextureSlot = 0;
	std::shared_ptr<Engine::VertexArray> VA;
	std::shared_ptr<Engine::Texture2D> Texture;
	std::shared_ptr<Engine::Shader> Shader;
	Engine::ShaderMetadata ShaderMeta;
public:
	ViewportTexture(uint32_t width, uint32_t height);

	void Draw() const;
	void ResizeTexture(uint32_t width, uint32_t height);
	void Load();
	bool IsShaderLoaded() const { return m_IsShaderLoaded; }

private:
	bool m_IsShaderLoaded = false;

	void InitializeVertexArray();
};