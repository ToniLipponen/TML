# Contributing

If you find something you could improve, fix or would like to see added to this project, you can help by contributing.

We accept both code and non-code contributions. Code contributions include library code and example uses of this project.
Non-code contributions could be anything, including spell checking files in this project, multimedia content, 
documentation or just simply sharing ideas.

Any code contributed to this project should have the same license as the project itself (*Zlib license*). 
You can add a license / copyright notice to any of the files you have added to this project.

### License template:

___
```
Copyright (C) [current year] [your name]

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
```
___

## Guidelines

1. Any code contributed to this project should be kept as simple and readable as possible.
2. Long and descriptive variable / function names are preferred over short and cryptic ones. 
3. Any external libraries such as glfw or glm, should not be exposed to the end user. 
4. Definition and implementation of classes and functions should be split into separate .h and .cpp files, 
unless it is beneficial to keep everything in the header file.
5. If / else if / else-statements, for loops, etc. should **always** be braced.
6. Anything non-obvious should preferably have a comment explaining what it does and why.
7. Smart pointers are preferred over raw pointers.

## Preferred naming conventions

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


## Example code:

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
        static int s_staticVariable;
    };
}
```
