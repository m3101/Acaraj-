# Acarajé Devlog
Development logs, uptdates, lore, curiosities and more about the human aspect of developing this game.

---

## Day 4
### Starting the devlog

I decided to start documenting the development through here too. The individual updates to the game's features are already well documented on the commit history, so this will serve as a registry of the other aspects of development.

This includes ideas I had, tests I made, weird happenings and other (hopefully) interesting things.

* After playing around with the designer interface for a while, I decided to make a few corrections to it. When adjusting the weapon arm, the sword angle remains constant now (it feels more intuitive). The head is now always parallel to the torso too.

## Day 5
### Priority changes

I undid the angle constraining for the sword. That was taking too much work and providing too little results.

From now on my focus is going to be on making the game playable. First in a single computer, then through networking. Afterwards, I'll focus on making the game prettier and more comfortable to play (which would include the postponed designer state changes).

## Day 6
### OpenGL Shaders

I stopped working on this project for a while, so I kind of forgot what exactly I was working on.

I'm too lazy to reimplement everything in OpenGL, but I wanted to do some post-processing, so I made a workaround. This might be useful for some people, as I didn't find a comprehensive tutorial for OpenGL postprocessing in SDL2 contexts anywhere. Oh, and it also means this hasn't been tested by anyone but me, nor in any machine other than my trusty `m3101`(A laptop with a Nvidia GPU). I can only hope that it works well in other architectures.

## Day 7
### Lots of stuff

Once again I stopped working on this project for quite a while. It's 2021, in fact.

I changed the build process so it checks for some extra libraries (including OpenGL) before building with them. This way, someone without a configured OpenGL environment can still build the Custom-Shader-less version. I also added a TTS system to make things more ridiculous.

The game was lacking a menu, so I made one too. It also lacked music, but I'm lazy so I composed a couple loops and made a system to loop them over each other 