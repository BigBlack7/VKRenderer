#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <Window/Application.hpp>

int main()
{
    Core::Application app;
    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}