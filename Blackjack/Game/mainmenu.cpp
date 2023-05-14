//#include "mainmenu.h"
//#include "Core/scene.h"
//#include "Core/animations.h"
//
//#define MAIN_MENU_BUTTON_WIDTH 280
//#define MAIN_MENU_BUTTON_HEIGHT 90
//
//namespace blackjack
//{
//	namespace
//	{
//
//		class MainMenuButtonEventHandler : public EventHandler
//		{
//		public:
//			MainMenuButtonEventHandler(const GameObject& obj) : EventHandler(obj), m_transform(obj.GetTransformation()),
//				m_sprite(obj.GetSprite())
//			{
//				SDL_Texture* btnTexture;
//				{
//					SDL_Surface* surface = SDL_CreateRGBSurface(0, MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT, 32, 0, 0, 0, 0);
//					SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 128, 128, 128));
//					btnTexture = SDL_CreateTextureFromSurface(g_renderer, surface);
//					SDL_FreeSurface(surface);
//				}		
//
//				m_idle = CMP_SPRITE(btnTexture);
//				m_idle.width = MAIN_MENU_BUTTON_WIDTH;
//				m_idle.height = MAIN_MENU_BUTTON_HEIGHT;
//
//				{
//					SDL_Surface* surface = SDL_CreateRGBSurface(0, MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT, 32, 0, 0, 0, 0);
//					SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 228, 228, 228));
//					btnTexture = SDL_CreateTextureFromSurface(g_renderer, surface);
//					SDL_FreeSurface(surface);
//				}
//				
//				m_hover = CMP_SPRITE(btnTexture);
//				m_hover.width = MAIN_MENU_BUTTON_WIDTH;
//				m_hover.height = MAIN_MENU_BUTTON_HEIGHT;
//			}
//
//			bool HandleEvent(SDL_Event& event)
//			{
//				if (event.type == SDL_EventType::SDL_MOUSEMOTION)
//				{
//					uint32_t vpWidth;
//					uint32_t vpHeight;
//					{
//						int windowWidth, windowHeight;
//						SDL_GetWindowSize(g_mainWindow, &windowWidth, &windowHeight);
//						int top, left, bottom, right;
//						SDL_GetWindowBordersSize(g_mainWindow, &top, &left, &bottom, &right);
//						vpWidth = windowWidth - left - right;
//						vpHeight = windowHeight - top - bottom;
//					}
//					int mouseX = event.motion.x;
//					int mouseY = event.motion.y;
//
//					SDL_Rect btnRect =
//					{
//						(m_transform.x * vpWidth) - MAIN_MENU_BUTTON_WIDTH / 2,
//						(m_transform.y * vpHeight) - MAIN_MENU_BUTTON_HEIGHT / 2,
//						(m_transform.x * vpWidth) + MAIN_MENU_BUTTON_WIDTH / 2,
//						(m_transform.y * vpHeight) + MAIN_MENU_BUTTON_HEIGHT / 2,
//					};
//
//					if (mouseX >= btnRect.x && mouseX <= btnRect.w &&
//						mouseY >= btnRect.y && mouseY <= btnRect.h)
//					{
//						m_state = BUTTON_STATE_HOVER;
//						m_sprite = m_hover;
//						return true;
//					}
//					if (m_state == BUTTON_STATE_HOVER)
//					{
//						m_sprite = m_idle;
//						m_state = BUTTON_STATE_IDLE;
//					}
//					return true;
//				}
//				return false;
//			}
//		private:
//			enum BUTTON_STATE
//			{
//				BUTTON_STATE_IDLE,
//				BUTTON_STATE_HOVER
//			};
//
//			BUTTON_STATE m_state;
//			CMP_TRANSFORMATION& m_transform;
//			CMP_SPRITE& m_sprite;
//
//			CMP_SPRITE m_idle{};
//			CMP_SPRITE m_hover{};
//		};
//
//	} // anon namespace
//
//	void CreateMainMenu(Scene& scene)
//	{
//		// Play button
//		//GameObject objBtnPlay = scene.CreateObject();
//		//auto& btnPlayTransformation = objBtnPlay.GetTransformation();
//		//btnPlayTransformation.x = 0.0f;
//		//btnPlayTransformation.y = 0.0f;
//
//		//SDL_Surface* surface = SDL_CreateRGBSurface(0, MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT, 32, 0, 0, 0, 0);
//		//SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 128, 128, 128));
//		//SDL_Texture* btnTexture = SDL_CreateTextureFromSurface(g_renderer, surface);
//		//SDL_FreeSurface(surface);
//
//		//CMP_SPRITE btnSprite(btnTexture);
//		//btnSprite.width = MAIN_MENU_BUTTON_WIDTH;
//		//btnSprite.height = MAIN_MENU_BUTTON_HEIGHT;
//
//		//objBtnPlay.SetSprite(btnSprite);
//
//		//objBtnPlay.SetHandler(std::make_unique<MainMenuButtonEventHandler>(objBtnPlay));
//
//		//// Exit btn
//		//GameObject objBtnExt = scene.CreateObject();
//		//auto& btnExtTransformation = objBtnExt.GetTransformation();
//		//btnExtTransformation.x = 0.5;
//		//btnExtTransformation.y = 0.63;
//
//		//objBtnExt.SetSprite(btnSprite);
//
//		//objBtnExt.SetHandler(std::make_unique<MainMenuButtonEventHandler>(objBtnExt));
//
//		
//	}
//}