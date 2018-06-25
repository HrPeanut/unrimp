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
#include "ExampleRunner.h"
#include "Framework/PlatformTypes.h"
#include "Framework/CommandLineArguments.h"
#include "Framework/IApplicationRenderer.h"
#include "Framework/IApplicationRendererRuntime.h"
// Basics
#include "Basics/FirstTriangle/FirstTriangle.h"
#include "Basics/FirstIndirectBuffer/FirstIndirectBuffer.h"
#include "Basics/VertexBuffer/VertexBuffer.h"
#include "Basics/FirstTexture/FirstTexture.h"
#include "Basics/FirstRenderToTexture/FirstRenderToTexture.h"
#include "Basics/FirstMultipleRenderTargets/FirstMultipleRenderTargets.h"
#ifndef __ANDROID__
	#include "Basics/FirstMultipleSwapChains/FirstMultipleSwapChains.h"
#endif
#include "Basics/FirstInstancing/FirstInstancing.h"
#include "Basics/FirstGeometryShader/FirstGeometryShader.h"
#include "Basics/FirstTessellation/FirstTessellation.h"
// Advanced
#include "Advanced/FirstGpgpu/FirstGpgpu.h"
#include "Advanced/IcosahedronTessellation/IcosahedronTessellation.h"
#ifdef RENDERER_RUNTIME
	#ifdef RENDERER_RUNTIME_IMGUI
		#include "Runtime/ImGuiExampleSelector/ImGuiExampleSelector.h"
	#endif
	#include "Runtime/FirstMesh/FirstMesh.h"
	#include "Runtime/FirstCompositor/FirstCompositor.h"
	#include "Runtime/FirstScene/FirstScene.h"
	#include "Advanced/InstancedCubes/InstancedCubes.h"
#endif

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'return': conversion from 'int' to 'std::char_traits<wchar_t>::int_type', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
	PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::codecvt_base': copy constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt_base': assignment operator was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	PRAGMA_WARNING_DISABLE_MSVC(4774)	// warning C4774: 'sprintf_s' : format string expected in argument 3 is not a string literal
	PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
	#include <array>
	#include <algorithm>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
void ExampleRunner::switchExample(const char* exampleName, const char* rendererName)
{
	assert(nullptr != exampleName);
	mNextRendererName = (nullptr != rendererName) ? rendererName : mDefaultRendererName;
	mNextExampleName = exampleName;
}


