#include "MainLayer.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

MainLayer::MainLayer(uint32_t width, uint32_t height, std::filesystem::path assetPath)
	: Engine::Layer("MainLayer"), m_Scene{}, m_Camera{width, height},
	m_CPURayTracer{&m_Camera, &m_Scene, width, height},
	m_AssetPath{ std::move(assetPath) }, m_SceneImGUI(&m_Scene)
{
	m_ViewportTexture = std::make_unique<ViewportTexture>();
	m_ViewportTexture->ShaderMeta.AddBasePath(m_AssetPath);
	m_ViewportTexture->Load();
	m_SavePath = m_AssetPath / "Out";

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
	ImGui::Begin("General");

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

	ImGui::Spacing();

	// Save image
	// Textbox for the save name
	ImGui::InputText("##Save Name", &m_SaveName);
	ImGui::SameLine();
	if (ImGui::Button("Save Image"))
	{
		bool _ = m_CPURayTracer.SaveImage((m_SavePath / (m_SaveName + ".png")), m_RenderTargetType);
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
	// Materials
	RTS::Material ground_material;
	ground_material.Albedo = { 0.8f, 0.8f, 0.0f };
	ground_material.Name = "Ground";
	RTS::Material center_material;
	center_material.Albedo = { 0.1f, 0.2f, 0.5f };
	center_material.Name = "Center";
	RTS::Material left;
	left.Albedo = { 0.8f, 0.8f, 0.8f };
	left.Name = "Left";
	RTS::Material right;
	right.Albedo = { 0.8f, 0.6f, 0.2f };
	right.Name = "Right";
	auto ground_material_id = m_Scene.AddMaterial(ground_material);
	auto center_material_id = m_Scene.AddMaterial(center_material);
	auto left_material_id = m_Scene.AddMaterial(left);
	auto right_material_id = m_Scene.AddMaterial(right);

	// Spheres
	m_Scene.Spheres.emplace_back(
		RTS::Sphere{ {0.0f, -100.5f, -1.0f}, 100.0f, ground_material_id, "Ground" }
	);
	m_Scene.Spheres.emplace_back(
		RTS::Sphere{ {0.0f, 0.0f, -1.2f}, 0.5f, center_material_id, "Center" }
	);
	m_Scene.Spheres.emplace_back(
		RTS::Sphere{ {-1.0f, 0.0f, -1.0f}, 0.5f, left_material_id, "Left" }
	);
	m_Scene.Spheres.emplace_back(
		RTS::Sphere{ {1.0f, 0.0f, -1.0f}, 0.5f, right_material_id, "Right" }
	);

	m_SceneImGUI.SceneChanges();
}
