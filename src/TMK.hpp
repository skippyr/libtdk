#pragma once

#include <cstdio>

namespace TMK
{
    enum class Weight
    {
        Default,
        Bold,
        Dim
    };

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
        static int Write(const char* format, ...);
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
        static int Write(const char* format, ...);
        static bool IsTTY();
        static std::FILE* GetFile();
        static int GetFileNo();
    };

    class Font final
    {
    public:
        Font() = delete;
        static void SetWeight(Weight weight);
    };
}
