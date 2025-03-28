#include "MainLayer.h"
#include "imgui.h"

#include "misc/cpp/imgui_stdlib.h"

void MainLayer::OnAttach()
{
	m_ViewportTexture = std::make_unique<ViewportTexture>();
	m_ViewportTexture->ShaderMeta.AddBasePath("../Assets/");
	m_ViewportTexture->Load();
}

void MainLayer::OnUpdate(Engine::TimeStep ts)
{
	Engine::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Engine::RendererCommand::Clear();

	RenderImage();

	m_ViewportTexture->Draw();
}

void MainLayer::OnImGuiRender(Engine::TimeStep ts)
{
	ImGui::Begin("Settings");

	// Add a button, that renders a new random image if clicked
	/*if (ImGui::Button("Render Image"))
	{
		RenderImage();
	}*/
	// Show the render time
	ImGui::Text("Render Time: %.2fms", m_RenderTime);

	// Add a button, and filepath input to save the image
	ImGui::InputText("Filename", &m_Filename, ImGuiInputTextFlags_CharsNoBlank);
	if (ImGui::Button("Save Image"))
	{
		m_CPURaytracer.SaveImage(m_AssetDir / "Out" / (m_Filename + ".png"));
	}

	ImGui::Text("CPU Raytracer");
	m_CPURaytracer.OnImGuiRender();

	ImGui::End();
}

void MainLayer::RenderImage()
{
	m_RenderTime = m_CPURaytracer.CPURender();
	m_CPURaytracer.BindTexture(0);
}
