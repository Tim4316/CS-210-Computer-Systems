#!/bin/bash

for((i=0;i<=99;i++)); #looping throw all 99 pages
do
  ./$1 $i > find 2> error #takes the first argument of the command-line. Then processes and redirect the standard error from the standard output
  #standard output 
  output=$(cat find | grep -i waldo | wc -l) #looking for the number of standard output
  #standard error
  error=$(cat error | grep -i waldo | wc -l) #looking for the number of  standard error
  #file line
  fl=$( grep -i -r waldo PuzzleDir | wc -l) #looking for the number of file lines
  #file name
  fn=$(find PuzzleDir | grep -i waldo | wc -l) #looking for the number of file names
 if [[ $output -ne 0 || $error -ne 0 || $fl -ne 0 || $fn -ne 0 ]]; then
    echo $1 $i $output $error $fl $fn
 fi
 rm -r ./PuzzleDir #remove the conents before running the puzzle again
done

  