#include <TML/TML.h>
#include <TML/Interface/Interface.h>

using namespace tml;
using namespace Interface;

int main()
{
    Window window(300, 400, "Example 4", Window::VSync);
    Renderer::Init();

    TextInput input(0, 0, 200, 30);
    auto* button = new Button(200, 0, 100, 30, "Add to list");
    auto* list = new Listbox(0, 30, 300, 400 - 30);
    input.AddChild(button, "button");
    input.AddChild(list, "listbox");

    button->SetOnClick({[](BaseComponent* c)
    {
        auto* textInput = (TextInput*)c->GetRoot();
        if(textInput->GetValue().empty())
            return;
        auto* listBox = (Listbox*)textInput->FindComponent("listbox");
        listBox->AddValue(textInput->GetValue());
        textInput->SetValue("");
    }});
    input.Focus();

    while(!window.ShouldClose())
    {
        Renderer::Clear();
            input.Update();
        Renderer::EndBatch();
        window.Display();
    }
}