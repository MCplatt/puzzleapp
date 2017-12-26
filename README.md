# puzzleapp
Personal project that is meant as a portfolio peace
Disign Doc
------------------------------------------------
=convert picture to black and white
	-TODO: prod for doing this cess picture to be more easily/accuratly scanned
=scan every (pixeltotal)/300 lines x and y axis ,
	-TODO: have gaussian distrabution so more scans are near the middle of the photo to cut down on use less areas of the photo
	=record when black tresh starts to when it stops and store in a vector
		-TODO: Find proper threshold and make so its a unversal variable
	=Find line width via triangle height function using lowerQuartile of x and y axis vectors
		-TODO: Find better avrage of the vector
=resize the photo using (pixelamount)/(linewidth) for the x and y axis
	-TODO: Find a good method for doing this, current method unreliable?
=Create a class for the pixel
	-including
			   --current position
			   		TODO:use single structure for x and y value
			   --previous position
			   --H value (distance from end)
			   --P value (path node value +1 from the previous node)
			   --T value (h+p) 
	-needs 		
			   --End node 
			   		-TODO: Hard code into it (universal var?)
			   --previous node 
			   --matrix location value
=Impliment A* algorithm
	-create open vector
		TODO: use priority cue
	-create closed vector

	-set the end node
	-set the start node
	-set start = curr
	-start while loop(while curr node != end node)
		-for loop (for each surrounding node)
				TODO:find way to easily loop through surrounding nodes
			-calculate the node values
			-add node to open 
		-put curr in closed
		-scan open for smallest T value set as curr(if two are the same use smallest H or first arbitrary)
-TODO:use the closed list to trace back the nodes from the curr node
	-make final vector
	-while (curr != start)
		-take curr's prevX/Y and find it in closed list
		-set as the curr node
		-add to final vector
-TODO:make the value of each point relative to the full sized picture
	-multiply each x and y by the scaling factor (better method needed?)
-TODO:color each point red and surrounding yellow
	for (node in final)
		-on final picture with cords from final vector make value red
		-use the for loop from algorithm to color surrounding yellow

-TODO: print the final picture

