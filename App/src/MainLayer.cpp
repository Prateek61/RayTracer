#include "MainLayer.h"
#include "imgui.h"
#include "Utils.h"

void MainLayer::OnAttach()
{
	m_ViewportTexture = std::make_unique<ViewportTexture>(m_Width, m_Height);
	m_ViewportTexture->ShaderMeta.AddBasePath("../Assets/");
	m_ViewportTexture->Load();

	// Render the random image
	RenderImage();
}

void MainLayer::OnUpdate(Engine::TimeStep ts)
{
	Engine::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Engine::RendererCommand::Clear();

	m_ViewportTexture->Draw();
}

void MainLayer::OnImGuiRender(Engine::TimeStep ts)
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

void MainLayer::RenderImage()
{
	Engine::Timer timer;

	uint32_t* data = m_ImageBuffer.As<uint32_t>();

	// Loop over the pixels in the image buffer and set them to a random color
	for (uint32_t y = 0; y < m_Height; y++)
	{
		for (uint32_t x = 0; x < m_Width; x++)
		{
			float r = static_cast<float>(x) / m_Width;
			float g = static_cast<float>(y) / m_Height;
			float b = 0.2f;

			uint32_t pixel = ConvertToPixel(r, g, b);
			data[y * m_Width + x] = pixel;
		}
	}

	m_RenderTime = timer.ElapsedMillis();

	m_ViewportTexture->Texture->SetData(m_ImageBuffer);
}
