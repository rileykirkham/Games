// Instructions
// 
// Implement a code breaking game as a console application. At the beginning of the game, it randomly generates a secret 4 digit code.
// Each digit should be unique. For example, 0123, 9548, and 3217 are valid codes, but 1102, 4445, and 7313 are not.
//
// Once the number has been generated, the player has 8 chances to guess the code. After each guess, the game should report how many digits
// they guessed correctly, and how many had the right number, but in the wrong place. For example, with a secret code of 0123, a guess of 0451
// has one digit correct (the 0), and one digit with the right number in the wrong place (the 1).
// 
// After the player has either correctly broken the code or run out of guesses, ask if they would like to play again. If so, generate a new
// code and start again, otherwise exit the program.
//
// If the player enters a code that isn't valid, prompt them to try again but do NOT count the invalid guess as one of the 8 attempts. Also be sure that
// your game can handle any unexpected inputs without crashing.
//
// All your code should go in this file. Please do NOT include your name, as we anonymize tests during evaluation to reduce bias.
//
// Good luck!

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void print(std::string input) {
    std::cout << input << std::endl;
}

void clearScreen() {
    std::cout << std::string(30, '\n');
}

//Prints hidden codes in the form of arrays of ints
void printCode(int* code, int codeSize) {
    std::string temp = "";
    for (int i = 0; i < codeSize; i++) {
        temp = temp + std::to_string(code[i]);
    }
    print(temp);
}

//Create a valid 4-digit code
void genCode(int codeSize, int* code) {
    //Init rand
    srand(time(0));

    int numarr[4] = { -1 };

    for (int i = 0; i < codeSize; i++) {
        bool done = false;
        while (!done) {
            int temp = rand() % 10;
            //Checks if number is already in numarr
            bool alreadyInNumber = (std::find(std::begin(numarr), std::end(numarr), temp) != std::end(numarr));

            if (alreadyInNumber) {
                //Redo this loop and find a differnt number
                continue;
            }
            else {
                numarr[i] = temp;
                done = true;
            }
        }
        code[i] = numarr[i];
    }
}

//Prints user's lives left
void printLives(int lives,bool end) {
    std::string temp;
    if (end) {
        //Displays lives slightly differently when displaying end screen rather than displaying lives in game.
        temp = std::string(lives, '\3') + std::string(8 - lives, '-');
    }
    else {
        temp = "Lives: " + std::string(lives, '\3') + std::string(8 - lives, '-') + '(' + std::to_string(lives) + ')';
    }
    print(temp);

}

//Checks if player guess is valid (Is numerical, 4 digits long, and non-negative)
std::string validGuess(int* code, int codeSize, int lives, std::string (&pastGuesses)[8], bool* debugMode) {
    bool validGuess = false;
    bool debugChange = false;

    std::string guess;

    while (!validGuess) {

        if (*debugMode) {
            print("DB - The code is ");
            printCode(code, codeSize);
        }

        guess = "";
        bool nonIntChar = false;
        debugChange = false;
        printLives(lives,false);
        print("Please enter your guess:");
        std::cin >> guess;
        clearScreen();
        
        

        if (std::find(std::begin(pastGuesses), std::end(pastGuesses), guess) != std::end(pastGuesses)) {
            //User guessed something they already guessed
            print(std::string(2, '\n'));
            print("Oops! Looks like you already tried that combination! Have another go, on the house!");
            print(std::string(2, '\n'));
            continue;
        }

        //Check guess is valid
        for (int i = 0; i < guess.length();i++) {
            //Check if quit game
            if (guess[0] == 'q' && guess.length() == 1) {
                print("Hope you had fun!");
                exit(0);

            }
            if (guess[0] == 'd' && guess.length() == 1) {
                //Debug mode allows user to see hidden code and previous live's guesses.
                *debugMode = !(*debugMode);
                print("Debug is now ");
                if (*debugMode) {
                    print("Enabled.");
                }
                else {
                    print("Disabled.");
                }
                debugChange = true;
                break;
            }
            //Go through individual characters. If any are not digits, change the boolean.
            if (!isdigit(guess[i])) {
                nonIntChar = true;
                break;
            }
        }
        if (debugChange) {
            //Re-runs the loop to display the current state but now with added debug aspects.
            continue;
        }
        if (nonIntChar) {
            print("Sorry, but your guess must be a positive number that is " + std::to_string(codeSize) + " digits long. This has not counted against your life total.");
            print("Please try again");
            std::cin.clear();
            std::cin.ignore();
            continue;
        }
        else if (guess.length() < codeSize) {
            //Input is not the correct number of characters
            print("This code is " + std::to_string(codeSize) + " digits long. Please enter a guess of that length.");
        }
        else if (guess.length() > codeSize) {
            //Guess is too big
            print("Oops! Your number is too large to be the secret code - Try again with a number under 10,000!");
            print("Please try again");
        }
        else {
            //Guess valid
            validGuess = true;
        }
    }
    return guess;

}

//Calculates how many numbers are fully correct and how many are correct but in the wrong place.
void calcCorrect(int codeSize, int* code, int (&guessArr)[4], int* correctPosandInt, int* correctIntOnly) {
    for (int i = 0; i < codeSize;i++) {
        if (code[i] == guessArr[i]) {
            //Correct number in correct pos
            (*correctPosandInt)++;
        }
        else if (std::find(std::begin(guessArr), std::end(guessArr), code[i]) != std::end(guessArr)) {
            //Found number in guess, just wrong place.
            (*correctIntOnly)++;
        }
    }
}

