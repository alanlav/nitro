/* =========================================================================
 * This file is part of mt-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * mt-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program; If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 */

#include <sstream>
#include <stdio.h>

#include "import/sys.h"
#include "import/mt.h"
#include "TestCase.h"

using namespace sys;
using namespace mt;
using namespace std;

namespace
{
class AddOp
{
public:
    AddOp()
    {
    }

    void operator()(size_t element) const
    {
        std::ostringstream ostr;
        ostr << element << std::endl;

        // This SHOULD result in not getting the printouts garbled between
        // threads
        ::fprintf(stderr, "%s", ostr.str().c_str());
    }
};

class LocalStorage
{
public:
    LocalStorage() :
        mValue(0)
    {
    }

    void operator()(size_t element) const
    {
        // All we're really showing here is that we never print out the same
        // value twice due to the threads colliding
        mValue = element;
        std::ostringstream ostr;
        ostr << mValue << std::endl;
        ::fprintf(stderr, "%s", ostr.str().c_str());
    }

private:
    mutable size_t mValue;
};

TEST_CASE(Runnable1DTest)
{
    std::cout << "Running test case" << std::endl;
    const AddOp op;
    std::cout << "Calling run1D" << std::endl;
    run1D(10, 16, op);
}

TEST_CASE(Runnable1DWithCopiesTest)
{
    // TODO: Have LocalStorage actually store its values off in a vector, then
    //       show we got all those values.
    std::cout << "Running test case" << std::endl;
    const LocalStorage op;
    std::cout << "Calling run1D" << std::endl;
    run1DWithCopies(47, 16, op);
}
}

int main(int, char**)
{
    TEST_CHECK(Runnable1DTest);
    TEST_CHECK(Runnable1DWithCopiesTest);

    return 0;
}
