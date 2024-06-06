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

    private:
        Terminal() = delete;
    };
}
