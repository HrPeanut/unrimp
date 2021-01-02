/*********************************************************\
 * Copyright (c) 2012-2021 The Unrimp Team
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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Renderer/Public/Core/Manager.h"
#include "Renderer/Public/Core/Time/Stopwatch.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <limits>	// For "std::numeric_limits()"
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Time manager
	*/
	class TimeManager final : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline TimeManager() :
			mPastSecondsSinceLastFrame(std::numeric_limits<float>::min()),	// Don't initialize with zero or time advancing enforcement asserts will get more complicated
			mGlobalTimeInSeconds(0.0f),
			mNumberOfRenderedFrames(0),
			mFramesPerSecond(std::numeric_limits<float>::max()),	// Not zero to avoid division through zero border case
			mFramerateSecondsPerFrame{},
			mFramerateSecondsPerFrameIndex(0),
			mFramerateSecondsPerFrameAccumulated(0.0f)
		{
			// Nothing here
		}

		inline ~TimeManager()
		{
			// Nothing here
		}

		[[nodiscard]] inline float getPastSecondsSinceLastFrame() const
		{
			return mPastSecondsSinceLastFrame;
		}

		[[nodiscard]] inline float getGlobalTimeInSeconds() const
		{
			return mGlobalTimeInSeconds;
		}

		[[nodiscard]] inline uint64_t getNumberOfRenderedFrames() const
		{
			return mNumberOfRenderedFrames;
		}

		[[nodiscard]] inline float getFramesPerSecond() const
		{
			return mFramesPerSecond;
		}

		/**
		*  @brief
		*    Time manager update
		*
		*  @note
		*    - Call this once per frame
		*/
		void update();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit TimeManager(const TimeManager&) = delete;
		TimeManager& operator=(const TimeManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Stopwatch mStopwatch;
		float	  mPastSecondsSinceLastFrame;
		float	  mGlobalTimeInSeconds;
		uint64_t  mNumberOfRenderedFrames;
		float	  mFramesPerSecond;
		float	  mFramerateSecondsPerFrame[120];	// Calculate estimate of framerate over the last two seconds
		int		  mFramerateSecondsPerFrameIndex;
		float	  mFramerateSecondsPerFrameAccumulated;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
