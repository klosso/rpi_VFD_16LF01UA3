#include <pigpio.h>
#include "VFD_16LF01UA3.hpp"


char flags = 0;
unsigned int delay=100000;

int vfd_write(const char &data);

void reset_vfd() {
  gpioWrite(VFD_RST,0);
  gpioWrite(VFD_CLK,1);
  usleep(2000);
  gpioWrite(VFD_RST,1);
  // set 16 digits
  usleep(100);
  vfd_write(0xC0);
  // Brightness 100%
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
