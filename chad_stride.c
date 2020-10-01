#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <string.h> // strlen
#include <signal.h>
#include <stdbool.h>

#define CHAD_FRAMES 7
#define CHAD_ROWS   24
#define AVG_CHAD_WIDTH 25
#define DEFAULT_SIZE 1
#define MAX_MESSAGE_SIZE 50

bool RAINBOW = false, NORAINBOW = false, STAY = false, LOOP = false, NOFUCKS = false, COLOR = false, MES = false, REVERSE = false, FLIP = false;
long SLEEP_TIMER = 150000;
int SIZE = DEFAULT_SIZE, PAIR = 1;

const char* ARG_FLAGS = "slrfhRpb:a:c:m:";
char message[MAX_MESSAGE_SIZE+1];

/* Chad frames */
const char* chad_frames[CHAD_FRAMES][CHAD_ROWS] = {
    {
        "                          %",
        "                     .(....",
        "                  %......(",
        "              ,*........&",
        "           .#,.,,,,. .(",
        "         *.,.*,,,,&@/,%",
        "        /,,,,,,,,,,,,,,*",
        "        &,,((%.,,,,,,.#(*",
        "        &.,@.%,,,,,.(.*",
        "        .,..,.#*,,,,./.",
        "       /,,.@@/* ..,,,,/",
        "      @@@**@@@@@",
        "     @@@*@@...@@@",
        "   @@.@@***@@@......@@@",
        " @@...@@*********@@@...@@",
        " @@...@@@********@@ @@@",
        "@....@@@***@@@**@",
        " @@@@ @@******@@@",
        "     @******@@***@@",
        "     @...@@@  @@....@@*@@",
        " @@@@...@       @..@***@@",
        "@**@@@           @@***@",
        "@**@@              @@@",
        "@****@",
    },
    {
        "                         & /",
        "                     & ...%",
        "                 % ......%",
        "              (.........%",
        "           @.,,,,.%@@&",
        "         @,,,@.,,@(/@.&",
        "        /,,,..,,,,,,,,,.@",
        "        &,,*..*.,,,,.((.",
        "        &*, ,&,,,,,*@%@",
        "       ..,..,,./,,,,,.@",
        "       %,,,..*%@&#.,.*,",
        "      @@*****@@@",
        "     @********@@@",
        "     @*******@...@@",
        "   @@@********@@...@",
        "   @@.@@@*******@...@@",
        "     @@@*@@@@@**@.....@@",
        "     @********@@   @@@",
        "@@@  @@@***@@*@@@",
        "@@*@@.@@@@@@@@..@",
        "@@***@...@@@@...@",
        "@@*@@ @@@  @@.@@",
        "@@@      @@*****@",
        "         @@,,,,,,@@"
    },
    {
        "                       %...",
        "                   #...../",
        "               /........@",
        "            /,%######&%@",
        "          .,#,,,...(@%",
        "        %,,,,,,,,..,,,.",
        "        ,,,&./,,,,,,.#,,&",
        "        *,#*%%,,,,,,.#&",
        "       ...*../,,,,,.%@",
        "       %,,@,,,,,,,,,,,..",
        "     @@.,,,,,,,@ @&%",
        "  @*************@...@@",
        "  @@@@**********@@@.@@",
        "  @...@@******@@",
        "  @...@@******@@",
        "  @...@@******@@",
        "@@...@*****@@*@@",
        "@@...@**@****@@@",
        "  @@@@@@******@@",
        "   @@....@@@@@",
        "  @**@..@@@",
        "   @@***@",
        "   @@****@@",
        "     @*****@@"
    },
    {
        "                         (../",
        "                     /.....@",
        "                 %........(",
        "              //%#####%&/(",
        "           ,.,/,,,,..#%&",
        "          &,,,,.,,.,..,,,",
        "          .,.(,.,,,,,,.#.,#",
        "          .,@/*@,,,,,,,#(",
        "         #.,&,(,,,,,,,#@",
        "         .,,%,,,,,,,,,,,,",
        "      @@@,,,,,,,.@@ %,",
        "   @@.@@*********@@.@@",
        " @@.....@********@@.@@",
        "@.....@@@********@@...@",
        " @@...@@********@  @...@@",
        " @@.....@*******@  @...@@",
        "   @@....@@***@@",
        "     @@@@***@@@@",
        "      @@****@@@@",
        "     @..@@@@.....@@@***@@",
        "     @...@@@@@...@@***@",
        "   @@...@       @**@",
        " @@**@@@         @@",
        "   @@****@@"
    },
    {
        "                           /*%",
        "                       &....*",
        "                   (.......",
        "                %.........",
        "             /(.,,,,.../",
        "           ,,,@,,,.%@#@#",
        "          ,,,,,,,,,,,,,,.#",
        "          ,,(@(,.,,,,,%(/%",
        "          #,%#..,,,,,%.*&",
        "         %,,&,,*&,,,,,./",
        "        %.,,*@#/....,,,.#",
        "   @@.@@*@@@@@@@@",
        " @@.....@********@@",
        "@.....@@*********@@@",
        "@....@**********@...@@",
        " @@...@@********@@@.@@",
        " @@.....@@@***@@ @@.@@",
        "   @@@@@***@@@@@@...@@",
        "     @*****@**@@ @@@",
        "     @@@***@@@",
        "   @@.@@@@@.@@",
        " @@...@@ @@...@@",
        "@**@@@     @....@@@@",
        "@****@      @@*****@"
    },
    {
        "                       %..&",
        "                   %.....(",
        "               ,,........",
        "            #..**,/*, .",
        "         *.,,,,,%#/(,",
        "        *,,, .,,&@#.,,*",
        "       #,,.%#,,,,,,,,/,(",
        "       @,..%.&,,,,,,..",
        "       (@,%,#,,,,,/,.@",
        "      ,,,,.,,,.,,,,,,.#",
        "     @@,,,,,,.,  &,#*",
        "    @..@**@@@@@",
        "  @@...@*******@",
        "@@.....@*******@",
        "  @@....@@@****@",
        "     @@....@@@@@",
        "     @@@@@@****@",
        "     @@******@@.@@",
        "     @@***@@@@@.@@",
        "    @**@@@...@@@",
        "     @@@...@@",
        "       @...@@",
        "       @*****@@",
        "        @@@@@"
    },
    {
        "                     &..&",
        "                 (...../",
        "             /.........",
        "          (.*(((##/,.",
        "       ( ,,,,,&**(",
        "      ,,,, .,,#&/.,,(",
        "     #,, @&,,,,,,,,#,/",
        "     @,..&.%,,,,,,.*",
        "     /&,&,#,,,,,*..&",
        "    ,,,,.,,,.,,,,,,.%",
        "    &,,,,,,.,  &/&.",
        "  @@@*@@...@@*@@...@",
        "  @**@.....@@@.....@",
        "  @***@@........@@@",
        "@@.@@****@@@@@",
        "  @@@********@",
        "  @********@@",
        "  @**********@",
        "  @***@@***@@@",
        "  @...@@@....@",
        "  @...@@*@@@@",
        "@@...@  @****@",
        "@@@@@@  @****@",
        "@@******@"
    }
};

