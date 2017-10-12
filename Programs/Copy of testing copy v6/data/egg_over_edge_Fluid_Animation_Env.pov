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

#declare My_Focus_Position = <0, -50, 0>;
#declare My_Camera_Location = <600, -50, 400>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <-10000, 10000, -10000> colour White*0.1
}

light_source {
        <600, 400, 400> colour White
        spotlight
        point_at My_Focus_Position
        radius 200000
        falloff 500
        fade_distance 10000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}
          
#declare Obj_Image_Scale = 300;      
#declare My_Object_Material =
material {                                     
        texture {                                     
           pigment
           {
                image_map {jpeg "marble-lavender.jpg"}  
                rotate <0, 90, 0>
                scale <Obj_Image_Scale, Obj_Image_Scale, Obj_Image_Scale>
           }
           finish { Object_Finish }
        }
}

#declare Edge_Surface_Thickness = -5;

#declare CylinderRadius = solid_rod_1__m_cylRadius + Edge_Surface_Thickness;
#declare CylinderBtmCtrPos = solid_rod_1__m_cylBtmCtrPos;
#declare CylinderTopCtrPos = solid_rod_1__m_cylTopCtrPos;
#declare CylinderLenBtmCut = 300;
#declare CylinderLenTopCut = 300;

#declare Edge_Cylinder = 
cylinder {
        <CylinderBtmCtrPos.x, CylinderBtmCtrPos.y, CylinderBtmCtrPos.z + CylinderLenBtmCut>
        <CylinderTopCtrPos.x, CylinderTopCtrPos.y, CylinderTopCtrPos.z - CylinderLenTopCut>
        CylinderRadius
        material { My_Object_Material }
}

object { Edge_Cylinder }          

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

#declare Floor_Thickness = 0;
#declare Floor_Image_Scale = 200;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y + Floor_Thickness
   pigment {
        image_map {jpeg "marble-key-west-jade.jpg"}
        rotate <90, 0, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish { Object_Finish }
}

object { Floor }               
