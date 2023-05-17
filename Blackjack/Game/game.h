#pragma once
#include "hand.h"
#include "card.h"
#include "deck.h"
#include "player.h"
#include "imgui/imgui.h"

#define DEFAULT_BUTTON_WIDTH 200
#define DEFAULT_BUTTON_HEIGHT 70

namespace blackjack
{
	class GameBlueprint;

	class BtnBlueprint : public Blueprint
	{
	public:
		BtnBlueprint(const GameObject& obj)
			: Blueprint(obj), m_transform(obj.GetTransformation())
		{
			std::shared_ptr<Texture> buttonTexture;
			SDL_Texture* _btnTexture;
			SDL_Surface* surface = SDL_CreateRGBSurface(0, DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT, 32, 0, 0, 0, 0);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 128, 128, 128));
			_btnTexture = SDL_CreateTextureFromSurface(g_renderer, surface);
			buttonTexture = Texture::PlaceTexture(_btnTexture);
			SDL_FreeSurface(surface);
			
			m_idle = CMP_SPRITE(buttonTexture);
			m_idle.width = DEFAULT_BUTTON_WIDTH;
			m_idle.height = DEFAULT_BUTTON_HEIGHT;
			m_object.SetSprite(m_idle);

			buttonTexture.reset();
			surface = SDL_CreateRGBSurface(0, DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT, 32, 0, 0, 0, 0);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 228, 228, 228));
			_btnTexture = SDL_CreateTextureFromSurface(g_renderer, surface);
			buttonTexture = Texture::PlaceTexture(_btnTexture);
			SDL_FreeSurface(surface);

			m_hover = CMP_SPRITE(buttonTexture);
			m_hover.width = DEFAULT_BUTTON_WIDTH;
			m_hover.height = DEFAULT_BUTTON_HEIGHT;

		}

		bool HandleEvent(SDL_Event& e) override
		{
			if (disabled)
				return false;
			if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			{
				int mouseX = e.button.x;
				int mouseY = e.button.y;

				float x = m_transform.x - DEFAULT_BUTTON_WIDTH / 2.0f + 640;
				float y = -m_transform.y - DEFAULT_BUTTON_HEIGHT / 2.0f + 360;
				SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT };

				if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
					mouseY >= rect.y && mouseY <= rect.y + rect.h)
				{
					Click();
					return true;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int mouseX = e.motion.x;
				int mouseY = e.motion.y;

				float x = m_transform.x - DEFAULT_BUTTON_WIDTH / 2.0f + 640;
				float y = -m_transform.y - DEFAULT_BUTTON_HEIGHT / 2.0f + 360;
				SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT };

				if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
					mouseY >= rect.y && mouseY <= rect.y + rect.h)
				{
					m_object.SetSprite(m_hover);
					return true;
				}
				else
				{
					m_object.SetSprite(m_idle);
					return true;
				}
			}
			return false;
		}

		Event<> Click;
		bool disabled = false;
	private:
		CMP_TRANSFORMATION& m_transform;

		CMP_SPRITE m_idle;
		CMP_SPRITE m_hover;
	};

	class Game
	{
		friend class GameBlueprint;
	public:
		DISABLE_COPY(Game);
		Game(Scene& scene, Player& p)
			: m_scene(scene), m_player(p), m_hand(scene, -300, -200),
			m_dealer(scene, 300, 200), m_deck(), m_gameManager(m_scene.CreateObject("GameManager"))
		{
			std::function<void(void)> overlay =
				[&]() 
			{
				ImGui::GetIO().FontGlobalScale = 3.0f;
				ImGui::Begin("overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
				ImGui::Text("Player: %d", m_dealer.GetHandSum());
				if (ImGui::Button("reveal"))
				{
					m_dealer.Reveal();
				}
				if (ImGui::Button("destoy"))
				{
					Destroy();
				}
				ImGui::End();
				ImGui::GetIO().FontGlobalScale = 1.0f;
			};
			m_gameManager.SetOverlay(overlay);

			// HUD creation
			m_hitButton = scene.CreateObject("Hit button");
			m_hitButtonLabel = m_hitButton.CreateObject("Hit button label");
			m_standButton = scene.CreateObject("Stand button");
			m_standButtonLabel = m_standButton.CreateObject("Stand button label");

			{
				auto& transform = m_hitButton.GetTransformation();
				transform.x = -500;
				transform.y = -150;
				m_bpHitBtn = std::make_shared<BtnBlueprint>(m_hitButton);
				m_bpHitBtn->Click += [&](void) { Hit(); };
				m_hitButton.SetBlueprint(std::dynamic_pointer_cast<Blueprint>(m_bpHitBtn));
			}
			{
				auto& transform = m_standButton.GetTransformation();
				transform.x = -500;
				transform.y = -250;
				m_bpStandBtn = std::make_shared<BtnBlueprint>(m_standButton);
				m_bpStandBtn->Click += [&](void) { Stand(); };
				m_standButton.SetBlueprint(std::dynamic_pointer_cast<Blueprint>(m_bpStandBtn));
			}

			DispenseInitialCards();
			m_handLabelScoreObject = m_scene.CreateObject("Player label");

		}

		void Destroy()
		{
			m_handLabelScoreObject.Destroy();
			m_gameManager.Destroy();
			m_hand.Reset();
			m_dealer.Reset();
		}
		
		void DrawDebugOverlay()
		{
			if (ImGui::CollapsingHeader("Current Game"))
			{
				ImGui::Text("Dealer hand score: %d", m_dealer.GetHandSum());
				ImGui::Text("Player hand score: %d", m_hand.GetHandSum());
				if (ImGui::Button("Add card to dealer"))
				{
					m_dealer.Recieve({CARD_SUIT::CARD_SUIT_SPADES, 14});
				}
				if (ImGui::Button("Reset"))
				{
					_Reset();
				}
			}
		}

		void DispenseInitialCards();

		inline void Hit()
		{
			m_hand.Recieve(m_deck.PullCard()).Flip(true);
		}

		void Stand()
		{
			m_dealer.Reveal();
		}

	private:
		void _Reset()
		{
			m_dealer.Reset();
			m_hand.Reset();
			m_deck = Deck();
			DispenseInitialCards();
		}

		// logical elements
		Scene& m_scene;
		Player& m_player;
		Hand m_hand;
		Hand m_dealer;
		Deck m_deck;

		// Buttons and HUD
		GameObject m_handLabelScoreObject;

		GameObject m_hitButton;
		GameObject m_hitButtonLabel;
		GameObject m_standButton;
		GameObject m_standButtonLabel;
		
		std::shared_ptr<BtnBlueprint> m_bpHitBtn;
		std::shared_ptr<BtnBlueprint> m_bpStandBtn;

		GameObject m_gameManager;
	};

	class GameBlueprint : public Blueprint
	{
	public:
		GameBlueprint(const GameObject& o, Game& game)
			: Blueprint(o), m_game(game)
		{}

		void Initialize() override
		{

		}

		void Update(float fDeltaTime) override
		{
			
		}

	private:
		Game& m_game;
	};
}
