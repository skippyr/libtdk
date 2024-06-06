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
            static int GetFileNumber();

        private:
            Error() = delete;
        };

    private:
        Terminal() = delete;
    };
}
