#include <stdio.h>

static FILE uartout;

static int uart_putchar (char c, FILE *stream) {
    if (Serial.write(c) > 0) {
      return 0;
    } else {
      return -1;
    }
}

void setup(void) {
  Serial.begin(9600);
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;

  int i = 10;
  char s[] = "abcd";
  printf("i = %d, s = %s\n", i, s);
}

void loop(void) {
}
