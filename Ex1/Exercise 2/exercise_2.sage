x = var('x')
y = var('y')

r = input('Give the radius of the circle to find the lattice points: ')

assume(x, 'integer')
assume(y, 'integer')

res_1st_way = len(solve([x^2 + y^2 == r^2],x,y))

#Count Lattice Second way
def countLattice(r): 
    
    result = 0 
    
    # Every possible x 
    for x in range(0, r):
        
        # Find a potential y 
        yy = r*r - x*x  
        y = int(math.sqrt(yy))  
  
        #check if value is in the circle
        # if yes add 4 
        if (y*y + x*x) == r*r:
            result += 4 
       
    return result 

if countLattice(r) != res_1st_way:
	print 'Some algorithm is wrong'
else:
	print '\nCircle with center the point (0,0) and radius ' + str(r) +\
		' has ' + str(res_1st_way) +\
		' lattice points'
 
