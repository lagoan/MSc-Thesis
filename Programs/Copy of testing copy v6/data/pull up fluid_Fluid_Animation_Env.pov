// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "woods.inc"
                
#declare display_particles_1 = 0; 

#declare Translate_pull_up_fluid = <0, 0, 0>;
 
#declare Fluid_Finish =
finish {
        specular 0.5
        roughness 0.01
        ambient 0.5
        diffuse 0.5
        reflection { 0.001 }
}

#declare My_Object_Material =
material {                                     
   texture {
        pigment { color <2, 1, 0> }
        finish {
              specular 0.5
              ambient 0.5
              diffuse 0.5
        }
   }
}

#declare My_Fluid_Texture =
texture {                                     
   pigment {color <0.25, 0.45, 0.25>  filter 0.995 }
   finish {
      specular 0.75
      roughness 0.001
      ambient 0.95
      diffuse 0.95
      reflection {
         0.01
      }
   }
}

#declare My_Fluid_Interior =
interior {
    ior 1.33
    fade_distance 50
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material_1 =
material {
        texture {  }
        interior {  }  
}

#declare My_Focus_Position = <0, 150, 0>;
#declare My_Camera_Location = <-550, 100, 350>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <0, 0, 1000> colour White*0.75
}

light_source {
        <-600, 600, -200> colour White*0.9
        spotlight
        point_at My_Focus_Position
        radius 120
        falloff 150
        fade_distance 10000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}
        
#declare Moving_Cylinder_Wall_Thickness = 10;                       
#declare Moving_Cylinder_End_Thickness = 10;                       

#declare Moving_Cylinder_1 = 
cylinder {
        <solid_rod_1__m_cylTopCtrPos.x,
         solid_rod_1__m_cylTopCtrPos.y + Moving_Cylinder_End_Thickness,
         solid_rod_1__m_cylTopCtrPos.z>
        <solid_rod_1__m_cylBtmCtrPos.x,
         solid_rod_1__m_cylBtmCtrPos.y,
         solid_rod_1__m_cylBtmCtrPos.z>   
        solid_rod_1__m_cylRadius + Moving_Cylinder_Wall_Thickness
}

object {
        Moving_Cylinder_1
        material { My_Object_Material }
}

/* Sky */
sky_sphere {
    pigment {
        gradient y
        color_map {
            [0 color White]//rgb <0.05, 0.05, 0.05>]
            [1 color Blue]
        }
        scale 2
        translate -1
    }
}

/* Floor */
#declare Floor_Thickness = -5;
#declare Floor_Image_Scale = 200;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y + Floor_Thickness
   pigment {
        image_map {jpeg "jade_green.jpg"}
        rotate <90, 0, 0>       
        rotate <0, 180, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish {
         specular 0.05
         ambient 0.25
         diffuse 0.25
   }
}

object { Floor }               
