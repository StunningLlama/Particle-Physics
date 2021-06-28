=== Particle physics ===

This is a particle based physics simulation written in C++ and rendered with OpenGL. Here is a list of things it can simulate:
* Compressible gasses
* Liquids
* Rigid bodies
* And more to come.

=== Keybinds ===

Key		Action
A		Brush mode: Add
R		Brush mode: Replace
D		Brush mode: Delete
M		Brush mode: Move (default)
L Mouse	Draw
R Mouse	Delete

1		Material: Water (default)
2		Material: Air
3		Material: Barrier
4		Material: Stone
5		Material: Sand
6		Material: Cloth
7		Material: Rubber
8		Material: Plastic
9		Material: Ceramic

P		Pause
CTRL-S	Save from file
CTRL-O	Load from file
C		Reset simulation
F		Advance frame
,		Change boundary condition [Wall/Wind tunnel]
]		Increase flow velocity
[		Decrease flow velocity

W		Increase brush size
S		Decrease brush size

~		Change background apperance [Blank/Pressure]
TAB		Change particle rendering mode [Material/Pressure/Invisible]
/		Toggle bond visualization
F5		Decrease pressure contrast
F6		Increase pressure contrast
F7		Decrease pressure brightness
F8		Increase pressure brightness

+		Increase draw density
-		Decrease draw density

X		Special function 