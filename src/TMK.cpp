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
#pragma region WideCharacterOrientationException
#ifndef _WIN32
    WideCharacterOrientationException::WideCharacterOrientationException() noexcept
    {
    }
#endif
#pragma endregion

#pragma region StreamRedirectionException
    StreamRedirectionException::StreamRedirectionException() noexcept
    {
    }
#pragma endregion

#pragma region InvalidStreamAttributesException
    InvalidStreamAttributesException::InvalidStreamAttributesException() noexcept
    {
    }
#pragma endregion

#pragma region WideCharacterOrientationException
    WideCharacterOrientationException::WideCharacterOrientationException()
    {
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

#pragma region Terminal::Stream
    template class Terminal::Stream<0>;
    template class Terminal::Stream<1>;
    template class Terminal::Stream<2>;

    template <int T>
    HANDLE Terminal::Stream<T>::GetHandle() noexcept
    {
        return GetStdHandle(!T ? STD_INPUT_HANDLE : T == 1 ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
    }

    template <int T>
    DWORD Terminal::Stream<T>::GetMode()
    {
        DWORD mode;
        if (!GetConsoleMode(GetHandle(), &mode))
        {
            throw StreamRedirectionException();
        }
        return mode;
    }

    template <int T>
    void Terminal::Stream<T>::SetMode(DWORD mode)
    {
        if (IsRedirected())
        {
            throw StreamRedirectionException();
        }
        if (!SetConsoleMode(GetHandle(), mode))
        {
            throw InvalidStreamAttributesException();
        }
    }

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

#pragma region Terminal::WritableStream
    template class Terminal::WritableStream<1>;
    template class Terminal::WritableStream<2>;

    template <int T>
    void Terminal::WritableStream<T>::Write(std::string format, std::va_list arguments)
    {
        if (GetFile() == Error::GetFile())
        {
            Output::Flush();
        }
        if (std::vfprintf(GetFile(), format.c_str(), arguments) < 0)
        {
            throw WideCharacterOrientationException();
        }
    }

    template <int T>
    void Terminal::WritableStream<T>::Write(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        Write(format, arguments);
        va_end(arguments);
    }

    template <int T>
    void Terminal::WritableStream<T>::WriteLine(std::string format, std::va_list arguments)
    {
        Write(format, arguments);
        std::fputc('\n', GetFile());
    }

    template <int T>
    void Terminal::WritableStream<T>::WriteLine(std::string format, ...)
    {
        std::va_list arguments;
        va_start(arguments, format);
        WriteLine(format, arguments);
        va_end(arguments);
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
