#include "IWstHttpEngine.h"
#include "WstHttpEngine.h"

IWstHttpEngine* IWstHttpEngine::createHttpEngine(IWstHttpEngineEventHandler* event) {
    std::cout << "IWstHttpEngine::createHttpEngine()" << std::endl;
    return WstHttpEngine::CreateHttpEngine(event);
} 