//Displays game text after guess.
void printText(int incorrect, int correctIntOnly, int wrongNum) {
    print("");
    switch (incorrect) {
    case 1: print("You're very close! - 3 digits were fully correct");
        break;
    case 2: print("Half way there! - 2 digits were fully correct");
        break;
    case 3: print("Hey, you got one! - 1 digit was fully correct");
        break;
    case 4: print("Tough Luck! - No digits were fully correct");
        break;
    }
    if (correctIntOnly) {
        if (correctIntOnly == 1) {
            print("but,  1 digit was in an incorrect location.");
        }
        else {
            print("but, " + std::to_string(correctIntOnly) + " of your numbers were in incorrect locations.");
        }
    }
    if (incorrect > 1) {
        print(std::to_string(wrongNum) + " digits are incorrect.");
    }
    else {
        print("1 digit is incorrect.");
    }
}


//Main game loop. Handles user guesses and determines when game is over.
void playGame(int* code, int codeSize, bool debugMode) {
    int lives = 8;
    std::string pastGuesses[8];
    bool gameOver = false;
    //Make list of arrays and track valid guesses so we don't count off for codes you've already guessed?
    while (!gameOver && lives) {
        //Get player guess

        

        std::string guess = validGuess(code, codeSize, lives, pastGuesses,&debugMode);
        std::string guessBackUp = guess;
        pastGuesses[8 - lives] = guess;

        if (debugMode) {
            for (int i = 0; i < 8;i++) {
                print("DB - Life " + std::to_string(i) + "'s guess:" + pastGuesses[i]);
            }
        }

        //Converts guess string into array of single integers. "1234" -> [1,2,3,4]
        int guessArr[4] = { -1,-1,-1,-1 };
        for (int i = 0; i < 4; i++) {
            guessArr[i] = guess[i] - '0'; // - '0' Changes guess[i] from char to int.
        }


        //Calculate how many numbers are correct, how many are incorrect positions, and how many are incorrect entirely.
        int correctPosandInt = 0;
        int correctIntOnly = 0;
        calcCorrect(codeSize, code, guessArr, &correctPosandInt, &correctIntOnly);
       
        

        int incorrect = codeSize - (correctPosandInt);
        int wrongNum = codeSize - (correctPosandInt + correctIntOnly);
        if (debugMode) {
            print("DB - Current Incorrect == " + std::to_string(incorrect));
            print("");
        }   
         if (incorrect) {
            //At least one number is in the wrong spot, or is incorrect.
             print("Your previous guess was:" + guessBackUp);
             //Display text determining user correctness
             printText(incorrect,correctIntOnly,wrongNum);
            print("");
            lives--;
        }
        else{
            //Guess is Correct!
            print("Congrats! You have successfully guessed the code! It was ");
            printCode(code, codeSize);
            if (lives == 1) {
                print("You had 1 life left!");
            }
            else {
                print("You had " + std::to_string(lives) + " lives left!");
            }
            printLives(lives,true);
            print("Please press Enter to continue.");
            std::cin.ignore();
            gameOver = true;
        }
    }
    if (!lives) {
        //Game Over - Out of lives
        print("Unfortunately, you are out of lives :(");
        print("The code was ");
        printCode(code, codeSize);
        print("Please press Enter to continue.");
        std::cin.ignore();
        gameOver = true;
    }
}


//Allows for users to play another game, if they choose to, or otherwise to quit.
void outerGameLoop(int* code, int codeSize, bool debugMode) {
    bool quitGame = false;
    bool validInput = false;
    bool firstLoop = true;
    std::string input = "";
    while (!quitGame) {
        if (firstLoop) {
            playGame(code, codeSize, debugMode);
            firstLoop = false;
        }
        else {
            std::cin.ignore();
            clearScreen();
            print("The game is now over. Would you like to play again? (y/n)");
            while (!validInput) {
                std::cin >> input;
                if (input == "n" || input == "no") {
                    clearScreen();
                    print("Ok, I hope you enjoyed this game! Bye!");
                    quitGame = true;
                    validInput = true;
                    exit(0);
                }
                else if (input == "y" || input == "yes") {
                    quitGame = false;
                    validInput = true;
                    std::cin.ignore();
                    clearScreen();
                    print("Alright, have fun! I've chosen a new code.");
                    genCode(codeSize, code);
                    playGame(code, codeSize, debugMode);
                }
                else {
                    print("Oops I don't really know how to respond to that. Please try again :) (y/n)");
                    validInput = false;
                }
            }
            validInput = false;
        }
    }
}

int main()
{
    std::string input;
    bool debugMode = !true;
    int codeSize = 4;
    
    int code[4] = { -1 };
    genCode(codeSize, code);
    //By this point, numarr should be holding a valid number.

    //Reveals code at beginnning of game
    if (debugMode) {
        printCode(code, codeSize);
    }

    
    print("Hello! The game is ready and loaded. Please have fun!");
    print("Enter 'q' if you want to quit");
    print("");

    outerGameLoop(code,codeSize,debugMode);
    
    return 0;
}