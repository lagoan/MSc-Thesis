// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "woods.inc"

#declare display_particles_1 = 1; 
#declare s_r = 4;
#declare Old_Particle_Material =
material {
        texture {
                pigment {color <0.25, 0.75, 0.25>}
                finish {
                        specular 0.75
                        roughness 0.01
                        ambient 0.95
                        diffuse 0.95
                }
        }
}                         
#declare New_Particle_Material =
material {
        texture {
                pigment {color <0.95, 0.15, 0.15>}
                finish {
                        specular 0.75
                        roughness 0.01
                        ambient 0.95
                        diffuse 0.95
                }
        }
}                               
                                   
#declare Translate_fluid_nozzle = <0, 0, 0>;

#declare My_Fluid_Interior =
interior {
    ior 1.33
    fade_distance 50
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material_1 =
material {
        texture {
                pigment {color <0.25, 0.45, 0.25>  filter 0.5 }
                finish {
                        specular 0.75
                        roughness 0.01
                        ambient 0.95
                        diffuse 0.95
                        reflection { 0.01 }
                }
        }
        interior { My_Fluid_Interior }  
}                         

#declare My_Focus_Position = <-0, -150, 0>;
#declare My_Camera_Location = <400, 150, 800>;

light_source { <1000, 0, 10000> colour White*0.5 }
 
light_source {
        <600, 400, 800> colour White
        spotlight
        point_at My_Focus_Position
        radius 120
        falloff 140
        fade_distance 100000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}
                       
#declare My_Nozzle_Material =
material {                                     
        texture {                                     
           pigment {color <0.5, 0.5, 0.75> }
           finish {
                specular 0.1
                roughness 0.01
                ambient 0.4
                diffuse 0.4
                reflection { 0.0 }
           }
        }
        interior {
            ior 1.0
            fade_distance 500
            fade_power 1
        }
}

#declare My_Nozzle_Thickness = 20;
#declare My_Nozzle_Inner_Radius = nozzle__m_cylRadius + 5;
#declare My_Nozzle_Outer_Radius = My_Nozzle_Inner_Radius + My_Nozzle_Thickness;
#declare My_Nozzle =
union {
        difference {
        cylinder {
                nozzle__m_cylBtmCtrPos,
                nozzle__m_cylTopCtrPos,
                My_Nozzle_Outer_Radius
                open
        }
        cylinder {
                nozzle__m_cylBtmCtrPos,
                nozzle__m_cylTopCtrPos,
                My_Nozzle_Inner_Radius
                open
        }
        }
        torus {
                0.5*(My_Nozzle_Outer_Radius + My_Nozzle_Inner_Radius),
                0.5*(My_Nozzle_Outer_Radius - My_Nozzle_Inner_Radius)
                rotate <0, 0, 90>
                translate nozzle_outlet__m_trsCtrPos
        }
}
         
object { My_Nozzle material { My_Nozzle_Material } }
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
#declare Floor_Image_Scale = 1000;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y + Floor_Thickness
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, -80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
        finish {
              specular 0.001
              ambient 0.3
              diffuse 0.3
        }
}

object { Floor }               

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}
