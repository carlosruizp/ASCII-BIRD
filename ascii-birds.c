#include "ascii-birds.h"

/* Necessary functions*/
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

int aleat_num(int inf, int sup) {
    int num = 0;
    if (inf > sup) return 0;
    num = rand();
    return num % (sup - inf + 1) + inf;
}


/* Game functions*/
int fcrash;
int score;
int listo1, listo2;
short touch;
BIRD * bird;
PIPE * pipes[NUM_PIPES];
PIPE * current_pipes[NUM_PIPES];
PIPE * next_pipes[NUM_PIPES];

int ini_bird() {
    bird = (BIRD *) malloc(sizeof (BIRD));
    bird->height = 15;
    bird->fall = 0;
    bird->mode = 0;
}

int play_AsciiBirds(INTERF * interf) {
    pthread_t pth, pth2;
    int i;
    fcrash = 0;
    score = 0;
    listo1 = 0;
    listo2 = 0;
    touch = 0;
    ini_bird();
    print_bird(interf);


    /* Creation of the pipes thread*/
    pthread_create(&pth, NULL, pipes_function, (void*) interf);

    /* Creation of the falling thread*/
    pthread_create(&pth2, NULL, free_fall, (void*) interf);


    //for (i = 0; i < 100000; i++);
    while (!listo1 && !listo2);

    while (!crash() && !fcrash) {
        
        touch = 0;
        getch();        
        erase_bird(interf);
        go_up();
        print_bird(interf);
        usleep(100000);
        touch = 1;
    }
    pthread_join(pth, NULL);
    pthread_join(pth2, NULL);
    free(bird);
    for (i = 0; i < NUM_PIPES; i++) {
        free(pipes[i]);
        free(current_pipes[i]);
    }
    return score;
}

void * free_fall(void * pointer) {
    INTERF * interf;
    interf = (INTERF *) pointer;
    while (!listo1);
    listo2 = 1;
    while (!crash() && !fcrash) {

        if (!touch) {
            erase_bird(interf);
            bird->height++;
            bird->fall++;
            print_bird(interf);

        }
        if (bird->fall/2 < 13)
            usleep(11000 * (18 - bird->fall/2));
        else
            usleep(11000 * 6);
    }
    pthread_exit(NULL);

}

int ini_pipes() {
    int i;

    for (i = 0; i < NUM_PIPES; i++) {
        if (score == 0) {
            pipes[i] = (PIPE *) malloc(sizeof (PIPE));
            current_pipes[i] = (PIPE *) malloc(sizeof (PIPE));
        }
        pipes[i]->number = i;
        pipes[i]->position = 143 + 30 * i;
    }

    pipes[0]->min = 20;
    pipes[0]->max = 30;
    pipes[1]->min = 3;
    pipes[1]->max = 13;
    pipes[2]->min = 5;
    pipes[2]->max = 15;
    pipes[3]->min = 8;
    pipes[3]->max = 18;
    pipes[4]->min = 12;
    pipes[4]->max = 22;
    pipes[5]->min = 16;
    pipes[5]->max = 26;
}

int calculate_next() {
    int i;

    for (i = 0; i < NUM_PIPES; i++) {
        next_pipes[i] = pipes[aleat_num(0, NUM_PIPES - 1)];
    }
    return 0;
}

int copy_pipes() {
    int i;
    for (i = 0; i < NUM_PIPES; i++) {
        current_pipes[i]->number = next_pipes[i]->number;
        current_pipes[i]->min = next_pipes[i]->min;
        current_pipes[i]->max = next_pipes[i]->max;
    }
}

int calculate_next_u(int i) {
    next_pipes[i] = pipes[aleat_num(0, NUM_PIPES - 1)];
}

int copy_pipes_u(int i) {
    current_pipes[i]->number = next_pipes[i]->number;
    current_pipes[i]->min = next_pipes[i]->min;
    current_pipes[i]->max = next_pipes[i]->max;    
}

