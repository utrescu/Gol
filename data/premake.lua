project.name="gol"

package = newpackage()
package.name = "gol"

project.config["Debug"].bindir  = "debug"
project.config["Release"].bindir = "Release"

package.config["Debug"].objdir="debug"
package.config["Release"].objdir="Release"

package.language = "c++"
package.kind="exe"

-- Per les llibreries

if (OS == "linux") then
  tinsert(package.buildoptions, "`sdl-config --cflags`")
  tinsert(package.linkoptions, "`sdl-config --libs` -L/usr/X11R6/lib -lGL -lSDL_mixer -lSDL_image -lSDL_ttf -lSDL_gfx")
end

-- Fitxers

package.files = 
{
  matchfiles("src/*.h", "src/*.cpp")
}

