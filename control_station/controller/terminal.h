/*
    terminal.h
    Simple terminal functionality by Arjan J.C. van Gemund
    + mods by Ioannis Protonotarios
*/

void term_initio();
void term_exitio();
void term_puts(char *s);
void term_putchar(char c);
int	term_getchar_nb();
int	term_getchar();

void rs232_open(void);
void rs232_close(void);
int	rs232_getchar_nb();
int rs232_getchar();
int rs232_putchar(char c);
