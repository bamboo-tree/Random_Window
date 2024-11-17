/*

This program moves window in random directions.
why? because it's fun

*/


#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <math.h>

#define WIDTH           400
#define HEIGHT          300
#define SCREEN_WIDTH    1920
#define SCREEN_HEIGHT   1080
#define STEP            5 // minimum step
#define SLEEP           300 // ms


void update_time(unsigned int* current_time){
    // idk i found this on stackoverflow
    *current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int* generate_random_coordinates(){
    int* result;
    result[0] = ((rand() % (SCREEN_WIDTH - WIDTH)) / STEP) * STEP;
    result[1] = ((rand() % (SCREEN_HEIGHT - HEIGHT)) / STEP) * STEP;
    return result;
}

void print_position(int* array){
    printf("x: %d, y: %d\n", *(array), *(array+1));
}

float* generate_sin_table(int size){
    float step = M_PI * 2 / size;
    float* array = new float[size];
    float sum = 0;

    for(int i = 0; i < size; i++){
        array[i] = (sin((step*i) - (M_PI/2)) + 1) / 2;
        sum += array[i];
    }
    for(int i = 0; i < size; i++){
        array[i] /= sum;
    }

    return array;
}


void smooth_transition(int* c_position, int* n_position, sf::Window& window){
    int step = 15;
    int delay = 20;

    print_position(c_position);
    print_position(n_position);

    // total vector
    int vector[2] = {n_position[0] - c_position[0], n_position[1] - c_position[1]};
    // divide vector to move by "step" each frame
    int sub_distances = sqrt(pow(vector[0], 2) + pow(vector[1], 2)) / step;
    float* scale = generate_sin_table(sub_distances);
    // create subvectors
    float small_vector[2];
    float real_x = float(c_position[0]);
    float real_y = float(c_position[1]);

    // create timestamps for delay
    unsigned int current_time;
    update_time(&current_time);
    unsigned int stop_time = current_time;

    // move n times - n = sub_distances
    for(int i = 0; i < sub_distances; i++){
        // this probably shouldn't look like that :3
        while(current_time - stop_time < delay){
            update_time(&current_time);
        }
        stop_time = current_time;

        // update small vector
        small_vector[0] = float(vector[0])*scale[i];
        small_vector[1] = float(vector[1])*scale[i];

        // update position and move window
        real_x += small_vector[0];
        real_y += small_vector[1];
        // print current position
        // printf("x: %.2f, y: %.2f\n", real_x, real_y);
        window.setPosition(sf::Vector2i(int(real_x), int(real_y)));
    }
    // to be sure place it in desired position
    window.setPosition(sf::Vector2i(n_position[0], n_position[1]));
}

void linear_transition(int* c_position, int* n_position, sf::Window& window){
    int step = 15;
    int delay = 20;

    print_position(c_position);
    print_position(n_position);

    // total vector
    int vector[2] = {n_position[0] - c_position[0], n_position[1] - c_position[1]};
    // divide vector to move by "step" each frame
    int sub_distances = sqrt(pow(vector[0], 2) + pow(vector[1], 2)) / step;
    // create subvectors
    float small_vector[2] = {float(vector[0])/sub_distances, float(vector[1])/sub_distances};
    float real_x = float(c_position[0]);
    float real_y = float(c_position[1]);

    // create timestamps for delay
    unsigned int current_time;
    update_time(&current_time);
    unsigned int stop_time = current_time;

    // move n times - n = sub_distances
    for(int i = 0; i < sub_distances; i++){
        // this probably shouldn't look like that :3
        while(current_time - stop_time < delay){
            update_time(&current_time);
        }
        stop_time = current_time;

        // update position and move window
        real_x += small_vector[0];
        real_y += small_vector[1];
        // print current position
        // printf("x: %.2f, y: %.2f\n", real_x, real_y);
        window.setPosition(sf::Vector2i(int(real_x), int(real_y)));
    }
    // to be sure place it in desired position
    window.setPosition(sf::Vector2i(n_position[0], n_position[1]));
}


int main(){
    // create window
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Random Positon");

    // create timestamps for delay
    unsigned int current_time;
    update_time(&current_time);
    unsigned int stop_time = current_time;

    // start with window in random position
    srand(time(NULL));
    int *current_position = generate_random_coordinates();
    window.setPosition(sf::Vector2i(*current_position, *current_position+1));

    // main loop
    while(window.isOpen()){
        update_time(&current_time);

        // close window with X
        sf::Event event;
        while (window.pollEvent(event))        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // wait before moving to next position
        if(current_time - stop_time >= SLEEP){
            // save "now" time
            stop_time = current_time;
            int* new_position = generate_random_coordinates();
            // window.setPosition(sf::Vector2i(*new_position, *new_position+1));
            // linear_transition(current_position, new_position, window);
            smooth_transition(current_position, new_position, window);
            current_position[0] = new_position[0];
            current_position[1] = new_position[1];
        }
    }

    return 0;
}
