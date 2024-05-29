#include <iostream>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
  attributes.c_iflag &= ~IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  std::cout << "Waiting for a key press to dump...";
  int character = std::getchar();
  std::cout << "\x1b[2K\x1b[1G" << character;
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
  while ((character = std::getchar()) != EOF) {
    std::cout << " " << character;
  }
  std::cout << std::endl;
  attributes.c_lflag |= ICANON | ECHO | ISIG;
  attributes.c_iflag |= IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags);
  return 0;
}
