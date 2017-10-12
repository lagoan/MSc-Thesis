// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"

#declare display_particles_1 = 0; 
#declare display_particles_2 = 0;
#declare display_particles_3 = 0;
#declare display_particles_4 = 0;
#declare display_particles_5 = 0;
#declare display_particles_6 = 0;
                                   
#declare Translate_eggwhite = <0, 0, 0>;
#declare Translate_yolk0 = <0, 0, 0>;
                                   
#declare Object_Finish =
finish {
      specular 0.05
      ambient 0.25
      diffuse 0.25
}

#declare Egg_Finish =
finish {
        specular 0.5
        roughness 0.01
        ambient 0.5
        diffuse 0.5
        reflection { 0.001 }
}

#declare Yolk_Finish =
finish {
        specular 0.995
        roughness 0.02
        ambient 0.995
        diffuse 0.995
        reflection { 0.05 }
}

#declare Egg_White_Color = <0.8, 0.8, 0.6>;
#declare Egg_White_Texture =
texture {                                     
   pigment {color Egg_White_Color  filter 0.95 }
   finish { Egg_Finish }
}

#declare Yolk_Color = <0.75, 0.5, 0.1>;
#declare Yolk_Texture =
texture {                                     
   pigment {color Yolk_Color  filter 0.5 }
   finish { Yolk_Finish }
}

#declare Egg_White_Interior =
interior {
    ior 1.01
    fade_distance 500
    fade_power 1
    fade_color Egg_White_Color
}

#declare Yolk_Interior =
interior {
    ior 1.33
    fade_distance 5
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material_1 =
material {
        texture { Egg_White_Texture  }
        interior { Egg_White_Interior  }  
}

#declare My_Fluid_Material_2 =
material {
        texture { Yolk_Texture  }
        interior { Yolk_Interior  }  
}

#declare Dislocation_X = 0;
#declare My_Focus_Position = <Dislocation_X, -50, 0>;
#declare My_Camera_Location = <Dislocation_X, 450, 600>;
              
camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <600, 800, 400> colour White
        spotlight
        point_at My_Focus_Position
        radius 1000
        falloff 500
        fade_distance 10000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
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

#declare Object_Finish =
finish {
      specular 0.01
      ambient 0.25
      diffuse 0.25
}
 
#declare Obj_Image_Scale = 300;      
#declare My_Object_Material =
material {                                     
        texture {                                     
           pigment
           {
                image_map {jpeg "Wood-Light-birch.jpg"}  
                rotate <90, 0, 0>
                scale <Obj_Image_Scale, Obj_Image_Scale, Obj_Image_Scale>
           }
           finish { Object_Finish }
        }
}
                              
/* bowl */                           
#declare Bowl_Thickness = 10;
#declare Bowl_Height = bowl__m_bbHeight - 50;
#declare Bowl =
difference {
        sphere { bowl__m_ovalCtrPos, bowl__m_bbBigRadius+Bowl_Thickness }
        sphere { bowl__m_ovalCtrPos, bowl__m_bbBigRadius }
        cylinder { <bowl__m_ovalCtrPos.x, bowl__m_ovalCtrPos.y-(bowl__m_bbBigRadius-Bowl_Height), bowl__m_ovalCtrPos.z>,
                   <bowl__m_ovalCtrPos.x, bowl__m_ovalCtrPos.y+bowl__m_bbBigRadius+Bowl_Thickness, bowl__m_ovalCtrPos.z>,
                   bowl__m_bbBigRadius+Bowl_Thickness }     
        material { My_Object_Material }
        }

object { Bowl }
                              
/* Floor */            
#declare FloorThickness = 0;
#declare Floor_Image_Scale = 500;      
#declare Floor =
plane {
   y, bowl__m_bbBtmPos.y + FloorThickness
   pigment {
        image_map {jpeg "marble-key-west-jade.jpg"}  
        rotate <90, 0, 0>
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
        rotate <0, 45, 0>
   }
}

object { Floor }               
