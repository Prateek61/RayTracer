#include "CPURayTracer.h"

#include "imgui.h"

namespace RTS
{
	CPURayTracer::CPURayTracer(Camera* camera, Scene* scene, uint32_t width, uint32_t height)
		: m_Camera(camera), m_Scene(scene), m_SceneInteraction(scene)
	{
		PROFILE_FUNCTION();

		OnResize(width, height);
		OnSceneOrCameraChange();
	}

	CPURayTracer::~CPURayTracer()
	{
		PROFILE_FUNCTION();

		// Clear all buffers
		m_AccumulationBuffer.Release();
		m_DepthBuffer.Release();
		m_BaseColorBuffer.Release();
		m_RenderImageBuffer.Release();
		m_DepthImageBuffer.Release();
		m_BaseColorImageBuffer.Release();
	}

	void CPURayTracer::Update(RenderTargetType type)
	{
		PROFILE_FUNCTION();

		if (type == RenderTargetType::None)
		{
			type = m_ActiveRenderType;
		}
		m_ActiveRenderType = type;

		if (type == RenderTargetType::Render)
		{
			if (m_AccumulationCount == 0)
			{
				ResetFloatBuffer(m_AccumulationBuffer);
			}
			Render();
		}
		else if (type == RenderTargetType::Depth)
		{
			RenderDepth();
		}
		else if (type == RenderTargetType::BaseColor)
		{
			RenderBaseColor();
		}
	}

	void CPURayTracer::BindTexture(uint32_t slot, RenderTargetType type)
	{
		PROFILE_FUNCTION();

		if (type == RenderTargetType::None)
		{
			type = m_ActiveRenderType;
		}

		if (type == RenderTargetType::Render)
		{
			m_RenderTexture->Bind(slot);
		}
		else if (type == RenderTargetType::Depth)
		{
			m_DepthTexture->Bind(slot);
		}
		else if (type == RenderTargetType::BaseColor)
		{
			m_BaseColorTexture->Bind(slot);
		}
	}

	void CPURayTracer::OnImGUIRender()
	{
		PROFILE_FUNCTION();

		ImGui::Text("CPU Ray Tracer");

		ImGui::Spacing();
		// Show last render time
		ImGui::Text("Last Render Time: %.2f ms", m_LastAccumulationTime);
		ImGui::Spacing();

		// Checkbox to toggle accumulation
		ImGui::Checkbox("Accumulate", &m_Accumulate);
		// Show the accumulation count
		ImGui::Text("Accumulation Count: %d", m_AccumulationCount);

		ImGui::Spacing();

		// Drag for bounces
		ImGui::DragInt("Bounces", &m_Bounces, 1.0f, 1, 5000);

		// Button to reset the accumulation
		if (ImGui::Button("Clear"))
		{
			OnSceneOrCameraChange();
		}
	}

	void CPURayTracer::OnResize(uint32_t width, uint32_t height)
	{
		PROFILE_FUNCTION();

		m_Width = width;
		m_Height = height;

		ResizeBuffersAndTextures();
		OnSceneOrCameraChange();
	}

	void CPURayTracer::OnSceneOrCameraChange()
	{
		PROFILE_FUNCTION();

		m_AccumulationCount = 0;
		m_BaseColorRendered = false;
		m_DepthRendered = false;
	}

	Engine::Buffer CPURayTracer::GetImageBuffer(RenderTargetType type)
	{
		PROFILE_FUNCTION();

		if (type == RenderTargetType::None)
		{
			type = m_ActiveRenderType;
		}

		if (type == RenderTargetType::Render)
		{
			if (m_AccumulationCount == 0)
				Render();
			return m_RenderImageBuffer;
		}
		else if (type == RenderTargetType::Depth)
		{
			if (!m_DepthRendered)
				RenderDepth();
			return m_DepthImageBuffer;
		}
		else if (type == RenderTargetType::BaseColor)
		{
			if (!m_BaseColorRendered)
				RenderBaseColor();
			return m_BaseColorImageBuffer;
		}

		LOG_ERROR("Invalid RenderTargetType");
		return Engine::Buffer();
	}

	void CPURayTracer::ResizeBuffersAndTextures()
	{
		PROFILE_FUNCTION();

		// Clear all buffers
		m_AccumulationBuffer.Release();
		m_DepthBuffer.Release();
		m_BaseColorBuffer.Release();
		m_RenderImageBuffer.Release();
		m_DepthImageBuffer.Release();
		m_BaseColorImageBuffer.Release();

		// Resize the float buffers
		m_AccumulationBuffer.Allocate(static_cast<uint64_t>(m_Width * m_Height) * sizeof(float) * 4);
		m_DepthBuffer.Allocate(static_cast<uint64_t>(m_Width * m_Height) * sizeof(float) * 4);
		m_BaseColorBuffer.Allocate(static_cast<uint64_t>(m_Width * m_Height) * sizeof(float) * 4);
		// Resize the image buffers
		m_DepthImageBuffer.Allocate(static_cast<uint64_t>(m_Width * m_Height) * sizeof(uint8_t) * 4);
		m_BaseColorImageBuffer.Allocate(static_cast<uint64_t>(m_Width * m_Height) * sizeof(uint8_t) * 4);
		m_RenderImageBuffer.Allocate(static_cast<uint64_t>(m_Width * m_Height) * sizeof(uint8_t) * 4);

		// Create new texture specification
		Engine::TextureSpecification spec;
		spec.Height = m_Height;
		spec.Width = m_Width;
		// Create new textures
		m_DepthTexture = std::make_shared<Engine::Texture2D>(spec);
		m_BaseColorTexture = std::make_shared<Engine::Texture2D>(spec);
		m_RenderTexture = std::make_shared<Engine::Texture2D>(spec);
	}