void * pipes_function(void * pointer) {
    INTERF *interf = (INTERF *) pointer;
    int i;
    char cadena[10];
  
    ini_pipes();

    calculate_next();
    copy_pipes();
    for (i = 0; i < NUM_PIPES; i++)
        current_pipes[i]->position = 123 + 35 * i;

    listo1 = 1;
    while (!crash() && !fcrash) {

        for (i = 0; i < NUM_PIPES && !fcrash; i++) {
            
            usleep(25000);
            sprintf(cadena, "%d", score);
            write_interf(interf, 0, 13, 70, cadena);
            current_pipes[i]->position--;
            print_pipe(interf, current_pipes[i]);
            if (current_pipes[i]->position <= 0) {
                erase_first(interf);
                score++;
                calculate_next_u(i);
                copy_pipes_u(i);
                current_pipes[i]->position = 87 + 123;
            }
        }
    }
    pthread_exit(NULL);

}

int go_up() {
    bird->height -= 5;
    bird->fall = 0;
}

PIPE * actual_pipe() {
    int i;
    for (i = 0; i < NUM_PIPES; i++) {
        if (current_pipes[i]->position < BIRD_POS + BIRD_LONG && current_pipes[i]->position + PIPE_TAM > BIRD_POS)
            return current_pipes[i];
    }
    return NULL;
}

int crash() {
    PIPE * actpipe;
    //return 0;
    actpipe = actual_pipe();
    if(bird->height>28){
        fcrash=1;
        return 1;    
    }
    else if (actpipe == NULL)
        return 0;
    if (bird->height < actpipe->min || (bird->height + BIRD_TAM) > actpipe->max || bird->height > 28) {
        fcrash = 1;
        return 1;
    } else
        return 0;
}

/* Printing*/
int print_pipe(INTERF * interf, PIPE * pipe) {
    FILE * f;
    char path[200];
    sprintf(path, "pipes/pipe%d", pipe->number);
    f = fopen(path, "r");
    load_map2(interf, 0, 0, pipe->position, f);
    fclose(f);
}

int print_bird(INTERF * interf) {
    FILE * f;
    char path[200];
    sprintf(path, "birds/bird");
    f = fopen(path, "r");
    load_map2(interf, 0, bird->height, BIRD_POS, f);
    fclose(f);
}

int erase_pipe(INTERF * interf, PIPE * pipe) {
    FILE * f;
    char path[200];
    sprintf(path, "pipes/erase_pipe");
    f = fopen(path, "r");
    load_map2(interf, 0, 0, pipe->position, f);
    fclose(f);
}

int erase_bird(INTERF * interf) {
    FILE * f;
    char path[200];
    sprintf(path, "birds/erase_bird");
    f = fopen(path, "r");
    load_map2(interf, 0, bird->height - 2, BIRD_POS, f);
    fclose(f);
}

int erase_first(INTERF * interf){
    FILE * f;
    char path[200];
    sprintf(path, "pipes/erase_first");
    f = fopen(path, "r");
    load_map2(interf, 0, 0, 1, f);
    fclose(f);
}

int main(){
	INTERF * interf;
    FILE *f, *g;
    int ret;
    char *answer;
   
    system("clear");

    f = fopen("interface_params", "r");
    g = fopen("interface_limit", "r");
    interf = createInterface(f, g);
    print_design(interf);
    do {
        master_clear_interf(interf);
        ret=play_AsciiBirds(interf);
        usleep(1000000);
        usleep(3500000);
        print_design(interf);
        master_clear_interf(interf);
        write_interf(interf, 0, 1, 1, "Try again? (Yes : Y, NO: N)");
        fflush(stdout);
        usleep(2500000);
        answer=get_string(interf, 0, 3, 1);
        //write_interf(interf, 1, 5, 1, answer);
        //getch();
        //master_clear_interf(interf);
    } while(answer[0]!='N' && answer[0]!='n');
    printf("%c[%d;%d;%dm", 27, 0, 0, 0);
    fflush(stdout);
    usleep(10000);
    system("clear");
    destroy_interf(interf);
    return (EXIT_SUCCESS);
}