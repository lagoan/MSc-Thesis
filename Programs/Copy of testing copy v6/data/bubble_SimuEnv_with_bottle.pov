// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare My_Focus = <0.0000, 1.2, 0.0000>;

#declare My_Camera_Location = <0.0, 1.2, -7.0>;
                                    
#declare Camera_Focus_Up = 1.0;
#declare Camera_Zoom_In = 0;
camera {
   location  <My_Camera_Location.x,
              My_Camera_Location.y+Camera_Focus_Up,
              My_Camera_Location.z+Camera_Zoom_In>
                 //<0.0, 0.0, -10.0>
//   direction <0.0, 0.0,  1.0>
//   up        <0.0, 1.0,  0.0>
   up        <0.0, 1.0,  0.0>
   right     <2/3 , 0.0,  0.0>
   look_at   <My_Focus.x, My_Focus.y+Camera_Focus_Up, My_Focus.z>
}

#declare Bubble_Color = color White filter 0.95;

#declare My_Bubble_Texture =
texture {
        pigment {Bubble_Color}
    finish {                
//        phong 0.9 phong_size 100
        specular 0.95
        ambient 0.75
        diffuse 0.75
        conserve_energy
    }
}

#declare My_Bubble_Int =
interior {
    ior 1.0
    fade_distance 40
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Lava_Material =
material {
    texture {My_Bubble_Texture}
    interior {My_Bubble_Int}
}
                                         
#declare light_magnitude = 0.5;                                             
light_source { <5.0, 0.0, 10.0> colour light_magnitude}
light_source { <-5.0, 0.0, -10.0> colour light_magnitude}
// two side lights
light_source { <100.0, 0.0, 10.0> colour light_magnitude}
light_source { <-100.0, 0.0, 10.0> colour light_magnitude}
    
/* Floor */
plane { y, -70
   pigment { color red 0.2 green 0.2 blue 0.95}
}                                  

/* Background */    
plane {
   z, 200

   texture {
      pigment {
         checker color red 0.2 green 0.2 blue 0.5
                 color red 0.6 green 0.6 blue 0.6
         scale 20
      }
   }
}

/*                     
#declare Water = color White filter 0.95;

#declare My_Water_Texture =
texture {
        pigment {Water}
    finish {                
//        phong 0.9 phong_size 100
        specular 0.25
        ambient 0.25
        diffuse 0.25
        conserve_energy
    }
}

#declare My_Water_Int =
interior {
    ior 1.33
    fade_distance 40
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Bottle_Glass_Texture =
texture {
        pigment {rgbf < 1.0, 1.0, 1.0, 0.95> filter 0.95}
        finish {
                ambient 0.0
                diffuse 0.0
                reflection 0.1
                phong 0.3
                phong_size 90
        }
}             
#declare My_Bottle_Glass_Int =
   interior{
      ior 1.5
      caustics 2
      fade_distance 1.5
      fade_power 1
   }

#declare Water_Bottle_Radius = 1.5;
#declare Water_Bottle_Thickness = 0.02;
#declare Water_Bottle_Out =
cylinder {
        <0, 8, 0>
        <0, -2, 0>
        Water_Bottle_Radius
}

#declare Bottle_Water =
cylinder {
        <0, 8-Water_Bottle_Thickness, 0>
        <0, -2+Water_Bottle_Thickness, 0>
        Water_Bottle_Radius-Water_Bottle_Thickness 
        texture {My_Water_Texture}
        interior {My_Water_Int}
}             
object {Bottle_Water}

#declare Bottle_Glass =
difference {
        object {Water_Bottle_Out}
        object {Bottle_Water}
        texture {My_Bottle_Glass_Texture}
        interior {My_Bottle_Glass_Int}
}
//object {Bottle_Glass}

#declare Sphere_Water = 
sphere {
    My_Focus, 1
    material {My_Lava_Material}
}            

sky_sphere {
    pigment {
        gradient y
        color_map {
            [0 color CornflowerBlue]
            [1 color MidnightBlue]
        }
        scale 10
        translate -3.65
    }
}
*/
