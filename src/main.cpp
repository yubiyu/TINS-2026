#include "core.h"

#include <cassert>

int main(int argc, char **argv) // Apparently the main function must have this signature for cross-platform compatibility
{
    (void)argc;

    if (!Core::Initialize(argv))
        assert("Initialization error!");

    Core::MainLoop();

    Core::Uninitialize();
    return 0;
}
