#include "Application.h"
#include <memory>

// エントリポイント
int main(int, char**)
{
    std::unique_ptr<Application> app(new Application());
    
    if (app->Initialize())
    {
        app->RunLoop();
    }
    app->Shutdown();
    return 0;
}



