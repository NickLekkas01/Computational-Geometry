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

p1 = input('Give first triangle point in the form (x,y): ')
p2 = input('Give second triangle point in the form (x,y): ')
p3 = input('Give third triangle point in the form (x,y): ')


if orientation(p1,p2,p3) == Orientation.COLINEAR:
	print '\nThese 3 points does not form a triangle'
else:
	l1 = var('l1')
	l2 = var('l2')
	l3 = var('l3')

	# Just checking if the point (0,0) can be written as convex
	# combination of the triangle points. If can then
	# it is inside a tringle.

	solutions = solve([l1*p1[0] + l2*p2[0] + l3*p3[0] == 0, 
			l1*p1[1] + l2*p2[1] + l3*p3[1] == 0,
			l1 + l2 + l3 == 1, l1 > 0, l2 > 0, l3 > 0], l1,l2,l3)

	if len(solutions) > 0:
		print '\nPoint (0,0) lies inside the triangle.'
	else:
		print '\nPoint (0,0) does not lie inside the triangle.'
