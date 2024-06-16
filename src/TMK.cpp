#pragma region Headers
#ifdef _WIN32
#include <io.h>
#endif

#include "TMK.h"
#pragma endregion

#pragma region Macros
#ifdef _WIN32
#define IS_REDIRECTED(stream) !_isatty(stream::GetFileID())
#else
#define IS_REDIRECTED(stream) !isatty(stream::GetFileID())
#endif
#pragma endregion

namespace TMK
{
#pragma region Terminal
    bool Terminal::s_hasInitializedStreamRedirectionCache = false;
    bool Terminal::s_isInputRedirected = true;
    bool Terminal::s_isOutputRedirected = true;
    bool Terminal::s_isErrorRedirected = true;

#ifdef _WIN32
    void Terminal::InitializeVirtualTerminalProcessing() noexcept
    {
        try
        {
            Output::SetMode(Output::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        catch (StreamRedirectionException&)
        {
            try
            {
                Error::SetMode(Error::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
            catch (StreamRedirectionException&)
            {
            }
        }
    }
#endif

    void Terminal::InitializeStreamRedirectionCache() noexcept
    {
        if (s_hasInitializedStreamRedirectionCache)
        {
            return;
        }
        s_hasInitializedStreamRedirectionCache = true;
        s_isInputRedirected = IS_REDIRECTED(Input);
        s_isOutputRedirected = IS_REDIRECTED(Output);
        s_isErrorRedirected = IS_REDIRECTED(Error);
#ifdef _WIN32
        Encoding::SetOutputCodePage(CP_UTF8);
        InitializeVirtualTerminalProcessing();
#endif
    }
#pragma endregion

#pragma region Terminal::Output
    void Terminal::Output::Flush() noexcept
    {
        std::fflush(GetFile());
    }
#pragma endregion

#pragma region Terminal::Encoding
    void Terminal::Encoding::SetOutputCodePage(UINT codePage)
    {
        if (!SetConsoleOutputCP(codePage))
        {
            throw InvalidStreamAttributesException();
        }
    }
#pragma endregion
}
