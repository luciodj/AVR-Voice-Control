/*
 * File:   neopixel.h
 * Author: m91329
 *
 * Created on February 14, 2019, 2:34 PM
 */

#ifndef NEOPIXEL_H
#define	NEOPIXEL_H

void NEO_init(void);
void NEO_display(void);
void NEO_fill(uint8_t r, uint8_t g, uint8_t b);
void NEO_migrate(uint8_t r, uint8_t g, uint8_t b);

#endif	/* NEOPIXEL_H */

