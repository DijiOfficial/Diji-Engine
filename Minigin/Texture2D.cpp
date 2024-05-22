#include "Texture2D.h"
#include <SDL.h>

diji::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_TexturePtr);
}

glm::ivec2 diji::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* diji::Texture2D::GetSDLTexture() const
{
	return m_TexturePtr;
}

diji::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_TexturePtr = texture;
}
