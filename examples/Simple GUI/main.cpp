#include <TML/TML.h>
#include "TML/Interface.h"

using namespace tml;
using namespace Interface;

int main()
{
    RenderWindow window(300, 400, "Simple GUI", Window::VSync);

    TextInput input(0, 0, 200, 30);
    auto* button = new Button(200, 0, 100, 30, "Add to list");
    auto* list = new Listbox(0, 30, 300, 400 - 30);
    input.AddChild(button, "button");
    input.AddChild(list, "listbox");

    button->AddListener("Click", [](BaseComponent* c, const Event&)
    {
        auto* textInput = (TextInput*)c->GetRoot();
        if(textInput->GetValue().empty())
            return true;

        auto* listBox = (Listbox*)textInput->FindComponent("listbox");
        listBox->AddValue(textInput->GetValue());
        textInput->SetValue("");
        return true;
    });
    input.Focus();

    while(!window.ShouldClose())
    {
        auto event = window.PollEvents();
        if(event.type == Event::Closed)
            window.Close();

        window.Clear();
        input.Update(event, window);
        window.Display();
    }
}