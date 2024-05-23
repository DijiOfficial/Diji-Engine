#include "Texture.h"

#include "TimeSingleton.h"
#include "Render.h"
#include "ResourceManager.h"
#include "GameObject.h"

diji::Texture::Texture(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_FilePath{ "" }
	, m_Width{ 0 }
	, m_Height{ 0 }
{
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_FilePath{ filename }
	, m_Width{ 0 }
	, m_Height{ 0 }
{
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename, int width, int height)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_FilePath{ filename }
	, m_Width{ width }
	, m_Height{ height }
{
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename, int width, int height, int frames)
	: Texture(ownerPtr, filename, width, height)
{
	m_NrOfFrames = frames;
	m_IsAnimated = true;
}

void diji::Texture::SetTexture(const std::string& filename)
{
	m_TexturePtr = ResourceManager::GetInstance().LoadTexture(filename);
	if (GetOwner()->HasComponent<Render>())
		GetOwner()->GetComponent<Render>()->UpdateTexture(m_TexturePtr);
}

void diji::Texture::SetStartingFrame(int frame)
{
	if (frame > m_StartingFrame)
	{
		m_Frame += frame;
		if ((m_Frame - frame) >= m_NrOfFrames)
			m_Frame = m_StartingFrame;
	}

	m_StartingFrame = frame;
}

void diji::Texture::Init()
{
	if (not m_FilePath.empty())
		SetTexture(m_FilePath);
}

void diji::Texture::Update()
{
	if (not m_IsAnimated or m_IsAnimationPaused)
		return;

	m_FrameTime += TimeSingleton::GetInstance().GetDeltaTime();
	if (m_FrameTime >= 0.3f / m_NrOfFrames)
	{
		++m_Frame;
		m_FrameTime = 0;
		if ((m_Frame - m_StartingFrame) >= m_NrOfFrames)
			m_Frame = m_StartingFrame;
	}
}
