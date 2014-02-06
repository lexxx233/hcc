// Copyright (c) Microsoft
// All rights reserved
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
// INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache Version 2.0 License for specific language governing permissions and limitations under the License.
/// <tags>P1</tags>
/// <summary>Test a nested section (at (3, 3, 3) of size (4, 4, 4) of a section (at (0, 0, 0) of size (10, 10, 10) of a 3D array(10, 10, 10) </summary>

// RUN: %amp_device -D__GPU__ %s -m32 -emit-llvm -c -S -O2 -o %t.ll && mkdir -p %t
// RUN: %clamp-device %t.ll %t/kernel.cl
// RUN: pushd %t && objcopy -B i386:x86-64 -I binary -O elf64-x86-64 kernel.cl %t/kernel.o && popd
// RUN: %cxxamp %link %t/kernel.o %s -o %t.out && %t.out
#include <amp.h>
#include "../../../../../array_test.h"
using namespace Concurrency;
using namespace Concurrency::Test;

int main()
{
    ArrayTest<float, 3> original(extent<3>(10, 10, 10));
    ArrayViewTest<float, 3> section1 = original.section(index<3>(0, 0, 0), extent<3>(10, 10, 10));
    ArrayViewTest<float, 3> section2 = section1.section(index<3>(3, 3, 3), extent<3>(4, 4, 4));
    
    // the index<1> parameters here are of the offset (second - first)
    return 
        TestSection(original, section1, index<3>(0, 0, 0)) &&
        TestSection(original, section2, index<3>(3, 3, 3)) &&
        TestSection(section1, section2, index<3>(3, 3, 3))
        ? 0 : 1;
}