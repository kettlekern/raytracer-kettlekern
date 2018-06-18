I used simplex noise to create fog, either globally or contained within an object. 
I wanted to do fog because I thought it would be interesting to look at and easy to animate. 
I also thought it would be versatile in that I could use objects to contain the fog and move it around the scene to make more interesting scenes


When I first wrote my parser it was a total mess. This was partially because I was unfamiliar with parsing files in c++, and partially because it seemed like the boring thing I had to do in order to write the cool graphics code.
I was very rigid with my first implementation of parsing, expecting consistent spacing and order to object definitions. 
Then I decided to stop getting a ton of parsing bugs every new project by writing a tokenizer class. While I later found out that I could have done this more easily by using c++ standard library functions, I still think moving to some sort of tokenizer was an improvement. 
The main feature I needed to add was delimiters. Once I got delimiters working in a general case, I was able to add lots of previously hard to parse characters to my list of delimiters and quickly and easily parse files that would have taken a lot more special cases from my previous implementation. 
This functionality can be found in my tokenizer class's addSplit function.
I also think I had good design throughout the rest of my project, as I never had to significantly change my architecture or code structure to add new features. Fragment.cpp is an overloaded file for sure, but I don't think I have that problem in any other file and the overload in fragment.cpp is manageable.

TODO:
Add BRDF shading with fog
fix fog cloud bugs
Fix boxes refraction
	
	Fix small memory leak
		
	Implement spacial data structure
	
	Implement Global Illumination
	
	
	

	