// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare My_Focus = <0.0000, 1.0, 0.0000>;

#declare My_Camera_Location = <0.0, 1.0, -7.0>;
                     
#declare Water = color White filter 0.95;

#declare My_Water_Texture =
texture {
        pigment {Water}
    finish {                
//        phong 0.9 phong_size 100
        specular 0.75
        ambient 0.75
        diffuse 0.75
        conserve_energy
    }
}

#declare My_Water_Int =
interior {
    ior 1.05
    fade_distance 40
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Lava_Material =
material {
    texture {My_Water_Texture}
    interior {My_Water_Int}
}
                                    
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
    
/* Floor */
plane { y, -50
   pigment { color NeonPink*1.2}
}
                                             
#declare light_magnitude = 0.5;                                             
light_source { <0.0, 0.0, 10.0> colour light_magnitude}
light_source { <0.0, 0.0, -10.0> colour light_magnitude}
// two side lights
light_source { <100.0, 0.0, 10.0> colour light_magnitude}
light_source { <-100.0, 0.0, 10.0> colour light_magnitude}

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

