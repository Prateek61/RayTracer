#include "ExampleLayer.h"
#include "imgui.h"

void ExampleLayer::OnAttach()
{
	m_ViewportTexture = std::make_unique<ViewportTexture>(m_Width, m_Height);
	m_ViewportTexture->ShaderMeta.AddBasePath("../Assets/");
	m_ViewportTexture->Load();

	// Render the random image
	RenderImage();
}

void ExampleLayer::OnUpdate(Engine::TimeStep ts)
{
	Engine::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Engine::RendererCommand::Clear();

	m_ViewportTexture->Draw();
}

void ExampleLayer::OnImGuiRender(Engine::TimeStep ts)
{
	ImGui::Begin("Settings");

	// Add a button, that renders a new random image if clicked
	if (ImGui::Button("Render Image"))
	{
		RenderImage();
	}

	// Show the render time
	ImGui::Text("Render Time: %.2fms", m_RenderTime);

	ImGui::End();
}

void ExampleLayer::RenderImage()
{
	Engine::Timer timer;

	// Loop over the pixels in the image buffer and set them to a random color
	uint32_t size = m_Width * m_Height;
	for (uint32_t i = 0; i < size; i++)
	{
		uint32_t color = Engine::Random::UInt();
		color |= 0xFF000000;
		m_ImageBuffer.As<uint32_t>()[i] = color;
	}

	m_RenderTime = timer.ElapsedMillis();

	m_ViewportTexture->Texture->SetData(m_ImageBuffer);
}
