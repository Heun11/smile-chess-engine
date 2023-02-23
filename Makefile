LINUX_CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc

SRCDIR = ./source
INCDIR = ./include
LIBDIR = ./library
OSDIR = ./os_sdl
WIN_OBJDIR = ./objects/win
LINUX_OBJDIR = ./objects/linux

NAME = main

CFILES := $(wildcard $(SRCDIR)/*.c)
WIN_OFILES = $(patsubst $(SRCDIR)/%.c, $(WIN_OBJDIR)/%.o, $(CFILES))
LINUX_OFILES = $(patsubst $(SRCDIR)/%.c, $(LINUX_OBJDIR)/%.o, $(CFILES))

OFLAGS = -O0
CFLAGS = -Wall -g -I$(INCDIR) $(OFLAGS) 

LINUX_SDLFLAGS = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm 
WIN_LINK_SDLFLAGS = -mwindows -L$(LIBDIR)/SDL2/lib -L$(LIBDIR)/SDL2_Image/lib -L$(LIBDIR)/SDL2_Mixer/lib -L$(LIBDIR)/SDL2_TTF/lib
WIN_LINK_AFTER_SDLFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
WIN_INCL_SDLFLAGS = -I$(LIBDIR)/SDL2/include/SDL2 -I$(LIBDIR)/SDL2_Image/include/SDL2 -I$(LIBDIR)/SDL2_Mixer/include/SDL2 -I$(LIBDIR)/SDL2_TTF/include/SDL2 -I$(OSDIR)/win

start:
	@echo "help:"
	@echo "make win -> compile for windows"
	@echo "make linux -> compile for linux"
	@echo "make git -> push to git repo"

linux: $(NAME)

$(NAME): $(LINUX_OFILES)
	$(LINUX_CC) -o $(NAME) $^ $(LINUX_SDLFLAGS)

$(LINUX_OBJDIR)/%.o:$(SRCDIR)/%.c
	$(LINUX_CC) $(CFLAGS) -c -o $@ $< -I$(OSDIR)/linux

win: $(NAME).exe

$(NAME).exe: $(WIN_OFILES)
	$(WIN_CC) -o $(NAME).exe $(WIN_LINK_SDLFLAGS) $^ $(WIN_LINK_AFTER_SDLFLAGS)

$(WIN_OBJDIR)/%.o:$(SRCDIR)/%.c
	$(WIN_CC) $(CFLAGS) -o $@ -c $(WIN_INCL_SDLFLAGS) $<

clean:
	rm $(WIN_OBJDIR)/*.o $(LINUX_OBJDIR)/*.o

git:
	git add . -- ':!$(LIBDIR)' 
	git commit 
	git push
