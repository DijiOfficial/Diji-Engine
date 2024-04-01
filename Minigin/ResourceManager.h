#pragma once
#include "Singleton.h"

#include <string>
#include <memory>
#include <unordered_map>
#include "Font.h"

namespace diji
{
	// Inspired from Adam's resource manager, using maps to store textures and fonts as to load them only once
	// resource manager now owns the resources
	class Texture2D;
	class Font;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;

		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_TexturesUPtrUMap{};
		std::unordered_multimap<std::string, std::pair<unsigned int, std::unique_ptr<Font>>> m_FontsUPtrUMap;
	};
}
