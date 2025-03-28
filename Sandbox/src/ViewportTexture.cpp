#include "ViewportTexture.h"

ViewportTexture::ViewportTexture()
	: ShaderMeta{"ViewportTexture", "Shader/ViewportTexture/vert.glsl", "Shader/ViewportTexture/frag.glsl"}
{
	InitializeVertexArray();
}

void ViewportTexture::Draw() const
{
	if ( !m_IsShaderLoaded )
	{
		LOG_ERROR("ViewportTexture::Draw: Shader is not loaded!");
		return;
	}

	Shader->Bind();
	Engine::RendererCommand::DrawIndexed(VA);
}

void ViewportTexture::Load()
{
	try
	{
		Shader = Engine::ShaderLoader::Load(ShaderMeta);
		m_IsShaderLoaded = true;
	}
	catch ( const std::exception& e )
	{
		LOG_ERROR("ViewportTexture::Load: %s", e.what());
	}
}

void ViewportTexture::InitializeVertexArray()
{
	VA = std::make_shared<Engine::VertexArray>();

	float vertices[3 * 3 * 2] = {
		-1.0, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	auto vertex_buffer = std::make_shared<Engine::VertexBuffer>(vertices, static_cast<uint32_t>(sizeof(vertices)));
	Engine::BufferLayout layout = {
		{Engine::ShaderDataType::Float2, "a_Position"},
		{Engine::ShaderDataType::Float2, "a_TexCoord"}
	};
	vertex_buffer->SetLayout(layout);
	VA->AddVertexBuffer(vertex_buffer);

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	auto index_buffer = std::make_shared<Engine::IndexBuffer>(indices, static_cast<uint32_t>(sizeof(indices) / sizeof(uint32_t)));
	VA->SetIndexBuffer(index_buffer);
}
