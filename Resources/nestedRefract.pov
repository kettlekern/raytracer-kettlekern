camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}


sphere { <0, 0, 0>, 4.5
  pigment { color rgbf <1.0, 1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.1}
  fog 0.2
}

sphere { <0, 0, 0>, 2.6
  pigment { color rgbf <1.0, 0.8, 0.7, 0.75>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.33}
}

sphere { <2.8, 1, -9>, 3.1
  pigment { color rgb <0.38, 0.69, 0.40>}
  finish {ambient 0.4 diffuse 0.8}
}

plane {<0, 1, 0>, -6
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}
