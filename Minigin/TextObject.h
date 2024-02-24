#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"

class Font;
class Texture2D;
class TextObject final : public GameObject
{
public:
	TextObject(const std::string& text, std::shared_ptr<Font> font);
	virtual ~TextObject() = default;
	TextObject(const TextObject& other) = delete;
	TextObject(TextObject&& other) = delete;
	TextObject& operator=(const TextObject& other) = delete;
	TextObject& operator=(TextObject&& other) = delete;
	
	void SetNeedsFrameUpdate() { m_NeedsFrameUpdate = true; };
	bool GetNeedsFrameUpdate() const { return m_NeedsFrameUpdate; };

	void Update() override;
	void Render() const override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);

private:
	bool m_needsUpdate;
	bool m_NeedsFrameUpdate;
	std::string m_Text;
	Transform m_transform{};
	std::shared_ptr<Font> m_FontPtr;
	std::shared_ptr<Texture2D> m_TexturePtr;
};
