I used simplex noise to create fog, either globally or contained within an object. 
I wanted to do fog because I thought it would be interesting to look at and easy to animate. 
I also thought it would be versatile in that I could use objects to contain the fog and move it around the scene to make more interesting scenes

TODO:
Add BRDF shading with fog
fix fog cloud bugs
	
	Fix memory leak
		I think this is caused by creating new random samples every iteration, a solution would be a singleton class for Simplex noise

	Fix Fresnel3
	
	Implement boxes
	Implement spacial data structure
	
	Implement Global Illumination
	
	
	