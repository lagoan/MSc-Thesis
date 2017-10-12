// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare My_Water =
texture {                                     
    pigment{ rgbf <1.0, 1.0, 1.0, 0.4> }
    finish {                
//        phong 0.9 phong_size 100
        specular 0.9 roughness 0.01
        reflection {0.02 1 fresnel}
//        ambient 0.05
        diffuse 0.99
        conserve_energy
    }
}
#declare My_Water_Int =
interior {
    ior 1.33
    fade_distance 5
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_M_Water =
material {
    texture {My_Water}
    interior {My_Water_Int}
}

//#declare My_Focus = <0.0000, 0.0000, 0.0000>;
//#declare My_Camera_Location = <0.0000, 0.0000, -10.0000>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <4/3, 0.0,  0.0>
   look_at   My_Focus
}

//light_source { <10.0, 20.0, 0.0> colour White}
light_source { <0.0, 20.0, 0.0> colour White
/*    spotlight
    radius 14
    falloff 16
    tightness 10
    point_at My_Focus
*/
}

#declare Sphere_Water = 
sphere {
    4*x, 1//My_Focus, 1
//  pigment { color rgb <1, 1, 1> }
//  finish { ambient 0 diffuse 0 reflection 0.9 }
    material {My_M_Water}
}

//object { Sphere_Water }
  
sky_sphere {
    pigment {
        gradient y
        color_map {
            [0 color White]//rgb <0.05, 0.05, 0.05>]
            [1 color rgb <0.05, 0.05, 0.55>]
        }
        scale 5
        translate -1
    }
}
    
/* Floor */
plane {
   y, -10

   texture {
      pigment {
//        color rgb <0.5, 0.9, 0.5>
//         checker color red 1.0 green 0.1 blue 0.1
         checker color red 0.5 green 0.9 blue 0.5
                 color red 0.9 green 0.9 blue 0.9
         scale 10
      }
   }
}
