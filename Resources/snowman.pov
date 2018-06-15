camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}


sphere { <0, 0, 0>, 6.1
  pigment { color rgbf <1.0, 1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.0}
  fog 0.18
}

sphere { <0, 5, 0>, 1
  pigment { color rgb <.89, 0.93, 0.92>}
  finish {ambient 0.2 diffuse 0.4 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
}

sphere { <0, 2, 0>, 2
  pigment { color rgbf <.89, 0.93, 0.92, .75>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.33}
}

sphere { <0, -3, 0>, 3
  pigment { color rgb <.89, 0.93, 0.92>}
  finish {ambient 0.4 diffuse 0.8}
}

plane {<0, 1, 0>, -7
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}
