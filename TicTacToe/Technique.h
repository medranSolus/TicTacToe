#pragma once
#include "TechniqueStep.h"

namespace GFX::Pipeline
{
	class Technique : public Probe::IProbeable
	{
		bool active;
		std::string name;
		std::vector<TechniqueStep> steps;

	public:
		inline Technique(const std::string& name, bool active = true) noexcept : active(active), name(name) {}
		inline Technique(std::string&& name, bool active = true) noexcept : active(active), name(std::move(name)) {}
		Technique(Technique&&) = default;
		Technique& operator=(Technique&&) = default;
		~Technique() = default;

		constexpr const std::string& GetName() const noexcept { return name; }
		constexpr bool IsActive() const noexcept { return active; }
		constexpr bool& Active() noexcept { return active; }
		constexpr void Activate() noexcept { active = true; }
		constexpr void Deactivate() noexcept { active = false; }
		inline TechniqueStep& GetStep(size_t i = 0) noexcept { return steps.at(i); }
		inline void AddStep(TechniqueStep&& step) noexcept { steps.emplace_back(std::forward<TechniqueStep&&>(step)); }

		void SetParentReference(Graphics& gfx, const GfxObject& parent);
		void Submit(JobData& data) noexcept;
		bool Accept(Graphics& gfx, Probe::BaseProbe& probe) noexcept override;
	};
}