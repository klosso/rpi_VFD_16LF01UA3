
#include <getopt.h>
#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for exit */
#include <unistd.h>
#include <pigpio.h>
#include <string.h>
#include <ctype.h>
#include "vfd.hpp"
#include "VFD_16LF01UA3.hpp"



int main(int argc, char *argv[]) {
  int c;
  if (gpioInitialise() < 0)
    return 1;
  gpioSetMode(VFD_RST,PI_OUTPUT);
  gpioSetMode(VFD_CLK,PI_OUTPUT);
  gpioSetMode(VFD_DTA,PI_OUTPUT);

  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"help", no_argument, 0, 0},
      {"append", no_argument, 0, 0},
      {"delete", required_argument, 0, 0},
      {"verbose", no_argument, 0, 0},
      {"clear", required_argument, 0, 'c'},
      {"rotatecw", required_argument, 0, 'w'},
      {0, 0, 0, 0}};

    c = getopt_long(argc, argv, "is:mcr:w:b:d:h?", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 's':
        if (optarg)
          write_string(optarg);
        break;
      case 'm':
        flags |= SWIRL_EFFECT;
        break;

      case 'i':
        reset_vfd();
        break;

      case 'c':
        set_position(0);
        write_string("                ");
        break;
      case 'b':
        if (optarg)
          set_brightnes(atoi(optarg));
        break;
      case 'd':
        if (optarg)
          delay = atoi(optarg);
        break;
      case 'r':
        if(optarg)
          printRotateLine(optarg,0,0);
        break;
      case 'w':
        if(optarg)
          printRotateCW(optarg);
        break;

      case '?':
      case 'h':
        help(argv[0]);
        break;

      default:
        printf("?? getopt returned character code 0%o ??\n", c);
        help(argv[0]);
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    printf("\n");
  }
  gpioTerminate();
  exit(EXIT_SUCCESS);
}

void help(const char *name)
{
  printf("Program to manage text string to VFD display VFD_16LF01UA3 connected to gpio pins\n");
  printf("%s [-icm], [-s <txt>], [-r <txt>], [-w <txt>], [-b <0-31>], [-d <txt>] \n",name);       
  printf("Options:\n");
  printf(" -i        :Initialize VFD display( reset)\n");       
  printf(" -c        :Clear display\n");       
  printf(" -m        :enable swirl effect on displayed text\n");       
  printf(" -b        :set brightness in range 0 to 31 \n");       
  printf(" -s <txt>  :display static text max 16 chars \n");       
  printf(" -r <txt>  :rotate given text in CCW driection. Unlimited length. \n");       
  printf(" -w <txt>  :rotate given text in CW driection. Unlimited length. \n");       
  printf(" -d <Num>  :set delay between rotating characters in us. Default is 100000us \n");       
}
