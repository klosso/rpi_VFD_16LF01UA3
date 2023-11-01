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

#define SWIRL_EFFECT 0x1

//using namespace std::chrono_literals;


char flags = 0;
unsigned int delay=100000;
void write_string(const char *txt);
void set_position(unsigned char pos);
void set_brightnes(unsigned int br);
void reset_vfd();
void printRotateCC(const char* txt);
int16_t printRotateLine(const char* txt, const int16_t x, int16_t y);

int main(int argc, char *argv[]) {
  int c;
	   if (gpioInitialise() < 0) return 1;
	
	gpioSetMode(VFD_RST,PI_OUTPUT);
	gpioSetMode(VFD_CLK,PI_OUTPUT);
	gpioSetMode(VFD_DTA,PI_OUTPUT);

  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        {"delay", required_argument, 0, 'd'},
        {"append", no_argument, 0, 0},
        {"delete", required_argument, 0, 0},
        {"verbose", no_argument, 0, 0},
        {"clear", required_argument, 0, 'c'},
        {"rotatecw", required_argument, 0, 'w'},
        {0, 0, 0, 0}};

    c = getopt_long(argc, argv, "is:mcr:w:b:d:", long_options, &option_index);
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
        printRotateCC(optarg);
      break;

    case '?':
      break;

    default:
      printf("?? getopt returned character code 0%o ??\n", c);
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

int count_comas(const char* txt)
{
  int len =strlen(txt);
  int count=0;
  for (int i=0 ; i < len;i++)
    if ((*(txt+i) == ',' ) || (*(txt+i) == '.'))
      count++;
  return count;
}
void write_string(const char *txt) {
  char len = strlen(txt);
  char comas = count_comas(txt);
  len = (len > (16 +comas)) ? 16 : len;

  for (char i = 0; i < len; i++) {
    if (flags & SWIRL_EFFECT) {
      for (char j = ' '; j <= *(txt+i); j++) {
        //vfd_write(0xA0 | (0xF & (i-1)) );
        set_position(i);
        write_char(j);
        usleep(5000); // 5ms
      }
    } else
      write_char(*(txt+i));
  }
}

void set_position(unsigned char pos)
{
  vfd_write(0xA0 | (( pos-1) & 0xF) );
}




int16_t printRotateLine(const char* txt, const int16_t x, int16_t y)
{
  char subStr[256]="\0";
  int16_t txtW = strlen(txt);
  int16_t DISPLAY_WIDTH = 16;


 //---CW rotate
for (char i=0;i<=txtW+DISPLAY_WIDTH;i++)
{
  if(i<DISPLAY_WIDTH){
    set_position(DISPLAY_WIDTH-i);
    strncpy(subStr,txt, i);
  }else
  {
    set_position(0);
    strncpy(subStr,txt + i - DISPLAY_WIDTH, DISPLAY_WIDTH);
  }
  write_string(subStr);
   if((txtW- i)<0)
     write_char(' ');
  usleep( delay );
}
  return 0;
}
/// ----- CC
void printRotateCC(const char* txt)
{
  char subStr[256]="\0";
  int16_t txtW = strlen(txt);
  int16_t DISPLAY_WIDTH = 16;
/// ----- CC
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
/*  impl on one loop , ... but not working well
 for ( char i=0 ; i<txtW+DISPLAY_WIDTH; i++)
 {
  if(i>=DISPLAY_WIDTH){
      set_position(0);
      strncpy(subStr,txt+txtW-i+DISPLAY_WIDTH,i-DISPLAY_WIDTH);
      write_string(subStr);
      usleep(90000);
 }else
 {
   set_position(i);
   strncpy(subStr,txt,DISPLAY_WIDTH- i- 1);
   write_char(' ');
   subStr[DISPLAY_WIDTH-i-1]='\0';
 }
   write_string(subStr);
   usleep(90000);
 }
*/
}



void set_brightnes(unsigned int br)
{
  vfd_write(0xE0 | (0x1F & br));
}
