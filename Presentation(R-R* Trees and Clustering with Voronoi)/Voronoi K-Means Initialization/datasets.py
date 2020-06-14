from random import randint, uniform

def five_blobs():
    points = [[uniform(-20,-10),uniform(5,20)] for i in range(0,200)]
    points += [[uniform(-20,-15),uniform(-20,-15)] for i in range(0,40)]
    points += [[uniform(0,5),uniform(0,5)] for i in range(0,10)]
    points += [[uniform(10,20),uniform(-20,-10)] for i in range(0,200)]
    points += [[uniform(10,20),uniform(15,20)] for i in range(0,20)]

    return points

def r15():
    file = open('r15.csv', 'r')
    lines = file.readlines()

    file.close

    return [[float(l.split(',')[0]), float(l.split(',')[1])] for l in lines]
