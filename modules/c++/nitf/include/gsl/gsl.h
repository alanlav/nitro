#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Need a fairly decent C++ compiler to use the real GSL
#if (__cplusplus >= 201402L /*C++14*/)
#define NITRO_USE_gsl_ 1
#endif

#if !NITRO_USE_gsl_

#include "gsl_.h"

#else
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4464) // relative include path contains '..'
#endif

#include "../../GSL-3.1.0/include/gsl/gsl_algorithm"	// copy
#include  "../../GSL-3.1.0/include/gsl/gsl_assert"		// Ensures/Expects
#include  "../../GSL-3.1.0/include/gsl/gsl_byte"			// byte
#include  "../../GSL-3.1.0/include/gsl/gsl_util"			// finally()/narrow()/narrow_cast()...
#include  "../../GSL-3.1.0/include/gsl/multi_span"		// multi_span, strided_span...
#include  "../../GSL-3.1.0/include/gsl/pointers"			// owner, not_null
#include  "../../GSL-3.1.0/include/gsl/span"				// span
#include  "../../GSL-3.1.0/include/gsl/string_span"		// zstring, string_span, zstring_builder...

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif