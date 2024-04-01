#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>

void diji::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

diji::Texture2D* diji::ResourceManager::LoadTexture(const std::string& file)
{
	// check if texture is already loaded
	const auto fullPath = m_DataPath + file;
	const auto it = m_TexturesUPtrUMap.find(fullPath);
	if (it != m_TexturesUPtrUMap.cend())
	{
		return it->second.get();
	}

	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	// Store it if it's not already loaded
	m_TexturesUPtrUMap[fullPath] = std::make_unique<Texture2D>(texture);
	return m_TexturesUPtrUMap[fullPath].get();
}

diji::Font* diji::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
    // check if texture is already loaded
    const auto fullPath = m_DataPath + file;
    const auto range = m_FontsUPtrUMap.equal_range(fullPath);
    for (auto it = range.first; it != range.second; ++it)
    {
        if (it->second.first == size)
        {
            return it->second.second.get();
        }
    }

    // Store it if it's not already loaded
    auto font = std::make_unique<Font>(fullPath, size);
    Font* fontPtr = font.get();
    m_FontsUPtrUMap.emplace(fullPath, std::make_pair(size, std::move(font)));

	// I wonder if I could just return LoadFont(fullPath, size) here
    return fontPtr;
}
