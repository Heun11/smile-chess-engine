#include"chess.h"

SDL_Rect CHESS_WhitePiece[7];
SDL_Rect CHESS_BlackPiece[7];
SDL_Rect CHESS_MoveIcon;
int board[64];
int selected_piece = -1;
char starting_fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
int on_turn = 1;
int turn_count = 0;
int is_check = 0;
int promotion_piece = 5;
int win = 0;
int w_can_castle_k = 0;
int w_can_castle_q = 0;
int b_can_castle_k = 0;
int b_can_castle_q = 0;

int CHESS_GetPieceFromFenSymbol(char symbol)
{
    char _symbol = tolower(symbol);
    int c = islower(symbol)?-1:1;
    if(_symbol=='p'){
        return CHESS_Pawn*c;
    }
    if(_symbol=='r'){
        return CHESS_Rook*c;
    }
    if(_symbol=='n'){
        return CHESS_Knight*c;
    }
    if(_symbol=='b'){
        return CHESS_Bishop*c;
    }
    if(_symbol=='q'){
        return CHESS_Queen*c;
    }
    if(_symbol=='k'){
        return CHESS_King*c;
    }
    return 0;
}

void CHESS_LoadPositionFromFen(char* fen)
{
    int file = 0, rank = 0;
    char symbol;
    int setup_info = 0;

    // load fen pieces
    for(int i=0;i<strlen(fen);i++){
        symbol = fen[i];
        if(symbol==' '){
            setup_info = i;
            break;
        }
        if(symbol=='/'){
            file = 0;
            rank++;
        }
        else{
            if(isdigit(symbol)!=0){
                file+=symbol-'0';
            }
            else{
                board[rank*8+file] = CHESS_GetPieceFromFenSymbol(symbol);
                file++;
            }
        }
    }

    // set fen info
    for(int i=setup_info;i<strlen(fen);i++){
        symbol = fen[i];
        if(symbol=='w'){
            on_turn = 1;
        }
        if(symbol=='b'){
            on_turn = -1;
        }
        if(symbol=='K'){
            w_can_castle_k = 1;
        }
        if(symbol=='Q'){
            w_can_castle_q = 1;
        }
        if(symbol=='k'){
            b_can_castle_k = 1;
        }
        if(symbol=='q'){
            b_can_castle_q = 1;
        }
    }
}

void CHESS_LoadPieces()
{
    for(int i=0;i<6;i++){
        CHESS_WhitePiece[i+1] = TOOLS_Get_Image_Src(i, 0, 16, 16);
        CHESS_BlackPiece[i+1] = TOOLS_Get_Image_Src(i, 1, 16, 16);
    }
    CHESS_MoveIcon = TOOLS_Get_Image_Src(6, 0, 16, 16);
	for(int i=0;i<64;i++){
		board[i] = 0;
	}
    CHESS_LoadPositionFromFen(starting_fen);
}

void CHESS_DrawBoardAndPieces()
{
    for(int i=0;i<64;i++){
        // drawing board
        if((((i+(i/8))%2))==0){
            SDL_SetRenderDrawColor(rend, 247, 243, 220, 255);
        }else{
            SDL_SetRenderDrawColor(rend, 51, 107, 163, 255);
        }
        SDL_RenderFillRect(rend, &(SDL_Rect){(i%8)*75,(i/8)*75,75,75});
    
        // drawing pieces
        if(i==selected_piece){
            SDL_SetRenderDrawColor(rend, 66, 176, 44, 100);
            SDL_RenderFillRect(rend, &(SDL_Rect){(i%8)*75,(i/8)*75,75,75});
        }
        if(board[i]>0){
            TOOLS_Render_Image_From_Texture(rend, tex, &CHESS_WhitePiece[board[i]], (i%8)*75, (i/8)*75, 75, 75);
        }
        else if(board[i]<0){
            TOOLS_Render_Image_From_Texture(rend, tex, &CHESS_BlackPiece[abs(board[i])], (i%8)*75, (i/8)*75, 75, 75);
        }
    }

    // drawing promotion set menu
    for(int i=0;i<4;i++){
        TOOLS_Render_Image_From_Texture(rend, tex, &CHESS_WhitePiece[i+2], SCREEN_WIDTH-75, i*75, 75, 75);
            SDL_SetRenderDrawColor(rend, 232, 74, 203, 50);
            SDL_RenderFillRect(rend, &(SDL_Rect){SCREEN_WIDTH-75,(promotion_piece-2)*75,75,75});
    }
}

