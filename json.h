/*
 * File:   json.h
 * Author: Lucio Di Jasio
 *
 * Created on February 14, 2019, 2:42 PM
 */

#ifndef JSON_H
#define	JSON_H

uint8_t * JSON_findQuote(uint8_t *p);
uint8_t * JSON_getValue(uint8_t *json, char* key);
bool JSON_getInt(uint8_t *p, char * key, uint32_t *value);
bool JSON_getString(uint8_t *p, char * key, uint8_t** value, uint8_t *len);
bool JSON_getBool(uint8_t *p, char * key, bool *value);

#endif	/* JSON_H */