//[-------------------------------------------------------]
//[ Protected methods                                     ]
//[-------------------------------------------------------]
ExampleRunner::ExampleRunner() :
	// Case sensitive name of the renderer to instance, might be ignored in case e.g. "RENDERER_DIRECT3D12" was set as preprocessor definition
	// -> Example renderer names: "Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"
	// -> In case the graphics driver supports it, the OpenGL ES 3 renderer can automatically also run on a desktop PC without an emulator (perfect for testing/debugging)
	mDefaultRendererName(
		#ifdef RENDERER_DIRECT3D11
			"Direct3D11"
		#elif defined(RENDERER_OPENGL)
			"OpenGL"
		#elif defined(RENDERER_DIRECT3D10)
			"Direct3D10"
		#elif defined(RENDERER_DIRECT3D9)
			"Direct3D9"
		#elif defined(RENDERER_OPENGLES3)
			"OpenGLES3"
		#elif defined(RENDERER_VULKAN)
			"Vulkan"
		#elif defined(RENDERER_DIRECT3D12)
			"Direct3D12"
		#elif defined(RENDERER_NULL)
			"Null"
		#endif
	)
{
	// Sets of supported renderer backends
	std::array<std::string, 8> supportsAllRenderer	   = {{"Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"}};
	std::array<std::string, 7> doesNotSupportOpenGLES3 = {{"Null", "Vulkan", "OpenGL", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"}};
	std::array<std::string, 6> onlyShaderModel4Plus	   = {{"Null", "Vulkan", "OpenGL", "Direct3D10", "Direct3D11", "Direct3D12"}};
	std::array<std::string, 5> onlyShaderModel5Plus    = {{"Null", "Vulkan", "OpenGL", "Direct3D11", "Direct3D12"}};

	// Basics
	addExample("FirstTriangle",					&runRenderExample<FirstTriangle>,				supportsAllRenderer);
	addExample("FirstIndirectBuffer",			&runRenderExample<FirstIndirectBuffer>,			supportsAllRenderer);
	addExample("VertexBuffer",					&runRenderExample<VertexBuffer>,				supportsAllRenderer);
	addExample("FirstTexture",					&runRenderExample<FirstTexture>,				supportsAllRenderer);
	addExample("FirstRenderToTexture",			&runRenderExample<FirstRenderToTexture>,		supportsAllRenderer);
	addExample("FirstMultipleRenderTargets",	&runRenderExample<FirstMultipleRenderTargets>,	supportsAllRenderer);
	#ifndef __ANDROID__
		addExample("FirstMultipleSwapChains",	&runBasicExample<FirstMultipleSwapChains>,		supportsAllRenderer);
	#endif
	addExample("FirstInstancing",				&runRenderExample<FirstInstancing>,				supportsAllRenderer);
	addExample("FirstGeometryShader",			&runRenderExample<FirstGeometryShader>,			onlyShaderModel4Plus);
	addExample("FirstTessellation",				&runRenderExample<FirstTessellation>,			onlyShaderModel5Plus);

	// Advanced
	addExample("FirstGpgpu",					&runBasicExample<FirstGpgpu>,					supportsAllRenderer);
	addExample("IcosahedronTessellation",		&runRenderExample<IcosahedronTessellation>,		onlyShaderModel5Plus);
	#ifdef RENDERER_RUNTIME
		// Renderer runtime
		#ifdef RENDERER_RUNTIME_IMGUI
			addExample("ImGuiExampleSelector",	&runRenderRuntimeExample<ImGuiExampleSelector>,	supportsAllRenderer);
		#endif
		addExample("FirstMesh",					&runRenderRuntimeExample<FirstMesh>,			supportsAllRenderer);
		addExample("FirstCompositor",			&runRenderRuntimeExample<FirstCompositor>,		supportsAllRenderer);
		addExample("FirstScene",				&runRenderRuntimeExample<FirstScene>,			supportsAllRenderer);
		addExample("InstancedCubes",			&runRenderRuntimeExample<InstancedCubes>,		supportsAllRenderer);
		mDefaultExampleName = "ImGuiExampleSelector";
	#else
		mDefaultExampleName = "FirstTriangle";
	#endif

	#ifdef RENDERER_NULL
		mAvailableRenderers.insert("Null");
	#endif
	#ifdef RENDERER_VULKAN
		mAvailableRenderers.insert("Vulkan");
	#endif
	#ifdef RENDERER_OPENGL
		mAvailableRenderers.insert("OpenGL");
	#endif
	#ifdef RENDERER_OPENGLES3
		mAvailableRenderers.insert("OpenGLES3");
	#endif
	#ifdef RENDERER_DIRECT3D9
		mAvailableRenderers.insert("Direct3D9");
	#endif
	#ifdef RENDERER_DIRECT3D10
		mAvailableRenderers.insert("Direct3D10");
	#endif
	#ifdef RENDERER_DIRECT3D11
		mAvailableRenderers.insert("Direct3D11");
	#endif
	#ifdef RENDERER_DIRECT3D12
		mAvailableRenderers.insert("Direct3D12");
	#endif
}

int ExampleRunner::runExample(const std::string& rendererName, const std::string& exampleName)
{
	// Get selected renderer and selected example
	const AvailableRenderers::iterator selectedRenderer = mAvailableRenderers.find(rendererName);
	const std::string& selectedExampleName = exampleName.empty() ? mDefaultExampleName : exampleName;
	const AvailableExamples::iterator selectedExample = mAvailableExamples.find(selectedExampleName);

	// Ensure the selected renderer is supported by the selected example
	ExampleToSupportedRenderers::iterator supportedRenderer = mExampleToSupportedRenderers.find(selectedExampleName);
	bool rendererNotSupportedByExample = false;
	if (mExampleToSupportedRenderers.end() != supportedRenderer)
	{
		const SupportedRenderers& supportedRendererList = supportedRenderer->second;
		rendererNotSupportedByExample = std::find(supportedRendererList.begin(), supportedRendererList.end(), rendererName) == supportedRendererList.end();
	}
	if (mAvailableExamples.end() == selectedExample || mAvailableRenderers.end() == selectedRenderer || rendererNotSupportedByExample)
	{
		if (mAvailableExamples.end() == selectedExample)
		{
			showError("No or unknown example given");
		}
		if (mAvailableRenderers.end() == selectedRenderer)
		{
			showError("Unknown renderer: \"" + rendererName + "\"");
		}
		if (rendererNotSupportedByExample)
		{
			showError("The example \"" + selectedExampleName + "\" doesn't support renderer: \"" + rendererName + "\"");
		}

		// Print usage
		printUsage(mAvailableExamples, mAvailableRenderers);
		return 0;
	}
	else
	{
		// Run example
		return selectedExample->second(*this, rendererName.c_str());
	}
}