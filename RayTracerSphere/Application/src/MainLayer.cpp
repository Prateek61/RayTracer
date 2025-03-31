#include "MainLayer.h"
#include "imgui.h"

MainLayer::MainLayer(uint32_t width, uint32_t height, std::filesystem::path assetPath)
	: Engine::Layer("MainLayer"), m_Scene{}, m_Camera{width, height},
	m_CPURayTracer{&m_Camera, &m_Scene, width, height},
	m_AssetPath{ std::move(assetPath) }, m_SceneImGUI(&m_Scene)
{
	m_ViewportTexture = std::make_unique<ViewportTexture>();
	m_ViewportTexture->ShaderMeta.AddBasePath(m_AssetPath);
	m_ViewportTexture->Load();

	InitializeScene();
}

void MainLayer::OnUpdate(Engine::TimeStep ts)
{
	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RendererCommand::Clear();

	if (m_CameraOrSceneChanged)
	{
		m_CPURayTracer.OnSceneOrCameraChange();
		m_CameraOrSceneChanged = false;
	}

	m_CPURayTracer.Update(m_RenderTargetType);
	m_CPURayTracer.BindTexture(0);
	m_ViewportTexture->Draw();
}

void MainLayer::OnEvent(Engine::Event& e)
{
}

void MainLayer::OnImGuiRender(Engine::TimeStep ts)
{
	ImGui::Begin("Status");

	// Frame time
	ImGui::Text("Frame Time: %.2f ms", ts.GetMilliseconds());
	ImGui::Text("FPS: %.1f", 1.0f / ts.GetSeconds());

	// 3 buttons to change the render target
	if (ImGui::Button("Render"))
	{
		m_RenderTargetType = RTS::RenderTargetType::Render;
	}
	ImGui::SameLine();
	if (ImGui::Button("Depth"))
	{
		m_RenderTargetType = RTS::RenderTargetType::Depth;
	}
	ImGui::SameLine();
	if (ImGui::Button("Base Color"))
	{
		m_RenderTargetType = RTS::RenderTargetType::BaseColor;
	}

	ImGui::End();

	ImGui::Begin("Scene Properties");
	m_CameraOrSceneChanged |= m_SceneImGUI.ImGuiProperties();
	ImGui::End();

	ImGui::Begin("Renderer Properties");
	m_CPURayTracer.OnImGUIRender();
	ImGui::End();

	ImGui::Begin("Camera Properties");
	m_CameraOrSceneChanged |= m_Camera.ImGuiProperties();
	ImGui::End();
}

void MainLayer::InitializeScene()
{
	RTS::Material ground_material;
	ground_material.Albedo = { 0.8f, 0.8f, 0.0f };
	ground_material.Name = "Ground";

	auto ground_material_id = m_Scene.AddMaterial(ground_material);
	m_Scene.Spheres.emplace_back(
		RTS::Sphere{ {0.0f, -100.0f, -1.0f}, 100.0f, ground_material_id, "Ground" }
	);
	m_SceneImGUI.SceneChanges();
}
