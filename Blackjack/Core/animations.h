#pragma once

namespace blackjack
{	
	class AnimationPath
	{
	public:
		AnimationPath() = default;

		inline void AddStep(float x, float y, float width, float height, float rotation, SDL_Color color, float duration)
		{
			if (duration < m_steps.back().duration)
				throw std::exception("Ivalid argument");
			m_steps.push_back({ x, y, width, height, rotation, color, duration });
		}

		void Update(float deltaTime)
		{

		}

		void draw(SDL_Renderer* renderer)
		{

		}
	private:
		struct Step
		{
			float x, y;
			float width, height;
			float rotation;
			SDL_Color color;
			float duration;
		};

		std::vector<Step> m_steps;
		Step m_currentStep;
		int m_currentIndex = 0;
		float m_elapsedTime = 0.0f;
	};

	class IAnimationState 
	{
	public:
		virtual ~IAnimationState() = default;
		virtual void Enter() {}
		virtual void Update(float deltaTime) {}
		virtual void Exit() {}
		virtual AnimationPath& GetAnimation() = 0;
	};

	class AnimationStateMachine 
	{
	public:
		void SetState(std::unique_ptr<IAnimationState> state) 
		{
			if (m_currentState)
			{
				m_currentState->Exit();
			}
			m_currentState = std::move(state);
			if (m_currentState)
			{
				m_currentState->Enter();
			}
		}

		void Update(float deltaTime) {
			if (m_currentState) {
				m_currentState->Update(deltaTime);
			}
		}

		void Render(SDL_Renderer* renderer) 
		{
			if (m_currentState) 
			{
				m_currentState->GetAnimation().draw(renderer);
			}
		}

	private:
		std::unique_ptr<IAnimationState> m_currentState;
	};
}

