#!/bin/bash

# Define directories
SRCDIR=src
INCLUDEDIR=include
SDLINCLUDEDIR=C:/SDL2/include
OBJDIR=obj/debug

# Parse flags
RELEASE=false
DOCSGENERATOR=false

while getopts ":rd" opt; do
  case $opt in
    r)
      echo "COMPILING RELEASE VERSION!"
      RELEASE=true
      ;;
    d)
      echo "COMPILING DOCS GENERATOR!"
      DOCSGENERATOR=true
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

if $DOCSGENERATOR; then
   g++ -c iceDocsGenerator.cpp -std=c++14 -g -m64 -o obj/iceDocsGenerator.o
   g++ obj/iceDocsGenerator.o -o iceDocsGenerator
  exit 0
fi

if $RELEASE; then
  OBJDIR=obj/release
fi

# Create the output directory if it doesn't exist
mkdir -p $OBJDIR

# Compile all .cpp and .c files in the src directory
for file in $SRCDIR/*.cpp $SRCDIR/*.c $SRCDIR/detail/*.cpp; do
  if [ -f "$file" ]; then
    objfile=$OBJDIR/$(basename ${file%.*}.o)
    # Check if the object file needs to be recompiled
    if [ ! -f "$objfile" ] || [ "$file" -nt "$objfile" ]; then
      echo "Compiling $file..."
      if $RELEASE; then
        g++ -c $file -o $objfile -std=c++14 -O3 -g -m64 -I $INCLUDEDIR -I $SDLINCLUDEDIR
      else
        g++ -c $file -o $objfile -std=c++14 -g -m64 -I $INCLUDEDIR -I $SDLINCLUDEDIR        
      fi
    else
      echo "Skipping $file, object file is up to date."
    fi
  fi
done

echo "Creating the main.exe file"

if $RELEASE; then
  g++ obj/release/*.o -o bin/release/main -L C:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
else
  g++ obj/debug/*.o -o bin/debug/main -L C:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
fi