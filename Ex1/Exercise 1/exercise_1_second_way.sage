import enum

class Orientation(enum.Enum):
	COLINEAR = 0
	CLOCKWISE = 1
	COUNTER_CLOCKWISE = 2

def orientation(p0,p1,p2):
    # Adding 3rd dimension in order to do cross product
    p0_vec = vector(p0 + tuple([0]))
    p1_vec = vector(p1 + tuple([0]))
    p2_vec = vector(p2 + tuple([0]))

    v1 = p1_vec - p0_vec
    v2 = p2_vec - p0_vec

    orient_num = v1.cross_product(v2)
    if orient_num == 0:
    	return Orientation.COLINEAR
    elif orient_num > 0:
    	return Orientation.COUNTER_CLOCKWISE
    else:
    	return Orientation.CLOCKWISE

#Calculate area of triangle
def area(x1, y1, x2, y2, x3, y3): 
    
    return abs( (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0 ) 
  
  
# Check if point is inside the triangle
def isInside(x1, y1, x2, y2, x3, y3, x, y): 
  
    #ABC 
    ABC = area (x1, y1, x2, y2, x3, y3) 
  
    #PBC  
    PBC = area (x, y, x2, y2, x3, y3) 
      
    # Calculate area of triangle PAC  
    PAC = area (x1, y1, x, y, x3, y3) 
      
    #PAB  
    PAB = area (x1, y1, x2, y2, x, y) 
      
    if(ABC == PBC + PAC + PAB): 
        return True
    else: 
        return False
  
#            (B)O
#               |
#               |
#               |
# -----------(P)0---------
#               |
#               |
#       (A)O    |   (C)O

p1 = input('Give first triangle point in the form (x,y): ')
p2 = input('Give second triangle point in the form (x,y): ')
p3 = input('Give third triangle point in the form (x,y): ')


if orientation(p1,p2,p3) == Orientation.COLINEAR:
	print '\nThese 3 points does not form a triangle'
else:
    if (isInside(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], 0, 0)): 
        print('\nPoint (0,0) lies inside the triangle.') 
    else: 
        print('\nPoint (0,0) does not lie inside the triangle.')