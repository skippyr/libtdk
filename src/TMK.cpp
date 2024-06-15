#ifdef _WIN32
#include <io.h>
#endif

#include "TMK.h"

#ifdef _WIN32
#define ISATTY(streamFileID) _isatty(streamFileID)
#else
#define ISATTY(streamFileID) isatty(streamFileID)
#endif

namespace TMK
{
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

    StreamRedirectionException::StreamRedirectionException(std::string description) noexcept : Exception(description)
    {
    }

    InvalidStreamAttributesException::InvalidStreamAttributesException(std::string description) noexcept : Exception(description)
    {
    }

    bool Terminal::s_hasInitializedStreamRedirectionCache = false;
    bool Terminal::s_isInputRedirected = true;
    bool Terminal::s_isOutputRedirected = true;
    bool Terminal::s_isErrorRedirected = true;

#ifdef _WIN32
    void Terminal::InitializeVirtualTerminalProcessing() noexcept
    {
    }
#endif

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

    DWORD Terminal::GetStreamMode(HANDLE handle, const std::string& name)
    {
        DWORD mode;
        if (!GetConsoleMode(handle, &mode))
        {
            throw StreamRedirectionException("can not get mode of the terminal " + name + " stream due to it is redirected.");
        }
        return mode;
    }

    void Terminal::SetStreamMode(HANDLE handle, bool isRedirected, const std::string& name, DWORD mode)
    {
        if (isRedirected)
        {
            throw StreamRedirectionException("can not set the terminal " + name + " mode due to it is redirected.");
        }
        if (!SetConsoleMode(handle, mode))
        {
            throw InvalidStreamAttributesException("can not set the terminal " + name + " mode due to it is invalid.");
        }
    }
#ifdef _WIN32
    HANDLE Terminal::Input::GetHandle() noexcept
    {
        return GetStdHandle(STD_INPUT_HANDLE);
    }
#endif

    int Terminal::Input::GetFileID() noexcept
    {
        return 0;
    }

    const std::string Terminal::Output::m_name = "output";

#ifdef _WIN32
    HANDLE Terminal::Output::GetHandle() noexcept
    {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    DWORD Terminal::Output::GetMode()
    {
        return GetStreamMode(GetHandle(), m_name);
    }

    void Terminal::Output::SetMode(DWORD mode)
    {
        SetStreamMode(GetHandle(), IsRedirected(), m_name, mode);
    }
#endif

    int Terminal::Output::GetFileID() noexcept
    {
        return 1;
    }

    bool Terminal::Output::IsRedirected() noexcept
    {
        InitializeStreamRedirectionCache();
        return s_isOutputRedirected;
    }

#ifdef _WIN32
    HANDLE Terminal::Error::GetHandle() noexcept
    {
        return GetStdHandle(STD_ERROR_HANDLE);
    }
#endif

    int Terminal::Error::GetFileID() noexcept
    {
        return 2;
    }
}
