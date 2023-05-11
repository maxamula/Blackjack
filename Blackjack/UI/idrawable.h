#pragma once

namespace blackjack
{
	class IDrawable
	{
	public:
		virtual void Draw() const = 0;
		virtual ~IDrawable() = default;
	};

	class UIElement : IDrawable
	{
	public:
		virtual bool HandleEvent(const SDL_Event& event) = 0;
	};

    class Button : public UIElement
    {
    public:
        Button(SDL_Rect rect, std::string text, SDL_Texture* texture)
            : m_rect(rect), m_text(text), m_texture(texture) {}

        void Draw() const override
        {
            // Отрисовка текстуры кнопки и текста на ней
            // Используйте SDL_RenderCopy и SDL_RenderCopyEx для отрисовки текстуры
        }

        bool HandleEvent(const SDL_Event& event) override
        {
            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= m_rect.x && mouseX <= m_rect.x + m_rect.w &&
                    mouseY >= m_rect.y && mouseY <= m_rect.y + m_rect.h)
                {
                    ::Beep(1000, 1000);
                    return true;
                }
            }
            return false;
        }

    private:
        SDL_Rect m_rect;
        std::string m_text;
        SDL_Texture* m_texture;
    };
}

