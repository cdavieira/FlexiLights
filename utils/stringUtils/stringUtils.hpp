#pragma once

uint8_t getLength(const char* const name);
bool compareNames(const char* const name1, const char* const name2);
/* Essa funçao é MUITO mais rápida que a outra versão que usa o tipo String */
/* A versão que usava tipo String foi apagada porque era muito muito mais
 * lenta */
void ChoosePalleteByName(const char* const name_ptr);

