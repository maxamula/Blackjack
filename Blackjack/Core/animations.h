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
			m_elapsedTime += deltaTime;
			float totalDuration = m_steps[m_steps.size() - 1].duration;
			float timeRatio = m_elapsedTime / totalDuration;
			if (timeRatio >= 1.0f) {
				m_currentIndex = m_steps.size() - 1;
				m_currentStep = m_steps[m_currentIndex];
			}
			else 
			{
				float targetTime = timeRatio * totalDuration;
				while (m_currentIndex < m_steps.size() - 1 && targetTime > m_steps[m_currentIndex + 1].duration) {
					m_currentIndex++;
				}
				float t = (targetTime - m_steps[m_currentIndex].duration) / (m_steps[m_currentIndex + 1].duration - m_steps[m_currentIndex].duration);
				m_currentStep.x = m_steps[m_currentIndex].x + t * (m_steps[m_currentIndex + 1].x - m_steps[m_currentIndex].x);
				m_currentStep.y = m_steps[m_currentIndex].y + t * (m_steps[m_currentIndex + 1].y - m_steps[m_currentIndex].y);
				m_currentStep.width = m_steps[m_currentIndex].width + t * (m_steps[m_currentIndex + 1].width - m_steps[m_currentIndex].width);
				m_currentStep.height = m_steps[m_currentIndex].height + t * (m_steps[m_currentIndex + 1].height - m_steps[m_currentIndex].height);
				m_currentStep.rotation = m_steps[m_currentIndex].rotation + t * (m_steps[m_currentIndex + 1].rotation - m_steps[m_currentIndex].rotation);
				m_currentStep.color.r = m_steps[m_currentIndex].color.r + t * (m_steps[m_currentIndex + 1].color.r - m_steps[m_currentIndex].color.r);
				m_currentStep.color.g = m_steps[m_currentIndex].color.g + t * (m_steps[m_currentIndex + 1].color.g - m_steps[m_currentIndex].color.g);
				m_currentStep.color.b = m_steps[m_currentIndex].color.b + t * (m_steps[m_currentIndex + 1].color.b - m_steps[m_currentIndex].color.b);
				m_currentStep.color.a = m_steps[m_currentIndex].color.a + t * (m_steps[m_currentIndex + 1].color.a - m_steps[m_currentIndex].color.a);
				m_currentStep.duration = m_steps[m_currentIndex].duration + t * (m_steps[m_currentIndex + 1].duration - m_steps[m_currentIndex].duration);
			}
		}

		void Draw(SDL_Renderer* renderer)
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
				m_currentState->GetAnimation().Draw(renderer);
			}
		}

	private:
		std::unique_ptr<IAnimationState> m_currentState;
	};
}

