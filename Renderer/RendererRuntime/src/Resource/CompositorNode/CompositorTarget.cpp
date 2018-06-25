/*********************************************************\
 * Copyright (c) 2012-2018 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RendererRuntime/Resource/CompositorNode/CompositorTarget.h"
#include "RendererRuntime/Resource/CompositorNode/Pass/ICompositorPassFactory.h"
#include "RendererRuntime/Resource/CompositorNode/Pass/ICompositorResourcePass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RendererRuntime
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ICompositorResourcePass* CompositorTarget::addCompositorResourcePass(const ICompositorPassFactory& compositorPassFactory, CompositorPassTypeId compositorPassTypeId)
	{
		ICompositorResourcePass* compositorResourcePass = compositorPassFactory.createCompositorResourcePass(*this, compositorPassTypeId);
		mCompositorResourcePasses.push_back(compositorResourcePass);
		return compositorResourcePass;
	}

	void CompositorTarget::removeAllCompositorResourcePasses()
	{
		const size_t numberOfCompositorResourcePasses = mCompositorResourcePasses.size();
		for (size_t i = 0; i < numberOfCompositorResourcePasses; ++i)
		{
			delete mCompositorResourcePasses[i];
		}
		mCompositorResourcePasses.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RendererRuntime