#include "game.h"

namespace blackjack
{
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

	Game::Game(Player& p)
		: m_player(p), m_hand(*this, -300, -200),
		m_dealer(*this, 300, 200), m_deck(), m_bet(*this, -400, 150), m_overlay(this->CreateObject("Overlay"))
	{
		

		std::function<void(void)> overlay = [&]()
		{
			ImGui::SetNextWindowPos(ImVec2(490, 200));
			ImGui::SetNextWindowSize(ImVec2(300, 200));
			ImGui::Begin("overlay bet", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
			ImGui::Text("Player points: %d$", m_player.points);

			ImGui::Text("Your bet: %d$", m_bet.GetBetValue());
			ImGui::Columns(3, 0, false);
			if (ImGui::Button("+50"))
			{
				if (m_player.points >= 50)
				{
					m_player.points -= 50;
					m_bet.Add50();
				}
			}
			ImGui::NextColumn();
			if (ImGui::Button("+10"))
			{
				if (m_player.points >= 10)
				{
					m_player.points -= 10;
					m_bet.Add10();
				}
			}
			ImGui::NextColumn();
			if (ImGui::Button("Remove")) { m_player.points += m_bet.Remove(); }
			ImGui::Columns(1);
			if (ImGui::Button("OK"))
			{
				if (m_bet.GetBetValue() > 0)
				{
					m_overlay.Destroy();
					// HUD creation
					m_hitButton = this->CreateObject("Hit button");
					m_hitButtonLabel = m_hitButton.CreateObject("Hit button label");
					m_standButton = this->CreateObject("Stand button");
					m_standButtonLabel = m_standButton.CreateObject("Stand button label");
					m_optionsButton = this->CreateObject("Options button");
					m_optionsButtonLabel = m_optionsButton.CreateObject("Options button label");

					{
						auto& transform = m_hitButton.GetTransformation();
						transform.x = -500;
						transform.y = -150;
						auto bpHitBtn = std::make_shared<BtnBlueprint>(m_hitButton);
						bpHitBtn->Click += [&](void) { Hit(); };
						m_hitButton.SetBlueprint(std::dynamic_pointer_cast<Blueprint>(bpHitBtn));
						m_hitButtonLabel.GetTransformation() = transform;
						m_hitButtonLabel.SetSprite(CMP_SPRITE::CreateSprite("assets\\hit.png"));
					}
					{
						auto& transform = m_standButton.GetTransformation();
						transform.x = -500;
						transform.y = -250;
						auto bpStandBtn = std::make_shared<BtnBlueprint>(m_standButton);
						bpStandBtn->Click += [&](void) { Stand(); };
						m_standButton.SetBlueprint(std::dynamic_pointer_cast<Blueprint>(bpStandBtn));
						m_standButtonLabel.GetTransformation() = transform;
						m_standButtonLabel.SetSprite(CMP_SPRITE::CreateSprite("assets\\stand.png"));
					}
					{
						auto& transform = m_optionsButton.GetTransformation();
						transform.x = -500;
						transform.y = -50;
						auto bpOptionsBtn = std::make_shared<BtnBlueprint>(m_optionsButton);
						bpOptionsBtn->Click += [&](void) { GotoOptions(); };
						m_optionsButton.SetBlueprint(std::dynamic_pointer_cast<Blueprint>(bpOptionsBtn));
						m_optionsButtonLabel.GetTransformation() = transform;
						m_optionsButtonLabel.SetSprite(CMP_SPRITE::CreateSprite("assets\\options.png"));
					}
					m_hand.Recieve(m_deck.PullCard()).Flip(true);
					m_hand.Recieve(m_deck.PullCard()).Flip(true);
					m_dealer.Recieve(m_deck.PullCard()).Flip(true);
					m_dealer.Recieve(m_deck.PullCard());
#ifdef ENABLE_DEALER_MUSTHAVE
					while (m_dealer.GetHandSum() < 17)
						m_dealer.Recieve(m_deck.PullCard());
#endif
					if (m_dealer.GetHandSum() > 21)
						::Beep(1000, 100); // player win
				}
				else
				{
					MessageBox(nullptr, L"Bet cannot be zero", L"Warning", 0);
				}
				
			}
			ImGui::End();
		};

		m_overlay.SetOverlay(overlay);	
		m_handLabelScoreObject = this->CreateObject("Player label");
	}

	Game::~Game()
	{
		Scene::Destroy();
	}

	void Game::DrawDebugOverlay()
	{
		if (ImGui::CollapsingHeader("Current Game"))
		{
			ImGui::Text("Dealer hand score: %d", m_dealer.GetHandSum());
			ImGui::Text("Player hand score: %d", m_hand.GetHandSum());
			if (ImGui::Button("Add card to dealer"))
			{
				m_dealer.Recieve({ CARD_SUIT::CARD_SUIT_SPADES, 14 });
			}
		}
	}

	void Game::DispenseInitialCards()
	{
		m_hand.Recieve(m_deck.PullCard()).Flip(true);
		m_hand.Recieve(m_deck.PullCard()).Flip(true);
		m_dealer.Recieve(m_deck.PullCard()).Flip(true);
		m_dealer.Recieve(m_deck.PullCard());
#ifdef ENABLE_DEALER_MUSTHAVE
		while (m_dealer.GetHandSum() < 17)
			m_dealer.Recieve(m_deck.PullCard());
#endif
		if (m_dealer.GetHandSum() > 21)
			::Beep(1000, 100); // player win
	};


	void Game::Hit()
	{
		Resources().GetResource<Sound>("assets\\flip.wav")->Play();
		m_hand.Recieve(m_deck.PullCard(), [&]() {
			if (m_hand.GetHandSum() > 21)
			{
				m_dealer.Reveal();
				Resources().GetResource<Sound>("assets\\l.wav")->Play();
				MessageBox(nullptr, L"Bust!", L"Lose", 0);
				g_defferedAction += [&]() {NewGame(); };
			}
		}).Flip(true);	
	}

	void Game::Stand()
	{
		m_dealer.Reveal();
		
		if (m_dealer.GetHandSum() < m_hand.GetHandSum() && m_hand.GetHandSum() != 21)
		{
			Resources().GetResource<Sound>("assets\\w.wav")->Play();
			MessageBox(nullptr, L"You win!", L"Victory", 0);
			m_player.points += 2 * m_bet.GetBetValue();
			g_defferedAction += [&]() { NewGame(); };
		}
		else
		{
			Resources().GetResource<Sound>("assets\\l.wav")->Play();
			MessageBox(nullptr, L"You lose!", L"Lose", 0);
			g_defferedAction += [&]() { NewGame(); };
		}
	}

}