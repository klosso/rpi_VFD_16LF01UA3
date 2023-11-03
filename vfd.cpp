#include <getopt.h>
#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for exit */
#include <unistd.h>
//#include <chrono>
#include <pigpio.h>
#include <string.h>
#include <ctype.h>


#define VFD_RST 20
#define VFD_CLK 21
#define VFD_DTA 16
/*
 * connection VFD display to Raspbery pi GPIO
 *  rpi                          VFD_16LF01UA3
 *  ---|                       |------
 *     |             +5V(1,2)--|
 *     |-- VFD_RST -- RST(10)--|
 *     |-- VFD_CLK -- SCLK(8)--|
 *     |-- VFD_DTA -- DATA(9)--|
 *     |           GND(11,12)--|
 *  ---|                       |-------
 */

#define SWIRL_EFFECT 0x1


char flags = 0;
unsigned int delay=100000;
void help(const char *name);
int write_string(const char *txt);
void set_position(unsigned char pos);
void set_brightnes(unsigned int br);
void reset_vfd();
void printRotateCW(const char* txt);
int16_t printRotateLine(const char* txt, const int16_t x, int16_t y);

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

int vfd_write(const char &data);

void reset_vfd() {
  gpioWrite(VFD_RST,0);
  gpioWrite(VFD_CLK,1);
  usleep(2000);
  gpioWrite(VFD_RST,1);
  // set 16 digits
  usleep(100);
  vfd_write(0xC0);
  // Bightness 100%
  vfd_write(0xFF);
  // set pointer to left
  vfd_write(0xAF);
}

int vfd_write(const char &data) {
  char mask = 0x80;

  // 8bit MSB first on fallowing edge
  while (mask) {

    if (data & mask )
      gpioWrite(VFD_DTA,1);
    else
      gpioWrite(VFD_DTA,0);
    usleep(1);
    gpioWrite(VFD_CLK,0);
    usleep(1);
    gpioWrite(VFD_CLK,1);
    mask >>= 1;
  }
  //   usleep(80);
  return 0;
}

void write_char(const char data) {
  switch (data) {
    case '\n':
    case '\r':
      vfd_write(0xAF);
      break;
    default:
      vfd_write((char)toupper((unsigned char)data) & 0x3F);
  }
}


void swirl_char(const int x,const char c)
{
      for (char j = ' '; j <= c; j++) {
        set_position(x);
        write_char(j);
        usleep(5000); // 5ms
      }
}

int write_string(const char *txt) {
  int i=0,len=0;
  for ( char zn;( zn = txt[i]) != '\0';){
    (flags & SWIRL_EFFECT) ? swirl_char(i,zn) : write_char(zn);
    i++;
    len += (zn != '.' && zn != ',') ? 1 : 0;
  }
  return len;
}

void set_position(unsigned char pos)
{
  vfd_write(0xA0 | (( pos-1) & 0xF) );
}




int16_t printRotateLine(const char* txt, const int16_t x, int16_t y)
{
  int16_t txtW = strlen(txt);
  int16_t DISPLAY_WIDTH = 16;
  int16_t ROTATE_WIDTH = DISPLAY_WIDTH + txtW;
  int txt_pos = 0;
  /// ----- CC
  while( txt_pos < ROTATE_WIDTH)
  {
    int display_pos = (DISPLAY_WIDTH < txt_pos)? 0 : DISPLAY_WIDTH-txt_pos;
    int char_pos=(DISPLAY_WIDTH < txt_pos) ? txt_pos - DISPLAY_WIDTH : 0;
    set_position(display_pos);
    while(display_pos < DISPLAY_WIDTH )
    {
      char zn=txt[char_pos];
      if ( zn == '\0')
      {
        write_char(' ');
        break;
      }
      write_char((zn == '\n') ? ' ': zn);
      if( ( zn !='.' ) && (zn != ',') )
        display_pos++;
      char_pos++;
    }
    txt_pos++;
    usleep(delay);
  }
  set_position(0);
  write_char(' ');
  return 0;
}
void printRotateCW(const char* txt)
{
  /// ----- CW
  char subStr[256] = "\0";
  int16_t txtW = strlen(txt);
  int16_t DISPLAY_WIDTH = 16;
  for ( char i=txtW-1; i>0;i--)
  {
    set_position(0);
    strncpy(subStr,txt+i,txtW - i);
    write_string(subStr);
    usleep(delay);
  }
  for (char i=0;i<DISPLAY_WIDTH;i++)
  {
    set_position(i);
    write_char(' ');
    strncpy(subStr,txt,DISPLAY_WIDTH- i- 1);
    subStr[DISPLAY_WIDTH-i-1]='\0';
    write_string(subStr);
    usleep(delay);
  }
}

void set_brightnes(unsigned int br)
{
  vfd_write(0xE0 | (0x1F & br));
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