int CHESS_FindKing(int c)
{
    for(int i=0;i<64;i++){
        if(board[i]==(6*c)){
            return i;
        }
    }
    return -1;
}

int CHESS_IsCheck(int i, int c)
{
    int x = i%8;
    int y = i/8;
    int r=1, l=1, u=1, d=1;
    // printf("king pos: %d\n",i);

    // horizontal and vertical check
    for(int i=1;i<8;i++){
        if((y+i)<8 && board[(y+i)*8+(x)]!=0 && d){
            if(board[(y+i)*8+(x)]==(-1*c)*2 || board[(y+i)*8+(x)]==(-1*c)*5){
                // printf("%d\n", (y+i)*8+(x));
                return c;
            }
            else{
                d = 0;
            }
        }
        if((y-i)>=0 && board[(y-i)*8+(x)]!=0 && u){
            if(board[(y-i)*8+(x)]==(-1*c)*2 || board[(y-i)*8+(x)]==(-1*c)*5){
                // printf("%d\n", (y-i)*8+(x));
                return c;
            }
            else{
                u = 0;
            }
        }
        if((x+i)<8 && board[(y)*8+(x+i)]!=0 && r){
            if(board[(y)*8+(x+i)]==(-1*c)*2 || board[(y)*8+(x+i)]==(-1*c)*5){
                // printf("%d\n", (y)*8+(x+i));
                return c;
            }
            else{
                r = 0;
            }
        }
        if((x-i)>=0 && board[(y)*8+(x-i)]!=0 && l){
            if(board[(y)*8+(x-i)]==(-1*c)*2 || board[(y)*8+(x-i)]==(-1*c)*5){
                // printf("%d\n", (y)*8+(x-i));
                return c;
            }
            else{
                l = 0;
            }
        }
    }
    
    // diagonal check
    r=1, l=1, u=1, d=1;
    for(int i=1;i<8;i++){
        if((y+i)<8 && (x+i)<8 && board[(y+i)*8+(x+i)]!=0 && d){
            if(board[(y+i)*8+(x+i)]==(-1*c)*4 || board[(y+i)*8+(x+i)]==(-1*c)*5){
                // printf("%d\n", (y+i)*8+(x+i));
                return c;
            }
            else{
                d = 0;
            }
        }
        if((y-i)>=0 && (x-i)>=0 && board[(y-i)*8+(x-i)]!=0 && u){
            if(board[(y-i)*8+(x-i)]==(-1*c)*4 || board[(y-i)*8+(x-i)]==(-1*c)*5){
                // printf("%d\n", (y-i)*8+(x-i));
                return c;
            }
            else{
                u = 0;
            }
        }
        if((y+i)<=7 && (x-i)>=0 && board[(y+i)*8+(x-i)]!=0 && r){
            if(board[(y+i)*8+(x-i)]==(-1*c)*4 || board[(y+i)*8+(x-i)]==(-1*c)*5){
                // printf("%d\n", (y+i)*8+(x-i));
                return c;
            }
            else{
                r = 0;
            }
        }
        if((y-i)>=0 && (x+i)<=7 && board[(y-i)*8+(x+i)]!=0 && l){
            if(board[(y-i)*8+(x+i)]==(-1*c)*4 || board[(y-i)*8+(x+i)]==(-1*c)*5){
                // printf("%d\n", (y-i)*8+(x+i));
                return c;
            }
            else{
                l = 0;
            }
        }
    }

    // knight check
    int r_c[8] = {-2, -2, -1, -1, +1, +1, +2, +2};
    int f_c[8] = {-1, +1, -2, +2, -2, +2, +1, -1};
    for(int i=0;i<8;i++){
        if(board[(y+r_c[i])*8+(x+f_c[i])]==(-1*c)*3){
            // printf("%d\n", (y+r_c[i])*8+(x+f_c[i]));
            return c;
        }
    }

    // pawn check
    if(board[(y-c)*8+(x+1)]==(-1*c)){
        // printf("%d\n", (y-c)*8+(x+1));
        return c;
    }
    if(board[(y-c)*8+(x-1)]==(-1*c)){
        // printf("%d\n", (y-c)*8+(x-1));
        return c;
    }

    // king check
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i==0 && j==0){
                continue;
            }
            else{
                if(board[(y+i)*8+(x+j)]==6*(-1*c) && (y+i)<8 && (y+i)>=0 && (x+j)>=0 && (x+j)<8){
                    return c;
                }
            }
        }
    }


    return 0;
}

