#include"move_log.h"

void MOVE_LOG_LogInit(MOVE_LOG_Log* log)
{
    log->log_len = 1;
    log->log_str = malloc(sizeof(char)*log->log_len);
    log->log_str[0] = '\0';
    log->moves_c = 0;
}

void MOVE_LOG_AddMove(MOVE_LOG_Log* log, char* move)
{
    log->log_len += strlen(move);
    log->log_str = realloc(log->log_str, log->log_len);
    strcat(log->log_str, move);
    log->moves_c++;
}

char MOVE_LOG_GetPieceFromIndex(int piece)
{
    if(piece==2){
        return 'R';
    }
    if(piece==-2){
        return 'r';
    }
    if(piece==3){
        return 'N';
    }
    if(piece==-3){
        return 'n';
    }
    if(piece==4){
        return 'B';
    }
    if(piece==-4){
        return 'b';
    }
    if(piece==5){
        return 'Q';
    }
    if(piece==-5){
        return 'q';
    }
    if(piece==6){
        return 'K';
    }
    if(piece==-6){
        return 'k';
    }
    return ' ';
}

int MOVE_LOG_MoreInSameLine(int piece, int y)
{
    int count = 0;
    for(int i=0;i<8;i++){
        if(board[y*8+i]==piece){
            count++;
        }
    }
    return count;
}

char* MOVE_LOG_IdentifyMove(int previous_position, int current_position, int previous_piece)
{
    int capture = (previous_piece==0)?0:1;
    int check, promote;

    /*
        riadky:
        0 -> 1
        1 -> 2
        2 -> 3
        3 -> 4
        4 -> 5
        5 -> 6
        7 -> 8

        stlpce:
        0 -> a
        1 -> b
        2 -> c
        3 -> d
        4 -> e
        5 -> f
        6 -> g
        7 -> h
    */

    int piece = board[current_position];
    char _piece = MOVE_LOG_GetPieceFromIndex(piece);
    char p_x = 97+(previous_position%8);

    char c_x = 97+(current_position%8);
    char c_y = 47+(8-(current_position/8))+1;

    // Nexd6+\0
    char* move = malloc(sizeof(char)*7);
    int i = 0;

    if(_piece!=' '){
        move[i] = _piece;
        i++;
    }

    // add capture and previous column
    if(capture){
        move[i] = p_x;
        i++;
        move[i] = 'x';
        i++;
    }
    else if(MOVE_LOG_MoreInSameLine(piece, current_position/8)>0 && _piece!=' '){
        move[i] = p_x;
        i++;
    }

    // add where moved
    move[i] = c_x;
    i++;
    move[i] = c_y;
    i++;

    // ending of string
    move[i] = ' ';
    i++;
    move[i] = '\0';

    return move;
}