# ProceduralCity
My third-year project on 'Procedural Generation of Cities'. Information about the project itself can be found [here](https://robcarlan.github.io/procedural.html).

## Building
Boost is manually included by the include path located in the .pro file. This project relies on compiled boost libraries, so make sure the compiled boost libraries match the compiler version, i.e. MSVC140

### Visual Studio:
Using the Developer Command Prompt:

  `qmake -tp vc ProceduralCity.pro`
  
  Creates a .vcxproj for the solution. If the UI components change, then qmake has to be rerun in order to regenerate the ui .cpp.

### Linux
Create a makefile with:

`qmake ProceduralCity.pro` 

and then just `make`. I wasn't able to test the program fully with Linux, as my graphics card (NVidia 480m) doesnt have recent enough OpenGL to support the indexed rendering.
