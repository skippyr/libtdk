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

#pragma region Terminal::Encoding
#ifdef _WIN32
    void Terminal::Encoding::SetOutputCodePage(UINT codePage)
    {
        if (!SetConsoleOutputCP(codePage))
        {
            throw InvalidStreamAttributesException();
        }
    }

    std::string Terminal::Encoding::ConvertUTF16ToUTF8(const std::wstring& utf16String)
    {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, buffer.get(), bufferSize, nullptr, nullptr);
        return buffer.get();
    }

    std::wstring Terminal::Encoding::ConvertUTF8ToUTF16(const std::string& utf8String)
    {
        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
        std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(bufferSize);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, buffer.get(), bufferSize);
        return buffer.get();
    }
#endif
#pragma endregion

#pragma region Terminal::Output
    void Terminal::Output::Flush() noexcept
    {
        std::fflush(GetFile());
    }
#pragma endregion
}
