
 //////////////////////////////
 //                          //
 //         C - Maze         //
 //                          //
 //////////////////////////////

 Not the most glamorous name..

 This is rewritten from a java/swing game I made for a class project, mostly
 as a bit of practice in learning the C language (and SDL!)

 Right now it is feature-complete to the java version (minus dialog boxes
 explaining the "story") but has an updated lighting algorithm that *doesn't*
 go through walls! (still needs work)

 Dependancies:
    
    make, gcc, SDL2, SDL2_image

 Installation/Build:
    
    on Unix-likes, cd to the directory with the files and run make
    
    if building on Windows.. ?  I don't really know Windows development

 Objective:
    
    find the treasure


 Controls:
    
    WASD for turning and movement

    ESC to quit (or just close the window)

 
 Things I'd like to add:

    cleaner code
        This is the first thing I've written in C, and I had no idea how
        to organize things.

    animations

    items/powerups
        Flashlight batteries might be a good place to start

    multiple levels

    story?

    monsters?
        really this could become a rogue-like pretty easily

    randomized mazes?
