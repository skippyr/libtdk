#pragma once

#include <cstdio>

namespace TMK
{
    class Input final
    {
    public:
        Input() = delete;
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Output final
    {
    public:
        Output() = delete;
        static int WriteLine(const char* format, ...);
        static int WriteLine();
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Error final
    {
    public:
        Error() = delete;
        static int WriteLine(const char* format, ...);
        static int WriteLine();
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };
}
