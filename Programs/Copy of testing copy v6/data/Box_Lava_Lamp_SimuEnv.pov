// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare My_Focus = <0.0000, 0.0000, 0.0000>;

#declare My_Camera_Location = <2.0000, 0.0000, -8.0000>;
                     
#declare Lava = color Orange filter 0.05;

#declare My_Lava_Texture =
texture {
        pigment {Lava}
    finish {                
//        phong 0.9 phong_size 100
        specular 0
        ambient 0.95
        diffuse 0.95
        conserve_energy
    }
}

#declare My_Lava_Int =
interior {
    ior 1.33
    fade_distance 2
    fade_power 2
//    fade_color <1, 1, 1>
}

#declare My_Lava_Material =
material {
    texture {My_Lava_Texture}
    interior {My_Lava_Int}
}
                                    
#declare Camera_Focus_Up = 0.55;
#declare Camera_Zoom_In = 0;
camera {
   location  <My_Camera_Location.x,
              My_Camera_Location.y+Camera_Focus_Up,
              My_Camera_Location.z+Camera_Zoom_In>
                 //<0.0, 0.0, -10.0>
//   direction <0.0, 0.0,  1.0>
//   up        <0.0, 1.0,  0.0>
   up        <0.0, 1.0,  0.0>
   right     <2/5 , 0.0,  0.0>
   look_at   <My_Focus.x-0.15, My_Focus.y+Camera_Focus_Up, My_Focus.z>
}

#declare LampGlass = texture {
   pigment {rgbf < 0.98, 1.0, 0.98, 1> }
   finish {
      specular 1
      roughness 0.001
      ambient 0
      diffuse 0
      reflection {
         0,1
         fresnel
      }         
   }
}

#declare Lamp = 
  box {
       <-1, -1, -1> <1, 1, 1>
      translate < 0, 0.125, 0>
//      scale < 1.15, 3.75, 1.15>
      scale < 1.2, 3.75, 1.2>
      pigment {rgbf 0.75}
      texture {LampGlass}
      interior {
         ior 1.5
         fade_distance 5
         fade_power 1
      }
   }

#declare LampTopThickness = 0.02;
#declare LampTopHeight = 0.3;
#declare LampTopWidth = 1.2;
#declare LampTopTranslateY = 4.45;
#declare LampTopTop =
   box { <-1, -1, -1> <1, 1, 1>
      scale <LampTopWidth, LampTopThickness, LampTopWidth>
      translate <0, LampTopTranslateY+LampTopHeight, 0>
      texture {Bright_Bronze}
   }
#declare LampTopFront =
   box { <-1, -1, -1> <1, 1, 1>
      scale <LampTopWidth, LampTopHeight, LampTopThickness>
      translate <0, LampTopTranslateY, -LampTopWidth>
      texture {Bright_Bronze}
   }
#declare LampTopBack =
   box { <-1, -1, -1> <1, 1, 1>
      scale <LampTopWidth, LampTopHeight, LampTopThickness>
      translate <0, LampTopTranslateY, LampTopWidth>
      texture {Bright_Bronze}
   }
#declare LampTopLeft =
   box { <-1, -1, -1> <1, 1, 1>
      scale <LampTopThickness, LampTopHeight, LampTopWidth>
      translate <-LampTopWidth, LampTopTranslateY, 0>
      texture {Bright_Bronze}
   }
#declare LampTopRight =
   box { <-1, -1, -1> <1, 1, 1>
      scale <LampTopThickness, LampTopHeight, LampTopWidth>
      translate <LampTopWidth, LampTopTranslateY, 0>
      texture {Bright_Bronze}
   }
      
#declare LampBaseHeight = 0.5;
#declare LampBaseTranslateY = -3.4;
#declare LampBase =
   box {
      <-1, -1, -1> <1, 1, 1>
      scale <1.25, LampBaseHeight, 1.25>
      translate <0, LampBaseTranslateY, 0>
      texture {Bright_Bronze}
//      no_shadow
   }

#declare Sphere_Lava = 
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
plane { y, -3.65
   pigment { color NeonPink*1.2}
}
                                             
#declare top_light_magnitude = 0.5;                                             
#declare base_light_magnitude = 10;                                             
//light_source { <100.0, 20.0, 20.0> colour White}
//light_source { <0.0, 200.0, -200.0> colour Gray05 shadowless}
light_source { <0.0, 4.75, 0.0> colour top_light_magnitude}
light_source { <0.0, -2, 0.0> colour base_light_magnitude}

object {Lamp}
object {LampBase}  
//object {LampTopTop}
object {LampTopFront}
object {LampTopBack}
object {LampTopLeft} 
object {LampTopRight}
//object {LampTop}
//object {Sphere_Lava translate <0, -3, 0>}                                      

/*
#declare LampBaseFront =
   superellipsoid {
      < 0.05, 0.05>
      scale <1.21, LampBaseHeight, 0.02>
      translate <0, Lampbasetranslatey, -1.2>
      texture {Bright_Bronze}
   }
object {LampBaseFront}
object {LampBaseBack}
object {LampBaseLeft} 
object {LampBaseRight}
*/

/* Background */    
/*
plane {
   z, 10

   texture {
      pigment {
        color rgb <0.95 , 0.25, 0.25>
      }
   }
}
*/