const char* chad_frames_flip[CHAD_FRAMES][CHAD_ROWS] = {
    {
        "%",
        "....).",
        " )......%",
        "  &........*,",
        "    ). .,,,,.,#.",
        "    %,\\@&,,,,*.,.*",
        "   *,,,,,,,,,,,,,,\\",
        "  *)#.,,,,,,.%)),,&",
        "    *.).,,,,,%.@,.&",
        "    .\\.,,,,*#.,..,.",
        "    \\,,,,.. *\\@@.,,\\",
        "           @@@@@**@@@",
        "          @@@...@@*@@@",
        "    @@@......@@@***@@.@@",
        "  @@...@@@*********@@...@@",
        "    @@@ @@********@@@...@@",
        "          @**@@@***@@@....@",
        "          @@@******@@ @@@@",
        "        @@***@@******@",
        "  @@*@@....@@  @@@...@",
        "  @@***@..@       @...@@@@",
        "    @**@@           @@@***@",
        "     @@@              @@**@",
        "                     @****@",
    },
    {
        "\\ &",
        " %... &",
        "  %...... %",
        "   %.........)",
        "      &@@%.,,,,.@",
        "     &.@\\)@,,.@,,,@",
        "   @.,,,,,,,,,..,,,\\",
        "    .)).,,,,.*..*,,&",
        "     @%@*,,,,,&, ,*&",
        "     @.,,,,,\\.,,.,..",
        "     ,*.,.#&@%*..,,,%",
        "            @@@*****@@",
        "           @@@********@",
        "         @@...@*******@",
        "        @...@@********@@@",
        "      @@...@*******@@@.@@",
        "    @@.....@**@@@@@*@@@",
        "      @@   @@********@",
        "           @@@*@@***@@@  @@@",
        "           @..@@@@@@@@.@@*@@",
        "           @...@@@@...@***@@",
        "            @@.@@  @@@ @@*@@",
        "           @*****@@      @@@",
        "         @@,,,,,,@@"
    },
    {
        "...%",
        " \\.....#",
        "  @........\\",
        "   @%&######%,\\",
        "     %@)...,,,#,.",
        "    .,,,..,,,,,,,,%",
        "  &,,#.,,,,,,\\.&,,,",
        "    &#.,,,,,,%%*#,*",
        "     @%.,,,,,\\..*...",
        "   ..,,,,,,,,,,,@,,%",
        "       %&@ @,,,,,,,.@@",
        "     @@...@*************@",
        "     @@.@@@**********@@@",
        "           @@******@@...@",
        "           @@******@@...@",
        "           @@******@@...@",
        "           @@*@@*****@...@@",
        "           @@@****@**@...@@",
        "           @@******@@@@@@",
        "             @@@@@....@@",
        "                @@@..@**@",
        "                  @***@@",
        "                @@****@@",
        "              @@*****@"
    },
    {
        "\\..)",
        " @.....\\",
        "  )........%",
        "   )\\&%#####%\\\\",
        "     &%#..,,,,\\,.,",
        "    ,,,..,.,,.,,,,&",
        "  #,.#.,,,,,,.,).,.",
        "    )#,,,,,,,@*\\@,.",
        "     @#,,,,,,,),&,.#",
        "    ,,,,,,,,,,,,%,,.",
        "       ,% @@.,,,,,,,@@@",
        "       @@.@@*********@@.@@",
        "       @@.@@********@.....@@",
        "      @...@@********@@@.....@",
        "    @@...@  @********@@...@@",
        "    @@...@  @*******@.....@@",
        "             @@***@@....@@",
        "             @@@@***@@@@",
        "             @@@@****@@",
        "    @@***@@@.....@@@@..@",
        "      @***@@...@@@@@...@",
        "         @**@       @...@@",
        "          @@         @@@**@@",
        "                  @@****@@"
    },
    {
        "%*\\",
        " *....&",
        "   .......)",
        "    .........%",
        "      \\...,,,,.)\\",
        "      #@#@%.,,,@,,,",
        "    #.,,,,,,,,,,,,,,",
        "    %\\)%,,,,,.,)@),,",
        "     &*.%,,,,,..#%,#",
        "      \\.,,,,,&*,,&,,%",
        "     #.,,,....\\#@*,,.%",
        "             @@@@@@@@*@@.@@",
        "           @@********@.....@@",
        "          @@@*********@@.....@",
        "        @@...@**********@....@",
        "        @@.@@@********@@...@@",
        "        @@.@@ @@***@@@.....@@",
        "        @@...@@@@@@***@@@@@",
        "          @@@ @@**@*****@",
        "                @@@***@@@",
        "                @@.@@@@@.@@",
        "              @@...@@ @@...@@",
        "          @@@@....@     @@@**@",
        "          @*****@@      @****@"
    },
    {
        "&..%",
        " ).....%",
        "  ........,,",
        "    . ,*\\,**..#",
        "      ,)\\#%,,,,,.*",
        "    *,,.#@&,,. ,,,*",
        "   ),\\,,,,,,,,#%.,,#",
        "     ..,,,,,,&.%..,@",
        "     @.,\\,,,,,#,%,@)",
        "    #.,,,,,,.,,,.,,,,",
        "      *#,&  ,.,,,,,,@@",
        "            @@@@@**@..@",
        "           @*******@...@@",
        "           @*******@.....@@",
        "           @****@@@....@@",
        "           @@@@@....@@",
        "           @****@@@@@@",
        "         @@.@@******@@",
        "         @@.@@@@@***@@",
        "           @@@...@@@**@",
        "              @@...@@@",
        "              @@...@",
        "            @@*****@",
        "              @@@@@"
    },
    {
        "&..&",
        " \\.....)",
        "  .........\\",
        "    .,\\##)))*.)",
        "       )**&,,,,, )",
        "    ),,.\\&#,,. ,,,, ",
        "   \\,#,,,,,,,,&@ ,,#",
        "     *.,,,,,,%.&..,@",
        "     &..*,,,,,#,&,&\\",
        "    %.,,,,,,.,,,.,,,,",
        "      .&\\&  ,.,,,,,,&",
        "     @...@@*@@...@@*@@@",
        "     @.....@@@.....@**@",
        "      @@@........@@***@",
        "           @@@@@****@@.@@",
        "           @********@@@",
        "            @@********@",
        "           @**********@",
        "           @@@***@@***@",
        "           @....@@@...@",
        "            @@@@*@@...@",
        "           @****@  @...@@",
        "           @****@  @@@@@@",
        "                @******@@"
    }
};

