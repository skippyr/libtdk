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

    class Dimensions final
    {
    public:
        Dimensions();
        Dimensions(unsigned short totalColumns, unsigned short totalRows);
        unsigned short GetTotalColumns() const;
        unsigned short GetTotalRows() const;

    private:
        unsigned short m_totalColumns;
        unsigned short m_totalRows;
    };

    class Font final
    {
    public:
        Font() = delete;
        static void SetWeight(Weight weight);
    };

    class Window final
    {
    public:
        Window() = delete;
        static int GetDimensions(Dimensions& dimensions);
        static void SetTitle(const char* title);
    };
}
