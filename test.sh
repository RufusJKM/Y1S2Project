#!/bin/bash
echo -e "\n~~ Argument Tests ~~"

#There are 4 arguments being passed into the program 
#Executable name, file name, height and width.
#Must check all information is entered and in the right format

echo -n "Testing no arguments - "

./maze > tmp

if grep -q "Usage: maze <filename> <width> <height>" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing no dimensions - "

./maze mazes/easy1.txt > tmp

if grep -q "Usage: maze <filename> <width> <height>" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing dimensions out of bounds (-1) - "

./maze mazes/easy1.txt -1 50 > tmp

if grep -q "Usage: dimension out of bounds (5-100)" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing dimensions out of bounds (101) - "

./maze mazes/easy1.txt 11 101 > tmp

if grep -q "Usage: dimension out of bounds (5-100)" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing non numerical dimensions - "

./maze mazes/easy1.txt 11 five > tmp

if grep -q "Usage: dimensions must be integers" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing non integer dimensions - "

./maze mazes/easy1.txt 11 10.5 > tmp

if grep -q "Usage: dimensions must be integers" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -e "\n\n~~ File Tests ~~"

#Here, i am testing all instances of invalid mazes
#One with unpermitted characters
#One with no start and one with no end
#One with different dimensions to the arguments
#One thats impossible to solve
#And a file that doesn't exist

echo -n "Testing nonexistant file - "

./maze mazes/notReal.txt 16 11 > tmp

if grep -q "Error: Could not find or open file" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing maze file with illegal characters - "

./maze mazes/badChar.txt 16 11 > tmp

if grep -q "Error: File contains illegal characters" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing maze file with no start - "

./maze mazes/noS.txt 16 11 > tmp

if grep -q "Error: No start point S" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing maze file with no end - "

./maze mazes/noE.txt 16 11 > tmp

if grep -q "Error: No end point E" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing maze file with wrong dimensions - "

./maze mazes/wrongDims.txt 20 15 > tmp

if grep -q "Error: Maze file contains wrong dimensions" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

#When the maze is accepted, there should be an output saying so

echo -n "Testing valid maze - "

./maze mazes/easy1.txt 16 11 < inputs/easy1Solve.txt > tmp

if grep -q "File loaded successfully" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -e "\n\n~~ Input Tests ~~"

#This is where i will test all menu inputs
#They can enter multiple values, numbers, or values not on the menu
#All of the above should produce an error message

echo -n "Testing invalid input (9) - "

./maze mazes/easy1.txt 16 11 < inputs/invalid1.txt > tmp

if grep -q "Error: invalid input" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi


echo -n "Testing invalid input (T) - "

./maze mazes/easy1.txt 16 11 < inputs/invalid2.txt > tmp

if grep -q "Error: invalid input" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi


echo -n "Testing invalid input (WA) - "

./maze mazes/easy1.txt 16 11 < inputs/invalid3.txt > tmp

if grep -q "Error: invalid input" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing invalid input (WW) - "

./maze mazes/easy1.txt 16 11 < inputs/invalid4.txt > tmp

if grep -q "Error: invalid input" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

#For valid inputs i will tet both uppercase and lowercase
#I will test just one for each as otherwise, 8 tests would be too much
#For a normal move, the loop should restart with a prompt
#For a Map call, the next output will be a prompt to close the map

echo -n "Testing valid input (S) - "

./maze mazes/easy1.txt 16 11 < inputs/valid1.txt > tmp

if grep -q "Make your next move... " tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing valid input (m) - "

./maze mazes/easy1.txt 16 11 < inputs/valid2.txt > tmp

if grep -q "Enter M to close map" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -e "\n\n~~ Gameplay Tests ~~"

#Here i need to test for the user making invalid moves
#These can either be into walls or off the map
#An invalid move should be met with an error message

echo -n "Testing invalid move (into wall) - "

./maze mazes/easy1.txt 16 11 < inputs/intoWall.txt > tmp

if grep -q "Invalid move" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

rm -f tmp

echo -n "Testing invalid move (off map) - "

./maze mazes/easy1.txt 16 11 < inputs/outOfMap.txt > tmp

if grep -q "Invalid move" tmp;
then
    echo "PASS"
else
    echo "FAIL"
fi

echo -n "Testing endscript runs - "

#I also need to ensure that there is an end message upon completion
#Here, i'm testing the same thing for 3 different mazes
#Rather than showing pass/fail for each maze
#I wanted pass if all three passed and fail if any failed

./maze mazes/easy1.txt 16 11 < inputs/easy1Solve.txt > tmp

if grep -q "Congratulations, you solved the maze!" tmp;
then
    ./maze mazes/easy2.txt 19 13 < inputs/easy2Solve.txt > tmp

    if grep -q "Congratulations, you solved the maze!" tmp;
    then
        ./maze mazes/easy3.txt 19 13 < inputs/easy3Solve.txt > tmp

        if grep -q "Congratulations, you solved the maze!" tmp;
        then
            echo "PASS"
        else
            echo "FAIL"
        fi
    else
        echo "FAIL"
    fi
else
    echo "FAIL"
fi

#At the end, remove the tmp file to keep the directory tidy
rm -f tmp;
