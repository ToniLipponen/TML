<h2>Contributing</h2>

If you find something you could improve, fix or would like to see added to this project, you can help by contributing some code.
Any contribution to this project is going to be have to be the same license as the project itself (Zlib license). 
You can add a license/copyright notice to any of the files you have added to this project.

<h3>License template:</h3>
<b>
Copyright (C) 2021 [insert your name here]

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
</b>

<h2>Guidelines</h2>

Any code contributed to this project should be kept as simple and readable as possible. 
Long and descriptive variable names are preferred over short and cryptic ones. 
Any external libraries such as glfw or glm, should not be exposed to the end user. 
Definition and implementation of classes and functions should be split into separate .h and .cpp files, unless it is benefitial to keep everything in the header file.

<h3>Preferred naming conventions</h3>

| Item                  | Convention    |
|-----------------------|---------------|
| Namespaces            | PascalCase    |
| Class/struct names    | PascalCase    |
| Function names        | PascalCase    |
| Member variables      | m_camelCase   |
| Static variables      | s_camelCase   |
| Function parameters   | camelCase     |
| Variables             | camelCase     |
| Enums                 | PascalCase    |


<h3>Example code:</h3>

```c++
#pragma once

namespace SomeNamespace
{
    class SomeClass
    {
    public:
        enum SomeEnum
        {
            FirstValue,
            SecondValue,
            ThirdValue
        };
    public:
        SomeClass();
        ~SomeClass();
        void SomeFunction();
        void AnotherFunction(float parameterName);
        int ShortFunction() const { return m_memberVariable; }
    private:
        int m_memberVariable;
        int s_staticVariable;
    };
}
```
