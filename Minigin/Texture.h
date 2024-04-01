#pragma once
#include "GameObject.h"

namespace diji
{
	class Texture final : public Component
	{
	public:
		Texture(GameObject* ownerPtr);
		Texture(GameObject* ownerPtr, const std::string& filename);
		Texture(GameObject* ownerPtr, const std::string& filename, int width, int height);
		Texture(GameObject* ownerPtr, const std::string& filename, int width, int height, int frames);
		~Texture() override = default;

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		void Update() override;

		void SetTexture(const std::string& filename);
		Texture2D* GetTexture() const { return m_TexturePtr; };

		void SetWidth(int width) { m_Width = width; }
		void SetHeight(int height) { m_Height = height; }
		void SetAnimated() { m_IsAnimated = true; }
		void SetNrOfFrames(int nrOfFrames) { m_NrOfFrames = nrOfFrames; }

		bool IsAnimated() const { return m_IsAnimated; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetFrame() const { return m_Frame; }

	private:
		Texture2D* m_TexturePtr;
		int m_Width;
		int m_Height;
		int m_NrOfFrames;
		int m_Frame;
		float m_FrameTime;
		
		bool m_IsAnimated;
	};
}

