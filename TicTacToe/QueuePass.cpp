#include "QueuePass.h"
#include "JobData.h"

namespace GFX::Pipeline::RenderPass::Base
{
	void QueuePass::Execute(Graphics& gfx)
	{
		DRAW_TAG_START(gfx, GetName());
		BindAll(gfx);
		for (auto& job : jobs)
		{
			DRAW_TAG_START(gfx, job.GetData().GetName());
			job.Execute(gfx);
			DRAW_TAG_END(gfx);
		}
		DRAW_TAG_END(gfx);
	}
}