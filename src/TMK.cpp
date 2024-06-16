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
#pragma region CMDArguments
#ifdef _WIN32
    CMDArguments::CMDArguments(const std::vector<std::wstring>& utf16Arguments, const std::vector<std::string>& utf8Arguments) noexcept
        : m_utf16Arguments(utf16Arguments), m_utf8Arguments(utf8Arguments)
    {
    }

    const std::vector<std::wstring>& CMDArguments::GetUTF16Arguments()
    {
        return m_utf16Arguments;
    }
#else
    CMDArguments::CMDArguments(const std::vector<std::string>& utf8Arguments) noexcept : m_utf8Arguments(utf8Arguments)
    {
    }
#endif
    std::size_t CMDArguments::GetTotalArguments() noexcept
    {
        return m_utf8Arguments.size();
    }

    const std::vector<std::string>& CMDArguments::GetUTF8Arguments()
    {
        return m_utf8Arguments;
    }
#pragma endregion

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
        catch (const StreamRedirectionException&)
        {
            try
            {
                Error::SetMode(Error::GetMode() | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
            catch (const StreamRedirectionException&)
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
        ResetAttributes();
#ifdef _WIN32
        Encoding::SetOutputCodePage(CP_UTF8);
        InitializeVirtualTerminalProcessing();
#endif
    }

    void Terminal::ResetAttributes() noexcept
    {
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

    std::string Terminal::Encoding::ConvertUTF16ToUTF8(const std::wstring_view& utf16String)
    {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, utf16String.data(), -1, nullptr, 0, nullptr, nullptr);
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.data(), -1, buffer.get(), bufferSize, nullptr, nullptr);
        return buffer.get();
    }

    std::wstring Terminal::Encoding::ConvertUTF8ToUTF16(const std::string_view& utf8String)
    {
        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), -1, nullptr, 0);
        std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(bufferSize);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), -1, buffer.get(), bufferSize);
        return buffer.get();
    }
#endif
#pragma endregion

#pragma region Terminal::Process
    CMDArguments Terminal::Process::GetCMDArguments(int totalRawCMDArguments, const char** rawCMDArguments) noexcept
    {
#ifdef _WIN32
        wchar_t** rawUTF16Arguments = CommandLineToArgvW(GetCommandLineW(), &totalRawCMDArguments);
        std::vector<std::wstring> utf16Arguments;
        std::vector<std::string> utf8Arguments;
        for (std::size_t offset = 0; offset < totalRawCMDArguments; ++offset)
        {
            utf16Arguments.push_back(rawUTF16Arguments[offset]);
            utf8Arguments.push_back(Encoding::ConvertUTF16ToUTF8(rawUTF16Arguments[offset]));
        }
        LocalFree(rawUTF16Arguments);
        return CMDArguments(utf16Arguments, utf8Arguments);
#else
        std::vector<std::string> utf8Arguments;
        for (std::size_t offset = 0; offset < totalRawCMDArguments; ++offset)
        {
            utf8Arguments.push_back(rawCMDArguments[offset]);
        }
        return CMDArguments(utf8Arguments);
#endif
    }

    void Terminal::Process::Exit(unsigned char code) noexcept
    {
        ResetAttributes();
        std::exit(code);
    }

    void Terminal::Process::Exit(ExitCode code) noexcept
    {
        Exit(static_cast<unsigned char>(code));
    }
#pragma endregion

#pragma region Terminal::Output
    void Terminal::Output::Flush() noexcept
    {
        std::fflush(GetFile());
    }
#pragma endregion
}