CHESS_Moves CHESS_GenerateMovesForPiece(int i)
{
    CHESS_Moves possible_moves;
    possible_moves.moves = malloc(sizeof(CHESS_Move)*30);
    possible_moves.len = 0;
    int piece = board[i];
    int c = piece/abs(piece);
    int x = i%8;
    int y = i/8;
    int king_pos = CHESS_FindKing(c);
    int previous_piece = 0;
    int r=1, l=1, u=1, d=1;

    // pawn movement
    if(abs(piece)==1){
        if(board[(y-1*c)*8+(x)]==0){
            previous_piece = board[(y-1*c)*8+(x)];
            board[(y-1*c)*8+(x)] = piece;
            board[y*8+x] = 0;
            if(CHESS_IsCheck(king_pos, c)!=c){
                possible_moves.len++;
                possible_moves.moves[possible_moves.len-1].x = x;
                possible_moves.moves[possible_moves.len-1].y = (y-1*c);
            }
            board[(y-1*c)*8+(x)] = previous_piece;
            board[y*8+x] = piece;
            if((float)(y)-(2.5f*(float)c)==3.5f && board[(y-2*c)*8+(x)]==0){
                previous_piece = board[(y-2*c)*8+(x)];
                board[(y-2*c)*8+(x)] = piece;
                board[y*8+x] = 0;
                if(CHESS_IsCheck(king_pos, c)!=c){
                    possible_moves.len++;
                    possible_moves.moves[possible_moves.len-1].x = x;
                    possible_moves.moves[possible_moves.len-1].y = (y-2*c);
                }
                board[(y-2*c)*8+(x)] = previous_piece;
                board[y*8+x] = piece;
            }
        }
        if(((board[(y-1*c)*8+(x+1)]>0 && c<0) || (board[(y-1*c)*8+(x+1)]<0 && c>0)) && x+1<=7){
            previous_piece = board[(y-1*c)*8+(x+1)];
            board[(y-1*c)*8+(x+1)] = piece;
            board[y*8+x] = 0;
            if(CHESS_IsCheck(king_pos, c)!=c){
                possible_moves.len++;
                possible_moves.moves[possible_moves.len-1].x = x+1;
                possible_moves.moves[possible_moves.len-1].y = (y-1*c);
            }
            board[(y-1*c)*8+(x+1)] = previous_piece;
            board[y*8+x] = piece;
        }
        if(((board[(y-1*c)*8+(x-1)]>0 && c<0) || (board[(y-1*c)*8+(x-1)]<0 && c>0)) && x-1>=0){
            previous_piece = board[(y-1*c)*8+(x-1)];
            board[(y-1*c)*8+(x-1)] = piece;
            board[y*8+x] = 0;
            if(CHESS_IsCheck(king_pos, c)!=c){
                possible_moves.len++;
                possible_moves.moves[possible_moves.len-1].x = x-1;
                possible_moves.moves[possible_moves.len-1].y = (y-1*c);
            }
            board[(y-1*c)*8+(x-1)] = previous_piece;
            board[y*8+x] = piece;
        }
    }

    // rook movement and queen
    if(abs(piece)==2 || abs(piece)==5){
        r=1, l=1, u=1, d=1;
        for(int i=1;i<8;i++){
            if((y+i)<8 && d){
                if(board[(y+i)*8+(x)]==0 || (board[(y+i)*8+(x)]<0 && c>0) || (board[(y+i)*8+(x)]>0 && c<0)){
                    previous_piece = board[(y+i)*8+(x)];
                    board[(y+i)*8+(x)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x;
                        possible_moves.moves[possible_moves.len-1].y = y+i;
                    }
                    board[(y+i)*8+(x)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y+i)*8+(x)]<0 && c>0) || (board[(y+i)*8+(x)]>0 && c<0)){
                        d = 0;
                    }
                }
                else{
                    d = 0;
                }
            }
            if((y-i)>=0 && u){
                if(board[(y-i)*8+(x)]==0 || (board[(y-i)*8+(x)]<0 && c>0) || (board[(y-i)*8+(x)]>0 && c<0)){
                    previous_piece = board[(y-i)*8+(x)];
                    board[(y-i)*8+(x)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x;
                        possible_moves.moves[possible_moves.len-1].y = y-i;
                    }
                    board[(y-i)*8+(x)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y-i)*8+(x)]<0 && c>0) || (board[(y-i)*8+(x)]>0 && c<0)){
                        u = 0;
                    }
                }
                else{
                    u = 0;
                }
            }
            if((x+i)<8 && r){
                if(board[(y)*8+(x+i)]==0 || (board[(y)*8+(x+i)]<0 && c>0) || (board[(y)*8+(x+i)]>0 && c<0)){
                    previous_piece = board[(y)*8+(x+i)];
                    board[(y)*8+(x+i)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x+i;
                        possible_moves.moves[possible_moves.len-1].y = y;
                    }
                    board[(y)*8+(x+i)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y)*8+(x+i)]<0 && c>0) || (board[(y)*8+(x+i)]>0 && c<0)){
                        r = 0;
                    }
                }
                else{
                    r = 0;
                }
            }
            if((x-i)>=0 && l){
                if(board[(y)*8+(x-i)]==0 || (board[(y)*8+(x-i)]<0 && c>0) || (board[(y)*8+(x-i)]>0 && c<0)){
                    previous_piece = board[(y)*8+(x-i)];
                    board[(y)*8+(x-i)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x-i;
                        possible_moves.moves[possible_moves.len-1].y = y;
                    }
                    board[(y)*8+(x-i)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y)*8+(x-i)]<0 && c>0) || (board[(y)*8+(x-i)]>0 && c<0)){
                        l = 0;
                    }
                }
                else{
                    l = 0;
                }
            }
        }
    }

    // knight movement
    if(abs(piece)==3){
        int r_c[8] = {-2, -2, -1, -1, +1, +1, +2, +2};
        int f_c[8] = {-1, +1, -2, +2, -2, +2, +1, -1};

        for(int i=0;i<8;i++){
            if(((board[(y+r_c[i])*8+(x+f_c[i])])==0 || (((board[(y+r_c[i])*8+(x+f_c[i])])<0 && c>0) || ((board[(y+r_c[i])*8+(x+f_c[i])])>0 && c<0))) 
            && (x+f_c[i]>=0) && (x+f_c[i]<=7) && (y+r_c[i]>=0) && (y+r_c[i]<=7)){
                previous_piece = board[(y+r_c[i])*8+(x+f_c[i])];
                board[(y+r_c[i])*8+(x+f_c[i])] = piece;
                board[y*8+x] = 0;
                if(CHESS_IsCheck(king_pos, c)!=c){
                    possible_moves.len++;
                    possible_moves.moves[possible_moves.len-1].x = x+f_c[i];
                    possible_moves.moves[possible_moves.len-1].y = y+r_c[i];
                }
                board[(y+r_c[i])*8+(x+f_c[i])] = previous_piece;
                board[y*8+x] = piece;
            }
        }
    }

    // bishop movement and queen
    if(abs(piece)==4 || abs(piece)==5){
        r=1, l=1, u=1, d=1;
        for(int i=1;i<8;i++){
            if((y+i)<8 && (x+i)<8 && d){
                if(board[(y+i)*8+(x+i)]==0 || (board[(y+i)*8+(x+i)]<0 && c>0) || (board[(y+i)*8+(x+i)]>0 && c<0)){
                    previous_piece = board[(y+i)*8+(x+i)];
                    board[(y+i)*8+(x+i)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x+i;
                        possible_moves.moves[possible_moves.len-1].y = y+i;
                    }
                    board[(y+i)*8+(x+i)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y+i)*8+(x+i)]<0 && c>0) || (board[(y+i)*8+(x+i)]>0 && c<0)){
                        d = 0;
                    }
                }
                else{
                    d = 0;
                }
            }
            if((y-i)>=0 && (x-i)>=0 && u){
                if(board[(y-i)*8+(x-i)]==0 || (board[(y-i)*8+(x-i)]<0 && c>0) || (board[(y-i)*8+(x-i)]>0 && c<0)){
                    previous_piece = board[(y-i)*8+(x-i)];
                    board[(y-i)*8+(x-i)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x-i;
                        possible_moves.moves[possible_moves.len-1].y = y-i;
                    }
                    board[(y-i)*8+(x-i)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y-i)*8+(x-i)]<0 && c>0) || (board[(y-i)*8+(x-i)]>0 && c<0)){
                        u = 0;
                    }
                }
                else{
                    u = 0;
                }
            }
            if((y+i)<8 && (x-i)>=0 && r){
                if(board[(y+i)*8+(x-i)]==0 || (board[(y+i)*8+(x-i)]<0 && c>0) || (board[(y+i)*8+(x-i)]>0 && c<0)){
                    previous_piece = board[(y+i)*8+(x-i)];
                    board[(y+i)*8+(x-i)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x-i;
                        possible_moves.moves[possible_moves.len-1].y = y+i;
                    }
                    board[(y+i)*8+(x-i)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y+i)*8+(x-i)]<0 && c>0) || (board[(y+i)*8+(x-i)]>0 && c<0)){
                        r = 0;
                    }
                }
                else{
                    r = 0;
                }
            }
            if((y-i)>=0 && (x+i)<8 && l){
                if(board[(y-i)*8+(x+i)]==0 || (board[(y-i)*8+(x+i)]<0 && c>0) || (board[(y-i)*8+(x+i)]>0 && c<0)){
                    previous_piece = board[(y-i)*8+(x+i)];
                    board[(y-i)*8+(x+i)] = piece;
                    board[y*8+x] = 0;
                    if(CHESS_IsCheck(king_pos, c)!=c){
                        possible_moves.len++;
                        possible_moves.moves[possible_moves.len-1].x = x+i;
                        possible_moves.moves[possible_moves.len-1].y = y-i;
                    }
                    board[(y-i)*8+(x+i)] = previous_piece;
                    board[y*8+x] = piece;
                    if((board[(y-i)*8+(x+i)]<0 && c>0) || (board[(y-i)*8+(x+i)]>0 && c<0)){
                        l = 0;
                    }
                }
                else{
                    l = 0;
                }
            }
        }

    }

    // king movement
    if(abs(piece)==6){
        for(int i=-1;i<=1;i++){
            for(int j=-1;j<=1;j++){
                if(i==0 && j==0){
                    continue;
                }
                else{
                    if((board[(y+i)*8+(x+j)]==0 || (board[(y+i)*8+(x+j)]<0 && c>0) || (board[(y+i)*8+(x+j)]>0 && c<0))
                    && (y+i)<8 && (y+i)>=0 && (x+j)>=0 && (x+j)<8){
                        previous_piece = board[(y+i)*8+(x+j)];
                        board[(y+i)*8+(x+j)] = piece;
                        board[y*8+x] = 0;
                        if(CHESS_IsCheck((y+i)*8+(x+j), c)!=c){
                            possible_moves.len++;
                            possible_moves.moves[possible_moves.len-1].x = x+j;
                            possible_moves.moves[possible_moves.len-1].y = y+i;
                        }
                        board[(y+i)*8+(x+j)] = previous_piece;
                        board[y*8+x] = piece;
                    }
                }
            }
        }
    }

    return possible_moves;
}