/* usage - print usage message and exit
* exit_code: Integer exit code value (0 success, 1 error, ...)
*
* Returns: void (exits program on run)
*/
void usage(const int exit_code) {
    printf( "\nUsage:\n  ./chad_stride [flags]\n\nFlags:\n"
            "  -s              Chad stays still and doesn't move forever\n"
            "  -l              Chad walks indefinitely\n"
            "  -r              Chad walks backwards\n"
            "  -b SIZE         Big Chad (each b increases chad)\n"
            "  -f              Chad gives no fucks (ignores signals)\n"
            "  -a SLEEP_TIME   Adjust sleep timer for chad\n"
            "  -c COLOR        Chad strolls by in the color of your choice (r|g|b|m|c|y)\n"
            "  -m MESSAGE      Display message under chad [capped at 50 characters]\n"
            "  -R              Have Chad stroll with a rainbow effect\n"
            "  -p              Have Chad flip when he hits the edge of the screen\n"
            "  -h              Display this message\n\n");

    exit(exit_code);
}

/* get_color_pair - Helper function to turn char to color value
* c: character input representing desired color
* pair: previous pair value
*
* returns: New color pair based on input
*/
int get_color_pair(char c, int pair){
    switch (c){
        case 'w':
            return 1;
        case 'b':
            return 2;
        case 'r':
            return 3;
        case 'g':
            return 4;
        case 'c':
            return 5;
        case 'y':
            return 6;
        case 'm':
            return 7;
        default:
            return pair;
    }
}

