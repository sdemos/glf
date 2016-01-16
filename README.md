# OpenGL Framework #

This is an opengl framework that I am building up so I can more effectively
make stupid opengl applications for fun. 

## GLFW and SOIL

Neither of these packages come with a Find\*.cmake file, at least on gentoo, so
I had to install them manually. I took the GLFW finder from
[here](https://github.com/PixarAnimationStudios/OpenSubdiv/blob/master/cmake/FindGLFW.cmake)
and the SOIL finder from
[here](https://github.com/JoeyDeVries/LearnOpenGL/blob/master/cmake/modules/FindSOIL.cmake).
I then copied those files to `/usr/share/cmake/Modules` so that cmake could
find them without any help, and everything worked.

Actually, SOIL isn't even in the default gentoo repos. I stole an ebuild from
someone elses overlay that I found who packaged it and I threw it into [my
overlay](https://github.com/sdemos/sdemos-overlay), which can be added by
running `layman -o https://github.com/sdemos/sdemos-overlay.git -f -a
sdemos-overlay`.
