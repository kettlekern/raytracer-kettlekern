camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}


sphere { <0, 0, 0>, 5
  pigment { color rgbf <1.0, 1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.0}
  fog 0.2
}

plane {<0, 1, 0>, -6
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}
