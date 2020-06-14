
# This file was *autogenerated* from the file exercise_1.sage
from sage.all_cmdline import *   # import sage library

_sage_const_2 = Integer(2); _sage_const_1 = Integer(1); _sage_const_0 = Integer(0)
import enum

class Orientation(enum.Enum):
	COLINEAR = _sage_const_0 
	CLOCKWISE = _sage_const_1 
	COUNTER_CLOCKWISE = _sage_const_2 

def orientation(p0,p1,p2):
    # Adding 3rd dimension in order to do cross product
    p0_vec = vector(p0 + tuple([_sage_const_0 ]))
    p1_vec = vector(p1 + tuple([_sage_const_0 ]))
    p2_vec = vector(p2 + tuple([_sage_const_0 ]))

    v1 = p1_vec - p0_vec
    v2 = p2_vec - p0_vec

    orient_num = v1.cross_product(v2)
    if orient_num == _sage_const_0 :
    	return Orientation.COLINEAR
    elif orient_num > _sage_const_0 :
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

	solutions = solve([l1*p1[_sage_const_0 ] + l2*p2[_sage_const_0 ] + l3*p3[_sage_const_0 ] == _sage_const_0 , 
			l1*p1[_sage_const_1 ] + l2*p2[_sage_const_1 ] + l3*p3[_sage_const_1 ] == _sage_const_0 ,
			l1 + l2 + l3 == _sage_const_1 , l1 > _sage_const_0 , l2 > _sage_const_0 , l3 > _sage_const_0 ], l1,l2,l3)

	if len(solutions) > _sage_const_0 :
		print '\nPoint (0,0) lies inside the triangle.'
	else:
		print '\nPoint (0,0) does not lie inside the triangle.'

