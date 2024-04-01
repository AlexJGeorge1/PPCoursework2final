#!/bin/bash

# Test that the program runs without crashing on valid input
echo "Testing valid input..."
./maze valid_maze.txt > output.txt
if [ $? -eq 0 ]; then
    echo "Passed"
else
    echo "Failed"
fi

# Test that the program handles invalid input correctly
echo "Testing invalid input..."
./maze invalid_maze.txt > output.txt
if [ $? -eq 1 ]; then
    echo "Passed"
else
    echo "Failed"
fi

# Test that the program prevents the player from moving into a wall
echo "Testing wall collision..."
echo "W" | ./maze wall_collision_maze.txt > output.txt
if grep -q "Cannot move in this direction" output.txt; then
    echo "Passed"
else
    echo "Failed"
fi

# Test that the program ends when the player reaches the exit
echo "Testing maze completion..."
echo "D" | ./maze completion_maze.txt > output.txt
if grep -q "Congratulations, you reached the end of the maze!" output.txt; then
    echo "Passed"
else
    echo "Failed"
fi

# Clean up the output file
rm output.txt
