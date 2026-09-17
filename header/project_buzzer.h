//buzzer.h

#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER 19       // P0.19 connected to buzzer

/* Initialize buzzer GPIO pin. */
void Buzzer_Init(void);

/* Turn buzzer ON. */
void Buzzer_On(void);

/* Turn buzzer OFF. */
void Buzzer_Off(void);

/* Generate a short buzzer beep. */
void Buzzer_Beep(void);

/* Generate multiple buzzer beeps. */
void Buzzer_Alert(void);

#endif

