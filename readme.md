# Getting started

Clone repository using recursive flag to setup all submodules:
```
git clone --recursive https://github.com/BlurryRoots/hannelore.git
```

Build a makefile or project file using cmake.
If you are using (unix) make just run the following command in project root:

```
cmake . && make
```

To start the program execute ```make run```.

Current controls:
Input | Action
--- | ---
Q | Increase altitude
E | Decrease altitude
W | Move forward
A | Move left
S | Move right
D | Move back
UP | Look up
LEFT | Look left
RIGHT | Look right
DOWN | Look down

By scrolling one can control the radius and intensity of the point light,
placed in the scene. Toggle between intensity and radius by hitting control.