void CHESS_MovePiece()
{
    if(win==0){
        int _mouse_x, _mouse_y, buttons;
        int mouse_x = -1;
        int mouse_y = -1;
        CHESS_Moves possible_moves = CHESS_GenerateMovesForPiece(selected_piece);
        buttons = SDL_GetMouseState(&_mouse_x, &_mouse_y);
        if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
            // select promotion piece
            if(_mouse_x>8*75){
                for(int i=0;i<4;i++){
                    if(_mouse_x>SCREEN_WIDTH-75 && _mouse_y>i*75 && _mouse_y<(i+1)*75){
                        promotion_piece = i+2;
                    }
                }
            }

            // move piece
            mouse_x = _mouse_x/75;
            mouse_y = _mouse_y/75;
            if(mouse_x>7){mouse_x=0; mouse_y=0;}
            if(selected_piece>=0){
                int piece = board[selected_piece];
                for(int i=0;i<possible_moves.len;i++){
                    if(mouse_x==possible_moves.moves[i].x && mouse_y==possible_moves.moves[i].y){
                        int previous_piece = board[possible_moves.moves[i].y*8+possible_moves.moves[i].x];
                        board[possible_moves.moves[i].y*8+possible_moves.moves[i].x] = piece;
                        board[selected_piece] = 0;

                        int c = piece/abs(piece);
                        if(abs(piece)==1 && ((c>0 && possible_moves.moves[i].y==0) || (c<0 && possible_moves.moves[i].y==7))){
                            board[possible_moves.moves[i].y*8+possible_moves.moves[i].x] = promotion_piece*c;
                        }

                        turn_count++;
                        if(on_turn>0){
                            on_turn = -1;
                        }
                        else{
                            on_turn = 1;
                        }

                        // log move
                        char* move = MOVE_LOG_IdentifyMove(selected_piece, possible_moves.moves[i].y*8+possible_moves.moves[i].x, previous_piece);
                        MOVE_LOG_AddMove(&MoveLog, move);
                        printf("%s\n", MoveLog.log_str);
                        free(move);
                    }
                }
            }
            if(board[mouse_y*8+mouse_x]!=0){
                selected_piece = mouse_y*8+mouse_x;
                int c = board[selected_piece]/abs(board[selected_piece]);
                if(c!=on_turn){
                    selected_piece = -1;
                }
            }
            else{
                selected_piece = -1;
            }
        }
        if(possible_moves.len>0){
            for(int i=0;i<possible_moves.len;i++){
                SDL_SetRenderDrawColor(rend, 224, 54, 54, 100);
                SDL_RenderFillRect(rend, &(SDL_Rect){possible_moves.moves[i].x*75,possible_moves.moves[i].y*75,75,75});
            }
        }
        free(possible_moves.moves);
    }
    else{
        char win_str[11];
        if(win>0){
            strncpy(win_str, "White Won!", 11);
        }
        else{
            strncpy(win_str, "Black Won!", 11);
        }
		TOOLS_SDL_Text_RenderCopy(rend, font, win_str, SCREEN_WIDTH-150, SCREEN_HEIGHT-180, 150, 80, (SDL_Color){52, 97, 235});
    }
}