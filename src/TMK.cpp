#pragma region Headers
#ifdef _WIN32
#include <io.h>
#endif

#include "TMK.h"
#pragma endregion

#pragma region Macros
#ifdef _WIN32
#define ISATTY(stream) _isatty(stream)
#else
#define ISATTY(stream) isatty(stream)
#endif
#pragma endregion

namespace TMK
{
#pragma region Exception
    template <ExitCode T>
    Exception<T>::Exception(std::string description) noexcept : m_description(description)
    {
    }

    template <ExitCode T>
    std::string Exception<T>::GetDescription() const noexcept
    {
        return m_description;
    }

    template <ExitCode T>
    ExitCode Exception<T>::GetExitCode() const noexcept
    {
        return m_exitCode;
    }

    template <ExitCode T>
    const char* Exception<T>::what() const noexcept
    {
        return m_description.c_str();
    }
#pragma endregion

#pragma region WideCharacterOrientationException
#ifndef _WIN32
    WideCharacterOrientationException::WideCharacterOrientationException(std::string description) noexcept : Exception(description)
    {
    }
#endif
#pragma endregion

#pragma region StreamRedirectionException
    StreamRedirectionException::StreamRedirectionException(std::string description) noexcept : Exception(description)
    {
    }

    InvalidStreamAttributesException::InvalidStreamAttributesException(std::string description) noexcept : Exception(description)
    {
    }
#pragma endregion

#pragma region Terminal
    bool Terminal::s_hasInitializedStreamRedirectionCache = false;
    bool Terminal::s_isInputRedirected = true;
    bool Terminal::s_isOutputRedirected = true;
    bool Terminal::s_isErrorRedirected = true;

    void Terminal::InitializeStreamRedirectionCache() noexcept
    {
        if (s_hasInitializedStreamRedirectionCache)
        {
            return;
        }
        s_hasInitializedStreamRedirectionCache = true;
        s_isInputRedirected = ISATTY(Input::GetFileID());
        s_isOutputRedirected = ISATTY(Output::GetFileID());
        s_isErrorRedirected = ISATTY(Error::GetFileID());
    }
#pragma endregion

#pragma region Terminal::Stream
    template <int T>
    std::FILE* Terminal::Stream<T>::GetFile() noexcept
    {
        return !T ? stdin : T == 1 ? stdout : stderr;
    }

    template <int T>
    int Terminal::Stream<T>::GetFileID() noexcept
    {
        return T;
    }

    template <int T>
    bool Terminal::Stream<T>::IsRedirected() noexcept
    {
        InitializeStreamRedirectionCache();
        return !T ? s_isInputRedirected : T == 1 ? s_isOutputRedirected : s_isErrorRedirected;
    }
#pragma endregion

#pragma region Terminal::Encoding
    void Terminal::Encoding::SetOutputCodePage(UINT codePage)
    {
        if (!SetConsoleOutputCP(codePage))
        {
            throw InvalidStreamAttributesException("can not set the code page of the terminal output stream due to it is being invalid.");
        }
    }
#pragma endregion
}
