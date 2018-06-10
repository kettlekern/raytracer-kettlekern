// depth-of-field test
// Author: Richard Yang


//default camera pointing down negative z
camera {
location  <0, 0, 14>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

//Center
sphere { <0, 0, 0>, 2
  pigment { color rgb <0.1, 0.3, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.3 roughness 0.25}
}

//Left
sphere { <-2, 0, 10>, 1
  pigment { color rgb <1.0, 0.2, 0.7>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.1}
}

//Right
sphere { <5, 0, -10>, 3
  pigment { color rgb <0.2, 0.8, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.1}
}

plane {<0, 1, 0>, -5
      pigment {color rgb <0.2, 0.6, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}
