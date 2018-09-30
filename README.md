# puzzleapp
Personal project that is meant as a portfolio peace
Disign Doc
------------------------------------------------
- ## Create a node class for a pixel
  - ### including
  - current position
    - TODO:use single structure for x and y value
  - previous position
  - H value (distance from end)
  - P value (path node value +1 from the previous node)
  - T value (h+p) 
  - ### Init needs 		
  - End node 
    - TODO: Hard code into it (universal var?)
  - previous node 
  - Matrix location value
			   
- ## Process picture to make easier to process
  - Convert picture to grayscale (values now range etween 0 and 254 for each pixel)
  - Multiply image by 2
  - Erode image (element 5x5, at point -1x-1)
- ## Find avg size of maze line
  - If img > 900x900
    - Scan middle 300 lines (1/3> line <2/3)
    - Record pixel size of line horizantally then vertically 
  - If img < 900x900
    - Scan 1/3 of the lines, horizontal then vertical
    - Record pixel size of line horizantally then vertically
  - Take horizontal and vertical line length avgs and find **line width** via tiangle height function
  

- ## Resize the photo using (pixelamount)/(linewidth) for the x and y axis
  - TODO: Find a good method for doing this, current method unreliable?
  

- Impliment A* algorithm
  - Create open vector
    - TODO: use priority cue
  - Create closed vector
  - set the end node
  - set the start node
  - set start = curr
  - start while loop(while curr node != end node)
  - for loop (for each surrounding node)
    - TODO:find way to easily loop through surrounding nodes
  - calculate the node values
  - add node to open 
  - put curr in closed
  - scan open for smallest T value set as curr(if two are the same use smallest H or first arbitrary)
    - TODO:use the closed list to trace back the nodes from the curr node
  - make final vector
  - while (curr != start)
  - take curr's prevX/Y and find it in closed list
  - set as the curr node
  - add to final vector
    - TODO:make the value of each point relative to the full sized picture
  - multiply each x and y by the scaling factor (better method needed?)
    - TODO:color each point red and surrounding yellow
  - for (node in final)
  - on final picture with cords from final vector make value red
  - use the for loop from algorithm to color surrounding yellow
    - TODO: print the final picture

