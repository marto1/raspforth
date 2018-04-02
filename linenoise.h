/* linenoise.h -- VERSION 1.0
 * See LICENSE for license stuff.
 *
 * See linenoise.c for more information.
 *
 */

#ifndef __LINENOISE_H
#define __LINENOISE_H

/* termios. Replace with appropriate constants. */

/* struct termios { */
/* 	tcflag_t c_iflag; */
/* 	tcflag_t c_oflag; */
/* 	tcflag_t c_cflag; */
/* 	tcflag_t c_lflag; */
/* 	cc_t c_line; */
/* 	cc_t c_cc[NCCS]; */
/* 	speed_t __c_ispeed; */
/* 	speed_t __c_ospeed; */
/* }; */

/* #define BRKINT  0000002 */
/* #define ICRNL   0000400 */
/* #define INPCK   0000020 */
/* #define ISTRIP  0000040 */
/* #define IXON    0002000 */
/* #define OPOST   0000001 */
/* #define CS8     0000060 */
/* #define ISIG    0000001 */
/* #define ICANON  0000002 */
/* #define ECHO    0000010 */
/* #define IEXTEN  0100000 */
/* #define VMIN          6 */
/* #define VTIME         5 */
/* #define TCSAFLUSH     2 */

/* end  termios */

#define LINENOISE_HISTORY_MAX_LEN 100
#define LINENOISE_MAX_LINE 512
#define LINENOISE_ABUF_MAX_LEN 768
#define LINENOISE_MAX_COLS 80 /* as God intended. */

char *linenoise(const char *prompt, char* input);
int linenoiseHistoryAdd(const char *line);
void linenoiseAtExit(void);

#endif /* __LINENOISE_H */