/*
* arg_to_long: converts char* to long
* arg: string (should be a number)
*
* returns: converted long from string
*/
long arg_to_long(char* arg) {
    char* p_end;
    long modifier = strtol(optarg, &p_end, 10);
    if (p_end == optarg) {
        fprintf(stderr, "Invalid digits for flag -b\n");
        usage(1);
    }
    return modifier;
}

/*
* arg_to_long: enables flags set by user
* arg: main args
*
* returns: void
*/
void handle_args(int argc, char *argv[]) {
    char c;
    while ((c = getopt(argc, argv, ARG_FLAGS)) != -1) {
        switch (c) {
            case 's':
                STAY = true;
                break;
            case 'l':
                LOOP = true;
                break;
            case 'r':
                REVERSE = true;
                break;
            case 'm':
            {
                MES = true;
                size_t input_length = strlen(optarg);
                if (input_length > MAX_MESSAGE_SIZE) {
                    fprintf(stderr, "Message size is capped at %d characters.\n", MAX_MESSAGE_SIZE);
                }
                input_length = input_length < MAX_MESSAGE_SIZE ? input_length : MAX_MESSAGE_SIZE;
                message[MAX_MESSAGE_SIZE] = '\0';
                strncpy(message, optarg, input_length);
                break;
            }
            case 'b':
            {
                long modifier = arg_to_long(optarg);
                if (modifier < 0) {
                    fprintf(stderr, "Error: invalid argument -b. Must be positive!\n");
                }
                SIZE += modifier;
                break;
            }
            case 'f':
                NOFUCKS = true;
                break;
            case 'a':
            {
                long modifier = arg_to_long(optarg);
                if (modifier < 0) {
                    fprintf(stderr, "Error: Access flag isn't positive!\n");
                }
                SLEEP_TIMER = modifier;
                break;
            }
            case 'R':
            {
                COLOR = true;
                RAINBOW = true;
                PAIR = 1;
                break;
            }
            case 'c':
                NORAINBOW = true;
                COLOR = true;
                /* check ascii char val a-z */
                if (((unsigned char) *optarg) > 96 && ((unsigned char) *optarg) < 173) {
                    PAIR = get_color_pair(*optarg, PAIR);
                } else {
                    fprintf(stderr, "Error: invalid color value. Must be (w|r|g|b|m|c|y)\n\n");
                    usage(1);
                }
                break;
            case 'p':
                FLIP = true;
                break;
            case 'h':
                usage(0);
            default:
                usage(1);
        }

        if(NORAINBOW && RAINBOW){
            fprintf(stderr, "Error: Cannot do rainbow and color simultaneously\n");
            usage(1);
        }

        if(FLIP && LOOP){
            fprintf(stderr, "Error: Cannot do loop and flip simultaneously\n");
            usage(1);
        }
    }
}


