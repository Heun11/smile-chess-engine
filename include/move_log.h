#ifndef MOVE_LOG_H
#define MOVE_LOG_H

typedef struct{
    int log_len;
    char* log_str;
    int moves_c;
}MOVE_LOG_Log;

#include"main.h"

void MOVE_LOG_LogInit(MOVE_LOG_Log* log);
void MOVE_LOG_AddMove(MOVE_LOG_Log* log, char* move);
char* MOVE_LOG_IdentifyMove(int previous_position, int current_position, int previous_piece);

#endif