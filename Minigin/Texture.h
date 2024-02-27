#pragma once
#include "GameObject.h"

namespace diji
{
	class GameObject;

	class Texture final : public Component
	{
	public:
		Texture(GameObject* ownerPtr);
		Texture(GameObject* ownerPtr, const std::string& filename);
		Texture(GameObject* ownerPtr, const std::string& filename, int width, int height, int idx);
		~Texture() override = default;

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		void Update() override;

		void SetTexture(const std::string& filename);
		std::shared_ptr<Texture2D> GetTexture() const { return m_TexturePtr; };

		void SetWidth(int width) { m_Width = width; }
		void SetHeight(int height) { m_Height = height; }
		void SetIdx(int idx) { m_Idx = idx; }

		bool HasVariableSize() const { return m_HasVariableSize; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetIdx() const { return m_Idx; }

	private:
		std::shared_ptr<Texture2D> m_TexturePtr{};
		int m_Width{};
		int m_Height{};
		int m_Idx{};
		bool m_HasVariableSize{ false };
	};
}