	void CPURayTracer::ResetFloatBuffer(Engine::Buffer& buffer) const
	{
		PROFILE_FUNCTION();

		float* data = buffer.As<float>();

		for (uint32_t i = 0; i < buffer.Size / sizeof(float); ++i)
		{
			data[i] = 1.0f;
		}
	}

	void CPURayTracer::ResetImageBuffer(Engine::Buffer& buffer) const
	{
		PROFILE_FUNCTION();

		uint32_t* data = buffer.As<uint32_t>();
#pragma omp parallel for
		for (int i = 0; i < static_cast<int>(buffer.Size / sizeof(int)); ++i)
		{
			data[i] = 0xffffffff;
		}
	}

	void CPURayTracer::AddToTexture(Engine::Buffer& buffer, std::shared_ptr<Engine::Texture2D>& texture)
	{
		PROFILE_FUNCTION();

		texture->SetData(buffer);
	}

	void CPURayTracer::ToImageBuffer(Engine::Buffer& buffer, Engine::Buffer& imageBuffer)
	{
		PROFILE_FUNCTION();

		Color4* data = buffer.As<Color4>();
		uint32_t* image_data = imageBuffer.As<uint32_t>();
#pragma omp parallel for
		for (int i = 0; i < static_cast<int>(m_Width * m_Height); ++i)
		{
			image_data[i] = VectorUtils::ConvertToPixel(data[i]);
		}
	}

	void CPURayTracer::Render()
	{
		PROFILE_FUNCTION();

		if (!m_Accumulate && m_AccumulationCount > 0)
		{
			return;
		}

		Engine::Timer timer;
		Accumulate();
		m_LastAccumulationTime = timer.ElapsedMillis();

		Color4* data = m_AccumulationBuffer.As<Color4>();
		uint32_t* image_data = m_RenderImageBuffer.As<uint32_t>();
#pragma omp parallel for
		for (int i = 0; i < static_cast<int>(m_Width * m_Height); ++i)
		{
			Color4 color = data[static_cast<uint32_t>(i)] / static_cast<float>(m_AccumulationCount);
			image_data[i] = VectorUtils::ConvertToPixel(color);
		}

		AddToTexture(m_RenderImageBuffer, m_RenderTexture);

		m_AccumulationCount++;
	}

	void CPURayTracer::RenderDepth()
	{
		PROFILE_FUNCTION();

		if (m_DepthRendered)
		{
			return;
		}

		ComputeDepth();
		ToImageBuffer(m_DepthBuffer, m_DepthImageBuffer);
		AddToTexture(m_DepthImageBuffer, m_DepthTexture);
		m_DepthRendered = true;
	}

	void CPURayTracer::RenderBaseColor()
	{
		PROFILE_FUNCTION();

		if (m_BaseColorRendered)
		{
			return;
		}

		ComputeBaseColor();
		ToImageBuffer(m_BaseColorBuffer, m_BaseColorImageBuffer);
		AddToTexture(m_BaseColorImageBuffer, m_BaseColorTexture);
		m_BaseColorRendered = true;
	}

	void CPURayTracer::Accumulate()
	{
		PROFILE_FUNCTION();

		Color4* data = m_AccumulationBuffer.As<Color4>();

#pragma omp parallel for
		for (int j = 0; j < static_cast<int>(m_Height); ++j)
		{
			for (uint32_t i = 0; i < m_Width; ++i)
			{
				Ray ray = m_Camera->GetRay(i, static_cast<uint32_t>(j));

				Color color = m_SceneInteraction.RayColor(ray, Interval(0.0f, INF), m_Bounces);
				Color4 color4 = Color4(color, 1.0f);
				data[j * m_Width + i] += color4;
			}
		}
	}

	void CPURayTracer::ComputeDepth()
	{
		PROFILE_FUNCTION();

		Color4* data = m_DepthBuffer.As<Color4>();

#pragma omp parallel for
		for (int j = 0; j < static_cast<int>(m_Height); ++j)
		{
			for (uint32_t i = 0; i < m_Width; ++i)
			{
				Ray ray = m_Camera->GetRay(i, static_cast<uint32_t>(j));

				float depth = m_SceneInteraction.Depth(ray, Interval(0.0f, INF));
				depth = VectorUtils::ReinHardTonemap(depth);
				data[j * m_Width + i] = Color4(Color(depth), 1.0f);
			}
		}
	}

	void CPURayTracer::ComputeBaseColor()
	{
		PROFILE_FUNCTION();

		Color4* data = m_BaseColorBuffer.As<Color4>();

#pragma omp parallel for
		for (int j = 0; j < static_cast<int>(m_Height); ++j)
		{
			for (uint32_t i = 0; i < m_Width; ++i)
			{
				Ray ray = m_Camera->GetRay(i, static_cast<uint32_t>(j));
				Color color = m_SceneInteraction.BaseColor(ray, Interval(0.0f, INF));
				data[j * m_Width + i] = Color4(color, 1.0f);
			}
		}
	}
}