int main(int argc, char *argv[]) {
    handle_args(argc, argv);

    int rainbowIterator = 1;

    // set signals
    if (NOFUCKS) {
        signal(SIGHUP , SIG_IGN);
        signal(SIGINT , SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        signal(SIGTERM, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
    }

    /* ncurses stuff */
    int col, row;
    initscr();

    /* start color if term supports it */
    if (COLOR && has_colors()) start_color();

    getmaxyx(stdscr, row, col); // stdscr is default screen

    // center chad if stay is set
    int x_pos = STAY ? (col / 2) - (AVG_CHAD_WIDTH / 2): 0;
    int i = 0;
    noecho();
    curs_set(0);
    nodelay(stdscr, 1);
    leaveok(stdscr, 1);
    scrollok(stdscr, 0);

    /* initialize colors */
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

    /* loop until done */
    int endpoint = col;
    if (REVERSE) {
        endpoint = 0;
        x_pos = col;
    }

    while(x_pos != endpoint || LOOP || FLIP) {

        // call this just in case resize of term
        getmaxyx(stdscr, row, col);
        /* set color */
        attron(COLOR_PAIR(PAIR));

        // check for wrap on loop
        if(FLIP){
            if(REVERSE && x_pos <= 0){
                REVERSE = false;
            } else if(x_pos + strlen(chad_frames[i % CHAD_FRAMES][1]) + 1 >= col) {
                REVERSE = true;
            }
        } else if (REVERSE && x_pos <= 0) {
            x_pos = col;
        } else if (x_pos >= col) {
            x_pos = 0;
        }

        if(!FLIP){
            for (int j = 0; j < CHAD_ROWS; j++) {
                // find position of furthest character to the right
                int right_pos = x_pos + strlen(chad_frames[i % CHAD_FRAMES][j]);
                // draw row, checking it doesn't go off screen if not looping
                mvaddnstr(j * SIZE + row/2 - (CHAD_ROWS*SIZE/2), x_pos,
                          chad_frames[i % CHAD_FRAMES][j],
                          (right_pos > col && !LOOP) ? col - x_pos : right_pos);
            }
        } else if(!REVERSE) {
            // Start going right
            for (int j = 0; j < CHAD_ROWS; j++) {
                // find position of furthest character to the right
                int right_pos = x_pos + strlen(chad_frames[i % CHAD_FRAMES][j]);
                // draw row, checking it doesn't go off screen if not looping
                mvaddnstr(j * SIZE + row/2 - (CHAD_ROWS*SIZE/2), x_pos,
                          chad_frames[i % CHAD_FRAMES][j],
                          (right_pos > col && !LOOP && !FLIP) ? col - x_pos : right_pos);
            }
        } else {
            // Start going left
            for(int j = 0; j < CHAD_ROWS; j++){
                // find position of furthest character to the right
                int right_pos = x_pos + strlen(chad_frames_flip[i % CHAD_FRAMES][j]);
                // draw row, checking it doesn't go off screen if not looping
                mvaddnstr(j * SIZE + row/2 - (CHAD_ROWS*SIZE/2), x_pos,
                          chad_frames_flip[i % CHAD_FRAMES][j],
                          (right_pos > col && !LOOP && !FLIP) ? col + x_pos : right_pos);
            }
        }

        i++; // increment animation counter
        if (!STAY) {
            if (REVERSE) {
                x_pos--;
            } else {
                x_pos++; // move across screen
            }
        } else {
            x_pos = (col / 2) - (AVG_CHAD_WIDTH / 2);
        }

        /* Ncurses stuff */
        refresh();

        /* display message */
        if (MES) {
            int mesg_x = x_pos + AVG_CHAD_WIDTH / 2 - (strlen(message) / 2);
            int mesg_y = ((3 * row / 4) + (CHAD_ROWS * SIZE / 4));

            if (mesg_x + strlen(message) >= col && !LOOP){
                mvaddnstr(mesg_y, mesg_x, message, col - mesg_x);
            } else if (mesg_x + strlen(message) >= col && LOOP) {
                mvaddnstr(mesg_y, mesg_x, message, col - mesg_x);
                mvaddnstr(mesg_y, 0, message + col - mesg_x, strlen(message) - col + mesg_x);
            } else {
                mvaddnstr(mesg_y, mesg_x, message, strlen(message));
            }
        }

        /* Color stuff */
        char c = getch();

        if (COLOR && has_colors()){
            PAIR = get_color_pair(c, PAIR);
        }

        if(RAINBOW && !NORAINBOW){
            PAIR = rainbowIterator;
            if(rainbowIterator < 7){
                rainbowIterator++;
            } else {
                rainbowIterator = 1;
            }
        }

        clear();

        /* animation wait */
        usleep(SLEEP_TIMER);
    }

    /* Clean up */
    delwin(stdscr);
    endwin();
    refresh();

    return 0;
}
