#pragma once

#include <iostream>

namespace TMK
{
    class Terminal
    {
    public:
        class Input
        {
        public:
            static std::FILE* GetFile();
            static int GetFileNumber();

        private:
            Input() = delete;
        };

        class Output
        {
        public:
            static void Flush();
            static int WriteLine(std::string format, ...);
            static int WriteLine();
            static int Write(std::string format, ...);
            static std::FILE* GetFile();
            static int GetFileNumber();

        private:
            Output() = delete;
        };

        class Error
        {
        public:
            static int WriteLine(std::string format, ...);
            static int WriteLine();
            static int Write(std::string format, ...);
            static std::FILE* GetFile();
            static int GetFileNumber();

        private:
            Error() = delete;
        };

        class Process
        {
        public:
            class Arguments
            {
            public:
#ifdef _WIN32
                Arguments(int totalArguments, char** utf8Arguments, wchar_t** utf16Arguments);
                std::wstring GetUTF16ArgumentByOffset(std::size_t offset) const;
#endif
                Arguments(int totalArguments, char** utf8Arguments);
                ~Arguments();
                int GetTotalArguments() const;
                std::string GetUTF8ArgumentByOffset(std::size_t offset) const;
                std::string operator[](std::size_t offset);

            private:
#ifdef _WIN32
                wchar_t** m_utf16Arguments;
#endif
                int m_totalArguments;
                char** m_utf8Arguments;
            };

            static Arguments GetArguments(int rawTotalArguments, char** rawArguments);

        private:
            Process() = delete;
        };

        class Dimensions
        {
        public:
            Dimensions();
            Dimensions(unsigned short width, unsigned short height);
            unsigned short GetWidth() const;
            unsigned short GetHeight() const;

        private:
            unsigned short m_width;
            unsigned short m_height;
        };

        class Window
        {
        public:
            static int GetDimensions(Dimensions& dimensions);

        private:
            Window() = delete;
        };

        class Bell
        {
        public:
            static void Ring();

        private:
            Bell() = delete;
        };

    private:
        Terminal() = delete;
    };
}
