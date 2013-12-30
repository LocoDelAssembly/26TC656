#include <IRremote.h>
#define PULSE 500
#define ZERO 1500
#define ONE  3500

/* Códigos de transmisión */
#define CH_UP   040
#define CH_DOWN 020
#define VO_UP   060
#define VO_DOWN 010
#define BR_UP   050
#define BR_DOWN 030
#define CS_UP   070
#define CS_DOWN 004
#define MUTE    044
#define VO_1_3  024
#define BR_1_2  064
#define CS_1_2  014
#define CALL    054
#define POWER   034
#define CH_1    002
#define CH_2    042
#define CH_3    022
#define CH_4    062
#define CH_5    012
#define CH_6    052
#define CH_7    032
#define CH_8    072
#define CH_9    006
#define CH_10   046
#define CH_11   026
#define CH_12   066
#define CH_13   016
#define CH_14   056
#define CH_15   036
#define CH_16   076

IRsend irsend;

void send_cmd(int cmd, int repeat)
{
  unsigned int buff[13];
  int gap = 48000 - PULSE;
  
  for (int i = 5; i >= 0; i--) {
    buff[i*2] = PULSE;
    gap -= (buff[i*2 + 1] = (cmd & 1 ? ONE : ZERO)) + PULSE;
    cmd >>= 1;
  }
  buff[12] = PULSE;

  for (; repeat >= 0; repeat--) {
    for (int i = 3; i; i--) {
      irsend.sendRaw(buff, 13, 38);
      delay((gap + 999)/1000);
    }
  } 
}

typedef struct {
  char *name;
  int code;
} Button;

/* No sé todos los botones que tenía el control remoto del 26TC656, pero estoy seguro que sobran códigos de transmisión.
   Solo pongo los que son importantes tener. */
Button buttons[] = {
  {"ENCEND", POWER},
  {"CANAL+", CH_UP},
  {"CANAL-", CH_DOWN},
  {"VOLUMEN+", VO_UP},
  {"VOLUMEN-", VO_DOWN},
  {"MUTE", MUTE},
  {"2", CH_1},
  {"3", CH_2},
  {"4", CH_3},
  {"5", CH_4},
  {"6", CH_5},
  {"7", CH_6},
  {"8", CH_7},
  {"8", CH_8},
  {"9", CH_9},
  {"10", CH_10},
  {"11", CH_11},
  {"12", CH_12}
};

void setup() {
}

void loop() {
  char buff[256];
  int len;
  
  len = Serial.readBytesUntil('\n', buff, 256);
  buff[len > 0 ? len - 1 : 0] = 0;
  
  for (int i = 0; i < sizeof buttons / sizeof *button; i++) {
    if (strcmp(buttons[i].name, buff) == 0) send_cmd(buttons[i].code, 0);
  }
}
