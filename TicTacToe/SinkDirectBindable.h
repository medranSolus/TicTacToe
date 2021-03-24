#pragma once
#include "Sink.h"

namespace GFX::Pipeline::RenderPass::Base
{
	template<typename T>
	class SinkDirectBindable : public Sink
	{
		static_assert(std::is_base_of_v<GFX::Resource::IBindable, T>, "SinkDirectBindable target type must be a IBindable type!");

		GfxResPtr<T>& target;

	public:
		inline SinkDirectBindable(const std::string& registeredName, GfxResPtr<T>& bind) : Sink(registeredName), target(bind) {}
		virtual ~SinkDirectBindable() = default;

		inline static std::unique_ptr<Sink> Make(const std::string& registeredName, GfxResPtr<T>& bind) { return std::make_unique<SinkDirectBindable>(registeredName, bind); }

		void Bind(Source& source) override;
	};

	template<typename T>
	void SinkDirectBindable<T>::Bind(Source& source)
	{
		auto ptr = source.LinkBindable().CastDynamic<T>();
		if (ptr == nullptr)
			throw RGC_EXCEPT("Binding Sink \"" + GetRegisteredName() + "\" of type {" + typeid(T).name() +
				"} to Source \"" + GetPassPathString() + "." + GetSourceName() + "\" of incompatible type {" + typeid(source.LinkBindable()).name());
		target = std::move(ptr);
		linked = true;
	}
}