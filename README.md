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

### Todos

**Saving and loading**

Include a configuration file for specifying the whole strucutre - maps, building parameters and final outputs
Output formats for each stage. i.e. using the OpenStreetMap format. Should allow each stage to be a separate program ideally.

**Better CSG**

Better examples using the CSG models. 

**Unique models**

Allow loading of models of a variety of formats which can be loaded in at placeholders following specific conditions. I.e. a park.

